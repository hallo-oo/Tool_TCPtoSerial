#include "stdafx.h"
#include "TCP_SERVER.h"
#include <Winsock2.h>  
//******************************************************************************/
//
//  功能描述 : 打开服务器 地址：127.0.0.1  端口：8080。


TCP_Server::TCP_Server()
{

}

TCP_Server::~TCP_Server()
{
	close();
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
x_int32_t TCP_Server::start(HWND xst_app_tid, x_size_t xst_app_mid, x_pvoid_t xpvt_app_ctx)
{




	m_xst_notify_tid  = xst_app_tid;
	m_xst_notify_mid  = xst_app_mid;
	m_xpvt_notify_ctx = xpvt_app_ctx;

		


	return 1;
}


int TCP_Server::open(void)
{
	//用来保存WinSock库的版本号  
	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  

	wVersionRequested = MAKEWORD(2,2);  
	err = WSAStartup(wVersionRequested, &wsaData);  
	if(err != 0) 
	{  
		AfxMessageBox(_T("加载套接字版本号失败"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(_T("加载套接字版本号成功"));
	}
	
	 
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
	{  
		//若不是所请求的版本号2.2,则终止WinSock库的使用  
		WSACleanup();  
		return -1;  
	}  
	//创建用于监听的套接字  
	m_Server = socket(AF_INET, SOCK_STREAM, 0);  
	if(m_Server == INVALID_SOCKET) 
	{  
		AfxMessageBox(TEXT("创建套接字失败"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(TEXT("创建套接字成功"));
	}
	
	  
	//填充服务器端套接字结构  
	SOCKADDR_IN addrServer;  
	//将主机字节顺序转换为TCP/IP网络字节顺序  
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  
	addrServer.sin_family = AF_INET;  
	addrServer.sin_port = htons(8080);  
	//将套接字绑定到一个本地地址和端口上  
	err = bind(m_Server, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));  
	if(err == SOCKET_ERROR) 
	{  
		AfxMessageBox(TEXT("绑定套接字失败 "));
		return -1;  
	} 
	else
	{
		//AfxMessageBox(TEXT("绑定套接字成功"));

	}
	  

	//将套接字设置为监听模式,准备接收客户端请求  
	err = listen(m_Server, 2);  
	if(err  == SOCKET_ERROR) 
	{  
		AfxMessageBox(TEXT("监听套接字失败"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(TEXT("监听套接字成功"));

	}
	  
	//开启线程

	pThread_TCP = (x_handle_t)CreateThread(X_NULL, 0,
		(LPTHREAD_START_ROUTINE)&TCP_Server::iorecv_thread_entry,
		(LPVOID)this, 0, X_NULL);

	return 1;
}


//关闭套接字

void TCP_Server::close()
{
	if (INVALID_SOCKET != m_sockClient)
	{
		closesocket(m_sockClient);
		m_sockClient = INVALID_SOCKET;
	}

	if (INVALID_SOCKET != m_Server)
	{
		closesocket(m_Server);
		m_Server = INVALID_SOCKET;
	}
	if (X_NULL != pThread_TCP)
	{
		CloseHandle(pThread_TCP);
		pThread_TCP = X_NULL;
	}


}


//发送数据


int TCP_Server::send_packet(char * xct_dptr, int xit_dlen)
{
	if (INVALID_SOCKET == m_sockClient)
	{
		return -1;
	}


	int xit_nsend = send(m_sockClient, (char *)xct_dptr, xit_dlen, 0);
	if (xit_nsend < 0)
	{
		int err = WSAGetLastError();
		close();
	}

	return xit_nsend;
}


int TCP_Server::recv_packet(char * xct_dptr, int xit_dlen)
{
	if (INVALID_SOCKET == m_sockClient)
	{
		return -1;
	}

	int xit_nrecv = recv(m_sockClient, (char *)xct_dptr, xit_dlen, 0);
	if (xit_nrecv <= 0)
	{
		int err = WSAGetLastError();
		close();
	}

	return xit_nrecv;
}


/******************************************************************************/
/**
* 数据接收线程入口函数。
*/
x_uint32_t __stdcall TCP_Server::iorecv_thread_entry(x_pvoid_t xpvt_param)
{
	TCP_Server * this_obj_ptr = (TCP_Server *)xpvt_param;
	this_obj_ptr->iorecv_fncbk();

	return 0;
}


x_void_t TCP_Server::iorecv_fncbk()
{
	  
	
	
	//保存发送请求链接的客户端的套接字信息  
	SOCKADDR_IN addrClient;  
	int len = sizeof(SOCKADDR);  
	
	TCP_DATA *Recv_Buff;
	Recv_Buff = (struct _TCP_DATA*)malloc(sizeof(_TCP_DATA));//初始化结构体指针

	SOCKET sockConn;
	//接收到的客户端请求socket
	char sendBuf[100];
	//发送数据
	int Recv_LEN = 0;
	char recvBuf[100] ;
	memset(recvBuf,0,100);
	x_int32_t err=-1;
	sockConn = accept( m_Server , (SOCKADDR *)&addrClient , &len ) ;
	m_sockClient=sockConn;
	//AfxMessageBox(_T("等待客户端连接成功"));
	sprintf_s( sendBuf , " Hello ") ;
	
	send_packet(sendBuf,strlen(sendBuf)+1);
	
	while(Recv_LEN!=-1)
	{///不断得到客户端发来的请求


		//接收数据
		
	
		Recv_LEN = recv_packet(recvBuf,100);
		if (Recv_LEN>0)
		{
		
			Recv_Buff->TCP_len=Recv_LEN;
			memcpy(Recv_Buff->TCP_buff,recvBuf,Recv_LEN);
			//AfxMessageBox(_T("接收数据"));

			PostMessage(m_xst_notify_tid, m_xst_notify_mid, 0, (LPARAM)Recv_Buff);//向主线程投递消息，接收客户度消息发送至COM口
			//Sleep(1);
			memset(recvBuf,0,100);
			//memset(Recv_Buff->TCP_buff,0,100);
		}
		// TCP 模式错误处理
		if (Recv_LEN == SOCKET_ERROR ) 
		{
			err = WSAGetLastError();
			
			//AfxMessageBox(TEXT("接收数据失败 "));
			free(Recv_Buff);
			//PostMessage(m_xst_notify_tid, 1011, 0, (LPARAM)NULL);
			close();
			break;
			
		}
		else
		{
			//AfxMessageBox(_T("接收数据成功"));
		}	

	}

	
	
	//AfxMessageBox(_T("释放套接字"));
}