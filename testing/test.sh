#!/bin/sh

#set -e
cd testing

for f in *.csv; do
	s=$(../bin/sdks < $f | diff ${f/csv/solved} -)
	if [ -n "$s" ]; then
		echo $s > ${f/csv/err\.log}
		exit 1
	fi
done
