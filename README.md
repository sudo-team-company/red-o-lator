# red-o-lator

Emulator for GCN GPUs

## Configuration

1. Install CLRX from https://github.com/CLRX/CLRX-mirror
2. Either:
   * set environment variable PKG_CONFIG_PATH to include *.pc files from this installation 
   (will be found in CLRX `make install` logs)
   * copy CLRX *.pc files to `./driver/pkgconfig`

### CLion
#### clang-format
Project uses google-style clang-format.

To make use of it, you need to install ClangFormatIJ plugin
and change format options to clang-format at the right bottom corner.