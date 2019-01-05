#ifndef _NET_CONFIG_H_
#define _NET_CONFIG_H_
#include "base_define.h"
extern int32 MAX_CONNECTION;						//最大网络连接数
extern int32 RECV_LOOP_QUEUE_SIZE;					//接收循环buffer大小，Epoll线程到主线程的数据管道大小
extern int32 SEND_LOOP_QUEUE_SIZE;					//发送循环buffer大小，主线线程到Epoll线程的数据管道大小
extern int32 POP_TO_BUF_COUNT;						//每次向临时缓冲队列中弹出请求或事件的个数
extern int32 VAL_SO_SNDLOWAT;						//发送低潮，即setsockopt中的SO_SNDLOWAT
extern uint32 DELAY_RELEASE_CPSOCK_TIME;


const int32 MAX_NET_REQUEST_SIZE = 512 * 1024;	    //主线程到Epoll线程的请求队列大小
const int32 MAX_NET_EVENT_SIZE = 512 * 1024;		//Epoll线程到主线程的网络事件队列大小
const int32 MAX_NET_DATA_QUEUE_SIZE = 512 * 1024;		//Epoll线程<->主线程的接收发送的数据队列大小
const int32	INIT_NET_EVENT_POOL_SIZE = 128 * 1024;		//网络事件内存池初始大小

extern int32 g_nThreadNum;
extern int32 g_nLoopSize;
extern bool  g_bNodelay;
#define DEFAULT_SENDBUF_SIZE	65536
#define DEFAULT_RECVBUF_SIZE	65536
#endif // 