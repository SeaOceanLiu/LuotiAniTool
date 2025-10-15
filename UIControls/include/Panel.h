#ifndef PanelH
#define PanelH

#include <functional>
#include <vector>
#include <unordered_map>
#include "Actor.h"

using namespace std;

class Panel: public ControlImpl {
    friend class PanelBuilder;
    using OnClickHandler = std::function<void (shared_ptr<Control>)>;
private:
    SDL_Color m_bgColor;
    SDL_Color m_borderColor;
    bool m_isTransparent;
    bool m_isBorderVisible;

    unordered_map<shared_ptr<Actor>, SPoint> m_actors;
public:
    Panel(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;

    void setBGColor(SDL_Color color);
    SDL_Color getBGColor(void) { return m_bgColor; }
    void setBorderColor(SDL_Color color);
    SDL_Color getBorderColor(void) { return m_borderColor; }
    void setTransparent(bool isTransparent);
    bool isTransparent(void) { return m_isTransparent; }
    void addControl(shared_ptr<Control> control) override;;
};
class PanelBuilder {
private:
    shared_ptr<Panel> m_panel;
public:
    PanelBuilder(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    PanelBuilder& setBGColor(SDL_Color color);
    PanelBuilder& setBorderColor(SDL_Color color);
    PanelBuilder& setTransparent(bool isTransparent);
    PanelBuilder& setBorderVisible(bool isBorderVisible);
    PanelBuilder& addControl(shared_ptr<Control> control);
    shared_ptr<Panel> build(void);
};
#endif