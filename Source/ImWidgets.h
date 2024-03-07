#pragma once
#ifndef IMWIDHETS_H
#define IMWIDHETS_H
#include <imgui.h>
#include <vector>

enum ImMessageBoxFlags
{
	ImMessageBoxFlags_OKButton = 1 << 0,
	ImMessageBoxFlags_YesNoButton = 1 << 1,
	ImMessageBoxFlags_YesNoCancelButton = 1 << 2
};
enum ImMessageBoxResult
{
	ImMessageBoxResult_OK,
	ImMessageBoxResult_Yes,
	ImMessageBoxResult_No,
	ImMessageBoxResult_Cancel,
	ImMessageBoxResult_None
};
enum ImNotificationFlags
{
	ImNotificationFlags_TopLeft = 1 << 0,
	ImNotificationFlags_TopRight = 1 << 1,
	ImNotificationFlags_BottomLeft = 1 << 2,
	ImNotificationFlags_BottomRight = 1 << 3
};

namespace ImWidgets
{
	struct Notification {
		const char* message;
		float lifetime;
		float creation_time;
		ImNotificationFlags flags;
	};

	//static std::vector<Notification> notifications = {};

	/// <summary>
	/// Creates a simple message box to the center of the screen.
	/// </summary>
	/// <param name="message">: Message to be displayed.</param>
	/// <param name="title">: Title of the message box.</param>
	/// <param name="open">: Condition checker, for opening the message box.</param>
	/// <param name="flags">: Flags for the buttons of the message box.</param>
	/// <returns></returns>
	static ImMessageBoxResult MessageBox(const char* message, const char* title, bool* open, ImMessageBoxFlags flags = ImMessageBoxFlags_OKButton)
	{
		if (*open)
		{
			ImMessageBoxResult result = ImMessageBoxResult_None;
			int width = 400;

			ImGui::SetNextWindowSize(ImVec2(width, 0));

			ImGui::OpenPopup(title);

			if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::NewLine();
				ImGui::TextWrapped(message);
				ImGui::NewLine();
				ImGui::Separator();

				if (flags & ImMessageBoxFlags_OKButton)
				{
					if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						result = ImMessageBoxResult_OK;
						*open = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SetItemDefaultFocus();
				}

				if (flags & ImMessageBoxFlags_YesNoButton)
				{
					if (ImGui::Button("Yes", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, 0)))
					{
						result = ImMessageBoxResult_Yes;
						*open = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("No", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						result = ImMessageBoxResult_No;
						*open = false;
						ImGui::CloseCurrentPopup();
					}
				}

				if (flags & ImMessageBoxFlags_YesNoCancelButton)
				{
					if (ImGui::Button("Yes", ImVec2(ImGui::GetContentRegionAvail().x / 3, 0)))
					{
						result = ImMessageBoxResult_Yes;
						*open = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("No", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
					{
						result = ImMessageBoxResult_No;
						*open = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						result = ImMessageBoxResult_Cancel;
						*open = false;
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			return result;
		}
		else
		{
			return ImMessageBoxResult_None;
		}
	}

	/// <summary>
	/// Creates a notification with a message and a lifetime.
	/// Call this function only after a condition has been met.
	/// </summary>
	/// <param name="message">: The notification to be displayed,</param>
	/// <param name="life_time">: Life time of the notification, after which it will be removed.</param>
	/// <param name="flags">: Flags for the position of the notification.</param>
	static void CreateNotification(const char* message, std::vector<Notification>* notifications, int life_time = 3, ImNotificationFlags flags = ImNotificationFlags_TopRight)
	{
		Notification notification;
		notification.message = message;
		notification.lifetime = life_time;
		notification.creation_time = ImGui::GetTime();
		notification.flags = flags;
		notifications->push_back(notification);
	}

	/// <summary>
	/// Renders every notification in the notification list and removes them if their lifetime has expired.
	/// Call this function just before ImGui::End() in your main loop.
	/// </summary>
	static void RenderNotifications(std::vector<Notification>* notifications) {
		float current_time = ImGui::GetTime();
		const float fade_in = 0.3f;
		const float fade_out = 0.3f;
		float vertical_offset = 0.0f;
		bool force_remove = false;

		for (auto it = notifications->begin(); it != notifications->end();) {
			Notification& notification = *it;

			ImVec2 position;
			if (notification.flags & ImNotificationFlags_TopLeft)
				position = ImVec2(10, 10);
			else if (notification.flags & ImNotificationFlags_TopRight)
				position = ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize(notification.message).x - 30, 10);
			else if (notification.flags & ImNotificationFlags_BottomLeft)
				position = ImVec2(10, ImGui::GetIO().DisplaySize.y - ImGui::CalcTextSize(notification.message).y - 30);
			else if (notification.flags & ImNotificationFlags_BottomRight)
				position = ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize(notification.message).x - 30, ImGui::GetIO().DisplaySize.y - ImGui::CalcTextSize(notification.message).y - 30);

			float alpha = 1.0f;
			float elapsed_time = current_time - notification.creation_time;
			if (elapsed_time < fade_in) {
				alpha = elapsed_time / fade_in;
			}
			else if (elapsed_time > notification.lifetime - fade_out) {
				alpha = (notification.lifetime - elapsed_time) / fade_out;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

			ImGui::SetNextWindowPos(position, ImGuiCond_Always, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
			ImGui::Text("%s", notification.message);
			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				force_remove = true;
				// TODO: Add a callback function here if required
			}
			ImGui::End();

			ImGui::PopStyleVar();

			vertical_offset += ImGui::GetTextLineHeightWithSpacing();

			if (notification.lifetime > 0.0f && elapsed_time > notification.lifetime || force_remove) {
				it = notifications->erase(it);
			}
			else {
				++it;
			}
		}
	}
};
#endif // !IMWIDHETS_H
