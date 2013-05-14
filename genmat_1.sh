#!/bin/sh -

count=$1
while [ $count -gt 0 ]; do
	cat /dev/random | od -td | head -1 | tr -s "[:blank:]" | cut -d" " -f2 | sed 's/\(^-*\)\(\).*\(.\)$/\1\3\2/'
	count=`expr $count - 1`
done
