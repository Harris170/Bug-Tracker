#include "SaveLoad.h"

bool show = false;

void SaveLoad::SetSavePath(const char* path)
{
	save_path = path;
}

std::string SaveLoad::GetSavePath()
{
	return save_path;
}

void SaveLoad::SetCurrentFile(std::string file_name)
{
	current_file = file_name;
}

std::string SaveLoad::GetCurrentFile()
{
	return current_file;
}

bool SaveLoad::Save(std::vector<BugModel> bugs, std::string file_name)
{
	if (file_name == ".txt")
	{
		return false;
	}

	std::ofstream save_file(GetSavePath() + (std::string)"/" + file_name);

	if (save_file.is_open())
	{
		int index = 0;
		for (auto bug : bugs)
		{
			index += 1;
			save_file << "UID:\t" << bug.uid << std::endl;
			save_file << "Title:\t" << bug.title << std::endl;
			save_file << "Status:\t" << bug.status << std::endl;

			if (index < bugs.size())
			{
				save_file << std::endl;
			}
		}
		save_file.close();
		ImWidgets::CreateNotification("Saved file successfully.", &notifications, 3, ImNotificationFlags_BottomLeft);
		return true;
	}

	ImWidgets::CreateNotification("Could not save file.", &notifications, 3, ImNotificationFlags_BottomLeft);
	return false;
}

bool SaveLoad::Load(std::string load_file_path, BugManager* bug_manager)
{
	std::ifstream load_file(load_file_path);

	bool is_empty = load_file.peek() == std::ifstream::traits_type::eof();

	if (load_file.good() and not is_empty)
	{
		std::string line;

		while (not load_file.eof())
		{
			std::getline(load_file, line);
			std::string uid_str = line.substr(5);
			size_t uid = std::stoull(uid_str);

			std::getline(load_file, line);
			std::string title = line.substr(7);

			std::getline(load_file, line);
			std::string status_str = line.substr(8);
			bool status = status_str == "1" ? true : false;
			BugModel bug = bug_manager->CreateNewBug(uid, title, status);

			if (not bug_manager->BugAlreadyExists(&bug))
			{
				bug_manager->PushBackBug(&bug);
			}

			std::getline(load_file, line);	// Skip one line between each bug
		}
		load_file.close();
		ImWidgets::CreateNotification("Opened file successfully.", &notifications, 3, ImNotificationFlags_BottomLeft);
		return true;
	}
	ImWidgets::CreateNotification("Could not open file.", &notifications, 3, ImNotificationFlags_BottomLeft);
	return false;
}

bool SaveLoad::SaveConfig()
{
	std::ofstream save_file(GetSavePath() + (std::string)"/config.txt");

	if (save_file.is_open())
	{
		save_file << "SavePath:\t" << GetSavePath() << std::endl;
		save_file << "CurrentFile:\t" << GetCurrentFile() << std::endl;

		//? FONT SIZE IS NOT BEING SAVED CORRECTLY. SAVE IT DIFFERENTLY.
		save_file << "FontSize:\t" << ImGui::GetFontSize() << std::endl;
		save_file << "ActiveColor:\t" << default_color.x << " " << default_color.y << " " << default_color.z << " " << default_color.w << std::endl;
		save_file << "InactiveColor:\t" << dimmed_color.x << " " << dimmed_color.y << " " << dimmed_color.z << " " << dimmed_color.w << std::endl;

		save_file.close();
		return true;
	}
	return false;
}

bool SaveLoad::LoadConfig()
{
	std::ifstream load_file(GetSavePath() + (std::string)"/config.txt");

	if (load_file.is_open())
	{
		std::string line;
		std::getline(load_file, line);
		std::string save_path = line.substr(10);
		SetSavePath(save_path.c_str());

		std::getline(load_file, line);
		std::string current_file = line.substr(13);
		SetCurrentFile(current_file);

		std::getline(load_file, line);
		std::string font_size_str = line.substr(10);
		config.font_size = std::stof(font_size_str);

		std::getline(load_file, line);
		std::string active_color_str = line.substr(12);
		std::string active_color_x = active_color_str.substr(0, active_color_str.find(" "));
		active_color_str.erase(0, active_color_str.find(" ") + 1);
		std::string active_color_y = active_color_str.substr(0, active_color_str.find(" "));
		active_color_str.erase(0, active_color_str.find(" ") + 1);
		std::string active_color_z = active_color_str.substr(0, active_color_str.find(" "));
		active_color_str.erase(0, active_color_str.find(" ") + 1);
		std::string active_color_w = active_color_str;
		config.active_color = ImVec4(std::stof(active_color_x), std::stof(active_color_y), std::stof(active_color_z), std::stof(active_color_w));
		SetDefaultColor(config.active_color);

		std::getline(load_file, line);
		std::string inactive_color_str = line.substr(14);
		std::string inactive_color_x = inactive_color_str.substr(0, inactive_color_str.find(" "));
		inactive_color_str.erase(0, inactive_color_str.find(" ") + 1);
		std::string inactive_color_y = inactive_color_str.substr(0, inactive_color_str.find(" "));
		inactive_color_str.erase(0, inactive_color_str.find(" ") + 1);
		std::string inactive_color_z = inactive_color_str.substr(0, inactive_color_str.find(" "));
		inactive_color_str.erase(0, inactive_color_str.find(" ") + 1);
		std::string inactive_color_w = inactive_color_str;
		config.inactive_color = ImVec4(std::stof(inactive_color_x), std::stof(inactive_color_y), std::stof(inactive_color_z), std::stof(inactive_color_w));
		SetDimmedColor(config.inactive_color);

		load_file.close();
	}
	else
	{
		ImWidgets::CreateNotification("No config file found. Loading defaults.", &notifications, 3, ImNotificationFlags_BottomLeft);
		SaveConfig();
	}
}