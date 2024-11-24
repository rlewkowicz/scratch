#include <Windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <dxgi.h>
#include <assert.h>
#include <vector>
#include <stdio.h> 
#include "WinCompHandler.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dcomp.lib")

// Global variables
HWND hwnd = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
Compositor* pCompositor = nullptr;
Layer* pLayer = nullptr;
ID3D11Texture2D* pBackBuffer = nullptr;
ID3D11Texture2D* pBlueTexture = nullptr;

// Function declarations
HWND CreateAppWindow(HINSTANCE hInstance);
HRESULT CreateDevice();
HRESULT CreateBlueTexture(UINT width, UINT height);
void Render();

// Modify CreateBlueTexture2D to take width and height
HRESULT CreateBlueTexture2D(
    ID3D11Device* pDevice,
    ID3D11DeviceContext* pContext,
    UINT width,
    UINT height,
    ID3D11Texture2D** ppTexture)
{
    if (!pDevice || !ppTexture)
        return E_INVALIDARG;

    // Define the texture description
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width; // Texture width
    texDesc.Height = height; // Texture height
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 32-bit color format
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

    // Create an array to hold the pixel data
    const UINT textureSize = texDesc.Width * texDesc.Height * 4; // 4 bytes per pixel (BGRA)
    std::vector<BYTE> pixelData(textureSize);

    // Fill the pixel data with blue color
    for (UINT i = 0; i < textureSize; i += 4)
    {
        pixelData[i]     = 255; // Blue
        pixelData[i + 1] = 0;   // Green
        pixelData[i + 2] = 0;   // Red
        pixelData[i + 3] = 255; // Alpha
    }

    // Define the initial data
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixelData.data();
    initData.SysMemPitch = texDesc.Width * 4; // Row size in bytes

    // Create the texture
    HRESULT hr = pDevice->CreateTexture2D(&texDesc, &initData, ppTexture);
    if (FAILED(hr))
    {
        return hr;
    }

    // Optional: Update subresource if needed
    if (pContext)
    {
        pContext->UpdateSubresource(*ppTexture, 0, nullptr, pixelData.data(), texDesc.Width * 4, 0);
    }

    return S_OK;
}

HRESULT CreateBlueTexture(UINT width, UINT height)
{
    HRESULT hr = CreateBlueTexture2D(pDevice, pContext, width, height, &pBlueTexture);
    return hr;
}

HWND CreateAppWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[]  = L"MyWindowClass";

    WNDCLASS wc = {};

    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Set window style to WS_POPUP to have no border and not resizable
    DWORD dwStyle =  WS_VISIBLE | WS_OVERLAPPED;

    // Adjust the window size to get a client area of 1024x1024
    RECT rect = { 0, 0, 1024, 1024 };
    AdjustWindowRectEx(&rect, dwStyle, FALSE, 0);
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles.
        CLASS_NAME,                 // Window class
        L"Blue Texture Window",     // Window text
        dwStyle,                    // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return NULL;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    return hwnd;
}

HRESULT CreateDevice()
{
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };

    HRESULT hr = D3D11CreateDevice(
        NULL,                    // specify null to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &pDevice,
        NULL,
        &pContext);

    return hr;
}

void Render()
{
    // Copy blue texture to back buffer
    pContext->CopyResource(pBackBuffer, pBlueTexture);

    // Present the layer
    wrc_present_layer(pLayer);

    // End frame
    wrc_end_frame(pCompositor);
}

int main()
{
    // Get the instance handle
    HINSTANCE hInstance = GetModuleHandle(NULL);

    hwnd = CreateAppWindow(hInstance);
    if (!hwnd)
    {
        printf("Failed to create window.\n");
        return 0;
    }

    HRESULT hr = CreateDevice();
    if (FAILED(hr))
    {
        printf("Failed to create D3D11 device.\n");
        return 0;
    }

    // Initialize the compositor using C bindings
    pCompositor = wrc_new(pDevice, hwnd);

    // Create a layer the size of the window
    pLayer = wrc_create_layer(pCompositor, 1024, 1024);

    // Get the back buffer for the window
    pBackBuffer = (ID3D11Texture2D*)wrc_get_layer_backbuffer(pLayer);

    // Create the blue texture
    hr = CreateBlueTexture(1024, 1024);
    if (FAILED(hr))
    {
        printf("Failed to create blue texture.\n");
        return 0;
    }

    // Main message loop
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Process messages if any
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Render the frame
            Render();
        }
    }

    // Cleanup
    if (pBlueTexture) pBlueTexture->Release();
    if (pBackBuffer) pBackBuffer->Release();
    // No delete function provided for Layer, assuming it's managed internally
    if (pCompositor) wrc_delete(pCompositor);
    if (pContext) pContext->Release();
    if (pDevice) pDevice->Release();

    return (int)msg.wParam;
}
