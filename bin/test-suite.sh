#!/usr/bin/env bash

PROJECT_DIR="$(dirname $(readlink -f ${0}))"
TEST_SUITE="${PROJECT_DIR}/../writing-a-c-compiler-tests"

function test () {
    echo ""
    echo "----------------------------------------------------------------------"
    echo "${@}"
    ./test_compiler ${PROJECT_DIR}/driver.sh ${@}
    if [ ${?} -ne 0 ]; then exit 1; fi
}

cd ${TEST_SUITE}

if [ ${#} -ne 0 ]; then
    test ${@}
else
    for i in $(seq 1 18); do
        test --chapter ${i} --latest-only --extra-credit
    done
    for i in $(seq 19 20); do
        test --chapter ${i} --extra-credit
    done
fi

exit 0
