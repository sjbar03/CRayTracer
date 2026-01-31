#include "windows_display.h"
#include <stdio.h>
#include <windows.h>
#include <stdint.h>

// Macros
#define RGB_BITS 16 // number of bits attributed to each pixel
#define WIN_W 640
#define WIN_H 480

HINSTANCE hInstance;
ATOM wcClass;

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;

static int quit = 0;
Frame frame = {0};



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            quit = 1;
            return 0;

        case WM_SIZE:

            frame.width  = LOWORD(lParam);
            frame.height = HIWORD(lParam);

            frame_bitmap_info.bmiHeader.biWidth  = frame.width;
            frame_bitmap_info.bmiHeader.biHeight = -frame.height;


            if (frame_bitmap)
            {
                DeleteObject(frame_bitmap);
            }

            frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, (void**) &frame.pixels, 0, 0);
            SelectObject(frame_device_context, frame_bitmap);

            break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int nCmdShow)
{
    // Register the window class.
    char CLASS_NAME[]  = "Game Window";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    ZeroMemory(&frame_bitmap_info, sizeof(frame_bitmap_info));

    frame_bitmap_info.bmiHeader.biSize        = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes      = 1;
    frame_bitmap_info.bmiHeader.biBitCount    = 16;
    frame_bitmap_info.bmiHeader.biCompression = BI_BITFIELDS;

    // 565 masks go in bmiColors (first 3 DWORDs)
    ((DWORD*)frame_bitmap_info.bmiColors)[0] = 0xF800; // Red mask
    ((DWORD*)frame_bitmap_info.bmiColors)[1] = 0x07E0; // Green mask
    ((DWORD*)frame_bitmap_info.bmiColors)[2] = 0x001F; // Blue mask

    frame_device_context = CreateCompatibleDC(0);

    // Create the window.

    static HWND hwnd;
    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Ray Tracer",    // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT,  // x 
        CW_USEDEFAULT,  // y
        WIN_W,            // width
        WIN_H,            // height

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    frame.hwnd = hwnd;

    ShowWindow(hwnd, nCmdShow);

    while (!quit)
    {
        static MSG message = { 0 };
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }

    }

    return 0;

}

void window_init()
{
    WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT) ;
    return;
}

void draw()
{
    HDC dc = GetDC(frame.hwnd);   

    BitBlt(dc, 0, 0,
        frame.width, frame.height,
        frame_device_context,
        0, 0,
        SRCCOPY);

    ReleaseDC(frame.hwnd, dc);
}