#ifndef ConstDefH
#define ConstDefH

#include <filesystem>

// 前向声明
struct SDL_Color;

// ********************************在这里定义创建主窗体的初始参数********************************
#define APP_NAME u8"LuotiAniTool"
#define APP_VERSION "1.0.0"
#define APP_AUTHOR "SeaOcean"
#define APP_COPYRIGHT "Copyright (c) 2023 SeaOcean"
#define APP_COMPANY "SeaOcean.Ltd."
#define APP_IDENTIFY "com.seaocean.luotianitool"
// 主窗体横向像素个数
#define INITIAL_WIDTH  800
// 主窗体纵向像素个数
#define INITIAL_HEIGHT 600
// 主窗体初始位置X坐标，SDL_WINDOWPOS_CENTERED表示居中
#define INITIAL_POSX SDL_WINDOWPOS_CENTERED
// 主窗体初始位置Y坐标，SDL_WINDOWPOS_CENTERED表示居中
#define INITIAL_POSY SDL_WINDOWPOS_CENTERED
#define INITIAL_BG_COLOR SDL_Color{255, 255, 255, 255} // 白色
#define INITIAL_BORDER_COLOR SDL_Color{0, 0, 0, 255}   // 黑色
// 在这里定义窗口特性标志
#define WINDOW_FLAG (SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)
// 是否开启垂直同步，0表示不启用垂直同步，1表示启用垂直同步
#define VSYNC_FLAG (0)

// **********************************************************************************************
#define DEFAULT_BTN_MS_INTERVAL (200)       // 默认按钮事件间隔时间，单位毫秒
#define DEFAULT_BTN_REPEAT_MS_INTERVAL (50) // 默认按钮重复事件间隔时间，单位毫秒
#define DEFAULT_BTN_MS_REPEAT (50)          // 按钮自动重复的时间间隔，单位毫秒

class ConstDef {
public:
    // 路径定义
    static const std::filesystem::path workforldPath;  // 工作目录路径
    static const std::filesystem::path pathPrefix;     // 资源路径前缀

    // 按钮相关常量
    static const float BUTTON_CAPTION_SIZE;
    static const SDL_Color BUTTON_NORMAL_COLOR;
    static const SDL_Color BUTTON_HOVER_COLOR;
    static const SDL_Color BUTTON_DOWN_COLOR;

    // 窗口相关常量
    static const float WINDOW_TITLE_HEIGHT;

    // 字体相关常量
    static const float FONT_MARGIN;
};

#endif // ConstDefH