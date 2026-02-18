#include <windows.h>

const char CLASS_NAME[] = "MVPWindow";

int selectedIndex = 0;
const char* menuItems[] = { "Settings", "Quit" };
const int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_DOWN)
            {
                selectedIndex = (selectedIndex + 1) % menuCount;
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            else if (wParam == VK_UP)
            {
                selectedIndex = (selectedIndex - 1 + menuCount) % menuCount;
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            else if (wParam == VK_RETURN)
            {
                if (selectedIndex == 1)
                    PostQuitMessage(0);
            }
            return 0;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &clientRect, blackBrush);
            DeleteObject(blackBrush);

            int itemHeight = 60;
            int spacing = 20;
            int startY = 100;

            for (int i = 0; i < menuCount; ++i)
            {
                RECT rect = { 200, startY + i * (itemHeight + spacing), 560, startY + i * (itemHeight + spacing) + itemHeight };

                HBRUSH brush;
                COLORREF textColor;
                if (i == selectedIndex)
                {
                    brush = CreateSolidBrush(RGB(0, 255, 0));
                    textColor = RGB(255, 255, 255);
                }
                else
                {
                    brush = CreateSolidBrush(RGB(66, 66, 66));
                    textColor = RGB(255, 255, 255);
                }

                FillRect(hdc, &rect, brush);
                DeleteObject(brush);

                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, textColor);
                DrawTextA(hdc, menuItems[i], -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassA(&wc);

    RECT rect = { 0, 0, 760, 480 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Xbox UI Debug Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
