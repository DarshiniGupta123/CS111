#!/bin/bash 

#sort < pg98.txt | cat b - | tr A-Z a-z > c 2>> d

#cat pg98.txt | sort | grep "the" >c 2>d

cat pg98.txt | sort | wc -l >c 2>d

echo -e "\nOutput from times:"
times
