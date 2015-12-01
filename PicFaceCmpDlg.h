// PicFaceCmpDlg.h : header file
//

#if !defined(AFX_PICFACECMPDLG_H__5DF1636F_33C2_48B2_974C_3E72793C782E__INCLUDED_)
#define AFX_PICFACECMPDLG_H__5DF1636F_33C2_48B2_974C_3E72793C782E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPicFaceCmpDlg dialog

class CPicFaceCmpDlg : public CDialog
{
// Construction
public:
	CPicFaceCmpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPicFaceCmpDlg)
	enum { IDD = IDD_PICFACECMP_DIALOG };
	CEdit	m_edtRes;
	CEdit	m_edtPic2;
	CEdit	m_edtPic1;
	CButton	m_btnBrow2;
	CButton	m_btnBrow1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicFaceCmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPicFaceCmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnBrow1();
	afx_msg void OnBtnBrow2();
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    void BrowsePicFile( int iPicFile );
    bool ComparePictures(float& fScore);
    bool TryInitFaceCmp();
    void PromptInfo( const char* pstrInfo );
private:
    CString m_strPicFile1;
    CString m_strPicFile2;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICFACECMPDLG_H__5DF1636F_33C2_48B2_974C_3E72793C782E__INCLUDED_)
