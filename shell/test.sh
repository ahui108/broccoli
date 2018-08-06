#!/bin/sh
:<<XX
ret=1.0
while true; do
    #ret=$((1/3))
    #let "ret=1/3"
    #ret=`expr 1%3`
    ret=`echo "1%3"|bc`
    #echo $ret
done
XX

#! /bin/sh 
# filename killcpu.sh
if [ $# != 1  ] ; then
      echo "USAGE: $0 <CPUs>"
        exit 1;
    fi
    for i in `seq $1`
    do
          echo -ne " 
          i=0; 
          while true
          do
              i=i+1; 
          done" | /bin/sh &&
            pid_array[$i]=$! ;
        done

        for i in "${pid_array[@]}"; do
              echo 'kill ' $i ';';
          done
