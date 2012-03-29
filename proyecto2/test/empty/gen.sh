#!/usr/bin/env bash

for j in {0..9}
do
        (mkdir -p "dir$j" && cd "dir$j" && {
                for i in {0..9}
                do
                        mkdir -p "dir$i" && touch "dir$i/$RANDOM"
                done
        })
done
