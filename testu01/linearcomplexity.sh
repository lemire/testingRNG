make -s
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
declare -a commands=('testlehmer64' 'testaesctr' 'testxoroshiro128plus -H' 'testxoroshiro128plus' 'testxorshift128plus -H' 'testxorshift128plus' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testsplitmix64 -H' 'testsplitmix64' 'testxorshift32' );
f="-l"
for order in  "-z" "-r" "" ; do
  echo "--- using bit order flag: " $order
  for t in "${commands[@]}"; do
     wf=$(echo $t | sed 's/ //g')
     filelog=$wf$f$order.log
     echo "# RUNNING" $t $f $order "Outputting result to " $filelog
    ./$t $f $order | grep . | egrep -v '^(HOST|Generator)' > $filelog
    egrep -s "p-value of test (.*) \*\*\*\*\*" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 0 ]; then
     echo -e "${RED}Failure!${NC}"
    else
     echo -e "${GREEN}Success!${NC}"
    fi
  done
  echo
  echo
done
