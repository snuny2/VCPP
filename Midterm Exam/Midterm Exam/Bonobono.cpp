#include <windows.h>
#include <tchar.h>
#include <string>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DrawingAreaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 드로잉 영역 윈도우 핸들
HWND hDrawingArea = NULL;

HINSTANCE hInstance = GetModuleHandle(NULL);

// 전역 변수
HBRUSH g_hBrush = NULL;
bool g_isDrawing = false;  // 드래그 중인지 여부를 나타내는 플래그
bool g_buttonPressed = false;  // 버튼이 눌렸는지 여부를 나타내는 플래그
bool g_drawRectangle = false;  // 사각형을 그릴지 여부를 나타내는 플래그
POINT g_startPoint;        // 드래그 시작 지점
RECT g_drawnShape = { 0, 0, 0, 0 };  // 그려진 도형의 좌표

// 현재 커서 모양 저장 변수
HCURSOR g_hCursorCross = NULL;
HCURSOR g_hCursorArrow = NULL;

static bool isDrawingCircle = false;
static bool isDrawing = false;   // 드래그 중인 상태 여부를 저장하는 변수
static POINT startDrag = { 0 };  // 드래그 시작 지점
static POINT endDrag = { 0 };
static POINT startCircle = { 0 };  // 드래그 시작 지점
static POINT endCircle = { 0 };

// 원 그리기 함수
void DrawEllipse(HDC hdc, int x1, int y1, int x2, int y2) {
	SelectObject(hdc, g_hBrush);
	Ellipse(hdc, x1, y1, x2, y2);
}

// 사각형 그리기 함수
void DrawRectangle(HDC hdc, int x1, int y1, int x2, int y2) {
	SelectObject(hdc, g_hBrush);
	Rectangle(hdc, x1, y1, x2, y2);
}

// 이전에 그린 원을 지우는 함수
void ErasePreviousEllipse(HWND hwnd, RECT rect) {
	HDC hdc = GetDC(hwnd);
	FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH)); // 흰색으로 이전에 그린 원을 지웁니다.
	ReleaseDC(hwnd, hdc);
}

// 커서 모양 설정 함수
void SetCursorShape(HWND hwnd, bool isDrawing) {
	if (isDrawing) {
		SetCursor(g_hCursorCross);
	}
	else {
		SetCursor(g_hCursorArrow);
	}
}

void DrawRectangle(HDC hdc, POINT start, POINT end) {
	// 그림을 그리기 전에 이전에 그린 것을 지웁니다.
	SelectObject(hdc, GetStockObject(WHITE_PEN));
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	Rectangle(hdc, startDrag.x, startDrag.y, endDrag.x, endDrag.y);

	// 현재 사각형을 그립니다.
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, start.x, start.y, end.x, end.y);
}

