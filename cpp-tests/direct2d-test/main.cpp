#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

HINSTANCE hInst;

ID2D1Factory* pD2DFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitDeviceIndependentResources();
void                DiscardDeviceIndependentResources();
BOOL                InitDeviceResources(HWND);
void                DiscardDeviceResources();
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    MyRegisterClass(hInstance);

    InitDeviceIndependentResources();

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"Direct2DTest";

    return RegisterClassExW(&wcex);
}

BOOL InitDeviceIndependentResources()
{
    return SUCCEEDED(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    ));
}

void DiscardDeviceIndependentResources()
{
    if (pD2DFactory != nullptr)
    {
        pD2DFactory->Release();
        pD2DFactory = nullptr;
    }
}

BOOL InitDeviceResources(HWND hWnd)
{
    if (pRenderTarget != nullptr)
        return TRUE;

    RECT rc;
    GetClientRect(hWnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top);

    return SUCCEEDED(pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWnd, size),
        &pRenderTarget));
}

void DiscardDeviceResources()
{
    if (pRenderTarget != nullptr)
    {
        pRenderTarget->Release();
        pRenderTarget = nullptr;
    }
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowExW(0L, L"Direct2DTest", L"Direct2D Test", 0x80000000L | 0x10000000L, 0, 0, w, h, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case WM_QUIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYUP:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        if (!InitDeviceResources(hWnd))
            break;

        pRenderTarget->BeginDraw();
        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        pRenderTarget->Clear(D2D1::ColorF(0x005370));
        HRESULT hr = pRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET)
            DiscardDeviceResources();
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
