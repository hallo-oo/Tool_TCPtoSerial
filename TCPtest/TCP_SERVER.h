/**
* @file    TCPWorker.h
* <pre>
* Copyright (c) 2018, HelloWorld
* 
* �ļ����ƣ�TCP_SERVER.h
* �������ڣ�2018��1��26��
* �ļ���ʶ��
* �ļ�ժҪ��TCP ����������ģ�͡�
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2018/1/26
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
* </pre>
*/
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#define WM_RECVDATA WM_USER+1

#include "vxDTypeDeclare.h"

class TCP_Server
{
public:
	TCP_Server();
	~TCP_Server();


public:
	/******************************************************************************/
	//����ָ��ָ��
	typedef struct _TCP_DATA
	{
		int       TCP_len;       ///< ���泤��
		char      TCP_buff[100];    ///< ��������
	
	} TCP_DATA;

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   �������� : public 
	*   �������� : ������������
	* </pre>
	* 
	* @param [in ] xst_app_tid  : ֪ͨ��Ŀ���߳� ID ��������ֵΪ 0 ʱ���������������̣߳���
	* @param [in ] xst_app_mid  : ֪ͨ��Ŀ���߳� ��Ϣ��ʶ��xst_app_tid ��Ϊ 0 ʱ����Ч����
	* @param [in ] xpvt_app_ctx : ֪ͨ��Ŀ���߳� ������������xst_app_tid ��Ϊ 0 ʱ����Ч����
	* 
	* @return x_int32_t
	*         - �ɹ������� 0��
	*         - ʧ�ܣ����� �����롣
	*/
	x_int32_t start(HWND xst_app_tid, x_size_t xst_app_mid, x_pvoid_t xpvt_app_ctx);


	/******************************************************************************/
	/**
	

	*   �������� : �򿪷����� ��ַ��127.0.0.1  �˿ڣ�8080��

	*/
	int open(void);

	/******************************************************************************/
	/**
	* �رշ�������
	*/
	void close();

	/******************************************************************************/


	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   �������� : public 
	*   �������� : �������ݡ�
	* </pre>
	* 
	* @param [in ] Send_Buff : ��Ϣ���ݡ�
	* @param [in ] Buff_len : ��Ϣ���ȡ�
	* 
	* @return x_int32_t
	*         - ���� �����ֽ�����
	*/
	int send_packet(char * Send_Buff, int Buff_len);

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   �������� : public 
	*   �������� : ����Ӧ����Ϣ��
	* </pre>
	* 
	* @param [in ] Recv_Buff : ������Ϣ���ݵĻ��档
	* @param [in ] Buff_len : ������Ϣ���ݵĻ��泤�ȡ�
	* 
	* @return x_int32_t
	*         - ���� �����ֽ�����
	*/
	int recv_packet(char * Recv_Buff, int Buff_len);


	/******************************************************************************/
	/**
	* ���ݽ����߳���ں�����
	*/
	static x_uint32_t __stdcall iorecv_thread_entry(x_pvoid_t xpvt_param);

	/******************************************************************************/
	/**
	* ���ݽ������̣������̵߳��ã���
	*/
	x_void_t iorecv_fncbk();

	// inner invoking
protected:

	// class data
protected:
	HWND                 m_xst_notify_tid;     ///< ֪ͨ���߳� ID
	x_size_t                 m_xst_notify_mid;     ///< ֪ͨ���߳� ��Ϣ��ʶ
	x_pvoid_t                m_xpvt_notify_ctx;    ///< ֪ͨ���߳� ����������

	SOCKET				m_Server;			///< �׽���
	x_handle_t			pThread_TCP;		//�����߳�TCP Server
	SOCKET				m_sockClient;		//�ͻ����׽���

};

#endif 