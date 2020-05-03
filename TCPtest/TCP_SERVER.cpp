#include "stdafx.h"
#include "TCP_SERVER.h"
#include <Winsock2.h>  
//******************************************************************************/
//
//  �������� : �򿪷����� ��ַ��127.0.0.1  �˿ڣ�8080��


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
x_int32_t TCP_Server::start(HWND xst_app_tid, x_size_t xst_app_mid, x_pvoid_t xpvt_app_ctx)
{




	m_xst_notify_tid  = xst_app_tid;
	m_xst_notify_mid  = xst_app_mid;
	m_xpvt_notify_ctx = xpvt_app_ctx;

		


	return 1;
}


int TCP_Server::open(void)
{
	//��������WinSock��İ汾��  
	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  

	wVersionRequested = MAKEWORD(2,2);  
	err = WSAStartup(wVersionRequested, &wsaData);  
	if(err != 0) 
	{  
		AfxMessageBox(_T("�����׽��ְ汾��ʧ��"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(_T("�����׽��ְ汾�ųɹ�"));
	}
	
	 
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
	{  
		//������������İ汾��2.2,����ֹWinSock���ʹ��  
		WSACleanup();  
		return -1;  
	}  
	//�������ڼ������׽���  
	m_Server = socket(AF_INET, SOCK_STREAM, 0);  
	if(m_Server == INVALID_SOCKET) 
	{  
		AfxMessageBox(TEXT("�����׽���ʧ��"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(TEXT("�����׽��ֳɹ�"));
	}
	
	  
	//�����������׽��ֽṹ  
	SOCKADDR_IN addrServer;  
	//�������ֽ�˳��ת��ΪTCP/IP�����ֽ�˳��  
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  
	addrServer.sin_family = AF_INET;  
	addrServer.sin_port = htons(8080);  
	//���׽��ְ󶨵�һ�����ص�ַ�Ͷ˿���  
	err = bind(m_Server, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));  
	if(err == SOCKET_ERROR) 
	{  
		AfxMessageBox(TEXT("���׽���ʧ�� "));
		return -1;  
	} 
	else
	{
		//AfxMessageBox(TEXT("���׽��ֳɹ�"));

	}
	  

	//���׽�������Ϊ����ģʽ,׼�����տͻ�������  
	err = listen(m_Server, 2);  
	if(err  == SOCKET_ERROR) 
	{  
		AfxMessageBox(TEXT("�����׽���ʧ��"));
		return -1;  
	}
	else
	{
		//AfxMessageBox(TEXT("�����׽��ֳɹ�"));

	}
	  
	//�����߳�

	pThread_TCP = (x_handle_t)CreateThread(X_NULL, 0,
		(LPTHREAD_START_ROUTINE)&TCP_Server::iorecv_thread_entry,
		(LPVOID)this, 0, X_NULL);

	return 1;
}


//�ر��׽���

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


//��������


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
* ���ݽ����߳���ں�����
*/
x_uint32_t __stdcall TCP_Server::iorecv_thread_entry(x_pvoid_t xpvt_param)
{
	TCP_Server * this_obj_ptr = (TCP_Server *)xpvt_param;
	this_obj_ptr->iorecv_fncbk();

	return 0;
}


x_void_t TCP_Server::iorecv_fncbk()
{
	  
	
	
	//���淢���������ӵĿͻ��˵��׽�����Ϣ  
	SOCKADDR_IN addrClient;  
	int len = sizeof(SOCKADDR);  
	
	TCP_DATA *Recv_Buff;
	Recv_Buff = (struct _TCP_DATA*)malloc(sizeof(_TCP_DATA));//��ʼ���ṹ��ָ��

	SOCKET sockConn;
	//���յ��Ŀͻ�������socket
	char sendBuf[100];
	//��������
	int Recv_LEN = 0;
	char recvBuf[100] ;
	memset(recvBuf,0,100);
	x_int32_t err=-1;
	sockConn = accept( m_Server , (SOCKADDR *)&addrClient , &len ) ;
	m_sockClient=sockConn;
	//AfxMessageBox(_T("�ȴ��ͻ������ӳɹ�"));
	sprintf_s( sendBuf , " Hello ") ;
	
	send_packet(sendBuf,strlen(sendBuf)+1);
	
	while(Recv_LEN!=-1)
	{///���ϵõ��ͻ��˷���������


		//��������
		
	
		Recv_LEN = recv_packet(recvBuf,100);
		if (Recv_LEN>0)
		{
		
			Recv_Buff->TCP_len=Recv_LEN;
			memcpy(Recv_Buff->TCP_buff,recvBuf,Recv_LEN);
			//AfxMessageBox(_T("��������"));

			PostMessage(m_xst_notify_tid, m_xst_notify_mid, 0, (LPARAM)Recv_Buff);//�����߳�Ͷ����Ϣ�����տͻ�����Ϣ������COM��
			//Sleep(1);
			memset(recvBuf,0,100);
			//memset(Recv_Buff->TCP_buff,0,100);
		}
		// TCP ģʽ������
		if (Recv_LEN == SOCKET_ERROR ) 
		{
			err = WSAGetLastError();
			
			//AfxMessageBox(TEXT("��������ʧ�� "));
			free(Recv_Buff);
			//PostMessage(m_xst_notify_tid, 1011, 0, (LPARAM)NULL);
			close();
			break;
			
		}
		else
		{
			//AfxMessageBox(_T("�������ݳɹ�"));
		}	

	}

	
	
	//AfxMessageBox(_T("�ͷ��׽���"));
}