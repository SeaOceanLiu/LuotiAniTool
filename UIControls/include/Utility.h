
#ifndef UtilityH
#define UtilityH
#include <SDL3/SDL.h>
class SMultipleSize{
public:
    float scaleX;
    float scaleY;
    SMultipleSize():scaleX(1), scaleY(1){}
    SMultipleSize(float x, float y):scaleX(x), scaleY(y){}
    SMultipleSize(const SMultipleSize& m):scaleX(m.scaleX), scaleY(m.scaleY){}
    SMultipleSize(const SMultipleSize&& m):scaleX(m.scaleX), scaleY(m.scaleY){}

    SMultipleSize& operator=(const SMultipleSize& m){
        scaleX = m.scaleX;
        scaleY = m.scaleY;
        return *this;
    }
    SMultipleSize& operator=(const SMultipleSize&& m){
        scaleX = m.scaleX;
        scaleY = m.scaleY;
        return *this;
    }
    bool operator==(const SMultipleSize& m){
        return scaleX == m.scaleX && scaleY == m.scaleY;
    }
    SMultipleSize operator*(const SMultipleSize& m){
        return SMultipleSize(scaleX * m.scaleX, scaleY * m.scaleY);
    }
};
class SSize{
public:
    float width;
    float height;
    SSize():width(0), height(0){}
    SSize(float w, float h):width(w), height(h){}
    SSize(const SSize& s):width(s.width), height(s.height){}
    SSize(const SSize&& s):width(s.width), height(s.height){}
    SSize& operator=(const SSize& s){
        width = s.width;
        height = s.height;
        return *this;
    }
    SSize& operator=(const SSize&& s){
        width = s.width;
        height = s.height;
        return *this;
    }
    bool operator==(const SSize& s){
        return width == s.width && height == s.height;
    }
    SSize operator+(const SSize& s){
        return SSize(width + s.width, height + s.height);
    }
    SSize operator-(const SSize& s){
        return SSize(width - s.width, height - s.height);
    }
    SSize operator*(const SMultipleSize& m){
        return SSize(width * m.scaleX, height * m.scaleY);
    }
};
class SPoint{
public:
    float x;
    float y;
    SPoint():x(0), y(0){}
    SPoint(float x, float y):x(x), y(y){}
    SPoint(const SPoint& p):x(p.x), y(p.y){}
    SPoint(const SPoint&& p):x(p.x), y(p.y){}
    SPoint& operator=(const SPoint& p){
        x = p.x;
        y = p.y;
        return *this;
    }
    SPoint& operator=(const SPoint&& p){
        x = p.x;
        y = p.y;
        return *this;
    }
    bool operator==(const SPoint& p){
        return x == p.x && y == p.y;
    }
    SPoint operator+(const SPoint& p){
        return SPoint(x + p.x, y + p.y);
    }
    SPoint operator-(const SPoint& p){
        return SPoint(x - p.x, y - p.y);
    }
    SDL_FPoint toSDLFPoint(void){
        return SDL_FPoint{x, y};
    }
};

class SRect{
private:
    SDL_FRect m_sdlFRect;
public:
    float left;
    float top;
    float width;
    float height;
    SRect():left(0), top(0), width(0), height(0){}
    SRect(SPoint p, SSize s):left(p.x), top(p.y), width(s.width), height(s.height){}
    SRect(float l, float t, float w, float h):left(l), top(t), width(w), height(h){}
    SRect(const SRect& r):left(r.left), top(r.top), width(r.width), height(r.height){}
    SRect(const SRect&& r):left(r.left), top(r.top), width(r.width), height(r.height){}
    SRect& operator=(const SRect& r){
        left = r.left;
        top = r.top;
        width = r.width;
        height = r.height;
        return *this;
    }
    SRect& operator=(const SRect&& r){
        left = r.left;
        top = r.top;
        width = r.width;
        height = r.height;
        return *this;
    }
    bool operator==(const SRect& r){
        return left == r.left && top == r.top && width == r.width && height == r.height;
    }
    bool contains(float x, float y){
        return x >= left && x <= left + width && y >= top && y <= top + height;
    }
    bool contains(const SPoint& p){
        return contains(p.x, p.y);
    }
    SRect operator+(const SPoint& p){
        return SRect(left + p.x, top + p.y, width, height);
    }
    SRect operator-(const SPoint& p){
        return SRect(left - p.x, top - p.y, width, height);
    }
    SRect operator*(const SMultipleSize& m){
        return SRect(left, top, width * m.scaleX, height * m.scaleY);
    }
    SDL_FRect* toSDLFRect(void){
        m_sdlFRect = SDL_FRect{left, top, width, height};
        return &m_sdlFRect;
    }
    SDL_Rect toSDLRect(void){
        return SDL_Rect{(int)left, (int)top, (int)width, (int)height};
    }
};
template<class F>
class final_action{
public:
    static_assert(!std::is_reference<F>::value && !std::is_const<F>::value &&
                    !std::is_volatile<F>::value,
                    "Final_action must be a callable type");
    // 构造函数，传进来的东西，比如函数，就会赋值给自己的成员F m_f;
    explicit final_action(F f) noexcept : m_f(std::move(f)){}
    final_action(final_action&& other) noexcept :
        m_f(std::move(other.m_f)), m_invoke(other.m_invoke)
    {}
    final_action(const final_action& other) = delete;
    final_action& operator=(const final_action& other) = delete;
    final_action& operator=(final_action&& other) = delete;

    // 析构函数时候，就调用m_f()函数，也就是构造函数时候传进来的函数
    ~final_action() noexcept{
        if(m_invoke){
            m_f();
        }
    }
private:
    F m_f;
    bool m_invoke = true; // 默认为true，表示析构时调用m_f
};
template<class F>
final_action<typename std::remove_cv<typename std::remove_reference<F>::type>::type>
finally(F&& f) noexcept{
    return final_action<typename std::remove_cv<typename std::remove_reference<F>::type>::type>(
        std::forward<F>(f));
}

#endif // UtilityH