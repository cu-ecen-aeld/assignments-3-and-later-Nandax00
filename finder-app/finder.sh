#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Two positional arguments required: files directory, search string"
	exit 1
fi

if [ ! -d "$1" ]
then
	echo "$1 is not a directory"
	exit 1
fi

files_dir=$1
search_str=$2

num_of_files=`find $files_dir -type f | wc -l`
num_of_matching_lines=`grep -r $search_str $files_dir | wc -l`

echo "The number of files are $num_of_files and the number of matching lines are $num_of_matching_lines"
exit 0
