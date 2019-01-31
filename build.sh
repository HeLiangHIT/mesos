
#  ---- build and package libprocess ----
INSTALL_DIR=${PWD}/install

# build
./bootstrap
mkdir build && cd build && ../configure && make -j

# package
mkdir -p ${INSTALL_DIR}/lib
find ./ -name lib*.a -type f -exec cp {} ${INSTALL_DIR}/lib \;
cp -rp 3rdparty/include ${INSTALL_DIR}/
cp -rp ../3rdparty/stout/include/stout ${INSTALL_DIR}/include
cp -rp ../3rdparty/libprocess/include/process ${INSTALL_DIR}/include


