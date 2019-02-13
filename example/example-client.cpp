/*
客户端程序：
1. 启动时连接服务器端， 然后依次输入发送到服务端的内容， q 退出
2. 客户端退出前会发送一个 stop 命令停止服务器端

编译： 
g++ example-client.cpp -o client $(pkg-config --cflags --libs libprocess)
运行： ./client 127.0.0.1 55444 # 参数为服务器地址+端口
 */
#include <iostream>
#include <process/process.hpp>
#include <stout/ip.hpp>

using namespace process;
using namespace process::http;
using namespace std;

class CProcess : public Process<CProcess> {
public:
  CProcess() {}
  virtual ~CProcess() {}

  void stop(const UPID &to, const string body) {
    send(to, "stop", body.c_str(), body.length());
    cout << "stop msg send! terminate client!" << endl;
  }
  void Send(const UPID &serv, string msg) {
    send(serv, "msg", msg.c_str(), msg.length());
  }

protected:
  virtual void initialize() {
    install("msg", [](const UPID &from, const string &body) {
      cout << "from " << from << " get msg : " << body << endl;
    });
  }
};


int main(int argc, char **argv) {
  CProcess process;
  PID<CProcess> pid = spawn(&process);
  cout << "client start at: \n"
       << "IP addr = " << address() << endl;
  cout << "PID = " << pid << endl;

  string addr = argv[1];
  uint16_t port = atoi(argv[2]);
  UPID serv = UPID("server", net::IP::parse(addr, AF_INET).get(),  port);;
  cout << "server = " << serv << endl;
  cout << "please input msg to send: " << endl;

  string msg;
  do {
    cin >> msg;
    process.Send(serv, msg);
  } while (msg != "q");
  process.stop(serv, "stop please!");
  terminate(pid);
  wait(pid);
  return 0;
}
