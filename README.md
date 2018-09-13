# apl-portable-ap
Example of a portable single-source implementation of Dyalog APL Auxiliary Processor (AP) based on the source code from Dyalog examples.

There are several downsides with the stock Dyalog examples:

* cumbersome build system, which exists in two variants: one for Windows and another one for Linux/UNIX;

* differences in codebase for Linux/UNIX and Windows.

The project addresses both of the issues with the single streamlined cross-platform build system and single cross-platform codebase, which contains the bare necessary minimum for building a functionning AP.

Ensure you have installed distribution of Dyalog APL for your operating system. The instructions below use Dyalog APL v17.0.

## Linux build

Ensure you have the necessary build tools and libraries installed:

```bash

sudo apt install build-essential libtinfo-dev

```

You will also need recent version of CMake:

```bash

sudo apt install python-pip
pip install cmake
cd ~/bin
ln -s ~/.local/bin/cmake

```

Configure and build the project:

```bash

cd <apl-portable-ap clone directory>
# copy Dyalog APL library
cp -a /opt/mdyalog/17.0/64/unicode/xflib .
mkdir build
cd build
cmake ../xfsrc
make

```

The newly built Auxiliary Processor (AP) binary `example` will be in the `build` directory.

Test the AP in an APL session (the example is inspired by [Auxilliary Processors](http://help.dyalog.com/17.0/Content/UserGuide/Installation%20and%20Configuration/Auxiliary%20Processors.htm):

```apl

⍝ start the AP
'<the-full-path>/build/example' ⎕CMD ''
)fns ⍝ should show DATE_TO_IDN IDN_TO_DATE 

⍝ invoke an AP's function
xx←IDN_TO_DATE 19407

⍝ inspect the result
)copy display
DISPLAY xx ⍝ should show 18 F 1953

```
