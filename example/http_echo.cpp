
/*HttpEcho 服务器端，打印输出客户端接收日志并原封不动的返回内容

编译：g++ http_echo.cpp -o http_echo $(pkg-config --cflags --libs libprocess)

使用：
./http_echo 127.0.0.1 55444 > http_echo.log 2>&1 &
curl -X POST -d 'test echo' http://127.0.0.1:55444/HttpEcho/echo
curl -X POST http://127.0.0.1:55444/HttpEcho/stop
 */

#include <process/process.hpp>
#include <stout/os.hpp>

using namespace process;
using namespace process::http;

inline void prepareProcessEnv(std::string ip, int port){
  os::setenv("LIBPROCESS_IP", ip);
  os::setenv("LIBPROCESS_PORT", stringify(port));//HTTP default
}

class HttpEcho : public Process<HttpEcho>{
public:
  HttpEcho() : ProcessBase("HttpEcho"){}
  ~HttpEcho() {
      LOG(INFO) << "====== HttpEcho exit ====== ";
  }

  Future<Response> echo(const Request& request)  {
    // parser request
    std::string info = "method=" + request.method + ",url=" + request.url.path + ",client=";
    if (request.client.isSome()) {
        std::ostringstream client;
        client << request.client.get();
        info = info + client.str();
    }
    LOG(INFO) << info << ",body=" << request.body;
    // return to sender
    return OK(request.body);
  }
  Future<Response> stop(const Request& request)  {
    // do terminate self
    terminate(self());
    //return to sender
    return OK("stop ok");
  }

protected:
  virtual void initialize(){
    LOG(INFO) << "====== HttpEcho start ====== ";
    // post /HttpEcho/echo
    route("/echo", "echo func", &HttpEcho::echo);
    // post /HttpEcho/stop
    route("/stop", "stop HttpEcho", &HttpEcho::stop);
  }
};

int main(int argc, char** argv){
  std::string addr = argv[1];
  int port = atoi(argv[2]);
  prepareProcessEnv(addr, port);
  LOG(INFO) << "http_echo is listen on " << addr << ":" << port;

  HttpEcho process;
  PID<HttpEcho> pid = spawn(&process);
  
  wait(pid);
  return 0;
}




