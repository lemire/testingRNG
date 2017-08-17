make -s
declare -a commands=('testxoroshiro128plus' 'testpcg32' 'testpcg64' 'testsplitmix64' 'testxorshift32' );
for order in  "-R" "-r" "" ; do
 for f in  "-s" "-c" "-b" ; do
  for t in "${commands[@]}"; do
     filelog=$t$f$order.log
     echo "# RUNNING" $t $f $order "Outputting result to " $filelog
    ./$t $f $order > $filelog
    RESULT=$?
    if [ $RESULT == 0 ]; then
     echo "Success!"
    else
     echo "Failure!"
    fi
  done
  echo
  echo
 done
done
