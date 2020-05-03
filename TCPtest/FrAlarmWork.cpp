/*
* Copyright (c) 2014, 深圳市飞瑞斯科技有限公司
* All rights reserved.
* 
* 文件名称：FrAlarmWork.cpp
* 创建日期：2014年11月21日
* 文件标识：
* 文件摘要：告警操作接口。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2014年11月21日
* 版本摘要：
* 
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
* 
*/

#include "stdafx.h"
#include "FrAlarmWork.h"

///////////////////////////////////////////////////////////////////////////

/**************************************************************************
* FunctionName:
*     OpenComm
* Description:
*     打开串口。
* Parameter:
*     @[in ] szComPort: 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者。
*     @[in ] nBaudRate: 波特率。
*     @[in ] nParity: 奇偶校验。
*     @[in ] nByteSize: 数据字节宽度。
*     @[in ] nStopBits: 停止位。
* ReturnValue:
*     成功，返回 串口操作句柄；失败，返回 INVALID_HANDLE_VALUE。
*/
HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	HANDLE hComm = INVALID_HANDLE_VALUE;

	DCB dcbCom;		// 串口控制块

	// 串口超时控制参数
	COMMTIMEOUTS timeouts =
	{
		100,    // 读字符间隔超时时间: 100 ms
		1,		// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
		500,	// 基本的(额外的)读超时时间: 500 ms
		1,		// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
		100     // 基本的(额外的)写超时时间: 100 ms
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
		return INVALID_HANDLE_VALUE;
	}

	// 获取取 DCB
	if (!GetCommState(hComm, &dcbCom))
	{
		CloseHandle(hComm);
		return INVALID_HANDLE_VALUE;
	}

	dcbCom.BaudRate = nBaudRate;
	dcbCom.ByteSize = nByteSize;
	dcbCom.Parity   = nParity;
	dcbCom.StopBits = nStopBits;

	SetCommState(hComm, &dcbCom);       // 设置DCB
	SetupComm(hComm, 4096, 1024);       // 设置输入输出缓冲区大小
	SetCommTimeouts(hComm, &timeouts);  // 设置超时

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
void CloseComm(HANDLE hComm)
{
	CloseHandle(hComm);
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
*     串口读取数据。
* Parameter:
*     @[in ] hComm: 串口操作句柄。
*     @[in ] rbuf: 读取数据的接收缓存。
*     @[in ] buflen: 读取数据的接收缓存长度。
* ReturnValue:
*     实际读出的数据长度
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
*     开启告警。
* Parameter:
*     @[in ] szComPort: 告警输出 COM 端口号。
* ReturnValue:
*     返回 告警 操作句柄（返回 INVALID_HANDLE_VALUE，表示操作失败）。
*/
HANDLE AlarmStart(LPCTSTR szComPort)
{
	HANDLE hAlarm = INVALID_HANDLE_VALUE;

	const int nDelayTime = 50;   // 操作间隔时间值

	BYTE rAlarmResponse[256];     // 应答读取缓存
	int  rAlarmResponseLen = 0;   // 应答读取缓存的数据长度

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

	// 打开串口设备
	hAlarm = OpenComm(szComPort, CBR_9600, NOPARITY, 8, ONE5STOPBITS);
	if (INVALID_HANDLE_VALUE == hAlarm)
	{
		return INVALID_HANDLE_VALUE;
	}

	//=========================================================================

	// 写入命令 A
	WriteComm(hAlarm, wAlarmCmdA, wAlarmCmdA_size);

	// 暂停
	Sleep(nDelayTime);

	// 读取命令 A 的应答
	rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
	if ((rAlarmCmdA_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdA, rAlarmResponse, rAlarmCmdA_size)))
	{
		AlarmStop(hAlarm);
		return INVALID_HANDLE_VALUE;
	}

	//=========================================================================

	// 写入命令 B
	WriteComm(hAlarm, wAlarmCmdB, wAlarmCmdB_size);

	// 暂停
	Sleep(nDelayTime);

	// 读取命令 B 的应答
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
*     停止告警。
* Parameter:
*     @[in ] hAlarm: 告警操作句柄。
* ReturnValue:
*     void
*/
void AlarmStop(HANDLE hAlarm)
{
	const int nDelayTime = 50;    // 操作间隔时间值

	BYTE rAlarmResponse[256];     // 应答读取缓存
	int  rAlarmResponseLen = 0;   // 应答读取缓存的数据长度

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

		// 写入命令 A
		WriteComm(hAlarm, wAlarmCmdA, wAlarmCmdA_size);

		// 暂停
		Sleep(nDelayTime);

		// 读取命令 A 的应答
		rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
		if ((rAlarmCmdA_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdA, rAlarmResponse, rAlarmCmdA_size)))
		{
			// ...
		}

		//=========================================================================

		// 写入命令 B
		WriteComm(hAlarm, wAlarmCmdB, wAlarmCmdB_size);

		// 暂停
		Sleep(nDelayTime);

		// 读取命令 B 的应答
		rAlarmResponseLen = ReadComm(hAlarm, rAlarmResponse, sizeof(rAlarmResponse));
		if ((rAlarmCmdB_size != rAlarmResponseLen) || (0 != memcmp(rAlarmCmdB, rAlarmResponse, rAlarmCmdB_size)))
		{
			// ...
		}

	} while (0);

	CloseComm(hAlarm);
}


