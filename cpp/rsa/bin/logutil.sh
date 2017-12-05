#!/bin/sh

if [ "$1" = "en" ]
then
    logutil.bin.x86 -encrypt -inkey armpub.ky -in ARMLog.tbz -out ARMLog.en 
elif [ "$1" = "de" ]
then
    logutil.bin.x86 -decrypt -inkey arm.ky -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
    [ -d ARMLOG  ] && rm -rf ARMLOG
    mkdir ARMLOG
    tar xvf ARMLog.tbz -C ./ARMLOG
    logutil.bin.x86 -decrypt -inkey armdg.ky -in ARMLOG/diags.log -out ARMLOG/diags.tbz
    mkdir -p ARMLOG/diags_log
    tar xvf ARMLOG/diags.tbz -C ./ARMLOG/diags_log

    #rm ARMLog.tbz
    rm ARMLOG/diags.log
    rm ARMLOG/diags.tbz
fi