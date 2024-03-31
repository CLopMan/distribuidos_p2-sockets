#!/bin/bash

for i in {0..5}
do
    printf "\033[0;33mRUNNING: test_imp $i\033[0m\n";
    ./tests_imp $i
    exit_st=$?
    if [ $exit_st -eq 0 ]; then
        printf "\033[0;32m[OK]\033[0m test_imp $i\n"
    else 
        printf "\033[0;31m[FAILED]\033[0m test_imp $i\n"
    fi
done