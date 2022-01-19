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
// 열거형 및 구조체 ============================================================
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

	// 돌 구조체
	struct stRock
	{
		int nX;		// X 인덱스
		int nY;		// Y 인덱스
		ePlayer ePlayerIdx;

		stRock(int nX = 0, int nY = 0, ePlayer ePlayerIdx = ePlayer::Player_None)
			: nX(nX), nY(nY), ePlayerIdx(ePlayerIdx){}
	};

	// 마우스 포인터 구조체
	struct stMousePoint
	{
		GFloat fLeftTopX;		// 왼쪽 위 X 좌표
		GFloat fLeftTopY;		// 왼쪽 위 Y 좌표
		GFloat fRightBotX;		// 오른쪽 아래 X 좌표
		GFloat fRightBotY;		// 오른쪽 아래 Y 좌표

		ePlayer eStatus;		// 돌의 상태

		stMousePoint(GFloat fLeftTopX = 0.0f, GFloat fLeftTopY = 0.0f, GFloat fRightBotX = 0.0f, GFloat fRightBotY = 0.0f) :
			fLeftTopX(fLeftTopX), fLeftTopY(fLeftTopY), fRightBotX(fRightBotX), fRightBotY(fRightBotY),
			eStatus(ePlayer::Player_None)
		{

		}
	};
	
	// 설정용 구조체
	struct stSettings
	{
		eGameStatus eCurGameStatus;	// 게임 시작 유무

		int nXLine;					// 가로 줄수
		int nYLine;					// 세로 줄수
		int nMargin;				// 여백
		GColor clrArr[ColorMax];	// 색 값

		bool bIsClickEnabled;		// 마우스 클릭 활성화 유무

		GFloat fRockWidth;			// 돌의 넓이
		GFloat fRockHeight;			// 돌의 높이

		GFloat fMiniWinWidth;		// 가운데 띄울 윈도우의 넓이
		GFloat fMiniWinHeight;		// 가운데 띄울 윈도우의 높이
		GFloat fMiniWinFontSize;	// 가운데 띄울 폰트의 크기
		GFloat fMiniWinBtnWidth;	// 가운데 띄울 윈도우의 버튼 넓이
		GFloat fMiniWinBtnHeight;	// 가운데 띄울 윈도우의 버튼 높이

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

	// 플레이어용 구조체
	struct stPlayer
	{
		ePlayer ePlayerIdx;			// 플레이어 번호
		vector<stRock> vRocks;		// 해당 플레이어의 기보
		DWORD dwTick;				// 해당 플레이어의 Tick Count

		stPlayer() : ePlayerIdx(ePlayer::Player_None), dwTick(0) {}
	};

public:
	enum eEvent
	{
		Event_Clicked = (WM_USER + 4000),
	};

// ===========================================================================


// 필드 영역 ==================================================================
private:
	CRect					m_rcWindow;			// 윈도우 Rect
	HWND					m_hParent;			// 이벤트를 전달할 부모 컨트롤의 HWND

	stSettings				m_stSettings;		// 설정값

	stPlayer				m_stPlayers[PlayerMax];	// 플레이어 구조체

	vector<stMousePoint>	m_vMousePoint;		// 마우스 포인터 (마우스 클릭시)

	bool					m_bRockToggle;		// 돌을 놓을 차례를 나타낼 토글

	GFloat					m_fGdiBtnLeftTopX;	// GDI 버튼의 왼쪽 위 X좌표
	GFloat					m_fGdiBtnLeftTopY;	// GDI 버튼의 왼쪽 위 Y좌표

	stRock					m_stOmokPan[Default_YLine][Default_XLine];		// 알고리즘에 사용할 오목판 상황

public:

// ===========================================================================


// 함수 영역 =================================================================
private:
	// 오목을 그리는 함수
	void DrawOmok(GPaint* pDC);
	// 오목판 한 가운데 작은 윈도우를 그리는 함수
	void DrawMiniWindow(GPaint* pDC);

	// 마우스 포인터 초기화
	void InitMousePoint();
	// 마우스 클릭 유효 체크
	bool HitCheck(CPoint point, int* outX, int* outY, bool bIsBtnCheck);

	// GDI+ 텍스트를 쓰는 함수
	void WriteGdiPText(GPaint* pDC, CString strText, GFloat fX, GFloat fY, GFloat fFontSize, GColor clrFont, bool bIsCenter = true);
	// 해당 문장의 폰트 크기를 Pixel 값으로 반환하는 함수
	void GetTextSizeToPixel(const TCHAR* strText, GFont* font, GFloat& outTextWidth, GFloat& outTextHeight);

	// 오목 승자 확인 체크 알고리즘
	ePlayer CheckOMok();
	int CheckOMok_Go(eFindPos ePos, int nX, int nY, ePlayer ePlayerIdx);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

public:
	COmokCtrl();
	~COmokCtrl();

	// 컨트롤 생성시 꼭 넣어주어야 하는 함수 (초기화용)
	void Init(int nXLine = Default_XLine, int nYLine = Default_YLine, HWND hParent = nullptr);
	// 돌 초기화
	void ResetRock();
	// 현재 돌의 개수
	int GetRockCount(ePlayer ePlay);
	// 해당 인덱스(nIdx)에 대한 점을 반환
	bool GetPoint(ePlayer ePlay, int nIdx, int& outX, int& outY);
	// 해당 플레이어(ePlay)의 Tick Count를 반환
	int GetTick(ePlayer ePlay);

	// 게임 종료
	void StopGame();
	// 게임 시작
	void StartGame();
	// 게임 일시정지
	void PauseGame();
	// 게임 상태 반환
	eGameStatus GetGameStatus();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// ===========================================================================
	
};

