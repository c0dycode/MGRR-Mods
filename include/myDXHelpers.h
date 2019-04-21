#pragma once

#define D3D9

#ifdef D3D9
#include "d3d9.h"
#include <dwmapi.h>
#pragma comment(lib, "d3d9.lib")
#elif D3D11
#include "d3d11.h"
#pragma comment(lib, "d3d11.lib")
#endif

#pragma comment(lib, "d3dcompiler.lib")
#include <string>

//#define MAX_VERTEX_BUFFER 512 * 1024
//#define MAX_INDEX_BUFFER 128 * 1024
//#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
//#define NK_INCLUDE_STANDARD_VARARGS
//#define NK_INCLUDE_DEFAULT_ALLOCATOR
//#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
//#define NK_INCLUDE_FONT_BAKING
//#define NK_INCLUDE_DEFAULT_FONT
//#include "F:/Programmieren/C++/nuklear/nuklear.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

#ifdef D3D9
//#include "F:/Programmieren/C++/nuklear/demo/d3d9/nuklear_d3d9.h"
//#include "F:/Programmieren/C++/Imgui/imgui.h"
//#include "F:/Programmieren/C++/myhelpers/include/ImGui/directx9_example/imgui_impl_dx9.h"
//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#elif D3D11
//#include "F:/Programmieren/C++/nuklear/demo/d3d9/nuklear_d3d11.h"
#include "F:/Programmieren/C++/myhelpers/include/ImGui/directx11_example/imgui_impl_dx11.h"
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RESETINDEX 16
#define PRESENTINDEX 17
#define ENDSCENEINDEX 42

typedef HRESULT(_stdcall * f_EndScene)(IDirect3DDevice9 * pDevice);
f_EndScene oEndScene;
typedef HRESULT(__stdcall * f_resetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* presentParam);
f_resetFn oResetFunction;

