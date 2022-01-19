#pragma once
#include <vector>
#include <list>

using namespace std;

typedef Gdiplus::Color				GColor;
typedef Gdiplus::REAL				GFloat;
typedef Gdiplus::Graphics			GPaint;
typedef Gdiplus::Font				GFont;
typedef Gdiplus::StringAlignment	GStringAlign;
typedef Gdiplus::Bitmap				GBitmap;

class COmokCtrl : public CStatic
{
// ������ �� ����ü ============================================================
public:
	enum ePlayer
	{
		Player_None = -1,
		Player_Rock1,
		Player_Rock2,
		PlayerMax,
	};

	enum class eGameStatus
	{
		GameStop = 0,
		GameStart,
		GamePause,
	};

private:
	enum eDefaultSetting
	{
		Default_XLine = 19,
		Default_YLine = 19,
		Default_Margin = 26,

		Default_MiniW_Width = 300,
		Default_MiniW_Height = 200,
		Default_MiniW_Font = 14,
		Default_MiniW_BtnWidth = 160,
		Default_MiniW_BtnHeight = 24,
	};

	enum eColor
	{
		BaseBackground = 0,
		PanLine,
		PanBackground,
		RockLine1,
		RockBackground1,
		RockLine2,
		RockBackground2,
		WinBackgournd,
		WinLine,
		WinFont,
		WinButton,
		WinButtonFont,

		ColorMax
	};

	enum eFindPos
	{
		FindLeftDown = 0,
		FindDown,
		FindRightDown,
		FindRight,
		FindPosMax
	};

	// �� ����ü
	struct stRock
	{
		int nX;		// X �ε���
		int nY;		// Y �ε���
		ePlayer ePlayerIdx;

		stRock(int nX = 0, int nY = 0, ePlayer ePlayerIdx = ePlayer::Player_None)
			: nX(nX), nY(nY), ePlayerIdx(ePlayerIdx){}
	};

	// ���콺 ������ ����ü
	struct stMousePoint
	{
		GFloat fLeftTopX;		// ���� �� X ��ǥ
		GFloat fLeftTopY;		// ���� �� Y ��ǥ
		GFloat fRightBotX;		// ������ �Ʒ� X ��ǥ
		GFloat fRightBotY;		// ������ �Ʒ� Y ��ǥ

		ePlayer eStatus;		// ���� ����

		stMousePoint(GFloat fLeftTopX = 0.0f, GFloat fLeftTopY = 0.0f, GFloat fRightBotX = 0.0f, GFloat fRightBotY = 0.0f) :
			fLeftTopX(fLeftTopX), fLeftTopY(fLeftTopY), fRightBotX(fRightBotX), fRightBotY(fRightBotY),
			eStatus(ePlayer::Player_None)
		{

		}
	};
	
	// ������ ����ü
	struct stSettings
	{
		eGameStatus eCurGameStatus;	// ���� ���� ����

		int nXLine;					// ���� �ټ�
		int nYLine;					// ���� �ټ�
		int nMargin;				// ����
		GColor clrArr[ColorMax];	// �� ��

		bool bIsClickEnabled;		// ���콺 Ŭ�� Ȱ��ȭ ����

		GFloat fRockWidth;			// ���� ����
		GFloat fRockHeight;			// ���� ����

		GFloat fMiniWinWidth;		// ��� ��� �������� ����
		GFloat fMiniWinHeight;		// ��� ��� �������� ����
		GFloat fMiniWinFontSize;	// ��� ��� ��Ʈ�� ũ��
		GFloat fMiniWinBtnWidth;	// ��� ��� �������� ��ư ����
		GFloat fMiniWinBtnHeight;	// ��� ��� �������� ��ư ����

