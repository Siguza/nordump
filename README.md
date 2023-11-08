# nordump

CLI tool to dump SPI NOR of Apple Silicon Macs.

Needs an entitlement since macOS 13.0 beta 4 (22A5311f).

### Building

```
make
```

### Usage

<table><tr><td>
If you're on macOS 13.0 beta 3 or earlier, run `codesign -f -s - nordump` after compiling to remove the entitlement, then you can just run it.

If you're on macOS 13.0 beta 4 or later, then you need to lower the security of your machine in order to run this. To do so, run:

```
sudo nvram boot-args=amfi_get_out_of_my_way=1
```

Then boot into recovery and run:

```
bputil -a
reboot
```

This will break certain apps and certain OS functionality though, so once you're done messing with NOR, you can undo these changes with:

```
sudo nvram -d boot-args
sudo bputil -f
```
</td></tr></table>

**Simple mode**:

```
nordump [outdir]
```

Example:

```
nordump /tmp/nor
```

This just dumps everything in NOR into separate files in the folder specified on the command line.

**Manual mode**:

```
nordump [offset] [length] [filename]
```

```
nordump 0x0 0x20000 /tmp/nor-1
```

This dumps the given range of NOR into the filename specified on the command line.

### Technical info

There seem to be multiple disjoint ranges in NOR, which are exported via the `reg` property of the `firmware` node in the IOKit registry, as uint32 pairs of offset+length. For example, this:

```
% ioreg -w0 -rf -n firmware
+-o firmware@0  <class AppleARMNORFlashDevice, id 0x1000009c0, registered, matched, active, busy 0 (111 ms), retain 8>
  | {
  |   "device_type" = <"firmware">
  |   "reg" =
  |     00000000: 00 00 00 00 00 00 02 00 00 00 02 00 00 00 2C 00 00 00 2E 00 00 00 2C 00                         ..............,.......,.
  |   "IODeviceMemory" = ()
  |   "name" = <"firmware">
  |   "AAPL,phandle" = <02010000>
  |   "compatible" = <"iboot,boot">
  | }
  |
  +-o AppleEmbeddedSimpleSPINORFlasherDriver  <class AppleEmbeddedSimpleSPINORFlasherDriver, id 0x100000c23, registered, matched, active, busy 0 (5 ms), retain 5>
```

Translates into the following offset+length pairs:

```
0x00000000 0x00020000
0x00020000 0x002c0000
0x002e0000 0x002c0000
```

So far I've always seen 3 ranges, but the tool can handle up to 8. The first range has a few bytes at the start with apparent `HUFA` magic, then just a ton of `0xFF`. The third range consists entirely of `0xFF`. The second range contains LLB and ANS2 firmware images.

### License

[MIT](https://github.com/Siguza/nordump/blob/master/LICENSE).
