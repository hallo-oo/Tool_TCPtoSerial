
// TCPtestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPtest.h"
#include "TCPtestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HANDLE hTEMP=NULL;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

//UINT ThreadFunc(LPVOID lpParam);	//线程函数说明


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTCPtestDlg 对话框




CTCPtestDlg::CTCPtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPtestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PORT, m_wndCmbPort);
	DDX_Control(pDX, IDC_CMB_BAUD, m_wndCmbBaud);
}




// CTCPtestDlg 消息处理程序



void CTCPtestDlg::OnDestroy()
{
	CloseHandle(COMFile);
	CloseHandle(hTEMP);
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}




BOOL CTCPtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	//======================================
	// 读可用取COM数目

	GetComList_Reg( &m_wndCmbPort );

	//======================================
	// 波特率初始化

	//m_wndCmbBaud.InsertString(0,  _T ("1200"));
	//m_wndCmbBaud.InsertString(1,  _T ("2400"));
	//m_wndCmbBaud.InsertString(2,  _T ("4800"));
	//m_wndCmbBaud.InsertString(3,  _T ("9600"));
	//m_wndCmbBaud.InsertString(4,  _T ("14400"));
	//m_wndCmbBaud.InsertString(5,  _T ("19200"));
	//m_wndCmbBaud.InsertString(6,  _T ("38400"));
	//m_wndCmbBaud.InsertString(7,  _T ("43000"));
	//m_wndCmbBaud.InsertString(8,  _T ("56000"));
	//m_wndCmbBaud.InsertString(9,  _T ("57600"));
	//m_wndCmbBaud.InsertString(10, _T ("115200"));
	//m_wndCmbBaud.InsertString(11, _T ("128000"));
	//m_wndCmbBaud.InsertString(12, _T ("230400"));
	//m_wndCmbBaud.InsertString(13, _T ("256000"));
	
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("1200"		)), 1200);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("2400"		)), 2400);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("4800"		)), 4800);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("9600"     )), 9600);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("14400"    )), 14400);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("19200"    )), 19200);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("38400"	)), 38400);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("43000"	)), 43000);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("56000"    )), 56000);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("57600"    )), 57600);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("115200"   )), 115200);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("128000"   )), 128000);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("230400"	)), 230400);
	m_wndCmbBaud.SetItemData(m_wndCmbBaud.AddString(TEXT("256000"	)), 256000);
	


	
	m_wndCmbBaud.SetCurSel(3);

	//======================================
	// 串口开关标志位初始化
	//bOpenFlag=0;
	////======================================
	//// 串口句柄初始化
	//COMFile=NULL;

	//======================================
	//开启读取COM口线程

	//pThread_COM=AfxBeginThread(ThreadFunc,COMFile,0,0);
	

	//======================================
	// TCP初始化
	
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLOSETCP)->EnableWindow(FALSE);
	//======================================
	// 参数传递，
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	m_TcpServer.start(hWnd, WM_RECVDATA, this);
	m_TcpCom.start(hWnd, WM_COMRECVDATA, this);

	//======================================
	

	//======================================
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTCPtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





int CTCPtestDlg::GetComList_Reg( CComboBox * CCombox )
{
	HKEY hkey;  
	int result;
	int i = 0;

	CString strComName;//串口名称
	CString strDrName;//串口详细名称

	result = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T( "Hardware\\DeviceMap\\SerialComm" ),
		NULL,
		KEY_READ,
		&hkey );

	if( ERROR_SUCCESS == result )   //   打开串口注册表   
	{   
		TCHAR portName[ 0x100 ], commName[ 0x100 ];
		DWORD dwLong, dwSize;

		CCombox->ResetContent();
		do
		{   
			dwSize = sizeof( portName ) / sizeof( TCHAR );
			dwLong = dwSize;
			result = RegEnumValue( hkey, i, portName, &dwLong, NULL, NULL, ( LPBYTE )commName, &dwSize );
			if( ERROR_NO_MORE_ITEMS == result )
			{
				//   枚举串口
				break;   //   commName就是串口名字"COM2"
			}

			strComName=commName;
			strDrName = portName;

			// 从右往左边开始查找第一个'\\'，获取左边字符串的长度
			int len = strDrName.ReverseFind( '\\');
			// 获取'\\'左边的字符串
			CString strFilePath = strDrName.Left( len+1 );
			// 获取'\\'右边的字符串
			//CString fileName = strDrName.Right(strDrName.GetLength() - len-1);
			//fileName = strComName + _T(": ")+fileName;

			//CCombox->AddString(fileName);
			CCombox->InsertString(i,strComName);
			i++;   
		} while ( 1 );

		RegCloseKey( hkey );   
	}

	CCombox->SetCurSel(0);
	return i;
}



