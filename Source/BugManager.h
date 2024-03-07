#pragma once
#include "BugModel.h"

#include <vector>
#include <algorithm>

class BugManager
{
private:
	std::vector<BugModel> bugs = {};
public:
	void DrawBugsList();
	void RegisterNewBug();
	void RemoveResolvedBugs();
	std::vector<BugModel> GetAllBugs();
	size_t GetTotalBugs();
	BugModel CreateNewBug(size_t uid, std::string title, bool status);
	void PushBackBug(BugModel* bug);
	bool BugAlreadyExists(BugModel* bug);
};