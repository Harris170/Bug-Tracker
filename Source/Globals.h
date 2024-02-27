#ifndef GLOBALS_H
#define GLOBALS_H
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

extern ImFont* default_font;
extern ImFont* italic_font;
extern ImFont* bold_font;
extern ImFont* bold_italic_font;

extern bool open_new_bug_modal;

#define FONT_SIZE 24.0f
#define FONT_PATH "Assets/CourierPrime/"
#define APP_NAME "Bug Tracker"
#define APP_VERSION "v0.5"
#define APP_WIDTH 900
#define APP_HEIGHT 600
#define APP_BG_COLOR ImVec4(0.45f, 0.55f, 0.60f, 1.00f)
#define FPS 60

extern ImFont* MakeNewFont(std::string path);
extern void PushFontBold();
#endif