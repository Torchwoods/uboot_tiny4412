#!/bin/sh

make distclean
make tiny4412_config
make -j4

make -C sd_fuse

