#!/bin/bash

make -s
parent=$(dirname "$0")

declare -a commands=('testsplitmix64 -H' 'testsplitmix64' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testxoroshiro128plus -H' 'testxoroshiro128plus' 'testxorshift128plus -H' 'testlehmer64' 'testaesctr' 'testxorshift128plus' 'testxorshift32' );
for t in "${commands[@]}"; do
   $parent/bigcrush.sh $t 
done # for t in "${commands[@]}"; do
