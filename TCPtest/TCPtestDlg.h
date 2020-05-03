
// TCPtestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

//#include "FrAlarmWork.h"
#include "TCP_SERVER.h"
#include "TCP_COM.h"
#include "FrAlarmWork.h"
#define WM_RECVDATA WM_USER+1
#define WM_COMRECVDATA WM_USER+2
// CTCPtestDlg 对话框
class CTCPtestDlg : public CDialogEx
{
// 构造
public:
	CTCPtestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	HANDLE COMFile;			//串口句柄
	CWinThread* pThread_COM;	//定义线程COM口
	
	
protected:
	CComboBox m_wndCmbPort;		///< COM口数量
	int m_nComArray[20];		///< 临时变量
	CComboBox m_wndCmbBaud;		///< 波特率
	BOOL bOpenFlag;			///< 串口开关标志位


	
	int		Recv_LEN;		///< 返回字节长度

	
};
