#include "Globals.h"

extern ImFont* default_font = 0;
extern ImFont* default_small_font = 0;
extern ImFont* italic_font = 0;
extern ImFont* bold_font = 0;
extern ImFont* bold_italic_font = 0;
extern bool open_new_bug_modal = false;

ImFont* MakeNewFont(std::string path, float font_size) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(path.c_str(), font_size);
	ImGui::SFML::UpdateFontTexture();
	return io.Fonts->Fonts.back();
}