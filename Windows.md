# Windows

The ambition is to compile the AP on Windows and Linux with the same toolchain (GCC) in order to avoid possible discrepancies between platforms in results of floating point computations and other deviations.

Execute in the build directory:

```bat

D:\src\apl-portable-ap\build>cmake -G "MinGW Makefiles" ..\xfsrc
D:\src\apl-portable-ap\build>mingw32-make

```

MINGW/GCC (`gcc (Rev1, Built by MSYS2 project) 8.2.0`) is able to compile the `example.c` and produces `example.obj` but GCC linker is not able to link with the Dyalog library and produces following errors:

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

Apparently the Dyalog libraries (`apl.lib` (in Release) or `apld.lib` (in Debug), which are placed in `C:\Program Files\Dyalog\Dyalog APL-64 17.0 Unicode\xfsrc`) are built with MSVC and require a compatible version of MSVC and Windows SDK to link with.

The `__chkstk` and `__GSHandlerCheck` can be addressed with linking to `BufferOverflowU` (from Windows SDK) and to `runtmchk` (from MSVC). But the linking generates new errors.

## Possible workarounds

### Compile with GNU and link the obj files with MSVC linker

The first question is: is MINGW compiled obj file compatible with MSVC PE-COFF format? Let's have a look:

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

And finally the Dyalog's static library `apl.lib`, which we need to link to:

```bash

$ objdump -f /c/Program\ Files/Dyalog/Dyalog\ APL-64\ 17.0\ Unicode/xfsrc/apl.lib | head -n 7
In archive C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc/apl.lib:

c:/obj/17.0.dss/obj/libdecf/win/64/unicode/winapi/dev/opt/bid_binarydecimal.obj:     file format pe-x86-64
architecture: i386:x86-64, flags 0x0000003d:
HAS_RELOC, HAS_LINENO, HAS_DEBUG, HAS_SYMS, HAS_LOCALS
start address 0x0000000000000000

```

All of them have the same format `pe-x86-64`, therefore no conversion should be necessary.

Let's link the  `example.c.obj` compiled by GNU GCC with the Dyalog's `apl.lib` using VS2017 linker (in `x64 Native Tools Command Prompt for VS2017`):  

```bat

D:\src\apl-portable-ap\build>link /out:example.exe /subsystem:windows /release /machine:x64 CMakeFiles\example.dir\example.c.obj "c:\Program Files\Dyalog\Dyalog APL-64 17.0 Unicode\xfsrc\apl.lib" shlwapi.lib user32.lib gdi32.lib advapi32.lib legacy_stdio_definitions.lib

```

It produces the `example.exe` without errors!

The attempt to load the AP in APL hangs the APL session. The same happens if I build the stock Dyalog's example using stock Dyalog's scripts in VS2017 environment.

TODO:

* Find compatible MSVC environment which builds workable AP using stock Dyalog's build scripts.

* Rebuild the hybrid GNU compiler / MSVC linker solution using the compatible MSVC environment.

Question: do I need to add the `msvcrt.lib /nodefaultlib:libcmt`?

Regarding `obj` file conversions into different formats see also:

* `objconv` <https://github.com/gitGNU/objconv>

* GNU `objcopy --info`

* The necessity of `legacy_stdio_definitions.lib` in VS2015+ <https://stackoverflow.com/questions/32418766/c-unresolved-external-symbol-sprintf-and-sscanf-in-visual-studio-2015>

Will it be possible to debug the result?

### AP (compiled with MSVC) loads a shared library (compiled with any toolchain)

Implement a "thin" AP using the compatible version of MSVC on Windows and GNU GCC on Linux. Move the business logic into a separate cross-platform shared library, which can be built with any toolchain.

Once we get a shared library the most logical solution is to get rid of AP at all and switch to Name Association in APL.

See also:

* <http://gernotklingler.com/blog/creating-using-shared-libraries-different-compilers-different-operating-systems/>

* <http://www.mingw.org/wiki/createimportlibraries>

### Use the LLVM linker lld

According to <https://github.com/golang/go/issues/20982> `lld` can link MSVC object files. Maybe MSVC static libraries as well.

It's an open question if it will make any difference compared to GNU `ld`.

See <https://lld.llvm.org/windows_support.html>

## References

* <http://www.mingw.org/wiki/MSVC_and_MinGW_DLLs>