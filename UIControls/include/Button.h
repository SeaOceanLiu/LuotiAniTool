#ifndef ButtonH
#define ButtonH
#include <functional>
#include "ConstDef.h"
#include "Actor.h"
#include "Label.h"
#include "LuotiAni.h"

enum class ButtonState {
    Normal,
    Hover,
    Pressed
};

class Button: public ControlImpl {
    friend class ButtonBuilder;
public:
    using OnClickHandler = std::function<void (shared_ptr<Button>)>;
private:
    shared_ptr<Actor> m_actor;
    shared_ptr<Actor> m_hoverActor;
    shared_ptr<Actor> m_pressedActor;
    SDL_Color m_normalBGColor;
    SDL_Color m_hoverBGColor;
    SDL_Color m_pressedBGColor;
    SDL_Color m_normalTextColor;
    SDL_Color m_hoverTextColor;
    SDL_Color m_pressedTextColor;

    shared_ptr<Label> m_caption;
    shared_ptr<LuotiAni>m_luotiAni;

    string m_captionText;
    float m_captionSize;

    OnClickHandler m_onClick;
    ButtonState m_state;
    bool m_isTransparent;
public:
    Button(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;
    void setRect(SRect rect) override;

    void setBtnNormalStateActor(shared_ptr<Actor> actor);
    void setBtnHoverStateActor(shared_ptr<Actor> actor);
    void setBtnPressedStateActor(shared_ptr<Actor> actor);
    void setNormalStateBGColor(SDL_Color color);
    void setHoverStateBGColor(SDL_Color color);
    void setPressedStateBGColor(SDL_Color color);
    // 暴露Caption相关设置接口
    void setTextNormalStateColor(SDL_Color color);
    void setTextHoverStateColor(SDL_Color color);
    void setTextPressedStateColor(SDL_Color color);
    void setCaption(string caption);
    string getCaption(void) const;
    void setCaptionSize(float size);
    uint32_t getCaptionSize(float size) const;
    SRect getCaptionRect(void) const;

    void setLuotiAni(shared_ptr<LuotiAni>luotiAni);
    void setOnClick(OnClickHandler onClick);
    void setTransparent(bool isTransparent);
    void setState(ButtonState state);
};

class ButtonBuilder {
private:
    shared_ptr<Button> m_button;
public:
    ButtonBuilder(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    ButtonBuilder& setBtnNormalStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setBtnHoverStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setBtnPressedStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setNormalStateBGColor(SDL_Color color);
    ButtonBuilder& setHoverStateBGColor(SDL_Color color);
    ButtonBuilder& setPressedStateBGColor(SDL_Color color);
    ButtonBuilder& setTextNormalStateColor(SDL_Color color);
    ButtonBuilder& setTextHoverStateColor(SDL_Color color);
    ButtonBuilder& setTextPressedStateColor(SDL_Color color);
    ButtonBuilder& setCaption(string caption);
    ButtonBuilder& setCaptionSize(float size);
    ButtonBuilder& setLuotiAni(shared_ptr<LuotiAni> luotiAni);
    ButtonBuilder& addControl(shared_ptr<Control> child);
    ButtonBuilder& setOnClick(Button::OnClickHandler onClick);
    ButtonBuilder& setTransparent(bool isTransparent);
    ButtonBuilder& setId(int id);
    shared_ptr<Button> build(void);
};
#endif
