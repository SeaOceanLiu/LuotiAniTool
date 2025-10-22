#include <filesystem>
#include <SDL3/SDL.h>

#include "ConstDef.h"

// 路径定义
const fs::path ConstDef::pathPrefix = fs::path(std::string(SDL_GetBasePath()) + "assets");
const fs::path ConstDef::workforldPath = ConstDef::pathPrefix;

// 按钮相关常量
const float ConstDef::BUTTON_CAPTION_SIZE = 16;
const SDL_Color ConstDef::BUTTON_NORMAL_COLOR = { 70, 130, 180, 255 };  // Steel Blue
const SDL_Color ConstDef::BUTTON_HOVER_COLOR = { 100, 149, 237, 255 };  // Cornflower Blue
const SDL_Color ConstDef::BUTTON_DOWN_COLOR = { 30, 144, 255, 255 };    // Dodger Blue

const SDL_Color ConstDef::BUTTON_NORMAL_TEXT_COLOR = { 0, 0, 0, 255 };  // 黑色
const SDL_Color ConstDef::BUTTON_HOVER_TEXT_COLOR = { 128, 128, 128, 255 };   // 灰色
const SDL_Color ConstDef::BUTTON_DOWN_TEXT_COLOR  = { 200, 200, 200, 255 }; // 深灰色,

const float ConstDef::MENU_BAR_HEIGHT = 52.0f;
const SDL_Color ConstDef::MENU_BORDER_COLOR = { 83, 83, 90, 255 }; // 边框颜色
const SDL_Color ConstDef::MENU_NORMAL_COLOR = { 23, 23, 24, 255 };  // 默认背景色
const SDL_Color ConstDef::MENU_HOVER_COLOR = { 51, 65, 85, 255 };   // 默认背景色
const SDL_Color ConstDef::MENU_DOWN_COLOR = { 51, 65, 85, 255 };    // 默认背景色
const SDL_Color ConstDef::MENU_TEXT_NORMAL_COLOR = {203, 213, 225, 255};    // 默认字体颜色
const SDL_Color ConstDef::MENU_TEXT_HOVER_COLOR = { 203, 213, 225, 255 };   // 默认字体颜色
const SDL_Color ConstDef::MENU_TEXT_DOWN_COLOR = { 203, 213, 225, 255 };    // 默认字体颜色
const float ConstDef::MENU_MAIN_MENU_HEIGHT = 34.0f;
const float ConstDef::MENU_TEXT_SIZE = 20.0f;
const float ConstDef::MENU_CAPTION_MARGIN = 20.0f;  // 菜单标题边距，左右各10，共20

const float ConstDef::WINDOW_TITLE_HEIGHT = 30;
const float ConstDef::FONT_MARGIN = 10;