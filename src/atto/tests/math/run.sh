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
# Test geometry
pushd geometry
run make -f ../Makefile clean && \
    make -f ../Makefile -j48 all && \
    ./test.out
    make -f ../Makefile clean
popd

# -----------------------------------------------------------------------------
# Test random
pushd random
run make -f ../Makefile clean && \
    make -f ../Makefile -j48 all && \
    ./test.out
    make -f ../Makefile clean

pushd plot
[[ -x ./plot_ent_compute.bash ]] && ./plot_ent_compute.bash
popd
popd
