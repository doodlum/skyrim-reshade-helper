#pragma once

#include "RE/BSGraphicsTypes.h"

class Menu
{
public:
	static Menu* GetSingleton()
	{
		static Menu menu;
		return &menu;
	}

	bool g_DebugMenuVisible = false;

	uint32_t selectedRenderTarget = 0;
	uint32_t selectedDepth = 0;
	uint32_t selectedStencil = 0;

	void DrawRenderTargets();
	void DrawDepth();
	void DrawStencil();

	void DrawDebugOverlay(reshade::api::effect_runtime*);
	void DrawSettingsOverlay(reshade::api::effect_runtime*);

	static void DrawDebugOverlayCallback(reshade::api::effect_runtime*);
	static void DrawSettingsOverlayCallback(reshade::api::effect_runtime*);
	void        DisplayTechniqueSelection();
};

