#!/bin/bash

# Arguments
directory=$1
prefix=$2

num_counter=0

for old_filepath in "$directory"/*; do
    if [ -f "$old_filepath" ]; then
        old_filename=$(basename "$old_filepath")

        # Determine sequential number
        ((num_counter++))
        num=$(printf "%03d" "$num_counter")

        # Deal with the new filepath
        new_filename="${prefix}${num}${old_filename}"
        new_filepath="${directory}/${new_filename}"
        echo -e "$old_filename -> \n  $new_filename"
        mv $old_filepath $new_filepath
    fi

if [ "$num_counter" -eq 0 ]; then
    echo "No files to rename in directory '$directory'"
fi

done