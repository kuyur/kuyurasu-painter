// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "painter.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

IMPLEMENT_DYNCREATE(CChildView, CScrollView)

BEGIN_MESSAGE_MAP(CChildView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_UNDO, &CChildView::OnEditUndo)
	ON_COMMAND(ID_EDIT_SIZE, &CChildView::OnEditSize)
	ON_COMMAND(ID_EDIT_CLEAR, &CChildView::OnEditClear)
	ON_COMMAND(ID_EDIT_PEN, &CChildView::OnEditPen)
	ON_COMMAND(ID_EDIT_LINE, &CChildView::OnEditLine)
	ON_COMMAND(ID_EDIT_RECT, &CChildView::OnEditRect)
	ON_COMMAND(ID_EDIT_ROUNDRECT, &CChildView::OnEditRoundrect)
	ON_COMMAND(ID_EDIT_ELLIPSE, &CChildView::OnEditEllipse)
	ON_COMMAND(ID_EDIT_POLYGON, &CChildView::OnEditPolygon)
	ON_COMMAND(ID_EDIT_BEZIER, &CChildView::OnEditBezier)
	ON_COMMAND(ID_EDIT_COLOR, &CChildView::OnEditColor)
	ON_COMMAND(ID_EDIT_NOTFILLING, &CChildView::OnEditNotfilling)
	ON_COMMAND(ID_EDIT_ERASEFILLING, &CChildView::OnEditErasefilling)
	ON_COMMAND(ID_EDIT_FULLFILLING, &CChildView::OnEditFullfilling)
	ON_COMMAND(ID_EDIT_ROUNDDEGREE, &CChildView::OnEditRounddegree)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CChildView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NOTFILLING, &CChildView::OnUpdateEditNotfilling)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ERASEFILLING, &CChildView::OnUpdateEditErasefilling)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FULLFILLING, &CChildView::OnUpdateEditFullfilling)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PEN, &CChildView::OnUpdateEditPen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LINE, &CChildView::OnUpdateEditLine)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RECT, &CChildView::OnUpdateEditRect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ROUNDRECT, &CChildView::OnUpdateEditRoundrect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ELLIPSE, &CChildView::OnUpdateEditEllipse)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POLYGON, &CChildView::OnUpdateEditPolygon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BEZIER, &CChildView::OnUpdateEditBezier)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ROUNDDEGREE, &CChildView::OnUpdateEditRounddegree)
	ON_COMMAND(ID_PAINTFILE_OPEN, &CChildView::OnPaintFileOpen)
	ON_COMMAND(ID_PAINTFILE_SAVE, &CChildView::OnPaintFileSave)
	ON_COMMAND(ID_EDIT_OPTION, &CChildView::OnEditOption)
	ON_COMMAND(ID_EDIT_PENSIZE, &CChildView::OnEditPensize)
	ON_COMMAND(ID_IMAGE_UDREVERSE, &CChildView::OnImageUdreverse)
	ON_COMMAND(ID_IMAGE_LRREVERSE, &CChildView::OnImageLrreverse)
	ON_COMMAND(ID_IMAGE_CORREVERSE, &CChildView::OnImageCorreverse)
END_MESSAGE_MAP()

// CChildView 构造/析构

CChildView::CChildView()
{
	//初始化
	currentColor=RGB(0,0,0);
	currentWidth=1;
	//mouseCur=AfxGetApp()->LoadCursor(IDC_CURSOR1);
	drawType=LINE;
	drawMode=NOTFILLING;
	mouseLDown=CPoint(0,0);
	mouseLUp=CPoint(0,0);
	pCBitmap=NULL;
	hBitmap=NULL;
	tmpBitmap=NULL;
	DDBSize=CSize(800,600); //画布默认大小：800x600
	bFlag=FALSE;
	bDrawDrag=FALSE;
	DragDC=NULL;
	MemDC=NULL;
	DragPen=NULL;
	DragBrush=NULL;
	percentX=0.20;
	percentY=0.20;
	bDrawPolygon=FALSE;
	isUndo=FALSE;
	bTracePolygon=FALSE;
	WndportCenter=CPoint(0,0);
	ViewportOrg=CPoint(0,0);
	DragBezier=FALSE;
	DragBezierPos=NULL;
	DragBezierLoc=4;

	m_pDocument=NULL;
}

CChildView::~CChildView()
{
	if (DragPen)
	{
		delete DragPen;
		DragPen=NULL;
	}
	if (DragBrush)
	{
		delete DragBrush;
		DragBrush=NULL;
	}
	if (DragDC)
	{
		delete DragDC;
		DragDC=NULL;
	}
	if (MemDC)
	{
		delete MemDC;
		MemDC=NULL;
	}
	if (pCBitmap)
	{
		delete pCBitmap;
		pCBitmap=NULL;
	}
	m_pDocument=NULL;
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CScrollView::PreCreateWindow(cs))
		return FALSE;

	//初始化鼠标指针
	//mouseCur=::LoadCursor(NULL,IDC_ARROW);
	mouseCur=AfxGetApp()->LoadCursor(IDC_CURSOR1);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	//背景颜色:灰色(COLOR_WINDOW+12)
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		mouseCur, reinterpret_cast<HBRUSH>(COLOR_WINDOW+12), NULL);

	return TRUE;
}

void CChildView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, DDBSize);
}

// CChildView 绘制

void CChildView::OnDraw(CDC *pDC)
{
	//取得中心点坐标（窗口坐标系）
	WndportCenter=returnDrawRect().CenterPoint();
	//取得视口原点（窗口坐标系）
	ViewportOrg=CPoint(0,0);
	pDC->DPtoLP(&ViewportOrg);

	if (!bFlag)
	{
		//首次执行或者收到清空命令
		bFlag=TRUE;
		if (hBitmap!=NULL)
		{
			::DeleteObject(hBitmap);
		}
		hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC,DDBSize.cx,DDBSize.cy);
		if (tmpBitmap==NULL)
			tmpBitmap = ::CreateCompatibleBitmap(pDC->m_hDC,DDBSize.cx,DDBSize.cy);
		CDC dcToDDB;
		dcToDDB.CreateCompatibleDC(pDC);
		::SelectObject(dcToDDB.m_hDC,hBitmap);
		//清空
		ClearCanvas(&dcToDDB);
		//显示
		pDC->BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcToDDB,0,0,SRCCOPY);
		//::BitBlt(dcToDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dc.m_hDC,returnDrawRect().left,returnDrawRect().top,SRCCOPY);
		dcToDDB.DeleteDC(); //记得一定要删除对象！
		//CBitmap *pBufferDDB = CBitmap::FromHandle(hBitmap);
	}
	else
	{
		//将bufferDDB显示到屏幕
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap); //句柄转化为指针
		CDC dcFromDDB;
		dcFromDDB.CreateCompatibleDC(pDC);
		dcFromDDB.SelectObject(pBufferDDB);
		pDC->BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcFromDDB,0,0,SRCCOPY);
		//::BitBlt(dc.m_hDC,returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,dcFromDDB.m_hDC,0,0,SRCCOPY);
		//释放DC资源
		dcFromDDB.DeleteDC();
	}
}

// CChildView 打印
BOOL CChildView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}
void CChildView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CChildView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

//dc2logic和logic2dc应成对使用，以免发生错误
//坐标点转换，窗口坐标到逻辑坐标
void CChildView::dc2logic(CPoint &cPoint)
{
	int x,y;
	x= cPoint.x-WndportCenter.x;
	y=-cPoint.y+WndportCenter.y;
	cPoint=CPoint(x,y);
}
//坐标点转换，逻辑坐标到窗口坐标
void CChildView::logic2dc(CPoint &cPoint)
{
	int x,y;
	x= cPoint.x+WndportCenter.x;
	y=-cPoint.y+WndportCenter.y;
	cPoint=CPoint(x,y);
}
//坐标点转换，逻辑坐标到虚拟dc坐标
CPoint CChildView::logic2virtualDC(CPoint cPoint)
{
	int x,y;
	x= cPoint.x+DDBSize.cx/2;
	y=-cPoint.y+DDBSize.cy/2;
	return CPoint(x,y);
}
//坐标点转换，窗口坐标到虚拟dc坐标
CPoint CChildView::dc2virtualDC(CPoint cPoint)
{
	int x,y;
	x= cPoint.x-WndportCenter.x+DDBSize.cx/2;
	y= cPoint.y-WndportCenter.y+DDBSize.cy/2;
	return CPoint(x,y);
}
//坐标点转换，视口坐标到逻辑坐标
CPoint CChildView::view2logic(CPoint cPoint)
{
	int x,y;
	x= cPoint.x+ViewportOrg.x-WndportCenter.x;
	y=-cPoint.y-ViewportOrg.y+WndportCenter.y;
	return CPoint(x,y);
}

