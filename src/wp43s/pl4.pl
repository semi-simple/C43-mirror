#! /bin/bash 

find . -name '*.c' -o -name '*.h' | xargs perl -p -i subs4.txt
