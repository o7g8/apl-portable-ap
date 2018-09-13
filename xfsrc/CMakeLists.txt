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
set(APL_LIB ${CMAKE_SOURCE_DIR}/../xflib/apl.a)

add_definitions(-DxxBIT=${BITNESS} -DHAS_UNICODE=1 -DUNIX=1 -D__STDC_WANT_DEC_FP__=0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -fno-strict-aliasing -fPIC -fvisibility=hidden -fno-common -U_FORTIFY_SOURCE -fno-stack-protector")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m${BITNESS} -Wl,-Bstatic -ltinfo -Wl,-Bdynamic -lm -lpthread -ldl -lrt -Xlinker -Map=example.map -Xlinker --cref -Xlinker --gc-sections")
link_directories(${CMAKE_SOURCE_DIR}/../xflib)

add_executable(example example.c machine.h apl.h trans.h gfns.h support.h)
target_include_directories(example PRIVATE .)
target_compile_definitions(example PRIVATE AP_BUILD=1)
target_compile_options(example PRIVATE -MMD -MF example.d -MP)
target_link_libraries(example ${APL_LIB})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
