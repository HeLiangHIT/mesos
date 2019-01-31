#!/bin/bash

#  ---- build and package libprocess ----
ROOT_PATH=${PWD}
INSTALL_DIR=${ROOT_PATH}/install

# build
./bootstrap
mkdir build
cd build
../configure
make -j

# package
mkdir -p ${INSTALL_DIR}/lib ${INSTALL_DIR}/pkgconfig
find ./ -name lib*.a -type f -exec cp {} ${INSTALL_DIR}/lib \;
cp -rp 3rdparty/include ${INSTALL_DIR}/
cp -rp ../3rdparty/stout/include/stout ${INSTALL_DIR}/include
cp -rp ../3rdparty/libprocess/include/process ${INSTALL_DIR}/include
cd ${ROOT_PATH}
rm -rf build

cat > ${INSTALL_DIR}/pkgconfig/libprocess.pc <<EOF
prefix=${INSTALL_DIR}
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib
includedir=\${prefix}/include

Name: libprocess
Description: Libprocess dependencies
Version: 0.0.1
Libs: -L\${libdir} -lprocess -lprotobuf -lglog -lgmock -lev -lgpr -lhashtable -lleveldb -lprotoc -larchive -lares -lry_http_parser -lzookeeper_mt -lz -lrt -lpthread 
Cflags: -I\${includedir} -Wliteral-suffix -D__STDC_FORMAT_MACROS -std=c++11
EOF

echo -e "\nlibprocess build success, usage: \n\
    1. link libprocess pkg by: export PKG_CONFIG_PATH=${INSTALL_DIR}/pkgconfig/:\${PKG_CONFIG_PATH} \n\
    2. build program by: \$(pkg-config --cflags --libs libprocess) \n\
for instance: cd example/
    g++ example-server.cpp -o server \$(pkg-config --cflags --libs libprocess)
    g++ example-client.cpp -o client \$(pkg-config --cflags --libs libprocess)
"


