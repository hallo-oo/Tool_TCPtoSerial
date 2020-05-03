/*
* Copyright (c) 2014, �����з���˹�Ƽ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�FrAlarmWork.cpp
* �������ڣ�2014��11��21��
* �ļ���ʶ��
* �ļ�ժҪ���澯�����ӿڡ�
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2014��11��21��
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
* 
*/

#include "stdafx.h"
#include "FrAlarmWork.h"

///////////////////////////////////////////////////////////////////////////

/**************************************************************************
* FunctionName:
*     OpenComm
* Description:
*     �򿪴��ڡ�
* Parameter:
*     @[in ] szComPort: �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú��ߡ�
*     @[in ] nBaudRate: �����ʡ�
*     @[in ] nParity: ��żУ�顣
*     @[in ] nByteSize: �����ֽڿ�ȡ�
*     @[in ] nStopBits: ֹͣλ��
* ReturnValue:
*     �ɹ������� ���ڲ��������ʧ�ܣ����� INVALID_HANDLE_VALUE��
*/
HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	HANDLE hComm = INVALID_HANDLE_VALUE;

	DCB dcbCom;		// ���ڿ��ƿ�

	// ���ڳ�ʱ���Ʋ���
	COMMTIMEOUTS timeouts =
	{
		100,    // ���ַ������ʱʱ��: 100 ms
		1,		// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		500,	// ������(�����)����ʱʱ��: 500 ms
		1,		// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		100     // ������(�����)д��ʱʱ��: 100 ms
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
		return INVALID_HANDLE_VALUE;
	}

	// ��ȡȡ DCB
	if (!GetCommState(hComm, &dcbCom))
	{
		CloseHandle(hComm);
		return INVALID_HANDLE_VALUE;
	}

	dcbCom.BaudRate = nBaudRate;
	dcbCom.ByteSize = nByteSize;
	dcbCom.Parity   = nParity;
	dcbCom.StopBits = nStopBits;

	SetCommState(hComm, &dcbCom);       // ����DCB
	SetupComm(hComm, 4096, 1024);       // �������������������С
	SetCommTimeouts(hComm, &timeouts);  // ���ó�ʱ

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
void CloseComm(HANDLE hComm)
{
	CloseHandle(hComm);
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
int WriteComm(HANDLE hComm, void* wbuf, int buflen)
{
	DWORD dwNumWrite;
	WriteFile(hComm, wbuf, (DWORD)buflen, &dwNumWrite, NULL);

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
int ReadComm(HANDLE hComm, void* rbuf, int buflen)
{
	DWORD dwNumRead;
	ReadFile(hComm, rbuf, (DWORD)buflen, &dwNumRead, NULL);
	return (int)dwNumRead;
}

///////////////////////////////////////////////////////////////////////////

/**************************************************************************
* FunctionName:
*     AlarmStart
* Description:
*     �����澯��
* Parameter:
*     @[in ] szComPort: �澯��� COM �˿ںš�
* ReturnValue:
*     ���� �澯 ������������� INVALID_HANDLE_VALUE����ʾ����ʧ�ܣ���
*/
HANDLE AlarmStart(LPCTSTR szComPort)
{
	HANDLE hAlarm = INVALID_HANDLE_VALUE;

	const int nDelayTime = 50;   // �������ʱ��ֵ

	BYTE rAlarmResponse[256];     // Ӧ���ȡ����
	int  rAlarmResponseLen = 0;   // Ӧ���ȡ��������ݳ���

	static BYTE wAlarmCmdA[64] = 
	{
		0x01, 0x10, 0x03, 0x00, 0x00, 0x04, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA3, 0x79
	};
	const int wAlarmCmdA_size = 17;

	static BYTE rAlarmCmdA[64] = 
	{
		0x01, 0x10, 0x03, 0x00, 0x00, 0x04, 0xC1, 0x8E
	};
	const int rAlarmCmdA_size = 8;

	static BYTE wAlarmCmdB[64] = 
	{
		0x01, 0x10, 0x03, 0x04, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x76
	};
	const int wAlarmCmdB_size = 17;

	static BYTE rAlarmCmdB[64] = 
	{
		0x01, 0x10, 0x03, 0x04, 0x00, 0x04, 0x80, 0x4F
	};
	const int rAlarmCmdB_size = 8;

	// �򿪴����豸
	hAlarm = OpenComm(szComPort, CBR_9600, NOPARITY, 8, ONE5STOPBITS);
	if (INVALID_HANDLE_VALUE == hAlarm)
	{
		return INVALID_HANDLE_VALUE;
	}

	//=========================================================================

	// д������ A
	WriteComm(hAlarm, wAlarmCmdA, wAlarmCmdA_size);

	// ��ͣ
	Sleep(nDelayTime);

	// ��ȡ���� A ��Ӧ��
	rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
	if ((rAlarmCmdA_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdA, rAlarmResponse, rAlarmCmdA_size)))
	{
		AlarmStop(hAlarm);
		return INVALID_HANDLE_VALUE;
	}

	//=========================================================================

	// д������ B
	WriteComm(hAlarm, wAlarmCmdB, wAlarmCmdB_size);

	// ��ͣ
	Sleep(nDelayTime);

	// ��ȡ���� B ��Ӧ��
	rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
	if ((rAlarmCmdB_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdB, rAlarmResponse, rAlarmCmdB_size)))
	{
		AlarmStop(hAlarm);
		return INVALID_HANDLE_VALUE;
	}

	//=========================================================================

	return hAlarm;
}

