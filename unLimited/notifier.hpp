#pragma once

namespace notifier
{
	void update_all_options();

	void option_changed(const char* name, const char* value);
	void option_changed(const char* name, bool value);
}
