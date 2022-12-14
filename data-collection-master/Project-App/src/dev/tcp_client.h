#pragma once 
#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__
#include "lib.h"
//串口时钟头文件
#include <signal.h>
#include <time.h>

//线程， 信号量头文件
#include <pthread.h>
#include <semaphore.h>

// socket头文件
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h> // for sockaddr_in
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h> // for socket

// C++流处理相关结构体
#include <cstdio>
#include <iostream>
#include <string.h>
//using std::string;


enum TCP_STEP{
    TCP_STEP_NULL = 0,

    TCP_STEP_OPEN,
    TCP_STEP_CONNECT,
    TCP_STEP_OPEN_ERROR,
    TCP_STEP_OPEN_OK,

    TCP_STEP_SEND, //=5
    TCP_STEP_RESEND,
    TCP_STEP_SEND_OK,
    TCP_STEP_SEND_ERROR,

    TCP_STEP_RECV_WAIT,  //=9
    TCP_STEP_RECV_GET_OK,
    TCP_STEP_RECV_TIMEOUT,
};

class tcp_client
{
private:
    //控制线程
    pthread_t pfd;  //数据



public:  

    //数据包解析
    packet_process *packet;             //packet process for read data

    //packet
    bool ppfd_get = false;
    pthread_t ppfd; 
    void*user;

    //keepalive
    bool keep = true;

    void tcp_state_function(void);
    TCP_STEP  step;
    TCP_STEP  lstep;
    int socketfd;
    bool opened = false;

    //链接，发送，接收 超时
    int connect_timeout_ms_max = 1000;
    int send_timeout_ms_max = 2000;
    int recv_timeout_ms_max = 5000;
    ul64 connect_timeout;
    ul64 send_timeout;
    ul64 recv_timeout;

    //服务器地址
    struct sockaddr_in targetAddr; // udp server address
    char setip[20] = {0};
    int  setport = 0;

    //发送缓冲
    u8 sendbuffer[1024];
    int sendlen=0;
    int sendreconnect=0;

    //接受缓冲
    u8 recvbuffer[1024];
    int recvwannlen=0;
    int recvlen=0;

    //重连
    int reconnect = 0;

    
public:
    void*user_data=NULL;
    tcp_client();
    ~tcp_client();
    int config(const char*ip,int port);
    int start(void*arg);
    int quit(void);
    int keepalive(bool keep);
    int senddata(void*buffer,int len,int send_timeout, int recv_timeout,int recv_wann);
    int set_packet_process(void* func,int frame_timeout);
    bool is_send_end(void);
};

#endif