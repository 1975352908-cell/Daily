#include<iostream>
#include"include/tcp_srever.hpp"
#include<chrono>
#include<thread>
int main()
{
    // 1. 在一个新线程启动服务端
    std::thread server_thread([](){
            TcpServer server(8088,"127.0.0.1");
            server.start();
        }
    );

    // 2. 主线程等一下，确保服务端先起来
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 3. 在主线程启动客户端去连接
    TcpClient client("127.0.0.1", 8088);
    client.start();

    // 4. 等待服务端线程结束
    server_thread.join();
    return 0;
}