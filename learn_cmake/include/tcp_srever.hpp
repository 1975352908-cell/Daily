#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
#include<iostream>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<thread>
#include<cstring>
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<cerrno>
class TcpServer
{
public:
    TcpServer(int port,std::string ip)
    :_ip(ip)
    ,_port(port)
    ,_socketfd(-1)
    {
        _socketfd=socket(AF_INET,SOCK_STREAM,0);
        if(_socketfd == -1) {
            throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
        }

        memset((void*)&_addr,0,sizeof(_addr));
        _addr.sin_port=htons(_port);
        _addr.sin_family=AF_INET;
        _addr.sin_addr.s_addr=INADDR_ANY;
        int ret=bind(_socketfd,(struct sockaddr*)&_addr,_addr_len);
        if(ret==-1)
        {
            ::close(_socketfd);
            _socketfd = -1;
            throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
        }
        if(listen(_socketfd,10)==-1)
        {
            ::close(_socketfd);
            _socketfd = -1;
            throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
        }
    }
    ~TcpServer()
    {
        if(_socketfd != -1) {
            ::close(_socketfd);
            _socketfd = -1;
        }
    }
    void start()
    {
        while(true)
        {
            int client_fd=::accept(_socketfd,(struct sockaddr*)&_addr,&_addr_len);
            if(client_fd==-1)
            {
                std::cerr<<"Accept failed: "<<strerror(errno)<<std::endl;
                continue;
            }
            //代表有新的客户端连接;
            std::cout<<"new client connected: "<<inet_ntoa(_addr.sin_addr)<<":"<<ntohs(_addr.sin_port)<<std::endl;

            // 为每个客户端创建独立的线程
            std::thread client_thread([this,client_fd](){
                handle(client_fd);
                close(client_fd);
            });
            client_thread.detach(); // 让线程独立运行
        }
    }
    void recv(int client_fd,char* buffer,int len)
    {
        int total=0;
        while(total<len)
        {
            int ret=::recv(client_fd,buffer+total,len-total,0);
            if(ret==-1)
            {
                throw std::runtime_error("recv failed");
            }
            else if(ret==0)
            {
                // 连接已关闭
                throw std::runtime_error("connection closed by peer");
            }
            total += ret;
        }
    }
    void send(int client_fd,char* buffer)
    {
        ::send(client_fd,buffer,strlen(buffer),0);
    }
    void close(int client_fd)
    {
        ::close(client_fd);
    }
private:
    void handle(int client_fd)
    {
        char buffer[1024];
        while(true)
        {
        memset(buffer,0,sizeof(buffer));
        //读取数据
        int bytes_received = ::recv(client_fd,buffer,sizeof(buffer)-1,0);
        if(bytes_received <= 0) {
            std::cerr << "Failed to receive data or connection closed" << std::endl;
            return;
        }
        buffer[bytes_received] = '\0'; // 确保字符串以null结尾
        //回显读取的数据;
        std::cout<<"Received: "<<buffer<<std::endl;
        //只发送实际接收到的数据
        ::send(client_fd,buffer,bytes_received,0);
        }
    }
private:
    int _port;
    std::string _ip;
    int _socketfd;
    struct sockaddr_in _addr;
    std::thread _thread;
    socklen_t _addr_len=sizeof(_addr);
};


//客户端;
class TcpClient
{
public:
    TcpClient(std::string ip,int port)
    :_ip(ip)
    ,_port(port)
    {
        _fd=socket(AF_INET,SOCK_STREAM,0);
        if(_fd == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        memset(&_tcp_client, 0, sizeof(_tcp_client));
        _tcp_client.sin_family = AF_INET;
        _tcp_client.sin_port = htons(_port);
        if(inet_pton(AF_INET, _ip.c_str(), &_tcp_client.sin_addr) <= 0) {
            std::cerr << "Invalid IP address: " << _ip << std::endl;
            ::close(_fd);
            _fd = -1;
            return;
        }

        if(connect(_fd, (struct sockaddr*)&_tcp_client, sizeof(_tcp_client)) == -1) {
            std::cerr << "Failed to connect to server: " << strerror(errno) << std::endl;
            ::close(_fd);
            _fd = -1;
        }
    }
    ~TcpClient()
    {
        ::close(_fd);
    }
    void start()
    {
        if(_fd == -1) {
            std::cerr << "Cannot start client: not connected to server" << std::endl;
            return;
        }
        //发送10条消息然后退出
        for(int i=0;i<10;i++)
        {
            std::string message="hello"+std::to_string(i);
            send(message);
            rev(message);
            // 添加短暂延迟，避免发送太快
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    void send(const std::string& message)
    {
        if(_fd == -1) {
            std::cerr << "Socket not connected" << std::endl;
            return;
        }
        int n=::send(_fd,message.c_str(),message.size(), 0);
        if(n>0)
        {
            std::cout<<"send success"<<std::endl;
        }
        else {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
        }
    }
    int rev(std::string& message)
    {
        char buffer[1024];
        ::memset(buffer, 0, sizeof(buffer));  // 确保每次接收前清空缓冲区
        int n=::recv(_fd,buffer,sizeof(buffer)-1, 0);
        if (n > 0)
        {
            buffer[n] = '\0';  // 确保字符串以 '\0' 结尾
            message = buffer;  // 设置消息
            std::cout << "Received from server: " << message << std::endl;
        }
        else if (n == 0)
        {
            std::cout << "Server closed connection" << std::endl;
        }
        else
        {
            perror("recv error");
        }
        return n;
    }
private:
    void stop()
    {
        if(_fd>0)
        {
            ::close(_fd);
            return;
        }
    }
private:
    int _fd;
    int _port;
    std::string _ip;
    struct sockaddr_in _tcp_client;
    socklen_t len;    
};







#endif