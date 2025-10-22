#include "Menu.h"
#include "Label.h"
#include <SDL3/SDL.h>

// ============================================================================
// MenuItem 实现
// ============================================================================

MenuItem::MenuItem(Control *parent, SRect rect, float xScale, float yScale)
    : Button(parent, rect, xScale, yScale)
{
    m_shortcut = "";
    m_type = MenuItemType::Normal;
    m_hasSubMenu = false;
    m_onClick = nullptr;
}

void MenuItem::update(void)
{
    // 更新子菜单面板位置
    if (m_subMenuPanel && m_subMenuPanel->getEnable()) {
        updateSubMenuPosition();
    }

    // 更新子控件
    Button::update();
}

void MenuItem::draw(void)
{
    if (!m_visible) return;

    // 分隔符处理
    if (m_type == MenuItemType::Separator) {
        SDL_Renderer* renderer = getRenderer();
        if (!renderer) return;

        SRect drawRect = getDrawRect();

        // 绘制分隔线
        SDL_SetRenderDrawColor(renderer, 68, 68, 75, 255);  // 分隔线颜色
        SDL_FRect lineRect = {drawRect.left, drawRect.top + drawRect.height/2,
                             drawRect.width, 1};
        SDL_RenderFillRect(renderer, &lineRect);
        return;
    }

    // 普通菜单项使用Button的绘制逻辑
    Button::draw();
}

bool MenuItem::handleEvent(shared_ptr<Event> event)
{
    if (!m_enable || !m_visible) return false;

    // 分隔符不处理事件
    if (m_type == MenuItemType::Separator) {
        return false;
    }

    // 处理位置事件
    if (EventQueue::isPositionEvent(event->m_eventName)) {
        shared_ptr<SPoint> pos = any_cast<shared_ptr<SPoint>>(event->m_eventParam);
        SRect drawRect = getDrawRect(); // 获取当前控件的绘制区域

        if (drawRect.contains(pos->x, pos->y)) {
            // 鼠标在菜单项内
            switch(event->m_eventName) {
                case EventName::MOUSE_MOVING:
                    if (m_hasSubMenu) {
                        showSubMenu();
                    }
                    break;
                case EventName::MOUSE_LBUTTON_DOWN:
                    if (m_onClick != nullptr) {
                        m_onClick(dynamic_pointer_cast<MenuItem>(this->getThis()));
                    }
                    if (m_hasSubMenu) {
                        showSubMenu();
                    }
                    return true;
                default:
                    break;
            }
            return true;
        } else {
            if (!m_hasSubMenu) {
                hideSubMenu();
            }
        }
    }

    return Button::handleEvent(event);
}

void MenuItem::setType(MenuItemType type)
{
    m_type = type;
}

void MenuItem::setShortcut(string shortcut)
{
    m_shortcut = shortcut;
}

void MenuItem::setOnClick(OnClickHandler onClick)
{
    m_onClick = onClick;
}

void MenuItem::addSubMenuItem(shared_ptr<MenuItem> menuItem)
{
    m_subMenuItems.push_back(menuItem);
    m_hasSubMenu = true;
    if (m_subMenuPanel) {
        m_subMenuPanel->addControl(menuItem);
    }
}

void MenuItem::removeSubMenuItem(shared_ptr<MenuItem> menuItem)
{
    auto it = std::find(m_subMenuItems.begin(), m_subMenuItems.end(), menuItem);
    if (it != m_subMenuItems.end()) {
        m_subMenuItems.erase(it);
        if (m_subMenuPanel) {
            m_subMenuPanel->removeControl(menuItem);
        }
    }
    m_hasSubMenu = !m_subMenuItems.empty();
}

void MenuItem::showSubMenu(void)
{
    if (!m_subMenuPanel && m_hasSubMenu) {
        // 创建子菜单面板
        m_subMenuPanel = PanelBuilder(getParent(), SRect{0, 0, 200, 300})
                            .setBGColor(SDL_Color{23, 23, 24, 255})
                            .setBorderColor(SDL_Color{68, 68, 75, 255})
                            .setTransparent(false)
                            .build();

        // 添加子菜单项到面板
        for (auto& menuItem : m_subMenuItems) {
            m_subMenuPanel->addControl(menuItem);
        }

        // 将面板添加到父控件
        getParent()->addControl(m_subMenuPanel);
    }

    if (m_subMenuPanel) {
        updateSubMenuPosition();
        m_subMenuPanel->show();
    }
}

void MenuItem::hideSubMenu(void)
{
    if (m_subMenuPanel) {
        m_subMenuPanel->hide();
    }
}

