
#include "RE/BSGraphicsTypes.h"

#include "Effects.h"
#include "Menu.h"
#include "PreFog.h"

using namespace reshade::api;

HMODULE m_hModule;

extern "C" __declspec(dllexport) const char* NAME = "SSE ReShade Helper";
extern "C" __declspec(dllexport) const char* DESCRIPTION = "Renders ReShade effects before the UI, by PureDark and doodlez";

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH) m_hModule = hModule;
	return TRUE;
}

void RegisterAddonEvents()
{
	reshade::register_event<reshade::addon_event::reshade_begin_effects>(Effects::OnBeginEffectsCallback);
	reshade::register_event<reshade::addon_event::reshade_reloaded_effects>(Effects::OnReShadeReloadedEffectsCallback);
	reshade::register_event<reshade::addon_event::bind_render_targets_and_depth_stencil>(PreFog::on_bind_render_targets_and_depth_stencil);
	reshade::register_overlay("Debug", Menu::DrawDebugOverlayCallback);
	reshade::register_overlay(nullptr, Menu::DrawSettingsOverlayCallback);
}

void Load()
{
	if (reshade::register_addon(m_hModule)) {
		logger::info("Registered addon");
		RegisterAddonEvents();
		// Pre-UI Hook for rendering effects
		Effects::InstallHooks();
		Effects::GetSingleton()->InitialiseCustomRenders();
		logger::info("Installed render hook");
	} else {
		logger::info("ReShade not present, not installing hook");
	}
}


//#include <DirectXMath.h>
//
//struct Cinematic
//{
//	DirectX::XMFLOAT4 Params01[5];
//};
//
//struct CinematicFade
//{
//	DirectX::XMFLOAT4 Params01[6];
//};
//
//CinematicFade reshade_params;
//
//int         flag = 0;
//static void on_map_buffer_region(device* device, resource resource, uint64_t offset, uint64_t size, map_access access, void** data)
//{
//	if (flag == 1) {
//		if (size == sizeof(Cinematic)) {
//			Cinematic& game = (Cinematic)&data;
//			flag = 0;
//		}
//	} else if (flag == 2) {
//		
//		flag = 0;
//	}

//}

//static void on_unmap_buffer_region(device* device, resource resource)
//{
//}

