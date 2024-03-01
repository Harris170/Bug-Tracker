#include "Globals.h"
#include "BugManager.h"
#include "SaveLoad.h"

BugManager bug_manager;
SaveLoad save_load;

bool settings_open = false;
bool save_as_open = false;
bool open_file_open = false;
bool confirm_quit_open = false;
bool first_time_saving = true;
bool force_quit = false;

// Set the window and frame sizes
ImVec2 size = ImVec2(10, 10);
int rounding = 2;
ImVec2 align = ImVec2(0.5f, 0.5f);
ImVec2 spacing = ImVec2(10, 4);

void SettingsModal()
{
	if (settings_open)
	{
		ImGui::OpenPopup("Settings");
		if (ImGui::BeginPopupModal("Settings", &settings_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			char save_path[256] = "";
			ImGui::SeparatorText("Change Default Save Path");
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputTextWithHint("##save_path", save_load.GetSavePath().c_str(), save_path, sizeof(save_path)))
			{
				save_load.SetSavePath(save_path);
			}
			ImGui::NewLine();

			ImGui::SeparatorText("Font Size");
			float font_size = ImGui::GetFontSize();
			if (ImGui::RadioButton("Small", font_size == FONT_SMALL))
			{
				ImGui::GetIO().FontGlobalScale = 0.75f;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Large", font_size == FONT_LARGE))
			{
				ImGui::GetIO().FontGlobalScale = 1.0f;
			}
			ImGui::NewLine();

			ImGui::SeparatorText("Colors");
			ImGui::ColorEdit3("Active Color", (float*)&default_color);
			ImGui::ColorEdit3("Inactive Color", (float*)&dimmed_color);
			ImGui::NewLine();

			float button_width = ImGui::GetContentRegionAvail().x / 4;
			if (ImGui::Button("Close", ImVec2(button_width, 0)))
			{
				settings_open = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();

			if (ImGui::Button("Reset", ImVec2(button_width, 0)))
			{
				default_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
				dimmed_color = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
				background_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
			}
			ImGui::EndPopup();
		}
	}
}

void SaveAsModal()
{
	if (save_as_open)
	{
		ImGui::OpenPopup("Save As");
		if (ImGui::BeginPopupModal("Save As", &save_as_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			char save_file_name[256] = "";
			ImGui::SeparatorText("File Name");
			ImGui::SetNextItemWidth(ImGui::CalcTextSize(save_load.GetSavePath().c_str()).x + 20);
			if (ImGui::InputTextWithHint("##save_file_name", "Save", save_file_name, sizeof(save_file_name), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::string save_file_without_extension = save_file_name;
				strcat_s(save_file_name, ".txt");
				if (save_load.Save(bug_manager.GetAllBugs(), save_file_name))
				{
					save_load.SetCurrentFile(save_file_without_extension);
					save_load.has_saved_file = true;
					first_time_saving = false;
				}
				save_as_open = false;
			}
			ImGui::NewLine();

			char save_path[256] = "";
			ImGui::SeparatorText("Save Path");
			ImGui::SetNextItemWidth(ImGui::CalcTextSize(save_load.GetSavePath().c_str()).x + 20);
			if (ImGui::InputTextWithHint("##save_path", save_load.GetSavePath().c_str(), save_path, sizeof(save_path)))
			{
				save_load.SetSavePath(save_path);
			}

			ImGui::NewLine();
			float button_width = ImGui::GetContentRegionAvail().x;

			if (ImGui::Button("Close", ImVec2(button_width, 0)))
			{
				save_as_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void OpenFileModal()
{
	if (open_file_open)
	{
		ImGui::OpenPopup("Open File");
		if (ImGui::BeginPopupModal("Open File", &open_file_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			char load_file_name[256] = "";
			ImGui::SeparatorText("File Name");
			ImGui::SetNextItemWidth(ImGui::CalcTextSize(save_load.GetSavePath().c_str()).x + 20);
			if (ImGui::InputTextWithHint("##open_file_name", "Save", load_file_name, sizeof(load_file_name), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::string load_file_without_extension = load_file_name;
				strcat_s(load_file_name, ".txt");
				if (save_load.Load(save_load.GetSavePath() + "/" + load_file_name, &bug_manager))
				{
					save_load.SetCurrentFile(load_file_without_extension);
					save_load.has_saved_file = true;
				}

				open_file_open = false;
			}
			ImGui::NewLine();

			char save_path[256] = "";
			ImGui::SeparatorText("Save Path");
			ImGui::SetNextItemWidth(ImGui::CalcTextSize(save_load.GetSavePath().c_str()).x + 20);
			if (ImGui::InputTextWithHint("##save_path", save_load.GetSavePath().c_str(), save_path, sizeof(save_path)))
			{
				save_load.SetSavePath(save_path);
			}

			ImGui::NewLine();
			float button_width = ImGui::GetContentRegionAvail().x;
			if (ImGui::Button("Close", ImVec2(button_width, 0)))
			{
				open_file_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void ConfirmQuitModal()
{
	if (confirm_quit_open)
	{
		ImGui::OpenPopup("Confirm Quit", ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginPopupModal("Confirm Quit", &confirm_quit_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Text("You have unsaved changes. Save before quitting?");
			ImGui::NewLine();

			float button_width = ImGui::GetContentRegionAvail().x / 3;
			if (ImGui::Button("Save", ImVec2(button_width, 0)))
			{
				save_as_open = true;
				confirm_quit_open = false;
			}
			ImGui::SameLine();

			if (ImGui::Button("Quit", ImVec2(button_width, 0)))
			{
				confirm_quit_open = false;
				force_quit = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(button_width, 0)))
			{
				confirm_quit_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void MainDashboard()
{
	// Add Edit and Settings buttons in the menu bar of the main window
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushFont(default_small_font);
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				if (first_time_saving)
				{
					save_as_open = true;
				}
				else
				{
					bool saved = save_load.Save(bug_manager.GetAllBugs(), save_load.GetCurrentFile() + ".txt");
					if (saved)
					{
						save_load.has_saved_file = true;
					}
				}
				//TODO: NOTIFY THE USER WHETHER THE FILE HAS BEEN SAVED OR NOT , WITH THE FILE PATH.
			}

			if (ImGui::MenuItem("Save As"))
			{
				save_as_open = true;
			}

			if (ImGui::MenuItem("Open"))
			{
				open_file_open = true;
				first_time_saving = false;
				//TODO: IF FILE LOADED OR NOT, DISPLAY NOTIFICATION TO USER
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			settings_open = true;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
		ImGui::PopFont();
	}

	SettingsModal();
	SaveAsModal();
	OpenFileModal();
	ConfirmQuitModal();

	ImGui::NewLine();
	ImGui::Separator();

	float available_space = ImGui::GetContentRegionAvail().x;

	if (ImGui::Button("Register New Bug"))
	{
		open_new_bug_modal = true;
	}

	if (open_new_bug_modal)
	{
		bug_manager.RegisterNewBug();
		save_load.has_saved_file = false;
	}

	const char* remove_button_label = "Remove Resolved Bugs";
	ImGui::SameLine(available_space - ImGui::CalcTextSize(remove_button_label).x);
	if (ImGui::Button(remove_button_label))
	{
		bug_manager.RemoveResolvedBugs();
		save_load.has_saved_file = false;
		//TODO: NOTIFY THAT THE BUGS HAVE BEEN REMOVED
	}

	bug_manager.DrawBugsList();
}

int WinMain()
{
	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_NAME);
	window.setFramerateLimit(FPS);

	// Initialize ImGui
	ImGui::SFML::Init(window);

	// Set ImGui style to match the background color
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = background_color;

	// Set colors of buttons, checkboxes, etc.
	ImVec4 normal_color = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
	ImVec4 hover_color = ImVec4(0.2f, 0.2f, 0.2f, 0.95f);

	style.Colors[ImGuiCol_Button] = normal_color;
	style.Colors[ImGuiCol_ButtonHovered] = hover_color;
	style.Colors[ImGuiCol_ButtonActive] = hover_color;

	style.Colors[ImGuiCol_CheckMark] = dimmed_color;

	style.Colors[ImGuiCol_FrameBg] = normal_color;
	style.Colors[ImGuiCol_FrameBgHovered] = hover_color;
	style.Colors[ImGuiCol_FrameBgActive] = hover_color;

	style.Colors[ImGuiCol_SliderGrab] = default_color;
	style.Colors[ImGuiCol_SliderGrabActive] = default_color;

	style.Colors[ImGuiCol_ScrollbarBg] = normal_color;
	style.Colors[ImGuiCol_ScrollbarGrab] = default_color;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = hover_color;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = hover_color;

	style.Colors[ImGuiCol_TitleBg] = background_color;
	style.Colors[ImGuiCol_TitleBgActive] = background_color;
	style.Colors[ImGuiCol_TitleBgCollapsed] = background_color;

	style.Colors[ImGuiCol_Header] = normal_color;
	style.Colors[ImGuiCol_HeaderHovered] = hover_color;
	style.Colors[ImGuiCol_HeaderActive] = hover_color;

	style.SeparatorTextBorderSize = 2;

	style.FramePadding = size;
	style.ScrollbarSize = size.x;
	style.GrabMinSize = size.x;

	style.FramePadding = size;
	style.WindowPadding = size;

	style.SeparatorTextAlign = align;
	style.WindowTitleAlign = align;
	style.ButtonTextAlign = align;
	style.SelectableTextAlign = align;

	style.ItemInnerSpacing = size;

	style.WindowRounding = rounding;
	style.ScrollbarRounding = rounding;
	style.GrabRounding = rounding;
	style.FrameRounding = rounding;
	style.ChildRounding = rounding;
	style.PopupRounding = rounding;

	// Load a font and set font size
	default_font = MakeNewFont(FONT_PATH "Default.ttf");
	default_small_font = MakeNewFont(FONT_PATH "Default.ttf", 18.0f);
	italic_font = MakeNewFont(FONT_PATH "Italic.ttf");
	bold_font = MakeNewFont(FONT_PATH "Bold.ttf");
	bold_italic_font = MakeNewFont(FONT_PATH "BoldItalic.ttf");

	// Preloads the bugs if the file exits.
	std::string load_file = save_load.GetSavePath() + "/" + save_load.GetCurrentFile();
	save_load.Load(load_file, &bug_manager);

	while (window.isOpen()) {
		ImGui::PushStyleColor(ImGuiCol_Text, default_color);
		if (force_quit) window.close();

		if (save_load.has_saved_file)
		{
			window.setTitle(APP_NAME + (std::string)" - " + save_load.GetCurrentFile());
		}
		else
		{
			window.setTitle(APP_NAME + (std::string)" - " + save_load.GetCurrentFile() + " *");
		}
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				if (not save_load.has_saved_file and bug_manager.GetTotalBugs() > 0)
				{
					confirm_quit_open = true;
				}
				else
				{
					window.close();
				}
			}

			ImGui::SFML::ProcessEvent(event);
		}

		// Start the ImGui frame
		ImGui::SFML::Update(window, sf::seconds(1.0f / 60.0f));

		// Set the ImGui window size to match the SFML window size
		ImGui::SetNextWindowSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		// Set ImGui window flags to remove borders and title bar
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar;

		// Begin ImGui window
		ImGui::Begin("Bug Tracker", nullptr, windowFlags);
		ImGui::PushFont(default_font);

		ImGui::PushStyleColor(ImGuiCol_Text, dimmed_color);
		ImGui::Text(APP_VERSION);
		ImGui::PopStyleColor();

		// Show total bugs count in same line
		ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Bugs: 999").x);
		ImGui::Text("Bugs: %d", bug_manager.GetTotalBugs());

		MainDashboard();
		ImGui::PopFont();
		ImGui::End();

		// Render ImGui
		window.clear(sf::Color(15, 15, 15));
		ImGui::SFML::Render(window);
		window.display();
	}

	// Check if file already exists, then ask if you want to overwrite the existing file or make new file.
	// if new file then ask for file name, default can be save1.txt, save2.txt etc
	//save_load.Save(bug_manager.GetAllBugs(), "save.txt");

	ImGui::PopStyleColor();
	// Shutdown ImGui
	ImGui::SFML::Shutdown();

	return 0;
}