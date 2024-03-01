#include "Globals.h"
#include "BugManager.h"
#include "save_load.h"

BugManager bug_manager;
SaveLoad save_load;

bool settings_open = false;
void SettingsModal()
{
	if (settings_open)
	{
		ImGui::OpenPopup("Settings");
		if (ImGui::BeginPopupModal("Settings", &settings_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			char save_path[256] = "";
			ImGui::SeparatorText("Save Path");
			if (ImGui::InputTextWithHint("##save_path", save_load.GetSavePath().c_str(), save_path, 256))
			{
				save_load.SetSavePath(save_path);
			}

			ImGui::SeparatorText("Font Size");
			float font_size = ImGui::GetFontSize();
			if (ImGui::RadioButton("Small", font_size == 18.0f))
			{
				ImGui::GetIO().FontGlobalScale = 0.75f;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Large", font_size == 24.0f))
			{
				ImGui::GetIO().FontGlobalScale = 1.0f;
			}

			if (ImGui::Button("Close"))
			{
				settings_open = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
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
			if (ImGui::MenuItem("Save") and bug_manager.GetTotalBugs() > 0)
			{
				save_load.Save(bug_manager.GetAllBugs(), "save.txt");
			}
			if (ImGui::MenuItem("Load"))
			{
				std::string load_file_path = save_load.GetSavePath() + "/" + "save.txt";
				bool loaded = save_load.Load(load_file_path, &bug_manager);
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
	}

	const char* remove_button_label = "Remove Resolved Bugs";
	ImGui::SameLine(available_space - ImGui::CalcTextSize(remove_button_label).x);
	if (ImGui::Button(remove_button_label))
	{
		bug_manager.RemoveResolvedBugs();
	}

	bug_manager.DrawBugsList();
}

int WinMain()
{
	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(FPS);

	// Initialize ImGui
	ImGui::SFML::Init(window);

	// Set ImGui style to match the background color
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = APP_BG_COLOR;

	// Load a font and set font size
	default_font = MakeNewFont(FONT_PATH "Default.ttf");
	default_small_font = MakeNewFont(FONT_PATH "Default.ttf", 16.0f);
	italic_font = MakeNewFont(FONT_PATH "Italic.ttf");
	bold_font = MakeNewFont(FONT_PATH "Bold.ttf");
	bold_italic_font = MakeNewFont(FONT_PATH "BoldItalic.ttf");

	std::ifstream load_file("save.txt");
	//save_load.Load(load_file, bug_manager.GetAllBugs());
	load_file.close();
	// Main loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// Pass SFML events to ImGui
			ImGui::SFML::ProcessEvent(event);
		}

		// Start the ImGui frame
		ImGui::SFML::Update(window, sf::seconds(1.0f / 60.0f));

		// Set the ImGui window size to match the SFML window size
		ImGui::SetNextWindowSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });

		// Center the ImGui window by setting the position to (0, 0) and using ImGuiWindowFlags_AlwaysAutoResize
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		// Set ImGui window flags to remove borders and title bar
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar;

		// Begin ImGui window
		ImGui::Begin("Bug Tracker", nullptr, windowFlags);
		ImGui::PushFont(default_font);
		ImGui::Text(APP_VERSION);
		// Show total bugs count in same line
		ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Bugs: 999").x);
		ImGui::Text("Bugs: %d", bug_manager.GetTotalBugs());

		//TODO: CHECK IF SAVE FILE EXISTS THEN LOAD IT, ELSE CREATE NEW
		MainDashboard();
		ImGui::PopFont();
		ImGui::End();

		// Render ImGui
		window.clear(sf::Color(15, 15, 15));
		ImGui::SFML::Render(window);
		window.display();
	}

	// Shutdown ImGui
	if (bug_manager.GetTotalBugs() > 0) {
		// Check if file already exists, then ask if you want to overwrite the existing file or make new file.
		// if new file then ask for file name, default can be save1.txt, save2.txt etc
		save_load.Save(bug_manager.GetAllBugs(), "save.txt");
	}
	ImGui::SFML::Shutdown();

	return 0;
}