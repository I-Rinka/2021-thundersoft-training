#!/bin/bash
check_process()
{
    ps -aux | grep $1 | grep -v grep
}

file=`cat ./check_procs.conf`

if (( $? != 0 ))
then
    echo "config file open error" >/dev/stderr
    exit 1
fi

for line in $file
do
    if [[ ${line:0:1} == "#" ]]
    then
        continue
    fi

    ans=$(check_process $line)

    if (( $? == 0 ))
    then
    #进程还活着，对ans进行字符串处理
        echo $line $ans
    else
        echo $line DOWN
    fi

done