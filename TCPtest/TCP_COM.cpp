#include "stdafx.h"
#include "TCP_COM.h"

//******************************************************************************/
//
//  �������� : �򿪲�����COM��


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
	DCB dcbCom;		// ���ڿ��ƿ�

	// ���ڳ�ʱ���Ʋ���
	COMMTIMEOUTS timeouts =
	{
		10,    // ���ַ������ʱʱ��: 100 ms
		1,		// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		50,	// ������(�����)����ʱʱ��: 500 ms
		1,		// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		10     // ������(�����)д��ʱʱ��: 100 ms
	};

	hComm = CreateFile(szComPort,       // �������ƻ��豸·��
		GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ
		0,                              // ����ʽ����ռ
		NULL,                           // Ĭ�ϵİ�ȫ������
		OPEN_EXISTING,                  // ������ʽ
		0,                              // ���������ļ�����
		NULL);                          // �������ģ���ļ�

	// �򿪴���ʧ��
	if(INVALID_HANDLE_VALUE == hComm)
	{
		AfxMessageBox(_T("�򿪴���ʧ��"));
		return INVALID_HANDLE_VALUE;
		
	}

	// ��ȡȡ DCB
	if (!GetCommState(hComm, &dcbCom))
	{
		AfxMessageBox(_T("��ȡȡ DCBʧ��"));
		CloseHandle(hComm);
		return INVALID_HANDLE_VALUE;
	}

	dcbCom.BaudRate = 38400;
	dcbCom.ByteSize = 8;
	dcbCom.Parity   = NOPARITY;
	dcbCom.StopBits = ONESTOPBIT;

	SetupComm(hComm, 1024, 1024);       // �������������������С
	SetCommTimeouts(hComm, &timeouts);  // ���ó�ʱ
	int i=SetCommState(hComm, &dcbCom);       // ����DCB
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
*     �رմ��ڡ�
* Parameter:
*     @[in ] hComm: ���ڲ��������
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
*     ����д�����ݡ�
* Parameter:
*     @[in ] hComm: ���ڲ��������
*     @[in ] wbuf: ��д������ݡ�
*     @[in ] buflen: ��д�����ݵĳ��ȡ�
* ReturnValue:
*     ʵ��д������ݳ��ȡ�
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
*     ���ڶ�ȡ���ݡ�
* Parameter:
*     @[in ] hComm: ���ڲ��������
*     @[in ] rbuf: ��ȡ���ݵĽ��ջ��档
*     @[in ] buflen: ��ȡ���ݵĽ��ջ��泤�ȡ�
* ReturnValue:
*     ʵ�ʶ��������ݳ���
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
* ���ݽ����߳���ں�����
*/
x_uint32_t __stdcall TCP_Com::iocom_thread_entry(x_pvoid_t xpvt_param)
{
	TCP_Com * this_obj_ptr = (TCP_Com *)xpvt_param;
	this_obj_ptr->iocom_fncbk();

	return 0;
}



x_void_t TCP_Com::iocom_fncbk()
{



	//���淢���������ӵĿͻ��˵��׽�����Ϣ  
	

	TCP_COM_DATA *Recv_Buff;
	Recv_Buff = (struct _TCP_COM_DATA*)malloc(sizeof(_TCP_COM_DATA));//��ʼ���ṹ��ָ��


	int  ReadCOMLen = 0;   // Ӧ���ȡ��������ݳ���
	BYTE ComResponse[256];     // Ӧ���ȡ����

	while (m_ComHandle!=INVALID_HANDLE_VALUE)
	{

		ReadCOMLen = ReadComm(ComResponse, 100);
		if (ReadCOMLen)
		{
			Recv_Buff->TCP_COM_len=ReadCOMLen;
			memcpy(Recv_Buff->TCP_COM_buff,ComResponse,ReadCOMLen);
			

			PostMessage(m_xst_com_tid, m_xst_com_mid, 0, (LPARAM)Recv_Buff);//�����߳�Ͷ����Ϣ�����տͻ�����Ϣ������COM��
			//Sleep(1);
			memset(ComResponse,0,100);
			//memset(Recv_Buff->TCP_COM_buff,0,100);
			

			
		}
	}
	

	free(Recv_Buff);
	//AfxMessageBox(_T("COM�ڽ����߳̽���"));
}
