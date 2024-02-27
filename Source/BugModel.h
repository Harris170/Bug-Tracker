#pragma once
#include <string>

class BugModel
{
private:
	std::string name;
	std::string description;
	bool completed;

public:
	BugModel(const std::string& name, const std::string& description, bool completed)
		: name(name), description(description), completed(completed) {}

	const std::string& GetName() const;
	const std::string& GetDescription() const;
	bool IsCompleted() const;
	void SetCompleted(bool completed);
};
