#!/bin/sh

#simply add the command you use to run your parser
if [ $# -eq 0 ]
then
	./scanner_parser "scan"
elif [ $# -eq 1 ]
then
	./scanner_parser < $1 "scan"
fi
