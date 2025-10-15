﻿#ifndef EventQueueH
#define EventQueueH

#include <queue>
#include <mutex>
#include "StateMachine.h"

using namespace std;

enum class EventName: int{
    None,

    FINGER_DOWN,
    FINGER_UP,
    FINGER_MOTION,

    MOUSE_LBUTTON_DOWN,
    MOUSE_LBUTTON_UP,
    MOUSE_RBUTTON_DOWN,
    MOUSE_RBUTTON_UP,
    MOUSE_MBUTTON_DOWN,
    MOUSE_MBUTTON_UP,
    MOUSE_MOVING,

    Begin,  //KeyEvent: S
    Paused, //KeyEvent: P
    GridOnOff,  //KeyEvent: Delete

    MoveDown,   //KeyEvent: Down
    MoveLeft,   //KeyEvent: Left
    MoveRight,  //KeyEvent: Right
    Rotate,     //KeyEvent: Space
    SpeedUp,    //KeyEvent: U

    Fillfull,   //ScreenEvent: Fillfull
    Draw,       //SystemEvent: Draw
    Update,     //SystemEvent: Update
    Timer,      //SystemEvent: Timer

    AnimationEnded, //Animationvent: AnimationEnded. Todo: 在事件的参数中增加id来表示是哪个动画
    LuotiAniEnded,  //LuotiAniEvent: LuotiAniEnded.
    LuotiInstanceEnded,
    AudioEnded,
    NextMusic,
    NextBackground,

    Exit
};

class EventQueue{
private:
    static std::mutex m_mtxForEventQueue;
    std::queue<shared_ptr<Event>> m_eventQueue;
    EventQueue(){}
    ~EventQueue(){clear();}
public:
    static EventQueue* getInstance(void){
        static EventQueue instance; // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }
    static bool isPositionEvent(EventName eventName) {
        switch(eventName){
            case EventName::FINGER_DOWN         :
            case EventName::FINGER_UP           :
            case EventName::FINGER_MOTION       :
            case EventName::MOUSE_LBUTTON_DOWN  :
            case EventName::MOUSE_LBUTTON_UP    :
            case EventName::MOUSE_RBUTTON_DOWN  :
            case EventName::MOUSE_RBUTTON_UP    :
            case EventName::MOUSE_MBUTTON_DOWN  :
            case EventName::MOUSE_MBUTTON_UP    :
            case EventName::MOUSE_MOVING        :
                return true;
            default:
                return false;
        }
    }
    void pushEventIntoQueue(shared_ptr<Event> event);
    shared_ptr<Event> popEventFromQueue(void);
    void clear(void);
};
#endif