//是否命中控制点（贝塞尔曲线专用）
BOOL CChildView::HitCtrlPoint(CPoint HitPoint)
{
	BOOL NotHit=FALSE;
	if (!PointList.IsEmpty())
	{
		//命中PointList中的一个控制点，返回TRUE
		POSITION pos;
		pos=PointList.GetHeadPosition();
		for (int i=0;i<4;i++)
		{
			if (pos!=NULL)
			{
				CPoint ctrlpoint;
				DragBezierPos=pos;
				DragBezierLoc=i;
				ctrlpoint=PointList.GetNext(pos); //pos已经被修改，指向下一个元素
				if ((abs(HitPoint.x-ctrlpoint.x)<=2)&&(abs(HitPoint.y-ctrlpoint.y)<=2))
				{
					return TRUE;
				}
			}
		}
	}
	DragBezierPos=NULL;
	DragBezierLoc=4;
	return NotHit;
}

//按下鼠标左键
void CChildView::OnLButtonDown(UINT nFlags,CPoint point)
{
	CScrollView::OnLButtonDown(nFlags,point);
	bDrawDrag=TRUE;
	if (DragPen)
	{
		delete DragPen;
		DragPen=NULL;
	}
	if (DragBrush)
	{
		delete DragBrush;
		DragBrush=NULL;
	}
	if (DragDC)
	{
		delete DragDC;
		DragDC=NULL;
	}
	if (MemDC)
	{
		delete MemDC;
		MemDC=NULL;
	}
	if (pCBitmap)
	{
		delete pCBitmap;
		pCBitmap=NULL;
	}

	DragDC = new CClientDC(this);
	CScrollView::OnPrepareDC(DragDC);
	DragDC->DPtoLP(&point); //视口->窗口
	mouseLDown=point;
	mouseLUp=point;
	DragDC->SetROP2(R2_NOTXORPEN);    //MS的作画模式定义真是神奇
	DragBrush = new CBrush();
	if (drawMode==NOTFILLING)
		DragBrush->CreateStockObject(NULL_BRUSH);
	else if (drawMode==ERASEFILLING)
		DragBrush->CreateSolidBrush(RGB(255,255,255));
	else if (drawMode==FULLFILLING)
		DragBrush->CreateSolidBrush(currentColor);
	else
		DragBrush->CreateStockObject(NULL_BRUSH);
	DragPen= new CPen(PS_SOLID,currentWidth,currentColor);
	switch (drawType)
	{
		case PEN:
			//画笔模式颇为特殊，是在鼠标左键按下时移动过程中完成作画
			//如果使用SetPixel函数描点，由于Windows采样鼠标移动中的坐标频率不够高，得到的结果是断续的离散点
			//因此采用连线方式来进行作画
			DragDC->SetROP2(R2_COPYPEN);
			DragDC->SelectObject(DragPen);
			DragDC->MoveTo(mouseLDown);
			DragDC->LineTo(mouseLUp);
			break;
		case LINE:
			DragDC->SelectObject(DragPen);
			DragDC->MoveTo(mouseLDown);
			DragDC->LineTo(mouseLUp);
			break;
		case RECTANGLE:
			DragDC->SelectObject(DragBrush);
			DragDC->SelectObject(DragPen);
			DragDC->Rectangle(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
			break;
		case ROUNDRECT:
			DragDC->SelectObject(DragBrush);
			DragDC->SelectObject(DragPen);
			DragDC->RoundRect(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y,(int)(mouseLDown.x-mouseLUp.x)*percentX,(int)(mouseLDown.y-mouseLUp.y)*percentY);
			break;
		case ELLIPSE:
			DragDC->SelectObject(DragBrush);
			DragDC->SelectObject(DragPen);
			DragDC->Ellipse(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
			break;
		case POLYGON:
			//多边形的处理逻辑上比较复杂
			if (PointList.IsEmpty())
			{
				dc2logic(point);
				//如果为空则先压入一点
				//压入的是逻辑坐标，但绘图用的仍然是dc坐标
				PointList.AddTail(point);
				logic2dc(point);
			}
			DragDC->SelectObject(DragPen);
			mouseLDown=PointList.GetTail();
			logic2dc(mouseLDown);//转换为dc坐标
			DragDC->MoveTo(mouseLDown);
			DragDC->LineTo(point);
			break;
		case BEZIER:
			if (PointList.GetSize()==4)
			{
				dc2logic(point);
				//命中控制点
				if (HitCtrlPoint(point))
				{
					DragBezier=TRUE;
					//重新载入旧图像
					CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
					CDC dcFromTmpDDB;
					dcFromTmpDDB.CreateCompatibleDC(DragDC);
					dcFromTmpDDB.SelectObject(pTmpBitmap);
					DragDC->BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcFromTmpDDB,0,0,SRCCOPY);
					dcFromTmpDDB.DeleteDC();
					pTmpBitmap=NULL;
					//重新显示控制区域并将覆盖区域压入vector容器
					DragDC->SetROP2(R2_COPYPEN);
					PointSets.clear();
					POSITION dPOS;
					dPOS=PointList.GetHeadPosition();
					for (int j=0;j<4;j++)
					{
						if (dPOS!=NULL)
							DrawControlPoint(DragDC,PointList.GetNext(dPOS));
					}
					//预览贝塞尔曲线
					DragDC->SetROP2(R2_NOTXORPEN);
					DrawPolyBezier(DragDC,RGB(192,192,192));
				}
				logic2dc(point);
			}
			break;
		default:
			break;
	}
	SetCapture(); //::SetCapture(HWND);
}

//鼠标移动
void CChildView::OnMouseMove(UINT nFlags,CPoint point)
{
	CScrollView::OnMouseMove(nFlags,point);
	if (bDrawDrag) //按下左键拖曳
	{
		DragDC->DPtoLP(&point); //视口->窗口
		switch (drawType)
		{
			case PEN:
				mouseLDown=mouseLUp;
				mouseLUp=point;
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				if (!MemDC)
				{
					if (!pCBitmap)
					{
						pCBitmap=new CBitmap;
						pCBitmap->CreateCompatibleBitmap(DragDC,DDBSize.cx,DDBSize.cy);
					}
					MemDC = new CDC;
					MemDC->CreateCompatibleDC(DragDC);
					MemDC->SelectObject(pCBitmap);
					MemDC->SelectObject(DragPen);
					CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
					CDC dcFromDDB;
					dcFromDDB.CreateCompatibleDC(DragDC);
					dcFromDDB.SelectObject(pBufferDDB);
					MemDC->BitBlt(0,0,DDBSize.cx,DDBSize.cy,&dcFromDDB,0,0,SRCCOPY);
					dcFromDDB.DeleteDC();
					pBufferDDB=NULL;
				}
				MemDC->MoveTo(dc2virtualDC(mouseLDown));
				MemDC->LineTo(dc2virtualDC(mouseLUp));
				break;
			case LINE:
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				mouseLUp=point;
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				break;
			case RECTANGLE:
				DragDC->Rectangle(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				mouseLUp=point;
				DragDC->Rectangle(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				break;
			case ROUNDRECT:
				DragDC->RoundRect(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y,(int)(mouseLDown.x-mouseLUp.x)*percentX,(int)(mouseLDown.y-mouseLUp.y)*percentY);
				mouseLUp=point;
				DragDC->RoundRect(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y,(int)(mouseLDown.x-mouseLUp.x)*percentX,(int)(mouseLDown.y-mouseLUp.y)*percentY);
				break;
			case ELLIPSE:
				DragDC->Ellipse(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				mouseLUp=point;
				DragDC->Ellipse(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				break;
			case POLYGON:
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				mouseLUp=point;
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				break;
			case BEZIER:
				if (DragBezier)
				{
					if (DragBezierLoc==4)
						break;
					if (DragBezierPos==NULL)
						break;

					//消除旧曲线
					DrawPolyBezier(DragDC,RGB(192,192,192));
					//替换控制顶点
					dc2logic(point);
					PointList.SetAt(DragBezierPos,point);
					//画曲线
					DrawPolyBezier(DragDC,RGB(192,192,192));
					logic2dc(point);
				}
				break;
			default:
				break;
		}
		DragDC->LPtoDP(&point);
	}
	//逻辑坐标显示
	CPoint logicPoint=view2logic(point);
	//CMainFrame *pFrame = (CMainFrame*)this->GetParentFrame();    //使用基类CWnd的成员函数取得指针
	CMainFrame *pFrame = (CMainFrame*)::AfxGetApp()->GetMainWnd(); //使用全局函数取得指针
	pFrame->ShowCoordinate(logicPoint);
}

//放开鼠标左键
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags,point);
	if (bDrawDrag)
	{
		//保存旧图像
		CDC dcToTmpDDB,dcDDB;
		dcToTmpDDB.CreateCompatibleDC(DragDC);
		dcDDB.CreateCompatibleDC(DragDC);
		CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
		dcToTmpDDB.SelectObject(pTmpBitmap);
		dcDDB.SelectObject(pBufferDDB);
		if (!bDrawPolygon)
			::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcDDB.m_hDC,0,0,SRCCOPY);
		else
		{
			if (!bTracePolygon) //多边形仅保存一次旧图像
				::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcDDB.m_hDC,0,0,SRCCOPY);
			bTracePolygon=TRUE;
		}
		dcToTmpDDB.DeleteDC();
		pTmpBitmap=NULL;

		if (MemDC)
		{
			dcDDB.BitBlt(0,0,DDBSize.cx,DDBSize.cy,MemDC,0,0,SRCCOPY);
			delete MemDC;
			MemDC=NULL;
			delete pCBitmap;
			pCBitmap=NULL;
		}
		dcDDB.DeleteDC();
		MemDC = new CDC;
		MemDC->CreateCompatibleDC(DragDC);
		MemDC->SelectObject(pBufferDDB);
		MemDC->SelectObject(DragPen);
		MemDC->SelectObject(DragBrush);

		DragDC->DPtoLP(&point); //视口->窗口
		CPoint vmouseLDown,vmouseLUp;
		switch (drawType)
		{
			case PEN:
				mouseLDown=mouseLUp;
				mouseLUp=point;
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				MemDC->MoveTo(dc2virtualDC(mouseLDown));
				MemDC->LineTo(dc2virtualDC(mouseLUp));
				break;
			case LINE:
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				mouseLUp=point;
				DragDC->SetROP2(R2_COPYPEN);
				DragDC->MoveTo(mouseLDown);
				DragDC->LineTo(mouseLUp);
				MemDC->MoveTo(dc2virtualDC(mouseLDown));
				MemDC->LineTo(dc2virtualDC(mouseLUp)); 
				break;
			case RECTANGLE:
				DragDC->Rectangle(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				mouseLUp=point;
				DragDC->SetROP2(R2_COPYPEN);
				DragDC->Rectangle(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				vmouseLDown=dc2virtualDC(mouseLDown);
				vmouseLUp=dc2virtualDC(mouseLUp);
				MemDC->Rectangle(vmouseLDown.x,vmouseLDown.y,vmouseLUp.x,vmouseLUp.y);
				break;
			case ROUNDRECT:
				DragDC->RoundRect(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y,(int)(mouseLDown.x-mouseLUp.x)*percentX,(mouseLDown.y-mouseLUp.y)*percentY);
				mouseLUp=point;
				DragDC->SetROP2(R2_COPYPEN);
				DragDC->RoundRect(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y,(int)(mouseLDown.x-mouseLUp.x)*percentX,(mouseLDown.y-mouseLUp.y)*percentY);
				vmouseLDown=dc2virtualDC(mouseLDown);
				vmouseLUp=dc2virtualDC(mouseLUp);
				MemDC->RoundRect(vmouseLDown.x,vmouseLDown.y,vmouseLUp.x,vmouseLUp.y,(int)(vmouseLDown.x-vmouseLUp.x)*percentX,(vmouseLDown.y-vmouseLUp.y)*percentY);
				break;
			case ELLIPSE:
				DragDC->Ellipse(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				mouseLUp=point;
				DragDC->SetROP2(R2_COPYPEN);
				DragDC->Ellipse(mouseLDown.x,mouseLDown.y,mouseLUp.x,mouseLUp.y);
				vmouseLDown=dc2virtualDC(mouseLDown);
				vmouseLUp=dc2virtualDC(mouseLUp);
				MemDC->Ellipse(vmouseLDown.x,vmouseLDown.y,vmouseLUp.x,vmouseLUp.y);
				break;
			case POLYGON:
				mouseLUp=point;
				MemDC->MoveTo(dc2virtualDC(mouseLDown));
				MemDC->LineTo(dc2virtualDC(mouseLUp));
				dc2logic(mouseLUp);
				PointList.AddTail(mouseLUp);
				logic2dc(mouseLUp);
				break;
			case BEZIER:
				mouseLUp=point;
				DragDC->SetROP2(R2_COPYPEN);
				if (DragBezier)
				{
					if (DragBezierLoc==4)
						break;
					if (DragBezierPos==NULL)
						break;
					PointSet recPoints;
					recPoints=PointSets[DragBezierLoc];
					//转换到dc坐标
					logic2dc(recPoints.center);
					//恢复旧控制点覆盖区域
					DragDC->SetPixel(recPoints.center.x-1,recPoints.center.y-1,recPoints.p1);
					DragDC->SetPixel(recPoints.center.x  ,recPoints.center.y-1,recPoints.p2);
					DragDC->SetPixel(recPoints.center.x+1,recPoints.center.y-1,recPoints.p3);
					DragDC->SetPixel(recPoints.center.x-1,recPoints.center.y  ,recPoints.p4);
					DragDC->SetPixel(recPoints.center.x  ,recPoints.center.y  ,recPoints.p5);
					DragDC->SetPixel(recPoints.center.x+1,recPoints.center.y  ,recPoints.p6);
					DragDC->SetPixel(recPoints.center.x-1,recPoints.center.y+1,recPoints.p7);
					DragDC->SetPixel(recPoints.center.x  ,recPoints.center.y+1,recPoints.p8);
					DragDC->SetPixel(recPoints.center.x+1,recPoints.center.y+1,recPoints.p9);
					//保存新的控制区域点覆盖区域
					recPoints.center=point;
					//覆盖区域中心点转换到逻辑坐标
					dc2logic(recPoints.center);
					recPoints.p1=DragDC->GetPixel(point.x-1,point.y-1);
					recPoints.p2=DragDC->GetPixel(point.x  ,point.y-1);
					recPoints.p3=DragDC->GetPixel(point.x+1,point.y-1);
					recPoints.p4=DragDC->GetPixel(point.x-1,point.y  );
					recPoints.p5=DragDC->GetPixel(point.x  ,point.y  );
					recPoints.p6=DragDC->GetPixel(point.x+1,point.y  );
					recPoints.p7=DragDC->GetPixel(point.x-1,point.y+1);
					recPoints.p8=DragDC->GetPixel(point.x  ,point.y+1);
					recPoints.p9=DragDC->GetPixel(point.x+1,point.y+1);
					PointSets[DragBezierLoc]=recPoints;
					//显示新的控制顶点区域
					COLORREF pColor=RGB(192,192,192);
					DragDC->SetPixel(point.x-1,point.y-1,pColor);
					DragDC->SetPixel(point.x  ,point.y-1,pColor);
					DragDC->SetPixel(point.x+1,point.y-1,pColor);
					DragDC->SetPixel(point.x-1,point.y  ,pColor);
					DragDC->SetPixel(point.x  ,point.y  ,pColor);
					DragDC->SetPixel(point.x+1,point.y  ,pColor);
					DragDC->SetPixel(point.x-1,point.y+1,pColor);
					DragDC->SetPixel(point.x  ,point.y+1,pColor);
					DragDC->SetPixel(point.x+1,point.y+1,pColor);
					//消除旧曲线
					DragDC->SetROP2(R2_NOTXORPEN);
					DrawPolyBezier(DragDC,RGB(192,192,192));
					DragDC->SetROP2(R2_COPYPEN);
					//替换控制顶点
					dc2logic(point);
					PointList.SetAt(DragBezierPos,point);
					logic2dc(point);
					//虚拟dc重新载入
					pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
					CDC dcFromTmpDDB;
					dcFromTmpDDB.CreateCompatibleDC(DragDC);
					dcFromTmpDDB.SelectObject(pTmpBitmap);
					MemDC->BitBlt(0,0,DDBSize.cx,DDBSize.cy,&dcFromTmpDDB,0,0,SRCCOPY);
					dcFromTmpDDB.DeleteDC();
					pTmpBitmap=NULL;
					//在虚拟dc画四个控制点
					POSITION dPOS=PointList.GetHeadPosition();
					for (int j=0;j<4;j++)
					{
						if (dPOS!=NULL)
							DrawVirControlPoint(MemDC,PointList.GetNext(dPOS));
					}
				}
				if((int)PointList.GetSize()<4)
				{
					dc2logic(mouseLUp);
					DrawVirControlPoint(MemDC,mouseLUp); //虚拟dc画控制点
					DrawControlPoint(DragDC,mouseLUp);   //客户区dc显示控制点
					PointList.AddTail(mouseLUp);
					logic2dc(mouseLUp);
				}
				if ((int)PointList.GetSize()==4)
				{
					//如果控制点数达到4点则显示预览效果
					DrawPolyBezier(DragDC,RGB(192,192,192));
					DrawPolyBezier(MemDC,RGB(192,192,192),TRUE);
				}
				break;
			default:
				break;
		}
		delete DragBrush;
		DragBrush=NULL;
		delete DragPen;
		DragPen=NULL;
		delete DragDC;
		delete MemDC;
		DragDC=NULL;
		MemDC=NULL;
		if (pCBitmap)
		{
			delete pCBitmap;
			pCBitmap=NULL;
		}

		bDrawDrag=FALSE;
		ReleaseCapture();
		isUndo=TRUE;
		DragBezier=FALSE;
		DragBezierPos=NULL;
		DragBezierLoc=4;
	}
}

//双击鼠标左键
void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDblClk(nFlags, point);
	if (bDrawPolygon)
	{
		CClientDC dc(this);
		CScrollView::OnPrepareDC(&dc);
		dc.DPtoLP(&point); //视口->窗口
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
		CDC dcToDDB;
		dcToDDB.CreateCompatibleDC(&dc);
		dcToDDB.SelectObject(pBufferDDB);
		dc2logic(point);
		PointList.AddTail(point);
		logic2dc(point);
		if (drawType==POLYGON)
		{
			//重新载入旧图像
			CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
			CDC dcFromTmpDDB;
			dcFromTmpDDB.CreateCompatibleDC(&dc);
			dcFromTmpDDB.SelectObject(pTmpBitmap);
			dcToDDB.BitBlt(0,0,DDBSize.cx,DDBSize.cy,&dcFromTmpDDB,0,0,SRCCOPY);
			dcFromTmpDDB.DeleteDC();
			//多边形定型
			DrawPolygon(&dcToDDB);
		}
		else if (drawType==BEZIER)
		{
			//重新载入旧图像
			CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
			CDC dcFromTmpDDB;
			dcFromTmpDDB.CreateCompatibleDC(&dc);
			dcFromTmpDDB.SelectObject(pTmpBitmap);
			dcToDDB.BitBlt(0,0,DDBSize.cx,DDBSize.cy,&dcFromTmpDDB,0,0,SRCCOPY);
			dcFromTmpDDB.DeleteDC();
			PointSets.clear();
			//贝塞尔曲线定型
			DrawPolyBezier(&dcToDDB,currentColor,TRUE);
		}
		//更新客户区
		dc.BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcToDDB,0,0,SRCCOPY);
		dcToDDB.DeleteDC();

		bTracePolygon=FALSE;
		DragBezier=FALSE;
		DragBezierPos=NULL;
		DragBezierLoc=4;
		PointList.RemoveAll();    //清空控制点
	}
}

//失去焦点
void CChildView::OnKillFocus(CWnd* pNewWnd)
{
	CScrollView::OnKillFocus(pNewWnd);
	if (bDrawDrag)
	{
		CBitmap *pBufferDDB = CBitmap::FromHandle(hBitmap);
		if (MemDC)
		{
			delete MemDC;
			MemDC=NULL;
		}
		MemDC = new CDC;
		MemDC->CreateCompatibleDC(DragDC);
		MemDC->SelectObject(pBufferDDB);
		DragDC->BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,MemDC,0,0,SRCCOPY);
		delete MemDC;
		MemDC=NULL;
		pBufferDDB=NULL;
		delete DragBrush;
		DragBrush=NULL;
		delete DragPen;
		DragPen=NULL;
		delete DragDC;
		DragDC=NULL;
		if (pCBitmap)
		{
			delete pCBitmap;
			pCBitmap=NULL;
		}
		bDrawDrag=FALSE;
		ReleaseCapture();
		DragBezier=FALSE;
		DragBezierPos=NULL;
		DragBezierLoc=4;
		if ((drawType==POLYGON)||(drawType==BEZIER))
		{
			bTracePolygon=FALSE;
			PointList.RemoveAll();
			PointSets.clear();
		}
	}
}

//获得焦点
void CChildView::OnSetFocus(CWnd* pOldWnd)
{
	CScrollView::OnSetFocus(pOldWnd);
	CWnd::Invalidate(TRUE);
	CWnd::UpdateWindow();
}

//重载OnSetCursor函数（鼠标移动至客户区时更改鼠标指针）
BOOL CChildView::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
	BOOL apiRet;
	apiRet=CScrollView::OnSetCursor(pWnd,nHitTest,message);

	if ((apiRet==FALSE)&&(nHitTest==HTCLIENT))
		::SetCursor(mouseCur);
	return apiRet;
}

