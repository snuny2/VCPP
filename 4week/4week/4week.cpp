#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

POINT startPoint = { 0 };
POINT endPoint = { 0 };
int isMouseLButtonPressed = 0;

RECT customRect = { 0 };

// 사각형 이동 관련 변수 추가
int isMovingRect = 0;
POINT offset = { 0 };
// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		startPoint.x = LOWORD(lParam);
		startPoint.y = HIWORD(lParam);

		// 마우스 왼쪽 버튼을 클릭하면서 사각형 내부인지 확인
		if (PtInRect(&customRect, startPoint))
		{
			// 마우스 클릭 위치와 사각형 위치 사이의 차이를 계산하여 이동에 사용
			offset.x = startPoint.x - customRect.left;
			offset.y = startPoint.y - customRect.top;
			isMovingRect = 1;
		}
		else
		{
			isMouseLButtonPressed = 1;
		}
	}
	break;

	//강의에 안나왔지만 마우스가 움직일때의 이벤트를 뜻합니다.
	case WM_MOUSEMOVE:
	{
		if (isMouseLButtonPressed)
		{
			endPoint.x = LOWORD(lParam);
			endPoint.y = HIWORD(lParam);

			// WM_PAINT 메시지를 유발하여 네모를 화면에 그립니다.
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (isMovingRect)
		{
			// 마우스 이동 중에 사각형 위치 업데이트
			int dx = LOWORD(lParam) - startPoint.x;
			int dy = HIWORD(lParam) - startPoint.y;

			customRect.left += dx;
			customRect.top += dy;
			customRect.right += dx;
			customRect.bottom += dy;

			startPoint.x = LOWORD(lParam);
			startPoint.y = HIWORD(lParam);

			// WM_PAINT 메시지를 유발하여 사각형 이동을 업데이트합니다.
			InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	break;


	case WM_LBUTTONUP:
	{
		endPoint.x = LOWORD(lParam);
		endPoint.y = HIWORD(lParam);

		if (isMouseLButtonPressed)
		{
			customRect.left = min(startPoint.x, endPoint.x);
			customRect.top = min(startPoint.y, endPoint.y);
			customRect.right = max(startPoint.x, endPoint.x);
			customRect.bottom = max(startPoint.y, endPoint.y);

			isMouseLButtonPressed = 0;

			// WM_PAINT 메시지를 유발하여 사각형을 그립니다.
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (isMovingRect)
		{
			isMovingRect = 0;
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 화면을 지우고 이전에 그린 사각형을 다시 그립니다.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		// 사용자가 선택한 위치와 크기로 사각형 그리기
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255)); // 핑크 브러시 생성
		FillRect(hdc, &customRect, hBrush); // 사용자가 선택한 위치와 크기로 사각형 그리기
		DeleteObject(hBrush);

		EndPaint(hwnd, &ps);
	}
	break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return S_OK;
}
#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
	/* 윈도우 클래스 선언.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// 모두 0으로 초기화.

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성 , 고정
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("202307075_김선호"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	// 오류 검사.
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
		exit(-1);
	}

	// 창 보이기.
	ShowWindow(hwnd, SW_SHOW); // 창 띄우고
	UpdateWindow(hwnd); // 업데이트해야 보임. 한 쌍으로 쓴다고 보면 됨.



	// 메시지 처리 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		// 메시지 처리.
		if (GetMessage(&msg, hwnd, 0, 0))
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미인데 지운다는 것임.
		{

			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);

		}
		/*else
		{

		}*/
	}

	//종료 메시지 보내기
	return (int)msg.wParam;

}
