#include "SaveLoad.h"

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
		return true;
	}
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
		return true;
	}
	return false;
}