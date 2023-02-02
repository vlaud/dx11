#include "WindowContainer.h"

LRESULT CALLBACK NewMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    default:
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pWindow->WindowProc(hWnd, msg, wParam, lParam);
    }
}
LRESULT CALLBACK MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* pWin = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        if (pWin == nullptr)
        {
            Debug::Log(L"윈도우 생성에 실패");
            exit(-1);
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWin));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(NewMessageProc));
        pWin->Awake();
        return pWin->WindowProc(hWnd, msg, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

int RenderWindow::ScreenWidth = 0;
int RenderWindow::ScreenHeight = 0;

void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc = { 0 };
    //WNDCLASSEX wc{ 0 }; 같은방법
    //ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = MessageProc;
    wc.hInstance = hInst;
    wc.lpszClassName = winClass.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}

bool RenderWindow::Initialize(WindowContainer* pWindow, HINSTANCE hInst, wstring title, wstring winClass, int w, int h)
{
    this->hInst = hInst;
    ScreenWidth = w;
    ScreenHeight = h;
    Title = title;
    this->winClass = winClass;
    RegisterWindowClass();

    RECT wr = { 0 };
    wr.left = 100;
    wr.top = 100;
    wr.right = wr.left + w;
    wr.bottom = wr.top + h;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

    handle = CreateWindowEx(0, winClass.c_str(), Title.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        wr.left, wr.top, //Window Postion
        wr.right - wr.left, wr.bottom - wr.top,
        0, 0, hInst, pWindow);

    if (handle == 0)
    {
        //Error
        return false;
    }

    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);

    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg = { 0 };
    while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL)
    {
        if (!IsWindow(handle))
        {
            handle = 0;
            UnregisterClass(winClass.c_str(), hInst);
            return false;
        }
    }
    return true;
}

RenderWindow::~RenderWindow()
{
    if (handle != 0)
    {
        UnregisterClass(winClass.c_str(), hInst);
        DestroyWindow(handle);
    }
}

HWND RenderWindow::GetHandle()
{
    return handle;
}
