#!/bin/bash 

disk=$mmZdf / |awk '/dev/{gsub(/[0-9]+/, "", $1);print $1}')
echo $diskmZ

echo 'KERNEL="$disk",MODE="0666"' >
