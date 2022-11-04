#!/usr/bin/env bash

#
# configure.sh
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
    [[ $code -ne 0 ]] && die "$@: failed with error code $code"
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
# Git clone function
git-clone() {
	local GITURL="${1}"
	local DIRECTORY="${2}"

	run install -d -m 755 "${DIRECTORY}"
	run git clone --depth 1 "${GITURL}" "${DIRECTORY}"
}

# Git subtree function
git-subtree() {
	local PREFIX="${1}"
	local GITURL="${2}"
	local BRANCH="${3-master}"
    local BASE="$(basename ${1})"

    run git config alias.add-"${BASE}" \
    "!f(){ git subtree add  --squash --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"; }; f"
    run git config alias.pull-"${BASE}" \
    "!f(){ git subtree pull --squash --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"; }; f"
    run git config alias.push-"${BASE}" \
    "!f(){ git subtree push          --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"; }; f"

    echo
    echo To add subtree "${GITURL}" at prefix "${PREFIX}" run:
    echo git subtree add  --squash --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"
    echo git subtree pull --squash --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"
    echo git subtree push          --prefix="${PREFIX}" "${GITURL}" "${BRANCH}"
    echo
}

# -----------------------------------------------------------------------------
# Catch2
run install -d -m 755 3rdparty/Catch2
run wget https://github.com/catchorg/Catch2/releases/download/v2.13.3/catch.hpp \
	-O 3rdparty/Catch2/catch.hpp

# gladload
git-clone git@github.com:ubikoo/gladload.git 3rdparty/gladload

# stb
git-clone git@github.com:ubikoo-3rdparty/stb.git 3rdparty/stb

# atto
#git-subtree atto git@github.com:ubikoo/atto.git
