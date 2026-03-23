#!/usr/bin/env bash
# Original script by Daniel Lemire <lemire@gmail.com>
# Parallel version by Jeff Epler <jepler@gmail.com>

SCRIPTDIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPTDIR"

. ./testlist.sh
printf "./bigcrush.sh %s\\0" "${commands[@]}" | xargs -0 -n1 -P`getconf _NPROCESSORS_ONLN` sh -c
