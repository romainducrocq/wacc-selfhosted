#!/usr/bin/env bash

KERNEL_NAME="$(uname -s)"
PACKAGE_NAME="$(cat ../bin/pkgname.cfg)"
CC="gcc"
CXX="g++"
if [[ "${KERNEL_NAME}" == "Darwin"* ]]; then
    CC="clang -arch x86_64"
    CXX="clang++ -arch x86_64"
elif [[ "${KERNEL_NAME}" == "FreeBSD"* ]]; then
    CC="clang"
    CXX="clang++"
fi

# ./build_preset.sh ${CC}
# if [ ${?} -ne 0 ]; then exit 1; fi

BUILD_RELEASE=0
PROJECT_DIR="$(dirname $(dirname $(readlink -f ${0})))"

CC="${CC} -std=c17"
CXX="${CXX} -std=c++17"
CC_FLAGS="-Wall -Wextra -Wpedantic"
CC_FLAGS_RELEASE="-O3 -DNDEBUG -Werror -pedantic-errors -D__NDEBUG__ -D__GCC_STDINT__"
CC_FLAGS_DEBUG="-ggdb3 -O0"
if [ ${BUILD_RELEASE} -eq 0 ]; then
    CC_FLAGS="${CC_FLAGS} ${CC_FLAGS_RELEASE}"
else
    CC_FLAGS="${CC_FLAGS} ${CC_FLAGS_DEBUG}"
fi

PROJECT_NAME="${PROJECT_DIR}/bin/${PACKAGE_NAME}"

SOURCE_FILES="${PROJECT_DIR}/compiler/main.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/ast.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/back_ast.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/back_symt.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/front_ast.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/front_symt.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/ast/interm_ast.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/optimization/optim_tac.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/optimization/reg_alloc.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/parser/errors.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/parser/lexer.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/parser/parser.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/intermediate/idents.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/intermediate/semantic.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/frontend/intermediate/tac_repr.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/backend/assembly/asm_gen.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/backend/assembly/registers.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/backend/assembly/stack_fix.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/backend/assembly/symt_cvt.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/backend/emitter/gas_code.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/util/fileio.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/util/str2t.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/util/throw.wheelcc"

SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/3rdparty/sds/sds.wheelcc"
SOURCE_FILES="${SOURCE_FILES} ${PROJECT_DIR}/compiler/3rdparty/stb_ds/stb_ds.wheelcc"

BUILD_CACHE="${PROJECT_DIR}/build/build_cache"
if [ -d "${BUILD_CACHE}/" ]; then
    rm -r ${BUILD_CACHE}/
    if [ ${?} -ne 0 ]; then exit 1; fi
fi
mkdir ${BUILD_CACHE}/
if [ ${?} -ne 0 ]; then exit 1; fi

OBJECT_FILES=""
LINK_CC="${CC}"
echo "-- Build objects ..."
for FILE in ${SOURCE_FILES}; do
    OBJECT="${BUILD_CACHE}/$(basename ${FILE%.*}).o"
    OBJECT_FILES="${OBJECT_FILES} ${OBJECT}"
    echo "${FILE} -> ${OBJECT}"
    BUILD_CC="${CC}"
    case "${FILE##*.}" in
        "c")
            ;;
        "wheelcc")
            FILE="${FILE%.*}"
            wheelcc -v -E -c ${FILE}.c
            if [ ${?} -ne 0 ]; then exit 1; fi
            mv -v ${FILE}.o ${OBJECT}
            continue
            ;;
        "cpp")
            BUILD_CC="${CXX}"
            LINK_CC="${CXX}"
            ;;
        *)
            exit 1
    esac
    ${BUILD_CC} -c ${FILE} ${CC_FLAGS} -o ${OBJECT}
    if [ ${?} -ne 0 ]; then exit 1; fi
done
echo "OK"

echo "-- Linking executable ..."
echo "${BUILD_CACHE}/*.o -> ${PROJECT_NAME}"
${LINK_CC} ${OBJECT_FILES} ${CC_FLAGS} -o ${PROJECT_NAME}
if [ ${?} -ne 0 ]; then exit 1; fi
echo "OK"

echo "-- Created target ${PROJECT_NAME}"

exit 0
