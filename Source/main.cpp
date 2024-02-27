#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "BugModel.h"

// TODO: TODO
//! Important
//? Bugs/Fix Me
//x Not Working

int main() {
	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Bug Tracker");
	window.setFramerateLimit(60);

	// Initialize ImGui
	ImGui::SFML::Init(window);

	// Set ImGui style to match the background color
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.0f);

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
			ImGuiWindowFlags_NoDecoration;

		// Begin ImGui window
		ImGui::Begin("ImGui Window", nullptr, windowFlags);
		MainDashboard();
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