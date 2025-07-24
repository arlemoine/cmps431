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
        new_filepath="${directory}/${prefix}${num}${old_filename}"
        echo "$old_filepath -> $new_filepath"
    fi
done
