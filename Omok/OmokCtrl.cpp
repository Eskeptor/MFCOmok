#include "pch.h"
#include "OmokCtrl.h"
#include <algorithm>


COmokCtrl::COmokCtrl() :
	m_hParent(nullptr),
	m_bRockToggle(true),
	m_fGdiBtnLeftTopX(0.0f), m_fGdiBtnLeftTopY(0.0f)
{
	m_stPlayers[Player_Rock1].ePlayerIdx = Player_Rock1;
	m_stPlayers[Player_Rock2].ePlayerIdx = Player_Rock2;
}


COmokCtrl::~COmokCtrl()
{

}

BEGIN_MESSAGE_MAP(COmokCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void COmokCtrl::OnPaint()
{
	CPaintDC dc(this);
					  
	Bitmap mBitmap(m_rcWindow.Width(), m_rcWindow.Height());
	GPaint G(dc);
	GPaint mDC(&mBitmap);

	DrawOmok(&mDC);
	DrawMiniWindow(&mDC);

	G.DrawImage(&mBitmap, 0, 0);
}


/**
오목을 그리는 함수
*/
void COmokCtrl::DrawOmok(GPaint* pDC)
{
	// ==============================================================================================================
	// 예외 선언
	if (pDC == nullptr)
		return;
	// ==============================================================================================================


	// ==============================================================================================================
	// 기본 베이스 그리기
	SolidBrush brBaseBgrd(m_stSettings.clrArr[BaseBackground]);
	pDC->FillRectangle(&brBaseBgrd, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
	// ==============================================================================================================


	// ==============================================================================================================
	// 모눈 그리기
	SolidBrush brPanBgrd(m_stSettings.clrArr[PanBackground]);
	Pen penPanLine(m_stSettings.clrArr[PanLine]);

	GFloat fXDiv = (GFloat)(m_rcWindow.Width() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nXLine - 1);
	GFloat fYDiv = (GFloat)(m_rcWindow.Height() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nYLine - 1);
	GFloat fPanWidth = fXDiv * (GFloat)(m_stSettings.nXLine - 1);
	GFloat fPanHeight = fYDiv * (GFloat)(m_stSettings.nYLine - 1);

	pDC->FillRectangle(&brPanBgrd,
		(GFloat)m_stSettings.nMargin,
		(GFloat)m_stSettings.nMargin,
		fPanWidth,
		fPanHeight);

	for (int nX = 0; nX < m_stSettings.nXLine; nX++)
	{
		pDC->DrawLine(&penPanLine,
			(GFloat)m_stSettings.nMargin + ((GFloat)nX * fXDiv),
			(GFloat)m_stSettings.nMargin,
			(GFloat)m_stSettings.nMargin + ((GFloat)nX * fXDiv),
			fPanHeight + m_stSettings.nMargin);
	}
	for (int nY = 0; nY < m_stSettings.nYLine; nY++)
	{
		pDC->DrawLine(&penPanLine,
			(GFloat)m_stSettings.nMargin,
			(GFloat)m_stSettings.nMargin + ((GFloat)nY * fYDiv),
			fPanWidth + m_stSettings.nMargin,
			(GFloat)m_stSettings.nMargin + ((GFloat)nY * fYDiv));
	}
	
	// ==============================================================================================================


	// ==============================================================================================================
	// 돌 그리기
	int nClrBgrdIdx = 0;
	int nClrLineIdx = 0;
	for (int i = 0; i < PlayerMax; i++)
	{
		if (i == Player_Rock1)
		{
			nClrBgrdIdx = RockBackground1;
			nClrLineIdx = RockLine1;
		}
		else
		{
			nClrBgrdIdx = RockBackground2;
			nClrLineIdx = RockLine2;
		}

		SolidBrush brRockBgrd(m_stSettings.clrArr[nClrBgrdIdx]);
		Pen penRockLine(m_stSettings.clrArr[nClrLineIdx]);

		auto iter = m_stPlayers[i].vRocks.begin();
		int nIdx = -1;
		while (iter != m_stPlayers[i].vRocks.end())
		{
			nIdx = iter->nY * m_stSettings.nYLine + iter->nX;
			if (nIdx >= 0 &&
				nIdx < (int)m_vMousePoint.size())
			{
				pDC->FillEllipse(&brRockBgrd,
					m_vMousePoint.at(nIdx).fLeftTopX,
					m_vMousePoint.at(nIdx).fLeftTopY,
					m_stSettings.fRockWidth,
					m_stSettings.fRockHeight);
				pDC->DrawEllipse(&penRockLine,
					m_vMousePoint.at(nIdx).fLeftTopX,
					m_vMousePoint.at(nIdx).fLeftTopY,
					m_stSettings.fRockWidth,
					m_stSettings.fRockHeight);
			}
			iter++;
		}
	}
	// ==============================================================================================================

}


/**
오목판 한 가운데 작은 윈도우를 그리는 함수
*/
void COmokCtrl::DrawMiniWindow(GPaint* pDC)
{
	// ==============================================================================================================
	// 예외 선언
	if (pDC == nullptr)
		return;

	if (m_stSettings.eCurGameStatus == eGameStatus::GameStart)
		return;
	// ==============================================================================================================


	// ==============================================================================================================
	// 윈도우 요소 계산
	GFloat fMainWinWidthHalf = m_rcWindow.Width() / 2.0f;
	GFloat fMainWinHeightHalf = m_rcWindow.Height() / 2.0f;
	GFloat fWindowLeftTopX = fMainWinWidthHalf - (m_stSettings.fMiniWinWidth / 2.0f);
	GFloat fWindowLeftTopY = fMainWinHeightHalf - (m_stSettings.fMiniWinHeight / 2.0f);
	// ==============================================================================================================


	// ==============================================================================================================
	// 윈도우 배경 그리기
	SolidBrush brWindowBgrd(m_stSettings.clrArr[WinBackgournd]);
	pDC->FillRectangle(&brWindowBgrd, fWindowLeftTopX, fWindowLeftTopY, m_stSettings.fMiniWinWidth, m_stSettings.fMiniWinHeight);
	// ==============================================================================================================


	// ==============================================================================================================
	// 문구 넣기
	CString strText = _T("");
	if (m_stSettings.eCurGameStatus == eGameStatus::GamePause)
		strText = _T("게임 일시 정지");
	else if (m_stSettings.eCurGameStatus == eGameStatus::GameStop)
		strText = _T("게임 시작");

	m_stSettings.fMiniWinFontSize = 24.0f;

	GFloat fTextCenterX = fMainWinWidthHalf;
	GFloat fTextCenterY = fWindowLeftTopY + (m_stSettings.fMiniWinFontSize * 2.0f);

	WriteGdiPText(pDC, 
		strText, 
		fTextCenterX,
		fTextCenterY,
		m_stSettings.fMiniWinFontSize, 
		m_stSettings.clrArr[WinFont]);
	// ==============================================================================================================


	// ==============================================================================================================
	// 버튼 넣기
	if (m_stSettings.eCurGameStatus == eGameStatus::GamePause)
		strText = _T("계속 하기");
	else if (m_stSettings.eCurGameStatus == eGameStatus::GameStop)
		strText = _T("시작");

	m_fGdiBtnLeftTopX = fMainWinWidthHalf - (m_stSettings.fMiniWinBtnWidth / 2.0f);
	m_fGdiBtnLeftTopY = (fWindowLeftTopY + m_stSettings.fMiniWinHeight) - 20.0f - m_stSettings.fMiniWinBtnHeight;
	
	SolidBrush brBtnBgrd(m_stSettings.clrArr[WinButton]);
	pDC->FillRectangle(&brBtnBgrd, m_fGdiBtnLeftTopX, m_fGdiBtnLeftTopY, m_stSettings.fMiniWinBtnWidth, m_stSettings.fMiniWinBtnHeight);

	float fBtnFontSize = m_stSettings.fMiniWinFontSize;
	fTextCenterX = m_fGdiBtnLeftTopX + (m_stSettings.fMiniWinBtnWidth / 2.0f);
	fTextCenterY = m_fGdiBtnLeftTopY + (m_stSettings.fMiniWinBtnHeight / 2.0f);
	
	WriteGdiPText(pDC,
		strText,
		fTextCenterX,
		fTextCenterY + 2.0f,
		fBtnFontSize,
		m_stSettings.clrArr[WinButtonFont]);
	// ==============================================================================================================
}


/**
GDI+ 텍스트를 쓰는 함수
@param		strText		텍스트
@param		fX			텍스트 박스의 X좌표
@param		fY			텍스트 박스의 Y좌표
@param		fFontSize	텍스트 폰트 사이즈
@param		clrFont		텍스트 색상
@param		bIsCenter	텍스트 정렬(기본값 : true)
*/
void COmokCtrl::WriteGdiPText(GPaint* pDC, 
	CString strText, 
	GFloat fX, 
	GFloat fY, 
	GFloat fFontSize,
	GColor clrFont,
	bool bIsCenter)
{
	if (pDC == nullptr)
		return;

	StringFormat stringFormat;
	GFont font(_T("Arial"), fFontSize, FontStyleBold, UnitPixel);

	GFloat fTextLength = 0.0f;
	GFloat fTextHeight = 0.0f;
	GetTextSizeToPixel(strText, &font, fTextLength, fTextHeight);

	RectF rect;
	if (bIsCenter)
	{
		rect.X = fX - (fTextLength / 2.0f);
		rect.Y = fY - (fTextHeight / 2.0f);
	}
	else
	{
		rect.X = fX;
		rect.Y = fY;
	}
	rect.Width = fTextLength;
	rect.Height = fTextHeight;

	pDC->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	SolidBrush brFont(clrFont);
	pDC->DrawString(strText, -1, &font, rect, &stringFormat, &brFont);
}


/**
해당 문장의 폰트 크기를 Pixel 값으로 반환하는 함수
(http://codewee.com/view.php?idx=48)
@param		strText			텍스트
@param		font			폰트
@param		outTextWidth	계산한 가로길이
@param		outTextHeight	계산한 세로길이
*/
void COmokCtrl::GetTextSizeToPixel(const TCHAR* strText, GFont* font, GFloat& outTextWidth, GFloat& outTextHeight)
{
	RectF rtBound;
	GBitmap bitmap(4, 4);
	GPaint* graphic = GPaint::FromImage(&bitmap);
	graphic->MeasureString(strText, (INT)_tcslen(strText), font, PointF(0, 0), &rtBound);

	outTextWidth = rtBound.Width;
	outTextHeight = rtBound.Height;

	delete graphic;
	graphic = nullptr;
}


/**
컨트롤 생성시 꼭 넣어주어야 하는 함수 (초기화용)
@param		nXLine		가로 줄수 (기본값 : Default_XLine)
@param		nYLine		세로 줄수 (기본값 : Default_YLine)
@param		hParent		이벤트를 넘겨줄 부모 컨트롤 (기본값 : nullptr)
*/
void COmokCtrl::Init(int nXLine, int nYLine, HWND hParent)
{
	// 윈도우 Rect
	GetWindowRect(m_rcWindow);
	ScreenToClient(m_rcWindow);

	// 이벤트용 HWND
	m_hParent = hParent;

	// 설정값
	m_stSettings.nXLine = nXLine;
	m_stSettings.nYLine = nYLine;

	// 마우스 포인터 초기화
	InitMousePoint();

	// 초기화 이후 한 번 현재 플레이어 이벤트
	if (m_hParent)
		::PostMessage(m_hParent, eEvent::Event_Clicked, ePlayer::Player_Rock1, 0);
}


/**
마우스 클릭 이벤트
*/
void COmokCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_stSettings.eCurGameStatus == eGameStatus::GameStart)
	{
		if (m_stSettings.bIsClickEnabled)
		{
			int nX = -1;
			int nY = -1;
			int nIdx = -1;
			ePlayer eCurPlayer = ePlayer::Player_None;
			if (m_bRockToggle)
				eCurPlayer = ePlayer::Player_Rock1;
			else
				eCurPlayer = ePlayer::Player_Rock2;

			if (HitCheck(point, &nX, &nY, false))
			{
				if (m_stOmokPan[nY][nX].ePlayerIdx == ePlayer::Player_None)
				{
					nIdx = nY * Default_YLine + nX;
					stRock rock = { nX, nY, eCurPlayer };

					m_stOmokPan[nY][nX].ePlayerIdx = eCurPlayer;
					m_vMousePoint.at(nIdx).eStatus = eCurPlayer;
					m_stPlayers[eCurPlayer].vRocks.push_back(rock);

					m_bRockToggle = !m_bRockToggle;
					if (m_hParent)
						::PostMessage(m_hParent, eEvent::Event_Clicked, eCurPlayer, m_stPlayers[eCurPlayer].vRocks.size() - 1);

					if (CheckOMok() != ePlayer::Player_None)
						m_stSettings.eCurGameStatus = eGameStatus::GameStop;
				}
			}

			Invalidate();
		}
	}
	else
	{
		if (HitCheck(point, NULL, NULL, true))
		{
			if (m_stSettings.eCurGameStatus == eGameStatus::GameStop)
			{
				//::PostMessage(m_hParent, eEvent::Event_Clicked, eCurPlayer, m_stPlayers[eCurPlayer].vRocks.size() - 1);
				ResetRock();
			}

			m_stSettings.eCurGameStatus = eGameStatus::GameStart;
			Invalidate();
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}


/**
마우스 포인터 초기화
*/
void COmokCtrl::InitMousePoint()
{
	GFloat fXDiv = (GFloat)(m_rcWindow.Width() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nXLine - 1);
	GFloat fYDiv = (GFloat)(m_rcWindow.Height() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nYLine - 1);
	GFloat fPanWidth = fXDiv * (GFloat)(m_stSettings.nXLine - 1);
	GFloat fPanHeight = fYDiv * (GFloat)(m_stSettings.nYLine - 1);

	// 돌 크기 설정
	m_stSettings.fRockWidth = fXDiv;
	m_stSettings.fRockHeight = fXDiv;

	GFloat fCenterX = 0.0f;
	GFloat fCenterY = 0.0f;
	for (int nY = 0; nY < m_stSettings.nYLine; nY++)
	{
		for (int nX = 0; nX < m_stSettings.nXLine; nX++)
		{
			fCenterX = (GFloat)m_stSettings.nMargin + ((GFloat)nX * fXDiv);
			fCenterY = (GFloat)m_stSettings.nMargin + ((GFloat)nY * fYDiv);

			stMousePoint stMouse;
			stMouse.fLeftTopX = fCenterX - (m_stSettings.fRockWidth / 2.0f);
			stMouse.fLeftTopY = fCenterY - (m_stSettings.fRockHeight / 2.0f);
			stMouse.fRightBotX = fCenterX + (m_stSettings.fRockWidth / 2.0f);
			stMouse.fRightBotY = fCenterY + (m_stSettings.fRockHeight / 2.0f);

			m_vMousePoint.push_back(stMouse);
		}
	}
}


/**
마우스 클릭 유효 체크
@param		point			마우스 클릭 포인트
@param		outX			유효한 클릭 포인트일때 클릭한 위치의 X 배열 인덱스
@param		outY			유효한 클릭 포인트일때 클릭한 위치의 Y 배열 인덱스
@param		bIsBtnCheck		GDI로 생성한 버튼 클릭 체크 유무
*/
bool COmokCtrl::HitCheck(CPoint point, int* outX, int* outY, bool bIsBtnCheck)
{
	if (bIsBtnCheck)
	{
		if (point.x >= m_fGdiBtnLeftTopX &&
			point.x <= m_fGdiBtnLeftTopX + m_stSettings.fMiniWinBtnWidth &&
			point.y >= m_fGdiBtnLeftTopY &&
			point.y <= m_fGdiBtnLeftTopY + m_stSettings.fMiniWinBtnHeight)
		{
			return true;
		}
	}
	else
	{
		int nIdx = 0;
		for (int nY = 0; nY < m_stSettings.nYLine; nY++)
		{
			for (int nX = 0; nX < m_stSettings.nXLine; nX++)
			{
				nIdx = nY * m_stSettings.nYLine + nX;
				if (nIdx < 0 ||
					nIdx >= (int)m_vMousePoint.size())
					return false;

				if (point.x >= m_vMousePoint.at(nIdx).fLeftTopX &&
					point.x <= m_vMousePoint.at(nIdx).fRightBotX &&
					point.y >= m_vMousePoint.at(nIdx).fLeftTopY &&
					point.y <= m_vMousePoint.at(nIdx).fRightBotY)
				{
					*outX = nX;
					*outY = nY;
					return true;
				}
			}
		}
	}

	return false;
}


/**
돌 초기화
*/
void COmokCtrl::ResetRock()
{
	m_stPlayers[Player_Rock1].vRocks.clear();
	m_stPlayers[Player_Rock2].vRocks.clear();

	for_each(m_vMousePoint.begin(), m_vMousePoint.end(), [](stMousePoint& point) { point.eStatus = Player_None; });

	for (int nY = 0; nY < Default_YLine; nY++)
	{
		for (int nX = 0; nX < Default_XLine; nX++)
			m_stOmokPan[nY][nX].ePlayerIdx = ePlayer::Player_None;
	}

	m_bRockToggle = true;

	Invalidate();
}


/**
현재 돌의 개수
@param		ePlay		해당하는 돌
*/
int COmokCtrl::GetRockCount(ePlayer ePlay)
{
	return m_stPlayers[Player_Rock1].vRocks.size();
}


/**
해당 인덱스(nIdx)에 대한 점을 반환
@param		ePlay		해당하는 돌
@param		nIdx		인덱스
@param		outX		X 위치 (반환값)
@param		outY		Y 위치 (반환값)
*/
bool COmokCtrl::GetPoint(ePlayer ePlay, int nIdx, int& outX, int& outY)
{
	if (ePlay <= ePlayer::Player_None ||
		ePlay >= ePlayer::PlayerMax)
		return false;

	if (nIdx < 0 ||
		nIdx >= (int)m_stPlayers[ePlay].vRocks.size())
		return false;

	outX = m_stPlayers[ePlay].vRocks.at(nIdx).nX;
	outY = m_stPlayers[ePlay].vRocks.at(nIdx).nY;

	return true;
}


/**
해당 플레이어(ePlay)의 Tick Count를 반환
*/
int COmokCtrl::GetTick(ePlayer ePlay)
{
	if (ePlay <= ePlayer::Player_None ||
		ePlay >= ePlayer::PlayerMax)
		return 0;

	return m_stPlayers[ePlay].dwTick;
}


/**
게임 종료
*/
void COmokCtrl::StopGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GameStop;
}


/**
게임 시작
*/
void COmokCtrl::StartGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GameStart;
}


