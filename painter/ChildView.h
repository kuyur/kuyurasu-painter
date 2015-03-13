// ChildView.h : CChildView 类的接口
//

#pragma once

enum DRAWTYPE{PEN=1,LINE=2,RECTANGLE=3,ROUNDRECT=4,ELLIPSE=5,POLYGON=6,BEZIER=7};
enum DRAWMODE{NOTFILLING=1,ERASEFILLING=2,FULLFILLING=3};
struct PointSet
{
	CPoint center;
	COLORREF p1;
	COLORREF p2;
	COLORREF p3;
	COLORREF p4;
	COLORREF p5;
	COLORREF p6;
	COLORREF p7;
	COLORREF p8;
	COLORREF p9;
};

class CChildView : public CScrollView
{
private:
	COLORREF currentColor;           //画笔/画刷当前颜色
	int currentWidth;                //画笔当前宽度
	HCURSOR mouseCur;                //鼠标光标
	DRAWTYPE drawType;               //画图类型
	DRAWMODE drawMode;               //画图模式（非填充/消除填充/填充）
	CPoint mouseLDown;               //鼠标左键按下时的坐标
	CPoint mouseLUp;                 //鼠标左键松开时的坐标
	//Metafile *bufferMetafile;      //图像缓冲（使用图元来记录作画图像）
	CBitmap *pCBitmap;               //图像缓冲指针
	HBITMAP hBitmap;                 //图像缓冲句柄（使用DDB来记录作画图像）
	HBITMAP tmpBitmap;               //临时图像（回放功能）
	CSize DDBSize;                   //图像大小
	BOOL bFlag;                      //初始化标记
	BOOL bDrawDrag;                  //拖曳标记
	CClientDC *DragDC;               //拖曳用DC
	CDC *MemDC;                      //虚拟DC
	CPen *DragPen;                   //拖曳用画笔
	CBrush *DragBrush;               //拖曳用画刷
	float percentX;                  //圆角矩形的圆角程度（水平方向）
	float percentY;                  //圆角矩形的圆角程度（垂直方向）
	CList <CPoint,CPoint&> PointList;//多边形/贝塞尔曲线顶点列表
	BOOL bDrawPolygon;               //多边形/贝塞尔曲线标记
	std::vector <PointSet> PointSets;//贝塞尔曲线控制顶点覆盖区域
	BOOL DragBezier;                 //贝塞尔曲线拖曳标记
	POSITION DragBezierPos;          //命中贝塞尔曲线控制点的位置（CList）
	int DragBezierLoc;               //命中贝塞尔曲线控制点的位置（vector）
	BOOL bTracePolygon;              //多边形构造标记
	BOOL isUndo;                     //回放标记
	CString FileName;                //文件名
	CPoint WndportCenter;            //窗口中心点坐标
	CPoint ViewportOrg;              //视口原点（窗口坐标系的坐标）

public: // 仅从序列化创建
	CChildView();
protected:
	DECLARE_DYNCREATE(CChildView)

// 属性
public:

// 操作
public:
	void ClearCanvas(CDC* pDC);
	void DrawPolygon(CDC *pDC);
	void DrawPolyBezier(CDC *pDC,COLORREF theColor,BOOL bVirtualDC=FALSE);
	void DrawControlPoint(CDC *pDC,CPoint ctrlPoint);
	void DrawVirControlPoint(CDC *pDC,CPoint ctrlPoint);
	CRect returnDrawRect(void);
	void ChangeCanvasSize(CSize *newsize);
	void UpDownReverse(CBitmap *theBitmap);
	BOOL HitCtrlPoint(CPoint HitPoint);
	void dc2logic(CPoint &cPoint);
	void logic2dc(CPoint &cPoint);
	CPoint logic2virtualDC(CPoint cPoint);
	CPoint dc2virtualDC(CPoint cPoint);
	CPoint view2logic(CPoint cPoint);

// 重写
protected:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CChildView();

// 生成的消息映射函数
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);            //按下鼠标左键
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);              //按住鼠标左键移动
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);              //松开鼠标左键
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point);          //双击鼠标左键
	afx_msg BOOL OnSetCursor(CWnd *pWnd,UINT nHitTest,UINT message); //设置鼠标光标
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);             //点击鼠标右键
	afx_msg void OnEditUndo();
	afx_msg void OnEditSize();
	afx_msg void OnEditClear();
	afx_msg void OnEditPen();
	afx_msg void OnEditLine();
	afx_msg void OnEditRect();
	afx_msg void OnEditRoundrect();
	afx_msg void OnEditEllipse();
	afx_msg void OnEditPolygon();
	afx_msg void OnEditBezier();
	afx_msg void OnEditColor();
	afx_msg void OnEditNotfilling();
	afx_msg void OnEditErasefilling();
	afx_msg void OnEditFullfilling();
	afx_msg void OnEditRounddegree();
	afx_msg void OnPaintFileOpen();
	afx_msg void OnPaintFileSave();
	afx_msg void OnEditOption();
	afx_msg void OnEditPensize();
	afx_msg void OnImageUdreverse();
	afx_msg void OnImageLrreverse();
	afx_msg void OnImageCorreverse();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditNotfilling(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditErasefilling(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFullfilling(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRoundrect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPolygon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditBezier(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRounddegree(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};


