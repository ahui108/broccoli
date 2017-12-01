#!/bin/bash
#openssl genrsa -out arm-pri.ky 1024
#openssl rsa -in arm-pri.ky -out arm-pub.ky -pubout
openssl rsa -in arm.ky -outform der -out arm.der
openssl rsa -in arm.ky -outform der -out armpub.der -pubout 
