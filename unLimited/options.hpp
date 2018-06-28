#pragma once

#include <Windows.h>

#include <array>

#include "sdk/sdk.hpp"

enum class aa_type
{
	LEGIT,
	RAGE,
	LBY_SIDEWAYS,
	SPIN_SLOW,
	SPIN_FAST,
	AA_COUNT
};

class options_class
{
public:
	std::array<std::pair<Bone, const char*>, 8> bones = { {
		{ BONE_INVALID, "dynamic" },
		{ BONE_HEAD, "head" },
		{ BONE_NECK, "neck" },
		{ BONE_UPPER_SPINAL_COLUMN, "chest" },
		{ BONE_MIDDLE_SPINAL_COLUMN, "nips xd" },
		{ BONE_LOWER_SPINAL_COLUMN, "stomach" },
		{ BONE_HIP, "hip" },
		{ BONE_PELVIS, "pelvis" }
	} };

	std::array<std::pair<int, const char*>, 6> keys = { {
		{ 0, "none" },
		{ VK_XBUTTON1, "mouse4" },
		{ VK_XBUTTON2, "mouse5" },
		{ VK_MBUTTON, "mouse3" },
		{ VK_MENU, "alt" },
		{ 0x57, "w" }
	} };

	void unload()
	{
		aim_enabled = false;
		aa_enabled = false;
		visuals_enabled = false;
		misc_bhop = false;
		misc_autostrafe = false;
		misc_backtracking = false;
		misc_show_ranks = false;
		misc_nightmode = false;
		misc_airstuck = false;
		misc_fov = 90.f;
		misc_chat_spam = false;
	}

	// aim

	bool aim_enabled = true;

	bool aim_aimbot = false;
	float aim_fov = 1.f;
	bool aim_smooth = true;
	float aim_smooth_amount = 5.f;
	int aim_bone = -1;
	bool aim_on_shoot = true;
	bool aim_reaim = true;
	int aim_aim_key = 0;
	bool aim_friendlies = false;
	bool aim_vis_check = true;

	bool aim_rcs = false;

	// anti-aim

	bool aa_enabled = false;
	bool aa_show = false;

	aa_type aa_type = aa_type::LEGIT;
	bool aa_fakelag = false;

	// visuals

	bool visuals_enabled = true;

	bool visuals_chams = false;
	bool visuals_chams_ignorez = false;
	bool visuals_chams_flat = false;
	bool visuals_chams_wireframe = false;
	bool visuals_chams_glass = false;

	// FIXMEW: add
	bool visuals_chams_arms = true;
	bool visuals_chams_no_hands = false;

	bool visuals_glow = false;
	float visuals_glow_alpha = 1.f;
	int visuals_glow_style = 0;

	bool visuals_skeletons = false;

	bool visuals_players = true;
	bool visuals_friendlies = true;
	bool visuals_chickens = false;
	bool visuals_defuse_kits = false;
	bool visuals_c4 = false;
	bool visuals_weapons = false;

	bool visuals_radar = false;

	bool visuals_thirdperson = false;
	float visuals_thirdperson_offset = 150.f;

	bool visuals_noscope = false;

	bool visuals_noflash = false;

	// misc

	bool misc_bhop = false;
	bool misc_autostrafe = false;

	bool misc_backtracking = false;
	bool misc_backtracking_vis = false;
	int misc_backtracking_amt = 12;

	bool misc_show_ranks = false;

	bool misc_nightmode = false;

	bool misc_airstuck = false;
	int misc_airstuck_key = 0;

	float misc_fov = 90.f;
	bool misc_fov_scoped = false;

	bool misc_chat_spam = false;

	bool misc_callout = false;
};

extern options_class options;
