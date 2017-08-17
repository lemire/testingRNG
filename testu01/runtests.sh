make -s
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
declare -a commands=('testxoroshiro128plus -H' 'testxoroshiro128plus' 'testxorshift128plus -H' 'testxorshift128plus' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testsplitmix64 -H' 'testsplitmix64' 'testxorshift32' );
for f in  "-s" "-c" "-b" ; do
 for order in  "-R" "-r" "" ; do
  echo "--- using bit order flag: " $order
  for t in "${commands[@]}"; do
     wf=$(echo $t | sed 's/ //g')
     filelog=$wf$f$order.log
     echo "# RUNNING" $t $f $order "Outputting result to " $filelog
    ./$t $f $order > $filelog
    grep -s "All tests were passed" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 0 ]; then
     echo -e "${GREEN}Success!${NC}"
    else
     echo -e "${RED}Failure!${NC}"
    fi
  done
  echo
  echo
 done
done
