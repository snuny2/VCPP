#include <windows.h>
#include <WinUser.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HBITMAP hBitmap = NULL;
HWND hDrawingArea = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
	case WM_CREATE: {
		int buttonWidth = 160;
		int buttonHeight = 64;
		int margin = 16;

		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		// 박스 컨트롤 생성
		hBox = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			8, 8, rcClient.right - 16, rcClient.bottom - 16, hWnd, NULL, NULL, NULL);

		// 박스의 배경색을 뷰의 색상과 같게 설정
		SendMessage(hBox, WM_SETTEXT, 0, (LPARAM)L"");
		SendMessage(hBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
		SendMessage(hBox, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hBox), (LPARAM)hBoxBackground);

		for (int i = 0; i < 5; i++) {
			int x = margin;
			int y = margin + i * (buttonHeight + margin); // 세로로 나열

			LPCWSTR buttonText; // 버튼의 텍스트를 설정할 변수

			// 버튼의 텍스트를 설정
			switch (i) {
			case 0:
				buttonText = L"Box";
				break;
			case 1:
				buttonText = L"Circle";
				break;
			case 2:
				buttonText = L"bonobono";
				break;
			case 3:
				buttonText = L"Ryan";
				break;
			case 4:
				buttonText = L"Cube";
				break;
			default:
				buttonText = L"Button";
				break;
			}

			// 버튼 컨트롤 생성
			hButtons[i] = CreateWindow(L"BUTTON", buttonText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				x, y, buttonWidth, buttonHeight, hWnd, (HMENU)(i + 1), NULL, NULL);
		}

		// 드로잉 영역 컨트롤 생성
		int drawingAreaWidth = 585;
		int drawingAreaHeight = rcClient.bottom - 2 * margin;
		int drawingAreaX = rcClient.right - drawingAreaWidth - margin;
		int drawingAreaY = margin;

		hDrawingArea = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_NOTIFY,
			drawingAreaX, drawingAreaY, drawingAreaWidth, drawingAreaHeight, hWnd, NULL, NULL, NULL);

		// 드로잉 영역의 배경색을 설정
		SendMessage(hDrawingArea, WM_SETTEXT, 0, (LPARAM)L"");
		SendMessage(hDrawingArea, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
		SendMessage(hDrawingArea, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hDrawingArea), (LPARAM)hDrawingAreaBackground);

		// 드로잉 영역의 마우스 커서 설정
		HCURSOR hCrossCursor = LoadCursor(NULL, IDC_CROSS);
		SetClassLongPtr(hDrawingArea, GCLP_HCURSOR, (LONG_PTR)hCrossCursor);

		return 0;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 3: // "bonobono" 버튼 클릭
			hBitmap = (HBITMAP)LoadImage(NULL, L"img/bonobono1.png", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (hBitmap) {
				InvalidateRect(hDrawingArea, NULL, TRUE);
			}
			break;
			// 다른 버튼 클릭 이벤트 처리
			// ...
			break;
		}
	case WM_CTLCOLORSTATIC: {
		if ((HWND)lParam == hBox) {
			// 박스의 배경색을 설정
			return (LRESULT)hBoxBackground;
		}
		else if ((HWND)lParam == hDrawingArea) {
			// 드로잉 영역의 배경색을 설정
			return (LRESULT)hDrawingAreaBackground;
		}
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDrawingArea, &ps);

		if (hBitmap) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), &bm);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			StretchBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			DeleteDC(hdcMem);
		}

		EndPaint(hDrawingArea, &ps);
		break;
	}
	case WM_DESTROY: {
		if (hBitmap) {
			DeleteObject(hBitmap);
		}
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
				   return 0;
	}
}
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
		WNDCLASS wc = { 0 };
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.hbrBackground = CreateSolidBrush(RGB(255, 240, 200));
		wc.lpszClassName = L"중간고사"; // RegisterClass에서 클래스 이름 수정
		RegisterClass(&wc);

		HWND hWnd = CreateWindow(L"중간고사", L"My Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 480, NULL, NULL, hInstance, NULL);
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME);
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

		HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		MSG msg = { 0 };
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}