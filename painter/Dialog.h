//对话框类总合

#pragma once
#include "Resource.h"
#include "afxwin.h"

//对话框1，圆角度设置
class DialogPageOne : public CPropertyPage
{
public:
	enum {IDD=IDD_PAGE1};
	float m_x;
	float m_y;

public:
	DialogPageOne():CPropertyPage(DialogPageOne::IDD)
	{
		m_x=0.0;
		m_y=0.0;
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};

//对话框2，画布设置
class DialogPageTwo : public CPropertyPage
{
public:
	enum {IDD=IDD_PAGE2};
	LONG m_width;
	LONG m_height;

public:
	DialogPageTwo():CPropertyPage(DialogPageTwo::IDD)
	{
		m_width=0;
		m_height=0;
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};

//对话框3，画笔设置
class DialogPageThree : public CPropertyPage
{
public:
	enum {IDD=IDD_PAGE3};
	int m_pensize;
	CComboBox *m_penSize;

public:
	DialogPageThree():CPropertyPage(DialogPageThree::IDD)
	{
		m_pensize=0;
	}
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP();
};

//多页对话框，综合选项
class DialogSheet : public CPropertySheet
{
public:
	DialogSheet(CWnd* pParent);
protected:
	DECLARE_MESSAGE_MAP();
};