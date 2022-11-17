
#pragma comment(lib, "d3d11.lib")

#include <Detours.h>
#include <d3d11.h>

ID3D11Device*        g_Device;
ID3D11DeviceContext* g_DeviceContext;
IDXGISwapChain*      g_SwapChain;

uintptr_t g_ModuleBase;
HMODULE   g_DllD3D11;

decltype(&D3D11CreateDeviceAndSwapChain)   ptrD3D11CreateDeviceAndSwapChain;


HRESULT WINAPI hk_D3D11CreateDeviceAndSwapChain(
	IDXGIAdapter*               pAdapter,
	D3D_DRIVER_TYPE             DriverType,
	HMODULE                     Software,
	UINT                        Flags,
	const D3D_FEATURE_LEVEL*    pFeatureLevels,
	UINT                        FeatureLevels,
	UINT                        SDKVersion,
	const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	IDXGISwapChain**            ppSwapChain,
	ID3D11Device**              ppDevice,
	D3D_FEATURE_LEVEL*          pFeatureLevel,
	ID3D11DeviceContext**       ppImmediateContext)
{
	logger::info("Calling original D3D11CreateDeviceAndSwapChain");
	HRESULT hr = (*ptrD3D11CreateDeviceAndSwapChain)(pAdapter,
		DriverType,
		Software,
		Flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		pSwapChainDesc,
		ppSwapChain,
		ppDevice,
		pFeatureLevel,
		ppImmediateContext);

	logger::info("Storing render device information");
	g_Device = *ppDevice;
	g_DeviceContext = *ppImmediateContext;
	g_SwapChain = *ppSwapChain;

	return hr;
}

struct Hooks
{

#define PatchIAT(detour, module, procname) Detours::IATHook(g_ModuleBase, (module), (procname), (uintptr_t)(detour));

	static void Install()
	{
		g_ModuleBase = (uintptr_t)GetModuleHandle(nullptr);
		g_DllD3D11 = GetModuleHandleA("d3d11.dll");
		if (g_DllD3D11) {
			*(FARPROC*)&ptrD3D11CreateDeviceAndSwapChain = GetProcAddress(g_DllD3D11, "D3D11CreateDeviceAndSwapChain");
			PatchIAT(hk_D3D11CreateDeviceAndSwapChain, "d3d11.dll", "D3D11CreateDeviceAndSwapChain");
			logger::info("Patched d3d11.dll address table");
		} else {
			logger::critical("Failed to patch d3d11.dll address table");
		}
		logger::info("Installed render startup hook");
	}
};

void PatchD3D11()
{
	Hooks::Install();
}
