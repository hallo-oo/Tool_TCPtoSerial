
// TCPtestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

//#include "FrAlarmWork.h"
#include "TCP_SERVER.h"
#include "TCP_COM.h"
#include "FrAlarmWork.h"
#define WM_RECVDATA WM_USER+1
#define WM_COMRECVDATA WM_USER+2
// CTCPtestDlg �Ի���
class CTCPtestDlg : public CDialogEx
{
// ����
public:
	CTCPtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCPTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnOpencom();


	afx_msg void OnBnClickedBtnConnect();
	

	afx_msg LRESULT  OnRecvData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  OnCOMRecvData(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	

public:
	afx_msg void OnBnClickedBtnClosetcp();
	int GetComList_Reg( CComboBox * CCombox );


	TCP_Server         m_TcpServer;
	TCP_Com			   m_TcpCom;
	HANDLE COMFile;			//���ھ��
	CWinThread* pThread_COM;	//�����߳�COM��
	
	
protected:
	CComboBox m_wndCmbPort;		///< COM������
	int m_nComArray[20];		///< ��ʱ����
	CComboBox m_wndCmbBaud;		///< ������
	BOOL bOpenFlag;			///< ���ڿ��ر�־λ


	
	int		Recv_LEN;		///< �����ֽڳ���

	
};
