#include <windows.h>
#include <wincodec.h>
// 프로젝트 > 속성 > 구성속성 > 링커 > 시스템 > 창

// 이미지를 저장할 변수
IWICImagingFactory* pWICFactory = NULL;
IWICBitmapDecoder* pDecoder = NULL;
IWICBitmapFrameDecode* pFrame = NULL;
IWICFormatConverter* pConverter = NULL;
HBITMAP hImage = NULL;

// 도형 변수
enum ShapeType {
	SHAPE_NONE,
	SHAPE_RECTANGLE,
	SHAPE_ELLIPSE,
	SHAPE_TRIANGLE,
	SHAPE_LINE
};

ShapeType currentShape = SHAPE_NONE; // 현재 선택된 도형
bool isShapeVisible = false;
RECT shapeRect; // 사각형, 원, 삼각형의 좌표를 저장하는 구조체
POINT lineStart, lineEnd; // 직선의 시작점과 끝점을 저장하는 변수
bool drawWhiteOnly = false; // 흰색 부분만 그릴지 여부를 나타내는 플래그
HCURSOR hCursor;
HBITMAP hBoxImage;

// 박스 그리기 함수
void DrawShape(HWND hWnd, HDC hdc) {
	if (isShapeVisible) {
		HBRUSH hBrush;
		if (currentShape == SHAPE_RECTANGLE) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 사각형
			SelectObject(hdc, hBrush);
			Rectangle(hdc, shapeRect.left, shapeRect.top, shapeRect.right, shapeRect.bottom);
			DeleteObject(hBrush);
		}
		else if (currentShape == SHAPE_ELLIPSE) {
			hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 원
			SelectObject(hdc, hBrush);
			Ellipse(hdc, shapeRect.left, shapeRect.top, shapeRect.right, shapeRect.bottom);
			DeleteObject(hBrush);
		}
		else if (currentShape == SHAPE_TRIANGLE) {
			
		}
		else if (currentShape == SHAPE_NONE) {
			// 이미지 그리기
			if (hBoxImage != NULL) {
				HDC hMemDC = CreateCompatibleDC(hdc);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBoxImage);
				BitBlt(hdc, 20, 160, 160, 64, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hMemDC, hOldBitmap);
				DeleteDC(hMemDC);
			}
		}
		else if (currentShape == SHAPE_LINE) {
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 검은색 펜
			SelectObject(hdc, hPen);
			MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
			LineTo(hdc, lineEnd.x, lineEnd.y);
			DeleteObject(hPen);
		}
	}
}