//更改绘图方式
void CChildView::OnEditPen()
{
	drawType=PEN;
	bDrawPolygon=FALSE;
}
void CChildView::OnEditLine()
{
	drawType=LINE;
	bDrawPolygon=FALSE;
}
void CChildView::OnEditRect()
{
	drawType=RECTANGLE;
	bDrawPolygon=FALSE;
}
void CChildView::OnEditRoundrect()
{
	drawType=ROUNDRECT;
	bDrawPolygon=FALSE;
}
void CChildView::OnEditEllipse()
{
	drawType=ELLIPSE;
	bDrawPolygon=FALSE;
}
void CChildView::OnEditPolygon()
{
	drawType=POLYGON;
	bTracePolygon=FALSE;
	bDrawPolygon=TRUE;
	PointList.RemoveAll();
}
void CChildView::OnEditBezier()
{
	drawType=BEZIER;
	DragBezierPos=NULL;
	DragBezierLoc=4;
	bDrawPolygon=TRUE;
	bTracePolygon=FALSE;
	DragBezier=FALSE;
	PointList.RemoveAll();
	PointSets.clear();
}

//改变颜色
void CChildView::OnEditColor()
{
	COLORREF theColor;
	CColorDialog selectColor;
	if (selectColor.DoModal() == IDOK)
	{
		theColor=selectColor.GetColor();
		currentColor=theColor;
	}
}

