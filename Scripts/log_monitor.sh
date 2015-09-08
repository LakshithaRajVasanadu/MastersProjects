#!/bin/bash

#Script to monitor a log file until the search pattern is found
 
logfile="/path/to/log/file.log"
pattern="hello"

rm -f abc.out
echo -e "\nStarting Log Monitor" >> abc.out

function logmonitor() {
tail -fn0 $logfile | \
while read line ; do
  if echo "$line" | grep $patter 1>/dev/null 2>&1
  then
     echo "String found" >> abc.out
           echo "Log Monitor Ended"
     exit 127
  fi
done
}

logmonitor
