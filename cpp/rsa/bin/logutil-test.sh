#!/bin/sh

if [ "$1" = "en" ]
then
    logutil.bin.x86 -v -encrypt -inkey armpub.ky -in INFO -out INFO.en 
    #openssl rsautl -encrypt -in hd.decrypt -out hd.encrypt -inkey armpub.ky --keyform pem -pubin
elif [ "$1" = "de" ]
then
    logutil.bin.x86 -v -decrypt -inkey arm.ky -in INFO.en -out INFO.de
    #openssl rsautl -decrypt -in hd.en -out hd.decrypt -inkey arm.ky -keyform pem -raw
fi
