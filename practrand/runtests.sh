make -s
MEM="512GB"
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo "Testing "$MEM " of data per run"
echo "Note: running the tests longer could expose new failures."

declare -a commands=('testmitchellmoore' 'testmersennetwister' 'testxorshift-k4' 'testxorshift-k5' 'testwidynski' 'testaesctr' 'testxorshift128plus -H' 'testxorshift128plus' 'testxoroshiro128plus -H' 'testxoroshiro128plus' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testsplitmix64 -H' 'testsplitmix64' 'testxorshift32'  'testxorshift1024star' 'testxorshift1024star -H');
for t in "${commands[@]}"; do
     wf=$(echo $t | sed 's/ //g')
     filelog=$wf.log
     echo "# RUNNING" $t  "Outputting result to " $filelog
    ./$t | ./RNG_test stdin64 -tlmax $MEM > $filelog
    grep -s "FAIL" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 1 ]; then
     echo -e "${GREEN}Success!${NC}"
    else
     echo -e "${RED}Failure!${NC}"
    fi
done
