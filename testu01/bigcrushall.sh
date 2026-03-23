#!/usr/bin/env bash
SCRIPTDIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPTDIR"

. ./testlist.sh
for t in "${commands[@]}"; do
   ./bigcrush.sh $t
done # for t in "${commands[@]}"; do
