#pragma once

#include "vex.h"

extern bool INITIALIZE_DONE;
extern bool MOTORCHECK;
extern int PROGRAMFLAG;

/// @brief elementOS主函数
/// @param cpt competition指针
void elementOS(void* cpt);

void tournament_route1(void* cpt);
void tournament_route2(void* cpt);
void tournament_route3(void* cpt);
void tournament_route4(void* cpt);
void skill_route(void* cpt);

//---------------------------------------------------------------//

namespace eos
{
    extern brain*           BRAIN;
    extern controller*      CONTROLLER;

    enum physical_button{
        N , A , B , X , Y , U , D , L , R , L1 , L2 , R1 , R2
    };

    void SystemInitialize(brain* _brain , controller* _controller);

    void SystemWait();
    void SystemWait(int time);

    bool MessageBox(const char* title , int title_line , const char* text , int text_line);

    void ClearControllerScreen();
    void ClearControllerLine(int column);
    void ClearBrainScreen();
    void ClearBrainLine(int column);

    template<class T>
    void ControllerPrint(T value , int c , int l);

    class controller_button{
    private:

        int                             column;
        int                             line;
        physical_button                 bind_button;
        const char*                     text;
        bool                            is_visual;

    public:

        /// @brief 创建一个可视按钮
        /// @param btn 绑定的按键
        /// @param t 文本信息
        controller_button(
            const physical_button    btn,
            const char*                 t
        );

        /// @brief 创建一个隐形按钮
        /// @param btn 绑定的按键
        controller_button(
            const physical_button    btn
        );

        /// @brief 显示按钮
        /// @param c 所在行数
        /// @param l 所在列数
        void Display(int c , int l);

        /// @brief 设置按钮所在列数
        /// @param c 行数
        void SetColumn(int c);

        /// @brief 设置按钮所在行数
        /// @param l 列数
        void SetLine(int l);

        /// @brief 设置按钮文字
        /// @param t 文字数组指针
        void SetText(const char* t);

        /// @brief 设置按钮绑定的遥控器按键
        /// @param btn 遥控器按键
        void BindButton(physical_button btn);

        /// @brief 解除按钮和遥控器按键的绑定
        void UnBindButton();

        /// @brief 获取列数
        /// @return 
        int GetColumn();

        /// @brief 获取行数
        /// @return 
        int GetLine();

        /// @brief 获取按钮绑定的遥控器按键
        /// @return 遥控器按键指针
        physical_button GetBindButton();

        /// @brief 判断按钮是否按下，按下返回true，否则返回false
        /// @return 
        bool IsClicked();
    };
}