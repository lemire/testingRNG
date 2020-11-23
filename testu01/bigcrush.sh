#!/usr/bin/env bash
make -s
if [ $# -eq 0 ]; then
    echo "No arguments provided"
    exit 1
fi
wc="-b" #big crush
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
     thiscommand=$t" "$SEEDCMD" "$order" "$wc 
     echo $thiscommand
     wf=$(echo $thiscommand | sed 's/ //g')
     filelog=$wf.log
     echo "# RUNNING" $thiscommand  "Outputting result to " $filelog
    ./$thiscommand  > $filelog
    CMDRESULT=$?
    if [ $CMDRESULT == 0 ]; then 
      grep -s "All tests were passed" $filelog > /dev/null
      RESULT=$?
      if [ $RESULT == 0 ]; then
       echo -e "${GREEN}Success!${NC}"
      else
       echo -e "${RED}Failure!${NC}"
      fi
    else
       echo -e "${RED} command $thiscommand returned error code $CMDRESULT, log file $filelog is suspect${NC} "
    fi
  echo
  echo
 done # for order in  "-z" "-r" "" ; do
done # for SEEDCMD in "${seeds[@]}"; do
