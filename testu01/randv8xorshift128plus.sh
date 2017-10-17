#!/bin/bash
make -s
wc="-b" #big crush
t="./testv8xorshift128plus" # this is the command

i="0"

while [ $i -lt 100 ] 
do
    SEEDCMD="-S "$RANDOM
     order="-z"
     thiscommand=$t" "$SEEDCMD" "$order" "$wc 
     echo $thiscommand
     wf=$(echo $thiscommand | sed 's/ //g')
     filelog=$wf.log
     echo "# test $i out of 100 "
     echo "# RUNNING" $thiscommand  "Outputting result to " $filelog
    ./$thiscommand  > ./longresults/$filelog
    echo
    i=$[$i+1]
done 
