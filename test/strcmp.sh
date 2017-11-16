#!/bin/bash

hexdump -e '16/1 "%02X ""\n"' str.txt > /tmp/str1.txt
#str1=`hexdump -e '16/1 "%02X ""\n"' str.txt`
str1=`cat /tmp/str1.txt`
echo str1={$str1}
len1=${#str1}
echo str1.len=$len1

echo "$str1" > /tmp/str2.txt
str1=`cat /tmp/str2.txt`
echo str1={$str1}
len1=${#str1}
echo str1.len=$len1

echo "--------------------"
str2="61 20 62 20 0A "
echo str2={$str2}
len2=${#str2}
echo str2.len=$len2

if [ "${str1}" = "${str2}" ]; then
    echo "equal"
else
    echo "not equal"
fi
