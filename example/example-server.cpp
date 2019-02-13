/*
服务器端程序：
1. route 提供 resetful 接口 /vars 返回 HTTP 内容。
2. install 注册 process 之间的消息，供客户端控制。
  + "msg" 消息用于输出客户端消息
  + "stop" 消息用户退出服务端

编译： 
g++ example-server.cpp -o server $(pkg-config --cflags --libs libprocess)

后台运行： ./server 127.0.0.1 55444 >server.log 2>&1 &
查看运行日志： cat server.log
      消息收发端口： 127.0.0.1:55444
      HTTP 端口： http://127.0.0.1:55444

测试是否启动： curl -XGET http://127.0.0.1:55444/server/vars -w "%{http_code}\n"
 */

#include <iostream>
#include <process/process.hpp>
#include <stout/os.hpp>

using namespace process;
using namespace process::http;
using namespace std;

inline void prepareProcessEnv(std::string ip, int port){
  os::setenv("LIBPROCESS_IP", ip);
  os::setenv("LIBPROCESS_TCP_UDP_IP", ip);
  os::setenv("LIBPROCESS_PORT", stringify(port));//HTTP default
  os::setenv("LIBPROCESS_TCP_PORT", stringify(port + 1));//TCP port
  os::setenv("LIBPROCESS_UDP_PORT", stringify(port + 2));
  os::setenv("LIBPROCESS_MSG_PORT", stringify(port + 3));
  os::setenv("LIBPROCESS_TCP_UDP_PORT", stringify(port + 4));
}

class Sprocess : public Process<Sprocess> {
public:
  Sprocess() : ProcessBase("server") {}
  virtual ~Sprocess() {}

  Future<Response> vars(const Request &request) {
    string body = "OK!";
    cout << "get curl request!" << request.body << endl;
    return OK(body);
  }

  void stop(const UPID &from, const string &body) {
    cout << "stop msg get! terminate server!" << endl;
    terminate(self());
  }

  void msg(const UPID &from, const string &body) {
    cout << "from " << from << " get msg : " << body << endl;
    string msg = "Processed msg: " + body;
    send(from, "msg", msg.c_str(), msg.length());
  }

protected:
  virtual void initialize() {
    route("/vars", "test for http route", &Sprocess::vars);
    install("msg", &Sprocess::msg);
    install("stop", &Sprocess::stop);
  }
};

int main(int argc, char **argv) {
  string addr = argv[1];
  int port = atoi(argv[2]);
  prepareProcessEnv(addr, port);
  cout << "server is listen on: " << addr << ":" << port << endl;

  Sprocess process;
  PID<Sprocess> pid = spawn(&process);
  cout << "server start at: \n"
       << "IP addr = " << address() << endl;
  cout << "PID = " << pid << endl << endl;

  wait(pid);
  return 0;
}
