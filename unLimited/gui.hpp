#pragma once

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <algorithm>
#include <functional>

#include "config.hpp"
#include "font.hpp"
#include "kit_parser.hpp"
#include "notifier.hpp"
#include "options.hpp"
#include "utils.hpp"

#define VERSION_MAJOR 0
#define VERSION_MINOR 7
#define VERSION_PATCH 11

namespace ImGui
{
    bool BetterCheckbox(const char* label, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2)); // We want a square shape to we use Y twice
        ItemSize(check_bb, style.FramePadding.y);

        ImRect total_bb = check_bb;
        if (label_size.x > 0)
            SameLine(0, style.ItemInnerSpacing.x / 5.f);
        const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
        if (label_size.x > 0)
        {
            ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
            total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
        }

        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        RenderFrame(check_bb.Min + ImVec2{ 5.f, 5.f }, check_bb.Max - ImVec2{ 5.f, 5.f }, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
        if (*v)
        {
            const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
            const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
            RenderFrame(check_bb.Min + ImVec2{ 6.f, 6.f }, check_bb.Max - ImVec2{ 6.f, 6.f }, GetColorU32(ImGuiCol_CheckMark));
        }

        if (label_size.x > 0.0f)
            RenderText(text_bb.Min, label);

        return pressed;
    }

    void BetterPopItemFlag()
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->DC.ItemFlagsStack.size())
            window->DC.ItemFlagsStack.pop_back();
        window->DC.ItemFlags = window->DC.ItemFlagsStack.empty() ? ImGuiItemFlags_Default_ : window->DC.ItemFlagsStack.back();
    }

    bool ListBox(const char* label, int* current_item, std::function<const char*(int)> lambda, int items_count, int height_in_items)
    {
        return ImGui::ListBox(label, current_item, [](void* data, int idx, const char** out_text)
        {
            *out_text = (*reinterpret_cast<std::function<const char*(int)>*>(data))(idx);
            return true;
        }, &lambda, items_count, height_in_items);
    }
}

// this might be the ugliest thing i have ever done holy shit
#define DISABLE(var, fade_time) \
{                                                                                                           \
    static double alpha = .3;                                                                               \
    static bool old = var;                                                                                  \
    static double start_alpha = alpha;                                                                       \
    static double start_time = ImGui::GetTime();                                                             \
                                                                                                            \
    if (var != old)                                                                                         \
    {                                                                                                       \
        start_alpha = alpha;                                                                                \
        start_time = ImGui::GetTime();                                                                      \
        old = var;                                                                                          \
    }                                                                                                       \
                                                                                                            \
    if (!var)                                                                                               \
        alpha = std::clamp(start_alpha - .7 * (ImGui::GetTime() - start_time) / fade_time, .3, 1.);      \
    else                                                                                                    \
        alpha = std::clamp(start_alpha + .7 * (ImGui::GetTime() - start_time) / fade_time, .3, 1.);      \
                                                                                                            \
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, float(std::min(double(style.Alpha), alpha)));                                 \
                                                                                                            \
    ImGuiWindow* window = ImGui::GetCurrentWindow();                                                        \
    if (!window->DC.ItemFlagsStack.size() || !(window->DC.ItemFlagsStack.back() & ImGuiItemFlags_Disabled)) \
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !var);                                                 \
}

#define ADD_CHECKBOX(name, var) if (ImGui::BetterCheckbox(name, var)) notifier::option_changed(name, *var);

namespace gui
{
    static char title[64];
    static float theme_color[3] = { 31.f / 255.f, 110.f / 255.f, 149.f / 255.f };
    static bool dark_mode = true;

