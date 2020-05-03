/**
* @file    TCPWorker.h
* <pre>
* Copyright (c) 2018, HelloWorld
* 
* 文件名称：TCP_SERVER.h
* 创建日期：2018年1月26日
* 文件标识：
* 文件摘要：TCP 服务器阻塞模型。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018/1/26
* 版本摘要：
* 
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
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
	//数据指针指针
	typedef struct _TCP_DATA
	{
		int       TCP_len;       ///< 缓存长度
		char      TCP_buff[100];    ///< 缓存数据
	
	} TCP_DATA;

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   访问类型 : public 
	*   功能描述 : 启动工作对象。
	* </pre>
	* 
	* @param [in ] xst_app_tid  : 通知的目标线程 ID （若设置值为 0 时，则启用自身工作线程）。
	* @param [in ] xst_app_mid  : 通知的目标线程 消息标识（xst_app_tid 不为 0 时，有效）。
	* @param [in ] xpvt_app_ctx : 通知的目标线程 上下文描述（xst_app_tid 不为 0 时，有效）。
	* 
	* @return x_int32_t
	*         - 成功，返回 0；
	*         - 失败，返回 错误码。
	*/
	x_int32_t start(HWND xst_app_tid, x_size_t xst_app_mid, x_pvoid_t xpvt_app_ctx);


	/******************************************************************************/
	/**
	

	*   功能描述 : 打开服务器 地址：127.0.0.1  端口：8080。

	*/
	int open(void);

	/******************************************************************************/
	/**
	* 关闭服务器。
	*/
	void close();

	/******************************************************************************/


	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   访问类型 : public 
	*   功能描述 : 发送数据。
	* </pre>
	* 
	* @param [in ] Send_Buff : 消息内容。
	* @param [in ] Buff_len : 消息长度。
	* 
	* @return x_int32_t
	*         - 返回 发送字节数。
	*/
	int send_packet(char * Send_Buff, int Buff_len);

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   访问类型 : public 
	*   功能描述 : 接收应答消息。
	* </pre>
	* 
	* @param [in ] Recv_Buff : 接收消息内容的缓存。
	* @param [in ] Buff_len : 接收消息内容的缓存长度。
	* 
	* @return x_int32_t
	*         - 返回 接收字节数。
	*/
	int recv_packet(char * Recv_Buff, int Buff_len);


	/******************************************************************************/
	/**
	* 数据接收线程入口函数。
	*/
	static x_uint32_t __stdcall iorecv_thread_entry(x_pvoid_t xpvt_param);

	/******************************************************************************/
	/**
	* 数据接收流程（工作线程调用）。
	*/
	x_void_t iorecv_fncbk();

	// inner invoking
protected:

	// class data
protected:
	HWND                 m_xst_notify_tid;     ///< 通知的线程 ID
	x_size_t                 m_xst_notify_mid;     ///< 通知的线程 消息标识
	x_pvoid_t                m_xpvt_notify_ctx;    ///< 通知的线程 上下文描述

	SOCKET				m_Server;			///< 套接字
	x_handle_t			pThread_TCP;		//定义线程TCP Server
	SOCKET				m_sockClient;		//客户端套接字

};

#endif 