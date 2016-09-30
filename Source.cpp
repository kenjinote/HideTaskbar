#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton1;
	static HWND hButton2;
	switch (msg)
	{
	case WM_CREATE:
		hButton1 = CreateWindow(TEXT("BUTTON"), TEXT("タスクバーを隠す"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("タスクバーを表示"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)101, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hButton1, 10, 10, 256, 32, TRUE);
		MoveWindow(hButton2, 10, 50, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			HWND hTray = FindWindow(TEXT("Shell_TrayWnd"), TEXT(""));
			if (hTray)
			{
				APPBARDATA ABData = { sizeof(APPBARDATA) };
				ABData.hWnd = hTray;
				ABData.lParam = ABS_AUTOHIDE;
				SHAppBarMessage(ABM_SETSTATE, &ABData);

				ShowWindow(hTray, SW_HIDE);
			}
		}
		else if (LOWORD(wParam) == 101)
		{
			HWND hTray = FindWindow(TEXT("Shell_TrayWnd"), TEXT(""));
			if (hTray)
			{
				ShowWindow(hTray, SW_SHOW);

				APPBARDATA ABData = { sizeof(APPBARDATA) };
				ABData.hWnd = hTray;
				ABData.lParam = ABS_ALWAYSONTOP;
				SHAppBarMessage(ABM_SETSTATE, &ABData);

				SetForegroundWindow(hTray); // スタートボタンが押せなくなる問題を回避
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("タスクーバーの表示/非表示"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}