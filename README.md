# SoWx
Coin GUI binding for wxWidgets

## Platform supported

Thanks to the powerful porting support provided by wxWidgets SoWx has been successfully compile on:

 - Linux
   - GTK binding (preferred)
   - X11 binding
   - Qt binding
 - Windows
 - Mac OS (coming soon)

## Linux compilation

Required libs are:

 - wxWidgets (version >= 3.2)
 - Coin (version >= 4.0)
 - boost (version >= 1.48 only for test )

For other details please refer to [INSTALL](./INSTALL) file.

## Windows compilation

Building SoWx on Windows is done by using the CMake build configuration.

Check out the detailed build instructions in the [INSTALL](./INSTALL) file.

***The Autotools build system will be not maintained.***


## cpack 

cpack can generate configuration for predefined platform.
cpack needs to be enabled in cmake setting SOWX_USE_CPACK to ON
e.g.:

    cmake -DSOWX_USE_CPACK=ON ...

Packages for different platform can be generated using specific platforms 
(e.g. cpack --config cmake-build-debug/cpack.d/centos.cmake).


## Windows address sanitizer
Add 
	"addressSanitizerEnabled": true,
in CMakeSettings.json

## Known limitations

    