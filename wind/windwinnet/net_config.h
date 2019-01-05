#ifndef _NET_CONFIG_H_
#define _NET_CONFIG_H_
#include "base_define.h"
extern int32 MAX_CONNECTION;						//�������������
extern int32 RECV_LOOP_QUEUE_SIZE;					//����ѭ��buffer��С��Epoll�̵߳����̵߳����ݹܵ���С
extern int32 SEND_LOOP_QUEUE_SIZE;					//����ѭ��buffer��С�������̵߳�Epoll�̵߳����ݹܵ���С
extern int32 POP_TO_BUF_COUNT;						//ÿ������ʱ��������е���������¼��ĸ���
extern int32 VAL_SO_SNDLOWAT;						//���͵ͳ�����setsockopt�е�SO_SNDLOWAT
extern uint32 DELAY_RELEASE_CPSOCK_TIME;


const int32 MAX_NET_REQUEST_SIZE = 512 * 1024;	    //���̵߳�Epoll�̵߳�������д�С
const int32 MAX_NET_EVENT_SIZE = 512 * 1024;		//Epoll�̵߳����̵߳������¼����д�С
const int32 MAX_NET_DATA_QUEUE_SIZE = 512 * 1024;		//Epoll�߳�<->���̵߳Ľ��շ��͵����ݶ��д�С
const int32	INIT_NET_EVENT_POOL_SIZE = 128 * 1024;		//�����¼��ڴ�س�ʼ��С

extern int32 g_nThreadNum;
extern int32 g_nLoopSize;
extern bool  g_bNodelay;
#define DEFAULT_SENDBUF_SIZE	65536
#define DEFAULT_RECVBUF_SIZE	65536
#endif // 