#include "options.hpp"

#include <Windows.h>

#include <vector>

#include "sdk/sdk.hpp"

namespace options
{
	std::vector<option_t> bones;
	std::vector<option_t> keys;

	void init()
	{
		bones.push_back({ BONE_HEAD, "head" });
		bones.push_back({ BONE_NECK, "neck" });
		bones.push_back({ BONE_UPPER_SPINAL_COLUMN, "chest" });
		bones.push_back({ BONE_MIDDLE_SPINAL_COLUMN, "nips xd" });
		bones.push_back({ BONE_LOWER_SPINAL_COLUMN, "stomach" });
		bones.push_back({ BONE_HIP, "hip" });
		bones.push_back({ BONE_PELVIS, "pelvis" });

		keys.push_back({ 0, "none" });
		keys.push_back({ VK_XBUTTON1, "mouse4" });
		keys.push_back({ VK_XBUTTON2, "mouse5" });
		keys.push_back({ VK_MBUTTON, "mouse3" });
		keys.push_back({ VK_MENU, "alt" });
		keys.push_back({ 0x57, "w" });
	}

	namespace aim
	{
		bool enabled = true;

		bool aimbot = false;
		float fov = 1.f;
		bool smooth = true;
		float smooth_amount = 5.f;
		int bone = 8;
		bool on_shoot = true;
		bool reaim = true;
		int aim_key = 0;
		bool friendlies = false;
		bool visible_only = true;

		bool rcs = false;
	}

	namespace esp
	{
		bool enabled = true;

		bool glow = false;
		float glow_alpha = 1.f;
		int glow_style = 0;

		bool players = true;
		bool friendlies = true;
		bool chickens = false;
		bool defuse_kits = false;
		bool c4 = false;
		bool weapons = false;
	}

	namespace misc
	{
		bool backtracking = false;
		bool backtracking_vis = true;

		bool airstuck = false;
		int airstuck_key = 0;

		float fov = 90.f;

		bool nightmode = false;

		bool show_ranks = false;
	}
}
