#include <windows.h>

// 프로젝트 > 속성 > 구성속성 > 링커 > 시스템 > 창

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
POINT points[3]; // 삼각형의 꼭지점 좌표를 저장하는 배열
POINT lineStart, lineEnd; // 직선의 시작점과 끝점을 저장하는 변수

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
			hBrush = CreateSolidBrush(RGB(0, 255, 0)); // 녹색 삼각형
			SelectObject(hdc, hBrush);
			POINT vertices[3];
			vertices[0] = points[0];
			vertices[1] = points[1];
			vertices[2] = points[2];
			Polygon(hdc, points, 3); // 삼각형을 그립니다.
			DeleteObject(hBrush);
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
			// 첫 번째 버튼 클릭
			currentShape = SHAPE_RECTANGLE;
		}
		else if (LOWORD(wParam) == 2) {
			EraseShape(hWnd);
			// 두 번째 버튼 클릭
			currentShape = SHAPE_ELLIPSE;
		}
		else if (LOWORD(wParam) == 3) {
			EraseShape(hWnd);
			// 세 번째 버튼 클릭
			currentShape = SHAPE_TRIANGLE;
		}
		else if (LOWORD(wParam) == 4) {
			EraseShape(hWnd);
			// 세 번째 버튼 클릭
			currentShape = SHAPE_LINE;
		}
		else if (LOWORD(wParam) == 5) {
			// 다섯 번째 버튼 클릭
			EraseShape(hWnd);
		}
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawShape(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
		if (currentShape == SHAPE_TRIANGLE) {
			// 삼각형을 그릴 때 좌표를 설정
			points[0].x = LOWORD(lParam);
			points[0].y = HIWORD(lParam);
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
			// 삼각형을 그릴 때 끝 좌표를 설정
			points[1].x = LOWORD(lParam);
			points[1].y = HIWORD(lParam);
			points[2].x = 2 * points[0].x - points[1].x; // 세 번째 점 계산
			points[2].y = points[1].y;
			isShapeVisible = true; // 삼각형을 그림
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
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return FALSE;
	}

	hButton1 = CreateWindow(
		L"BUTTON", L"square", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 20, 200, 60, hWnd, (HMENU)1, hInstance, NULL);

	hButton2 = CreateWindow(
		L"BUTTON", L"circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 90, 200, 60, hWnd, (HMENU)2, hInstance, NULL);

	hButton3 = CreateWindow(
		L"BUTTON", L"triangle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 160, 200, 60, hWnd, (HMENU)3, hInstance, NULL);

	hButton4 = CreateWindow(
		L"BUTTON", L"line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 230, 200, 60, hWnd, (HMENU)4, hInstance, NULL);

	hButton5 = CreateWindow(
		L"BUTTON", L"remove", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 300, 200, 60, hWnd, (HMENU)5, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}