/**
게임 일시정지
*/
void COmokCtrl::PauseGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GamePause;
}


/**
게임 상태 반환
*/
COmokCtrl::eGameStatus COmokCtrl::GetGameStatus()
{
	return m_stSettings.eCurGameStatus;
}


/**
오목 승자 확인 체크 알고리즘
*/
COmokCtrl::ePlayer COmokCtrl::CheckOMok()
{
	ePlayer eWinner = ePlayer::Player_None;

	int nResult = 0;
	for (int nY = 0; nY < Default_YLine; nY++)
	{
		for (int nX = 0; nX < Default_XLine; nX++)
		{
			ePlayer ePlayerIdx = m_stOmokPan[nY][nX].ePlayerIdx;

			if (ePlayerIdx != ePlayer::Player_None)
			{
				if (nX - 1 >= 0 &&
					nY + 1 < Default_YLine)
					nResult = CheckOMok_Go(eFindPos::FindLeftDown, nX, nY, ePlayerIdx);
				if (nResult == 5)
					eWinner = ePlayerIdx;

				if (nY + 1 < Default_YLine)
					nResult = CheckOMok_Go(eFindPos::FindDown, nX, nY, ePlayerIdx);
				if (nResult == 5)
					eWinner = ePlayerIdx;

				if (nX + 1 < Default_XLine &&
					nY + 1 < Default_YLine)
					nResult = CheckOMok_Go(eFindPos::FindRightDown, nX, nY, ePlayerIdx);
				if (nResult == 5)
					eWinner = ePlayerIdx;

				if (nX + 1 < Default_XLine)
					nResult = CheckOMok_Go(eFindPos::FindRight, nX, nY, ePlayerIdx);
				if (nResult == 5)
					eWinner = ePlayerIdx;
			}

			if (eWinner != ePlayer::Player_None)
				break;
		}

		if (eWinner != ePlayer::Player_None)
			break;
	}

	return eWinner;
}


/**
오목 승자 확인 체크 알고리즘
*/
int COmokCtrl::CheckOMok_Go(eFindPos ePos, int nX, int nY, ePlayer ePlayerIdx)
{
	int nResult = 0;

	if (m_stOmokPan[nY][nX].ePlayerIdx == ePlayerIdx)
	{
		nResult = 1;
		switch (ePos)
		{
			case eFindPos::FindLeftDown:
				if (nX - 1 >= 0 &&
					nY + 1 < Default_YLine)
					nResult += CheckOMok_Go(ePos, nX - 1, nY + 1, ePlayerIdx);
				break;
			case eFindPos::FindDown:
				if (nY + 1 < Default_YLine)
					nResult += CheckOMok_Go(ePos, nX, nY + 1, ePlayerIdx);
				break;
			case eFindPos::FindRightDown:
				if (nX + 1 < Default_XLine &&
					nY + 1 < Default_YLine)
					nResult += CheckOMok_Go(ePos, nX + 1, nY + 1, ePlayerIdx);
				break;
			case eFindPos::FindRight:
				if (nX + 1 < Default_XLine)
					nResult += CheckOMok_Go(ePos, nX + 1, nY, ePlayerIdx);
				break;
		}
	}

	return nResult;
}