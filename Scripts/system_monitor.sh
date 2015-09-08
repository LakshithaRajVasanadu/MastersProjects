#!/bin/bash

#script to monitor system statistics - CPU, memory.
#Start it as background process
#Use SIGTERM to kill the process gracefully so that it completes its monitoring successfully.

function sysstat {

echo -e "-----------------------------------------"
MYTIMESTAMP=`date +%s`
echo "Timestamp:$MYTIMESTAMP"
MPSTAT=`which mpstat`
MPSTAT=$?
if [ $MPSTAT != 0 ]
then
        echo "Please install mpstat!"
else
LSCPU=`which lscpu`
LSCPU=$?
if [ $LSCPU != 0 ]
then
        RESULT=$RESULT" lscpu required to producre acqurate reults"
else
cpus=`lscpu | grep -e "^CPU(s):" | cut -f2 -d: | awk '{print $1}'`
i=0
while [ $i -lt $cpus ]
do
        CPULOAD=`mpstat -P ALL | awk -v var=$i '{ if ($3 == var ) print $4 }' `
        echo "CPU$i:$CPULOAD"
        let i=$i+1
done
fi
LOADAVG=`uptime | awk -F'load average:' '{ print $2 }' | cut -f1 -d,`
echo -e "Load Average:$LOADAVG"
fi

TOTALMEM=`free -m | head -2 | tail -1| awk '{print $2}'`
TOTALBC=`echo "scale=2;if($TOTALMEM<1024 && $TOTALMEM > 0) print 0;$TOTALMEM/1024"| bc -l`
USEDMEM=`free -m | head -2 | tail -1| awk '{print $3}'`
USEDBC=`echo "scale=2;if($USEDMEM<1024 && $USEDMEM > 0) print 0;$USEDMEM/1024"|bc -l`
FREEMEM=`free -m | head -2 | tail -1| awk '{print $4}'`
FREEBC=`echo "scale=2;if($FREEMEM<1024 && $FREEMEM > 0) print 0;$FREEMEM/1024"|bc -l`
TOTALSWAP=`free -m | tail -1| awk '{print $2}'`
TOTALSBC=`echo "scale=2;if($TOTALSWAP<1024 && $TOTALSWAP > 0) print 0;$TOTALSWAP/1024"| bc -l`
USEDSWAP=`free -m | tail -1| awk '{print $3}'`
USEDSBC=`echo "scale=2;if($USEDSWAP<1024 && $USEDSWAP > 0) print 0;$USEDSWAP/1024"|bc -l`
FREESWAP=`free -m |  tail -1| awk '{print $4}'`
FREESBC=`echo "scale=2;if($FREESWAP<1024 && $FREESWAP > 0) print 0;$FREESWAP/1024"|bc -l`

echo "Physical Mem Total in GB:$TOTALBC"
echo "Physical Mem Used in GB:$USEDBC"
echo "Physical Mem Free in GB:$FREEBC"
echo "Physical Mem Free in %:$(($FREESWAP * 100 / $TOTALSWAP  ))"

echo "Swap Mem Total in GB:$TOTALSBC"
echo "Swap Mem Used in GB:$USEDSBC"
echo "Swap Mem Free in GB:$FREESBC"
echo "Swap Mem Free in %:$(($FREESWAP * 100 / $TOTALSWAP  ))"

echo "$MYTIMESTAMP,$CPULOAD,$LOADAVG,$TOTALBC,$USEDBC,$FREEBC,$(($FREESWAP * 100 / $TOTALSWAP  )),$TOTALSBC,$USEDSBC,$FREESBC,$(($FREESWAP * 100 / $TOTALSWAP  ))" >> mymonitor.csv

}

rm -f mymonitor.out
rm -f mymonitor.csv

echo "Timestamp,CPU,Load Average,Phy Mem Total in GB,Phy Mem Used in GB,Phy Mem Free in GB,Phy Mem Free in %,Swap Mem Total in GB,Swap Mem Used in GB, Swap Mem Free in GB, Swap Mem Free in %" >> mymonitor.csv

START="true"
echo -e "\nSystem Monitoring Started" >> mymonitor.out

trap "START='false'" SIGINT SIGTERM

while [ "$START" == true ]
do
  sysstat >> mymonitor.out
  sleep 10
done

echo -e "\nSystem Monitoring finished" >> mymonitor.out
