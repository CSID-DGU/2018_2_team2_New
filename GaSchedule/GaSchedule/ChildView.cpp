// ChildView.cpp : implementation of the CChildView class 윈도우에 그리는 함수 
//

#include "stdafx.h"
#include "GaSchedule.h"
#include "ChildView.h"

#include "Algorithm\Configuration.h"
#include "Algorithm\Room.h"
#include "Algorithm\Schedule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView() : _schedule(NULL),
	_running(false)
{
	Algorithm::GetInstance().GetObserver()->SetWindow( this );
}//생성자 

CChildView::~CChildView()
{
	if( _schedule )
		delete _schedule;
}

void CChildView::SetSchedule(const Schedule* schedule)
{
	CSingleLock lock( &_sect, TRUE );

	if( _schedule )
		delete _schedule;

	_schedule = schedule->MakeCopy( false );

	lock.Unlock();

	Invalidate();
}

void CChildView::SetNewState(AlgorithmState state)
{
	_running = state == AS_RUNNING;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_START, &CChildView::OnFileStart)
	ON_COMMAND(ID_FILE_STOP, &CChildView::OnFileStop)
	ON_COMMAND(ID_FILE_OPEN_CONFIGURATION, &CChildView::OnFileOpenConfiguration)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_CONFIGURATION, &CChildView::OnUpdateFileOpenConfiguration)
	ON_UPDATE_COMMAND_UI(ID_FILE_START, &CChildView::OnUpdateFileStart)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, &CChildView::OnUpdateFileStop)
END_MESSAGE_MAP()

// CChildView message handlers

bool CChildView::PreCreateWindow(CREATESTRUCT& cs) //윈도우 스타일 변경 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;//기본 

	//스타일 변경 
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

const int ROOM_CELL_WIDTH = 85;
const int ROOM_CELL_HEIGHT = 50;

const int ROOM_MARGIN_WIDTH = 50;
const int ROOM_MARGIN_HEIGHT = 50;

const int ROOM_COLUMN_NUMBER = DAYS_NUM + 1;
const int ROOM_ROW_NUMBER = DAY_HOURS + 1;

const int ROOM_TABLE_WIDTH = ROOM_CELL_WIDTH * ROOM_COLUMN_NUMBER + ROOM_MARGIN_WIDTH;
const int ROOM_TABLE_HEIGHT = ROOM_CELL_HEIGHT * ROOM_ROW_NUMBER + ROOM_MARGIN_HEIGHT;

