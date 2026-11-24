#!/usr/bin/env bash

KERNEL_NAME="$(uname -s)"
PROJECT_DIR="$(dirname $(readlink -f ${0}))"
EXEC_NAME="$(cat ${PROJECT_DIR}/exec.name)"

TEST_SUITE="$(dirname ${PROJECT_DIR})/writing-a-c-compiler-tests"

function help () {
    echo "Usage: ./test-suite.sh <compiler> [test_options ...]"
    echo ""
    echo "<compiler>          compiler executable passed to test_compiler"
    echo "[test_options ...]  optional list of arguments passed to test_compiler"
    echo ""
    echo "see ${TEST_SUITE}/test_compiler"
    exit 0
}

function em () {
    echo "\033[1m‘${1}’\033[0m"
}

function log_warning () {
    WARN_MESSAGE="${1}"
    echo -e "\033[1;34mwarning:\033[0m ${WARN_MESSAGE}"
}

function raise_error () {
    ERROR_MESSAGE="${1}"
    echo -e "test-suite: \033[0;31merror:\033[0m ${ERROR_MESSAGE}" 1>&2
    exit 1
}

if [[ "${KERNEL_NAME}" == "Darwin"* ]]; then
    log_warning "run $(em "arch -x86_64 zsh") first to start Rosetta 2 on aarch64 MacOS"
elif [[ "${KERNEL_NAME}" == "FreeBSD"* ]]; then
    raise_error "the test suite does not support FreeBSD, try to selfhost the compiler instead!"
fi

function reset_exec () {
    cd ${PROJECT_DIR}
    echo ""
    bash ${PROJECT_DIR}/set-exec.sh ${EXEC_NAME}
    return ${?}
}

function run_tests () {
    echo ""
    echo "----------------------------------------------------------------------"
    echo "(${TEST_EXEC}) ${@}"
    ./test_compiler ${PROJECT_DIR}/driver.sh ${@}
    if [ ${?} -ne 0 ]; then reset_exec; exit 1; fi
    return 0
}

if [ "${1}" = "--help" ]; then
    help
elif [ ${#} -lt 1 ]; then
    help
fi

bash ${PROJECT_DIR}/set-exec.sh ${1}
if [ ${?} -ne 0 ]; then exit 1; fi
TEST_EXEC="$(cat ${PROJECT_DIR}/exec.name)"

cd ${TEST_SUITE}
./test_compiler --check-setup
if [ ${?} -ne 0 ]; then reset_exec; exit 1; fi

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