//更改填充模式
void CChildView::OnEditNotfilling()
{
	drawMode=NOTFILLING;
}
void CChildView::OnEditErasefilling()
{
	drawMode=ERASEFILLING;
}
void CChildView::OnEditFullfilling()
{
	drawMode=FULLFILLING;
}

//清空画布
void CChildView::OnEditClear()
{
	bFlag=FALSE;
	isUndo=TRUE;
	CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap); //句柄转化为指针
	CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
	CDC dc;
	dc.Attach(::GetDC(NULL));
	CDC dcFromDDB,dcToTmpDDB;
	dcFromDDB.CreateCompatibleDC(&dc);
	dcFromDDB.SelectObject(pBufferDDB);
	dcToTmpDDB.CreateCompatibleDC(&dc);
	dcToTmpDDB.SelectObject(pTmpBitmap);
	//保存旧图像
	::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcFromDDB.m_hDC,0,0,SRCCOPY);
	dcToTmpDDB.DeleteDC();
	dcFromDDB.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());
	//多边形的特殊处理
	if ((drawType==POLYGON)||(drawType==BEZIER))
	{
		bTracePolygon=FALSE;
		PointList.RemoveAll();
		PointSets.clear();
	}
	CWnd::Invalidate(TRUE); //使区域无效，触发WM_PAINT消息
}

