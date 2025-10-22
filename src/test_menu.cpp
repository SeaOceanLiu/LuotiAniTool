#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>
#include "../UIControls/include/Menu.h"
#include "../UIControls/include/MainWindow.h"
#include "../UIControls/include/Bench.h"

using namespace std;

// 全局变量存储菜单控件
shared_ptr<MenuBar> g_menuBar;
shared_ptr<MainMenu> g_fileMenu;
shared_ptr<MainMenu> g_editMenu;

void debugTraceOutput(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    cout << "Category[" << category << "], priority[" << priority << "]: " << message << endl;
}
void testBenchInitialize(void) {
    // 创建菜单栏
    g_menuBar = MenuBarBuilder(nullptr, SRect{0, 0, 1920, 52})
                // .setBGColor(SDL_Color{23, 23, 24, 255})  // 灰色背景
                // .setTextColor(SDL_Color{255, 255, 255, 255})  // 白色文字
                .build();

    // 创建文件菜单
    g_fileMenu = MainMenuBuilder(g_menuBar.get(), SRect{0, 0, 80, 52})
                .setCaption(u8"文件(F)")
                // .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})  // 灰色背景
                // .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})  // 白色文字
                // .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})  // 浅灰色悬停
                .build();

    // 创建文件菜单项
    auto newFileItem = MenuItemBuilder(nullptr, SRect{0, 0, 200, 30})
                        .setCaption(u8"新建(N)")
                        .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                        .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                        .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                        .setOnClick([](shared_ptr<MenuItem> item) {
                            cout << "New file clicked" << endl;
                        })
                        .build();

    auto openFileItem = MenuItemBuilder(nullptr, SRect{0, 30, 200, 30})
                        .setCaption(u8"打开(O)")
                        .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                        .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                        .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                        .setOnClick([](shared_ptr<MenuItem> item) {
                            cout << "Open file clicked" << endl;
                        })
                        .build();

    auto saveFileItem = MenuItemBuilder(nullptr, SRect{0, 60, 200, 30})
                        .setCaption(u8"保存(S)")
                        .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                        .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                        .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                        .setOnClick([](shared_ptr<MenuItem> item) {
                            cout << "Save file clicked" << endl;
                        })
                        .build();

    // 添加分隔符
    auto separatorItem = MenuItemBuilder(nullptr, SRect{0, 90, 200, 5})
                        .setType(MenuItemType::Separator)
                        .build();

    auto exitItem = MenuItemBuilder(nullptr, SRect{0, 95, 200, 30})
                    .setCaption(u8"退出")
                    .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                    .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                    .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                    .setOnClick([](shared_ptr<MenuItem> item) {
                        cout << "Exit program" << endl;
                        SDL_Event quitEvent;
                        quitEvent.type = SDL_EVENT_QUIT;
                        SDL_PushEvent(&quitEvent);
                    })
                    .build();

    // 将菜单项添加到文件菜单
    g_fileMenu->addMenuItem(newFileItem);
    g_fileMenu->addMenuItem(openFileItem);
    g_fileMenu->addMenuItem(saveFileItem);
    g_fileMenu->addMenuItem(separatorItem);
    g_fileMenu->addMenuItem(exitItem);

    // 创建编辑菜单
    g_editMenu = MainMenuBuilder(g_menuBar.get(), SRect{80, 0, 80, 52})
                .setCaption(u8"编辑(E)")
                // .setNormalStateBGColor(SDL_Color{128, 128, 128, 255})  // 灰色背景
                // .setTextNormalStateColor(SDL_Color{255, 255, 255, 255})  // 白色文字
                // .setHoverStateBGColor(SDL_Color{200, 200, 200, 255})  // 浅灰色悬停
                .build();

    // 创建编辑菜单项
    auto undoItem = MenuItemBuilder(nullptr, SRect{0, 0, 200, 30})
                    .setCaption("Undo")
                    .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                    .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                    .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                    .setOnClick([](shared_ptr<MenuItem> item) {
                        cout << "Undo operation" << endl;
                    })
                    .build();

    auto redoItem = MenuItemBuilder(nullptr, SRect{0, 30, 200, 30})
                    .setCaption("Redo")
                    .setNormalStateBGColor(SDL_Color{23, 23, 24, 255})
                    .setTextNormalStateColor(SDL_Color{203, 213, 225, 255})
                    .setHoverStateBGColor(SDL_Color{51, 65, 85, 255})
                    .setOnClick([](shared_ptr<MenuItem> item) {
                        cout << "Redo operation" << endl;
                    })
                    .build();

    // 将菜单项添加到编辑菜单
    g_editMenu->addMenuItem(undoItem);
    g_editMenu->addMenuItem(redoItem);

    // 将菜单添加到菜单栏
    g_menuBar->addMenu(g_fileMenu);
    g_menuBar->addMenu(g_editMenu);

    // 将菜单栏添加到Bench控件管理器
    BENCH->addControl(g_menuBar);

    // // 调试信息
    // cout << "MenuBar created: " << (g_menuBar ? "Yes" : "No") << endl;
    // cout << "File menu created: " << (g_fileMenu ? "Yes" : "No") << endl;
    // cout << "Edit menu created: " << (g_editMenu ? "Yes" : "No") << endl;
    // cout << "MenuBar enabled: " << (g_menuBar->getEnable() ? "Yes" : "No") << endl;
    // cout << "File menu enabled: " << (g_fileMenu->getEnable() ? "Yes" : "No") << endl;
    // cout << "Edit menu enabled: " << (g_editMenu->getEnable() ? "Yes" : "No") << endl;

    // cout << "Menu controls test program started successfully" << endl;
    // cout << "Click File menu to see dropdown menu" << endl;
    // cout << "Click Exit menu item to exit program" << endl;
    // cout << "Press ESC to exit program" << endl;
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    cout << "Using SDL3 library for menu test" << endl;

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_SetLogOutputFunction(debugTraceOutput, nullptr);

    // 禁止触摸事件转换为鼠标事件
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    cout << "SDL_TOUCH_MOUSE_EVENTS = " << SDL_GetHint(SDL_HINT_TOUCH_MOUSE_EVENTS) << endl;

    SDL_SetAppMetadata("MenuTest", "1.0.0", "com.example.menutest");

    // Initialize SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cout << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // 初始化主窗口和Bench
    SSize displaySize = MAINWIN->getDisplaySize();
    BENCH->setOnInitial(testBenchInitialize);

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    shared_ptr<Event> gameEvent = nullptr;
    shared_ptr<SPoint> pos;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_MOUSE_MOTION:
            SDL_Log("Pushing mouse button moving event to Bench");
            pos = make_shared<SPoint>(event->motion.x, event->motion.y);
            gameEvent = make_shared<Event>(EventName::MOUSE_MOVING, pos);
            BENCH->inputControl(gameEvent);
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            cout << "Mouse button down @ {" << event->button.x << ", " << event->button.y << "}" << endl;
            switch(event->button.button){
                case SDL_BUTTON_LEFT:
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_RIGHT:
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_MIDDLE:
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_DOWN, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                SDL_Log("Pushing mouse button down event to Bench");
                BENCH->inputControl(gameEvent);
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            cout << "Mouse button up @ {" << event->button.x << ", " << event->button.y << "}" << endl;
            switch(event->button.button){
                case SDL_BUTTON_LEFT:
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_RIGHT:
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_MIDDLE:
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_UP, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                SDL_Log("Pushing mouse button up event to Bench");
                BENCH->inputControl(gameEvent);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE) {
                SDL_Event quitEvent;
                quitEvent.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quitEvent);
            }
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            MainWindow::handleWindowEvent(event->window);
            BENCH->resized({0, 0, (float)event->window.data1, (float)event->window.data2});
            break;

        case SDL_EVENT_WINDOW_MOVED:
            MainWindow::handleWindowEvent(event->window);
            break;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    BENCH->eventLoopEntry();
    BENCH->update();

    /* clear the window to the draw color. */
    SDL_SetRenderDrawColor(MainWindow::getInstance()->getRenderer(), 40, 40, 40, 255);  // 深灰色背景
    SDL_RenderClear(MainWindow::getInstance()->getRenderer());

    BENCH->draw();

    // Present rendering
    SDL_RenderPresent(GET_RENDERER);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // Clean up resources
    // 线程需要显式detach，否则Android下会报泄漏
    ResourceLoader::getInstance()->detachLoadingThread();
    TTF_Quit();
    cout << "Menu test program exiting" << endl;
}
