#!/usr/bin/env bash

for j in {0..9}
do
        (mkdir -p "subdir$j" && cd "subdir$j" && {
                for i in {0..9}
                do
                        mkdir -p "subdir$i" && touch "subdir$i/$RANDOM"
                done
        })
done
