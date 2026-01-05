#ifndef __M_TCP_H__
#define __M_TCP_H__
#include<iostream>
#include<string>
#include <netinet/in.h>
#include <sys/socket.h>
#include<thread>
#include<stdlib.h>
#include<memory.h>
#include"/root/work-wjk/logs/mylog.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
class TcpServer
{
public:
    TcpServer(int port,std::string ip)
    :_port(port)
    ,_ip(ip)
    ,_len(sizeof(_addr))
    {
        memset(&_addr,0,sizeof(_addr));
        _addr.sin_family=AF_INET;
        _addr.sin_addr.s_addr=INADDR_ANY;
        _addr.sin_port=htons(_port);
        _fd=socket(AF_INET,SOCK_STREAM,0); 
        if(_fd<0)
        {
            LOG_ERROR("socket failed");
            return;
        }
        if(bind(_fd,(struct sockaddr*)&_addr,_len)<0)
        {
            LOG_ERROR("bind failed");
            return;
        }
        if(listen(_fd,10)<0)
        {
            return;
        }
        LOG_INFO("服务器启动成功");
    }
    void start()
    {
        while(true)
        {
            int client_fd=::accept(_fd,(struct sockaddr*)&_addr,&_len);
            if(client_fd<0) continue;
            LOG_INFO("accept等待客户端成功");
            //创建线程执行任务;
            _thread=std::thread([this,client_fd](){
                handle(client_fd);
            });
            ::close(client_fd);
        }
    }
private:
    void handle(int client_fd)
    {
        char buffer[1024];
        while(true)
        {
            ::memset(buffer,0,sizeof(buffer));
            int rev_len=::recv(client_fd,buffer,sizeof(buffer)-1,0);
            if(rev_len<0) continue;
            buffer[rev_len]='\0';
            ::send(client_fd,buffer,sizeof(buffer),0);
        }
    }       
private:
    int _port;
    std::string _ip;
    int _fd;
    struct sockaddr_in _addr;
    std::thread _thread;
    socklen_t _len;
};

class TcpClient
{
public:
    TcpClient(int port,std::string ip)
        :_port(port)
        ,_ip(ip)
        ,_len(sizeof(_addr))
        {
            memset(&_addr,0,_len);
            _addr.sin_family=AF_INET;
            _addr.sin_port=htons(_port);
            _fd=socket(AF_INET,SOCK_STREAM,0); 
            if(::connect(_fd, (struct sockaddr*)&_addr,_len)<0)
            {
                LOG_ERROR("client connect failed");
                return;
            }
            LOG_ERROR("client success");
        }
        void start()
        {
            char buffer[1024];
            while(true)
            {
                memset(buffer,0,sizeof(buffer));         
                std::string message;
                getline(std::cin,message);
                int n=::send(_fd,message.c_str(),message.size(),0);
                if(n<0) continue;
                ::recv(_fd,buffer,n,0);
                std::cout<<buffer<<std::endl;
            }
        }
private:
    int _port;
    std::string _ip;
    int _fd;
    struct sockaddr_in _addr;
    std::thread _thread;
    socklen_t _len;
};






#endif