#!/usr/bin/bash

set -euo pipefail

if ! make -j 8
then
    echo "Make failed"
    exit 1
fi
if ! ./test.sh
then
    echo "Test failed"
    exit 2
fi

ssh rs "systemctl is-active --quiet rs-dev && sudo systemctl stop rs-dev" || :

scp -r server property.so rs:dev/
if [[ $? -ne 0 ]]
then
    echo "Transfer failed"
    exit 4
fi

sleep 0.1

ssh rs "sudo systemctl start rs-dev"
if ! ssh rs "systemctl is-active --quiet rs-dev"
then
    echo "Starting the service failed"
    exit 3
fi


