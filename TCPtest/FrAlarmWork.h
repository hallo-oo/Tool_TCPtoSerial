/*
* Copyright (c) 2014, �����з���˹�Ƽ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�FrAlarmWork.h
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

#ifndef __FRALARMWORK_H__
#define __FRALARMWORK_H__

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
HANDLE AlarmStart(LPCTSTR szComPort);

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
void AlarmStop(HANDLE hAlarm);

/**************************************************************************

��COM��

*/
HANDLE OpenComm(LPCTSTR szComPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);

/**************************************************************************

�ر�COM��

*/
void CloseComm(HANDLE hComm);

/**************************************************************************

��COM��

*/
int ReadComm(HANDLE hComm, void* rbuf, int buflen);

/**************************************************************************

дCOM��

*/
int WriteComm(HANDLE hComm, void* wbuf, int buflen);


///////////////////////////////////////////////////////////////////////////

#endif // __FRALARMWORK_H__


