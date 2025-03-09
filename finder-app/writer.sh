#!/bin/bash

if [ $# -ne 2 ]
then
  echo "Two positional arguments required: file to be written, string to be written into the file"
	exit 1
fi

write_file=$1
write_str=$2

# Ensure that directory is created in all cases
dir_path=`dirname "$write_file"`
mkdir -p $dir_path

echo $write_str > $write_file

if [ $? -ne 0 ]
then
	echo "File could not be created"
	exit 1
fi

exit 0
