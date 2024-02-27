#include "BugModel.h"

const std::string& BugModel::GetName() const
{
	return name;
}

const std::string& BugModel::GetDescription() const
{
	return description;
}

bool BugModel::IsCompleted() const
{
	return completed;
}

void BugModel::SetCompleted(bool completed)
{
	this->completed = completed;
}