#!/usr/bin/env bash
# Original script by Daniel Lemire <lemire@gmail.com>
# Parallel version by Jeff Epler <jepler@gmail.com>

make -s
parent=$(dirname "$0")

. "$parent/testlist.sh"
printf "$parent/bigcrush.sh %s\\0" "${commands[@]}" | xargs -0 -n1 -P`getconf _NPROCESSORS_ONLN` sh -c
