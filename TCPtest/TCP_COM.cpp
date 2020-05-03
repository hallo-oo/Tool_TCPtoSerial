#include "stdafx.h"
#include "TCP_COM.h"

//******************************************************************************/
//
//  功能描述 : 打开并操作COM口


TCP_Com::TCP_Com()
{

}

TCP_Com::~TCP_Com()
{
	CloseComm();
}


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
x_int32_t TCP_Com::start(HWND xst_app_tid, x_size_t xst_app_mid, x_pvoid_t xpvt_app_ctx)
{




	m_xst_com_tid  = xst_app_tid;
	m_xst_com_mid  = xst_app_mid;
	m_xpvt_com_ctx = xpvt_app_ctx;


	return 1;
}


HANDLE TCP_Com::OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	HANDLE hComm = INVALID_HANDLE_VALUE;
	m_ComHandle=hComm;
	DCB dcbCom;		// 串口控制块

	// 串口超时控制参数
	COMMTIMEOUTS timeouts =
	{
		10,    // 读字符间隔超时时间: 100 ms
		1,		// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
		50,	// 基本的(额外的)读超时时间: 500 ms
		1,		// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
		10     // 基本的(额外的)写超时时间: 100 ms
	};

	hComm = CreateFile(szComPort,       // 串口名称或设备路径
		GENERIC_READ | GENERIC_WRITE,   // 读写方式
		0,                              // 共享方式：独占
		NULL,                           // 默认的安全描述符
		OPEN_EXISTING,                  // 创建方式
		0,                              // 不需设置文件属性
		NULL);                          // 不需参照模板文件

	// 打开串口失败
	if(INVALID_HANDLE_VALUE == hComm)
	{
		AfxMessageBox(_T("打开串口失败"));
		return INVALID_HANDLE_VALUE;
		
	}

	// 获取取 DCB
	if (!GetCommState(hComm, &dcbCom))
	{
		AfxMessageBox(_T("获取取 DCB失败"));
		CloseHandle(hComm);
		return INVALID_HANDLE_VALUE;
	}

	dcbCom.BaudRate = 38400;
	dcbCom.ByteSize = 8;
	dcbCom.Parity   = NOPARITY;
	dcbCom.StopBits = ONESTOPBIT;

	SetupComm(hComm, 1024, 1024);       // 设置输入输出缓冲区大小
	SetCommTimeouts(hComm, &timeouts);  // 设置超时
	int i=SetCommState(hComm, &dcbCom);       // 设置DCB
	int a=GetLastError();
	PurgeComm(hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);

	pThread_COM = (x_handle_t)CreateThread(X_NULL, 0,
		(LPTHREAD_START_ROUTINE)&TCP_Com::iocom_thread_entry,
		(LPVOID)this, 0, X_NULL);



	m_ComHandle=hComm;
	return hComm;
}

/**************************************************************************
* FunctionName:
*     CloseComm
* Description:
*     关闭串口。
* Parameter:
*     @[in ] hComm: 串口操作句柄。
* ReturnValue:
*     void
*/
void TCP_Com::CloseComm()
{
	if (m_ComHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_ComHandle);
		m_ComHandle=INVALID_HANDLE_VALUE;
	}
	if (X_NULL != pThread_COM)
	{
		CloseHandle(pThread_COM);
		pThread_COM = X_NULL;
	}
	
}


/**************************************************************************
* FunctionName:
*     WriteComm
* Description:
*     串口写入数据。
* Parameter:
*     @[in ] hComm: 串口操作句柄。
*     @[in ] wbuf: 待写入的数据。
*     @[in ] buflen: 待写入数据的长度。
* ReturnValue:
*     实际写入的数据长度。
*/
int TCP_Com::WriteComm(void* wbuf, int buflen)
{
	DWORD dwNumWrite;
	if (INVALID_HANDLE_VALUE == m_ComHandle)
	{
		return -1;
	}

	WriteFile(m_ComHandle, wbuf, (DWORD)buflen, &dwNumWrite, NULL);

	return (int)dwNumWrite;
}

/**************************************************************************
* FunctionName:
*     ReadComm
* Description:
*     串口读取数据。
* Parameter:
*     @[in ] hComm: 串口操作句柄。
*     @[in ] rbuf: 读取数据的接收缓存。
*     @[in ] buflen: 读取数据的接收缓存长度。
* ReturnValue:
*     实际读出的数据长度
*/
int TCP_Com::ReadComm(void* rbuf, int buflen)
{
	DWORD dwNumRead;
	if (INVALID_HANDLE_VALUE == m_ComHandle)
	{
		return -1;
	}

	ReadFile(m_ComHandle, rbuf, (DWORD)buflen, &dwNumRead, NULL);
	return (int)dwNumRead;
}


/******************************************************************************/
/**
* 数据接收线程入口函数。
*/
x_uint32_t __stdcall TCP_Com::iocom_thread_entry(x_pvoid_t xpvt_param)
{
	TCP_Com * this_obj_ptr = (TCP_Com *)xpvt_param;
	this_obj_ptr->iocom_fncbk();

	return 0;
}



x_void_t TCP_Com::iocom_fncbk()
{



	//保存发送请求链接的客户端的套接字信息  
	

	TCP_COM_DATA *Recv_Buff;
	Recv_Buff = (struct _TCP_COM_DATA*)malloc(sizeof(_TCP_COM_DATA));//初始化结构体指针


	int  ReadCOMLen = 0;   // 应答读取缓存的数据长度
	BYTE ComResponse[256];     // 应答读取缓存

	while (m_ComHandle!=INVALID_HANDLE_VALUE)
	{

		ReadCOMLen = ReadComm(ComResponse, 100);
		if (ReadCOMLen)
		{
			Recv_Buff->TCP_COM_len=ReadCOMLen;
			memcpy(Recv_Buff->TCP_COM_buff,ComResponse,ReadCOMLen);
			

			PostMessage(m_xst_com_tid, m_xst_com_mid, 0, (LPARAM)Recv_Buff);//向主线程投递消息，接收客户度消息发送至COM口
			//Sleep(1);
			memset(ComResponse,0,100);
			//memset(Recv_Buff->TCP_COM_buff,0,100);
			

			
		}
	}
	

	free(Recv_Buff);
	//AfxMessageBox(_T("COM口接收线程结束"));
}
