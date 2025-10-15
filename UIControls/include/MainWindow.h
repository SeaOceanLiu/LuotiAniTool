﻿#ifndef MainWindowH
#define MainWindowH
#include "Utility.h"
#include <SDL3/SDL.h>
#include "Panel.h"

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
#define MAINWIN (MainWindow::getInstance())
#define GET_RENDERER (MAINWIN->getRenderer())

#define DEFAULT_BTN_MS_INTERVAL (200)       // 默认按钮事件间隔时间，单位毫秒
#define DEFAULT_BTN_REPEAT_MS_INTERVAL (50) // 默认按钮重复事件间隔时间，单位毫秒
#define DEFAULT_BTN_MS_REPEAT (50)          // 按钮自动重复的时间间隔，单位毫秒

class MainWindow: public Panel, public TopControl {
private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SSize m_size;
    SPoint m_pos;

    SDL_DisplayID m_displayId;
    float m_displayWidth;
    float m_displayHeight;

    uint64_t m_nextTick;
    uint64_t m_nextRepeatTick;
    shared_ptr<Event> m_lastAction;
    unordered_map<EventName, Uint64> m_eventJitter; // jitter for each event

    MainWindow():
        TopControl(),
        Panel(nullptr, {0, 0, INITIAL_WIDTH, INITIAL_HEIGHT}, 1.0f, 1.0f),
        m_window(nullptr),
        m_renderer(nullptr),
        m_size({INITIAL_WIDTH, INITIAL_HEIGHT}),
        m_pos({INITIAL_POSX, INITIAL_POSY})
    {
        // 创建SDL3窗口和渲染器
        if (!SDL_CreateWindowAndRenderer(APP_NAME, INITIAL_WIDTH, INITIAL_HEIGHT, WINDOW_FLAG, &m_window, &m_renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            throw ("Couldn't create window/renderer");
        }

        //设置窗口位置
        if(!SDL_SetWindowPosition(m_window, INITIAL_POSX, INITIAL_POSY)){
            SDL_Log("Couldn't set window position: %s", SDL_GetError());
        }

        // 是否开启垂直同步
        if(!SDL_SetRenderVSync(m_renderer, VSYNC_FLAG)){
            SDL_Log("Couldn't set vsync: %s", SDL_GetError());
        }

        // 获取显示器信息
        SDL_DisplayID m_displayId = SDL_GetPrimaryDisplay();
        if (m_displayId == 0) {
            SDL_Log("SDL_GetPrimaryDisplay Error: %s\n", SDL_GetError());
            throw ("SDL_GetPrimaryDisplay Error");
        }

        const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(m_displayId);
        if (displayMode == nullptr) {
            SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
            throw ("SDL_GetCurrentDisplayMode Error");
        }
        m_displayWidth = (float)displayMode->w * displayMode->pixel_density;
        m_displayHeight = (float)displayMode->h * displayMode->pixel_density;

            // Get window size
        int windowWidth = INITIAL_WIDTH;
        int windowHeight = INITIAL_HEIGHT;
        if(!SDL_GetWindowSize(m_window, &windowWidth, &windowHeight)){
            DEBUG_STREAM << "Couldn't get window size: " << SDL_GetError() << std::endl;
        }
        m_size = SSize{(float)windowWidth, (float)windowHeight};

        setBGColor(INITIAL_BG_COLOR);
        setBorderColor(INITIAL_BORDER_COLOR);
    }
public:
    static MainWindow* getInstance(void){
        static MainWindow instance; // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }

     // Window resize event callback function
    static void handleWindowEvent(const SDL_WindowEvent& windowEvent) {
        switch (windowEvent.type) {
            case SDL_EVENT_WINDOW_RESIZED:
                DEBUG_STREAM << "Window size changed: " << windowEvent.data1 << " x " << windowEvent.data2 << std::endl;
                MainWindow::getInstance()->m_size = SSize{(float)windowEvent.data1, (float)windowEvent.data2};
                break;
            case SDL_EVENT_WINDOW_MOVED:
                DEBUG_STREAM << "Window position changed: (" << windowEvent.data1 << ", " << windowEvent.data2 << ")" << std::endl;
                MainWindow::getInstance()->m_pos = SPoint{(float)windowEvent.data1, (float)windowEvent.data2};
                break;
            default:
                break;
        }
    }

    SDL_Window* getWindow(void) { return m_window; }
    SDL_Renderer* getRenderer(void) { return m_renderer; }
    SSize getWindowSize(void) { return m_size; }
    SPoint getWindowPos(void) { return m_pos; }
    // SDL_PixelFormat getPixelFormat(void) { return m_pixelFormat; }
    float getDisplayWidth(void) { return m_displayWidth; }
    float getDisplayHeight(void) { return m_displayHeight; }
    SSize getDisplaySize(void) { return SSize{m_displayWidth, m_displayHeight}; }

    void inputControl(shared_ptr<Event> event) {
        if (m_eventJitter.find(event->m_eventName) != m_eventJitter.end()){
            if (SDL_GetTicks() < m_eventJitter[event->m_eventName]){
                return;
            }
            m_eventJitter[event->m_eventName] = SDL_GetTicks() + DEFAULT_BTN_MS_INTERVAL;
        }

        if(EventQueue::isPositionEvent(event->m_eventName)){
            m_lastAction = event;
            m_nextTick = SDL_GetTicks() + DEFAULT_BTN_MS_INTERVAL;
        }

        triggerEvent(event);
    }
    void repeatTrigger(void){
    if (m_lastAction != nullptr){
        uint64_t currentTick = SDL_GetTicks();
        if (currentTick < m_nextRepeatTick || currentTick < m_eventJitter[m_lastAction->m_eventName]){
            return;
        }

        switch(m_lastAction->m_eventName){
            case EventName::FINGER_DOWN:
            case EventName::FINGER_MOTION:
            case EventName::MOUSE_LBUTTON_DOWN:
            case EventName::MOUSE_MBUTTON_DOWN:
            case EventName::MOUSE_RBUTTON_DOWN:
                triggerEvent(m_lastAction);
                break;
            default:
                break;
        }
        m_nextRepeatTick = SDL_GetTicks() + DEFAULT_BTN_MS_REPEAT;
    }
}
    void update(void) override {
        if (m_lastAction != nullptr){
            repeatTrigger();
        }
        Panel::update();
    }
    void draw(void) override {
        SDL_Color bgColor = getBGColor();
        SDL_SetRenderDrawColor(GET_RENDERER, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(GET_RENDERER);

        // Draw centered rectangle
        drawCenteredRectangle(GET_RENDERER, getWindowSize().width, getWindowSize().height);
    }
// Draw a centered rectangle
    void drawCenteredRectangle(SDL_Renderer* renderer, int windowWidth, int windowHeight) {
        // Calculate rectangle size (60% of window size)
        int rectWidth = static_cast<int>(windowWidth * 0.6f);
        int rectHeight = static_cast<int>(windowHeight * 0.6f);

        // Calculate rectangle position (centered)
        int rectX = (windowWidth - rectWidth) / 2;
        int rectY = (windowHeight - rectHeight) / 2;

        // Set draw color to dark blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255); // Dark blue

        // Draw filled rectangle
        SDL_FRect rect = { static_cast<float>(rectX), static_cast<float>(rectY),
                            static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
        SDL_RenderFillRect(renderer, &rect);

        // Set border color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

        // Draw rectangle border
        SDL_RenderRect(renderer, &rect);
    }
};
#endif // MainWindowH
