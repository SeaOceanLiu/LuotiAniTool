﻿#ifndef BenchH
#define BenchH
#include "MainWindow.h"
#include "ResourceLoader.h"
#include "Panel.h"
#include "Label.h"
#include "Dialog.h"
#include "LuotiAni.h"

#define BENCH (Bench::getInstance())

class Bench: public Panel, public TopControl
{
private:
    bool m_isLoading;
    bool m_isInitialed;
    SRect m_defaultArenaRect;
    SRect m_defaultBGRect;

    float m_N; // multiple of Arena
    float m_M; // multiple of BG

    uint64_t m_nextTick;
    unordered_map<EventName, uint64_t> m_eventJitter; // jitter for each event
    uint64_t m_nextRepeatTick;
    shared_ptr<Event> m_lastAction;
    SDL_AppResult m_isExiting;

    Bench(Control *parent, SRect rect, SDL_Renderer *renderer, float xScale=1.0f, float yScale=1.0f);

public:
    static Bench* getInstance(void){
        static Bench instance = Bench(nullptr, {0, 0, INITIAL_WIDTH, INITIAL_HEIGHT}, GET_RENDERER); // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }
    void initial(void);
    void inputControl(shared_ptr<Event> event);
    void repeatTrigger(void);
    void draw(void) override;
    void update(void) override;
    SDL_AppResult isExiting(void);

    void drawCenteredRectangle(SDL_Renderer* renderer, int windowWidth, int windowHeight);
};
#endif // BenchH