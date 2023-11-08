#include <errno.h>
#include <fcntl.h>              // open
#include <stdio.h>              // fprintf, stderr
#include <stdlib.h>             // free, malloc, strtoull
#include <string.h>             // strerror
#include <strings.h>            // bzero
#include <unistd.h>             // close, write
#include <sys/stat.h>           // mkdir
#include <mach/mach.h>
#include <CoreFoundation/CoreFoundation.h>

#define LOG(str, ...) do { fprintf(stderr, str "\n", ##__VA_ARGS__); } while(0)

typedef mach_port_t io_object_t;
typedef io_object_t io_registry_entry_t;
typedef io_object_t io_service_t;
typedef io_object_t io_connect_t;
extern const mach_port_t kIOMasterPortDefault;
CFMutableDictionaryRef IOServiceMatching(const char *name) CF_RETURNS_RETAINED;
io_service_t IOServiceGetMatchingService(mach_port_t masterPort, CFDictionaryRef matching CF_RELEASES_ARGUMENT);
kern_return_t IORegistryEntryGetProperty(io_registry_entry_t entry, const char *propertyName, void *buffer, uint32_t *size);
kern_return_t IOObjectRelease(io_object_t object);
kern_return_t IOServiceOpen(io_service_t service, task_port_t owningTask, uint32_t type, io_connect_t *client);
kern_return_t IOServiceClose(io_connect_t client);
kern_return_t IOConnectCallMethod(io_connect_t client, uint32_t selector, const uint64_t *in, uint32_t inCnt, const void *inStruct, size_t inStructCnt, uint64_t *out, uint32_t *outCnt, void *outStruct, size_t *outStructCnt);

int main(int argc, const char **argv)
{
    int retval = -1;
    kern_return_t ret = 0;
    const char *outfile = NULL;
    int fd = -1;
    int dirfd = -1;
    uint64_t ranges[8][2] = {};
    size_t numranges = 0;
    void *mem = NULL;
    io_service_t firmware = MACH_PORT_NULL;
    io_service_t service = MACH_PORT_NULL;
    io_connect_t client = MACH_PORT_NULL;

    if(argc == 4)
    {
        char *end = NULL;
        ranges[0][0] = strtoull(argv[1], &end, 0);
        if(argv[1][0] == '\0' || *end != '\0')
        {
            LOG("Bad number: %s", argv[1]);
            goto out;
        }

        end = NULL;
        ranges[0][1] = strtoull(argv[2], &end, 0);
        if(argv[2][0] == '\0' || *end != '\0')
        {
            LOG("Bad number: %s", argv[2]);
            goto out;
        }

        outfile = argv[3];
        numranges = 1;
    }
    else if(argc == 2)
    {
        CFMutableDictionaryRef matching = CFDictionaryCreateMutable( kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        if(!matching)
        {
            LOG("Failed to allocate CFDict");
            goto out;
        }
        CFDictionarySetValue(matching, CFSTR("IONameMatch"), CFSTR("firmware"));
        CFDictionarySetValue(matching, CFSTR("IOProviderClass"), CFSTR("AppleARMNORFlashDevice"));
        firmware = IOServiceGetMatchingService(kIOMasterPortDefault, matching);
        if(!MACH_PORT_VALID(firmware))
        {
            LOG("firmware: %x", firmware);
            goto out;
        }

        uint32_t buffer[8*2] = {};
        uint32_t size = sizeof(buffer);
        ret = IORegistryEntryGetProperty(firmware, "reg", buffer, &size);
        if(ret != KERN_SUCCESS || size % 8 != 0)
        {
            LOG("reg: 0x%x, %s", size, mach_error_string(ret));
            goto out;
        }

        numranges = size / 8;
        for(size_t i = 0; i < numranges; ++i)
        {
            ranges[i][0] = buffer[2*i];
            ranges[i][1] = buffer[2*i + 1];
        }

        IOObjectRelease(firmware);
        firmware = MACH_PORT_NULL;

        dirfd = open(argv[1], O_RDONLY | O_DIRECTORY);
        if(dirfd == -1)
        {
            if(mkdir(argv[1], 0755) != 0)
            {
                LOG("mkdir: %s", strerror(errno));
                goto out;
            }
            dirfd = open(argv[1], O_RDONLY | O_DIRECTORY);
            if(dirfd == -1)
            {
                LOG("open: %s", strerror(errno));
                goto out;
            }
        }
    }
    else
    {
        LOG("Usage: %s dir", argv[0]);
        LOG("Usage: %s off len file", argv[0]);
        goto out;
    }

    uint64_t largest = 0;
    for(size_t i = 0; i < numranges; ++i)
    {
        uint64_t len = ranges[i][1];
        if(len > largest)
        {
            largest = len;
        }
    }

    mem = malloc(largest);
    if(!mem)
    {
        LOG("malloc: %s", strerror(errno));
        goto out;
    }
    bzero(mem, largest);

    service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleEmbeddedSimpleSPINORFlasherDriver"));
    if(!MACH_PORT_VALID(service))
    {
        LOG("service: %x", service);
        goto out;
    }

    ret = IOServiceOpen(service, mach_task_self(), 69, &client);
    if(ret != KERN_SUCCESS || !MACH_PORT_VALID(client))
    {
        LOG("client: %x, %s", client, mach_error_string(ret));
        goto out;
    }

    for(size_t i = 0; i < numranges; ++i)
    {
        uint64_t len = ranges[i][1];
        size_t size = len;
        ret = IOConnectCallMethod(client, 2, ranges[i], 2, NULL, 0, NULL, NULL, mem, &size);
        if(ret != KERN_SUCCESS)
        {
            LOG("SPINORRead: %s", mach_error_string(ret));
            goto out;
        }

        if(len != size)
        {
            LOG("wtf, 0x%llx != 0x%lx", len, size);
            goto out;
        }

        if(outfile)
        {
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        else
        {
            char namebuf[32];
            snprintf(namebuf, sizeof(namebuf), "nor-%zu", i + 1);
            fd = openat(dirfd, namebuf, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if(fd == -1)
        {
            LOG("open: %s", strerror(errno));
            goto out;
        }

        ssize_t written = write(fd, mem, size);
        if((size_t)written != size)
        {
            LOG("write: %s", strerror(errno));
            goto out;
        }

        close(fd);
        fd = -1;
    }

    retval = 0;
out:;
    if(fd != -1) close(fd);
    if(dirfd != -1) close(dirfd);
    if(MACH_PORT_VALID(client)) IOServiceClose(client);
    if(MACH_PORT_VALID(service)) IOObjectRelease(service);
    if(MACH_PORT_VALID(firmware)) IOObjectRelease(firmware);
    if(mem) free(mem);

    return retval;
}
