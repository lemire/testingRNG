#!/bin/bash
make -s

whichcrush=" -b " #big crush
t=$@ # this is the command
# we use four seeds including the default seed
declare -a seeds=('' '-S 987654 ' ' -S 848432 ' '  -S 412451 ')
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
for SEEDCMD in "${seeds[@]}"; do
 for order in  "-z" "-r" "" ; do
     echo "--- using bit order flag: " $order
     echo "--- using seed flag: " $SEEDCMD
     thiscommand=$t" "$SEEDCMD" "$order " "$whichcrush
     wf=$(echo $thiscommand | sed 's/ //g')
     filelog=$wf.log
     echo "# RUNNING" $thiscommand  "Outputting result to " $filelog
    ./$thiscommand  > $filelog
    grep -s "All tests were passed" $filelog > /dev/null
    RESULT=$?
    if [ $RESULT == 0 ]; then
     echo -e "${GREEN}Success!${NC}"
    else
     echo -e "${RED}Failure!${NC}"
    fi
  echo
  echo
 done # for order in  "-z" "-r" "" ; do
done # for SEEDCMD in "${seeds[@]}"; do
