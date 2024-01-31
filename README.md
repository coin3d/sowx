# SoWx
Coin GUI binding for wxWidgets

[![Continuous Integration Build](https://github.com/coin3d/sowx/actions/workflows/continuous-integration-workflow.yml/badge.svg)](https://github.com/coin3d/sowx/actions/workflows/continuous-integration-workflow.yml)
[![Code Scanning - Action](https://github.com/coin3d/sowx/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/coin3d/sowx/actions/workflows/codeql-analysis.yml)

## Platform supported

Thanks to the powerful porting support provided by wxWidgets SoWx compile and run on:

- Linux
  - GTK binding (preferred)
  - X11 binding
  - Qt binding
- Windows 11
	- Visual Studio 2022
 	- msys2 

MacOS is in progress.

## Open Inventor SGI support

SoWx support also SGI Open Inventor.
SGI Open Inventor can be found in Ubuntu release (e.g. 22.04).
Install as:
	sudo apt-get install inventor-clients inventor-data inventor-demo inventor-dev
Enable Open Inventor in compilation with:
    cmake -DSOWX_USE_OIV=ON ...

## Linux compilation

Required libs are:

- wxWidgets (version >= 3.1)
- Coin (version >= 4.0)
- boost (version >= 1.48 only for test )

For other details please refer to [INSTALL](./INSTALL) file.


Note on cmake find_package on Linux systems

find_package uses a FindwxWidgets.cmake facility.
On Unix like systems requires wx-config, so be sure that wx-config will be in the PATH.


## Windows compilation

Building SoWx on Windows requires cmake.

Check out the detailed build instructions in the [INSTALL](./INSTALL) file.

***The Autotools build system will be not maintained.***

## cpack 

cpack can generate configuration for predefined platform.
cpack needs to be enabled in cmake setting SOWX_USE_CPACK to ON
e.g.:

    cmake -DSOWX_USE_CPACK=ON ...

Packages for different platform can be generated using specific platforms 
(e.g. cpack --config cmake-build-debug/cpack.d/centos.cmake).


## Address sanitizer
Added specific option on cmake
    
    cmake -DSOWX_SANITIZE_ADDRESS=ON ...

## Known limitations

### Off Screen Rendering

Off Screen rendering is not working on Coin and this impacts on SoWxMaterialEditor.

### SGI Open Inventor

SoWxMaterialEditor and SoWxColorEditor do not work with SGI Open Inventor


### X11 binding on wxWidgets
    
X11 does not provide checkbuttons. This impact on ExaminerViewer.

### Windows 

On Windows there is a memory leak when process exits.
Cursor are not fully working yet.
Test compilation requires static lib, turn off SOWX_BUILD_SHARED_LIBS

