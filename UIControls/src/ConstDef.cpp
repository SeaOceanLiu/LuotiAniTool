#include <filesystem>
#include <SDL3/SDL.h>

#include "ConstDef.h"

// 路径定义
const std::filesystem::path ConstDef::workforldPath = std::filesystem::path(".");  // 工作目录路径
const std::filesystem::path ConstDef::pathPrefix = std::filesystem::path(".");     // 资源路径前缀

// 按钮相关常量
const float ConstDef::BUTTON_CAPTION_SIZE = 16;
const SDL_Color ConstDef::BUTTON_NORMAL_COLOR = { 70, 130, 180, 255 };  // Steel Blue
const SDL_Color ConstDef::BUTTON_HOVER_COLOR = { 100, 149, 237, 255 };  // Cornflower Blue
const SDL_Color ConstDef::BUTTON_DOWN_COLOR = { 30, 144, 255, 255 };    // Dodger Blue
const float ConstDef::WINDOW_TITLE_HEIGHT = 30;
const float ConstDef::FONT_MARGIN = 10;