#ifdef D3D9
typedef HRESULT(__stdcall * f_PresentFn)(RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
f_PresentFn oPresentFunction;
#endif

static IDirect3DDevice9 *device								= nullptr;
static IDirect3DDevice9Ex *deviceEx							= nullptr;
static D3DPRESENT_PARAMETERS present						= {0};
static D3DDEVICE_CREATION_PARAMETERS d3dcreationParameters	= {0};
static void * d3d9Device[119];
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

static WNDPROC owndproc										= NULL;

// All D3D9Device-method indexes
enum MyDX3DDeviceIndexes
{
	QueryInterface = 0,
	AddRefIndex,
	ReleaseIndex,
	TestCooperativeLevelIndex,
	GetAvailableTextureMemIndex,
	EvictManagerResourcesIndex,
	GetDirect3DIndex,
	GetDeviceCapsIndex,
	GetDisplayModeIndex,
	GetCreationParametersIndex,
	SetCursorPropertiesIndex,
	SetCursorPositionIndex,
	ShowCursorIndex,
	CreateAdditionalSwapChainIndex,
	GetSwapChainIndex,
	GetNumberOfSwapChainsIndex,
	ResetIndex,
	PresentIndex,
	GetBackBufferIndex,
	GetRasterStatusIndex,
	SetDialogBoxModeIndex,
	SetGammaRampIndex,
	GetGammaRampIndex,
	CreateTextureIndex,
	CreateVolumeTextureIndex,
	CreateCubeTextureIndex,
	CreateVertexBufferIndex,
	CreateIndexBufferIndex,
	CreateRenderTargetIndex,
	CreateDepthStencilSurfaceIndex,
	UpdateSurfaceIndex,
	UpdateTextureIndex,
	GetRenderTargetDataIndex,
	GetFrontBufferDataIndex,
	StretchRectIndex,
	ColorFillIndex,
	CreateOffscreenPlainSurfaceIndex,
	SetRenderTargetIndex,
	GetRenderTargetIndex,
	SetDepthStencilSurfaceIndex,
	GetDepthStencilSurfaceIndex,
	BeginSceneIndex,
	EndSceneIndex,
	ClearIndex,
	SetTransformIndex,
	GetTransformIndex,
	MultiplyTransformIndex,
	SetViewportIndex,
	GetViewportIndex,
	SetMaterialIndex,
	GetMaterialIndex,
	SetLightIndex,
	GetLightIndex,
	LightEnableIndex,
	GetLightEnableIndex,
	SetClipPlaneIndex,
	GetClipPlaneIndex,
	SetRenderStateIndex,
	GetRenderStateIndex,
	CreateStateBlockIndex,
	BeginStateBlockIndex,
	EndStateBlockIndex,
	SetClipStatusIndex,
	GetClipStatusIndex,
	GetTextureIndex,
	SetTextureIndex,
	GetTextureStageStateIndex,
	SetTextureStageStateIndex,
	GetSamplerStateIndex,
	SetSamplerStateIndex,
	ValidateDeviceIndex,
	SetPaletteEntriesIndex,
	GetPaletteEntriesIndex,
	SetCurrentTexturePaletteIndex,
	GetCurrentTexturePaletteIndex,
	SetScissorRectIndex,
	GetScissorRectIndex,
	SetSoftwareVertexProcessingIndex,
	GetSoftwareVertexProcessingIndex,
	SetNPatchModeIndex,
	GetNPatchModeIndex,
	DrawPrimitiveIndex,
	DrawIndexedPrimitiveIndex,
	DrawPrimitiveUPIndex,
	DrawIndexedPrimitiveUPIndex,
	ProcessVerticesIndex,
	CreateVertexDeclarationIndex,
	SetVertexDeclarationIndex,
	GetVertexDeclarationIndex,
	SetFVFIndex,
	GetFVFIndex,
	CreateVertexShaderIndex,
	SetVertexShaderIndex,
	GetVertexShaderIndex,
	SetVertexShaderConstantFIndex,
	GetVertexShaderConstantFIndex,
	SetVertexShaderConstantIIndex,
	GetVertexShaderConstantIIndex,
	SetVertexShaderConstantBIndex,
	GetVertexShaderConstantBIndex,
	SetStreamSourceIndex,
	GetStreamSourceIndex,
	SetStreamSourceFreqIndex,
	GetStreamSourceFreqIndex,
	SetIndicesIndex,
	GetIndicesIndex,
	CreatePixelShaderIndex,
	SetPixelShaderIndex,
	GetPixelShaderIndex,
	SetPixelShaderConstantFIndex,
	GetPixelShaderConstantFIndex,
	SetPixelShaderConstantIIndex,
	GetPixelShaderConstantIIndex,
	SetPixelShaderConstantBIndex,
	GetPixelShaderConstantBIndex,
	DrawRectPatchIndex,
	DrawTriPatchIndex,
	DeletePatchIndex,
	CreateQuery
};

//Example below
/*
 *
 * Example:
HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9 * pDevice)
{
	// Do directx things here..
	D3DRECT BarRect = { 100, 100, 200, 200 };
	pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(1, 1, 1, 1), 0.0f, 0);
	
	// std::cout << "[+] Execution of hooked function successfull.\n";

	return oEndScene(pDevice);
}


DWORD WINAPI MainThread(LPVOID hModule)
{
	std::cout << "[*] Started new thread\n";

	// Thanks to Broihon
	// From https://guidedhacking.com/showthread.php?9937-Getting-direct3d-device&p=54138&highlight=#post54138
	void * d3d9Device[119];

	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		std::cout << "[+] Found DirectXDevice vTable at: ";
		std::cout << std::hex << d3d9Device[0] << "\n";

		std::cout << "[+] Trying to hook function..\n";

		// Using detours
		// The following line is NOT needed if we already have a ** void (aka *void [])
		// void ** pVTable = *reinterpret_cast<void***>(DirectXDevice);

		oEndScene = (f_EndScene)DetourFunction((PBYTE)d3d9Device[42], (PBYTE)Hooked_EndScene);
	}
	return false;
}
 */

// From https://guidedhacking.com/threads/getting-direct3d-device.9937/
#ifdef D3D11
bool GetD3D11SwapchainDeviceContext(void ** pSwapchainTable, size_t Size_Swapchain, void ** pDeviceTable, size_t Size_Device, void ** pContextTable, size_t Size_Context)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferCount					= 1;
	swapChainDesc.BufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage					= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow					= GetForegroundWindow();
	swapChainDesc.SampleDesc.Count				= 1;
	swapChainDesc.Windowed						= ((GetWindowLong(swapChainDesc.OutputWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect					= DXGI_SWAP_EFFECT_DISCARD;
	
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain		* pDummySwapChain	= nullptr;
	ID3D11Device		* pDummyDevice		= nullptr;
	ID3D11DeviceContext * pDummyContext		= nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, NULL, &pDummyContext)))
		return false;
	
	if (pSwapchainTable)
	{
		memcpy(pSwapchainTable, *reinterpret_cast<void***>(pDummySwapChain), Size_Swapchain);
	}
	
	if (pDeviceTable)
	{
		memcpy(pDeviceTable, *reinterpret_cast<void***>(pDummyDevice), Size_Device);
	}
	
	if (pContextTable)
	{
		memcpy(pContextTable, *reinterpret_cast<void***>(pDummyContext), Size_Context);
	}

	pDummySwapChain->Release();
	pDummyDevice->Release();
	pDummyContext->Release();

	return true;
}

