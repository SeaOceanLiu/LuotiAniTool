#include "ConstDef.h"
#include <filesystem>

// 路径定义
const std::filesystem::path ConstDef::workforldPath = std::filesystem::path(".");  // 工作目录路径
const std::filesystem::path ConstDef::pathPrefix = std::filesystem::path(".");     // 资源路径前缀

// 按钮相关常量
const float ConstDef::BUTTON_CAPTION_SIZE = 16;
const float ConstDef::WINDOW_TITLE_HEIGHT = 30;
const float ConstDef::FONT_MARGIN = 10;