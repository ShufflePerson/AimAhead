#include "Screen.h"

namespace screen {
    HWND g_hWnd = nullptr;
    std::vector<BoundingBox> current_boxes = {};
    HDC g_hDCBuffer = nullptr; 
    HBITMAP g_hBitmapBuffer = nullptr; 
    int g_bufferWidth = 0;
    int g_bufferHeight = 0;
    HPEN g_hPen = nullptr;
    HBRUSH g_hNullBrush = nullptr;
    bool should_draw = true;

    void set_should_draw(bool value) {
        should_draw = value;
    }

    void ResizeBuffer() {
        if (g_hWnd) {
            RECT clientRect;
            GetClientRect(g_hWnd, &clientRect);
            int newWidth = clientRect.right - clientRect.left;
            int newHeight = clientRect.bottom - clientRect.top;

            if (newWidth > 0 && newHeight > 0 && (newWidth != g_bufferWidth || newHeight != g_bufferHeight)) {
                if (g_hBitmapBuffer) {
                    SelectObject(g_hDCBuffer, (HBITMAP)GetStockObject(WHITE_BRUSH));
                    DeleteObject(g_hBitmapBuffer);
                }
                if (g_hDCBuffer) {
                    DeleteDC(g_hDCBuffer);
                }

                HDC hdc = GetDC(g_hWnd);
                g_hDCBuffer = CreateCompatibleDC(hdc);
                g_hBitmapBuffer = CreateCompatibleBitmap(hdc, newWidth, newHeight);
                ReleaseDC(g_hWnd, hdc);

                SelectObject(g_hDCBuffer, g_hBitmapBuffer);
                g_bufferWidth = newWidth;
                g_bufferHeight = newHeight;
            }
        }
    }


    bool InitializeDrawing() {
        if (!g_hWnd) return false;

        HDC hdc = GetDC(g_hWnd);
        if (!hdc) return false;

        g_hDCBuffer = CreateCompatibleDC(hdc);
        if (!g_hDCBuffer) {
            ReleaseDC(g_hWnd, hdc);
            return false;
        }

        RECT clientRect;
        GetClientRect(g_hWnd, &clientRect);
        g_bufferWidth = clientRect.right - clientRect.left;
        g_bufferHeight = clientRect.bottom - clientRect.top;
        g_hBitmapBuffer = CreateCompatibleBitmap(hdc, g_bufferWidth, g_bufferHeight);
        if (!g_hBitmapBuffer) {
            DeleteDC(g_hDCBuffer);
            ReleaseDC(g_hWnd, hdc);
            return false;
        }
        SelectObject(g_hDCBuffer, g_hBitmapBuffer);

        g_hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); 
        g_hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        ReleaseDC(g_hWnd, hdc);
        return true;
    }

    void clearScreen() {
        if (g_hDCBuffer) {
            HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
            RECT clientRect = { 0, 0, g_bufferWidth, g_bufferHeight };
            FillRect(g_hDCBuffer, &clientRect, hBrush);
        }
    }

    void drawBox(int minX, int minY, int maxX, int maxY, float confidence) {
        if (g_hDCBuffer) {
            COLORREF penColor = RGB(20, static_cast<BYTE>(std::round(confidence * 255.0f)), 20);
            if (g_hPen) {
                LOGPEN lp;
                GetObject(g_hPen, sizeof(LOGPEN), &lp);
                if (lp.lopnColor != penColor) {
                    DeleteObject(g_hPen);
                    g_hPen = CreatePen(PS_SOLID, 1, penColor);
                }
            }
            else {
                g_hPen = CreatePen(PS_SOLID, 1, penColor);
            }

            SelectObject(g_hDCBuffer, g_hPen);
            SelectObject(g_hDCBuffer, g_hNullBrush);

            Rectangle(g_hDCBuffer, minX, minY, maxX, maxY);
        }
    }




    void Present() {
        if (g_hWnd && g_hDCBuffer && g_hBitmapBuffer) {
            HDC hdc = GetDC(g_hWnd);
            if (hdc) {
                BitBlt(hdc, 0, 0, g_bufferWidth, g_bufferHeight, g_hDCBuffer, 0, 0, SRCCOPY);
                ReleaseDC(g_hWnd, hdc);
            }
        }
    }
    LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            clearScreen();

            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    void add_box(BoundingBox box) {
        current_boxes.push_back(box);
    }
    void clear_boxes() {
        current_boxes.clear();
    }

    void main_loop() {
        while (true) {
            if (!should_draw) {
                clearScreen();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            clearScreen();

            for (auto& box : current_boxes) {
                drawBox(box.xmin, box.ymin, box.xmax, box.ymax, box.confidence);
            }
            Present();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    int run() {
        const wchar_t CLASS_NAME[] = L"TransparentClickThroughClass";

        WNDCLASSW wc = {};
        wc.lpfnWndProc = windowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

        if (!RegisterClassW(&wc)) {
            std::cerr << "Error registering class!" << std::endl;
            return 1;
        }

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        int windowWidth = 640;
        int windowHeight = 640;

        int windowX = (screenWidth - windowWidth) / 2;
        int windowY = (screenHeight - windowHeight) / 2;

        g_hWnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TRANSPARENT,
            CLASS_NAME,
            L"Transparent Window",
            WS_POPUP | WS_VISIBLE,
            windowX, windowY, // Use calculated centered position
            windowWidth, windowHeight,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr
        );

        if (!g_hWnd) {
            std::cerr << "Error creating window! Error code: " << GetLastError() << std::endl;
            return 1;
        }

        if (!SetLayeredWindowAttributes(g_hWnd, 0, 255, LWA_ALPHA)) {
            std::cerr << "Error setting transparency! Error code: " << GetLastError() << std::endl;
        }

        if (!SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE)) {
            std::cerr << "Error setting topmost! Error code: " << GetLastError() << std::endl;
        }

        MARGINS margins = { -1 }; 
        DwmExtendFrameIntoClientArea(g_hWnd, &margins);

        ShowWindow(g_hWnd, SW_SHOW);
        UpdateWindow(g_hWnd); 
        InitializeDrawing();
        // Message loop
        MSG msg = {};
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        ResizeBuffer();

        return 0;
    }

}