//返回中心绘图区域
CRect CChildView::returnDrawRect()
{
	CRect CanvasArea;
	GetClientRect(&CanvasArea);
	CPoint center;
	center=CanvasArea.CenterPoint();//中心点
	if (CanvasArea.Width()<DDBSize.cx)
	{
		center.x=DDBSize.cx*0.5;
	}
	if (CanvasArea.Height()<DDBSize.cy)
	{
		center.y=DDBSize.cy*0.5;
	}
	CanvasArea=CRect(CPoint(center.x-(LONG)(DDBSize.cx*0.5),center.y-(LONG)(DDBSize.cy*0.5)),DDBSize); //画布：中心点为工作区中心点，默认800*600

	return CanvasArea;
}

//清空画布/初始化画布
void CChildView::ClearCanvas(CDC *pDC)
{
	CBrush brush(RGB(255,255,255));
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->SelectObject(&pen);
	pDC->Rectangle(0,0,DDBSize.cx,DDBSize.cy);
	brush.DeleteObject();
	pen.DeleteObject();
}

//画多边形
void CChildView::DrawPolygon(CDC *pDC)
{
	if (!PointList.IsEmpty())
	{
		CPen pen;
		pen.CreatePen(PS_SOLID,currentWidth,currentColor);
		pDC->SelectObject(&pen);
		CBrush brush;
		if (drawMode==NOTFILLING)
			brush.CreateStockObject(NULL_BRUSH);
		else if (drawMode==ERASEFILLING)
			brush.CreateSolidBrush(RGB(255,255,255));
		else if (drawMode==FULLFILLING)
			brush.CreateSolidBrush(currentColor);
		else
			brush.CreateStockObject(NULL_BRUSH);
		pDC->SelectObject(&brush);
		//读取PointList
		int nCount;
		nCount=PointList.GetSize();
		CPoint *pPoint = new CPoint[nCount];
		POSITION pos;
		pos=PointList.GetHeadPosition();
		for (int i=0;i<nCount;i++)
		{
			pPoint[i]=PointList.GetNext(pos);
			pPoint[i]=logic2virtualDC(pPoint[i]); //转换为dc坐标
		}
		pDC->Polygon(pPoint,nCount);
		pen.DeleteObject();
		brush.DeleteObject();
		delete []pPoint;
		pPoint=NULL;
	}
}

//画贝塞尔曲线
//贝塞尔曲线是由四个点控制的曲线，其中首尾两个端点，中间两个控制顶点
//存放在PointList中的控制点的坐标是虚拟逻辑坐标，这个坐标轴以作图区域中心为原点，以向上为y轴正向
//取出来画图时必须转换为dc坐标，而在压入时又必须为逻辑坐标
void CChildView::DrawPolyBezier(CDC *pDC,COLORREF theColor,BOOL bVirtualDC)
{
	if (!PointList.IsEmpty())
	{
		CPen pen;
		pen.CreatePen(PS_SOLID,currentWidth,theColor);
		pDC->SelectObject(&pen);
		//读取PointList
		CPoint *pPoint = new CPoint[4];
		POSITION pos;
		pos=PointList.GetHeadPosition();
		for (int i=0;i<4;i++)
		{
			if (pos!=NULL)
			{
				pPoint[i]=PointList.GetNext(pos);
				if (!bVirtualDC)
					logic2dc(pPoint[i]); //转换为客户区dc坐标
				else
					pPoint[i]=logic2virtualDC(pPoint[i]); //转换为虚拟dc坐标
			}
			else
				pPoint[i]=pPoint[i-1];
		}
		pDC->PolyBezier(pPoint,4);   //画曲线
		pen.DeleteObject();
		delete []pPoint;
		pPoint=NULL;
	}
}
//显示贝塞尔曲线的控制点并保存覆盖区域，ctrlPoint为逻辑坐标
void CChildView::DrawControlPoint(CDC *pDC,CPoint ctrlPoint)
{
	logic2dc(ctrlPoint); //转换到dc坐标
	int x,y;
	x=ctrlPoint.x;
	y=ctrlPoint.y;
	dc2logic(ctrlPoint); //转换回来（又变成逻辑坐标）
	PointSet coverPoints;
	coverPoints.center=ctrlPoint;
	coverPoints.p1=pDC->GetPixel(x-1,y-1);
	coverPoints.p2=pDC->GetPixel(x,  y-1);
	coverPoints.p3=pDC->GetPixel(x+1,y-1);
	coverPoints.p4=pDC->GetPixel(x-1,y);
	coverPoints.p5=pDC->GetPixel(x,  y);
	coverPoints.p6=pDC->GetPixel(x+1,y);
	coverPoints.p7=pDC->GetPixel(x-1,y+1);
	coverPoints.p8=pDC->GetPixel(x,  y+1);
	coverPoints.p9=pDC->GetPixel(x+1,y+1);
	PointSets.push_back(coverPoints);

	COLORREF pColor=RGB(192,192,192);
	pDC->SetPixel(x-1,y-1,pColor);
	pDC->SetPixel(x,  y-1,pColor);
	pDC->SetPixel(x+1,y-1,pColor);
	pDC->SetPixel(x-1,y,  pColor);
	pDC->SetPixel(x,  y,  pColor);
	pDC->SetPixel(x+1,y,  pColor);
	pDC->SetPixel(x-1,y+1,pColor);
	pDC->SetPixel(x,  y+1,pColor);
	pDC->SetPixel(x+1,y+1,pColor);
}
//虚拟dc中绘制覆盖区域 
void CChildView::DrawVirControlPoint(CDC *pDC, CPoint ctrlPoint)
{
	CPoint vPoint=logic2virtualDC(ctrlPoint);
	int x,y;
	x=vPoint.x;
	y=vPoint.y;
	COLORREF pColor=RGB(192,192,192);
	pDC->SetPixel(x-1,y-1,pColor);
	pDC->SetPixel(x,  y-1,pColor);
	pDC->SetPixel(x+1,y-1,pColor);
	pDC->SetPixel(x-1,y,  pColor);
	pDC->SetPixel(x,  y,  pColor);
	pDC->SetPixel(x+1,y,  pColor);
	pDC->SetPixel(x-1,y+1,pColor);
	pDC->SetPixel(x,  y+1,pColor);
	pDC->SetPixel(x+1,y+1,pColor);
}

//更改画布大小，重新初始化画布
void CChildView::ChangeCanvasSize(CSize *newsize)
{
	bFlag=TRUE;
	isUndo=FALSE; //不允许再撤销
	//多边形重初始化
	if ((drawType==POLYGON)||(drawType==BEZIER))
	{
		bTracePolygon=FALSE;
		PointList.RemoveAll();
		PointSets.clear();
	}
	CDC dc;
	dc.Attach(::GetDC(NULL));
	CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
	//清空临时缓冲
	if (tmpBitmap!=NULL)
		::DeleteObject(tmpBitmap);
	//新尺寸
	tmpBitmap = ::CreateCompatibleBitmap(dc.m_hDC,newsize->cx,newsize->cy);
	CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
	CDC dcFromDDB,dcTmpDDB;
	dcFromDDB.CreateCompatibleDC(&dc);
	dcFromDDB.SelectObject(pBufferDDB);
	dcTmpDDB.CreateCompatibleDC(&dc);
	dcTmpDDB.SelectObject(pTmpBitmap);
	//先清空一次dc，创建一张白画布
	CBrush brush(RGB(255,255,255));
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	dcTmpDDB.SelectObject(&brush);
	dcTmpDDB.SelectObject(&pen);
	dcTmpDDB.Rectangle(0,0,newsize->cx,newsize->cy);
	brush.DeleteObject();
	pen.DeleteObject();
	//保存旧图像，拷贝最小尺寸
	::BitBlt(dcTmpDDB.m_hDC,0,0,((DDBSize.cx>newsize->cx)?newsize->cx:DDBSize.cx),
		((DDBSize.cy>newsize->cy)?newsize->cy:DDBSize.cy),dcFromDDB.m_hDC,0,0,SRCCOPY);
	dcFromDDB.DeleteDC();

	DDBSize.cx=newsize->cx;
	DDBSize.cy=newsize->cy;

	pBufferDDB=NULL;
	::DeleteObject(hBitmap);
	//新尺寸
	hBitmap = ::CreateCompatibleBitmap(dc.m_hDC,DDBSize.cx,DDBSize.cy);
	pBufferDDB=CBitmap::FromHandle(hBitmap);
	//回拷，全尺寸
	CDC dcToDDB;
	dcToDDB.CreateCompatibleDC(&dc);
	dcToDDB.SelectObject(pBufferDDB);
	::BitBlt(dcToDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcTmpDDB.m_hDC,0,0,SRCCOPY);
	dcToDDB.DeleteDC();
	dcTmpDDB.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());

	SetScrollSizes(MM_TEXT, DDBSize);
	CWnd::Invalidate(TRUE); //重绘客户区
}

