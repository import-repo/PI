
// UI-DialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UI-Dialog.h"
#include "UI-DialogDlg.h"
#include "afxdialogex.h"
#include <SOIL/SOIL.h>
#include <IJZBaseImageProcessProc.h>
#include <IJZBaseRenderProc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "opengl32.lib")
#ifdef _DEBUG
#	ifdef _WIN64
#		pragma comment(lib, "SOIL64s_d.lib")
#	else
#		pragma comment(lib, "SOIL32s_d.lib")
#	endif
#else
#	ifdef _WIN64
#		pragma comment(lib, "SOIL64s.lib")
#	else
#		pragma comment(lib, "SOIL32s.lib")
#	endif
#endif // _DEBUG

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIDialogDlg 对话框



CUIDialogDlg::CUIDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UIDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUIDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUIDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CUIDialogDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CUIDialogDlg::OnBnClickedButtonClear)
//	ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUIDialogDlg 消息处理程序

BOOL CUIDialogDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	IJZBaseImageProcessAPI* pAPI = NULL;
	JZ_RESULT res = g_JZBaseImageProcessAPI->pfnGetInterface(&pAPI);
	JZImageBuf src = { 0 };
	JZImageBuf des = { 0 };
	JZCommonParam param;
	res = pAPI->ReadImage("../../sys/images/test.jpg", NULL);
	res = g_JZBaseImageProcessAPI->pfnReleaseInterface(pAPI);

	// TODO: 在此添加额外的初始化代码
	/*HMODULE hDLL = LoadLibrary(_T("D:\\GitCode\\PI\\dll\\x64\\Debug\\JZBaseRenderd.dll"));
	typedef IJZScene* (*GetSceneAPI)();
	GetSceneAPI pfn = (GetSceneAPI)GetProcAddress(hDLL, "GetSceneAPI");
	m_pScene = pfn();*/

	m_pScene = NULL;
	g_JZBaseRenderAPI->pfnGetSceneInterface(&m_pScene);
	CWnd* cwnd = GetDlgItem(IDC_STATIC_PIC);

	m_pScene->SetDevice(cwnd->m_hWnd);
	m_pScene->PrepareData();
	

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUIDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUIDialogDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		m_pScene->RenderScene();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUIDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUIDialogDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	int texWidth = 1;
	int texHeight = 1;
	unsigned char* image = SOIL_load_image("../../sys/images/awesomeface.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	JZImageBuf imageBuf = { 0 };
	imageBuf.color = image;
	imageBuf.pitch = texWidth * 3;
	imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
	imageBuf.width = texWidth;
	imageBuf.height = texHeight;
	m_pScene->SetImage(&imageBuf);
	SOIL_free_image_data(image);
	m_pScene->SetResStatus(true);
}


void CUIDialogDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	m_pScene->SetImage(NULL);
	m_pScene->SetResStatus(true);
}


//void CUIDialogDlg::OnClose()
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialogEx::OnClose();
//}


void CUIDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	g_JZBaseRenderAPI->pfnReleaseSceneInterface(m_pScene);
}
