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

// PlanetaryMotionView.h : interface of the CPlanetaryMotionView class
//

#pragma once
#include <queue>
#include <list>


class CPlanetaryMotionView : public CView
{
	CWinThread* m_pCurrentThread;
	

protected: // create from serialization only
	CPlanetaryMotionView() noexcept;
	DECLARE_DYNCREATE(CPlanetaryMotionView)

// Attributes
public:
	// the counter 
	int m_iCounter;
	CPoint m_Center;
	CPoint m_EarthCenter;

	double m_EarthVelocity;
	double m_MarsVelocity;

	int m_EarthDistance;
	int m_MarsDistance;

	std::list<CPoint> m_EarthPointArray;
	std::list<CPoint> m_MarsPointArray;

	CPlanetaryMotionDoc* GetDocument() const;

// Operations
public:

	static void DrawEntity(CDC* pDC, int xSize, int ySize, CPoint point, COLORREF color);
	static void DrawOrbit(CDC* pDC, int xSize, CPoint point, COLORREF color);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPlanetaryMotionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonStart();
	afx_msg void OnButtonSuspend();
	afx_msg void OnButtonResume();
	static UINT StartFunction(LPVOID parm);
};

#ifndef _DEBUG  // debug version in PlanetaryMotionView.cpp
inline CPlanetaryMotionDoc* CPlanetaryMotionView::GetDocument() const
   { return reinterpret_cast<CPlanetaryMotionDoc*>(m_pDocument); }
#endif

