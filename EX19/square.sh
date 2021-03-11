#!/bin/bash 
#filename square 
int=1 
while [ $int -le 5 ] 
do 
    sq=`expr $int \* $int` 
    echo $sq 
    int=`expr $int + 1` 
done 
echo "Completed!"