//撤销
void CChildView::OnEditUndo()
{
	if (isUndo)
	{
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap); //句柄转化为指针
		CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
		CClientDC dc(this);
		CScrollView::OnPrepareDC(&dc);
		CDC dcToDDB,dcFromTmpDDB;
		dcToDDB.CreateCompatibleDC(&dc);
		dcToDDB.SelectObject(pBufferDDB);
		dcFromTmpDDB.CreateCompatibleDC(&dc);
		dcFromTmpDDB.SelectObject(pTmpBitmap);
		//恢复旧图像
		::BitBlt(dcToDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcFromTmpDDB.m_hDC,0,0,SRCCOPY);
		//更新客户区
		dc.BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcFromTmpDDB,0,0,SRCCOPY);
		//释放DC资源
		dcToDDB.DeleteDC();
		dcFromTmpDDB.DeleteDC();

		isUndo=FALSE;
		if ((drawType==POLYGON)||(drawType==BEZIER))
		{
			bTracePolygon=FALSE;
			PointList.RemoveAll();
			PointSets.clear();
		}
	}
}

//编辑画布大小
void CChildView::OnEditSize()
{
	DialogPageTwo   m_page2;
	m_page2.m_width  = DDBSize.cx;
	m_page2.m_height = DDBSize.cy;

	DialogSheet sheet(this);
	sheet.AddPage(&m_page2);
	sheet.SetActivePage(0);
	
	if (sheet.DoModal() == IDOK)
	{
		CSize newSize;
		newSize.cx=m_page2.m_width;
		newSize.cy=m_page2.m_height;
		if (DDBSize != newSize)
			ChangeCanvasSize(&newSize);
	}
}

//判定撤销有效性
void CChildView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if (isUndo)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

