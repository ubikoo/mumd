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

execute 0-gl
execute 1-glfw
execute 2-graphics
execute 3-triangle
execute 4-triangle-instanced-a
execute 5-triangle-instanced-b
execute 6-triangle-instanced-c
execute 7-quad
execute 8-quad-rotate
execute 9-quad-image
execute 10-sphere-image
execute 11-assimp-bunny
execute 12-panorama-mesh
execute 13-framebuffer
execute 14-iobuffer

#
# Check image output
#
FILES="
/tmp/out.img1_color-wheel-red.png
/tmp/out.img2_color-wheel-reda.png
/tmp/out.img3_color-wheel-blue.png
/tmp/out.img4_color-wheel-bluea.png
/tmp/out.img10_color-wheel-80x80-rgba_p3.ppm
/tmp/out.img10_color-wheel-80x80-rgba_p6.ppm
/tmp/out.img1_color-wheel-red_p3.ppm
/tmp/out.img1_color-wheel-red_p6.ppm
/tmp/out.img2_color-wheel-reda_p3.ppm
/tmp/out.img2_color-wheel-reda_p6.ppm
/tmp/out.img3_color-wheel-blue_p3.ppm
/tmp/out.img3_color-wheel-blue_p6.ppm
/tmp/out.img4_color-wheel-bluea_p3.ppm
/tmp/out.img4_color-wheel-bluea_p6.ppm
/tmp/out.img6_color-wheel-80x80-rgb_p3.ppm
/tmp/out.img6_color-wheel-80x80-rgb_p6.ppm
/tmp/out.img7_color-wheel-80x80-rgb_p3.ppm
/tmp/out.img7_color-wheel-80x80-rgb_p6.ppm
/tmp/out.img8_color-wheel-80x80-rgb_p3.ppm
/tmp/out.img8_color-wheel-80x80-rgb_p6.ppm
/tmp/out.img9_color-wheel-80x80-rgb_p3.ppm
/tmp/out.img9_color-wheel-80x80-rgb_p6.ppm
/tmp/out.img9_color-wheel-80x80-rgba_p3.ppm
/tmp/out.img9_color-wheel-80x80-rgba_p6.ppm"
for f in $FILES; do
    [[ ! -f "$f" ]] && echo "file $f does not exist" || open "$f"
done
