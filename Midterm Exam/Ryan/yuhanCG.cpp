#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

// ���� ����
HINSTANCE hInst;
HWND hButton;
bool drawImage = false;
bool isDragging = false;
int imageX = 100;
int imageY = 100;
int dragStartX, dragStartY;
Image* ryanImage = NULL;

// GDI+ �ʱ�ȭ �Լ�
ULONG_PTR gdiplusToken;

// GDI+ �ʱ�ȭ �Լ�
void InitializeGDIPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

// GDI+ ���� �Լ�
void ShutdownGDIPlus()
{
    GdiplusShutdown(gdiplusToken);
}

// â ���ν��� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        // PNG ���� �ε�
        LPCWSTR filePath = L"Ryan.png";
        ryanImage = Image::FromFile(filePath);
    if (ryanImage == NULL)
    {
        MessageBox(NULL, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
        //�ƹ��͵� �ȳ���
    }
}
    break;

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == BN_CLICKED)
        {
            drawImage = !drawImage;

            // ���ο� �ڵ�: ��ư�� Ŭ���� ������ ����
            SendMessage(hButton, BM_CLICK, 0, 0);

            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_LBUTTONDOWN:
        if (drawImage)
        {
            isDragging = true;
            dragStartX = LOWORD(lParam);
            dragStartY = HIWORD(lParam);
        }
        break;

    case WM_LBUTTONUP:
        if (isDragging)
        {
            isDragging = false;
            imageX = dragStartX;
            imageY = dragStartY;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_MOUSEMOVE:
        if (isDragging)
        {
            int deltaX = LOWORD(lParam) - dragStartX;
            int deltaY = HIWORD(lParam) - dragStartY;

            imageX = dragStartX + deltaX;
            imageY = dragStartY + deltaY;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (drawImage)
        {
            // �̹��� �׸���
            Graphics graphics(hdc);
            graphics.DrawImage(ryanImage, imageX, imageY);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        delete ryanImage;
        ShutdownGDIPlus();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// â ��� �Լ�
ATOM MyRegisterClass(HINSTANCE hInstance)
{
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
    wcex.lpszClassName = L"MyWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}

// �ʱ�ȭ �Լ�
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ ����

    HWND hWnd = CreateWindow(L"MyWindowClass", L"Image Drawing Example",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    hButton = CreateWindow(L"BUTTON", L"Toggle Image",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 120, 30, hWnd, NULL, hInstance, NULL);

    if (!hButton)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

// ���� �Լ�
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // GDI+ �ʱ�ȭ
    InitializeGDIPlus();

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
