make -s

declare -a commands=('testmitchellmoore' 'testmersennetwister' 'testxorshift-k4' 'testxorshift-k5' 'testwidynski' 'testaesctr' 'testxorshift128plus -H' 'testxorshift128plus' 'testxoroshiro128plus -H' 'testxoroshiro128plus' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testsplitmix64 -H' 'testsplitmix64' 'testxorshift32' );
for t in "${commands[@]}"; do
     ./$t | dd of=scratchfile bs=1024 count=1024 > out.log 2> err.log
     echo -n $t " : "
     ./ent scratchfile | grep Entropy
done
rm scratchfile
rm err.log      
rm out.log
