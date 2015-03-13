#include "stdafx.h"
#include "Dialog.h"

void DialogPageOne::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDV_MinMaxFloat(pDX, m_x, 0, 1.00);
	DDV_MinMaxFloat(pDX, m_y, 0, 1.00);
}

void DialogPageTwo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxLong(pDX, m_width, 10, 1600);   //宽度上限1600，下限10
	DDV_MinMaxLong(pDX, m_height, 10, 1200);  //高度上限1200，下限10
}

BOOL  DialogPageThree::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_penSize=(CComboBox*)GetDlgItem(IDC_COMBO_PEN);
	if (!m_penSize)
		return FALSE;
	m_penSize->AddString((CString)"1px");
	m_penSize->AddString((CString)"2px");
	m_penSize->AddString((CString)"3px");
	m_penSize->AddString((CString)"4px");
	m_penSize->AddString((CString)"5px");
	m_penSize->AddString((CString)"6px");
	m_penSize->AddString((CString)"7px");
	m_penSize->AddString((CString)"8px");
	m_penSize->AddString((CString)"9px");
	m_penSize->SetCurSel(m_pensize);

	return TRUE;
}
void DialogPageThree::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_PEN, m_pensize);
}
BEGIN_MESSAGE_MAP(DialogPageThree, CPropertyPage)
END_MESSAGE_MAP()

//多页对话框
DialogSheet::DialogSheet(CWnd *pParent):CPropertySheet(L"选项",pParent)
{
}
BEGIN_MESSAGE_MAP(DialogSheet, CPropertySheet)
END_MESSAGE_MAP()
