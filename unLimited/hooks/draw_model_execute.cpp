#include "hooks.hpp"

#include "../features/features.hpp"
#include "../options.hpp"

void __stdcall hooks::hk_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
    static const auto o_draw_model_execute = model_render_hook->get_original<DrawModelExecuteFn>(index::draw_model_execute);

    //visuals::chams2(o_draw_model_execute, ctx, state, pInfo, pCustomBoneToWorld);

    // FIXMEW: add arm chams, etc, and fix glass chams

    o_draw_model_execute(g_model_render, ctx, state, info, matrix);
}
