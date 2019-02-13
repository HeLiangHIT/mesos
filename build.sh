#!/bin/bash

#  ---- build and package libprocess ----
ROOT_PATH=${PWD}
INSTALL_DIR=${ROOT_PATH}/install

# build
./bootstrap
mkdir build
cd build
../configure
make
if [[ $? != 0 ]]; then
	echo -e "\033[31mbuild libprocess failed! please check log for more infomation. you can try:\033[0m"
	echo -e "yum groupinstall -y 'Development Tools'"
	echo -e "yum install -y zlib-devel libcurl-devel openssl-devel cyrus-sasl-devel cyrus-sasl-md5 apr-devel subversion-devel apr-util-devel apache-maven python-devel python-six python-virtualenv"
	exit 1
fi

# package
mkdir -p ${INSTALL_DIR}/lib ${INSTALL_DIR}/pkgconfig
find ./ -name lib*.a -type f -exec cp {} ${INSTALL_DIR}/lib \;

cp -rp 3rdparty/bin ${INSTALL_DIR}/
cp -rp 3rdparty/include ${INSTALL_DIR}/

cp -rp 3rdparty/googletest-release-*/google*/include/ \
  3rdparty/concurrentqueue*/*.h  3rdparty/concurrentqueue*/internal \
  3rdparty/grpc*/include/  3rdparty/http-parser*/http_parser.h \
  3rdparty/leveldb*/include/ \
  3rdparty/picojson*/picojson.h \
  ${INSTALL_DIR}/include
mkdir -p ${INSTALL_DIR}/include/libarchive && cp 3rdparty/libarchive*/libarchive/*.h ${INSTALL_DIR}/include/libarchive
mkdir -p ${INSTALL_DIR}/include/libev && cp 3rdparty/libev*/*.h ${INSTALL_DIR}/include/libev

cp -rp ../3rdparty/stout/include/stout ${INSTALL_DIR}/include
cp -rp ../3rdparty/libprocess/include/process ${INSTALL_DIR}/include


cd ${ROOT_PATH}
# rm -rf build

cat > ${INSTALL_DIR}/pkgconfig/libprocess.pc <<EOF
prefix=${INSTALL_DIR}
exec_prefix=\${prefix}/bin
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: libprocess
Description: Libprocess dependencies
Version: 0.0.1
Libs: -L\${libdir} -lprocess -lprotobuf -lglog -lgmock -lev -lgpr -lhashtable -lleveldb -lprotoc -larchive -lares -lry_http_parser -lzookeeper_mt -lz -lrt -lpthread 
Libs.private: -lz
Cflags: -I\${includedir} -Wliteral-suffix -D__STDC_FORMAT_MACROS -std=c++11
EOF

echo -e "\nbuild libprocess success, usage:
    1. link libprocess pkg by: \033[32m export PKG_CONFIG_PATH=${INSTALL_DIR}/pkgconfig/:\${PKG_CONFIG_PATH} \033[0m
    2. build program by: \033[32m \$(pkg-config --cflags --libs libprocess) \033[0m
    3. if you need to use protoc/zookeeper, export path by: \033[32m export PATH=${INSTALL_DIR}/bin/:\${PATH} \033[0m
for instance: cd example/
    g++ example-server.cpp -o server \$(pkg-config --cflags --libs libprocess)
    g++ example-client.cpp -o client \$(pkg-config --cflags --libs libprocess)
"


