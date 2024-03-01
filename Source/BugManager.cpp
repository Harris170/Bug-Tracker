#include "BugManager.h"

bool select_all = false;
bool just_pressed_select_all = false;

void BugManager::DrawBugsList()
{
	// Table header
	ImGui::BeginChild("BugsTable", ImVec2(ImGui::GetWindowWidth() - 16, 0), true);
	ImGui::PushFont(bold_font);

	ImGui::Columns(3, "bugTable", true);
	ImGui::Text("ID");

	ImGui::NextColumn();
	ImGui::Text("Title");
	ImGui::NextColumn();

	ImGui::Checkbox("##select_all", &select_all);
	ImGui::NextColumn();
	ImGui::PopFont();

	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.10f); // 10% for UID
	ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() * 0.82f); // 82% for Title
	ImGui::SetColumnWidth(2, ImGui::GetWindowWidth() * 0.3f); // 03% for Checkboxes

	// Custom sorting function to move checked bugs to the bottom
	auto checked_sort = [](const BugModel& a, const BugModel& b) {
		// Sort checked bugs to the bottom
		if (a.status && !b.status) {
			return false;
		}
		else if (!a.status && b.status) {
			return true;
		}
		// Preserve order for unchecked bugs and checked bugs
		return a.uid < b.uid;
		};
	std::sort(bugs.begin(), bugs.end(), checked_sort);

	// Table content with checkbox that changes the status of the bug
	if (bugs.size() > 0) {
		for (auto& bug : bugs) {
			// If bug is checked, it becomes dark gray
			if (bug.status)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, dimmed_color);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, default_color);
			}

			ImGui::Separator();
			// Center-align the ID column content
			ImGui::Text("%d", bug.uid);
			ImGui::NextColumn();

			// Center-align the Title column content
			ImGui::TextWrapped("%s", bug.title.c_str());
			ImGui::NextColumn();

			// Center-align the Status column content
			std::string checkboxId = "##checkbox" + std::to_string(bug.uid);
			if (ImGui::Checkbox(checkboxId.c_str(), &bug.status) && bug.status) {
				std::rotate(bugs.begin(), bugs.begin() + 1, bugs.end());
				saved_current_file = false;
			}

			ImGui::NextColumn();
			ImGui::PopStyleColor();
		}
	}

	if (select_all && not just_pressed_select_all) {
		for (auto& bug : bugs) {
			bug.status = true;
			just_pressed_select_all = true;
			saved_current_file = false;
		}
	}

	if (just_pressed_select_all && not select_all) {
		for (auto& bug : bugs) {
			bug.status = false;
		}
		just_pressed_select_all = false;
		saved_current_file = false;
	}

	ImGui::Separator();
	ImGui::EndChild();
}

void BugManager::RegisterNewBug()
{	// Open a modal window to add a new bug
	ImGui::OpenPopup("Register Bug");
	if (ImGui::BeginPopupModal("Register Bug", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::PushFont(bold_font);
		ImGui::Text("Enter Title");
		ImGui::Separator();
		ImGui::PopFont();

		static char title[128] = "";
		bool enter_pressed = ImGui::InputText("##title", title, IM_ARRAYSIZE(title), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll);

		if (ImGui::Button("Register Bug") || enter_pressed)
		{
			if (strlen(title) != 0)
			{
				BugModel bug = CreateNewBug(GetTotalBugs() + 1, title, false);
				bug.status = false;
				PushBackBug(&bug);
				std::memset(title, 0, sizeof(title));
				open_new_bug_modal = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			std::memset(title, 0, sizeof(title));
			open_new_bug_modal = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void BugManager::RemoveResolvedBugs()
{
	bugs.erase(std::remove_if(bugs.begin(), bugs.end(), [](const BugModel& bug) { return bug.status; }), bugs.end());
	select_all = false;
	just_pressed_select_all = false;
}

std::vector<BugModel> BugManager::GetAllBugs()
{
	return bugs;
}

size_t BugManager::GetTotalBugs()
{
	return bugs.size();
}

BugModel BugManager::CreateNewBug(size_t uid, std::string title, bool status)
{
	BugModel bug(uid, title, status);
	return bug;
}

void BugManager::PushBackBug(BugModel* bug)
{
	this->bugs.push_back(*bug);
}

bool BugManager::BugAlreadyExists(BugModel* bug)
{
	for (auto b : GetAllBugs())
	{
		if (bug->title == b.title)
		{
			return true;
		}
	}

	return false;
}