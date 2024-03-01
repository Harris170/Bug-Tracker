#include "save_load.h"

void SaveLoad::SetSavePath(const char* path)
{
	save_path = path;
}

std::string SaveLoad::GetSavePath()
{
	return save_path;
}

bool SaveLoad::Save(std::vector<BugModel> bugs, std::string file_name = "save.txt")
{
	// Create a text file in given path
	std::ofstream save_file(GetSavePath() + (std::string)"/" + file_name);

	if (save_file.is_open())
	{
		// Store the info from bugs to text file
		int index = 0;
		for (auto bug : bugs)
		{
			index += 1;
			save_file << "UID:\t" << bug.uid << std::endl;
			save_file << "Title:\t" << bug.title << std::endl;
			save_file << "Status:\t" << bug.status << std::endl;

			if (index < bugs.size())	// Only add a new line if current bug is not the last bug
			{
				save_file << std::endl;
			}
		}
		// Close text file
		save_file.close();
		return true;
	}
	return false;
}

bool SaveLoad::Load(std::string load_file_path, BugManager* bug_manager)
{
	std::ifstream load_file;
	load_file.open(load_file_path);

	if (load_file.is_open())
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
			bug_manager->PushBackBug(&bug);

			std::getline(load_file, line);	// Skip one line between each bug
		}
		return true;
	}
	return false;
}