bool MenuItem::isSubMenuVisible(void)
{
    return m_subMenuPanel && m_subMenuPanel->getEnable() && m_subMenuPanel->getRect().height > 0;
}

void MenuItem::updateSubMenuPosition(void)
{
    if (m_subMenuPanel) {
        SRect menuRect = getRect();
        SRect parentRect = getParent()->getRect();

        // 计算子菜单位置（在菜单项右侧）
        float subMenuX = menuRect.left + menuRect.width;
        float subMenuY = menuRect.top;

        // 确保子菜单不会超出父控件边界
        if (subMenuX + m_subMenuPanel->getRect().width > parentRect.width) {
            subMenuX = menuRect.left - m_subMenuPanel->getRect().width;
        }

        if (subMenuY + m_subMenuPanel->getRect().height > parentRect.height) {
            subMenuY = parentRect.height - m_subMenuPanel->getRect().height;
        }

        m_subMenuPanel->setRect(SRect{subMenuX, subMenuY,
                                     m_subMenuPanel->getRect().width,
                                     m_subMenuPanel->getRect().height});
    }
}

// MenuItemBuilder 实现
MenuItemBuilder::MenuItemBuilder(Control *parent, SRect rect, float xScale, float yScale)
{
    m_menuItem = make_shared<MenuItem>(parent, rect, xScale, yScale);
}

