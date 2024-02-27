#pragma once
#include "BugModel.h"
#include "Globals.h"

#include <imgui.h>
#include <imgui-SFML.h>
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
	size_t GetTotalBugs();
};