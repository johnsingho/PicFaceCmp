// PicFaceCmp.h : main header file for the PICFACECMP application
//

#if !defined(AFX_PICFACECMP_H__37239C22_09DD_43DF_BE71_D7E729C95A72__INCLUDED_)
#define AFX_PICFACECMP_H__37239C22_09DD_43DF_BE71_D7E729C95A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPicFaceCmpApp:
// See PicFaceCmp.cpp for the implementation of this class
//

class CPicFaceCmpApp : public CWinApp
{
public:
	CPicFaceCmpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicFaceCmpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPicFaceCmpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICFACECMP_H__37239C22_09DD_43DF_BE71_D7E729C95A72__INCLUDED_)
