
#include <reshade/reshade.hpp>

using namespace reshade::api;

HMODULE m_hModule = nullptr;

static effect_runtime* m_runtime = nullptr;
static command_list*   m_cmdlist = nullptr;
static resource_view   m_rtv;
static resource_view   m_rtv_srgb;

struct __declspec(uuid("7251932A-ADAF-4DFC-B5CB-9A4E8CD5D6EB")) device_data
{
	effect_runtime* main_runtime = nullptr;
};

static void on_reshade_begin_effects(effect_runtime* runtime, command_list* cmd_list, resource_view rtv, resource_view rtv_srgb)
{
	m_runtime = runtime;
	m_cmdlist = cmd_list;
	m_rtv = rtv;
	m_rtv_srgb = rtv_srgb;
}
int         index = 0;
resource_view backbuffer;

static void on_init_resource_view([[maybe_unused]] device* device, [[maybe_unused]] resource resource, [[maybe_unused]] resource_usage usage_type, [[maybe_unused]] const resource_view_desc& desc, [[maybe_unused]] resource_view view)
{
	if (index == 1) {
		backbuffer = view;
	}
	index++;
}


void register_addon_events()
{
	reshade::register_event<reshade::addon_event::reshade_begin_effects>(on_reshade_begin_effects);
	reshade::register_event<reshade::addon_event::init_resource_view>(on_init_resource_view);
}

void unregister_addon_events()
{
	reshade::unregister_event<reshade::addon_event::reshade_begin_effects>(on_reshade_begin_effects);
	reshade::unregister_event<reshade::addon_event::init_resource_view>(on_init_resource_view);
}

extern "C" __declspec(dllexport) const char* NAME = "FO4 ReShade Helper";
extern "C" __declspec(dllexport) const char* DESCRIPTION = "Renders ReShade effects before the UI, by PureDark and doodlez";

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		m_hModule = hModule;
		break;
	case DLL_PROCESS_DETACH:
		unregister_addon_events();
		reshade::unregister_addon(hModule);
		break;
	}

	return TRUE;
}

struct Hooks
{
	struct Main_DrawWorld_MainDraw
	{
		static void thunk(INT64 BSGraphics_Renderer, int unk)
		{
			func(BSGraphics_Renderer, unk);
			if (m_runtime) {
				m_runtime->render_effects(m_cmdlist, backbuffer, backbuffer);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};
};

void PatchD3D11();

void Load()
{
	if (reshade::register_addon(m_hModule)) {
		logger::info("Registered addon");
		register_addon_events();
		// Pre-UI Hook for rendering effects
		stl::write_thunk_call<Hooks::Main_DrawWorld_MainDraw>(REL::ID(408683).address() + 0xDA);
		logger::info("Installed render hook");
	} else {
		logger::info("ReShade not present, not installing hook");
	}
}
