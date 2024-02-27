#pragma once
#include <string>

class BugModel
{
public:
	size_t uid;
	std::string title;
	bool status;

	BugModel(size_t uid, const std::string& title, bool status)
		: uid(uid), title(title), status(status) {}
};
