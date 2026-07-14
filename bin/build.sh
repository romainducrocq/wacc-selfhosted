#!/usr/bin/env bash

KERNEL_NAME="$(uname -s)"
PROJECT_DIR="$(dirname $(readlink -f ${0}))"
# EXEC_NAME="$(cat ${PROJECT_DIR}/exec.name)"
CC="gcc"
if [[ "${KERNEL_NAME}" == "Darwin"* ]]; then
    CC="clang -arch x86_64"
elif [[ "${KERNEL_NAME}" == "FreeBSD"* ]]; then
    CC="clang"
fi
CC_FLAGS="-std=c17 -Wall -Wextra -Wpedantic"
CC_FLAGS_RELEASE="-O3 -DNDEBUG -Werror -pedantic-errors -D__GCC_STDINT__"
CC_FLAGS="${CC_FLAGS} ${CC_FLAGS_RELEASE}"
LD="${CC}"

function help () {
    # TODO
    exit 0
}

function em() {
    echo "\033[1m‘${1}’\033[0m"
}

function raise_error () {
    ERROR_MESSAGE="${1}"
    echo -e "build: \033[0;31merror:\033[0m ${ERROR_MESSAGE}, see $(em "--help")" 1>&2
    exit 1
}

function build_preset () {
    case "${LD}" in
        "clang"*)
            CLANG_MAJOR_VERSION=$(clang -dumpversion | cut -d"." -f1)
            if [ ${CLANG_MAJOR_VERSION} -lt 5 ]; then
                raise_error "requires $(em "clang") >= 5.0.0"
            fi
            ;;
        "gcc")
            GCC_MAJOR_VERSION=$(gcc -dumpversion | cut -d"." -f1)
            if [ ${GCC_MAJOR_VERSION} -lt 8 ]; then
                raise_error "requires $(em "gcc") >= 8.1.0"
            elif [ ${GCC_MAJOR_VERSION} -eq 8 ]; then
                GCC_MINOR_VERSION=$(gcc -dumpfullversion | cut -d"." -f2)
                if [ ${GCC_MINOR_VERSION} -eq 0 ]; then
                    raise_error "requires $(em "gcc") >= 8.1.0"
                fi
            fi
            ;;
        *)
            return 1
    esac

    as --help > /dev/null 2>&1
    if [ ${?} -ne 0 ]; then
        raise_error "requires package $(em "binutils")"
    fi
    return 0
}

build_preset

PROJECT_NAME="${PROJECT_DIR}/wacc-selfhosted-1"
if [ -f "${PROJECT_NAME}" ]; then
    rm ${PROJECT_NAME}
    if [ ${?} -ne 0 ]; then exit 1; fi
fi

COMPILER_DIR="$(dirname ${PROJECT_DIR})/compiler"

SOURCE_FILES="${COMPILER_DIR}/lib/main.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/3rdparty/sds.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/3rdparty/stb_ds.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/ast.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/back_ast.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/back_symt.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/front_ast.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/front_symt.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/ast/interm_ast.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/backend/asm_gen.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/backend/gas_code.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/backend/registers.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/backend/stack_fix.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/backend/symt_cvt.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/errors.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/idents.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/lexer.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/parser.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/semantic.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/frontend/tac_repr.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/optimizer/optim_tac.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/optimizer/reg_alloc.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/util/fileio.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/util/str2t.c"
SOURCE_FILES="${SOURCE_FILES} ${COMPILER_DIR}/util/throw.c"

BUILD_DIR="$(dirname ${PROJECT_DIR})/build"
if [ -d "${BUILD_DIR}/" ]; then
    rm -r ${BUILD_DIR}/
    if [ ${?} -ne 0 ]; then exit 1; fi
fi
mkdir ${BUILD_DIR}/
if [ ${?} -ne 0 ]; then exit 1; fi

OBJECT_FILES=""
echo "-- Build objects ..."
for FILE in ${SOURCE_FILES}; do
    OBJECT="${BUILD_DIR}/$(basename ${FILE%.*}).o"
    OBJECT_FILES="${OBJECT_FILES} ${OBJECT}"
    echo "(${CC}) ${FILE} -> ${OBJECT}"

    # wheelcc -v -E -c ${FILE}.c
    # if [ ${?} -ne 0 ]; then exit 1; fi
    # mv -v ${FILE}.o ${OBJECT}

    ${CC} -c ${FILE} ${CC_FLAGS} -o ${OBJECT}
    if [ ${?} -ne 0 ]; then exit 1; fi
done
echo "OK"

echo "-- Linking executable ..."
echo "${BUILD_DIR}/*.o -> ${PROJECT_NAME}"
${LD} ${OBJECT_FILES} ${CC_FLAGS} -o ${PROJECT_NAME}
if [ ${?} -ne 0 ]; then exit 1; fi
echo "OK"

EXEC_NAME="$(basename ${PROJECT_NAME})"
bash ${PROJECT_DIR}/set-exec.sh ${EXEC_NAME}
if [ ${?} -ne 0 ]; then exit 1; fi

echo "-- Created target ${PROJECT_NAME}"

find ${PROJECT_DIR}/ -maxdepth 1 -name wacc-selfhosted -type l -delete
if [ ${?} -ne 0 ]; then exit 1; fi

ln -s ${PROJECT_DIR}/driver.sh ${PROJECT_DIR}/wacc-selfhosted
if [ ${?} -ne 0 ]; then exit 1; fi

echo -e "-- Created symlink \033[1;36m${PROJECT_DIR}/wacc-selfhosted\033[0m -> \033[1;32m${PROJECT_DIR}/driver.sh\033[0m"
echo -e "See usage with command \033[1m‘./wacc-selfhosted --help’\033[0m"

exit 0
