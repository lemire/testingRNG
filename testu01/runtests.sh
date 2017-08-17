make
declare -a commands=('testxoroshiro128plus' 'testpcg32' 'testpcg64' 'testsplitmix64' 'testxorshift32' );
for f in  "-s" "-c" "-b" ; do
  for t in "${commands[@]}"; do
     filelog=$t$f.log
     echo "# RUNNING" $t $f "Outputting result to " $filelog
    ./$t $f > $filelog
  done
  echo
  echo
done
