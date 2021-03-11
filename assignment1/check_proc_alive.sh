#!/bin/bash
#检查并格式化进程
check_process()
{
    ans=$(ps -aux | grep $1 | grep -v grep)

    if (($? != 0))
    then
        return -1
    else
        echo "$ans" | awk '{
        print $2
        }'
    fi
}

file=`cat ./check_procs.conf`

#检查文件打开情况
if (( $? != 0 ))
then
    echo "config file open error" >/dev/stderr
    exit 1
fi

for line in $file
do
    #判断是否有注释
    if [[ ${line:0:1} == "#" ]]
    then
        continue
    fi

    #得到awk
    ans=$(check_process $line)

    #判断是否查到进程
    if (( $? == 0 ))
    then
        echo $line: $ans

    else
        echo $line: DOWN
    fi

done