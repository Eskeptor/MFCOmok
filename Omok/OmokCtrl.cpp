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
������ �׸��� �Լ�
*/
void COmokCtrl::DrawOmok(GPaint* pDC)
{
	// ==============================================================================================================
	// ���� ����
	if (pDC == nullptr)
		return;
	// ==============================================================================================================


	// ==============================================================================================================
	// �⺻ ���̽� �׸���
	SolidBrush brBaseBgrd(m_stSettings.clrArr[BaseBackground]);
	pDC->FillRectangle(&brBaseBgrd, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
	// ==============================================================================================================


	// ==============================================================================================================
	// �� �׸���
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
	// �� �׸���
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
������ �� ��� ���� �����츦 �׸��� �Լ�
*/
void COmokCtrl::DrawMiniWindow(GPaint* pDC)
{
	// ==============================================================================================================
	// ���� ����
	if (pDC == nullptr)
		return;

	if (m_stSettings.eCurGameStatus == eGameStatus::GameStart)
		return;
	// ==============================================================================================================


	// ==============================================================================================================
	// ������ ��� ���
	GFloat fMainWinWidthHalf = m_rcWindow.Width() / 2.0f;
	GFloat fMainWinHeightHalf = m_rcWindow.Height() / 2.0f;
	GFloat fWindowLeftTopX = fMainWinWidthHalf - (m_stSettings.fMiniWinWidth / 2.0f);
	GFloat fWindowLeftTopY = fMainWinHeightHalf - (m_stSettings.fMiniWinHeight / 2.0f);
	// ==============================================================================================================


	// ==============================================================================================================
	// ������ ��� �׸���
	SolidBrush brWindowBgrd(m_stSettings.clrArr[WinBackgournd]);
	pDC->FillRectangle(&brWindowBgrd, fWindowLeftTopX, fWindowLeftTopY, m_stSettings.fMiniWinWidth, m_stSettings.fMiniWinHeight);
	// ==============================================================================================================


	// ==============================================================================================================
	// ���� �ֱ�
	CString strText = _T("");
	if (m_stSettings.eCurGameStatus == eGameStatus::GamePause)
		strText = _T("���� �Ͻ� ����");
	else if (m_stSettings.eCurGameStatus == eGameStatus::GameStop)
		strText = _T("���� ����");

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
	// ��ư �ֱ�
	if (m_stSettings.eCurGameStatus == eGameStatus::GamePause)
		strText = _T("��� �ϱ�");
	else if (m_stSettings.eCurGameStatus == eGameStatus::GameStop)
		strText = _T("����");

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
GDI+ �ؽ�Ʈ�� ���� �Լ�
@param		strText		�ؽ�Ʈ
@param		fX			�ؽ�Ʈ �ڽ��� X��ǥ
@param		fY			�ؽ�Ʈ �ڽ��� Y��ǥ
@param		fFontSize	�ؽ�Ʈ ��Ʈ ������
@param		clrFont		�ؽ�Ʈ ����
@param		bIsCenter	�ؽ�Ʈ ����(�⺻�� : true)
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
�ش� ������ ��Ʈ ũ�⸦ Pixel ������ ��ȯ�ϴ� �Լ�
(http://codewee.com/view.php?idx=48)
@param		strText			�ؽ�Ʈ
@param		font			��Ʈ
@param		outTextWidth	����� ���α���
@param		outTextHeight	����� ���α���
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
��Ʈ�� ������ �� �־��־�� �ϴ� �Լ� (�ʱ�ȭ��)
@param		nXLine		���� �ټ� (�⺻�� : Default_XLine)
@param		nYLine		���� �ټ� (�⺻�� : Default_YLine)
@param		hParent		�̺�Ʈ�� �Ѱ��� �θ� ��Ʈ�� (�⺻�� : nullptr)
*/
void COmokCtrl::Init(int nXLine, int nYLine, HWND hParent)
{
	// ������ Rect
	GetWindowRect(m_rcWindow);
	ScreenToClient(m_rcWindow);

	// �̺�Ʈ�� HWND
	m_hParent = hParent;

	// ������
	m_stSettings.nXLine = nXLine;
	m_stSettings.nYLine = nYLine;

	// ���콺 ������ �ʱ�ȭ
	InitMousePoint();

	// �ʱ�ȭ ���� �� �� ���� �÷��̾� �̺�Ʈ
	if (m_hParent)
		::PostMessage(m_hParent, eEvent::Event_Clicked, ePlayer::Player_Rock1, 0);
}


/**
���콺 Ŭ�� �̺�Ʈ
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
���콺 ������ �ʱ�ȭ
*/
void COmokCtrl::InitMousePoint()
{
	GFloat fXDiv = (GFloat)(m_rcWindow.Width() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nXLine - 1);
	GFloat fYDiv = (GFloat)(m_rcWindow.Height() - (m_stSettings.nMargin * 2)) / (GFloat)(m_stSettings.nYLine - 1);
	GFloat fPanWidth = fXDiv * (GFloat)(m_stSettings.nXLine - 1);
	GFloat fPanHeight = fYDiv * (GFloat)(m_stSettings.nYLine - 1);

	// �� ũ�� ����
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
���콺 Ŭ�� ��ȿ üũ
@param		point			���콺 Ŭ�� ����Ʈ
@param		outX			��ȿ�� Ŭ�� ����Ʈ�϶� Ŭ���� ��ġ�� X �迭 �ε���
@param		outY			��ȿ�� Ŭ�� ����Ʈ�϶� Ŭ���� ��ġ�� Y �迭 �ε���
@param		bIsBtnCheck		GDI�� ������ ��ư Ŭ�� üũ ����
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
�� �ʱ�ȭ
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
���� ���� ����
@param		ePlay		�ش��ϴ� ��
*/
int COmokCtrl::GetRockCount(ePlayer ePlay)
{
	return m_stPlayers[Player_Rock1].vRocks.size();
}


/**
�ش� �ε���(nIdx)�� ���� ���� ��ȯ
@param		ePlay		�ش��ϴ� ��
@param		nIdx		�ε���
@param		outX		X ��ġ (��ȯ��)
@param		outY		Y ��ġ (��ȯ��)
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
�ش� �÷��̾�(ePlay)�� Tick Count�� ��ȯ
*/
int COmokCtrl::GetTick(ePlayer ePlay)
{
	if (ePlay <= ePlayer::Player_None ||
		ePlay >= ePlayer::PlayerMax)
		return 0;

	return m_stPlayers[ePlay].dwTick;
}


/**
���� ����
*/
void COmokCtrl::StopGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GameStop;
}


/**
���� ����
*/
void COmokCtrl::StartGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GameStart;
}


/**
���� �Ͻ�����
*/
void COmokCtrl::PauseGame()
{
	m_stSettings.eCurGameStatus = eGameStatus::GamePause;
}


/**
���� ���� ��ȯ
*/
COmokCtrl::eGameStatus COmokCtrl::GetGameStatus()
{
	return m_stSettings.eCurGameStatus;
}


/**
���� ���� Ȯ�� üũ �˰���
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
���� ���� Ȯ�� üũ �˰���
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