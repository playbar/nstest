#pragma once
#include "afxwin.h"

class CAccEnu :
	public CButton
{
public:
	CAccEnu(void);
	~CAccEnu(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void Init();
protected:
	DECLARE_MESSAGE_MAP()	//afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	int	_enuW,_enuH;
private:
	double getRotateX(double x,double y);
	double getRotateY(double x,double y);
	double getRotateZ(double x,double y);
	void DrawEnu(CDC*pDC);
	double _accX,_accY,_accZ;
	double _gAccX,_gAccY,_gAccZ;
};
