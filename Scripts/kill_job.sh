#!/bin/sh

#Script which kills a job and retries the same until job is successfully killed

echo -e "\n[INFO]Killing System Monitor..."
RETRY=5
SLEEP=5
PROCESSNAME="mymonitor.sh"
COUNTER=1
while [ $COUNTER -lt $RETRY ]; do
        repl_process=$(ps -ef | grep $PROCESSNAME | grep -v 'grep' | awk '{print $2}')
        if [ -z "$repl_process" ]
        then
            echo -e "\nmonitor process not running"
            break
        fi
            echo -e "\nKilling the following list of monitor processes $repl_process"
            kill -15 $repl_process
            let COUNTER=COUNTER+1
            sleep $SLEEP
done
if [ $COUNTER -eq $RETRY ]; then
        echo -e "\nUnable to kill monitor processes after $RETRY iterations"
        exit 1;
fi
