#!/usr/bin/env bash
# Original script by Daniel Lemire <lemire@gmail.com>
# Parallel version by Jeff Epler <jepler@gmail.com>

SCRIPTDIR="$(cd "$(dirname "$0")" && pwd)"
export BUILDDIR="${BUILDDIR:-$SCRIPTDIR/../build/testu01}"

if [ ! -d "$BUILDDIR" ]; then
    echo "Build directory $BUILDDIR not found."
    echo "Run: cmake -B build && cmake --build build"
    exit 1
fi

. "$SCRIPTDIR/testlist.sh"
printf "$SCRIPTDIR/bigcrush.sh %s\\0" "${commands[@]}" | xargs -0 -n1 -P`getconf _NPROCESSORS_ONLN` sh -c
