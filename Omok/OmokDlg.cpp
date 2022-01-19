#include "pch.h"
#include "framework.h"
#include "Omok.h"
#include "OmokDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


COmokDlg::COmokDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OMOK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void COmokDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COmokDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(COmokCtrl::eEvent::Event_Clicked, &COmokDlg::ClickedOmok)
	ON_BN_CLICKED(IDC_BTN_RESET, &COmokDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


BOOL COmokDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitControls();

	return TRUE; 
}

void COmokDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void COmokDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR COmokDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COmokDlg::InitControls()
{
	// 오목 컨트롤 초기화
	m_ctrlOmok.SubclassDlgItem(IDC_OMOK, this);
	m_ctrlOmok.Init(19, 19, GetSafeHwnd());
	m_ctrlOmok.StopGame();

	// XColorStatic 초기화
	m_arrStatic[Static_CurTurn].SubclassDlgItem(IDC_STATIC_CUR_TURN, this);
	m_arrStatic[Static_CurTurn].SetBackgroundColor(RGB(255, 192, 192));
	m_arrStatic[Static_CurTurn].SetFont(_T("Arial"), 130);
	SetDlgItemText(IDC_STATIC_CUR_TURN, _T("흰"));

	// ListBox 초기화
	m_ctrlListLog.SubclassDlgItem(IDC_LIST_LOG, this);
}


/**
플레이어 변경 이벤트
(마우스로 클릭했을 때 돌이 변경될 때 플레이어 변경)
@param		wParam		클릭한 플레이어
@param		lParam		클릭된 위치의 돌이 위치하는 인덱스
*/
LRESULT COmokDlg::ClickedOmok(WPARAM wParam, LPARAM lParam)
{
	COmokCtrl::ePlayer eClickedPlayer = (COmokCtrl::ePlayer)wParam;
	COmokCtrl::ePlayer eNextPlayer = eClickedPlayer == COmokCtrl::ePlayer::Player_Rock1 ? COmokCtrl::ePlayer::Player_Rock2 : COmokCtrl::ePlayer::Player_Rock1;
	int nIdx = (int)lParam;

	int nX = 0;
	int nY = 0;
	bool bIsReturn = m_ctrlOmok.GetPoint(eClickedPlayer, nIdx, nX, nY);
	if (bIsReturn)
	{
		CString strLog = _T("");
		if (eClickedPlayer == COmokCtrl::ePlayer::Player_Rock1)
			strLog.Format(_T("흑 : %d, %d"), nX, nY);
		else
			strLog.Format(_T("백 : %d, %d"), nX, nY);
		m_ctrlListLog.AddString(strLog);
		m_ctrlListLog.SetCurSel(m_ctrlListLog.GetCount() - 1);
	}

	if (eNextPlayer == COmokCtrl::ePlayer::Player_Rock2)
	{
		SetDlgItemText(IDC_STATIC_CUR_TURN, _T("흰"));
		m_arrStatic[Static_CurTurn].SetFont(_T("Arial"), 130);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_CUR_TURN, _T("검"));
		m_arrStatic[Static_CurTurn].SetFont(_T("Arial"), 130);
	}

	return 1L;
}


/**
초기화 버튼
*/
void COmokDlg::OnBnClickedBtnReset()
{
	// 오목 컨트롤 초기화
	m_ctrlOmok.ResetRock();
	Invalidate();

	// 현재 플레이어를 보여주는 Static 초기화
	SetDlgItemText(IDC_STATIC_CUR_TURN, _T("검"));
	m_arrStatic[Static_CurTurn].SetFont(_T("Arial"), 130);

	// ListBox 초기화
	m_ctrlListLog.ResetContent();
}
