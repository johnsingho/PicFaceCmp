// PicFaceCmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PicFaceCmp.h"
#include "PicFaceCmpDlg.h"
#include "highgui.h"
#include "cv.h"
#include "Compare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicFaceCmpDlg dialog

CPicFaceCmpDlg::CPicFaceCmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicFaceCmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicFaceCmpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicFaceCmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicFaceCmpDlg)
	DDX_Control(pDX, IDC_EDIT_RES, m_edtRes);
	DDX_Control(pDX, IDC_EDIT_PIC2, m_edtPic2);
	DDX_Control(pDX, IDC_EDIT_PIC1, m_edtPic1);
	DDX_Control(pDX, IDC_BTN_BROW2, m_btnBrow2);
	DDX_Control(pDX, IDC_BTN_BROW1, m_btnBrow1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicFaceCmpDlg, CDialog)
	//{{AFX_MSG_MAP(CPicFaceCmpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROW1, OnBtnBrow1)
	ON_BN_CLICKED(IDC_BTN_BROW2, OnBtnBrow2)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicFaceCmpDlg message handlers

BOOL CPicFaceCmpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	TryInitFaceCmp();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPicFaceCmpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPicFaceCmpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPicFaceCmpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPicFaceCmpDlg::OnBtnBrow1() 
{
    PromptInfo("");
	BrowsePicFile(1);
}

void CPicFaceCmpDlg::OnBtnBrow2() 
{
    PromptInfo("");
	BrowsePicFile(2);	
}

void CPicFaceCmpDlg::OnOK() 
{
	if(m_strPicFile1.IsEmpty() || m_strPicFile2.IsEmpty())
    {
        MessageBox("请选择要对比的文件", "提示", MB_OK);
        return;
    }
    float fScore=0.0f;
    ComparePictures(fScore);
    CString strRes;
    strRes.Format("两张照片的相似度: %g%%", fScore*100.0);
    PromptInfo(strRes);
}

void CPicFaceCmpDlg::BrowsePicFile( int iPicFile)
{    
    TCHAR szFilters[] = _T ("位图|*.bmp|png|*.png|jpg图片|*.jpg||");
    CFileDialog fileDlg(TRUE, _T ("bmp"), _T ("*.bmp"), OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
    
    if(IDOK != fileDlg.DoModal())
    {
        return;
    }

    CString strFileName = fileDlg.GetPathName();
    if(1==iPicFile)
    {
        m_strPicFile1=strFileName;
        m_edtPic1.SetWindowText(m_strPicFile1);
    }else{
        m_strPicFile2=strFileName;
        m_edtPic2.SetWindowText(m_strPicFile2);
    }
}


bool CPicFaceCmpDlg::TryInitFaceCmp()
{
    int nRet = initialCompare();
    if(S_OK!=nRet)
    {
#ifndef USE_NEW_SDK
        const char* pstrErr = "人脸识别模块初始化失败！";
#else
        const char* pstrErr = "使用新版SDK，注意机器时间应该是2016年，否则结果有误！";
#endif        
        PromptInfo(pstrErr);
        return false;
    }
#ifndef USE_NEW_SDK
    PromptInfo("使用旧版SDK，注意机器时间应该是2015年，否则结果有误！");
#endif
    return true;
}

static void ResizeImg( IplImage* pImgSrc, IplImage* pImgDst)
{
    // 读取图片的宽和高;
    //     int w = pImgSrc->width;
    //     int h = pImgSrc->height;    
    // 对图片 img 进行缩放，并存入到 SrcImage 中;
    cvResize( pImgSrc, pImgDst);

    // 重置 SrcImage 的 ROI 准备读入下一幅图片;
    cvResetImageROI( pImgDst );
}


static void GetGrayPixel(IplImage* pImgPic, int nWidth, int nHeight, unsigned char* pOutPixel)
{    
    IplImage* pImgOper = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
    IplImage* pImgGray = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
    ResizeImg(pImgPic, pImgOper);
    cvCvtColor(pImgOper, pImgGray, CV_RGB2GRAY);
    cvReleaseImage(&pImgOper);
    
    TRACE1("***ImageSize=%d\n", pImgGray->imageSize);
    memcpy(pOutPixel, pImgGray->imageData, pImgGray->imageSize);
    cvReleaseImage(&pImgGray);
}

class CPicPixel
{
public:
    CPicPixel(IplImage* pImg, int w, int h):m_pPixel(NULL)
    {
        m_pPixel=CreatePicPixel(w, h);
    }
    ~CPicPixel()
    {
        DeletePicPixel(m_pPixel);
    }
    
    PICPIXEL* GetData()
    {
        return m_pPixel;
    }
private:
    PICPIXEL* m_pPixel;
};

bool CPicFaceCmpDlg::ComparePictures(float& fScore)
{
    IplImage* pImg1 = cvLoadImage(m_strPicFile1);
    if(!pImg1)
    {
        PromptInfo("加载图片1失败");
        return false;
    }
    IplImage* pImg2 = cvLoadImage(m_strPicFile2);
    if(!pImg2)
    {
        PromptInfo("加载图片2失败");
        cvReleaseImage(&pImg1);
        return false;
    }

    PICPIXEL* pbyPixel = NULL;
    CPicPixel pic1(pImg1, pImg1->width+10, pImg1->height+10);
    CPicPixel pic2(pImg2, pImg2->width+10, pImg2->height+10);
    pbyPixel = pic1.GetData();
    GetGrayPixel(pImg1, pbyPixel->width, pbyPixel->height, &pbyPixel->pixel[0]);
    pbyPixel = pic2.GetData();
    GetGrayPixel(pImg2, pbyPixel->width, pbyPixel->height, &pbyPixel->pixel[0]);
    
#ifndef USE_NEW_SDK
    extern HW_HANDLE MyHandle;
    fScore=TestCompare1V1(MyHandle, pic1.GetData(), pic2.GetData());
#else
    extern HW_HANDLE MyHandle;
    float fInit = 0.322F;
    fScore=TestCompare1V1(MyHandle, pic1.GetData(), pic2.GetData(), fInit, 0);
#endif

    cvReleaseImage(&pImg1);
    cvReleaseImage(&pImg2);
    PromptInfo("对比完成\n");
    return true;
}

void CPicFaceCmpDlg::PromptInfo( const char* pstrInfo)
{
    m_edtRes.SetWindowText(pstrInfo);
}

void CPicFaceCmpDlg::OnClose() 
{
	ReleaseEngine();	
	CDialog::OnClose();
}

