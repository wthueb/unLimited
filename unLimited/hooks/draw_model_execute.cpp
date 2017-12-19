#include "hooks.hpp"

#include "../features/features.hpp"

void __stdcall hooks::hk_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	static auto o_draw_model_execute = model_render_hook->get_original<DrawModelExecuteFn>(index::draw_model_execute);

	visuals::chams2(o_draw_model_execute, ctx, state, pInfo, pCustomBoneToWorld);

	o_draw_model_execute(g_model_render, ctx, state, pInfo, pCustomBoneToWorld);

	g_model_render->ForcedMaterialOverride(nullptr);
}
