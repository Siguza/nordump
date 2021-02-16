// 0xfffffe0008bf9ff0 0x00000002 0xffffffff 0x00000000 0x00000000 AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORWrite(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)
// 0xfffffe0008bfa2c0 0x00000002 0xffffffff 0x00000000 0x00000000 AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORFlash(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)
// 0xfffffe0008bfa594 0x00000002 0x00000000 0x00000000 0xffffffff AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)
// 0xfffffe0008bfa838 0x00000002 0x00000000 0x00000000 0x00000000 AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORErase(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)
// 0xfffffe0008bfa930 0x00000000 0x00000000 0x00000000 0x00000000 AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORObliterate(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)
// 0xfffffe0008bfa9b0 0x00000002 0x00000000 0x00000000 0x00000000 AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORTest(AppleEmbeddedSimpleSPINORFlasherDriver*, void*, IOExternalMethodArguments*)

static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *driver, void *refCon, IOExternalMethodArguments *args)
{
    // 0xfffffe0008bfa594      7f2303d5       pacibsp
    // 0xfffffe0008bfa598      ffc301d1       sub sp, sp, 0x70
    // 0xfffffe0008bfa59c      fa6702a9       stp x26, x25, [sp, 0x20]
    // 0xfffffe0008bfa5a0      f85f03a9       stp x24, x23, [sp, 0x30]
    // 0xfffffe0008bfa5a4      f65704a9       stp x22, x21, [sp, 0x40]
    // 0xfffffe0008bfa5a8      f44f05a9       stp x20, x19, [sp, 0x50]
    // 0xfffffe0008bfa5ac      fd7b06a9       stp x29, x30, [sp, 0x60]
    // 0xfffffe0008bfa5b0      fd830191       add x29, sp, 0x60
    // 0xfffffe0008bfa5b4      f80302aa       mov x24, x2
    // 0xfffffe0008bfa5b8      f40301aa       mov x20, x1
    // 0xfffffe0008bfa5bc      f50300aa       mov x21, x0
    // 0xfffffe0008bfa5c0      e00400b4       cbz x0, 0xfffffe0008bfa65c
    // 0xfffffe0008bfa5c4      d80400b4       cbz x24, 0xfffffe0008bfa65c
    if(!driver || !args)
    {
        // 0xfffffe0008bfa65c      53588052       mov w19, 0x2c2
        // 0xfffffe0008bfa660      1300bc72       movk w19, 0xe000, lsl 16

        // 0xfffffe0008bfa664      088201d0       adrp x8, sym._kmod_info_54
        // 0xfffffe0008bfa668      08010491       add x8, x8, 0x100
        // 0xfffffe0008bfa66c      080140b9       ldr w8, [x8]
        // 0xfffffe0008bfa670      a80a0034       cbz w8, 0xfffffe0008bfa7c4
        if(gDebugEnabled)
        {
            // 0xfffffe0008bfa674      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa678      00341891       add x0, x0, 0x60d
            // 0xfffffe0008bfa67c      9f010094       bl sym.IOLog_39
            IOLog("SPINORFlasherDriver::");

            // 0xfffffe0008bfa680      a841fff0       adrp x8, 0xfffffe0007431000
            // 0xfffffe0008bfa684      08bd0791       add x8, x8, 0x1ef
            // 0xfffffe0008bfa688      f46301a9       stp x20, x24, [sp, 0x10]
            // 0xfffffe0008bfa68c      e85700a9       stp x8, x21, [sp]
            // 0xfffffe0008bfa690      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa694      00303e91       add x0, x0, 0xf8c
            // 0xfffffe0008bfa698      47000014       b 0xfffffe0008bfa7b4
            // 0xfffffe0008bfa7b4      51010094       bl sym.IOLog_39
            IOLog("%s: Null pointer: target = %p reference = %p arguments = %p", "static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *, void *, IOExternalMethodArguments *)", driver, refCon, args);

            // 0xfffffe0008bfa7b8      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa7bc      00c41891       add x0, x0, 0x631
            // 0xfffffe0008bfa7c0      4e010094       bl sym.IOLog_39
            IOLog("\n");
        }
        return kIOReturnBadArgument;
    }
    // 0xfffffe0008bfa5c8      081340f9       ldr x8, [x24, 0x20]
    // 0xfffffe0008bfa5cc      130140b9       ldr w19, [x8]
    uint32_t offset = args->scalarInput[0];

    // 0xfffffe0008bfa5d0      140540f9       ldr x20, [x8, 8]
    uint64_t length = args->scalarInput[1];

    // 0xfffffe0008bfa5d4      003740f9       ldr x0, [x24, 0x68]
    IOMemoryDescriptor *desc = args->structureOutputDescriptor;
    IOMemoryMap *map;
    void *buf;
    uint32_t *sizep;

    // 0xfffffe0008bfa5d8      200600b4       cbz x0, 0xfffffe0008bfa69c
    if(desc)
    {
        // 0xfffffe0008bfa5dc      100040f9       ldr x16, [x0]
        // 0xfffffe0008bfa5e0      e80300aa       mov x8, x0
        // 0xfffffe0008bfa5e4      28b4f9f2       movk x8, 0xcda1, lsl 48
        // 0xfffffe0008bfa5e8      f10310aa       mov x17, x16
        // 0xfffffe0008bfa5ec      1019c1da       autda x16, x8
        // 0xfffffe0008bfa5f0      f147c1da       xpacd x17
        // 0xfffffe0008bfa5f4      1f0211eb       cmp x16, x17
        // 0xfffffe0008bfa5f8      40000054       b.eq 0xfffffe0008bfa600
        // 0xfffffe0008bfa5fc      408e38d4       brk 0xc472
        // 0xfffffe0008bfa600      08a20891       add x8, x16, 0x228
        // 0xfffffe0008bfa604      091641f9       ldr x9, [x16, 0x228]
        // 0xfffffe0008bfa608      c8dae7f2       movk x8, 0x3ed6, lsl 48
        // 0xfffffe0008bfa60c      01008052       mov w1, 0
        // 0xfffffe0008bfa610      28093fd7       blraa x9, x8
        map = desc->map(0);

        // 0xfffffe0008bfa614      400b00b4       cbz x0, 0xfffffe0008bfa77c
        if(!map)
        {
            // 0xfffffe0008bfa77c      53588052       mov w19, 0x2c2
            // 0xfffffe0008bfa780      1300bc72       movk w19, 0xe000, lsl 16

            // 0xfffffe0008bfa784      088201d0       adrp x8, sym._kmod_info_54
            // 0xfffffe0008bfa788      08010491       add x8, x8, 0x100
            // 0xfffffe0008bfa78c      080140b9       ldr w8, [x8]
            // 0xfffffe0008bfa790      a8010034       cbz w8, 0xfffffe0008bfa7c4
            if(gDebugEnabled)
            {
                // 0xfffffe0008bfa794      a041ffd0       adrp x0, 0xfffffe0007430000
                // 0xfffffe0008bfa798      00341891       add x0, x0, 0x60d
                // 0xfffffe0008bfa79c      57010094       bl sym.IOLog_39
                IOLog("SPINORFlasherDriver::");

                // 0xfffffe0008bfa7a0      a841fff0       adrp x8, 0xfffffe0007431000
                // 0xfffffe0008bfa7a4      08bd0791       add x8, x8, 0x1ef
                // 0xfffffe0008bfa7a8      e80300f9       str x8, [sp]
                // 0xfffffe0008bfa7ac      a041fff0       adrp x0, 0xfffffe0007431000
                // 0xfffffe0008bfa7b0      00080291       add x0, x0, 0x82
                // 0xfffffe0008bfa7b4      51010094       bl sym.IOLog_39
                IOLog("%s: map is null.", "static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *, void *, IOExternalMethodArguments *)");

                // 0xfffffe0008bfa7b8      a041ffd0       adrp x0, 0xfffffe0007430000
                // 0xfffffe0008bfa7bc      00c41891       add x0, x0, 0x631
                // 0xfffffe0008bfa7c0      4e010094       bl sym.IOLog_39
                IOLog("\n");
            }
            return kIOReturnBadArgument;
        }
        // 0xfffffe0008bfa618      f60300aa       mov x22, x0
        // 0xfffffe0008bfa61c      100040f9       ldr x16, [x0]
        // 0xfffffe0008bfa620      e80300aa       mov x8, x0
        // 0xfffffe0008bfa624      28b4f9f2       movk x8, 0xcda1, lsl 48
        // 0xfffffe0008bfa628      f10310aa       mov x17, x16
        // 0xfffffe0008bfa62c      1019c1da       autda x16, x8
        // 0xfffffe0008bfa630      f147c1da       xpacd x17
        // 0xfffffe0008bfa634      1f0211eb       cmp x16, x17
        // 0xfffffe0008bfa638      40000054       b.eq 0xfffffe0008bfa640
        // 0xfffffe0008bfa63c      408e38d4       brk 0xc472
        // 0xfffffe0008bfa640      08e20491       add x8, x16, 0x138
        // 0xfffffe0008bfa644      099e40f9       ldr x9, [x16, 0x138]
        // 0xfffffe0008bfa648      c89ee6f2       movk x8, 0x34f6, lsl 48
        // 0xfffffe0008bfa64c      28093fd7       blraa x9, x8
        // 0xfffffe0008bfa650      f70300aa       mov x23, x0
        buf = map->getVirtualAddress();

        // 0xfffffe0008bfa654      08c30191       add x8, x24, 0x70
        sizep = &args->structureOutputDescriptorSize;

        // 0xfffffe0008bfa658      14000014       b 0xfffffe0008bfa6a8
    }
    else
    {
        // 0xfffffe0008bfa69c      160080d2       mov x22, 0
        map = NULL;

        // 0xfffffe0008bfa6a0      172f40f9       ldr x23, [x24, 0x58]
        buf = args->structureOutput;

        // 0xfffffe0008bfa6a4      08830191       add x8, x24, 0x60
        sizep = &args->structureOutputSize;
    }
    // 0xfffffe0008bfa6a8      180140b9       ldr w24, [x8]
    uint32_t size = *sizep;

    // 0xfffffe0008bfa6ac      198201d0       adrp x25, sym._kmod_info_54
    // 0xfffffe0008bfa6b0      39030491       add x25, x25, 0x100
    // 0xfffffe0008bfa6b4      280340b9       ldr w8, [x25]
    // 0xfffffe0008bfa6b8      1f0d0071       cmp w8, 3
    // 0xfffffe0008bfa6bc      c3010054       b.lo 0xfffffe0008bfa6f4
    if(gDebugEnabled >= 3)
    {
        // 0xfffffe0008bfa6c0      a041ffd0       adrp x0, 0xfffffe0007430000
        // 0xfffffe0008bfa6c4      00341891       add x0, x0, 0x60d
        // 0xfffffe0008bfa6c8      8c010094       bl sym.IOLog_39
        IOLog("SPINORFlasherDriver::");

        // 0xfffffe0008bfa6cc      a841fff0       adrp x8, 0xfffffe0007431000
        // 0xfffffe0008bfa6d0      08bd0791       add x8, x8, 0x1ef
        // 0xfffffe0008bfa6d4      f3d300a9       stp x19, x20, [sp, 8]
        // 0xfffffe0008bfa6d8      e80300f9       str x8, [sp]
        // 0xfffffe0008bfa6dc      a041fff0       adrp x0, 0xfffffe0007431000
        // 0xfffffe0008bfa6e0      00740191       add x0, x0, 0x5d
        // 0xfffffe0008bfa6e4      85010094       bl sym.IOLog_39
        IOLog("%s: address = 0x%08x length = 0x%08x", "static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *, void *, IOExternalMethodArguments *)", (uint64_t)offset, length);

        // 0xfffffe0008bfa6e8      a041ffd0       adrp x0, 0xfffffe0007430000
        // 0xfffffe0008bfa6ec      00c41891       add x0, x0, 0x631
        // 0xfffffe0008bfa6f0      82010094       bl sym.IOLog_39
        IOLog("\n");
    }
    // 0xfffffe0008bfa6f4      9f020071       cmp w20, 0
    // 0xfffffe0008bfa6f8      e41a40fa       ccmp x23, 0, 4, ne
    // 0xfffffe0008bfa6fc      e1010054       b.ne 0xfffffe0008bfa738
    if(!length || !buf)
    {
        // 0xfffffe0008bfa700      53588052       mov w19, 0x2c2
        // 0xfffffe0008bfa704      1300bc72       movk w19, 0xe000, lsl 16

        // 0xfffffe0008bfa708      280340b9       ldr w8, [x25]
        // 0xfffffe0008bfa70c      c8050034       cbz w8, 0xfffffe0008bfa7c4
        if(gDebugEnabled)
        {
            // 0xfffffe0008bfa710      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa714      00341891       add x0, x0, 0x60d
            // 0xfffffe0008bfa718      78010094       bl sym.IOLog_39
            IOLog("SPINORFlasherDriver::");

            // 0xfffffe0008bfa71c      a841fff0       adrp x8, 0xfffffe0007431000
            // 0xfffffe0008bfa720      08bd0791       add x8, x8, 0x1ef
            // 0xfffffe0008bfa724      f7d300a9       stp x23, x20, [sp, 8]
            // 0xfffffe0008bfa728      e80300f9       str x8, [sp]
            // 0xfffffe0008bfa72c      a041fff0       adrp x0, 0xfffffe0007431000
            // 0xfffffe0008bfa730      004c0291       add x0, x0, 0x93
            // 0xfffffe0008bfa734      20000014       b 0xfffffe0008bfa7b4
            // 0xfffffe0008bfa7b4      51010094       bl sym.IOLog_39
            IOLog("%s: Null pointer or zero-length: buffer = %p length = %u", "static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *, void *, IOExternalMethodArguments *)", buf, length);

            // 0xfffffe0008bfa7b8      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa7bc      00c41891       add x0, x0, 0x631
            // 0xfffffe0008bfa7c0      4e010094       bl sym.IOLog_39
            IOLog("\n");
        }
        return kIOReturnBadArgument;
    }
    // 0xfffffe0008bfa738      887e4092       and x8, x20, 0xffffffff
    // 0xfffffe0008bfa73c      1f0118eb       cmp x8, x24
    // 0xfffffe0008bfa740      29050054       b.ls 0xfffffe0008bfa7e4
    if((length & 0xffffffff) > size)
    {
        // 0xfffffe0008bfa744      53588052       mov w19, 0x2c2
        // 0xfffffe0008bfa748      1300bc72       movk w19, 0xe000, lsl 16

        // 0xfffffe0008bfa74c      280340b9       ldr w8, [x25]
        // 0xfffffe0008bfa750      a8030034       cbz w8, 0xfffffe0008bfa7c4
        if(gDebugEnabled)
        {
            // 0xfffffe0008bfa754      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa758      00341891       add x0, x0, 0x60d
            // 0xfffffe0008bfa75c      67010094       bl sym.IOLog_39
            IOLog("SPINORFlasherDriver::");

            // 0xfffffe0008bfa760      a841fff0       adrp x8, 0xfffffe0007431000
            // 0xfffffe0008bfa764      08bd0791       add x8, x8, 0x1ef
            // 0xfffffe0008bfa768      f4e300a9       stp x20, x24, [sp, 8]
            // 0xfffffe0008bfa76c      e80300f9       str x8, [sp]
            // 0xfffffe0008bfa770      a041fff0       adrp x0, 0xfffffe0007431000
            // 0xfffffe0008bfa774      003c0791       add x0, x0, 0x1cf
            // 0xfffffe0008bfa778      0f000014       b 0xfffffe0008bfa7b4
            // 0xfffffe0008bfa7b4      51010094       bl sym.IOLog_39
            IOLog("%s: length %u > buffer_size %lu", "static IOReturn AppleEmbeddedSimpleSPINORFlasherDriverUC::SPINORRead(AppleEmbeddedSimpleSPINORFlasherDriver *, void *, IOExternalMethodArguments *)", length, size);

            // 0xfffffe0008bfa7b8      a041ffd0       adrp x0, 0xfffffe0007430000
            // 0xfffffe0008bfa7bc      00c41891       add x0, x0, 0x631
            // 0xfffffe0008bfa7c0      4e010094       bl sym.IOLog_39
            IOLog("\n");
        }
        return kIOReturnBadArgument;
    }
    // 0xfffffe0008bfa7e4      e00315aa       mov x0, x21
    // 0xfffffe0008bfa7e8      e10313aa       mov x1, x19
    // 0xfffffe0008bfa7ec      e20314aa       mov x2, x20
    // 0xfffffe0008bfa7f0      e30317aa       mov x3, x23
    // 0xfffffe0008bfa7f4      0cfdff97       bl method AppleEmbeddedSimpleSPINORFlasherDriver::readRegion(unsigned int, unsigned int, unsigned char const*)
    // 0xfffffe0008bfa7f8      f30300aa       mov x19, x0
    IOReturn ret = driver->readRegion(offset, (uint32_t)length, buf);

    // 0xfffffe0008bfa7fc      56feffb4       cbz x22, 0xfffffe0008bfa7c4
    if(map)
    {
        // 0xfffffe0008bfa800      d00240f9       ldr x16, [x22]
        // 0xfffffe0008bfa804      e80316aa       mov x8, x22
        // 0xfffffe0008bfa808      28b4f9f2       movk x8, 0xcda1, lsl 48
        // 0xfffffe0008bfa80c      f10310aa       mov x17, x16
        // 0xfffffe0008bfa810      1019c1da       autda x16, x8
        // 0xfffffe0008bfa814      f147c1da       xpacd x17
        // 0xfffffe0008bfa818      1f0211eb       cmp x16, x17
        // 0xfffffe0008bfa81c      40000054       b.eq 0xfffffe0008bfa824
        // 0xfffffe0008bfa820      408e38d4       brk 0xc472
        // 0xfffffe0008bfa824      088e42f8       ldr x8, [x16, 0x28]!
        // 0xfffffe0008bfa828      f050e7f2       movk x16, 0x3a87, lsl 48
        // 0xfffffe0008bfa82c      e00316aa       mov x0, x22
        // 0xfffffe0008bfa830      10093fd7       blraa x8, x16
        map->release();

        // 0xfffffe0008bfa834      e4ffff17       b 0xfffffe0008bfa7c4
    }
    // 0xfffffe0008bfa7c4      e00313aa       mov x0, x19
    // 0xfffffe0008bfa7c8      fd7b46a9       ldp x29, x30, [sp, 0x60]
    // 0xfffffe0008bfa7cc      f44f45a9       ldp x20, x19, [sp, 0x50]
    // 0xfffffe0008bfa7d0      f65744a9       ldp x22, x21, [sp, 0x40]
    // 0xfffffe0008bfa7d4      f85f43a9       ldp x24, x23, [sp, 0x30]
    // 0xfffffe0008bfa7d8      fa6742a9       ldp x26, x25, [sp, 0x20]
    // 0xfffffe0008bfa7dc      ffc30191       add sp, sp, 0x70
    // 0xfffffe0008bfa7e0      ff0f5fd6       retab
    return ret;
}
