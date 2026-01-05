#include"./include/tcp.hpp"

int main()
{
    TcpServer server(8088,"127.0.0.1");
    server.start();
    TcpClient client(8088,"127.0.0.1");
    client.start();
    return 0;
}