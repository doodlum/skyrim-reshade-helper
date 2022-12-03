
#include "BSGraphicsTypes.h"

BSGraphics::RendererShadowState* BSGraphics::RendererShadowState::QInstance()
{
	static BSGraphics::RendererShadowState* g_RendererShadowState = (RendererShadowState*)RELOCATION_ID(524773, 388819).address();
	return g_RendererShadowState;
}

BSGraphics::Renderer* BSGraphics::Renderer::QInstance()
{
	static BSGraphics::Renderer* g_Renderer = (Renderer*)RELOCATION_ID(524907, 524907).address();  // no added ae address
	return g_Renderer;
}
