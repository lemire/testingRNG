make
declare -a commands=('testxoroshiro128plus' 'testpcg32' 'testpcg64' 'testsplitmix64' 'testxorshift32' );
for f in  "-s" "-c" "-b" ; do
  echo "# using test flag " $f
  for t in "${commands[@]}"; do
     echo "#" $t
    ./$t $f;
  done
  echo
  echo
done
