#pragma once
#ifndef PCH_H
#define PCH_H

// Define these to avoid including unnecessary parts of the Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Include standard Windows headers first
#include <windows.h>
#include <presentation.h>
#include <sdkddkver.h>
#include <unknwn.h>   // For IUnknown and related COM interfaces
#include <roapi.h>    // For RoInitialize, RoUninitialize, RoGetActivationFactory
#include <activation.h> // For IActivationFactory
#include <ShellScalingAPI.h>
#include <DispatcherQueue.h>
#include <shcore.h>
#include <shobjidl_core.h>
#include <ppltasks.h>
#include <combaseapi.h>


// DirectX and Direct2D headers
#include <dcomp.h>
#include <d3d11.h>
#include <d3d11_3.h>
#include <d3d11_4.h>
#include <dxgi1_2.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <Windows.Graphics.DirectX.Direct3D11.interop.h>
#include <Windows.Graphics.Interop.h>
#include <windows.ui.composition.h>
#include <windows.ui.composition.desktop.h>
#include <windows.ui.composition.interop.h>
#include <windows.foundation.h>
#include <windows.foundation.numerics.h>
#include <Windows.Graphics.h>

// WRL headers for ComPtr
#include <wrl.h>
#include <wrl/client.h>

// CRT headers
#include <cstdio>
#include <iostream>
#include <memory>
#include <atomic>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <optional>
#include <future>
#include <mutex>

// WIL (Windows Implementation Library)
#include <wil/cppwinrt.h>
#include <wil/resource.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/result_macros.h>
#include <wil/coroutine.h>
#include <wil/com.h>


// C++/WinRT headers
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Security.Authorization.AppCapabilityAccess.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.UI.Composition.Interactions.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.Web.Syndication.h>

// Helper headers
#include <robmikh.common/composition.interop.h>
#include <robmikh.common/composition.desktop.interop.h>
#include <robmikh.common/d2dHelpers.h>
#include <robmikh.common/d3d11Helpers.h>
#include <robmikh.common/d3d11Helpers.desktop.h>
#include <robmikh.common/capture.desktop.interop.h>
#include <robmikh.common/hwnd.interop.h>
#include <robmikh.common/direct3d11.interop.h>
#include <robmikh.common/dispatcherqueue.desktop.interop.h>
#include <robmikh.common/stream.interop.h>
#include <robmikh.common/ControlsHelper.h>

// Link necessary libraries
#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")

#endif // PCH_H