//UINT ThreadFunc(LPVOID lpParam)
//{
//	int  ReadCOMLen = 0;   // 应答读取缓存的数据长度
//	BYTE ComResponse[256];     // 应答读取缓存
//	int CmdLen=10;
//	static BYTE RCmdA[10] = 
//	{
//		0x55, 0x0A, 0x01, 0x00, 0x00, 0x2A, 0x00, 0x00 ,0x00 ,0xFF
//	};
//
//	while (1)
//	{
//		if (hTEMP!=NULL)
//		{
//
//			ReadCOMLen = ReadComm(hTEMP, ComResponse, sizeof(ComResponse));
//			if (ReadCOMLen)
//			{
//
//				if ((CmdLen != ReadCOMLen) || (0 != memcmp(RCmdA, ComResponse, CmdLen)))
//				{
//					AfxMessageBox(_T("接收失败"));
//				}
//				else
//				{
//					AfxMessageBox(_T("接收成功"));
//				}
//
//				WriteComm(hTEMP, ComResponse, ReadCOMLen);
//				memset(ComResponse, 0, 256);
//			}
//		}
//	}
//}




BEGIN_MESSAGE_MAP(CTCPtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENCOM, &CTCPtestDlg::OnBnClickedBtnOpencom)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CTCPtestDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CLOSETCP, &CTCPtestDlg::OnBnClickedBtnClosetcp)
	ON_MESSAGE(WM_RECVDATA,&CTCPtestDlg::OnRecvData)
	ON_MESSAGE(WM_COMRECVDATA,&CTCPtestDlg::OnCOMRecvData)
END_MESSAGE_MAP()





void CTCPtestDlg::OnBnClickedBtnOpencom()
{
	// TODO: 打开/关闭 串口
	CString str;
	CString ComPort;
	int nBaudRate;
	ComPort.Format(_T("\\\\.\\"));
	m_wndCmbPort.GetLBText(m_wndCmbPort.GetCurSel(),str);
	ComPort=ComPort+str;

	nBaudRate=(int)m_wndCmbBaud.GetItemData(m_wndCmbBaud.GetCurSel());
	LPCTSTR hCom=ComPort.GetString();

	if (bOpenFlag==1)
	{
		SetDlgItemText(IDC_BTN_OPENCOM,_T("打开串口"));
		bOpenFlag=0;
		m_TcpCom.CloseComm();
		//CloseComm(COMFile);
		
		COMFile=NULL;
	}
	else
	{
		COMFile=m_TcpCom.OpenComm( hCom,  nBaudRate,  NOPARITY,  8,  ONE5STOPBITS);
		//COMFile=OpenComm( hCom,  nBaudRate,  NOPARITY,  8,  ONE5STOPBITS);
		hTEMP=COMFile;
		if(INVALID_HANDLE_VALUE != COMFile)
		{
			SetDlgItemText(IDC_BTN_OPENCOM,_T("关闭串口"));
			bOpenFlag=1;
		}
		else
			AfxMessageBox(_T("打开COM口失败"));
	}
}




void CTCPtestDlg::OnBnClickedBtnConnect()
{
	// 打开TCP服务器

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CLOSETCP)->EnableWindow(TRUE);
	m_TcpServer.open();
}


void CTCPtestDlg::OnBnClickedBtnClosetcp()
{
	// 关闭TCP服务器

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLOSETCP)->EnableWindow(FALSE);
	m_TcpServer.close();
}



LRESULT  CTCPtestDlg::OnRecvData(WPARAM wParam,LPARAM lparam)
{
	/*CString str;
	char *sendBuf;
	str=(char *)lparam;

	AfxMessageBox(str);

	sendBuf = (char*)str.GetBuffer(0);
	m_TcpServer.send_packet(sendBuf,100);*/

	
	TCP_Com::TCP_COM_DATA *COMRecvBuff=(TCP_Com::TCP_COM_DATA *)lparam;
	m_TcpCom.WriteComm(COMRecvBuff->TCP_COM_buff,COMRecvBuff->TCP_COM_len);
	
	return 1;
}


LRESULT  CTCPtestDlg::OnCOMRecvData(WPARAM wParam,LPARAM lparam)
{

	TCP_Server::TCP_DATA *RecvBuff=(TCP_Server::TCP_DATA *)lparam;
	
	m_TcpServer.send_packet(RecvBuff->TCP_buff,RecvBuff->TCP_len);

	return 1;
}
