/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>
#include <dcomp.h>
#include <assert.h>
#include <dxgi.h>

#include <stdio.h>

using namespace Microsoft::WRL;

// A basic composition layer backed by a swap-chain for DirectComposition
class Layer {
public:
    Layer(int width, int height, const ComPtr<IDXGIFactory2>& dxgiFactory,
        const ComPtr<ID3D11Device>& d3dDevice,
        const ComPtr<IDCompositionDesktopDevice>& dCompDevice);

    ComPtr<IDXGISwapChain1> mSwapChain;
    ComPtr<IDCompositionVisual2> mVisual;
    ComPtr<ID3D11Texture2D> mBackBuffer;
};

// A basic DirectComposition compositor implementation
class Compositor {
public:
    Compositor(ID3D11Device* aDevice, HWND hWnd);
    ~Compositor();

    // Construct a layer of given dimensions.
    Layer* create_layer(int width, int height);

    void end_frame();

private:
    ComPtr<ID3D11Device> pD3DDevice;
    ComPtr<IDXGIDevice> pDXGIDevice;
    ComPtr<IDCompositionDesktopDevice> pDCompDevice;
    ComPtr<IDCompositionTarget> pCompositionTarget;
    ComPtr<IDCompositionVisual2> pRootVisual;
    ComPtr<IDCompositionDeviceDebug> pDCompDebug;
    ComPtr<IDXGIAdapter> pDXGIAdapter;
    ComPtr<IDXGIFactory2> pIDXGIFactory;
};

// Bindings called by wrench rust impl of the Compositor2 trait
extern "C" {
    Compositor* wrc_new(void* d3d11_device, void* hwnd);

    void wrc_delete(Compositor* compositor);

    Layer* wrc_create_layer(Compositor* compositor, int width, int height);

    void* wrc_get_layer_backbuffer(Layer* layer);

    void wrc_present_layer(Layer* layer);

    void wrc_end_frame(Compositor* compositor);
}

#endif // _WIN32

#endif // COMPOSITOR_H
