#include "WindowContainer.h"
#include <memory>
#include "Engine/Input/Input.h"

KeyBoard* Input::keyboard = nullptr;
Mouse* Input::mouse = nullptr;

WindowContainer::WindowContainer()
{
    static bool rawInputInitialize = false;
    if (!rawInputInitialize)
    {
        //RegisterRawInputDevices() // 원시 입력(조이스틱, 마우스의 움직임)을 위해 호출 해야 한다.
        RAWINPUTDEVICE rid = { 0 };
        rid.usUsagePage = 0x01; //Mouse move raw (0x: 16진수
        rid.usUsage = 0x02;
        if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
        {
            Debug::Log(GetLastError(), L"마우스 로우 인풋 초기화에 실패하였습니다.");
            exit(-1);
        }
        rawInputInitialize = true;
    }
    Input::keyboard = &this->keyboard;
    Input::mouse = &this->mouse;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

    switch (msg)
    {
    case WM_KEYDOWN:
    {
        unsigned char key = static_cast<unsigned char>(wParam);
        if (keyboard.IsKeysAutoRepeat())
        {
            keyboard.OnKeyPressed(key);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnKeyPressed(key);
            }
        }
        return 0;
    }
    case WM_KEYUP:
    {
        unsigned char key = static_cast<unsigned char>(wParam);
        keyboard.OnkeyReleased(key);
        return 0;
    }
    case WM_CHAR:
    {
        unsigned char key = static_cast<unsigned char>(wParam);
        if (keyboard.IsCharsAutoRepeat())
        {
            keyboard.OnChar(key);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnChar(key);
            }
        }
        return 0;
    }
    case WM_LBUTTONDOWN:
        mouse.OnLeftPressed(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
        mouse.OnLeftReleased(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_RBUTTONDOWN:
        mouse.OnRightPressed(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_RBUTTONUP:
        mouse.OnRightReleased(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MBUTTONDOWN:
        mouse.OnMiddlePressed(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MBUTTONUP:
        mouse.OnMiddleReleased(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp(LOWORD(lParam), HIWORD(lParam));
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown(LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_MOUSEMOVE:
        mouse.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_INPUT:
    {
        UINT size = 0;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
        if (size > 0)
        {
            unique_ptr<BYTE[]> rawdata = make_unique < BYTE[]>(size); // 먼저 사이즈를 알아온다
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &size, sizeof(RAWINPUTHEADER)) == size)
            {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }
            }
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