void * SwapChain[18];
void * Device[40];
void * Context[108];
#endif

// if (GetD3D11SwapchainDeviceContext(SwapChain, sizeof(SwapChain), Device, sizeof(Device), Context, sizeof(Context)))
// {
// 	//hook stuff using the dumped addresses
// }

// From https://guidedhacking.com/showthread.php?9937-Getting-direct3d-device&p=54138&highlight=#post54138
#ifdef D3D9
bool GetD3D9Device(void ** pTable, size_t Size, HWND windowHandle)
{	
	if (!pTable)
	{
		OutputDebugStringA("[-] pTable not set.\n");
		return false;
	}

	IDirect3D9 * pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
	{
		OutputDebugStringA("[-] Direct3DCreate9 failed.\n");
		return false;
	}
	OutputDebugStringA("[+] Direct3DCreate9 successfull.\n");

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = windowHandle;
	d3dpp.Windowed = ((GetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;

	OutputDebugStringA("[*] Got here..\n");

	IDirect3DDevice9 * pDummyDevice = nullptr;
	HRESULT create_device_ret = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
	OutputDebugStringA("[*] Return of CreateDevice:\n");
	OutputDebugStringA(std::to_string(create_device_ret).c_str());
	OutputDebugStringA("\n");
	if (!pDummyDevice || FAILED(create_device_ret))
	{
		OutputDebugStringA("[-] CreateDevice failed\n");
		
		pD3D->Release();
		return false;
	}
	OutputDebugStringA("[+] CreateDevice successfull.\n");

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

	pDummyDevice->Release();
	pD3D->Release();

	OutputDebugStringA("[+] Success!\n");
	return true;
}
// void * d3d9Device[119];

// if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
// {
// 	//hook stuff using the dumped addresses
// }


//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;     // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = 80;    // set the width of the buffer
	d3dpp.BackBufferHeight = 60;    // set the height of the buffer

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	//D3DXCreateFont(d3ddev, 50, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);

}

// From nuklear - d3d9-demo
void create_d3d9_device(HWND wnd)
{
	HRESULT hr;

	present.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	present.BackBufferWidth = WINDOW_WIDTH;
	present.BackBufferHeight = WINDOW_HEIGHT;
	present.BackBufferFormat = D3DFMT_X8R8G8B8;
	present.BackBufferCount = 1;
	present.MultiSampleType = D3DMULTISAMPLE_NONE;
	present.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present.hDeviceWindow = wnd;
	present.EnableAutoDepthStencil = TRUE;
	present.AutoDepthStencilFormat = D3DFMT_D24S8;
	present.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	present.Windowed = TRUE;

	{/* first try to create Direct3D9Ex device if possible (on Windows 7+) */
		typedef HRESULT WINAPI Direct3DCreate9ExPtr(UINT, IDirect3D9Ex**);
		Direct3DCreate9ExPtr *Direct3DCreate9Ex = (Direct3DCreate9ExPtr *)GetProcAddress(GetModuleHandleA("d3d9"), "Direct3DCreate9Ex");
		if (Direct3DCreate9Ex) {
			IDirect3D9Ex *d3d9ex;
			if (SUCCEEDED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9ex))) {
				hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
					&present, NULL, &deviceEx);
				if (SUCCEEDED(hr)) {
					device = (IDirect3DDevice9 *)deviceEx;
					memcpy(d3d9Device, *reinterpret_cast<void***>(device), sizeof(d3d9Device));
					OutputDebugStringA("Successfully received D3DDevice via Direct3DCreate9Ex!");
				}
				else {
					/* hardware vertex processing not supported, no big deal
					retry with software vertex processing */
					hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
						&present, NULL, &deviceEx);
					if (SUCCEEDED(hr)) {
						device = (IDirect3DDevice9 *)deviceEx;
						memcpy(d3d9Device, *reinterpret_cast<void***>(device), sizeof(d3d9Device));
						OutputDebugStringA("Successfully received D3DDevice via Direct3DCreate9Ex!");
					}
				}
				IDirect3D9Ex_Release(d3d9ex);
			}
		}
	}

	if (!device) {
		/* otherwise do regular D3D9 setup */
		IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
			&present, &device);
		if (FAILED(hr)) {
			/* hardware vertex processing not supported, no big deal
			retry with software vertex processing */
			hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
				&present, &device);
			NK_ASSERT(SUCCEEDED(hr));
			if (device != nullptr)
			{
				memcpy(d3d9Device, *reinterpret_cast<void***>(device), sizeof(d3d9Device));
				OutputDebugStringA("Successfully received D3DDevice!");
			}
				
		}
		IDirect3D9_Release(d3d9);
	}
}

