#include <windows.h>

// ���� ����
HINSTANCE hInst;
LPCTSTR szWindowClass = TEXT("Win32Cube");
LPCTSTR szTitle = TEXT("Win32 Cube");

// �Լ� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCube(HDC hdc, int startX, int startY, int endX, int endY);

// ���� ����: �巡�� �� ����, ������, ����
BOOL isDragging = FALSE;
POINT startDrag, endDrag;

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // �ν��Ͻ� �ʱ�ȭ
    hInst = hInstance;

    // ������ Ŭ���� ���
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
        MessageBox(nullptr, TEXT("������ Ŭ���� ��� ����!"), szTitle, MB_ICONERROR);
        return 1;
    }

    // ������ ����
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        MessageBox(nullptr, TEXT("������ ���� ����!"), szTitle, MB_ICONERROR);
        return 1;
    }

    // ������ ǥ��
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // �޽��� ����
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

        // �巡�� ���̸� ������ü �׸���
        if (isDragging) {
            DrawCube(hdc, startDrag.x, startDrag.y, endDrag.x, endDrag.y);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
        // �巡�� ����
        isDragging = TRUE;
        startDrag.x = LOWORD(lParam);
        startDrag.y = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        // �巡�� ����
        isDragging = FALSE;
        endDrag.x = LOWORD(lParam);
        endDrag.y = HIWORD(lParam);

        // �����츦 �ٽ� �׸����� ������ WM_PAINT �̺�Ʈ �߻�
        InvalidateRect(hWnd, nullptr, FALSE);
        break;
    case WM_MOUSEMOVE:
        // ���콺 �̵� ���� ��� ���� ���콺 ��ġ ����
        if (isDragging) {
            endDrag.x = LOWORD(lParam);
            endDrag.y = HIWORD(lParam);

            // �����츦 �ٽ� �׸����� ������ WM_PAINT �̺�Ʈ �߻�
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
    // ������ü �׸���
    Rectangle(hdc, startX, startY, endX, endY);
}
