#pragma once
#include "OmokCtrl.h"
#include "XColorStatic/XColorStatic.h"


class COmokDlg : public CDialogEx
{
// 열거형 및 구조체 ============================================================
private:
	enum eStatic
	{
		Static_CurTurn = 0,

		StaticMax
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OMOK_DIALOG };
#endif
// ===========================================================================


// 필드 영역 ==================================================================
private:
	COmokCtrl		m_ctrlOmok;
	CXColorStatic	m_arrStatic[StaticMax];
	CListBox		m_ctrlListLog;

protected:
	HICON m_hIcon;

// ===========================================================================


// 함수 영역 =================================================================
private:
	void InitControls();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT ClickedOmok(WPARAM wParam, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

public:
	COmokDlg(CWnd* pParent = nullptr);	

// ===========================================================================

	afx_msg void OnBnClickedBtnReset();
};