    static void setup_style(const ImVec4& theme_color)
    {
        static auto& style = ImGui::GetStyle();

        style.WindowTitleAlign = { .5f, .5f };
        style.WindowRounding = 3.f;
        style.FrameBorderSize = 0.f;

        style.Colors[ImGuiCol_Text] = ImVec4{ .05f, .05f, .05f, 1.f };
        style.Colors[ImGuiCol_TextDisabled] = style.Colors[ImGuiCol_Text];
        style.Colors[ImGuiCol_WindowBg] = ImVec4{ .9f, .9f, .9f, 1.f };
        //style.Colors[ImGuiCol_ChildWindowBg] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_PopupBg] = ImVec4{ .6f, .6f, .6f, 1.f };
        style.Colors[ImGuiCol_Border] = ImVec4{ 0.f, 0.f, 0.f, 1.f };
        style.Colors[ImGuiCol_BorderShadow] = ImVec4{ 0.f, 0.f, 0.f, 1.f };
        style.Colors[ImGuiCol_FrameBg] = ImVec4{ .6f, .6f, .6f, 1.f };
        style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_FrameBg];
        style.Colors[ImGuiCol_FrameBgActive] = style.Colors[ImGuiCol_FrameBg];
        style.Colors[ImGuiCol_TitleBg] = theme_color;
        style.Colors[ImGuiCol_TitleBgActive] = theme_color;
        style.Colors[ImGuiCol_TitleBgCollapsed] = theme_color;
        style.Colors[ImGuiCol_MenuBarBg] = theme_color;
        style.Colors[ImGuiCol_ScrollbarBg] = theme_color;
        style.Colors[ImGuiCol_ScrollbarGrab] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = style.Colors[ImGuiCol_ScrollbarGrab];
        style.Colors[ImGuiCol_ScrollbarGrabActive] = style.Colors[ImGuiCol_ScrollbarGrab];
        style.Colors[ImGuiCol_CheckMark] = theme_color;
        style.Colors[ImGuiCol_SliderGrab] = theme_color;
        style.Colors[ImGuiCol_SliderGrabActive] = style.Colors[ImGuiCol_SliderGrab];
        style.Colors[ImGuiCol_Button] = theme_color;
        style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_Button];
        style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_Button];
        style.Colors[ImGuiCol_Header] = theme_color;
        style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_WindowBg];
    }

    static void draw_gui()
    {
        static bool skin_window_open = false;

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(850, 500));

        static auto& style = ImGui::GetStyle();

        if (ImGui::Begin(title, nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::Columns(3, nullptr, true); // aim
            {
                ImGui::PushItemWidth(150.f);

                ADD_CHECKBOX("aim", &options.aim_enabled);
                {
                    DISABLE(options.aim_enabled, .2);

                    ADD_CHECKBOX("aimbot", &options.aim_aimbot);
                    {
                        DISABLE(options.aim_aimbot, .2);

                        ImGui::SliderFloat("fov", &options.aim_fov, .1f, 180.f, "%.1f", 2.f);

                        ADD_CHECKBOX("smooth", &options.aim_smooth);
                        {
                            DISABLE(options.aim_smooth, .2);

                            ImGui::SliderFloat("smooth amount", &options.aim_smooth_amount, 1.f, 30.f, "%.1f", 1.f);

                            ImGui::PopStyleVar();
                            ImGui::BetterPopItemFlag();
                        }

                        // bone
                        {
                            static int selected = 0;
                            if (ImGui::Combo("bone", &selected, [](void* data, int idx, const char** out_text)
                            {
                                *out_text = options.bones.at(idx).second;
                                return true;
                            }, nullptr, int(options.bones.size()), -1))
                                options.aim_bone = options.bones.at(selected).first;
                        }

                        ADD_CHECKBOX("on shoot", &options.aim_on_shoot);

                        ADD_CHECKBOX("reaim", &options.aim_reaim);

                        // aim key
                        {
                            static int selected = 0;
                            if (ImGui::Combo("aim key", &selected, [](void* data, int idx, const char** out_text)
                            {
                                *out_text = options.keys.at(idx).second;
                                return true;
                            }, nullptr, int(options.keys.size()), -1))
                                options.aim_aim_key = options.keys.at(selected).first;
                        }

                        ADD_CHECKBOX("shoot teammates", &options.aim_friendlies);

                        ADD_CHECKBOX("visible check", &options.aim_vis_check);

                        ImGui::PopStyleVar();
                        ImGui::BetterPopItemFlag();
                    }

                    ADD_CHECKBOX("rcs", &options.aim_rcs);

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                ImGui::PopItemWidth();
            }

            ImGui::NextColumn(); // visuals
            {
                ImGui::PushItemWidth(150.f);

                ADD_CHECKBOX("visuals", &options.visuals_enabled);
                {
                    DISABLE(options.visuals_enabled, .2);

                    ADD_CHECKBOX("chams", &options.visuals_chams);
                    {
                        DISABLE(options.visuals_chams, .2);

                        ADD_CHECKBOX("ignorez (through walls)", &options.visuals_chams_ignorez);
                        ADD_CHECKBOX("flat", &options.visuals_chams_flat);
                                                
                        ImGui::PopStyleVar();
                        ImGui::BetterPopItemFlag();
                    }

                    ADD_CHECKBOX("glow", &options.visuals_glow);
                    {
                        DISABLE(options.visuals_glow, .2);

                        if (ImGui::InputFloat("glow alpha", &options.visuals_glow_alpha, .1f, 0.f, 1))
                            options.visuals_glow_alpha = std::clamp(options.visuals_glow_alpha, .1f, 1.f);

                        if (ImGui::InputInt("glow style", &options.visuals_glow_style))
                            options.visuals_glow_style = std::clamp(options.visuals_glow_style, 0, 3);

                        ImGui::PopStyleVar();
                        ImGui::BetterPopItemFlag();
                    }

                    ADD_CHECKBOX("skeletons", &options.visuals_skeletons);

                    ADD_CHECKBOX("radar", &options.visuals_radar);

                    ADD_CHECKBOX("thirdperson", &options.visuals_thirdperson);
                    {
                        DISABLE(options.visuals_thirdperson, .2);

                        ImGui::SliderFloat("thirdperson offset", &options.visuals_thirdperson_offset, 20.f, 300.f, "%.0f");

                        ImGui::PopStyleVar();
                        ImGui::BetterPopItemFlag();
                    }

                    ADD_CHECKBOX("disable scope", &options.visuals_noscope);

                    ADD_CHECKBOX("noflash", &options.visuals_noflash);

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                ImGui::PopItemWidth();
            }

            ImGui::NextColumn(); // misc
            {
                ImGui::PushItemWidth(150.f);

                if (ImGui::Button("skinchanger"))
                    skin_window_open = !skin_window_open;

                ADD_CHECKBOX("bhop", &options.misc_bhop);

                //ADD_CHECKBOX("autostrafe", options.misc_autostrafe);

                ADD_CHECKBOX("backtracking", &options.misc_backtracking);
                {
                    DISABLE(options.misc_backtracking, .2);

                    ADD_CHECKBOX("backtracking visual", &options.misc_backtracking_vis);

                    // backtracking amount
                    {
                        ImGui::PushItemWidth(30.f);

                        static float temp = float(options.misc_backtracking_amt);

                        // so we don't have the + -, kind of stupid but
                        if (ImGui::InputFloat("backtracking amount (ticks, max 12)", &temp, 0.f, 0.f, 0))
                            options.misc_backtracking_amt = std::clamp(int(temp), 1, 12);

                        ImGui::PopItemWidth();
                    }

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                ADD_CHECKBOX("anti-aim", &options.aa_enabled);
                {
                    DISABLE(options.aa_enabled, .2);

                    static auto selected = int(options.aa_type);

                    if (ImGui::Combo("aa type", &selected, [](void* data, int idx, const char** out_text)
                    {
                        switch (aa_type(idx))
                        {
                        case aa_type::LEGIT:
                            *out_text = "legit";
                            break;

                        case aa_type::RAGE:
                            *out_text = "rage";
                            break;

                        case aa_type::LBY_SIDEWAYS:
                            *out_text = "lby";
                            break;

                        case aa_type::SPIN_SLOW:
                            *out_text = "slow spin";
                            break;

                        case aa_type::SPIN_FAST:
                            *out_text = "fast spin";
                            break;

                        default:
                            return false;
                            break;
                        }

                        return true;
                    }, nullptr, int(aa_type::AA_COUNT), -1))
                        options.aa_type = aa_type(selected);

                    ADD_CHECKBOX("show angles/choke", &options.aa_show);

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                ADD_CHECKBOX("fakelag", &options.misc_fakelag);

                ADD_CHECKBOX("show ranks", &options.misc_show_ranks);

                ADD_CHECKBOX("nightmode", &options.misc_nightmode);

                ADD_CHECKBOX("airstuck", &options.misc_airstuck);
                {
                    DISABLE(options.misc_airstuck, .2);

                    // airstuck key
                    {
                        static int selected = 0;
                        if (ImGui::Combo("airstuck key", &selected, [](void* data, int idx, const char** out_text)
                        {
                            *out_text = options.keys.at(idx).second;
                            return true;
                        }, nullptr, int(options.keys.size()), -1))
                            options.misc_airstuck_key = options.keys.at(selected).first;
                    }

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                if (ImGui::InputFloat("view fov", &options.misc_fov, 1.f, 0.f, 0))
                    options.misc_fov = std::clamp(options.misc_fov, 5.f, 179.f);

                ADD_CHECKBOX("fov while scoped", &options.misc_fov_scoped);

                ADD_CHECKBOX("chat spam", &options.misc_chat_spam);

                ADD_CHECKBOX("callout", &options.misc_callout);
                {
                    DISABLE(options.misc_callout, .2);

                    ADD_CHECKBOX("callout self", &options.misc_callout_self);

                    ImGui::PopStyleVar();
                    ImGui::BetterPopItemFlag();
                }

                ImGui::PopItemWidth();
            }

            ImGui::Columns(1, nullptr, false);

            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::CalcTextSize("").y - 14.f);

            ImGui::BetterCheckbox("dark mode", &dark_mode);
            {
                static bool old_dark_mode = false;

                if (dark_mode != old_dark_mode)
                {
                    for (auto i = 0; i < ImGuiCol_COUNT; ++i)
                    {
                        ImVec4& col = style.Colors[i];

                        float hue, saturation, value;
                        ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, hue, saturation, value);

                        if (saturation < .1f)
                            value = 1.f - value;

                        ImGui::ColorConvertHSVtoRGB(hue, saturation, value, col.x, col.y, col.z);
                    }

                    old_dark_mode = dark_mode;
                }
            }

            ImGui::SameLine();
            
            if (ImGui::ColorEdit3("theme color", theme_color, ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip))
                setup_style(ImVec4{ theme_color[0], theme_color[1], theme_color[2], 1.f });

            // unload button
            {
                auto label_size = ImGui::CalcTextSize("unload");

                ImVec2 size{ label_size.x + style.FramePadding.x * 2.f, label_size.y + style.FramePadding.y * 2.f };

                ImGui::SameLine(ImGui::GetWindowWidth() / 2.f - (label_size.x + style.FramePadding.x * 2.f) / 2.f);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .4f, 0.f, 0.f, 1.f });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .4f, 0.f, 0.f, 1.f });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .4f, 0.f, 0.f, 1.f });
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ .9f, .9f, .9f, 1.f });

                if (ImGui::Button("unload", size))
                {
                    utils::should_unload = true;
                    utils::printf("unload called for by pressing menu button\n\n");
                }

                ImGui::PopStyleColor(4);
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("wi1.xyz").x - 14.f);

            ImGui::Text("wi1.xyz");

            ImGui::End();
        }

        if (skin_window_open)
        {
            ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(700, 415));
            
            if (ImGui::Begin("skinchanger", &skin_window_open,
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings))
            {
                auto& entries = config::items;

                if (entries.empty())
                    entries.push_back(econ_item_t{});

                static auto selected_idx = 0;

                ImGui::Columns(2, nullptr, false); // list of items
                {
                    ImGui::PushItemWidth(-1);

                    char element_name[64];

                    ImGui::ListBox("##items", &selected_idx, [&element_name, &entries](int idx)
                    {
                        snprintf(element_name, sizeof(element_name), "%s (%s)", entries.at(idx).name, weapon_names.at(entries.at(idx).definition_vector_index).name);
                        return element_name;
                    }, int(entries.size()), 12);

                    static auto button_size = ImVec2{ ImGui::GetColumnWidth() / 2.f - 12.5f, 20 };

                    if (ImGui::Button("add", button_size))
                        entries.insert(entries.begin() + ++selected_idx, econ_item_t{});

                    ImGui::SameLine();

                    if (ImGui::Button("remove", button_size))
                        if (selected_idx > 0)
                            entries.erase(entries.begin() + selected_idx--);
                        else if (entries.size() > 1)
                            entries.erase(entries.begin() + selected_idx);

                    ImGui::PopItemWidth();
                }

                auto& selected_entry = entries[selected_idx];

                ImGui::NextColumn(); // configurator
                {
                    ImGui::InputText("name", selected_entry.name, 32);

                    ImGui::Combo("item", &selected_entry.definition_vector_index, [](void* data, int idx, const char** out_text)
                    {
                        *out_text = weapon_names.at(idx).name;
                        return true;
                    }, nullptr, int(weapon_names.size()), 10);

                    ImGui::BetterCheckbox("enabled", &selected_entry.enabled);

                    if (ImGui::InputInt("seed", &selected_entry.seed))
                        selected_entry.seed = std::max(0, selected_entry.seed);

                    if (ImGui::InputInt("stattrak", &selected_entry.stat_trak))
                        selected_entry.stat_trak = std::max(0, selected_entry.stat_trak);

                    ImGui::SliderFloat("float", &selected_entry.wear, .00001f, 1.f, "%.5f");

                    if (selected_entry.definition_index != GLOVE_T_SIDE)
                    {
                        ImGui::Combo("paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
                        {
                            *out_text = weapon_kits.at(idx).name.c_str();
                            return true;
                        }, nullptr, int(weapon_kits.size()), 10);
                    }
                    else
                    {
                        ImGui::Combo("paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
                        {
                            *out_text = glove_kits.at(idx).name.c_str();
                            return true;
                        }, nullptr, int(glove_kits.size()), 10);
                    }

                    ImGui::Combo("quality", &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text)
                    {
                        *out_text = quality_names.at(idx).name;
                        return true;
                    }, nullptr, int(quality_names.size()), int(quality_names.size()));

                    selected_entry.update_values();

                    if (selected_entry.definition_index == WEAPON_KNIFE)
                    {
                        ImGui::Combo("knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
                        {
                            *out_text = knife_names.at(idx).name;
                            return true;
                        }, nullptr, int(knife_names.size()), int(knife_names.size()));
                    }
                    else if (selected_entry.definition_index == GLOVE_T_SIDE)
                    {
                        ImGui::Combo("glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
                        {
                            *out_text = glove_names.at(idx).name;
                            return true;
                        }, nullptr, int(glove_names.size()), int(glove_names.size()));
                    }
                    else
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, .3f);

                        static auto unused_value = 0;
                        selected_entry.definition_override_vector_index = 0;

                        ImGui::Combo("unavailable", &unused_value, "unavailable\0");

                        ImGui::PopStyleVar();
                    }

                    selected_entry.update_values();

                    ImGui::InputText("name tag", selected_entry.custom_name, 32);
                }

                ImGui::Columns(1, nullptr, false);

                ImGui::Separator();

                static auto selected_sticker_slot = 0;

                auto& selected_sticker = selected_entry.stickers[selected_sticker_slot];

                ImGui::Columns(2, nullptr, false); // sticker slots
                {
                    ImGui::PushItemWidth(-1);

                    char element_name[64];

                    ImGui::ListBox("##stickers", &selected_sticker_slot, [&selected_entry, &element_name](int idx)
                    {
                        auto kit_vector_index = selected_entry.stickers[idx].kit_vector_index;
                        snprintf(element_name, sizeof(element_name), "#%d (%s)", idx + 1, stickers.at(kit_vector_index).name.c_str());
                        return element_name;
                    }, 5, 5);

                    ImGui::PopItemWidth();
                }

                ImGui::NextColumn(); // sticker configurator
                {
                    ImGui::Combo("sticker", &selected_sticker.kit_vector_index, [](void* data, int idx, const char** out_text)
                    {
                        *out_text = stickers.at(idx).name.c_str();
                        return true;
                    }, nullptr, int(stickers.size()), 10);

                    ImGui::SliderFloat("float", &selected_sticker.wear, .00001f, 1.f, "%.5f");

                    ImGui::SliderFloat("scale", &selected_sticker.scale, 0.1f, 5.f, "%.3f");

                    ImGui::SliderFloat("rotation", &selected_sticker.rotation, 0.f, 360.f, "%.3f");
                }

                ImGui::Columns(1, nullptr, false);

                ImGui::Separator();

                ImGui::Columns(3, nullptr, false);

                auto button_size = ImVec2{ ImGui::GetColumnWidth() - 15.f, 20.f };

                if (ImGui::Button("update", button_size))
                    g_client_state->ForceFullUpdate();

                ImGui::NextColumn();

                if (ImGui::Button("save", button_size))
                    config::save();

                ImGui::NextColumn();

                if (ImGui::Button("load", button_size))
                    config::load();

                ImGui::End();
            }
        }
    }

    static void init()
    {
        snprintf(title, sizeof(title), "unLimited by wi1 | v%i.%i.%i", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

        auto& io = ImGui::GetIO();

        io.Fonts->AddFontDefault();
        auto font = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoFont_compressed_data, RobotoFont_compressed_size, 14.f);
        io.FontDefault = font;

        setup_style(ImVec4{ theme_color[0], theme_color[1], theme_color[2], 1.f });
    }
}
