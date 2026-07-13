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

# ./build_preset.sh ${CC}
# if [ ${?} -ne 0 ]; then exit 1; fi

PROJECT_NAME="${PROJECT_DIR}/wacc-selfhosted-1"
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
