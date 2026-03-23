#!/usr/bin/env bash
SCRIPTDIR="$(cd "$(dirname "$0")" && pwd)"
export BUILDDIR="${BUILDDIR:-$SCRIPTDIR/../build/testu01}"

if [ ! -d "$BUILDDIR" ]; then
    echo "Build directory $BUILDDIR not found."
    echo "Run: cmake -B build && cmake --build build"
    exit 1
fi

. "$SCRIPTDIR/testlist.sh"
for t in "${commands[@]}"; do
   $SCRIPTDIR/bigcrush.sh $t
done # for t in "${commands[@]}"; do
