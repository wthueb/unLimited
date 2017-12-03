#include "options.hpp"

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

	void unload()
	{
		aim::enabled = false;
		antiaim::enabled = false;
		visuals::enabled = false;
		misc::bhop = false;
		misc::autostrafe = false;
		misc::backtracking = false;
		misc::show_ranks = false;
		misc::nightmode = false;
		misc::airstuck = false;
		misc::fov = 90.f;
		misc::chat_spam = false;
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
		bool vis_check = true;

		bool rcs = false;
	}

	namespace antiaim
	{
		bool enabled = false;
		bool show = true;

		int type = LEGIT;
		bool fakelag = false;
	}

	namespace visuals
	{
		bool enabled = true;

		bool chams = false;
		bool ignorez = false;

		bool glow = false;
		float glow_alpha = 1.f;
		int glow_style = 0;

		bool players = true;
		bool friendlies = true;
		bool chickens = false;
		bool defuse_kits = false;
		bool c4 = false;
		bool weapons = false;

		bool radar = false;

		bool thirdperson = false;
		float thirdperson_offset = 150.f;

		bool noscope = false;

		bool noflash = false;
	}

	namespace misc
	{
		bool bhop = false;
		bool autostrafe = false;

		bool backtracking = false;
		bool backtracking_vis = true;
		int backtracking_amt = 12;

		bool show_ranks = false;

		bool nightmode = false;

		bool airstuck = false;
		int airstuck_key = 0;

		float fov = 90.f;

		bool chat_spam = false;
	}
}
