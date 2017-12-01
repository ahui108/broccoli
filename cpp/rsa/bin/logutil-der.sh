#!/bin/sh

if [ "$1" = "en" ]
then
    #logutil.bin.x86 -encrypt -inkey armpub.ky -in INFO -out INFO.en 
    openssl rsautl -encrypt -keyform der -in INFO -out INFO.en -inkey armpub.der 
elif [ "$1" = "de" ]
then
:<<!    
    logutil.bin.x86 -decrypt -inkey arm.ky -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
    [ -d ARMLOG  ] && rm -rf ARMLOG
    mkdir ARMLOG
    tar xvf ARMLog.tbz -C ./ARMLOG
    logutil.bin.x86 -decrypt -inkey armdg.ky -in ARMLOG/diags.log -out ARMLOG/diags.tbz
    mkdir -p ARMLOG/diags_log
    tar xvf ARMLOG/diags.tbz -C ./ARMLOG/diags_log

    rm ARMLog.tbz
    rm ARMLOG/diags.log
    rm ARMLOG/diags.tbz
!
    #dd if=ARMLog.INFO.tar.bz2 of=ARMLog.h bs=128 count=1
    openssl rsautl -decrypt -keyform der -in ARMLog.hd -out ARMLog.tbz -inkey arm.der 
    
fi