/**************************************************************************
* FunctionName:
*     AlarmClose
* Description:
*     ֹͣ�澯��
* Parameter:
*     @[in ] hAlarm: �澯���������
* ReturnValue:
*     void
*/
void AlarmStop(HANDLE hAlarm)
{
	const int nDelayTime = 50;    // �������ʱ��ֵ

	BYTE rAlarmResponse[256];     // Ӧ���ȡ����
	int  rAlarmResponseLen = 0;   // Ӧ���ȡ��������ݳ���

	static BYTE wAlarmCmdA[64] = 
	{
		0x01, 0x10, 0x03, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB3, 0xB9
	};
	const int wAlarmCmdA_size = 17;

	static BYTE rAlarmCmdA[64] = 
	{
		0x01, 0x10, 0x03, 0x00, 0x00, 0x04, 0xC1, 0x8E
	};
	const int rAlarmCmdA_size = 8;

	static BYTE wAlarmCmdB[64] = 
	{
		0x01, 0x10, 0x03, 0x04, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x76
	};
	const int wAlarmCmdB_size = 17;

	static BYTE rAlarmCmdB[64] = 
	{
		0x01, 0x10, 0x03, 0x04, 0x00, 0x04, 0x80, 0x4F
	};
	const int rAlarmCmdB_size = 8;

	if (INVALID_HANDLE_VALUE == hAlarm)
	{
		return;
	}

	do 
	{
		//=========================================================================

		// д������ A
		WriteComm(hAlarm, wAlarmCmdA, wAlarmCmdA_size);

		// ��ͣ
		Sleep(nDelayTime);

		// ��ȡ���� A ��Ӧ��
		rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
		if ((rAlarmCmdA_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdA, rAlarmResponse, rAlarmCmdA_size)))
		{
			// ...
		}

		//=========================================================================

		// д������ B
		WriteComm(hAlarm, wAlarmCmdB, wAlarmCmdB_size);

		// ��ͣ
		Sleep(nDelayTime);

		// ��ȡ���� B ��Ӧ��
		rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
		if ((rAlarmCmdB_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdB, rAlarmResponse, rAlarmCmdB_size)))
		{
			// ...
		}

	} while (0);

	CloseComm(hAlarm);
}


