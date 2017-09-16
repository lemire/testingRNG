#!/bin/bash
# Normally this script is executed in the context of another script,
# with the "." command.
declare -a commands=('testsplitmix64 -H' 'testsplitmix64' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testxoroshiro128plus -H' 'testxoroshiro128plus' 'testxorshift128plus -H' 'testxorshift128plus' 'testlehmer64' 'testlehmer64 -H' 'testaesctr'  'testxorshift32' 'testmersennetwister' 'testxorshift1024star' 'testxorshift1024star -H' );

