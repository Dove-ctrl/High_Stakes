#include "elementOS.h"

namespace eos
{
    brain*            BRAIN = NULL;
    controller*       CONTROLLER = NULL;

    void SystemInitialize(brain* _brain , controller* _controller){
        BRAIN = _brain; CONTROLLER = _controller;
        CONTROLLER->Screen.clearScreen();
        BRAIN->Screen.clearScreen();
    }

    void SystemWait(){wait(100,msec);}
    void SystemWait(int time){wait(time,msec);}

    bool MessageBox(const char* title , int title_line , const char* text , int text_line){
        ClearControllerScreen();
        ControllerPrint(title , 1 , title_line);
        ControllerPrint(text , 2 , text_line);
        ControllerPrint("确认: A         取消: B" , 3 , 1);
        while (true)
        {
            if(CONTROLLER->ButtonA.pressing()){
                ClearControllerScreen();
                return true;
            }else if(CONTROLLER->ButtonB.pressing()){
                ClearControllerScreen();
                return false;
            }
            SystemWait();
        }
    }

    void ClearControllerScreen(){CONTROLLER->Screen.clearScreen();}
    void ClearBrainScreen(){BRAIN->Screen.clearScreen();}
    void ClearControllerLine(int column){CONTROLLER->Screen.clearLine(column);}
    void ClearBrainLine(int column){BRAIN->Screen.clearLine(column);}

    template<class T>
    void ControllerPrint(T value , int c , int l){
        CONTROLLER->Screen.setCursor(c , l);
        CONTROLLER->Screen.print(value);
    }
    template void ControllerPrint(int value , int c , int l);
    template void ControllerPrint(double value , int c , int l);
    template void ControllerPrint(const char* value , int c , int l);

    controller_button::controller_button(
        physical_button       btn,
        const char*           t
    ){
        bind_button = btn;
        text = t;
        is_visual = true;
    }

    controller_button::controller_button(
        physical_button       btn
    ){
        column = 0 , line = 0;
        bind_button = btn;
        text = NULL;
        is_visual = false;
    }

    void controller_button::Display(int c , int l){
        if(is_visual){
            column = c; line = l;
            CONTROLLER->Screen.setCursor(column , line);
            CONTROLLER->Screen.print(text);
        }else{eos::SystemWait();}
    }

    void controller_button::SetColumn(int c){
        column = (is_visual ? c : 0);
    }

    void controller_button::SetLine(int l){
        line = (is_visual ? l : 0);
    }

    void controller_button::SetText(const char* t){
        text = (is_visual ? t : NULL);
    }

    void controller_button::BindButton(physical_button btn){
        bind_button = btn;
    }

    void controller_button::UnBindButton(){
        bind_button = N;
    }

    int controller_button::GetColumn(){
        return column;
    }

    int controller_button::GetLine(){
        return line;
    }

    physical_button controller_button::GetBindButton(){
        return bind_button;
    }

    bool controller_button::IsClicked(){
        switch (bind_button)
        {
        case A:
            if(CONTROLLER->ButtonA.pressing()){
                waitUntil(!CONTROLLER->ButtonA.pressing());return true;
            }else{return false;}
            break;
        case B:
            if(CONTROLLER->ButtonB.pressing()){
                waitUntil(!CONTROLLER->ButtonB.pressing());return true;
            }else{return false;}
            break;
        case X:
            if(CONTROLLER->ButtonX.pressing()){
                waitUntil(!CONTROLLER->ButtonX.pressing());return true;
            }else{return false;}
            break;
        case Y:
            if(CONTROLLER->ButtonY.pressing()){
                waitUntil(!CONTROLLER->ButtonY.pressing());return true;
            }else{return false;}
            break;
        case U:
            if(CONTROLLER->ButtonUp.pressing()){
                waitUntil(!CONTROLLER->ButtonUp.pressing());return true;        
            }else{return false;}
            break;
        case D:
            if(CONTROLLER->ButtonDown.pressing()){
                waitUntil(!CONTROLLER->ButtonDown.pressing());return true;               
            }else{return false;}
            break;
        case L:
            if(CONTROLLER->ButtonLeft.pressing()){
                waitUntil(!CONTROLLER->ButtonLeft.pressing());return true;        
            }else{return false;}
            break;
        case R:
            if(CONTROLLER->ButtonRight.pressing()){
                waitUntil(!CONTROLLER->ButtonRight.pressing());return true;               
            }else{return false;}
            break;
        case L1:
            if(CONTROLLER->ButtonL1.pressing()){
                waitUntil(!CONTROLLER->ButtonL1.pressing());return true;               
            }else{return false;}
            break;
        case L2:
            if(CONTROLLER->ButtonL2.pressing()){
                waitUntil(!CONTROLLER->ButtonL2.pressing());return true;                
            }else{return false;}
            break;
        case R1:
            if(CONTROLLER->ButtonR1.pressing()){
                waitUntil(!CONTROLLER->ButtonR1.pressing());return true;                
            }else{return false;}
            break;
        case R2:
            if(CONTROLLER->ButtonR2.pressing()){
                waitUntil(!CONTROLLER->ButtonR2.pressing());return true;                
            }else{return false;}
            break;
        default:
            return false;
            break;
        }
    }

    op_control_button::op_control_button(const controller::button* button){
        controller_bind_button = button;
    }

    op_control_button::op_control_button(){
        controller_bind_button = NULL;
    }

    void  op_control_button::SetBindButton(const controller::button* button){
        controller_bind_button = button;
    }

    const controller::button* op_control_button::GetBindButton(){
        return controller_bind_button;
    }
}