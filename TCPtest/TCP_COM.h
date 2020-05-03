/**
* @file    TCP_COM.h
* <pre>
* Copyright (c) 2018, HelloWorld
* 
* 文件名称：TCP_SERVER.h
* 创建日期：2018年1月30日
* 文件标识：
* 文件摘要：COM口操作。
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

#ifndef __TCPCOM_H__
#define __TCPCOM_H__
#include "vxDTypeDeclare.h"

class TCP_Com
{
public:
	TCP_Com();
	~TCP_Com();


public:
	/******************************************************************************/
	//数据指针指针
	typedef struct _TCP_COM_DATA
	{
		int       TCP_COM_len;       ///< 缓存长度
		char      TCP_COM_buff[100];    ///< 缓存数据
	
	} TCP_COM_DATA;

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   访问类型 : public 
	*   功能描述 : 传递参数。
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


	/**************************************************************************

	打开COM口

	*/
	HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);

	/**************************************************************************

	关闭COM口

	*/
	void CloseComm();

	/**************************************************************************

	读COM口

	*/
	int ReadComm(void* rbuf, int buflen);

	/**************************************************************************

	写COM口

	*/
	int WriteComm(void* wbuf, int buflen);


	/******************************************************************************/
	/**
	* 数据接收线程入口函数。
	*/
	static x_uint32_t __stdcall iocom_thread_entry(x_pvoid_t xpvt_param);

	/******************************************************************************/
	/**
	* 数据接收流程（工作线程调用）。
	*/
	x_void_t iocom_fncbk();

	// inner invoking
protected:

	// class data
protected:
	HWND					 m_xst_com_tid;     ///< 通知的线程 ID
	x_size_t                 m_xst_com_mid;     ///< 通知的线程 消息标识
	x_pvoid_t                m_xpvt_com_ctx;    ///< 通知的线程 上下文描述

	
	x_handle_t			pThread_COM;		//定义线程TCP Server
	HANDLE				m_ComHandle;		//COM口句柄

};


#endif 