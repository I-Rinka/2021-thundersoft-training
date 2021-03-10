#!/bin/bash
log_path="/home/sshDevice /var/log"
 
if  [ -d "${log_path}" ]; then 
echo start delete log 7 days ago...
    find "${log_path}"/* -name '*.log.gz' -mtime +240 -exec rm -rf {} ;
echo end delete log ...	
fi