//判定填充模式
void CChildView::OnUpdateEditNotfilling(CCmdUI *pCmdUI)
{
	if (drawMode==NOTFILLING)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditErasefilling(CCmdUI *pCmdUI)
{
	if (drawMode==ERASEFILLING)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditFullfilling(CCmdUI *pCmdUI)
{
	if (drawMode==FULLFILLING)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

//判定画图模式
void CChildView::OnUpdateEditPen(CCmdUI *pCmdUI)
{
	if (drawType==PEN)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditLine(CCmdUI *pCmdUI)
{
	if (drawType==LINE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditRect(CCmdUI *pCmdUI)
{
	if (drawType==RECTANGLE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditRoundrect(CCmdUI *pCmdUI)
{
	if (drawType==ROUNDRECT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditEllipse(CCmdUI *pCmdUI)
{
	if (drawType==ELLIPSE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditPolygon(CCmdUI *pCmdUI)
{
	if (drawType==POLYGON)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}
void CChildView::OnUpdateEditBezier(CCmdUI *pCmdUI)
{
	if (drawType==BEZIER)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

//弹出右键菜单
void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonUp(nFlags, point);
	CMenu menu;
	menu.LoadMenu(IDR_FLOATMENU);
	CMenu *pM=menu.GetSubMenu(0);

	if (isUndo)
		pM->EnableMenuItem(ID_EDIT_UNDO,MF_ENABLED);
	else
		pM->EnableMenuItem(ID_EDIT_UNDO,MF_GRAYED);
	if (drawType==PEN)
		pM->CheckMenuItem(ID_EDIT_PEN,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_PEN,MF_UNCHECKED);
	if (drawType==LINE)
		pM->CheckMenuItem(ID_EDIT_LINE,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_LINE,MF_UNCHECKED);
	if (drawType==RECTANGLE)
		pM->CheckMenuItem(ID_EDIT_RECT,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_RECT,MF_UNCHECKED);
	if (drawType==ROUNDRECT)
	{
		pM->CheckMenuItem(ID_EDIT_ROUNDRECT,MF_CHECKED);
		pM->EnableMenuItem(ID_EDIT_ROUNDDEGREE,MF_ENABLED);
	}
	else
	{
		pM->CheckMenuItem(ID_EDIT_ROUNDRECT,MF_UNCHECKED);
		pM->EnableMenuItem(ID_EDIT_ROUNDDEGREE,MF_GRAYED);
	}
	if (drawType==ELLIPSE)
		pM->CheckMenuItem(ID_EDIT_ELLIPSE,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_ELLIPSE,MF_UNCHECKED);
	if (drawType==POLYGON)
		pM->CheckMenuItem(ID_EDIT_POLYGON,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_POLYGON,MF_UNCHECKED);
	if (drawType==BEZIER)
		pM->CheckMenuItem(ID_EDIT_BEZIER,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_BEZIER,MF_UNCHECKED);
	if (drawMode==NOTFILLING)
		pM->CheckMenuItem(ID_EDIT_NOTFILLING,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_NOTFILLING,MF_UNCHECKED);
	if (drawMode==ERASEFILLING)
		pM->CheckMenuItem(ID_EDIT_ERASEFILLING,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_ERASEFILLING,MF_UNCHECKED);
	if (drawMode==FULLFILLING)
		pM->CheckMenuItem(ID_EDIT_FULLFILLING,MF_CHECKED);
	else
		pM->CheckMenuItem(ID_EDIT_FULLFILLING,MF_UNCHECKED);

	CPoint pt;
	GetCursorPos(&pt);
	pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);

	/*
	CMenu pMenu;
	pMenu.LoadMenu(IDR_MAINFRAME);
	CMenu *pSubMenu=pMenu.GetSubMenu(1);
	CPoint mpt;
	GetCursorPos(&mpt);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,mpt.x,mpt.y,this);
	*/
}

//圆角矩形的圆角程度设置
void CChildView::OnEditRounddegree()
{
	DialogPageOne   m_page1;
	m_page1.m_x=percentX;
	m_page1.m_y=percentY;

	DialogSheet sheet(this);
	sheet.AddPage(&m_page1); //0
	sheet.SetActivePage(0);
	
	if (sheet.DoModal() == IDOK)
	{
		percentX=m_page1.m_x;
		percentY=m_page1.m_y;
	}
}
void CChildView::OnUpdateEditRounddegree(CCmdUI *pCmdUI)
{
	if (drawType==ROUNDRECT)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CChildView::OnPaintFileOpen()
{
	CFileDialog openFile(TRUE,L"*.bmp",NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,L"位图文件(*.bmp)|*.bmp||");
	if (openFile.DoModal() == IDOK)
	{
		FileName = openFile.GetPathName();
		CFile openFile;
		if (!openFile.Open(FileName,CFile::modeRead|CFile::shareDenyWrite))
		{
			//打开失败
			openFile.Close();
			::AfxMessageBox(L"文件打开失败！",MB_OK);
			return;
		}
		//读文件头
		BITMAPFILEHEADER bHead;
		openFile.Read(&bHead,sizeof(bHead));
		if (bHead.bfType!=0x4d42)
		{
			//非BMP文件
			openFile.Close();
			::AfxMessageBox(L"文件类型不符！",MB_OK);
			return;
		}
		if (openFile.GetLength()!=bHead.bfSize)
		{
			//长度不等，文件损坏
			openFile.Close();
			::AfxMessageBox(L"文件损坏！",MB_OK);
			return;
		}
		//读信息头
		BITMAPINFOHEADER bi;
		openFile.Read(&bi,sizeof(bi));
		//计算调色板数目
		int numQuad=0;
		//15位色（32K）以上不需要调色板
		if(bi.biBitCount<15)
		{
			numQuad=1<<bi.biBitCount; //<<：按位左移运算符
		}
		/*
		switch (bi.biBitCount)
		{
		case 1:
			numQuad=2;
			break;
		case 4:
			numQuad=16;
			break;
		case 8:
			numQuad=256;
			break;
		}
		*/
				
		BITMAPINFO *pbi; //文件信息
		RGBQUAD *quad;   //调色板
		BYTE *lpBits;    //文件图像
		//为文件信息分配空间
		pbi=(BITMAPINFO*)HeapAlloc(GetProcessHeap(),0,sizeof(BITMAPINFOHEADER)+numQuad*sizeof(RGBQUAD));
		memcpy(pbi,&bi,sizeof(bi));

		//调色板指针
		quad=(RGBQUAD*)((BYTE*)pbi+sizeof(BITMAPINFOHEADER));
		
		//读取调色板
		if(numQuad!=0)
		{
			openFile.Read(quad,sizeof(RGBQUAD)*numQuad);
		}
		
		//为图像数据申请空间
		bi.biSizeImage=bHead.bfSize-bHead.bfOffBits;
		lpBits=(BYTE*)HeapAlloc(GetProcessHeap(),0,bi.biSizeImage);
		//读取图像数据
		openFile.Read(lpBits,bi.biSizeImage);
		//图像读取完毕，关闭文件
		openFile.Close();

		//声明客户区dc
		CClientDC dc(this);
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
		CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);
		CDC dcToDDB,dcToTmpDDB;
		dcToDDB.CreateCompatibleDC(&dc);
		dcToDDB.SelectObject(pBufferDDB);
		dcToTmpDDB.CreateCompatibleDC(&dc);
		dcToTmpDDB.SelectObject(pTmpBitmap);
		//保存旧图像
		isUndo=TRUE;
		::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcToDDB.m_hDC,0,0,SRCCOPY);
		dcToTmpDDB.DeleteDC();
		//先清空一次hBitmap
		ClearCanvas(&dcToDDB);
		//保存到hBitmap
		::SetDIBitsToDevice(dcToDDB.m_hDC,0,0,
			((bi.biWidth>DDBSize.cx)?DDBSize.cx:bi.biWidth),
			((bi.biHeight>DDBSize.cy)?DDBSize.cy:bi.biHeight),
			0,0,((bi.biHeight>DDBSize.cy)?DDBSize.cy-bi.biHeight:0),bi.biHeight,lpBits,pbi,DIB_RGB_COLORS);
		//释放空间
		HeapFree(GetProcessHeap(),0,pbi);
		HeapFree(GetProcessHeap(),0,lpBits);
		//显示到客户区dc
		CScrollView::OnPrepareDC(&dc); //调整视口
		dc.BitBlt(returnDrawRect().left,returnDrawRect().top,DDBSize.cx,DDBSize.cy,&dcToDDB,0,0,SRCCOPY);
		dcToDDB.DeleteDC();

		//多边形的重初始化
		if ((drawType==POLYGON)||(drawType==BEZIER))
		{
			bTracePolygon=FALSE;
			PointList.RemoveAll();
			PointSets.clear();
		}
	}
}

void CChildView::OnPaintFileSave()
{
	//如果文件存在，提示覆盖
	CFileDialog saveFile(FALSE,L"*.bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,L"位图文件(*.bmp)|*.bmp||");
	if (saveFile.DoModal() == IDOK)
	{
		FileName = saveFile.GetPathName();
		CFile saveBmp;
		if (!saveBmp.Open(FileName,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
		{
			::AfxMessageBox(L"无法写入文件！",MB_OK);
			return;
		}
		//从DC获取必要的调色板及其他信息
		int bitsperPixel;       //DC每像素位数
		WORD bitCount=1;        //位图的每像素位数，必须为1、4、8、16、24或32
		DWORD PaletteSize=0;    //调色板大小
		DWORD BitmapSize=0;     //位图数据大小
		CClientDC dc(this);
		bitsperPixel=dc.GetDeviceCaps(BITSPIXEL)*dc.GetDeviceCaps(PLANES);
		switch (bitsperPixel)
		{
		case 1:
			bitCount=1;
			break;
		case 2:
			bitCount=4;
			break;
		case 4:
			bitCount=4;
			break;
		case 8:
			bitCount=8;
			break;
		case 15:
			bitCount=16;
			break;
		case 16:
			bitCount=16;
			break;
		case 24:
			bitCount=24;
			break;
		case 32: //32b扩展预留
			bitCount=32;
			break;
		}
		/*
		if (bitsperPixel<=1)
			bitCount=1;
		else if (bitsperPixel<=4)
			bitCount=4;
		else if (bitsperPixel<=8)
			bitCount=8;
		else if (bitsperPixel<=16)
			bitCount=16;
		else if (bitsperPixel<=24)
			bitCount=24;
		else
			bitCount=32;
		*/
		if(bitCount<=8)
			PaletteSize=1<<bitCount;

		//取得DDB指针
		CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
		//位图属性结构
		BITMAP Bitmap;
		::GetObject(hBitmap,sizeof(BITMAP),(LPSTR)&Bitmap);
		BitmapSize=((Bitmap.bmWidth*bitCount+31)/32)*4*Bitmap.bmHeight;
		/*
		BitmapSize=(Bitmap.bmWidth*bitCount+7)/8; //除法取整，不足8位凑满8位
		BitmapSize=(BitmapSize+3)/4;              //字节凑满4的整数倍
		BitmapSize=BitmapSize*4                   //BitmapSize当前值是每行字节数（Bmp每行字节数必须为4的整数倍）
		BitmapSize=BitmapSize*Bitmap.bmHeight;
		*/
		//文件头
		BITMAPFILEHEADER bitmapfileheader;
		bitmapfileheader.bfType=0x4d42;
		bitmapfileheader.bfReserved1=bitmapfileheader.bfReserved2=0;
		bitmapfileheader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof(RGBQUAD);
		bitmapfileheader.bfSize=bitmapfileheader.bfOffBits+BitmapSize;
		//带调色板信息头
		BITMAPINFO *lpBitmapInfo;
		lpBitmapInfo=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof(RGBQUAD)];
		lpBitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); //40
		lpBitmapInfo->bmiHeader.biWidth=Bitmap.bmWidth;
		lpBitmapInfo->bmiHeader.biHeight=Bitmap.bmHeight;
		lpBitmapInfo->bmiHeader.biPlanes=1;
		lpBitmapInfo->bmiHeader.biBitCount=bitCount;
		lpBitmapInfo->bmiHeader.biCompression=BI_RGB;            //不压缩
		lpBitmapInfo->bmiHeader.biSizeImage=BitmapSize;          //每行字节数*行数
		lpBitmapInfo->bmiHeader.biXPelsPerMeter=3780;            //水平分辨率，打印用
		lpBitmapInfo->bmiHeader.biYPelsPerMeter=3780;            //垂直分辨率，打印用
		lpBitmapInfo->bmiHeader.biClrUsed=PaletteSize;           //15位色深以上为0
		lpBitmapInfo->bmiHeader.biClrImportant=PaletteSize;      //15位色深以上为0

		//处理调色板
		HPALETTE hPalette,hOldPalette;
		hPalette=(HPALETTE)::GetStockObject(DEFAULT_PALETTE); //取得系统调色板句柄
		if (hPalette)
		{
			hOldPalette=::SelectPalette(dc.m_hDC,hPalette,FALSE);
			::RealizePalette(dc.m_hDC);  //将dc的调色板匹配为系统调色板
		}
		//图像数据
		BYTE *DibBits;
		DibBits=(BYTE*)new char[BitmapSize];
		//从hBitmap取得当前调色板下的图像数据
		//如果DibBits非空（预先申请了空间获得了地址），则lpBitmapInfo中的BITMAPINFOHEADER需要初始化
		//颜色表（15位色以下）在函数执行成功后会附加到lpBitmapInfo后面
		::GetDIBits(dc.m_hDC,hBitmap,0,Bitmap.bmHeight,LPVOID(DibBits),lpBitmapInfo,DIB_RGB_COLORS);
		if (hOldPalette)
		{
			::SelectPalette(dc.m_hDC,hOldPalette,FALSE);
			::RealizePalette(dc.m_hDC); //恢复dc的调色板
		}

		//写文件头
		saveBmp.Write(&bitmapfileheader,sizeof(BITMAPFILEHEADER));
		//写信息头和调色板
		saveBmp.Write(lpBitmapInfo,sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof(RGBQUAD));
		//写图像数据
		saveBmp.Write(DibBits,BitmapSize);
		delete []lpBitmapInfo;
		delete []DibBits;
	}
}

//选项对话框
void CChildView::OnEditOption()
{
	DialogPageOne   m_page1;
	m_page1.m_x=percentX;
	m_page1.m_y=percentY;

	DialogPageTwo   m_page2;
	m_page2.m_width  = DDBSize.cx;
	m_page2.m_height = DDBSize.cy;

	DialogPageThree m_page3;
	m_page3.m_pensize=currentWidth-1;

	DialogSheet sheet(this);
	sheet.AddPage(&m_page1); //0
	sheet.AddPage(&m_page2); //1
	sheet.AddPage(&m_page3); //2
	
	if (sheet.DoModal() == IDOK)
	{
		percentX=m_page1.m_x;
		percentY=m_page1.m_y;
		CSize newSize;
		newSize.cx=m_page2.m_width;
		newSize.cy=m_page2.m_height;
		currentWidth=m_page3.m_pensize+1;
		if (DDBSize != newSize)
			ChangeCanvasSize(&newSize);
	}
}

void CChildView::OnEditPensize()
{
	DialogPageThree m_page3;
	m_page3.m_pensize=currentWidth-1;

	DialogSheet sheet(this);
	sheet.AddPage(&m_page3);
	sheet.SetActivePage(0);
	
	if (sheet.DoModal() == IDOK)
	{
		currentWidth=m_page3.m_pensize+1;
	}
}

//上下翻转
//通过绑定到dc然后利用dc的坐标翻转来实现
void CChildView::UpDownReverse(CBitmap *theBitmap)
{
	CDC dc;
	dc.Attach(::GetDC(NULL));

	CDC dcReverseBitmap;
	dcReverseBitmap.CreateCompatibleDC(&dc);
	dcReverseBitmap.SelectObject(theBitmap);

	CBitmap tmpBitmap;
	//创建一个临时空位图
	tmpBitmap.CreateCompatibleBitmap(&dc,DDBSize.cx,DDBSize.cy);
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&tmpBitmap);

	::BitBlt(dcTmp.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcReverseBitmap.m_hDC,0,0,SRCCOPY);
	//翻转dcTmp
	dcTmp.SetMapMode(MM_ISOTROPIC);
	dcTmp.SetViewportOrg(0,DDBSize.cy-1);
	dcTmp.SetViewportExt(DDBSize.cx,DDBSize.cy);
	dcTmp.SetWindowOrg(0,0);
	dcTmp.SetWindowExt(DDBSize.cx,-DDBSize.cy);

	//回拷
	::BitBlt(dcReverseBitmap.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcTmp.m_hDC,0,0,SRCCOPY);

	dcTmp.DeleteDC();
	dcReverseBitmap.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());
}

//上下翻转
void CChildView::OnImageUdreverse()
{
	CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
	CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);

	CDC dc;
	dc.Attach(::GetDC(NULL));

	CDC dcToDDB,dcToTmpDDB;
	dcToDDB.CreateCompatibleDC(&dc);
	dcToDDB.SelectObject(pBufferDDB);
	dcToTmpDDB.CreateCompatibleDC(&dc);
	dcToTmpDDB.SelectObject(pTmpBitmap);
	//保存旧图像
	isUndo=TRUE;
	::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcToDDB.m_hDC,0,0,SRCCOPY);
	dcToTmpDDB.DeleteDC();

	CBitmap tmpBitmap;
	//创建一个临时空位图
	tmpBitmap.CreateCompatibleBitmap(&dc,DDBSize.cx,DDBSize.cy);
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&tmpBitmap);

	::BitBlt(dcTmp.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcToDDB.m_hDC,0,0,SRCCOPY);
	//翻转dcTmp
	dcTmp.SetMapMode(MM_ISOTROPIC);
	dcTmp.SetViewportOrg(0,DDBSize.cy-1);
	dcTmp.SetViewportExt(DDBSize.cx,DDBSize.cy);
	dcTmp.SetWindowOrg(0,0);
	dcTmp.SetWindowExt(DDBSize.cx,-DDBSize.cy);

	//回拷
	::BitBlt(dcToDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcTmp.m_hDC,0,0,SRCCOPY);

	dcTmp.DeleteDC();
	dcToDDB.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());

	if ((drawType==POLYGON)||(drawType==BEZIER))
	{
		bTracePolygon=FALSE;
		PointList.RemoveAll();
		PointSets.clear();
	}
	CWnd::Invalidate(TRUE);
}

//左右翻转
void CChildView::OnImageLrreverse()
{
	CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
	CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);

	CDC dc;
	dc.Attach(::GetDC(NULL));

	CDC dcReverseBitmap,dcToTmpDDB;
	dcReverseBitmap.CreateCompatibleDC(&dc);
	dcReverseBitmap.SelectObject(pBufferDDB);
	dcToTmpDDB.CreateCompatibleDC(&dc);
	dcToTmpDDB.SelectObject(pTmpBitmap);
	//保存旧图像
	isUndo=TRUE;
	::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcReverseBitmap.m_hDC,0,0,SRCCOPY);
	dcToTmpDDB.DeleteDC();

	CBitmap tmpBitmap;
	//创建一个临时空位图
	tmpBitmap.CreateCompatibleBitmap(&dc,DDBSize.cx,DDBSize.cy);
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&tmpBitmap);

	::BitBlt(dcTmp.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcReverseBitmap.m_hDC,0,0,SRCCOPY);
	//翻转dcTmp
	dcTmp.SetMapMode(MM_ISOTROPIC);
	dcTmp.SetViewportOrg(DDBSize.cx-1,0);
	dcTmp.SetViewportExt(DDBSize.cx,DDBSize.cy);
	dcTmp.SetWindowOrg(0,0);
	dcTmp.SetWindowExt(-DDBSize.cx,DDBSize.cy);

	//回拷
	::BitBlt(dcReverseBitmap.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcTmp.m_hDC,0,0,SRCCOPY);

	dcTmp.DeleteDC();
	dcReverseBitmap.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());

	if ((drawType==POLYGON)||(drawType==BEZIER))
	{
		bTracePolygon=FALSE;
		PointList.RemoveAll();
		PointSets.clear();
	}
	CWnd::Invalidate(TRUE);
}

//反色
void CChildView::OnImageCorreverse()
{
	CBitmap *pBufferDDB=CBitmap::FromHandle(hBitmap);
	CBitmap *pTmpBitmap=CBitmap::FromHandle(tmpBitmap);

	CDC dc;
	dc.Attach(::GetDC(NULL));

	CDC dcReverseBitmap,dcToTmpDDB;
	dcReverseBitmap.CreateCompatibleDC(&dc);
	dcReverseBitmap.SelectObject(pBufferDDB);
	dcToTmpDDB.CreateCompatibleDC(&dc);
	dcToTmpDDB.SelectObject(pTmpBitmap);

	//保存旧图像
	isUndo=TRUE;
	::BitBlt(dcToTmpDDB.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcReverseBitmap.m_hDC,0,0,SRCCOPY);
	dcToTmpDDB.DeleteDC();
	//反色
	::BitBlt(dcReverseBitmap.m_hDC,0,0,DDBSize.cx,DDBSize.cy,dcReverseBitmap.m_hDC,0,0,NOTSRCCOPY);

	dcReverseBitmap.DeleteDC();
	::ReleaseDC(NULL,dc.Detach());

	if ((drawType==POLYGON)||(drawType==BEZIER))
	{
		bTracePolygon=FALSE;
		PointList.RemoveAll();
		PointSets.clear();
	}
	CWnd::Invalidate(TRUE);
}
