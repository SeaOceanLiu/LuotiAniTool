#pragma once

#include "ControlBase.h"
#include "Panel.h"
#include "Label.h"
#include "Button.h"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <functional>

using namespace std;

// 菜单项类型枚举
enum class MenuItemType {
    Normal,     // 普通菜单项
    Separator,  // 分隔符
    SubMenu     // 子菜单项
};

// 前向声明
class MenuItem;
class MainMenu;
class MenuBar;

// MenuItem类定义 - 从Button继承
class MenuItem : public Button {
public:
    using OnClickHandler = function<void(shared_ptr<MenuItem>)>;

    MenuItem(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;

    void setType(MenuItemType type);
    void setShortcut(string shortcut);
    void setOnClick(OnClickHandler onClick);

    void addSubMenuItem(shared_ptr<MenuItem> menuItem);
    void removeSubMenuItem(shared_ptr<MenuItem> menuItem);
    void showSubMenu(void);
    void hideSubMenu(void);
    bool isSubMenuVisible(void);

private:
    void updateSubMenuPosition(void);

    string m_shortcut;
    MenuItemType m_type;
    bool m_hasSubMenu;
    OnClickHandler m_onClick;
    shared_ptr<Panel> m_subMenuPanel;
    vector<shared_ptr<MenuItem>> m_subMenuItems;
};

// MenuItemBuilder类定义
class MenuItemBuilder {
public:
    MenuItemBuilder(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    MenuItemBuilder& setCaption(string caption);
    MenuItemBuilder& setShortcut(string shortcut);
    MenuItemBuilder& setType(MenuItemType type);
    MenuItemBuilder& setNormalStateBGColor(SDL_Color color);
    MenuItemBuilder& setHoverStateBGColor(SDL_Color color);
    MenuItemBuilder& setPressedStateBGColor(SDL_Color color);
    MenuItemBuilder& setTextNormalStateColor(SDL_Color color);
    MenuItemBuilder& setTextHoverStateColor(SDL_Color color);
    MenuItemBuilder& setTextPressedStateColor(SDL_Color color);
    MenuItemBuilder& setOnClick(MenuItem::OnClickHandler onClick);
    MenuItemBuilder& addSubMenuItem(shared_ptr<MenuItem> menuItem);

    shared_ptr<MenuItem> build(void);

private:
    shared_ptr<MenuItem> m_menuItem;
};

// MainMenu类定义 - 从Button继承
class MainMenu : public Button {
public:
    // using OnClickHandler = function<void(shared_ptr<MainMenu>)>;

    MainMenu(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;
    // void setRect(SRect rect) override;

    // void setOnClick(OnClickHandler onClick);

    void addMenuItem(shared_ptr<MenuItem> menuItem);
    void removeMenuItem(shared_ptr<MenuItem> menuItem);
    void showSubMenu(shared_ptr<Button> menu);
    bool isSubMenuVisible(void);

private:
    float m_maxSubMenuWidth;
    shared_ptr<Panel> m_subMenuPanel;
    vector<shared_ptr<MenuItem>> m_menuItems;
};

// MainMenuBuilder类定义
class MainMenuBuilder {
public:
    MainMenuBuilder(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    MainMenuBuilder& setCaption(string caption);
    MainMenuBuilder& setNormalStateBGColor(SDL_Color color);
    MainMenuBuilder& setHoverStateBGColor(SDL_Color color);
    MainMenuBuilder& setPressedStateBGColor(SDL_Color color);
    MainMenuBuilder& setTextNormalStateColor(SDL_Color color);
    MainMenuBuilder& setTextHoverStateColor(SDL_Color color);
    MainMenuBuilder& setOnClick(Button::OnClickHandler onClick);
    MainMenuBuilder& addMenuItem(shared_ptr<MenuItem> menuItem);

    shared_ptr<MainMenu> build(void);

private:
    shared_ptr<MainMenu> m_mainMenu;
};

// MenuBar类定义
class MenuBar : public Panel {
public:
    MenuBar(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;

    void addMenu(shared_ptr<MainMenu> menu);
    void removeMenu(shared_ptr<MainMenu> menu);
    // void setBGColor(SDL_Color color);
    void setHeight(float height);
    // void setActiveMenu(shared_ptr<MainMenu> menu);
    // void closeAllMenus(void);

private:
    // SDL_Color m_bgColor;
    SDL_Color m_textColor;
    float m_height;
    shared_ptr<MainMenu> m_activeMenu;
    vector<shared_ptr<MainMenu>> m_menus;
};

// MenuBarBuilder类定义
class MenuBarBuilder {
public:
    MenuBarBuilder(Control *parent, SRect rect, float xScale = 1.0f, float yScale = 1.0f);

    // MenuBarBuilder& setBGColor(SDL_Color color);
    MenuBarBuilder& setHeight(float height);
    MenuBarBuilder& addMenu(shared_ptr<MainMenu> menu);

    shared_ptr<MenuBar> build(void);

private:
    shared_ptr<MenuBar> m_menuBar;
};
