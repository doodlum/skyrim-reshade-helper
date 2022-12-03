#include "CustomRender.h"
#include "Effects.h"
#include "PreFog.h"

void PreFog::on_bind_render_targets_and_depth_stencil(reshade::api::command_list* cmd_list, uint32_t, const reshade::api::resource_view* rtv, reshade::api::resource_view)
{
	if (PreFog::GetSingleton()->flag == 1) {
		std::unordered_set<std::string> techniquesToRender;
		techniquesToRender.insert("Bloom");
		if (techniquesToRender.size() > 0) {
			Effects::GetSingleton()->m_runtime->enumerate_techniques(nullptr, [&](reshade::api::effect_runtime* runtime, reshade::api::effect_technique technique) {
				char buffer[256];
				runtime->get_technique_name(technique, buffer);
				std::string name = buffer;
				//logger::info("{}", name);
				if (techniquesToRender.contains(name)) {
					runtime->render_technique(technique, cmd_list, rtv[0]);
				}
			});
		}

		PreFog::GetSingleton()->flag = 0;
	}
}
