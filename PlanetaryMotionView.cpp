// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PlanetaryMotionView.cpp : implementation of the CPlanetaryMotionView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlanetaryMotion.h"
#endif

#include "PlanetaryMotionDoc.h"
#include "PlanetaryMotionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlanetaryMotionView

IMPLEMENT_DYNCREATE(CPlanetaryMotionView, CView)

BEGIN_MESSAGE_MAP(CPlanetaryMotionView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPlanetaryMotionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_START, &CPlanetaryMotionView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_SUSPEND, &CPlanetaryMotionView::OnButtonSuspend)
	ON_COMMAND(ID_BUTTON_RESUME, &CPlanetaryMotionView::OnButtonResume)
END_MESSAGE_MAP()

// CPlanetaryMotionView construction/destruction

CPlanetaryMotionView::CPlanetaryMotionView() noexcept
{
	m_pCurrentThread = NULL;
	m_iCounter = 0;
	m_Center = CPoint(600, 300);

	m_EarthVelocity = 10.0;
	m_EarthDistance = 130;

	m_MarsVelocity = 7.0;
	m_MarsDistance = 200;

}

CPlanetaryMotionView::~CPlanetaryMotionView()
{
}

BOOL CPlanetaryMotionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPlanetaryMotionView drawing

void CPlanetaryMotionView::OnDraw(CDC* pDC)
{
	CPlanetaryMotionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString cstrData;
	cstrData.Format(_T("The Counter: %d"), m_iCounter);

	pDC->TextOut(0, 0, cstrData);


	// draw Mars's Orbit
	DrawOrbit(pDC, m_MarsDistance, m_Center, RGB(0, 0, 150));

	// draw Earth's Orbit
	DrawOrbit(pDC, m_EarthDistance, m_Center, RGB(0, 150, 0));

	// draw sun
	DrawEntity(pDC, 30, 30, m_Center, RGB(255, 204, 51));

	
	if (m_EarthPointArray.size() > 1) {
		CPoint point = m_EarthPointArray.front();
		m_EarthPointArray.pop_front();

		//For Earth
		DrawEntity(pDC, 15, 10, point, RGB(128, 96, 67));
	}

	if (m_MarsPointArray.size() > 1) {
		CPoint point = m_MarsPointArray.front();
		m_MarsPointArray.pop_front();

		//For Mars
		DrawEntity(pDC, 15, 10, point, RGB(188, 39, 49));
	}
	
}


// CPlanetaryMotionView printing


void CPlanetaryMotionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPlanetaryMotionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPlanetaryMotionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPlanetaryMotionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPlanetaryMotionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPlanetaryMotionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPlanetaryMotionView diagnostics

#ifdef _DEBUG
void CPlanetaryMotionView::AssertValid() const
{
	CView::AssertValid();
}

void CPlanetaryMotionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlanetaryMotionDoc* CPlanetaryMotionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlanetaryMotionDoc)));
	return (CPlanetaryMotionDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlanetaryMotionView message handlers

void CPlanetaryMotionView::OnButtonStart()
{
	//AfxMessageBox(_T("Start"));
	if(!m_pCurrentThread)
		m_pCurrentThread = AfxBeginThread(CPlanetaryMotionView::StartFunction, this);
}


void CPlanetaryMotionView::OnButtonSuspend()
{
	//AfxMessageBox(_T("Suspend"));
	if(m_pCurrentThread)
		m_pCurrentThread->SuspendThread();
}

void CPlanetaryMotionView::OnButtonResume()
{
	//AfxMessageBox(_T("Resume"));
	if (m_pCurrentThread)
		m_pCurrentThread->ResumeThread();
}


void CPlanetaryMotionView::DrawEntity(CDC* pDC, int xSize, int ySize, CPoint point , COLORREF color) {
	
	CBrush brush(color);
	CPen pen(0, 1, color);
	CBrush* p_OldBrush = pDC->SelectObject(&brush);
	CPen* p_OldPen = pDC->SelectObject(&pen);

	pDC->Ellipse(point.x - xSize, point.y - ySize, point.x + xSize, point.y + ySize);

	pDC->SelectObject(p_OldBrush);
	pDC->SelectObject(p_OldPen);
}

void CPlanetaryMotionView::DrawOrbit(CDC* pDC, int xSize, CPoint point, COLORREF color){
	CPen pen(0, 5, color);
	CPen* p_OldPen = pDC->SelectObject(&pen);

	pDC->Ellipse(point.x - xSize, point.y - xSize, point.x + xSize, point.y + xSize);

	pDC->SelectObject(p_OldPen);
}


UINT CPlanetaryMotionView::StartFunction(LPVOID parm)
{
	CPlanetaryMotionView* pmView = (CPlanetaryMotionView*)parm;
	
	//v = R * omega
	double earth_omega = pmView->m_EarthVelocity / ((pmView->m_EarthDistance + pmView->m_EarthDistance +10)/2);
	double mars_omega = pmView->m_MarsVelocity / pmView->m_MarsDistance;
	
	
	/*
	x(t)	=	R * cos(omega * t)
	y(t)	=	R * sin(omega * t)
	*/

	CPoint point;

	while (true) {
		pmView->m_iCounter += 1;


		// For Earth
		point.x = pmView->m_Center.x + pmView->m_EarthDistance * cos(earth_omega * pmView->m_iCounter);
		point.y = pmView->m_Center.y + pmView->m_EarthDistance * sin(earth_omega * pmView->m_iCounter);

		pmView->m_EarthPointArray.push_back(point);
		
		//For Mars
		point.x = pmView->m_Center.x + pmView->m_MarsDistance * cos(mars_omega * pmView->m_iCounter);
		point.y = pmView->m_Center.y + pmView->m_MarsDistance * sin(mars_omega * pmView->m_iCounter);

		pmView->m_MarsPointArray.push_back(point);
		
		pmView->Invalidate();
		Sleep(100);
	}

	return 0;
}
