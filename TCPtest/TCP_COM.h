/**
* @file    TCP_COM.h
* <pre>
* Copyright (c) 2018, HelloWorld
* 
* �ļ����ƣ�TCP_SERVER.h
* �������ڣ�2018��1��30��
* �ļ���ʶ��
* �ļ�ժҪ��COM�ڲ�����
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
	//����ָ��ָ��
	typedef struct _TCP_COM_DATA
	{
		int       TCP_COM_len;       ///< ���泤��
		char      TCP_COM_buff[100];    ///< ��������
	
	} TCP_COM_DATA;

	/******************************************************************************/
	/**
	* @brief
	* <pre>
	*   �������� : public 
	*   �������� : ���ݲ�����
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


	/**************************************************************************

	��COM��

	*/
	HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);

	/**************************************************************************

	�ر�COM��

	*/
	void CloseComm();

	/**************************************************************************

	��COM��

	*/
	int ReadComm(void* rbuf, int buflen);

	/**************************************************************************

	дCOM��

	*/
	int WriteComm(void* wbuf, int buflen);


	/******************************************************************************/
	/**
	* ���ݽ����߳���ں�����
	*/
	static x_uint32_t __stdcall iocom_thread_entry(x_pvoid_t xpvt_param);

	/******************************************************************************/
	/**
	* ���ݽ������̣������̵߳��ã���
	*/
	x_void_t iocom_fncbk();

	// inner invoking
protected:

	// class data
protected:
	HWND					 m_xst_com_tid;     ///< ֪ͨ���߳� ID
	x_size_t                 m_xst_com_mid;     ///< ֪ͨ���߳� ��Ϣ��ʶ
	x_pvoid_t                m_xpvt_com_ctx;    ///< ֪ͨ���߳� ����������

	
	x_handle_t			pThread_COM;		//�����߳�TCP Server
	HANDLE				m_ComHandle;		//COM�ھ��

};


#endif 