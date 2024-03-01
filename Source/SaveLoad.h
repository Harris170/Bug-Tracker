#pragma once
#include "BugModel.h"
#include "BugManager.h"

#include <iostream>
#include <fstream>
#include <vector>

class SaveLoad {
private:
	std::string save_path = "D:/Development/C++/BugTracker";
	std::string current_file = "Untitled";
public:

	void SetSavePath(const char* path);
	std::string GetSavePath();
	void SetCurrentFile(std::string file_name);
	std::string GetCurrentFile();
	bool Save(std::vector<BugModel> bugs, std::string file_name);
	bool Load(std::string load_file_path, BugManager* bug_manager);
};