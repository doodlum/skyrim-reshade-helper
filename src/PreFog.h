#pragma once

#include "CustomRender.h"

class PreFog : public CustomRender
{
public:
	static PreFog* GetSingleton()
	{
		static PreFog render;
		return &render;
	}

	static void InstallHooks()
	{
		Hooks::Install();
	}

	const char* name = "PreFog";

	int flag = 0;
	static void on_bind_render_targets_and_depth_stencil(reshade::api::command_list* cmd_list, uint32_t count, const reshade::api::resource_view* rtvs, reshade::api::resource_view);

protected:
	struct Hooks
	{
		struct BSImagespaceShaderISSAOCompositeSAO_SetupTechnique
		{
			static void thunk(RE::BSShader* a_shader, RE::BSShaderMaterial* a_material)
			{
			//	GetSingleton()->RenderCustomEffects();
				GetSingleton()->flag = 1;
				func(a_shader, a_material);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct BSImagespaceShaderISSAOCompositeFog_SetupTechnique
		{
			static void thunk(RE::BSShader* a_shader, RE::BSShaderMaterial* a_material)
			{
				//GetSingleton()->RenderCustomEffects();
				GetSingleton()->flag = 1;
				func(a_shader, a_material);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct BSImagespaceShaderISSAOCompositeSAOFog_SetupTechnique
		{
			static void thunk(RE::BSShader* a_shader, RE::BSShaderMaterial* a_material)
			{
				//GetSingleton()->RenderCustomEffects();
				GetSingleton()->flag = 1;
				func(a_shader, a_material);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			stl::write_vfunc<0x2, BSImagespaceShaderISSAOCompositeSAO_SetupTechnique>(RE::VTABLE_BSImagespaceShaderISSAOCompositeSAO[0]);
			stl::write_vfunc<0x2, BSImagespaceShaderISSAOCompositeFog_SetupTechnique>(RE::VTABLE_BSImagespaceShaderISSAOCompositeFog[0]);
			stl::write_vfunc<0x2, BSImagespaceShaderISSAOCompositeSAOFog_SetupTechnique>(RE::VTABLE_BSImagespaceShaderISSAOCompositeSAOFog[0]);
			logger::info("Installed pre-fog rendering hook");
		}
	};
};