void DrawCube(HDC hdc, POINT start, POINT end) {
	// 간단한 방법으로 정육면체를 그립니다.
	int width = end.x - start.x;
	int height = end.y - start.y;

	// 정육면체를 그립니다.
	Rectangle(hdc, start.x, start.y, end.x, end.y);

	// 정육면체의 윗면을 그립니다.
	MoveToEx(hdc, start.x, start.y, NULL);
	LineTo(hdc, end.x, start.y);

	// 정육면체의 왼쪽 세로면을 그립니다.
	MoveToEx(hdc, start.x, start.y, NULL);
	LineTo(hdc, start.x, end.y);

	// 정육면체의 오른쪽 세로면을 그립니다.
	MoveToEx(hdc, end.x, start.y, NULL);
	LineTo(hdc, end.x, end.y);

	// 정육면체의 아랫면을 그립니다.
	MoveToEx(hdc, start.x, end.y, NULL);
	LineTo(hdc, end.x, end.y);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hBrush = CreateSolidBrush(RGB(0, 255, 0)); // 연두색
	// 윈도우 클래스 등록
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(255, 240, 200)); // 배경색 설정
	wc.lpszClassName = L"BoxWindowClass";

	if (!RegisterClass(&wc)) {
		return 1;
	}

	// 윈도우 생성
	HWND hwnd = CreateWindow(L"BoxWindowClass", L"Button Example", WS_OVERLAPPEDWINDOW, 100, 100, 800, 480, NULL, NULL, hInstance, NULL);
	// 창 크기를 고정
	SetWindowPos(hwnd, NULL, 0, 0, 800, 480, SWP_NOMOVE | SWP_NOZORDER);

	if (hwnd == NULL) {
		return 2;
	}

	// 윈도우 표시
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 기본 마우스 커서 설정 (화살표)
	HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(hCursor);

	// 5개의 버튼 추가
	int buttonCount = 5;
	int buttonWidth = (800 - (buttonCount + 1) * 18) / buttonCount; // 가로 크기 계산
	int buttonHeight = 55;
	int margin = 16;
	int xStart = 14; // 시작 위치 설정
	int y = 15; // 버튼이 그려질 y 위치
	LPCWSTR buttonNames[] = { L"Box", L"Circle", L"Bonobono", L"Ryan", L"Cube" };

	for (int i = 0; i < buttonCount; i++) {
		HWND hButton = CreateWindow(L"BUTTON", buttonNames[i], WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON,
			xStart, y, buttonWidth, buttonHeight, hwnd, (HMENU)(i + 1), hInstance, NULL);
		xStart += buttonWidth + margin;
	}

	// 드로잉 영역 윈도우 생성
	hDrawingArea = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS,
		7, 77, 769, 356, hwnd, NULL, hInstance, NULL);
	SetWindowLongPtr(hDrawingArea, GWLP_WNDPROC, (LONG_PTR)DrawingAreaProc);
	// 드로잉 영역의 배경색을 흰색으로 설정
	SetClassLongPtr(hDrawingArea, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(WHITE_BRUSH));

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool isLineEyes = false;

	switch (msg) {

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 박스 그리기
		RECT boxRect;
		boxRect.left = 8; // 왼쪽 마진 8px
		boxRect.top = 8; // 위쪽 마진 8px
		boxRect.right = ps.rcPaint.right - 8; // 오른쪽 마진 8px
		boxRect.bottom = ps.rcPaint.bottom - 8; // 아래쪽 마진 8px

		HBRUSH boxBrush = CreateSolidBrush(RGB(255, 240, 200)); // 뷰의 배경색과 동일
		FillRect(hdc, &boxRect, boxBrush);

		// 테두리 그리기
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // 검정색 테두리
		SelectObject(hdc, hPen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH)); // 테두리만 그리고 내부는 투명
		Rectangle(hdc, boxRect.left, boxRect.top, boxRect.right, boxRect.bottom);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_CREATE: {
		// 십자가 형태의 커서 생성
		g_hCursorCross = LoadCursor(NULL, IDC_CROSS);
		// 기본 화살표 커서 생성
		g_hCursorArrow = LoadCursor(NULL, IDC_ARROW);
		return 0;
	}
	case WM_COMMAND: {
		bool eraseDrawingArea = false;
		switch (LOWORD(wParam)) {

		case 1: {
			g_buttonPressed = true;  // 두 번째 버튼이 눌렸음을 표시
			g_drawRectangle = true;   // 사각형 그리기 모드 설정
			break;
		}
		case 2: {
			g_buttonPressed = true;  // 버튼이 눌렸음을 표시
			g_drawRectangle = false;  // 사각형 그리기 모드 해제
			break;
		}
		case 3: {
			// 화면 중앙에 보노보노 얼굴 그리기
			HDC hdc = GetDC(hwnd);

			// 얼굴 그리기
			int centerX = 400; // 화면 가로 중앙
			int centerY = 240; // 화면 세로 중앙
			int radius = 100; // 반지름

			HBRUSH faceBrush = CreateSolidBrush(RGB(127, 200, 255)); // 얼굴색 (파란색)
			SelectObject(hdc, faceBrush);

			Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

			// 대칭되는 것등 간격 조절
			int eyeDistance = 130;
			int eyeDistance2 = 153;
			int eyeDistance3 = 38;
			// 왼쪽 눈 그리기 (검은색)
			HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색
			SelectObject(hdc, eyeBrush);
			int leftEyeX = centerX - eyeDistance / 2;
			Ellipse(hdc, leftEyeX - 15, centerY - 22, leftEyeX - 5, centerY - 5);

			// 오른쪽 눈 그리기 (검은색)
			int rightEyeX = centerX + eyeDistance / 2;
			Ellipse(hdc, rightEyeX + 5, centerY - 22, rightEyeX + 15, centerY - 5);

			// 왼쪽 눈 흰색 눈동자 그리기
			HBRUSH whiteEyeBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색
			SelectObject(hdc, whiteEyeBrush);
			int leftEyeX2 = centerX - eyeDistance2 / 2;
			Ellipse(hdc, leftEyeX2 - 2, centerY - 18, leftEyeX2 + 2, centerY - 13);

			// 오른쪽 눈 흰색 눈동자 그리기
			int rightEyeX2 = centerX + eyeDistance2 / 2;
			Ellipse(hdc, rightEyeX2 - 2, centerY - 18, rightEyeX2 + 2, centerY - 13);

			// 입 그리기 (분홍과 빨강사이의 색)
			int mouthRadius = 10; // 입 반지름 조절
			HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 150, 255)); // 입색 (분홍색)
			SelectObject(hdc, mouthBrush);
			Ellipse(hdc, centerX - mouthRadius, centerY + 15, centerX + mouthRadius, centerY + 20 + mouthRadius * 2);

			// 왼쪽 ?? 흰색
			HBRUSH whitenomuBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색
			SelectObject(hdc, whitenomuBrush);
			int leftnomuX = centerX - eyeDistance3 / 2;
			Ellipse(hdc, leftnomuX - 20, centerY + 30, leftnomuX + 20, centerY - 10);

			// 오른쪽 ?? 흰색
			int rightnomuX = centerX + eyeDistance3 / 2;
			Ellipse(hdc, rightnomuX - 20, centerY + 30, rightnomuX + 20, centerY - 10);

			// 코 그리기 (동그라미)
			HBRUSH noseBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색
			SelectObject(hdc, noseBrush);
			int noseWidth = 20;
			int noseHeight = 20;
			Ellipse(hdc, centerX - noseWidth / 2, centerY + 3 - noseHeight, centerX + noseWidth / 2, centerY + 3);

			// 수염그리기
			int mustacheLength = 50;
			int mustacheHeight = 20;

			// 왼쪽 위 수염
			MoveToEx(hdc, centerX - mustacheLength / 2, centerY + 5, NULL);
			LineTo(hdc, centerX - mustacheLength / 2 - mustacheHeight, centerY - mustacheHeight + 20);

			MoveToEx(hdc, centerX - mustacheLength / 2, centerY + 12, NULL);
			LineTo(hdc, centerX - mustacheLength / 2 - mustacheHeight, centerY + mustacheHeight - 5);

			// 오른쪽 위 수염
			MoveToEx(hdc, centerX - mustacheLength / -2, centerY + 5, NULL);
			LineTo(hdc, centerX + mustacheLength / 2 + mustacheHeight, centerY - mustacheHeight + 20);

			MoveToEx(hdc, centerX - mustacheLength / -2, centerY + 12, NULL);
			LineTo(hdc, centerX + mustacheLength / 2 + mustacheHeight, centerY + mustacheHeight - 5);

			break;
		}
		case 4: {
			break;
		}
		case 5: {
			g_buttonPressed = true;  // 두 번째 버튼이 눌렸음을 표시
			g_drawRectangle = true;   // 사각형 그리기 모드 설정
			break;
		}
		default:
			break;
			// 버튼 클릭 후 드로잉 처리
			if (eraseDrawingArea) {
				// 드로잉 영역을 지우는 코드
				ErasePreviousEllipse(hDrawingArea, g_drawnShape);
				// 다른 지우기 동작이 필요하면 여기에 추가

				// 지우기 플래그를 다시 초기화
				eraseDrawingArea = false;
			}
		case WM_KEYDOWN: {
			if (wParam == VK_SPACE) {
				isLineEyes = !isLineEyes;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			break;
		}

		}
		break;

	}

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO: {
		// 창의 최소 크기와 최대 크기를 같게 설정하여 크기 변경을 방지
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 800;
		lpMMI->ptMinTrackSize.y = 480;
		lpMMI->ptMaxTrackSize.x = 800;
		lpMMI->ptMaxTrackSize.y = 480;
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK DrawingAreaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_LBUTTONDOWN: {
		// 마우스 왼쪽 버튼이 눌렸을 때 드래그 시작
		if (g_buttonPressed) {
			g_isDrawing = true;
			g_startPoint.x = LOWORD(lParam);
			g_startPoint.y = HIWORD(lParam);

			// 새로운 드래그 시작 지점에서 원을 지우지 않도록
			ErasePreviousEllipse(hwnd, g_drawnShape);
		}
		break;
	}
	case WM_LBUTTONUP: {
		// 마우스 왼쪽 버튼이 떼어졌을 때 드래그 종료
		g_isDrawing = false;
		SetCursorShape(hwnd, g_isDrawing);  // 드래그 종료 후 커서 모양 복구
		break;
	}
	case WM_MOUSEMOVE: {
		// 마우스 이동 중인 경우, 드래그 중인지 확인 후 원을 그림
		if (g_isDrawing) {
			ErasePreviousEllipse(hwnd, g_drawnShape);  // 이전에 그린 원 지우기

			HDC hdc = GetDC(hwnd);
			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);
			if (g_drawRectangle) {
				DrawRectangle(hdc, g_startPoint.x, g_startPoint.y, currentX, currentY);
				g_drawnShape = { g_startPoint.x, g_startPoint.y, currentX, currentY };
			}
			else {
				DrawEllipse(hdc, g_startPoint.x, g_startPoint.y, currentX, currentY);
				g_drawnShape = { g_startPoint.x, g_startPoint.y, currentX, currentY };
			}
			ReleaseDC(hwnd, hdc);
		}
		SetCursorShape(hwnd, g_isDrawing);  // 드래그 중일 때 커서 모양 설정
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 이전에 그린 도형 그리기
		if (g_drawRectangle) {
			DrawRectangle(hdc, g_drawnShape.left, g_drawnShape.top, g_drawnShape.right, g_drawnShape.bottom);
		}
		else {
			DrawEllipse(hdc, g_drawnShape.left, g_drawnShape.top, g_drawnShape.right, g_drawnShape.bottom);
		}

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case 1: {
			
			break;
		}
		case 2: {
			
		}
		}
	}
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY: {
		// 해제된 브러시 처리
		DeleteObject(g_hBrush);
		DestroyCursor(g_hCursorCross);
		DestroyCursor(g_hCursorArrow);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}