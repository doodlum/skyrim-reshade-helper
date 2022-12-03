#include "CustomRender.h"
#include "Effects.h"

std::unordered_set<std::string> CustomRender::GetCustomTechniques()
{
	std::lock_guard<std::shared_mutex> lk{ lock };
	return customTechniques;
}

void CustomRender::SetCustomTechniques(std::unordered_set<std::string> techniques)
{
	std::lock_guard<std::shared_mutex> lk{ lock };
	customTechniques = techniques;
}

void CustomRender::RenderCustomEffects()
{
	std::unordered_set<std::string> techniquesToRender;
	techniquesToRender.insert("ASCII");
	if (auto r = BSGraphics::Renderer::QInstance()) {
		auto rt = r->pRenderTargets[RENDER_TARGET_MAIN];
		if (rt.RTV) {
			if (techniquesToRender.size() > 0) {
				Effects::GetSingleton()->m_runtime->enumerate_techniques(nullptr, [&](reshade::api::effect_runtime* runtime, reshade::api::effect_technique technique) {
					char buffer[256];
					runtime->get_technique_name(technique, buffer);
					std::string name = buffer;
					//logger::info("{}", name);
					if (techniquesToRender.contains(name)) {
						reshade::api::resource_view shader_resource_view = { reinterpret_cast<uintptr_t>(rt.SRV) };

						runtime->render_technique(technique, runtime->get_command_queue()->get_immediate_command_list(), shader_resource_view);
					}
				});
			}
		}
	}
}



void CustomRender::DisplayTechniqueSelection()
{
	const uint32_t columns = 2;
	auto           techniques = Effects::GetSingleton()->GetAllTechniques();
	auto           curTechniques = GetCustomTechniques();
	;
	std::unordered_set<std::string> newTechniques;
	static char                     searchBuf[256] = "\0";

	ImGui::SetNextWindowSize({ 500, 300 }, ImGuiCond_Once);
	bool wndOpen = true;
	if (ImGui::Begin("Technique selection", &wndOpen)) {
		if (ImGui::BeginChild("Technique selection##child", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::Button("Disable all")) {
				curTechniques.clear();
			}

			ImGui::SameLine();
			ImGui::Text("Search: ");
			ImGui::SameLine();
			ImGui::InputText("", searchBuf, 256, ImGuiInputTextFlags_None);

			ImGui::Separator();

			if (ImGui::BeginTable("Technique selection##table", columns, ImGuiTableFlags_Resizable)) {
				std::string prefix(searchBuf);
				for (int i = 0; i < techniques.size(); i++) {
					bool enabled = curTechniques.find(techniques.at(i)) != curTechniques.end();
					if (techniques.at(i).rfind(prefix, 0) == 0) {
						ImGui::TableNextColumn();
						ImGui::Checkbox(techniques.at(i).c_str(), &enabled);
					}
					if (enabled) {
						newTechniques.insert(techniques.at(i));
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();
		SetCustomTechniques(curTechniques);
	}
	ImGui::End();
}
