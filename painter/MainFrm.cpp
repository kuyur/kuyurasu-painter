// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "painter.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_COOR,      // 坐标指示 index1
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndView = new CChildView;
	// 创建一个视图以占用框架的工作区
	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	InitToolBar(); //初始化工具栏

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_COOR,SBPS_POPOUT,130);

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	//  CREATESTRUCT cs 来修改窗口类或样式
	//获得屏幕大小
	CSize ScreenSize(::GetSystemMetrics(SM_CXSCREEN),::GetSystemMetrics(SM_CYSCREEN));
	if ((ScreenSize.cx<=1024)|(ScreenSize.cy<=768))
		cs.style |= WS_MAXIMIZE;
	else
	{
		cs.cx=1024;
		cs.cy=768;
	}
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE; //~逐位取反
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::InitToolBar()
{
    CBitmap bm;

    //活动的工具
    m_imageToolBar.Create( 16, 15, TRUE | ILC_COLOR24, 5, 0 );
    bm.LoadBitmap( IDB_TOOLBARCOLOR );
    m_imageToolBar.Add( &bm,(CBitmap*)NULL );
    bm.Detach();
    m_wndToolBar.GetToolBarCtrl().SetImageList( &m_imageToolBar );

    //禁止的工具
    m_imageToolBarDisable.Create( 16, 15, TRUE | ILC_COLOR24, 5, 0 );
    bm.LoadBitmap( IDB_TOOLBARDISABLE );
    m_imageToolBarDisable.Add( &bm,(CBitmap*)NULL );
    bm.Detach();
    m_wndToolBar.GetToolBarCtrl().SetDisabledImageList( &m_imageToolBarDisable );

    //当前的工具
    m_imageToolBarHot.Create( 16, 15, TRUE | ILC_COLOR24, 5, 0 ); 
    bm.LoadBitmap( IDB_TOOLBARHOT );
    m_imageToolBarHot.Add( &bm,(CBitmap*)NULL );
    bm.Detach();
    m_wndToolBar.GetToolBarCtrl().SetHotImageList( &m_imageToolBarHot );
}

//显示客户区坐标值（由子窗口调用）
void CMainFrame::ShowCoordinate(CPoint cPoint)
{
	CString text,x,y;
	x.Format((CString)"%d",cPoint.x);
	y.Format((CString)"%d",cPoint.y);
	text=(CString)"Coordinate: X "+x+(CString)",Y "+y;
	m_wndStatusBar.SetPaneText(1,text,TRUE);
}