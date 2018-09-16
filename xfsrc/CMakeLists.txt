cmake_minimum_required(VERSION 3.0.0)
project(xf VERSION 0.1.0)

#include(CTest)
#enable_testing()

#TODO: OGG - make Unix conditional and Debug as well
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( BITNESS 64 )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( BITNESS 32 )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# TODO: add the apld library for WIN+DEBUG and dll for WIN
if(WIN32)
	set(APL_LIB "C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc")
else()
	set(APL_LIB ${CMAKE_SOURCE_DIR}/../xflib/apl.a)
endif()

if(WIN32)
	add_definitions(-DxxBIT=${BITNESS} -DHAS_UNICODE=1 -D__STDC_WANT_DEC_FP__=0)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -fno-strict-aliasing -fPIC -fvisibility=hidden -fno-common -U_FORTIFY_SOURCE -fno-stack-protector")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m${BITNESS} -mwindows -Xlinker -Map=example.map")
	link_directories(${APL_LIB} "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/um/x64")
else()
	add_definitions(-DxxBIT=${BITNESS} -DHAS_UNICODE=1 -DUNIX=1 -D__STDC_WANT_DEC_FP__=0)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -fno-strict-aliasing -fPIC -fvisibility=hidden -fno-common -U_FORTIFY_SOURCE -fno-stack-protector")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m${BITNESS} -Wl,-Bstatic -ltinfo -Wl,-Bdynamic -lm -lpthread -ldl -lrt -Xlinker -Map=example.map -Xlinker --cref -Xlinker --gc-sections")
	link_directories(${CMAKE_SOURCE_DIR}/../xflib)
endif()



add_executable(example 
			example.c
			apl.h
			apllimits.h
			apltypes.h
			d_machin.h
			eltypes.h
			gfns.h
			io_maps.h
			machine.h
			ourdirent.h
			win/port.h
			linux/port.h
			support.h
			trans.h
			unique_fid.h
			windowstypes.h
		)
target_include_directories(example PRIVATE .)
target_compile_definitions(example PRIVATE AP_BUILD=1)
target_compile_options(example PRIVATE -MMD -MF example.d -MP)
if(WIN32)
# try also libapl
	#target_link_libraries(example apl user32 advapi32 shlwapi gdi32 BufferOverflowU)
	target_link_libraries(example apl)
else()
	target_link_libraries(example ${APL_LIB})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Find gshandler.obj!!   https://github.com/maandree/gprolog/blob/master/src/W32GUICons/w32_console.c
# Look into https://sourceforge.net/p/mingw/news/2017/02/mingwrt-50-and-w32api-50-snapshots-available/
# Which compiler do the Dyalog use? If it's C++, then it's teh end of story due to incompatible ABI!!!
# The -lapl thing results in "Warning: corrupt .drectve at end of def file"
# gcc -o aa.exe ./CMakeFiles/example.dir/example.c.obj -mwindows -L "C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc" -lapl -L "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/um/x64" -lBufferOverflowU -lshlwapi

# https://support.microsoft.com/en-us/help/894573/you-may-receive-the-linker-tools-error-lnk2001-error-messages-when-you
# -lruntmchk  Removes the GShandler errors but brings some  others 
# gcc -o aa.exe ./CMakeFiles/example.dir/example.c.obj -mwindows -L "C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc" -lapl -L "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/um/x64" -lBufferOverflowU -lshlwapi -L "C:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/lib/amd64" -lruntmchk 

# gcc -o aa.exe ./CMakeFiles/example.dir/example.c.obj -mwindows -L "C:/Program Files/Dyalog/Dyalog APL-64 17.0 Unicode/xfsrc" -lapl -L "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/um/x64" -lBufferOverflowU -lshlwapi -L "C:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/lib/amd64" -lruntmchk 

## It looks like a dead end with the AP because it requires static linking with the apl.lib. 
## The apl.lib uses VS toolchain and MS libraries. It seems impossible to link it using MINGW. 

### TRY another approach with the dynamic library instead (NA in APL).

## Ways to go now:
## 1) accept the fact that AP will be built with the different compilers on Linux/Windows
## 2) or  use the Principia via NA on both platforms:
##	a) test NA on windows: VS amd MINGW
##	b) test NA on Linux: compiled with GNU.