MenuItemBuilder& MenuItemBuilder::setCaption(string caption)
{
    m_menuItem->setCaption(caption);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setShortcut(string shortcut)
{
    m_menuItem->setShortcut(shortcut);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setType(MenuItemType type)
{
    m_menuItem->setType(type);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setNormalStateBGColor(SDL_Color color)
{
    m_menuItem->setNormalStateBGColor(color);
    return *this;
}
MenuItemBuilder& MenuItemBuilder::setHoverStateBGColor(SDL_Color color)
{
    m_menuItem->setHoverStateBGColor(color);
    return *this;
}
MenuItemBuilder& MenuItemBuilder::setPressedStateBGColor(SDL_Color color){
    m_menuItem->setPressedStateBGColor(color);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setTextNormalStateColor(SDL_Color color)
{
    m_menuItem->setTextNormalStateColor(color);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setTextHoverStateColor(SDL_Color color)
{
    m_menuItem->setTextHoverStateColor(color);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setTextPressedStateColor(SDL_Color color)
{
    m_menuItem->setTextPressedStateColor(color);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::setOnClick(MenuItem::OnClickHandler onClick)
{
    m_menuItem->setOnClick(onClick);
    return *this;
}

MenuItemBuilder& MenuItemBuilder::addSubMenuItem(shared_ptr<MenuItem> menuItem)
{
    m_menuItem->addSubMenuItem(menuItem);
    return *this;
}

shared_ptr<MenuItem> MenuItemBuilder::build(void)
{
    return m_menuItem;
}

// ============================================================================
// MainMenu 实现
// ============================================================================

MainMenu::MainMenu(Control *parent, SRect rect, float xScale, float yScale)
    : Button(parent, rect, xScale, yScale),
    m_maxSubMenuWidth(20.f),
    m_subMenuPanel(nullptr)
{
    setNormalStateBGColor(ConstDef::MENU_NORMAL_COLOR);
    setHoverStateBGColor(ConstDef::MENU_HOVER_COLOR);
    setPressedStateBGColor(ConstDef::MENU_DOWN_COLOR);
    setTextNormalStateColor(ConstDef::MENU_TEXT_NORMAL_COLOR);
    setTextHoverStateColor(ConstDef::MENU_TEXT_HOVER_COLOR);
    setTextPressedStateColor(ConstDef::MENU_TEXT_DOWN_COLOR);
    setCaptionSize(ConstDef::MENU_TEXT_SIZE);

    setOnClick(std::bind(&MainMenu::showSubMenu, this, std::placeholders::_1));
    m_rect.height = ConstDef::MENU_MAIN_MENU_HEIGHT;
}

void MainMenu::update(void)
{
    // 更新子控件
    Button::update();
}

void MainMenu::draw(void)
{
    if (!m_visible) return;

    // 使用Button的绘制逻辑
    Button::draw();
}

bool MainMenu::handleEvent(shared_ptr<Event> event)
{
    if (!m_enable || !m_visible) return false;

    if (Button::handleEvent(event)) {
        return true;
    }

    // 处理位置事件
    if (EventQueue::isPositionEvent(event->m_eventName)) {
        shared_ptr<SPoint> pos = any_cast<shared_ptr<SPoint>>(event->m_eventParam);
        SRect drawRect = getDrawRect(); // 获取当前控件的绘制区域

        if (!drawRect.contains(pos->x, pos->y)) {
            if (m_subMenuPanel) {
                m_subMenuPanel->hide();
            }
        }
    }

    return false;
}

void MainMenu::addMenuItem(shared_ptr<MenuItem> menuItem)
{
    m_menuItems.push_back(menuItem);
    m_maxSubMenuWidth = max(menuItem->getRect().width, m_maxSubMenuWidth);

    if (m_subMenuPanel) {
        removeControl(m_subMenuPanel);
        m_subMenuPanel.reset();
        m_subMenuPanel = nullptr;
    }
    m_subMenuPanel = PanelBuilder(getParent(), SRect{0, m_rect.top + m_rect.height, m_maxSubMenuWidth, ConstDef::MENU_BAR_HEIGHT * (float)m_menuItems.size()})
                    .setBGColor(SDL_Color{23, 23, 24, 255})
                    .setBorderColor(SDL_Color{68, 68, 75, 255})
                    .setTransparent(false)
                    .build();
    for (size_t i = 0; i < m_menuItems.size(); i++) {
        m_menuItems[i]->setRect(SRect{0, i * ConstDef::MENU_BAR_HEIGHT, m_maxSubMenuWidth, ConstDef::MENU_BAR_HEIGHT});
        m_subMenuPanel->addControl(m_menuItems[i]);
    }

    addControl(m_subMenuPanel);
}

void MainMenu::removeMenuItem(shared_ptr<MenuItem> menuItem)
{
    for (size_t i = 0; i < m_menuItems.size(); i++) {
        if (m_menuItems[i] == menuItem) {
            m_menuItems.erase(m_menuItems.begin() + i);
            break;
        }
    }
    auto it = std::find(m_menuItems.begin(), m_menuItems.end(), menuItem);
    if (it != m_menuItems.end()) {
        m_menuItems.erase(it);

            // 重新计算最大宽度
        m_maxSubMenuWidth = 20.f;
        auto lastItem = m_menuItems.empty() ? nullptr : m_menuItems.back();
        if (lastItem) {
            addMenuItem(lastItem); // 重新添加最后一个菜单项以更新所有面板宽度
        } else {
            if (m_subMenuPanel) {
                removeControl(m_subMenuPanel);
                m_subMenuPanel.reset();
                m_subMenuPanel = nullptr;
            }
        }
    }
}

void MainMenu::showSubMenu(shared_ptr<Button> menu)
{
    SDL_Log("MainMenu::showSubMenu is called.");
    if(!m_subMenuPanel) {
        return;
    }

    if (m_subMenuPanel->getVisible()) {
        m_subMenuPanel->hide();
    } else {
        m_subMenuPanel->show();
    }
}

bool MainMenu::isSubMenuVisible(void)
{
    return m_subMenuPanel && m_subMenuPanel->getEnable() && m_subMenuPanel->getRect().height > 0;
}



// MainMenuBuilder 实现
MainMenuBuilder::MainMenuBuilder(Control *parent, SRect rect, float xScale, float yScale)
{
    m_mainMenu = make_shared<MainMenu>(parent, rect, xScale, yScale);
}

MainMenuBuilder& MainMenuBuilder::setCaption(string caption)
{
    m_mainMenu->setCaption(caption);
    SRect rect = m_mainMenu->getRect();
    rect.width = m_mainMenu->getCaptionRect().width + 5; // 根据标题宽度调整菜单宽度，增加一些边距
    m_mainMenu->setRect(rect);
    return *this;
}

MainMenuBuilder& MainMenuBuilder::setNormalStateBGColor(SDL_Color color)
{
    m_mainMenu->setNormalStateBGColor(color);
    return *this;
}
MainMenuBuilder& MainMenuBuilder::setHoverStateBGColor(SDL_Color color)
{
    m_mainMenu->setHoverStateBGColor(color);
    return *this;
}
MainMenuBuilder& MainMenuBuilder::setPressedStateBGColor(SDL_Color color)
{
    m_mainMenu->setPressedStateBGColor(color);
    return *this;
}

MainMenuBuilder& MainMenuBuilder::setTextNormalStateColor(SDL_Color color)
{
    // Button类使用ConstDef中的颜色，这里暂时不实现
    m_mainMenu->setTextNormalStateColor(color);
    return *this;
}

MainMenuBuilder& MainMenuBuilder::setTextHoverStateColor(SDL_Color color)
{
    m_mainMenu->setTextHoverStateColor(color);
    return *this;
}

MainMenuBuilder& MainMenuBuilder::setOnClick(MainMenu::OnClickHandler onClick)
{
    m_mainMenu->setOnClick(onClick);
    return *this;
}

MainMenuBuilder& MainMenuBuilder::addMenuItem(shared_ptr<MenuItem> menuItem)
{
    m_mainMenu->addMenuItem(menuItem);
    return *this;
}

shared_ptr<MainMenu> MainMenuBuilder::build(void)
{
    return m_mainMenu;
}

// ============================================================================
// MenuBar 实现
// ============================================================================

MenuBar::MenuBar(Control *parent, SRect rect, float xScale, float yScale)
    : Panel(parent, rect, xScale, yScale),
    // m_bgColor(ConstDef::MENU_NORMAL_COLOR),
    m_height(ConstDef::MENU_BAR_HEIGHT)
{
    m_activeMenu = nullptr;
    setBGColor(ConstDef::MENU_NORMAL_COLOR);
    setBorderColor(ConstDef::MENU_BORDER_COLOR);
    setTransparent(false);
}

void MenuBar::update(void)
{
    // 更新子控件
    ControlImpl::update();
}

void MenuBar::draw(void)
{
    // if (!m_visible) return;

    // SDL_Renderer* renderer = getRenderer();
    // if (!renderer) return;

    // // 绘制背景
    // SDL_SetRenderDrawColor(renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    // SRect drawRect = getDrawRect();
    // SDL_RenderFillRect(renderer, drawRect.toSDLFRect());

    // 绘制子控件
    ControlImpl::draw();
}

bool MenuBar::handleEvent(shared_ptr<Event> event)
{
    if (!m_enable || !m_visible) return false;

    return ControlImpl::handleEvent(event);
}

void MenuBar::addMenu(shared_ptr<MainMenu> menu)
{
    m_menus.push_back(menu);
    addControl(menu);

    // 计算菜单宽度（基于文本长度）
    menu->setRect(SRect{0, (m_height - ConstDef::MENU_MAIN_MENU_HEIGHT) / 2,
                    menu->getRect().width + ConstDef::MENU_CAPTION_MARGIN, ConstDef::MENU_MAIN_MENU_HEIGHT});

    if (m_menus.size() == 1) {
        // 增加首个主菜单时，重置菜单栏的宽度
        setRect(SRect{0, 0, menu->getRect().width, m_height}); // 首个主菜单宽度需要强设菜单栏的宽度与之相同
    } else {
        // 增加其他主菜单，先修改添加进来的主菜单的m_rect.left值，再往菜单栏宽度直接叠加新增主菜单的宽度
        menu->setRect(SRect{m_rect.width, (m_height - ConstDef::MENU_MAIN_MENU_HEIGHT) / 2,
                    menu->getRect().width, ConstDef::MENU_MAIN_MENU_HEIGHT});
        setRect(SRect{0, 0, m_rect.width + menu->getRect().width, m_height});
    }
}

void MenuBar::removeMenu(shared_ptr<MainMenu> menu)
{
    auto it = std::find(m_menus.begin(), m_menus.end(), menu);
    if (it != m_menus.end()) {
        m_menus.erase(it);
        removeControl(menu);
    }

    m_rect.width -= menu->getRect().width;

    // 更新所有菜单的位置
    float currentX = 0;
    for (auto& menu : m_menus) {
        if (menu->getEnable()) {
            // 计算菜单宽度（基于文本长度）
            menu->setRect(SRect{currentX, 0, menu->getRect().width, m_height});
            currentX += menu->getRect().width;
        }
    }
}

// void MenuBar::setBGColor(SDL_Color color)
// {
//     m_bgColor = color;
// }

void MenuBar::setHeight(float height)
{
    m_height = height;
    m_rect.height = height;
}

// MenuBarBuilder 实现
MenuBarBuilder::MenuBarBuilder(Control *parent, SRect rect, float xScale, float yScale)
{
    m_menuBar = make_shared<MenuBar>(parent, rect, xScale, yScale);
}

// MenuBarBuilder& MenuBarBuilder::setBGColor(SDL_Color color)
// {
//     m_menuBar->setBGColor(color);
//     return *this;
// }

MenuBarBuilder& MenuBarBuilder::setHeight(float height)
{
    m_menuBar->setHeight(height);
    return *this;
}

MenuBarBuilder& MenuBarBuilder::addMenu(shared_ptr<MainMenu> menu)
{
    m_menuBar->addMenu(menu);
    return *this;
}

shared_ptr<MenuBar> MenuBarBuilder::build(void)
{
    return m_menuBar;
}
