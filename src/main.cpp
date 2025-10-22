#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include "DebugTrace.h"
#include "MainWindow.h"
#include "Bench.h"

void debugTraceOutput(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    DEBUG_PRINT("Category[%02d], priority[%02d]: %s", category, priority, message);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    DEBUG_STREAM << "Using SDL3 library" << std::endl;

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE); //设置日志级别
    SDL_SetLogOutputFunction(debugTraceOutput, nullptr);

   // 禁止触摸事件转换为鼠标事件，避免一次触摸同时产生一欠触摸和一欠鼠标事件
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    DEBUG_STREAM << "SDL_TOUCH_MOUSE_EVENTS = " << SDL_GetHint(SDL_HINT_TOUCH_MOUSE_EVENTS) << std::endl;

    SDL_SetAppMetadata(APP_NAME, APP_VERSION, APP_IDENTIFY);

    // Initialize SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        DEBUG_STREAM << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // MainWindow::getInstance()必须在SDL_Init之后//，TTF_Init之前调用
    SSize displaySize = MAINWIN->getDisplaySize();
    BENCH;    // 初始化Bench单例，创建Bench工作台，Todo: 从配置文件中读取窗体配置

    DEBUG_STREAM << "SDL3 Resizable Window Example" << std::endl;
    DEBUG_STREAM << "Instructions:" << std::endl;
    DEBUG_STREAM << "- Drag window edges to resize" << std::endl;
    DEBUG_STREAM << "- Press ESC key or close window to exit" << std::endl;
    DEBUG_STREAM << "Initial window size: " << MAINWIN->getWindowSize().width << " x " << MAINWIN->getWindowSize().height << std::endl;

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    shared_ptr<Event> gameEvent = nullptr;
    shared_ptr<SPoint> pos;
    int targetPosX = 0;
    int targetPosY = 0;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */

        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_UP:
        // case SDL_EVENT_FINGER_MOTION:
        // case SDL_EVENT_FINGER_CANCELED:
            targetPosX = event->tfinger.x * MAINWIN->getDisplayWidth();
            targetPosY = event->tfinger.y * MAINWIN->getDisplayHeight();

            // 转为FINGER_DOWN等位置事件
            switch(event->type)
            {
                case SDL_EVENT_FINGER_DOWN:
                    pos = make_shared<SPoint>(targetPosX, targetPosY);
                    gameEvent = make_shared<Event>(EventName::FINGER_DOWN, pos);
                    break;
                case SDL_EVENT_FINGER_UP:
                    pos = make_shared<SPoint>(targetPosX, targetPosY);
                    gameEvent = make_shared<Event>(EventName::FINGER_UP, pos);
                    break;
                case SDL_EVENT_FINGER_MOTION:
                    pos = make_shared<SPoint>(targetPosX, targetPosY);
                    gameEvent = make_shared<Event>(EventName::FINGER_MOTION, pos);
                    break;
            }
            if (gameEvent != nullptr) {
                BENCH->inputControl(gameEvent);
            }
        case SDL_EVENT_MOUSE_MOTION:        /**< Mouse moved */
            pos = make_shared<SPoint>(event->motion.x, event->motion.y);
            gameEvent = make_shared<Event>(EventName::MOUSE_MOVING, pos);
            BENCH->inputControl(gameEvent);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:   /**< Mouse button pressed */
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_DOWN @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_DOWN, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                BENCH->inputControl(gameEvent);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:     /**< Mouse button released */
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_UP @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_UP, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                BENCH->inputControl(gameEvent);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            BENCH->inputControl(gameEvent);
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            MainWindow::handleWindowEvent(event->window);
            BENCH->resized({0, 0, (float)event->window.data1, (float)event->window.data2});
            break;

        case SDL_EVENT_WINDOW_MOVED:
            MainWindow::handleWindowEvent(event->window);
            break;
    }

    // Display current window size (print at most once per second)
    static int lastPrintTime = 0;
    int currentTime = SDL_GetTicks();
    if (currentTime - lastPrintTime > 1000) {
        DEBUG_STREAM << "Current window size: " << MainWindow::getInstance()->getWindowSize().width << " x " << MainWindow::getInstance()->getWindowSize().height << std::endl;
        lastPrintTime = currentTime;
    }
    return SDL_APP_CONTINUE;
}
/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    BENCH->eventLoopEntry();
    BENCH->update();

    /* clear the window to the draw color. */
    SDL_SetRenderDrawColor(MainWindow::getInstance()->getRenderer(), 255, 0, 0, 255);
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
    // 这里要强制释放资源，因为要确保在后面调用TTF_Quit()之前，要把FontSuite打开的字体都关闭掉
    // BENCH.reset();
    // 线程需要显式detach，否则Android下会报泄漏
    ResourceLoader::getInstance()->detachLoadingThread();
    TTF_Quit();
    /* SDL will clean up the window/renderer for us. */
}