		stSettings() :
			eCurGameStatus(eGameStatus::GameStop),
			nXLine(Default_XLine), nYLine(Default_YLine),
			nMargin(Default_Margin),
			bIsClickEnabled(true),
			fRockWidth(0.0f), fRockHeight(0.0f),
			fMiniWinWidth(Default_MiniW_Width), fMiniWinHeight(Default_MiniW_Height),
			fMiniWinFontSize(Default_MiniW_Font), fMiniWinBtnWidth(Default_MiniW_BtnWidth), fMiniWinBtnHeight(Default_MiniW_BtnHeight)
		{
			clrArr[BaseBackground] = RGB(0, 120, 120);
			clrArr[PanLine] = GColor::Black;
			clrArr[PanBackground] = GColor::White;
			clrArr[RockLine1] = GColor::DimGray;
			clrArr[RockBackground1] = GColor::Black;
			clrArr[RockLine2] = GColor::DimGray;
			clrArr[RockBackground2] = GColor::White;
			clrArr[WinBackgournd] = GColor::Gray;
			clrArr[WinLine] = GColor::Black;
			clrArr[WinFont] = GColor::Black;
			clrArr[WinButton] = GColor::Black;
			clrArr[WinButtonFont] = GColor::White;
		}
	};

	// �÷��̾�� ����ü
	struct stPlayer
	{
		ePlayer ePlayerIdx;			// �÷��̾� ��ȣ
		vector<stRock> vRocks;		// �ش� �÷��̾��� �⺸
		DWORD dwTick;				// �ش� �÷��̾��� Tick Count

		stPlayer() : ePlayerIdx(ePlayer::Player_None), dwTick(0) {}
	};

public:
	enum eEvent
	{
		Event_Clicked = (WM_USER + 4000),
	};

// ===========================================================================


// �ʵ� ���� ==================================================================
private:
	CRect					m_rcWindow;			// ������ Rect
	HWND					m_hParent;			// �̺�Ʈ�� ������ �θ� ��Ʈ���� HWND

	stSettings				m_stSettings;		// ������

	stPlayer				m_stPlayers[PlayerMax];	// �÷��̾� ����ü

	vector<stMousePoint>	m_vMousePoint;		// ���콺 ������ (���콺 Ŭ����)

	bool					m_bRockToggle;		// ���� ���� ���ʸ� ��Ÿ�� ���

	GFloat					m_fGdiBtnLeftTopX;	// GDI ��ư�� ���� �� X��ǥ
	GFloat					m_fGdiBtnLeftTopY;	// GDI ��ư�� ���� �� Y��ǥ

	stRock					m_stOmokPan[Default_YLine][Default_XLine];		// �˰��� ����� ������ ��Ȳ

public:

// ===========================================================================


// �Լ� ���� =================================================================
private:
	// ������ �׸��� �Լ�
	void DrawOmok(GPaint* pDC);
	// ������ �� ��� ���� �����츦 �׸��� �Լ�
	void DrawMiniWindow(GPaint* pDC);

	// ���콺 ������ �ʱ�ȭ
	void InitMousePoint();
	// ���콺 Ŭ�� ��ȿ üũ
	bool HitCheck(CPoint point, int* outX, int* outY, bool bIsBtnCheck);

	// GDI+ �ؽ�Ʈ�� ���� �Լ�
	void WriteGdiPText(GPaint* pDC, CString strText, GFloat fX, GFloat fY, GFloat fFontSize, GColor clrFont, bool bIsCenter = true);
	// �ش� ������ ��Ʈ ũ�⸦ Pixel ������ ��ȯ�ϴ� �Լ�
	void GetTextSizeToPixel(const TCHAR* strText, GFont* font, GFloat& outTextWidth, GFloat& outTextHeight);

	// ���� ���� Ȯ�� üũ �˰���
	ePlayer CheckOMok();
	int CheckOMok_Go(eFindPos ePos, int nX, int nY, ePlayer ePlayerIdx);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

public:
	COmokCtrl();
	~COmokCtrl();

	// ��Ʈ�� ������ �� �־��־�� �ϴ� �Լ� (�ʱ�ȭ��)
	void Init(int nXLine = Default_XLine, int nYLine = Default_YLine, HWND hParent = nullptr);
	// �� �ʱ�ȭ
	void ResetRock();
	// ���� ���� ����
	int GetRockCount(ePlayer ePlay);
	// �ش� �ε���(nIdx)�� ���� ���� ��ȯ
	bool GetPoint(ePlayer ePlay, int nIdx, int& outX, int& outY);
	// �ش� �÷��̾�(ePlay)�� Tick Count�� ��ȯ
	int GetTick(ePlayer ePlay);

	// ���� ����
	void StopGame();
	// ���� ����
	void StartGame();
	// ���� �Ͻ�����
	void PauseGame();
	// ���� ���� ��ȯ
	eGameStatus GetGameStatus();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// ===========================================================================
	
};

