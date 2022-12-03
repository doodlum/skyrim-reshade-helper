#pragma once

#include "RE/BSGraphicsTypes.h"
#include <unordered_set>
#include <shared_mutex>

class CustomRender
{
public:

	std::shared_mutex lock;
	std::unordered_set<std::string> customTechniques;

	std::unordered_set<std::string> GetCustomTechniques();
	void                            SetCustomTechniques(std::unordered_set<std::string> techniques);
	void							RenderCustomEffects();
	void                            DisplayTechniqueSelection();
};

