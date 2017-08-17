make -s
declare -a commands=('testxoroshiro128plus -H' 'testxoroshiro128plus' 'testxorshift128plus -H' 'testxorshift128plus' 'testpcg32' 'testpcg64 -H' 'testpcg64' 'testsplitmix64 -H' 'testsplitmix64' 'testxorshift32' );
f="-l"
for order in  "-R" "-r" "" ; do
  echo "--- using bit order flag: " $order
  for t in "${commands[@]}"; do
     wf=$(echo $t | sed 's/ //g')
     filelog=$wf$f$order.log
     echo "# RUNNING" $t $f $order "Outputting result to " $filelog
    ./$t $f $order | grep . | egrep -v '^(HOST|Generator)' > $filelog
    egrep -s "p-value of test (.*) \*\*\*\*\*" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 0 ]; then
     echo "Failure!"
    else
     echo "Success!"
    fi
  done
  echo
  echo
done
