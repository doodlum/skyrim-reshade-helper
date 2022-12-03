#include "Effects.h"

#include "PreFog.h"

void Effects::InitialiseCustomRenders()
{
	PreFog::InstallHooks();
	customRenders.push_back(PreFog::GetSingleton());
}

std::vector<std::string> Effects::GetAllTechniques()
{
	return allTechniques;
}

void Effects::SetAllTechniques(std::vector<std::string> techniques)
{
	allTechniques = techniques;
}

void Effects::OnReShadeReloadedEffects(reshade::api::effect_runtime* runtime)
{
	allTechniques.clear();

	runtime->enumerate_techniques(nullptr, [&](reshade::api::effect_runtime* runtime, reshade::api::effect_technique technique) {
		char buffer[256];
		runtime->get_technique_name(technique, buffer);
		std::string name = buffer;
		logger::info("{}", name);
		allTechniques.push_back(name);
		//	bool enabled = runtime->get_technique_state(technique);
	});
}

void Effects::OnReShadeReloadedEffectsCallback(reshade::api::effect_runtime* runtime)
{
	GetSingleton()->OnReShadeReloadedEffects(runtime);
}

void Effects::OnBeginEffects(reshade::api::effect_runtime* runtime, reshade::api::command_list* cmd_list, reshade::api::resource_view rtv, reshade::api::resource_view rtv_srgb)
{
	m_runtime = runtime;
	m_cmdlist = cmd_list;
	m_rtv = rtv;
	m_rtv_srgb = rtv_srgb;
}

void Effects::OnBeginEffectsCallback(reshade::api::effect_runtime* runtime, reshade::api::command_list* cmd_list, reshade::api::resource_view rtv, reshade::api::resource_view rtv_srgb)
{
	GetSingleton()->OnBeginEffects(runtime, cmd_list, rtv, rtv_srgb);
}

void Effects::UpdateEffectRuntime(reshade::api::effect_runtime* runtime)
{
	if (!created) {
		if (auto r = BSGraphics::Renderer::QInstance()) {
			for (uint32_t n = 0; n < IM_ARRAYSIZE(RTNames); n++) {
				auto rt = r->pRenderTargets[n];
				if (rt.SRV) {
					reshade::api::resource_view shader_resource_view = { reinterpret_cast<uintptr_t>(rt.SRV) };
					runtime->update_texture_bindings(RTNames[n], shader_resource_view, reshade::api::resource_view{ 0 });
				} else {
					logger::info("{} is invalid", RTNames[n]);
				}
			}
			for (uint32_t n = 0; n < IM_ARRAYSIZE(RTCubemapNames); n++) {
				auto rt = r->pCubemapRenderTargets[n];
				if (rt.SRV) {
					reshade::api::resource_view shader_resource_view = { reinterpret_cast<uintptr_t>(rt.SRV) };
					runtime->update_texture_bindings(RTCubemapNames[n], shader_resource_view, reshade::api::resource_view{ 0 });
				} else {
					logger::info("{} is invalid", RTCubemapNames[n]);
				}
			}
			for (uint32_t n = 0; n < IM_ARRAYSIZE(DepthNames); n++) {
				auto rt = r->pDepthStencils[n];
				if (rt.DepthSRV) {
					reshade::api::resource_view shader_resource_view = { reinterpret_cast<uintptr_t>(rt.DepthSRV) };
					runtime->update_texture_bindings(DepthNames[n], shader_resource_view, reshade::api::resource_view{ 0 });
				} else {
					logger::info("{} is invalid", DepthNames[n]);
				}
				if (rt.StencilSRV) {
					reshade::api::resource_view shader_resource_view = { reinterpret_cast<uintptr_t>(rt.StencilSRV) };
					runtime->update_texture_bindings(StencilNames[n], shader_resource_view, reshade::api::resource_view{ 0 });
				} else {
					logger::info("{} is invalid", StencilNames[n]);
				}
			}
			created = true;
		}
	}
}

void Effects::MainDraw()
{
	if (m_runtime) {
		UpdateEffectRuntime(m_runtime);
	//	m_runtime->render_effects(m_cmdlist, m_rtv, m_rtv_srgb);
	}
}
