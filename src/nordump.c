#include <errno.h>
#include <fcntl.h>              // open
#include <stdio.h>              // fprintf, stderr
#include <stdlib.h>             // free, malloc, strtoull
#include <string.h>             // strerror
#include <strings.h>            // bzero
#include <unistd.h>             // close, write
#include <mach/mach.h>
#include <CoreFoundation/CoreFoundation.h>

#define LOG(str, ...) do { fprintf(stderr, str "\n", ##__VA_ARGS__); } while(0)

typedef mach_port_t io_object_t;
typedef io_object_t io_service_t;
typedef io_object_t io_connect_t;
extern const mach_port_t kIOMasterPortDefault;
CFMutableDictionaryRef IOServiceMatching(const char *name) CF_RETURNS_RETAINED;
io_service_t IOServiceGetMatchingService(mach_port_t masterPort, CFDictionaryRef matching CF_RELEASES_ARGUMENT);
kern_return_t IOObjectRelease(io_object_t object);
kern_return_t IOServiceOpen(io_service_t service, task_port_t owningTask, uint32_t type, io_connect_t *client);
kern_return_t IOServiceClose(io_connect_t client);
kern_return_t IOConnectCallMethod(io_connect_t client, uint32_t selector, const uint64_t *in, uint32_t inCnt, const void *inStruct, size_t inStructCnt, uint64_t *out, uint32_t *outCnt, void *outStruct, size_t *outStructCnt);

// firmware
// 0x000000 0x020000
// 0x020000 0x200000
// 0x220000 0x200000

int main(int argc, const char **argv)
{
    int retval = -1;
    kern_return_t ret = 0;
    int fd = -1;
    unsigned long long off = 0,
                       len = 0;
    void *mem = NULL;
    io_service_t service = MACH_PORT_NULL;
    io_connect_t client = MACH_PORT_NULL;

    if(argc != 4)
    {
        LOG("Usage: %s off len file", argv[0]);
        goto out;
    }

    char *end = NULL;
    off = strtoull(argv[1], &end, 0);
    if(argv[1][0] == '\0' || *end != '\0')
    {
        LOG("Bad number: %s", argv[1]);
        goto out;
    }

    end = NULL;
    len = strtoull(argv[2], &end, 0);
    if(argv[2][0] == '\0' || *end != '\0')
    {
        LOG("Bad number: %s", argv[2]);
        goto out;
    }

    mem = malloc(len);
    if(!mem)
    {
        LOG("malloc: %s", strerror(errno));
        goto out;
    }
    bzero(mem, len);

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

    uint64_t in[2] = { off, len };
    size_t size = len;
    ret = IOConnectCallMethod(client, 2, in, 2, NULL, 0, NULL, NULL, mem, &size);
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

    fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

    retval = 0;
out:;
    if(fd != -1) close(fd);
    if(MACH_PORT_VALID(client)) IOServiceClose(client);
    if(MACH_PORT_VALID(service)) IOObjectRelease(service);
    if(mem) free(mem);

    return retval;
}
