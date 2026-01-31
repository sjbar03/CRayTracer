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

struct {
    int width;
    int height;
    uint16_t *pixels;
} frame = {0};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            quit = 1;
            return 0;
        case WM_PAINT:
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt( device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom- paint.rcPaint.top,
                    frame_device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    SRCCOPY);
            EndPaint(hwnd, &paint);
            break;
        case WM_SIZE:
            // frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            // frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);
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

            // frame.width = LOWORD(lParam);
            // frame.height = HIWORD(lParam);
            break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int nCmdShow)
{
    // Register the window class.
    char CLASS_NAME[]  = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    // frame_bitmap_info.bmiHeader.biPlanes = 1;
    // frame_bitmap_info.bmiHeader.biBitCount = RGB_BITS;
    // frame_bitmap_info.bmiHeader.biCompression = BI_BITFIELDS;
    // frame_device_context = CreateCompatibleDC(0);

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
        "Learn to Program Windows",    // Window text
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

    ShowWindow(hwnd, nCmdShow);

    while (!quit)
    {
        static MSG message = { 0 };
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }
        static unsigned int p = 0;
        // frame.pixels[(p++)%(frame.width*frame.height)] = rand();
        // frame.pixels[rand()%(frame.width*frame.height)] = 0;
        int n = frame.width * frame.height;
        if (frame.pixels && n > 0) {
            frame.pixels[(p++) % n] = (uint16_t)rand();
            frame.pixels[rand() % n] = 0;
        }

        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }

    return 0;

}

int main()
{
    WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT) ;
    return 0;
}