#include "Globals.h"
#include "BugManager.h"

BugManager bug_manager;

void MainDashboard() {
	float buttonWidth = ImGui::GetContentRegionAvail().x;

	if (ImGui::Button("Register New Bug"))
	{
		open_new_bug_modal = true;
	}

	if (open_new_bug_modal)
	{
		bug_manager.RegisterNewBug();
	}

	const char* remove_button_label = "Remove Resolved Bugs";
	ImGui::SameLine(buttonWidth - ImGui::CalcTextSize(remove_button_label).x);
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
	italic_font = MakeNewFont(FONT_PATH "Italic.ttf");
	bold_font = MakeNewFont(FONT_PATH "Bold.ttf");
	bold_italic_font = MakeNewFont(FONT_PATH "BoldItalic.ttf");

	// Main loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				//TODO: SAVE FUNCTIONALITY
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
			ImGuiWindowFlags_NoDecoration;

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
	ImGui::SFML::Shutdown();

	return 0;
}