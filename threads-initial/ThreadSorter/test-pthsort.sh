#! /bin/bash

if ! [[ -x pthsort ]]; then
    echo "pthsort executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


