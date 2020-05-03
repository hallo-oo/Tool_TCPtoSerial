/*
* Copyright (c) 2014, 深圳市飞瑞斯科技有限公司
* All rights reserved.
* 
* 文件名称：FrAlarmWork.h
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

#ifndef __FRALARMWORK_H__
#define __FRALARMWORK_H__

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
HANDLE AlarmStart(LPCTSTR szComPort);

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
void AlarmStop(HANDLE hAlarm);

/**************************************************************************

打开COM口

*/
HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);

/**************************************************************************

关闭COM口

*/
void CloseComm(HANDLE hComm);

/**************************************************************************

读COM口

*/
int ReadComm(HANDLE hComm, void* rbuf, int buflen);

/**************************************************************************

写COM口

*/
int WriteComm(HANDLE hComm, void* wbuf, int buflen);


///////////////////////////////////////////////////////////////////////////

#endif // __FRALARMWORK_H__


