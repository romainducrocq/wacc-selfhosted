#!/usr/bin/env bash

PROJECT_DIR="$(dirname $(readlink -f ${0}))"
WHEELCC_DIR="$(dirname ${PROJECT_DIR})/wheelcc"

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
    echo -e "build: \033[0;31merror:\033[0m build \033[1m‘wheelcc’\033[0m failed, \
see https://github.com/romainducrocq/wheelcc#install" 1>&2
    exit 1
fi
exit 0
