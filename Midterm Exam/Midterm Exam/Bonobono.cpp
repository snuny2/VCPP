#include <windows.h>
#include <tchar.h>
#include <string>

#define ID_BONOBONO1 101
#define ID_BONOBONO2 102

// 리소스파일로 출력하기 왜 안돼지 + 이미지
// 오기생겨 해보는데 이유를 모르겠다
// 라이언 하면서 찾아봐야겠다

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DrawingAreaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 드로잉 영역 윈도우 핸들
HWND hDrawingArea = NULL;

HINSTANCE hInstance = GetModuleHandle(NULL);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
    HWND hwnd = CreateWindow(L"BoxWindowClass", L"Button Example", WS_OVERLAPPEDWINDOW, 100, 100, 816, 536, NULL, NULL, hInstance, NULL);
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
        HWND hButton = CreateWindow(L"BUTTON", buttonNames[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            xStart, y, buttonWidth, buttonHeight, hwnd, NULL, hInstance, NULL);
        xStart += buttonWidth + margin;
    }

    // 드로잉 영역 윈도우 생성
    hDrawingArea = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
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
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 검정색 테두리
        SelectObject(hdc, hPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH)); // 테두리만 그리고 내부는 투명
        Rectangle(hdc, boxRect.left, boxRect.top, boxRect.right, boxRect.bottom);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CREATE: {
        // 이미지 리소스 로드
        HBITMAP hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_BONOBONO1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

        if (hBitmap) {
            // 이미지를 화면에 표시
            HDC hdc = GetDC(hwnd);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
            BitBlt(hdc, 0, 0, 600, 400, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hBitmapOld);
            DeleteDC(hdcMem);
            ReleaseDC(hwnd, hdc);
        }
        else {
            DWORD error = GetLastError();
            std::wstring errorMsg = L"이미지 리소스를 로드하는 데 실패했습니다. 오류 코드: " + std::to_wstring(error);
            MessageBox(hwnd, errorMsg.c_str(), _T("오류"), MB_ICONERROR);
        }

        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK DrawingAreaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 드로잉 영역을 그리거나 그리기 작업을 수행
        // 여기에 원하는 드로잉 로직을 추가하십시오.

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SETCURSOR: {
        // 드로잉 영역에 마우스가 들어갈 때 커서를 십자로 설정
        if (LOWORD(lParam) == HTCLIENT) {
            HCURSOR hCursor = LoadCursor(NULL, IDC_CROSS);
            SetCursor(hCursor);
            return TRUE; // 이벤트 처리 완료
        }
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 3: // 3번 버튼의 ID를 사용해야 합니다.
            // 이미지 리소스를 로드
            HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(ID_BONOBONO1));
            if (hBitmap != NULL) {
                // 드로잉 영역의 위치와 크기 가져오기
                RECT drawingAreaRect;
                GetWindowRect(hDrawingArea, &drawingAreaRect);

                // 드로잉 영역의 상대적 위치 계산
                POINT pt = { 0, 0 };
                ScreenToClient(hwnd, &pt);
                OffsetRect(&drawingAreaRect, pt.x, pt.y);

                // 이미지의 크기 가져오기
                BITMAP bitmapInfo;
                GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);

                HDC hdc = GetDC(hwnd);
                HDC hdcMem = CreateCompatibleDC(hdc);
                SelectObject(hdcMem, hBitmap);

                // 이미지를 드로잉 영역 위에 중앙에 그릴 위치 계산
                int x = drawingAreaRect.left + (drawingAreaRect.right - drawingAreaRect.left - bitmapInfo.bmWidth) / 2;
                int y = drawingAreaRect.top + (drawingAreaRect.bottom - drawingAreaRect.top - bitmapInfo.bmHeight) / 2;

                BitBlt(hdc, x, y, bitmapInfo.bmWidth, bitmapInfo.bmHeight, hdcMem, 0, 0, SRCCOPY);

                DeleteDC(hdcMem);
                ReleaseDC(hwnd, hdc);
            }
            break;
        }
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}