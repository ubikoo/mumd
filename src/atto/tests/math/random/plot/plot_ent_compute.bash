#! /bin/bash

#
# plot_ent_compute.sh
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

#
# Number of output runs
#
NUMRUNS=256
NTHREADS=32


# -----------------------------------------------------------------------------
# Compile the ent executable
#
ENTDIR="./ent"
TMPDIR="/tmp"
cp -Rvf $ENTDIR $TMPDIR

make -C $TMPDIR/ent/src clean all
ENT="$TMPDIR/ent/src/ent.out"


# -----------------------------------------------------------------------------
# Create the output plot files
#
#KISS32_PLOT="$TMPDIR/out.kiss32_plot"
#KISS64_PLOT="$TMPDIR/out.kiss64_plot"
#CMWC32_PLOT="$TMPDIR/out.cmwc32_plot"
#CMWC64_PLOT="$TMPDIR/out.cmwc64_plot"
#echo > $KISS32_PLOT
#echo > $KISS64_PLOT
#echo > $CMWC32_PLOT
#echo > $CMWC64_PLOT

#for ((i = 0; i < NUMRUNS; i++)); do
    #kiss32="$TMPDIR/out.kiss32.$i"
    #kiss64="$TMPDIR/out.kiss64.$i"
    #cmwc32="$TMPDIR/out.cmwc32.$i"
    #cmwc64="$TMPDIR/out.cmwc64.$i"

    #$ENT $kiss32 | awk 'NR == 7 {print $5}' >> $KISS32_PLOT  &
    #$ENT $kiss64 | awk 'NR == 7 {print $5}' >> $KISS64_PLOT  &
    #$ENT $cmwc32 | awk 'NR == 7 {print $5}' >> $CMWC32_PLOT  &
    #$ENT $cmwc64 | awk 'NR == 7 {print $5}' >> $CMWC64_PLOT  && wait

    #x0=$(tail -n 1 $KISS32_PLOT)
    #x1=$(tail -n 1 $KISS64_PLOT)
    #x2=$(tail -n 1 $CMWC32_PLOT)
    #x3=$(tail -n 1 $CMWC64_PLOT)

    #echo $i ": " $x0 $x1 $x2 $x3
#done

KISS32_PLOT="$TMPDIR/out.kiss32_plot"
KISS64_PLOT="$TMPDIR/out.kiss64_plot"
CMWC32_PLOT="$TMPDIR/out.cmwc32_plot"
CMWC64_PLOT="$TMPDIR/out.cmwc64_plot"
echo > $KISS32_PLOT
echo > $KISS64_PLOT
echo > $CMWC32_PLOT
echo > $CMWC64_PLOT

thread_fun () {
    local kiss32="$TMPDIR/out.kiss32.$1"
    local kiss64="$TMPDIR/out.kiss64.$1"
    local cmwc32="$TMPDIR/out.cmwc32.$1"
    local cmwc64="$TMPDIR/out.cmwc64.$1"

    local ix=$(($1%NTHREADS))

    $ENT $kiss32 | awk 'NR == 7 {print $5}' >> $KISS32_PLOT
    $ENT $kiss64 | awk 'NR == 7 {print $5}' >> $KISS64_PLOT
    $ENT $cmwc32 | awk 'NR == 7 {print $5}' >> $CMWC32_PLOT
    $ENT $cmwc64 | awk 'NR == 7 {print $5}' >> $CMWC64_PLOT

    x0=$(tail -n 1 $KISS32_PLOT)
    x1=$(tail -n 1 $KISS64_PLOT)
    x2=$(tail -n 1 $CMWC32_PLOT)
    x3=$(tail -n 1 $CMWC64_PLOT)

    echo $1 "/" $ix ": " $x0 $x1 $x2 $x3
}

for (( i = 0; i < $((NUMRUNS - NUMRUNS % NTHREADS)); i+=$NTHREADS )); do
    for (( j = 0; j < $((NTHREADS-1)); j++ )); do
        thread_fun $((i+j)) &
    done
    thread_fun $((i+NTHREADS-1)) && wait
done

for (( i = $((NUMRUNS - NUMRUNS % NTHREADS)); i < $NUMRUNS; i+=1 )); do
    thread_fun $((i))
done


echo "$TMPDIR/out.kiss32.long" && $ENT "$TMPDIR/out.kiss32.long"
echo "$TMPDIR/out.kiss64.long" && $ENT "$TMPDIR/out.kiss64.long"

echo "$TMPDIR/out.cmwc32.long" && $ENT "$TMPDIR/out.cmwc32.long"
echo "$TMPDIR/out.cmwc64.long" && $ENT "$TMPDIR/out.cmwc64.long"


# -----------------------------------------------------------------------------
# Clean the ent executable
#
#make -C $ENTDIR clean
