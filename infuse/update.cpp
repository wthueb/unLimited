#include "update.hpp"

#include "colors.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <curl/curl.h>

#ifdef _DEBUG
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif

#pragma comment(lib, "ws2_32.lib")

size_t write_data_file(void* contents, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(contents, size, nmemb, stream);

	return written;
}

size_t write_data_string(void* contents, size_t size, size_t nmemb, std::string* s)
{
	size_t new_length = size * nmemb;
	size_t old_length = s->size();

	try
	{
		s->resize(old_length + new_length);
	}
	catch (std::bad_alloc)
	{
		return 0;
	}

	std::copy(static_cast<char*>(contents), static_cast<char*>(contents) + new_length, s->begin() + old_length);
	return size * nmemb;
}

bool get_latest_ver(CURL* &curl, std::string &latest_version)
{
	cyan;
	std::cout << "getting latest version number...\n\n";

	std::string url = "http://wi1.us.to/latest-unLimited.txt";
	
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_string);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &latest_version);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

	CURLcode result = curl_easy_perform(curl);

	if (result != CURLE_OK)
	{
		curl_easy_cleanup(curl);
		return false;
	}

	latest_version.erase(std::remove(latest_version.begin(), latest_version.end(), '\n'), latest_version.end());

	return true;
}

bool get_current_ver(std::string &currentversion)
{
	HANDLE find;
	WIN32_FIND_DATA data;

	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);

	sprintf_s(path, "%s\\unLimited*.dll", path);

	find = FindFirstFileA(path, &data);
	if (find != INVALID_HANDLE_VALUE)
	{
		currentversion = data.cFileName;

		do
		{
			if (currentversion.compare(data.cFileName) < 0)
				currentversion = data.cFileName;
		} while (FindNextFileA(find, &data));

		FindClose(find);
	}
	else
		return false;

	return true;
}

bool download_version(CURL* &curl, const std::string &version)
{
	std::string url = "http://wi1.us.to/unLimited-downloads/";

	url.append(version);

	cyan;
	std::cout << "downloading latest version from " << url << "...\n\n";

	char outfile_path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, outfile_path);
	sprintf_s(outfile_path, "%s\\%s", outfile_path, version.c_str());

	FILE* outfile;
	fopen_s(&outfile, outfile_path, "wb");

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_file);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);

	CURLcode result = curl_easy_perform(curl);

	if (result != CURLE_OK)
	{
		curl_easy_cleanup(curl);
		fclose(outfile);
		return false;
	}

	fclose(outfile);

	green;
	std::cout << "latest version successfully downloaded!\n\n";

	return true;
}

void delete_old_versions(const std::string &latest_version)
{
	HANDLE find;
	WIN32_FIND_DATA data;

	char working_path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, working_path);

	char find_files[MAX_PATH];

	sprintf_s(find_files, "%s\\unLimited*.dll", working_path);

	std::vector<std::string> old_dlls; // contains file names (not full paths)

	find = FindFirstFileA(find_files, &data);
	if (find != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (latest_version.compare(data.cFileName)) // if data.cFileName is not the latest version
				old_dlls.push_back(std::string(data.cFileName));
		} while (FindNextFileA(find, &data));

		FindClose(find);
	}

	if (old_dlls.size() <= 0)
		return;

	cyan;
	std::cout << "deleting old versions...\n\n";

	for (size_t i = 0; i < old_dlls.size(); ++i)
	{
		cyan;
		 std::cout << "- deleting " << old_dlls.at(i) << "...\n";

		 char path[MAX_PATH];
		 sprintf_s(path, "%s\\%s", working_path, old_dlls.at(i).c_str());

		 if (!remove(path))
		 {
			 green;
			 std::cout << "- " << old_dlls.at(i) << " successfully deleted\n\n";
		 }
		 else
		 {
			 red;
			 std::cout << "- " << old_dlls.at(i) << " could not be deleted...\n\n";
		 }
	}
}

bool update()
{
	CURL* curl = curl_easy_init();
	if (!curl)
		return false;

	std::string latest_version;
	
	if (!get_latest_ver(curl, latest_version))
		return false;

	cyan;
	std::cout << "latest version: " << latest_version << "\n\n";
	
	std::string current_version;

	if (get_current_ver(current_version))
	{
		cyan;
		std::cout << "current version: " << current_version << "\n\n";

		if (!current_version.compare("unLimited.dll") || current_version.compare(latest_version)) // if versions are different
		{
			red;
			std::cout << "current version is out of date!\n\n";
			if (!download_version(curl, latest_version))
				return false;
		}
		else
		{
			green;
			std::cout << "your version is up to date!\n\n";
		}
	}
	else
	{
		red;
		std::cout << "current version could not be found...\n\n";
		if (!download_version(curl, latest_version))
			return false;
	}

	delete_old_versions(latest_version);

	curl_easy_cleanup(curl);

	return true;
}
