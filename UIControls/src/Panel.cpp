#include "Panel.h"

Panel::Panel(Control *parent, SRect rect, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_bgColor(INITIAL_BG_COLOR),
    m_borderColor(INITIAL_BORDER_COLOR),
    m_isTransparent(false),
    m_isBorderVisible(true)
{
    m_rect = rect;
}

void Panel::update(void){
    if (!m_enable) return;

    // 如果有子控件，这里需要更新子控件
    ControlImpl::update();
}
void Panel::draw(void){
    if (!m_visible) return;

    SRect drawRect = getDrawRect();

    // 1. 先绘制当前控件的外观
    if(!m_isTransparent) {
        if(!SDL_SetRenderDrawColor(getRenderer(), m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a)){
            SDL_Log("Panel failed to set background color: %s", SDL_GetError());
        }
        if (!SDL_RenderFillRect(getRenderer(), drawRect.toSDLFRect())){
            SDL_Log("Panel failed to fill render rect: %s", SDL_GetError());
        }
    }

    // 2. 接着绘制子控件
    ControlImpl::draw();

    // 5. 最后绘制边框
    if(!m_isTransparent && m_isBorderVisible) {
        if(!SDL_SetRenderDrawColor(getRenderer(), m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a)){
            SDL_Log("Panel fFailed to set border color: %s", SDL_GetError());
        }
        if(!SDL_RenderRect(getRenderer(), drawRect.toSDLFRect())){
            SDL_Log("Panel failed to draw border: %s", SDL_GetError());
        }
    }
}

bool Panel::handleEvent(shared_ptr<Event> event){
    return ControlImpl::handleEvent(event);

}


void Panel::setBGColor(SDL_Color color){
    m_bgColor = color;
}
void Panel::setBorderColor(SDL_Color color)
{
    m_borderColor = color;
}
void Panel::setTransparent(bool isTransparent)
{
    m_isTransparent = isTransparent;
}
void Panel::addControl(shared_ptr<Control> control){
    ControlImpl::addControl(control);
}

// *********************************************************************************************
PanelBuilder::PanelBuilder(Control *parent, SRect rect, float xScale, float yScale):
    m_panel(nullptr)
{
    m_panel = make_shared<Panel>(parent, rect, xScale, yScale);
}
PanelBuilder& PanelBuilder::setBGColor(SDL_Color color){
    m_panel->setBGColor(color);
    return *this;
}
PanelBuilder& PanelBuilder::setBorderColor(SDL_Color color){
    m_panel->setBorderColor(color);
    return *this;
}
PanelBuilder& PanelBuilder::setTransparent(bool isTransparent){
    m_panel->setTransparent(isTransparent);
    return *this;
}
PanelBuilder& PanelBuilder::setBorderVisible(bool isBorderVisible){
    m_panel->m_isBorderVisible = isBorderVisible;
    return *this;
}
PanelBuilder& PanelBuilder::addControl(shared_ptr<Control> control){
    m_panel->addControl(control);
    return *this;
}
shared_ptr<Panel> PanelBuilder::build(void){
    return m_panel;
}