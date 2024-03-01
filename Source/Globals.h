#ifndef GLOBALS_H
#define GLOBALS_H
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

extern ImFont* default_font;
extern ImFont* default_small_font;
extern ImFont* italic_font;
extern ImFont* bold_font;
extern ImFont* bold_italic_font;
extern ImVec4 background_color;
extern ImVec4 default_color;
extern ImVec4 dimmed_color;

extern bool open_new_bug_modal;
extern bool saved_current_file;

#define FONT_LARGE 24.0f
#define FONT_SMALL 18.0f
#define FONT_PATH "Assets/CourierPrime/"
#define APP_NAME "Bug Tracker"
#define APP_VERSION "v0.8"
#define APP_WIDTH 900
#define APP_HEIGHT 600
#define APP_BG_COLOR ImVec4(0.45f, 0.55f, 0.60f, 1.00f)
#define DIMMED_COLOR ImVec4(0.3f, 0.3f, 0.3f, 1.00f)
#define FPS 60

extern ImFont* MakeNewFont(std::string path, float size = FONT_LARGE);
extern void SetBackgroundColor(ImVec4 color);
extern void SetDefaultColor(ImVec4 color);
extern void SetDimmedColor(ImVec4 color);
#endif