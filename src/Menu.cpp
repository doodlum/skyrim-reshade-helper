#include "Menu.h"
#include <unordered_set>
#include "Effects.h"

ImVec2 GetTextureDimensions(ID3D11ShaderResourceView* view)
{
	ID3D11Resource* res = nullptr;
	view->GetResource(&res);

	ID3D11Texture2D* texture2d = nullptr;
	HRESULT          hr = res->QueryInterface(&texture2d);

	ImVec2 dim(512, 512);
	if (SUCCEEDED(hr)) {
		D3D11_TEXTURE2D_DESC desc;
		texture2d->GetDesc(&desc);
		dim.x = static_cast<float>(desc.Width);
		dim.y = static_cast<float>(desc.Height);
	}
	float biggest = max(dim.x, dim.y);
	if (biggest > 512) {
		dim.x *= 512 / biggest;
		dim.y *= 512 / biggest;
	}
	return dim;
}


void Menu::DrawRenderTargets()
{
	ImGuiStyle& style = ImGui::GetStyle();
	float       w = ImGui::CalcItemWidth();
	float       spacing = style.ItemInnerSpacing.x;
	float       button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	static const char* current_item = NULL;
	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
		for (uint32_t n = 0; n < IM_ARRAYSIZE(RTNames); n++) {
			bool is_selected = (current_item == RTNames[n]);
			if (ImGui::Selectable(RTNames[n], is_selected)) {
				current_item = RTNames[n];
				selectedRenderTarget = n;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Left)) {
	}
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
	}
	ImGui::SameLine(0, style.ItemInnerSpacing.x);
	ImGui::Text("Render Target");

	if (auto r = BSGraphics::Renderer::QInstance()) {
		auto rt = r->pRenderTargets[selectedRenderTarget];
		if (rt.SRV) {
			ImVec2 vec = GetTextureDimensions(rt.SRV);
			ImGui::Image((void*)rt.SRV, vec);
		} else if (rt.SRVCopy) {
			ImVec2 vec = GetTextureDimensions(rt.SRVCopy);
			ImGui::Image((void*)rt.SRVCopy, vec);
		}
	}
}


void Menu::DrawDepth()
{
	ImGuiStyle& style = ImGui::GetStyle();
	float       w = ImGui::CalcItemWidth();
	float       spacing = style.ItemInnerSpacing.x;
	float       button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	static const char* current_item = NULL;
	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
		for (uint32_t n = 0; n < IM_ARRAYSIZE(DepthNames); n++) {
			bool is_selected = (current_item == DepthNames[n]);
			if (ImGui::Selectable(DepthNames[n], is_selected)) {
				current_item = DepthNames[n];
				selectedDepth = n;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Left)) {
	}
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
	}
	ImGui::SameLine(0, style.ItemInnerSpacing.x);
	ImGui::Text("Render Target Depth Stencil");

	if (auto r = BSGraphics::Renderer::QInstance()) {
		auto rt = r->pDepthStencils[selectedDepth];
		if (rt.DepthSRV) {
			ImVec2 vec = GetTextureDimensions(rt.DepthSRV);
			ImGui::Image((void*)rt.DepthSRV, vec);
		}
	}
}


void Menu::DrawStencil()
{
	ImGuiStyle& style = ImGui::GetStyle();
	float       w = ImGui::CalcItemWidth();
	float       spacing = style.ItemInnerSpacing.x;
	float       button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	static const char* current_item = NULL;
	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
		for (uint32_t n = 0; n < IM_ARRAYSIZE(DepthNames); n++) {
			bool is_selected = (current_item == DepthNames[n]);
			if (ImGui::Selectable(DepthNames[n], is_selected)) {
				current_item = DepthNames[n];
				selectedStencil = n;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Left)) {
	}
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
	}
	ImGui::SameLine(0, style.ItemInnerSpacing.x);
	ImGui::Text("Render Target Depth Stencil");

	if (auto r = BSGraphics::Renderer::QInstance()) {
		auto rt = r->pDepthStencils[selectedStencil];
		if (rt.StencilSRV) {
			ImVec2 vec = GetTextureDimensions(rt.StencilSRV);
			ImGui::Image((void*)rt.StencilSRV, vec);
		}
	}
}

void Menu::DrawDebugOverlay(reshade::api::effect_runtime*)
{
	if (g_DebugMenuVisible) {
		ImGui::Begin("SSE ReShade Helper - Debug Menu", &g_DebugMenuVisible);
		GetSingleton()->DrawRenderTargets();
		GetSingleton()->DrawDepth();
		GetSingleton()->DrawStencil();
		ImGui::End();
	}
}

void Menu::DrawSettingsOverlay(reshade::api::effect_runtime*)
{
	ImGui::Checkbox("Debug Menu", &GetSingleton()->g_DebugMenuVisible);
	GetSingleton()->DisplayTechniqueSelection();
}

void Menu::DrawDebugOverlayCallback(reshade::api::effect_runtime* runtime)
{
	GetSingleton()->DrawDebugOverlay(runtime);
}

void Menu::DrawSettingsOverlayCallback(reshade::api::effect_runtime* runtime)
{
	GetSingleton()->DrawSettingsOverlay(runtime);
}


void Menu::DisplayTechniqueSelection()
{
	auto effects = Effects::GetSingleton();
	for (auto render : effects->customRenders) {
		const uint32_t columns = 2;
		auto           techniques = effects->GetAllTechniques();
		auto           curTechniques = render->GetCustomTechniques();
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
			render->SetCustomTechniques(newTechniques);
		}
		ImGui::End();
	}
}
