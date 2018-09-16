# Windows

The ambition is to compile the AP on Windows and Linux with the same toolchain (GCC) in order to avoid possible discrepancies between platforms in results of floating point computations and other deviations.

The build of AP on Windows requires linking with Dyalog's library `apl.lib` (in Release) or `apld.lib` (in Debug), which are placed in `C:\Program Files\Dyalog\Dyalog APL-64 17.0 Unicode\xfsrc`.

MINGW/GCC (`gcc (Rev1, Built by MSYS2 project) 8.2.0`) is able to compile the `example.c` and produces `example.obj` but GCC linker is not able to link with the Dyalog libraries and produces following errors:

```text

Warning: corrupt .drectve at end of def file
d:/local/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: d:/local/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.2.0/../../../../x86_64-w64-mingw32/lib/../lib/libmingw32.a(lib64_libmingw32_a-crt0_c.o): in function `main':
C:/repo/mingw-w64-crt-git/src/mingw-w64/mingw-w64-crt/crt/crt0_c.c:18: undefined reference to `WinMain'
d:/local/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc/apl.lib(c:/obj/17.0.dss/obj/aps/win/64/unicode/winapi/dev/opt/access.obj):(.text$mn+0x31): undefined reference to `_dclass'
...
d:/local/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc/apl.lib(c:/obj/17.0.dss/obj/aps/win/64/unicode/winapi/dev/opt/w3task.obj):(.text$mn+0x103): undefined reference to `__chkstk'
...
d:/local/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc/apl.lib(c:/obj/17.0.dss/obj/libdecf/win/64/unicode/winapi/dev/opt/bid_from_int.obj):(.xdata+0x230): undefined reference to `__GSHandlerCheck'
...

```

Apparently the Dyalog libraries are built with MSVC and require a compatible version of MSVC and Windows SDK to link with.

The `__chkstk` and `__GSHandlerCheck` can be addressed with linking to `BufferOverflowU` (from Windows SDK) and to `runtmchk` (from MSVC). But the linking generates new errors.

## Possible workarounds

### Compile with GNU and link the obj files with MSVC linker

The first question is: is MINGW compiled obj file compatible with MSVC PE-COFF format?

```bash

$ objdump -f ./CMakeFiles/example.dir/example.c.obj

./CMakeFiles/example.dir/example.c.obj:     file format pe-x86-64
architecture: i386:x86-64, flags 0x00000039:
HAS_RELOC, HAS_DEBUG, HAS_SYMS, HAS_LOCALS
start address 0x0000000000000000

```

The object file of a hello world program (see <https://github.com/o7g8/cmake-reference-app>) compiled with VS2017 has the same format `pe-x86-64` and similar metadata:

```bash

objdump -f hello.obj

hello.obj:     file format pe-x86-64
architecture: i386:x86-64, flags 0x0000003d:
HAS_RELOC, HAS_LINENO, HAS_DEBUG, HAS_SYMS, HAS_LOCALS
start address 0x0000000000000000

```

Regarding `obj` file conversions into different formats see also:

* `objconv` <https://github.com/gitGNU/objconv>

* GNU `objcopy --info`

Will it be possible to debug the result?

### "Hybrid" solution: AP loads a shared library

Implement a "thin" AP using the compatible version of MSVC on Windows and GNU GCC on Linux. Move the business logic into a separate cross-platform shared library, which can be built with any toolchain.

Once we get a shared library the most logical solution is to get rid of AP at all and switch to Name Association in APL.

### Use the LLVM linker lld

According to <https://github.com/golang/go/issues/20982> `lld` can link with 

See <https://github.com/llvm-mirror/lld>

## References

* <http://www.mingw.org/wiki/MSVC_and_MinGW_DLLs>