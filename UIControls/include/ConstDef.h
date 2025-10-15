#ifndef ConstDefH
#define ConstDefH

#include <filesystem>

// 前向声明
struct SDL_Color;

class ConstDef {
public:
    // 路径定义
    static const std::filesystem::path workforldPath;  // 工作目录路径
    static const std::filesystem::path pathPrefix;     // 资源路径前缀

    // 按钮相关常量
    static const int BUTTON_CAPTION_SIZE;
    static const SDL_Color BUTTON_NORMAL_COLOR;
    static const SDL_Color BUTTON_HOVER_COLOR;
    static const SDL_Color BUTTON_DOWN_COLOR;

    // 窗口相关常量
    static const int WINDOW_TITLE_HEIGHT;

    // 字体相关常量
    static const int FONT_MARGIN;
};

#endif // ConstDefH