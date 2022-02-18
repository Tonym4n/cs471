#!/bin/sh

#simply add the command you use to run your parser
if [ $# -eq 0 ]
then
	./scanner_parser "parse"
elif [ $# -eq 1 ]
then
	./scanner_parser < $1 "parse"
fi
