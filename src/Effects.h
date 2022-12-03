#pragma once

#include "RE/BSGraphicsTypes.h"
#include <unordered_set>
#include "CustomRender.h"

class Effects
{
public:
	static Effects* GetSingleton()
	{
		static Effects effects;
		return &effects;
	}

	static void InstallHooks()
	{
		Hooks::Install();
	}

	bool created = false;
	
	reshade::api::effect_runtime* m_runtime = nullptr;
	reshade::api::command_list*   m_cmdlist = nullptr;
	reshade::api::resource_view   m_rtv;
	reshade::api::resource_view   m_rtv_srgb;
	
	std::vector<std::string>        allTechniques;

	std::vector<CustomRender*>        customRenders;

	void InitialiseCustomRenders();

	std::vector<std::string> GetAllTechniques();
	void                     SetAllTechniques(std::vector<std::string> techniques);

	void OnReShadeReloadedEffects(reshade::api::effect_runtime* runtime);
	static void OnReShadeReloadedEffectsCallback(reshade::api::effect_runtime* runtime);

	void OnBeginEffects(reshade::api::effect_runtime* runtime, reshade::api::command_list* cmd_list, reshade::api::resource_view rtv, reshade::api::resource_view rtv_srgb);
	static void OnBeginEffectsCallback(reshade::api::effect_runtime* runtime, reshade::api::command_list* cmd_list, reshade::api::resource_view rtv, reshade::api::resource_view rtv_srgb);

	void        UpdateEffectRuntime(reshade::api::effect_runtime* runtime);

	void MainDraw();

protected:
	struct Hooks
	{
		struct Main_DrawWorld_MainDraw
		{
			static void thunk(INT64 BSGraphics_Renderer, int unk)
			{
				func(BSGraphics_Renderer, unk);
				GetSingleton()->MainDraw();
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			stl::write_thunk_call<Hooks::Main_DrawWorld_MainDraw>(REL::RelocationID(79947, 82084).address() + REL::Relocate(0x16F, 0x17A));  // EBF510 (EBF67F), F05BF0 (F05D6A)
		}
	};
};

