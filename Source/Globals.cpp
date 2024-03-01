#include "Globals.h"

extern ImFont* default_font = 0;
extern ImFont* default_small_font = 0;
extern ImFont* italic_font = 0;
extern ImFont* bold_font = 0;
extern ImFont* bold_italic_font = 0;
extern ImVec4 background_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
extern ImVec4 default_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
extern ImVec4 dimmed_color = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
extern bool open_new_bug_modal = false;
extern bool saved_current_file = false;

ImFont* MakeNewFont(std::string path, float font_size) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(path.c_str(), font_size);
	ImGui::SFML::UpdateFontTexture();
	return io.Fonts->Fonts.back();
}

void SetBackgroundColor(ImVec4 color)
{
	background_color = color;
}

void SetDefaultColor(ImVec4 color)
{
	default_color = color;
}

void SetDimmedColor(ImVec4 color)
{
	dimmed_color = color;
}