// 도형 지우기 함수
void EraseShape(HWND hWnd) {
	isShapeVisible = false;
	currentShape = SHAPE_NONE; // 아무 도형도 선택되지 않은 상태로 초기화
	InvalidateRect(hWnd, NULL, TRUE);
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		if (LOWORD(wParam) == 1) {
			EraseShape(hWnd);
			currentShape = SHAPE_RECTANGLE;
			SetCursor(LoadCursor(NULL, IDC_CROSS)); // 커서를 십자가로 변경
		}
		else if (LOWORD(wParam) == 2) {
			EraseShape(hWnd);
			currentShape = SHAPE_ELLIPSE;
			SetCursor(LoadCursor(NULL, IDC_CROSS)); // 커서를 십자가로 변경
		}
		else if (LOWORD(wParam) == 3) {
			EraseShape(hWnd);
			// 이미지 로드
			hBoxImage = (HBITMAP)LoadImage(NULL, L"box.png", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SetCursor(LoadCursor(NULL, IDC_CROSS)); // 커서를 십자가로 변경
		}
		else if (LOWORD(wParam) == 4) {
			EraseShape(hWnd);
			currentShape = SHAPE_LINE;
			SetCursor(LoadCursor(NULL, IDC_CROSS)); // 커서를 십자가로 변경
		}
		else if (LOWORD(wParam) == 5) {
			EraseShape(hWnd);
			SetCursor(LoadCursor(NULL, IDC_ARROW)); // 커서를 기본 화살표로 변경
		}
		break;

	case WM_SETCURSOR:
            if (isShapeVisible) {
                if (currentShape == SHAPE_RECTANGLE || currentShape == SHAPE_ELLIPSE ||
                    currentShape == SHAPE_TRIANGLE || currentShape == SHAPE_LINE) {
                    hCursor = LoadCursor(NULL, IDC_CROSS); // 십자가 모양으로 변경
                    SetCursor(hCursor);
                    return TRUE;
                }
            }
            break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 240, 200));
		FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
		DeleteObject(hBackgroundBrush);

		// 두 번째 박스 그리기
		HBRUSH hNewBoxBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색 설정
		RECT newBoxRect = { 8, 8, ps.rcPaint.right - 8, ps.rcPaint.bottom - 8 }; // 박스의 좌표 및 크기
		FillRect(hdc, &newBoxRect, hNewBoxBrush);
		DeleteObject(hNewBoxBrush);

		// 새로운 박스 그리기
		HBRUSH hBoxBrush = CreateSolidBrush(RGB(200, 200, 200)); // 색상 설정
		RECT boxRect = { 10, 10, 190, 375 }; // 박스의 좌표 및 크기
		FillRect(hdc, &boxRect, hBoxBrush);
		DeleteObject(hBoxBrush);

		DrawShape(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
		if (currentShape == SHAPE_TRIANGLE) {
			
		}
		else if (currentShape == SHAPE_LINE) {
			// 직선을 그릴 때 시작점 좌표를 설정
			lineStart.x = LOWORD(lParam);
			lineStart.y = HIWORD(lParam);
		}
		else {
			// 다른 도형을 그릴 때 시작 좌표를 설정
			shapeRect.left = LOWORD(lParam);
			shapeRect.top = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		if (currentShape == SHAPE_TRIANGLE) {
		
		}
		else if (currentShape == SHAPE_LINE) {
			// 직선을 그릴 때 끝점 좌표를 설정
			lineEnd.x = LOWORD(lParam);
			lineEnd.y = HIWORD(lParam);
			isShapeVisible = true; // 직선을 그림
		}
		else {
			// 다른 도형을 그릴 때 끝 좌표를 설정
			shapeRect.right = LOWORD(lParam);
			shapeRect.bottom = HIWORD(lParam);
			isShapeVisible = true; // 다른 도형을 그림
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		// 이미지 핸들 해제
		if (hBoxImage != NULL) {
			DeleteObject(hBoxImage);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// 프로그램 진입점
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	HWND hWnd;
	HWND hButton1, hButton2, hButton3, hButton4, hButton5;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"ButtonWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		return 1;
	}

	hWnd = CreateWindow(
		L"ButtonWindowClass", L"202307075_김선호", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 800, 480, NULL, NULL, hInstance, NULL);

	SetWindowPos(hWnd, 0, 0, 0, 800, 480, SWP_NOMOVE | SWP_NOZORDER);

	if (!hWnd) {
		return FALSE;
	}

	// 버튼 1 (사각형)
	hButton1 = CreateWindow(
		L"BUTTON", L"square", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 20, 160, 64, hWnd, (HMENU)1, hInstance, NULL);

	// 버튼 2 (원)
	hButton2 = CreateWindow(
		L"BUTTON", L"circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 90, 160, 64, hWnd, (HMENU)2, hInstance, NULL);

	// 3번 버튼 (box)
	hButton3 = CreateWindow(
		L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		20, 160, 160, 64, hWnd, (HMENU)3, hInstance, NULL);

	// 버튼 4 (직선)
	hButton4 = CreateWindow(
		L"BUTTON", L"line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 230, 160, 64, hWnd, (HMENU)4, hInstance, NULL);

	// 버튼 5 (제거)
	hButton5 = CreateWindow(
		L"BUTTON", L"remove", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 300, 160, 64, hWnd, (HMENU)5, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}