void GetD3D9Addresses()
{
	while (device == NULL) {
		create_d3d9_device(GetForegroundWindow());
	}
	////Do this in a function or whatever
	//HMODULE hDLL = GetModuleHandleA("d3d9");
	//LPDIRECT3D9(__stdcall*pDirect3DCreate9)(UINT) = (LPDIRECT3D9(__stdcall*)(UINT))GetProcAddress(hDLL, "Direct3DCreate9");

	//LPDIRECT3D9 pD3D = pDirect3DCreate9(D3D_SDK_VERSION);

	//D3DDISPLAYMODE d3ddm;
	//HRESULT hRes = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	//D3DPRESENT_PARAMETERS d3dpp;
	//ZeroMemory(&d3dpp, sizeof(d3dpp));
	//d3dpp.Windowed = true;
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.BackBufferFormat = d3ddm.Format;
	//

	//WNDCLASSEX wc;
	//ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//wc.cbSize = sizeof(WNDCLASSEX);
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	////wc.lpfnWndProc = WindowProc;
	//wc.hInstance = hInstance;
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	//wc.lpszClassName = TEXT("WindowClass");

	//RegisterClassEx(&wc);
	//HWND hWnd = CreateWindow(TEXT("1"), NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	//hRes = pD3D->CreateDevice(
	//	D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	hWnd,
	//	D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
	//	&d3dpp, &device);

	//pD3D->Release();
	//DestroyWindow(hWnd);

	//if (pD3D == NULL) {
	//	//printf ("WARNING: D3D FAILED");
	//	return false;
	//}
	auto pInterface = (unsigned long*)*((unsigned long*)device);

	oPresentFunction = (f_PresentFn)(DWORD)pInterface[PresentIndex];
	oResetFunction = (f_resetFn)(DWORD)pInterface[ResetIndex];
	oEndScene = (f_EndScene)(DWORD)pInterface[EndSceneIndex];
}
#endif

//static LRESULT CALLBACK
//WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//
//	case WM_SIZE:
//		if (device)
//		{
//			UINT width = LOWORD(lparam);
//			UINT height = HIWORD(lparam);
//			if (width != 0 && height != 0 &&
//				(width != present.BackBufferWidth || height != present.BackBufferHeight))
//			{
//				nk_d3d9_release();
//				present.BackBufferWidth = width;
//				present.BackBufferHeight = height;
//				HRESULT hr = IDirect3DDevice9_Reset(device, &present);
//				NK_ASSERT(SUCCEEDED(hr));
//				nk_d3d9_resize(width, height);
//			}
//		}
//		break;
//	}
//
//	if (nk_d3d9_handle_event(wnd, msg, wparam, lparam))
//		return 0;
//
//	return DefWindowProcW(wnd, msg, wparam, lparam);
//}

namespace MyD3D9 {

	/*Example Hook:
	while (!MyD3D9::GetD3D9Device())
	{
		std::cout << "Failed to get D3D9 Device! Trying again!\n";
		Sleep(200);
	}

	std::cout << "Found/Created D3D9 Device!\n";
	std::cout << "VTable is at: " << std::hex << &MyD3D9::d3d9Device << "\n";
	std::cout << std::hex << &MyD3D9::d3d9Device[MyDX3DDeviceIndexes::EndSceneIndex] << "\n";

	getchar();

	MH_Initialize();
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::EndSceneIndex], &Hooked_EndScene, &oEndScene);
	MH_QueueEnableHook(MH_ALL_HOOKS);
	MH_ApplyQueued();
	
	*/

	void * d3d9Device[119];
	bool GetD3D9Device()
	{
		if (!d3d9Device)
			return false;

		IDirect3D9 * pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (!pD3D)
			return false;

		D3DPRESENT_PARAMETERS d3dpp = { 0 };
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetForegroundWindow();
		d3dpp.Windowed = ((GetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;;

		IDirect3DDevice9 * pDummyDevice = nullptr;
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice)))
		{
			pD3D->Release();
			return false;
		}

		memcpy(d3d9Device, *reinterpret_cast<void***>(pDummyDevice), sizeof(d3d9Device));

		pDummyDevice->Release();
		pD3D->Release();

		return true;
	}
}