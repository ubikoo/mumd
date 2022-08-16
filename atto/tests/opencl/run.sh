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
# Run examples
#
execute() {
    pushd "${1}"
    run make -f ../Makefile clean
    run make -f ../Makefile -j48 all
    run ./test.out
    run make -f ../Makefile clean
    popd
}

execute 0-helloworld
execute 1-platform
execute 2-context
execute 3-program
execute 4-vector
execute 5-arithmetic
execute 5-matrix
execute 6-image-float
animate /tmp/out_0*
execute 6-image-float4
animate /tmp/out_0*
execute 6-image-texture
animate /tmp/out_0*
execute 6-image-texture-gl
