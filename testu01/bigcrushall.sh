#!/bin/bash

make -s
parent=$(dirname "$0")

. "$parent/testlist.sh"
for t in "${commands[@]}"; do
   $parent/bigcrush.sh $t 
done # for t in "${commands[@]}"; do
