#!/bin/bash
echo "start check path"
if [[ ! -d /opt/codelab ]];then
    mkdir /opt/codelab
fi
cd .. && mv ctools /opt/codelab
cd /opt/codelab
if [[ -x ctools/ifile ]];then
    if [[ ! -d bin ]];then
        mkdir bin
    fi
    mv ctools/ifile bin/
else
    echo "ifile not found in ctools"
fi