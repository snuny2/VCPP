#include <windows.h>

// 전역 변수
HINSTANCE hInst;
LPCTSTR szWindowClass = TEXT("Win32Cube");
LPCTSTR szTitle = TEXT("Win32 Cube");

// 함수 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCube(HDC hdc, int startX, int startY, int endX, int endY);

// 전역 변수: 드래그 중 여부, 시작점, 끝점
BOOL isDragging = FALSE;
POINT startDrag, endDrag;

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 인스턴스 초기화
    hInst = hInstance;

    // 윈도우 클래스 등록
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(nullptr, TEXT("윈도우 클래스 등록 실패!"), szTitle, MB_ICONERROR);
        return 1;
    }

    // 윈도우 생성
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        MessageBox(nullptr, TEXT("윈도우 생성 실패!"), szTitle, MB_ICONERROR);
        return 1;
    }

    // 윈도우 표시
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 드래그 중이면 정육면체 그리기
        if (isDragging) {
            DrawCube(hdc, startDrag.x, startDrag.y, endDrag.x, endDrag.y);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
        // 드래그 시작
        isDragging = TRUE;
        startDrag.x = LOWORD(lParam);
        startDrag.y = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        // 드래그 종료
        isDragging = FALSE;
        endDrag.x = LOWORD(lParam);
        endDrag.y = HIWORD(lParam);

        // 윈도우를 다시 그리도록 강제로 WM_PAINT 이벤트 발생
        InvalidateRect(hWnd, nullptr, FALSE);
        break;
    case WM_MOUSEMOVE:
        // 마우스 이동 중인 경우 현재 마우스 위치 저장
        if (isDragging) {
            endDrag.x = LOWORD(lParam);
            endDrag.y = HIWORD(lParam);

            // 윈도우를 다시 그리도록 강제로 WM_PAINT 이벤트 발생
            InvalidateRect(hWnd, nullptr, FALSE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawCube(HDC hdc, int startX, int startY, int endX, int endY) {
    // 정육면체 그리기
    Rectangle(hdc, startX, startY, endX, endY);
}
