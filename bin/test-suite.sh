#!/usr/bin/env bash

PROJECT_DIR="$(dirname $(readlink -f ${0}))"
EXEC_NAME="$(cat ${PROJECT_DIR}/exec.name)"
TEST_SUITE="${PROJECT_DIR}/../writing-a-c-compiler-tests"

bash ${PROJECT_DIR}/set-exec.sh ${1}
if [ ${?} -ne 0 ]; then exit 1; fi

function reset_exec () {
    cd ${PROJECT_DIR}
    echo ""
    bash ${PROJECT_DIR}/set-exec.sh ${EXEC_NAME}
    return ${?}
}

function run_tests () {
    echo ""
    echo "----------------------------------------------------------------------"
    echo "(${EXEC_NAME}) ${@}"
    ./test_compiler ${PROJECT_DIR}/driver.sh ${@}
    if [ ${?} -ne 0 ]; then
        reset_exec
        exit 1
    fi
    return 0
}

cd ${TEST_SUITE}
if [ ${#} -gt 1 ]; then
    run_tests ${@:2}
else
    for i in $(seq 1 18); do
        run_tests --chapter ${i} --latest-only --extra-credit
    done
    for i in $(seq 19 20); do
        run_tests --chapter ${i} --extra-credit
    done
fi

reset_exec
if [ ${?} -ne 0 ]; then exit 1; fi

exit 0
