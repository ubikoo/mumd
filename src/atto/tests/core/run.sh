#! /bin/bash

#
# runtest.sh
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# -----------------------------------------------------------------------------
# Die with message
die() {
    echo >&2 "$@"
    exit 1
}

# Run command and check exit code
run() {
    echo "$@" && "$@"
    code=$?
    [[ $code -ne 0 ]] && die "[$@] failed with error code $code"
    return 0
}

# Ask for input query
ask() {
    echo -n "$@ (y/n [n]): "
    local ans
    read ans
    [[ "$ans" != "y" ]] && return 1
    return 0
}

# -----------------------------------------------------------------------------
# Test core
pushd core
run make clean && make -j48 all && ./test.out < data/lorem_ipsum.txt
make clean

run diff data/lorem_ipsum_1.bin             data/lorem_ipsum_2.bin
run diff data/lorem_ipsum.txt               data/lorem_ipsum_new.txt
run diff data/comment_lorem_ipsum_csv_1.txt data/comment_lorem_ipsum_csv_2.txt
run diff data/comment_lorem_ipsum_new_1.txt data/comment_lorem_ipsum_new_2.txt

rm -v data/lorem_ipsum_1.bin \
      data/lorem_ipsum_2.bin \
      data/comment_lorem_ipsum_csv_1.txt \
      data/comment_lorem_ipsum_csv_2.txt \
      data/comment_lorem_ipsum_new_1.txt \
      data/comment_lorem_ipsum_new_2.txt \
      data/lorem_ipsum_csv.txt \
      data/lorem_ipsum_new.txt
popd


