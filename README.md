# red-o-lator

Emulator for GCN GPUs

## Configuration
 
### ICD-Loader
Project does not provide bundled OpenCL ICD loader so you must install it on your own.

Example of such loader is https://github.com/khronosgroup/opencl-icd-loader

Then you should create .icd file in /etc/OpenCL/vendors with path to driver library.

TODO: make it automated and cross-platform.

### CLRX
1. Install CLRX from https://github.com/CLRX/CLRX-mirror
2. Set environment variable PKG_CONFIG_PATH to include *.pc files from this installation
     (will be found in CLRX `make install` logs)
    
### GPU config
Set environment variable `RED_O_LATOR_DEVICE_CONFIG_PATH` with device config path.

Example of such config can be found in `driver/resources/rx-570.ini`.

### CLion

#### clang-format

Project uses google-style clang-format.

To make use of it, you need to install ClangFormatIJ plugin and change format options to clang-format at the right
bottom corner.