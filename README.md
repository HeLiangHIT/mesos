# Apache Libprocess

**this branch is part of mesos for libprocess and its dependancies only with static lib**

# Installation && Usage

```sh
cd mesos && sh build.sh
cd example/
# build the code by g++ 
ln -s ${INSTALL_DIR} /opt/libprocess
g++ -std=c++11 -Wliteral-suffix -D__STDC_FORMAT_MACROS example-server.cpp -o server -I/opt/libprocess/include -I./ -L/opt/libprocess/lib -lprocess -lprotobuf -lglog -lev -larchive -lry_http_parser -lz -lrt -lpthread
g++ -std=c++11 -Wliteral-suffix -D__STDC_FORMAT_MACROS example-client.cpp -o client -I/opt/libprocess/include -I./ -L/opt/libprocess/lib -lprocess -lprotobuf -lglog -lev -larchive -lry_http_parser -lz -lrt -lpthread
# or
export PKG_CONFIG_PATH=${PWD}/install/pkgconfig/:${PKG_CONFIG_PATH}
g++ example-server.cpp -o server $(pkg-config --cflags --libs libprocess)
g++ example-client.cpp -o client $(pkg-config --cflags --libs libprocess)

# run the demo
./server 127.0.0.1 55444 >server.log 2>&1 &
curl -XGET http://127.0.0.1:55444/server/vars -w "%{http_code}\n"
./client 127.0.0.1 55444
```

# License

Apache Mesos is licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

For additional information, see the LICENSE and NOTICE files.
