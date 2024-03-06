#! /bin/bash

if ! [[ -x pthsv ]]; then
    echo "pthsv executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


