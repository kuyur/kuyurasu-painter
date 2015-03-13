// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public: // 仅从序列化创建
	CMainFrame();
protected:
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:
	void InitToolBar();
	void ShowCoordinate(CPoint cPoint);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar    m_wndStatusBar;        //状态栏
	CToolBar      m_wndToolBar;          //工具栏
	CChildView*   m_wndView;             //工作子窗口，修改为指针类型才不出错，天呐
	CImageList    m_imageToolBar;        //真彩工具栏
	CImageList    m_imageToolBarDisable; //无效时
	CImageList    m_imageToolBarHot;     //触发时

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
};


