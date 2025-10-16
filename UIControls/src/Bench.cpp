﻿#include "Bench.h"

void Bench::initial(void){
    setBGColor(INITIAL_BG_COLOR);
    setBorderColor(INITIAL_BORDER_COLOR);
    setTransparent(false);

    m_isInitialed = true;
    SDL_Log("Loading finished, waiting user starting game................................");
}

Bench::Bench(Control *parent, SRect rect, SDL_Renderer *renderer, float xScale, float yScale):
    TopControl(),
    Panel(parent, rect, xScale, yScale),
    m_isLoading(true),
    m_isInitialed(false),
    m_nextTick(0),
    m_nextRepeatTick(0),
    m_isExiting(SDL_APP_CONTINUE)
{
    setRenderer(renderer);
    setTransparent(true);

    SDL_Log("Loading resources.....................................");
    // 将资源加载到内存中
    // ResourceLoader::getInstance()->loadConfig();
}

void Bench::inputControl(shared_ptr<Event> event) {
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

void Bench::repeatTrigger(void){
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
void Bench::update() {
    if (!m_enable) return;

    if (m_lastAction != nullptr){
        repeatTrigger();
    }
    Panel::update();
}
void Bench::draw(void){
    if (!m_visible) return;
    // 绘制子控件
    Panel::draw();

    drawCenteredRectangle(getRenderer(), (int)m_rect.width, (int)m_rect.height);
}

SDL_AppResult Bench::isExiting(void) {
    return m_isExiting;
}


// Draw a centered rectangle
void Bench::drawCenteredRectangle(SDL_Renderer* renderer, int windowWidth, int windowHeight) {
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
