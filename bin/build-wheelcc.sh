#!/usr/bin/env bash

PROJECT_DIR="$(dirname $(readlink -f ${0}))"
WHEELCC_DIR="$(dirname ${PROJECT_DIR})/wheelcc"

function em () {
    echo "\033[1m‘${1}’\033[0m"
}

function raise_error () {
    ERROR_MESSAGE="${1}"
    echo -e "build-wheelcc: \033[0;31merror:\033[0m ${ERROR_MESSAGE}" 1>&2
    exit 1
}

function build_wheelcc () {
    echo "-- Build wheelcc for bootstrapping ..."
    cd ${WHEELCC_DIR}/build
    if [ ${?} -ne 0 ]; then return 1; fi
    echo -n "wheelcc" > ${WHEELCC_DIR}/bin/pkgname.cfg
    if [ ${?} -ne 0 ]; then return 1; fi
    bash build.sh
    if [ ${?} -ne 0 ]; then return 1; fi
}

build_wheelcc
if [ ${?} -ne 0 ]; then
    raise_error "failed to build $(em "wheelcc"), see https://github.com/romainducrocq/wheelcc#install"
fi

exit 0
