#!/bin/bash
openssl genrsa -out arm-pri.ky 1024
openssl rsa -in arm-pri.ky -out arm-pub.ky -pubout
