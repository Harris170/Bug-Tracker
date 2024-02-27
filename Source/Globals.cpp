#include "Globals.h"

extern ImFont* default_font = 0;
extern ImFont* italic_font = 0;
extern ImFont* bold_font = 0;
extern ImFont* bold_italic_font = 0;
extern bool open_new_bug_modal = false;

ImFont* MakeNewFont(std::string path) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(path.c_str(), FONT_SIZE);
	ImGui::SFML::UpdateFontTexture();
	return io.Fonts->Fonts.back();
}

void PushFontBold()
{
	ImGui::PushFont(bold_font);
}