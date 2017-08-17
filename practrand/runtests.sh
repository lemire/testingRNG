make -s
MEM="2GB"
echo "Testing "$MEM " of data per run"
declare -a commands=('testxoroshiro128plus -H' 'testxoroshiro128plus' 'testpcg32' 'testpcg64' 'testsplitmix64' 'testxorshift32' );
for t in "${commands[@]}"; do
     wf=$(echo $t | sed 's/ //g')
     filelog=$wf.log
     echo "# RUNNING" $t  "Outputting result to " $filelog
    ./$t | ./RNG_test stdin64 -tlmax $MEM > $filelog
    grep -s "FAIL" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 1 ]; then
     echo "Success!"
    else
     echo "Failure!"
    fi
done