void CChildView::OnPaint() //화면 출력을 위한 함수 
{
	CPaintDC wndDC(this);//그리기를 위한 디바이스 컨텍 

	CRect clientRect;//사각형 생성 
	GetClientRect( clientRect );//clientRect의 좌표값 반환 

	CDC dc;
	CBitmap bmp;
	dc.CreateCompatibleDC( &wndDC );//호환되는 그릴 장소 얻어옴 
	bmp.CreateCompatibleBitmap( &wndDC, clientRect.Width(), clientRect.Height() );//(그릴 장소, 가로, 세로)
	dc.SelectObject( &bmp );//도화지 선택 bmp

	int sx = -GetScrollPos( SB_HORZ );//가로축 스크롤바 초기위치 
	int sy = -GetScrollPos( SB_VERT );//세로축 스크롤바 초기위치 

	CBrush bgBrush( RGB( 255, 255, 255 ) );//브러쉬 생성 - 흰색  
	dc.FillRect( clientRect, &bgBrush );//clientRect를 채움 

	dc.SetBkColor( RGB( 255, 255, 255 ) );//배경색 변경 - 흰색
	dc.SetBkMode( TRANSPARENT ); //배경모드 투명 

	CFont tableHeadersFont;//헤더폰트 
	tableHeadersFont.CreateFont( 24, 0, 0, 0, 700, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial" );//생성 (글자 높이, 글자 너비, 출력 각도, 기준선에서의 각도, 글자굵기,Italic 적용여부,밑줄적용여부
					//취소선적용여부,문자셋종류,출력정밀도,클리핑정밀도,출력문자품질,글꼴Pitch,글꼴)

	CFont tableTextFont;
	tableTextFont.CreateFont( 14, 0, 0, 0, 100, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial" );

	CFont roomDescFont;
	roomDescFont.CreateFont( 12, 0, 0, 0, 100, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial" );

	CFont criteriaFont;
	criteriaFont.CreateFont( 14, 0, 0, 0, 900, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial" );

	CBrush classBrush( RGB( 255, 255, 245 ) );// 베이지색 브러쉬 생성 
	CBrush overlapBrush( HS_BDIAGONAL, RGB( 255, 0, 0 ) );//좌하향 줄무늬 
	CPen overlapPen( PS_NULL, 1, RGB( 255, 0, 0 ) );//투명 팬 새성 

	int nr = Configuration::GetInstance().GetNumberOfRooms();//현재 구성의 교실 해쉬맵 구성 갯수 반환값 저장 
	//기본 틀 출력 하는 삼중 for문 
	for( int k = 0; k < nr; k++ )//0붙터 룸 사이즈까지 
	{
		for( int i = 0; i < ROOM_COLUMN_NUMBER; i++ )//열 
		{
			for( int j = 0; j < ROOM_ROW_NUMBER; j++ )//행 
			{
				int l = k % 2;
				int m = k / 2;

				CRect rect( 
					sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + i * ROOM_CELL_WIDTH - 1,
					sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + j * ROOM_CELL_HEIGHT - 1,
					sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + ( i + 1 ) * ROOM_CELL_WIDTH,
					sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + ( j + 1 ) * ROOM_CELL_HEIGHT );

				if( i == 0 || j == 0 )//하나만 0이면 참 
					dc.Rectangle( rect );// rect 좌표인 사각형을 그린다 

				if( i == 0 && j == 0 )//둘다 0일때 (처음)
				{
					dc.SelectObject( &roomDescFont );//교실 폰트 컨택 

					rect.bottom -= rect.Height() / 2;
					dc.Rectangle( rect );//작은 사각형 생성 

					CString str;
					str.Format( "Room: %s", Configuration::GetInstance().GetRoomById( k )->GetName().c_str() );
					dc.DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					//구성의 교실 id에 맞는 이름을 출력 , 윈도우 창에 그림 
					rect.MoveToY( rect.bottom - 1 );

					rect.right -= rect.Width() / 2 + 7;//오른쪽에 더 그림 
					dc.Rectangle( rect );

					str.Format( "Lab: %c", Configuration::GetInstance().GetRoomById( k )->IsLab() ? 'Y' : 'N' );
					dc.DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					//현재 구성의 교실에 컴퓨터가 있으면 Y 없으면 N
					rect.MoveToX( rect.right - 1 );
					rect.right += 14;

					str.Format( "Seats: %d", Configuration::GetInstance().GetRoomById( k )->GetNumberOfSeats() );
					dc.DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					//현재 구성의 교실의 좌석수를 출력 
				}

				dc.SelectObject( &tableHeadersFont );//헤더 폰트 컨택 

				if( i == 0 && j > 0 )//0번째 열에서 행이 하나라도 있을때(시간 칸  
				{
					CString str;
					str.Format( "%d - %d", 9 + j - 1, 9 + j );

					dc.DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				}

				if( j == 0 && i > 0 )//요일 칸 출력 
				{
					static const char* days[] = { "MON", "THU", "WED", "THR", "FRI" };

					dc.DrawText( days[ i - 1 ], 3, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				}
			}
		}
	}

	dc.SelectObject( &classBrush );//classBrush (베이지색) 컨택 

	CSingleLock lock( &_sect, TRUE );//스레드락 

	if( _schedule )//스케줄이 있다면?
	{
		CString fit;
		fit.Format( "Fitness: %f, Generation: %d", _schedule->GetFitness(),
			Algorithm::GetInstance().GetCurrentGeneration() );// fitness값, 현재 

		dc.SelectObject( &tableHeadersFont );//헤더폰트 컨택

		dc.TextOutA( sx + 10, sy + 10, fit );//(초기위치에서 10 떨어진 곳에 fit 출력) 

		const hash_map<CourseClass*, int>& classes = _schedule->GetClasses();//수업 과목 해쉬맵 
		int ci = 0;
		//과목  해쉬맵의 시작부터 끝까지 1씩 증가 하면서 ci 1씩증가 과목을 전부 다 돈다 
		for( hash_map<CourseClass*, int>::const_iterator it= classes.begin(); it != classes.end(); ++it, ci += 5 )
		{
			CourseClass* c = ( *it ).first;//과목 클래스 
			int p = ( *it ).second;

			//수업 시간 계산 하는 것 같은데 잘 모르겠음 
			int t = p % ( nr * DAY_HOURS );//해쉬맵2번째%(교실 해쉬맵 요소 갯수 * 12 시간)
			int d = p / ( nr * DAY_HOURS ) + 1;//해쉬맵 2번째 /(교실 해쉬맵 요소 갯수*12 시간)
			int r = t / DAY_HOURS; // 
			t = t % DAY_HOURS + 1;

			int l = r % 2;
			int m = r / 2;

			CRect rect( 
				sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + d * ROOM_CELL_WIDTH - 1,
				sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + t * ROOM_CELL_HEIGHT - 1,
				sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + ( d + 1 ) * ROOM_CELL_WIDTH,
				sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + ( t + c->GetDuration() ) * ROOM_CELL_HEIGHT );

			dc.Rectangle( rect );

			CString str;
			str.Format( "%s\n%s\n/", c->GetCourse().GetName().c_str(), c->GetProfessor().GetName().c_str() );
			//과목 이름, 교수 이름 출력 

			for( list<StudentsGroup*>::const_iterator it = c->GetGroups().begin(); 
				it != c->GetGroups().end(); ++it )//과목을 수강하는 학생 학과 전부 출력 
			{
				str += ( *it )->GetName().c_str();
				str += "/";
			}

			if( c->IsLabRequired() )
				str += "\nLab";

			rect.top += 5;
			rect.bottom -= 5;
			rect.left += 5;
			rect.right -= 5;

			dc.SelectObject( &tableTextFont );
			
			dc.DrawText( str, rect, DT_CENTER | DT_WORDBREAK );

			dc.SelectObject( &criteriaFont );

			dc.SetTextColor( _schedule->GetCriteria()[ ci + 0 ] ? RGB( 49, 147, 120 ) : RGB( 206, 0, 0 ) );
			dc.TextOut( rect.left, rect.bottom - 10, "R", 1 );
			dc.SetTextColor( _schedule->GetCriteria()[ ci + 1 ] ? RGB( 49, 147, 120 ) : RGB( 206, 0, 0 ) );
			dc.TextOut( rect.left + 10, rect.bottom - 10, "S", 1 );
			dc.SetTextColor( _schedule->GetCriteria()[ ci + 2 ] ? RGB( 49, 147, 120 ) : RGB( 206, 0, 0 ) );
			dc.TextOut( rect.left + 20, rect.bottom - 10, "L", 1 );
			dc.SetTextColor( _schedule->GetCriteria()[ ci + 3 ] ? RGB( 49, 147, 120 ) : RGB( 206, 0, 0 ) );
			dc.TextOut( rect.left + 30, rect.bottom - 10, "P", 1 );
			dc.SetTextColor( _schedule->GetCriteria()[ ci + 4 ] ? RGB( 49, 147, 120 ) : RGB( 206, 0, 0 ) );
			dc.TextOut( rect.left + 40, rect.bottom - 10, "G", 1 );

			dc.SetTextColor( RGB( 0, 0, 0 ) );
		}

		dc.SelectObject( &overlapPen );
		dc.SelectObject( &overlapBrush );

		int i = 0;
		for( vector<list<CourseClass*>>::const_iterator it = _schedule->GetSlots().begin();
			it != _schedule->GetSlots().end(); ++it, ++i )
		{
			if( ( *it ).size() > 1 )
			{
				int t = i % ( nr * DAY_HOURS );
				int d = i / ( nr * DAY_HOURS ) + 1;
				int r = t / DAY_HOURS;
				t = t % DAY_HOURS + 1;

				int l = r % 2;
				int m = r / 2;

				CRect rect( 
					sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + d * ROOM_CELL_WIDTH - 1,
					sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + t * ROOM_CELL_HEIGHT - 1,
					sx + ROOM_TABLE_WIDTH * l + ROOM_MARGIN_WIDTH + ( d + 1 ) * ROOM_CELL_WIDTH,
					sy + ROOM_TABLE_HEIGHT * m + ROOM_MARGIN_HEIGHT + ( t + 1 ) * ROOM_CELL_HEIGHT );

				dc.Rectangle( rect );
			}
		}
	}

	lock.Unlock();

	wndDC.BitBlt( 0, 0, clientRect.Width(), clientRect.Height(), &dc, 0, 0, SRCCOPY );
}

DWORD WINAPI StartAlg(LPVOID param)
{
	Algorithm::GetInstance().Start();
	return 0;
}

void CChildView::OnFileStart()
{
	DWORD tid;
	HANDLE thread = CreateThread( NULL, 0, StartAlg, NULL, 0, &tid );
	CloseHandle( thread );
}

void CChildView::OnFileStop()
{
	Algorithm::GetInstance().Stop();
}

void CChildView::OnFileOpenConfiguration()
{
	CFileDialog dlg( TRUE, NULL, NULL, 0, 
		"Class Schedule Config Files (*.cfg)|*.cfg|All Files (*.*)|*.*|", this );

	if( dlg.DoModal() == IDOK )
	{
		Configuration::GetInstance().ParseFile( dlg.GetFileName().GetBuffer() );

		ComputeScrollBars();
		Invalidate();
	}
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}

void CChildView::ComputeScrollBars()
{
	int nr = Configuration::GetInstance().GetNumberOfRooms();

	int w = ROOM_TABLE_WIDTH;
	if( nr == 0 )
		w = 0;
	else if( nr > 1 )
		w *= 2;

	int h = ( ( nr / 2 ) + nr % 2 ) * ROOM_TABLE_HEIGHT;

	w += ROOM_MARGIN_WIDTH;
	h += ROOM_MARGIN_HEIGHT;

	CRect cr;
	GetClientRect( cr );

	SCROLLINFO hsi;
	hsi.cbSize = sizeof( hsi );
	hsi.fMask = SIF_RANGE | SIF_PAGE;
	hsi.nMin = 0;
	hsi.nMax = w;
	hsi.nPage = cr.Width();

	SetScrollInfo( SB_HORZ, &hsi, TRUE );

	hsi.nMax = h;
	hsi.nPage = cr.Height();

	SetScrollInfo( SB_VERT, &hsi, TRUE );
}

void CChildView::Scroll(int scrollBar, int nSBCode, int nPos)
{
	int minpos, maxpos, curpos;

	GetScrollRange( SB_HORZ, &minpos, &maxpos ); //스크롤 크기 설정 
	maxpos = GetScrollLimit( scrollBar );
	curpos = GetScrollPos( scrollBar );

	switch( nSBCode )
	{
	case SB_LEFT:
		curpos = minpos;
		break;

	case SB_RIGHT:
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:
		break;

	case SB_LINELEFT:
		if( curpos > minpos )
			curpos--;
		break;

	case SB_LINERIGHT:
		if( curpos < maxpos )
			curpos++;
		break;

	case SB_PAGELEFT:
		{
			SCROLLINFO   info;
			GetScrollInfo( scrollBar, &info, SIF_ALL);

			if( curpos > minpos )
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;

	case SB_PAGERIGHT:
		{
			SCROLLINFO info;
			GetScrollInfo( scrollBar, &info, SIF_ALL);

			if( curpos < maxpos )
				curpos = min( maxpos, curpos + (int)info.nPage );
		}
		break;

	case SB_THUMBPOSITION:
		curpos = nPos;
		break;

	case SB_THUMBTRACK:
		curpos = nPos;
		break;
	}

	SetScrollPos( scrollBar, curpos);
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Scroll( SB_HORZ, nSBCode, nPos );
	Invalidate();
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Scroll( SB_VERT, nSBCode, nPos );
	Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ComputeScrollBars();
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SetScrollPos( SB_VERT, GetScrollPos( SB_VERT ) - zDelta );
	Invalidate();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnClose()
{
	Algorithm::GetInstance().Stop();
	Algorithm::GetInstance().GetObserver()->WaitEvent();
	Algorithm::FreeInstance();

	CWnd::OnClose();
}

void CChildView::OnUpdateFileOpenConfiguration(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !_running );
}

void CChildView::OnUpdateFileStart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !_running && !Configuration::GetInstance().IsEmpty() );
}

void CChildView::OnUpdateFileStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( _running );
}
