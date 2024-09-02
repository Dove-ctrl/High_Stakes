#include "elementOS.h"

bool INITIALIZE_DONE = false;
int PROGRAMFLAG = 0;

uint32_t port_array[DEVICE_NUMMER];//储存uint32_t类型端口号的数组
char get_char_port_array[3*DEVICE_NUMMER];//储存从sdcard里面读取出来的char类型端口号字符串
uint8_t get_uint8_t_port_buffer[3*DEVICE_NUMMER];//把char类型端口号字符串转换成uint8_t类型的缓冲区备用
int port_page = 1;
int changed_port = 1;
int main_page = 1;

eos::op_control_button takein;
eos::op_control_button takeout;
eos::op_control_button armup;
eos::op_control_button armdown;
eos::op_control_button hook;

eos::controller_button tournament(eos::U , "-联赛-");
eos::controller_button skill(eos::L , "-技能赛-");
eos::controller_button driver_debug(eos::R , "-手动调试-");
eos::controller_button auto_debug(eos::D , "-自动调试-");

eos::controller_button tournament1(eos::U , "red+");
eos::controller_button tournament2(eos::L , "red-");
eos::controller_button tournament3(eos::R , "blue+");
eos::controller_button tournament4(eos::D , "blue-");

eos::controller_button port(eos::U , "-端口-");
eos::controller_button control(eos::L , "-控制-");
eos::controller_button setting(eos::R , "-设置-");
eos::controller_button check(eos::D , "-自检-");

eos::controller_button auto_debug_skill(eos::L , "技能赛");
eos::controller_button auto_debug_tournament(eos::R , "联赛");
eos::controller_button auto_debug_tournament1(eos::U , "red+");
eos::controller_button auto_debug_tournament2(eos::L , "red-");
eos::controller_button auto_debug_tournament3(eos::R , "blue+");
eos::controller_button auto_debug_tournament4(eos::D , "blue-");

eos::controller_button page_up(eos::R2);
eos::controller_button page_down(eos::R1);

eos::controller_button number_increase(eos::U);
eos::controller_button number_decrease(eos::D);

eos::controller_button port_change_confirm(eos::A);

void COMPETITIONINFO(const char* title){
    int i; i = 1;
    eos::ClearControllerScreen();
    while (true)
    {
        eos::ControllerPrint("电池:" , 1 , 2);eos::ControllerPrint("%%", 1 , 13);
        eos::ControllerPrint( 
            (int(eos::BRAIN->Battery.capacity()) >= 100 ? 99 : int(eos::BRAIN->Battery.capacity())), 
            1 , 10
        );
        eos::ControllerPrint(title , 1 , 18);
        eos::ControllerPrint("Beyond the Best" , 2 , 5);
        eos::ControllerPrint(">" , 3 , i);
        i = (i >= 18 ? 1 : i + 1);
        if(i == 1){eos::ClearControllerLine(3);}

        eos::SystemWait(1000);
    }
}

void AUTODEBUGINFO(const char* title){
    timer auto_debug_timer;
    eos::ClearControllerScreen();
    auto_debug_timer.clear();
    while (true)
    {
        eos::ControllerPrint("yaw:" , 1 , 1);
        eos::ControllerPrint(chassis::GetInstance().get_yaw_limit() , 1 , 7);
        eos::ControllerPrint("pos:" , 2 , 1);
        eos::ControllerPrint("当前时间:" , 3 , 14);
        eos::ControllerPrint(int(auto_debug_timer.time(sec)) , 3 , 28);
        eos::ControllerPrint("按B终止" , 3 , 1);

        cout << "\033c";
        cout << "////////////////////// " << title << " /////////////////////" << endl;
        cout << "\033[32mAutonomous Debug is running......\033[0m" << endl;
        cout << "\033[33myaw\033[0m: " << chassis::GetInstance().get_yaw_limit()
        << "   \033[34mtemperature\033[0m: " << chassis::GetInstance().get_temperature()
        << "   \033[31mmax temperature\033[0m: " << chassis::GetInstance().get_max_temperature()
        << "   time: " << int(auto_debug_timer.time(sec)) << endl;

        if(int(auto_debug_timer.time(sec)) >= TIMEOUT_WRRNING){
            cout << "\033[33mWarning(elementOS): The program timed out !\033[0m" << endl;
        }
        if(chassis::GetInstance().get_temperature() >= TEMPERATURE_WRRNING){
            cout << "\033[31mError(elementOS): The motor temperature is too high !\033[0m" << endl;
        }
        if(!chassis::GetInstance().get_connection_state()){
            cout<< "\033[31mError(elementOS): The sensor or motor is disconnected !\033[0m" << endl;
        }
        if(eos::CONTROLLER->ButtonB.pressing()){
            eos::CONTROLLER->rumble("-");
            cout << "\033[33mWarning(elementOS): The program has been terminated !\033[0m" << endl;
            thread::interruptAll();
        }

        eos::SystemWait(500);
    }
}

template<typename T>
void PORTINFO(const char* device_name , T* _device){
    eos::ControllerPrint(device_name , 1 , 2);
    eos::ControllerPrint("当前为:" , 2 , 2);
    if(int(_device->index()+1) < 10){
        eos::ControllerPrint(" " , 2 , 13);
        eos::ControllerPrint(int(_device->index())+1 , 2 , 14);
    }
    else{eos::ControllerPrint(int(_device->index())+1 , 2 , 13);}
    eos::ControllerPrint("更改为:" , 3 , 2);
    if(changed_port < 10){
        eos::ControllerPrint(" " , 3 , 13);
        eos::ControllerPrint(changed_port , 3 , 14);
    }
    else{eos::ControllerPrint(changed_port , 3 , 13);}

    if(number_increase.IsClicked()){(changed_port == 21 ? changed_port=21 : changed_port++);}
    else if(number_decrease.IsClicked()){(changed_port == 1 ? changed_port=1 : changed_port--);}

    if(page_up.IsClicked()){(port_page == DEVICE_NUMMER ? port_page = DEVICE_NUMMER : port_page++);}
    else if(page_down.IsClicked()){(port_page == 1 ? port_page = 1 : port_page--);}

    if(port_change_confirm.IsClicked()){
        if(eos::MessageBox("提示" , 11 , "确认端口更改" , 7)){
            //把int类型的端口号转化成char[]类型的端口号
            char char_changed_port[2];
            if(changed_port < 10){changed_port += 30;}
            itoa(changed_port , char_changed_port , 10);
            //查找要更改的设备端口在字符串中的地址，并进行端口号的替换
            char char_port_buffer[3];
            int i_pos = 0;
            int int_port_buffer = 0;
            for(int j=0 ; j<DEVICE_NUMMER ; j++){
                for(int i=i_pos ; i<3*DEVICE_NUMMER ; i++){
                    if(get_char_port_array[i] == '#'){i_pos = i+1; break;}
                    else{ char_port_buffer[i-i_pos] = get_char_port_array[i]; }
                }
                int_port_buffer = atoi(char_port_buffer);
                if(int_port_buffer%30 == int(_device->index())+1){
                    get_char_port_array[i_pos-3] = char_changed_port[0];
                    get_char_port_array[i_pos-2] = char_changed_port[1];
                    break;
                }
            }
            //把新的端口字符串写入sd卡
            FILE *fp;
            fp = fopen("port.txt" , "w");
            fclose(fp);
            uint8_t uint8_t_buffer[3*DEVICE_NUMMER];
            memcpy(uint8_t_buffer , (uint8_t*)get_char_port_array , sizeof(get_char_port_array));
            eos::BRAIN->SDcard.appendfile("port.txt" , uint8_t_buffer , sizeof(uint8_t_buffer));

            eos::ControllerPrint("完成, 即将自动退出" , 2 , 3);
            wait(1500,msec);
            eos::ClearControllerScreen();
            vexSystemExitRequest();
        }
        changed_port = 1;
    }
}
template void PORTINFO(const char* device_name , motor* _device);
template void PORTINFO(const char* device_name , inertial* _device);

int SYSTEMINFO(const controller::button btn){
    if(btn.pressing()){
        waitUntil(!btn.pressing());
        eos::ClearControllerScreen();
        while (true)
        {
            eos::ControllerPrint("电池:" , 1 , 2);eos::ControllerPrint("%%", 1 , 13);
            eos::ControllerPrint( 
                (int(eos::BRAIN->Battery.capacity()) >= 100 ? 99 : int(eos::BRAIN->Battery.capacity())), 
                1 , 10
            );
            
            eos::ControllerPrint("SD卡状态:" , 2 , 2);
            eos::ControllerPrint(
                ( eos::BRAIN->SDcard.isInserted() ? "是" : "否" ),
                2 , 15
            );

            eos::ControllerPrint("&由elementOS驱动&" , 3 , 2);

            if(btn.pressing()){
                waitUntil(!btn.pressing());
                eos::ClearControllerScreen();
                return 1;
            }

            eos::SystemWait();
        }
        
    }else{return 0;}
}

/*
    PROGRAMFLAG:
    1: 进入联赛/联赛路线1
    2: 技能赛
    3: 手动调试
    4: 进入自动调试/技能赛自动调试
    5: 端口
    6: 设置
    7: 控制
    8: 自检
    9: 联赛路线1自动调试
    10: 联赛路线2自动调试
    11: 联赛路线3自动调试
    12: 联赛路线4自动调试
    13: 联赛路线2
    14: 联赛路线3
    15: 联赛路线4
*/

void elementOS(void* cpt){
    competition* p = (competition*)cpt;

    //读取端口
    
    if(eos::BRAIN->SDcard.exists("port.txt")){
        eos::BRAIN->SDcard.loadfile("port.txt" , get_uint8_t_port_buffer , sizeof(get_uint8_t_port_buffer));
        memcpy(get_char_port_array , (char*)get_uint8_t_port_buffer , sizeof(get_uint8_t_port_buffer));
        
        char port_buffer[2];
        int int_port; int i_pos = 0;
        for(int j = 0 ; j<DEVICE_NUMMER ; j++){
            for(int i=i_pos ; i<3*DEVICE_NUMMER ; i++){
                if(get_char_port_array[i] == '#'){i_pos = i+1; break;}
                else{ port_buffer[i-i_pos] = get_char_port_array[i]; }
            }
            int_port = atoi(port_buffer);
            if(int_port == 31){port_array[j] = PORT1;}
            else if(int_port == 32){port_array[j] = PORT2;}
            else if(int_port == 33){port_array[j] = PORT3;}
            else if(int_port == 34){port_array[j] = PORT4;}
            else if(int_port == 35){port_array[j] = PORT5;}
            else if(int_port == 36){port_array[j] = PORT6;}
            else if(int_port == 37){port_array[j] = PORT7;}
            else if(int_port == 38){port_array[j] = PORT8;}
            else if(int_port == 39){port_array[j] = PORT9;}
            else if(int_port == 10){port_array[j] = PORT10;}
            else if(int_port == 11){port_array[j] = PORT11;}
            else if(int_port == 12){port_array[j] = PORT12;}
            else if(int_port == 13){port_array[j] = PORT13;}
            else if(int_port == 14){port_array[j] = PORT14;}
            else if(int_port == 15){port_array[j] = PORT15;}
            else if(int_port == 16){port_array[j] = PORT16;}
            else if(int_port == 17){port_array[j] = PORT17;}
            else if(int_port == 18){port_array[j] = PORT18;}
            else if(int_port == 19){port_array[j] = PORT19;}
            else if(int_port == 20){port_array[j] = PORT20;}
            else if(int_port == 21){port_array[j] = PORT21;}
            else {port_array[j] = 0;}
        }
    }//端口读取完成

    //加载配置

    motor lf = motor(port_array[0] , ratio6_1 , true);
    motor lmf = motor(port_array[1] , ratio6_1 , false);
    motor lmb = motor(port_array[2] , ratio6_1 , false);
    motor lb = motor(port_array[3] , ratio6_1 , true);
    motor rf = motor(port_array[4] , ratio6_1 , false);
    motor rmf = motor(port_array[5] , ratio6_1 , true);
    motor rmb = motor(port_array[6] , ratio6_1 , true);
    motor rb = motor(port_array[7] , ratio6_1 , false);
    inertial inertial_1 = inertial(port_array[8]);
    inertial inertial_2 = inertial(port_array[9]);

    encoder encoderx = encoder(Brain.ThreeWirePort.A);
    encoder encodery = encoder(Brain.ThreeWirePort.C);
    LF = &lf;
    LMF = &lmf;
    LMB = &lmb;
    LB = &lb;
    RF = &rf;
    RMF = &rmf;
    RMB = &rmb;
    RB = &rb;
    Inertial_1 = &inertial_1;
    Inertial_2 = &inertial_2;
    encoderX = &encoderx;
    encoderY = &encodery;

    //加载配置完成

    //读取按键设置

    

    //按键设置读取完成

    while (true)//功能选择循环，两页
    {

        while (main_page != 0)
        {   
            eos::ClearControllerScreen();
            if(main_page == 1){//第一页

                while (main_page == 1)
                {
                    tournament.Display(1 , 11);
                    skill.Display(2 , 2);
                    driver_debug.Display(2 , 16);
                    auto_debug.Display(3 , 9);

                    SYSTEMINFO(eos::CONTROLLER->ButtonX);

                    if(page_up.IsClicked()){main_page ++;}
                    
                    if(tournament.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "联赛" , 12)){PROGRAMFLAG = 1; main_page = 0;}
                        else{main_page = 1;}
                    }
                    else if(skill.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "技能赛" , 11)){PROGRAMFLAG = 2; main_page = 0;}
                        else{main_page = 1;}
                    }
                    else if(driver_debug.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "手动调试" , 10)){PROGRAMFLAG = 3; main_page = 0;}
                        else{main_page = 1;}
                    }
                    else if(auto_debug.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "自动调试" , 10)){PROGRAMFLAG = 4; main_page = 0;}
                        else{main_page = 1;}
                    }

                    eos::SystemWait();
                }

            }else if(main_page == 2){//第二页
                
                while (main_page == 2)
                {
                    port.Display(1 , 11);
                    control.Display(2 , 4);
                    setting.Display(2 , 18);
                    check.Display(3 , 11);

                    SYSTEMINFO(eos::CONTROLLER->ButtonX);

                    if(page_down.IsClicked()){main_page --;}

                    if(port.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "端口" , 12)){PROGRAMFLAG = 5; main_page = 0;}
                        else{main_page = 2;}
                    }
                    else if(control.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "控制" , 12)){PROGRAMFLAG = 6; main_page = 0;}
                        else{main_page = 2;}
                    }
                    else if(setting.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "设置" , 12)){PROGRAMFLAG = 7; main_page = 0;}
                        else{main_page = 2;}
                    }
                    else if(check.IsClicked()){
                        if(eos::MessageBox("确认进入：" , 9 , "自检" , 12)){PROGRAMFLAG = 8; main_page = 0;}
                        else{main_page = 2;}
                    }

                    eos::SystemWait();
                }

            }
            eos::SystemWait();
        }//功能选择结束

        //程序选择
        if (PROGRAMFLAG >= 1 && PROGRAMFLAG <= 2) {//tournament,skill
            eos::ControllerPrint("==================" , 1 , 1);
            eos::ControllerPrint("校准中" , 2 , 10);
            eos::ControllerPrint("==================" , 3 , 1);
            initialize_robot();
            eos::ClearControllerScreen();

            tournament1.Display(1 , 11);
            tournament2.Display(2 , 5);
            tournament3.Display(2 , 16);
            tournament4.Display(3 , 11);

            while(true){
                if(tournament1.IsClicked()){
                    PROGRAMFLAG = 1;break;
                }else if(tournament2.IsClicked()){
                    PROGRAMFLAG = 13;break;
                }else if(tournament3.IsClicked()){
                    PROGRAMFLAG = 14;break;
                }else if(tournament4.IsClicked()){
                    PROGRAMFLAG = 15;break;
                }

                eos::SystemWait();
            }
            eos::ClearControllerScreen();
            if(!(p->isCompetitionSwitch() || p->isFieldControl())){
                eos::ControllerPrint("==================" , 1 , 1);
                eos::ControllerPrint("等待场控连接" , 2 , 6);
                eos::ControllerPrint("==================" , 3 , 1);
                waitUntil(p->isCompetitionSwitch() || p->isFieldControl());
                eos::ClearControllerScreen();
                INITIALIZE_DONE = true; break;
            }
            else{INITIALIZE_DONE = true; break;}
        }
        else if(PROGRAMFLAG == 3){//user debug
            eos::ControllerPrint("==================" , 1 , 1);
            eos::ControllerPrint("校准中" , 2 , 10);
            eos::ControllerPrint("==================" , 3 , 1);
            initialize_robot();
            INITIALIZE_DONE = true; 
            eos::ClearControllerScreen();
            break;
        }
        else if(PROGRAMFLAG == 4){//auto debug
            eos::ControllerPrint("==================" , 1 , 1);
            eos::ControllerPrint("校准中" , 2 , 10);
            eos::ControllerPrint("==================" , 3 , 1);
            initialize_robot();
            eos::ClearControllerScreen();
            while (true)
            {
                eos::ControllerPrint("选择程序" , 1 , 9);
                auto_debug_skill.Display(2 , 4);
                auto_debug_tournament.Display(2 , 19);
                eos::ControllerPrint("==================" , 3 , 1);

                if(auto_debug_skill.IsClicked()){
                    PROGRAMFLAG = 4;break;
                }else if(auto_debug_tournament.IsClicked()){//选择自动调试联赛路线
                    eos::ClearControllerScreen();
                    auto_debug_tournament1.Display(1 , 11);
                    auto_debug_tournament2.Display(2 , 5);
                    auto_debug_tournament3.Display(2 , 16);
                    auto_debug_tournament4.Display(3 , 11);
                    while(true){
                        if(auto_debug_tournament1.IsClicked()){
                            PROGRAMFLAG = 9;break;
                        }else if(auto_debug_tournament2.IsClicked()){
                            PROGRAMFLAG = 10;break;
                        }else if(auto_debug_tournament3.IsClicked()){
                            PROGRAMFLAG = 11;break;
                        }else if(auto_debug_tournament4.IsClicked()){
                            PROGRAMFLAG = 12;break;
                        }

                        eos::SystemWait();
                    }
                    break;
                }
                eos::SystemWait();
            }
            INITIALIZE_DONE = true;
            eos::ClearControllerScreen();
            break;
        }
        else if(PROGRAMFLAG >= 5 && PROGRAMFLAG <= 7){//port,control,setting
            if(!eos::BRAIN->SDcard.isInserted()){
                eos::MessageBox("警告!" , 11 , "未插入SD卡" , 8);
                INITIALIZE_DONE = false;
            }else{INITIALIZE_DONE = true; break;}
        }else{INITIALIZE_DONE = true; break;}//self_check
        //程序选择结束
        eos::SystemWait();
    }

    //程序信息显示
    switch (PROGRAMFLAG)
    {
    case 1://tournament1
        COMPETITIONINFO("RED+");
        break;
    case 13://tournament2
        COMPETITIONINFO("RED-");
        break;
    case 14://tournament3
        COMPETITIONINFO("BLUE+");
        break;
    case 15://tournament4
        COMPETITIONINFO("BLUE-");
        break;
    case 2://skill
        COMPETITIONINFO("SKILL");
        break;
    case 3://driver_debug
        while (true)
        {
            eos::ControllerPrint("yaw:" , 1 , 1);

            if(chassis::GetInstance().get_yaw_limit() < 10){
                eos::ControllerPrint(chassis::GetInstance().get_yaw_limit() , 1 , 7);
                eos::ControllerPrint(" " , 1 , 8);
                eos::ControllerPrint(" " , 1 , 9);
            }else if(chassis::GetInstance().get_yaw_limit() >= 10 && chassis::GetInstance().get_yaw_limit() < 100){
                eos::ControllerPrint(chassis::GetInstance().get_yaw_limit() , 1 , 7);
                eos::ControllerPrint(" " , 1 , 9);
            }else{
                eos::ControllerPrint(chassis::GetInstance().get_yaw_limit() , 1 , 7);
            }
            
            eos::ControllerPrint("pos:" , 2 , 1);
            eos::ControllerPrint("电机温度:" , 3 , 1);
            eos::ControllerPrint(chassis::GetInstance().get_temperature() , 3 , 15);
            eos::SystemWait();
        }
        break;
    case 4://auto_debug_skill
        AUTODEBUGINFO("SKILL");
        break;
    case 9://auto_debug_tournament1
        AUTODEBUGINFO("TOURNAMENT RED+");
        break;
    case 10://auto_debug_tournament2
        AUTODEBUGINFO("TOURNAMENT RED-");
        break;
    case 11://auto_debug_tournament3
        AUTODEBUGINFO("TOURNAMENT BLUE+");
        break;
    case 12://auto_debug_tournament4
        AUTODEBUGINFO("TOURNAMENT BLUE-");
        break;
    case 5://port    
    eos::ClearControllerScreen();
    while(true){

        if(eos::BRAIN->SDcard.exists("port.txt")){
            eos::BRAIN->SDcard.loadfile("port.txt" , get_uint8_t_port_buffer , sizeof(get_uint8_t_port_buffer));
            memcpy(get_char_port_array , (char*)get_uint8_t_port_buffer , sizeof(get_uint8_t_port_buffer));
        }
    
        while(port_page == 1){
            PORTINFO("LF     " , LF);
            eos::SystemWait();
        }

        while(port_page == 2){
            PORTINFO("LMF     " , LMF);
            eos::SystemWait();
        }

        while(port_page == 3){
            PORTINFO("LMB     " , LMB);
            eos::SystemWait();
        }

        while(port_page == 4){
            PORTINFO("LB     " , LB);
            eos::SystemWait();
        }

        while(port_page == 5){
            PORTINFO("RF     " , RF);
            eos::SystemWait();
        }

        while(port_page == 6){
            PORTINFO("RMF     " , RMF);
            eos::SystemWait();
        }

        while(port_page == 7){
            PORTINFO("RMB     " , RMB);
            eos::SystemWait();
        }

        while(port_page == 8){
            PORTINFO("RB     " , RB);
            eos::SystemWait();
        }

        while(port_page == 9){
            PORTINFO("IMU_1     " , Inertial_1);
            eos::SystemWait();
        }

        while(port_page == 10){
            PORTINFO("IMU_2     " , Inertial_2);
            eos::SystemWait();
        }
    }
    break;
    case 6://control
        eos::ClearControllerScreen();

        break;
    case 7://setting
        eos::ClearControllerScreen();

        break;
    case 8://check
    {
        eos::ClearControllerScreen();
        eos::ControllerPrint("检查接线..." , 1 , 2);
        bool installed_check_list[10] = {
            LF->installed(),
            LMF->installed(),
            LMB->installed(),
            LB->installed(),
            RF->installed(),
            RMF->installed(),
            RMB->installed(),
            RB->installed(),
            Inertial_1->installed(),
            Inertial_2->installed(),
        };
        for(int i = 0 ; i < 10 ; i++){
            if(installed_check_list[i] == 0){
                eos::ClearControllerLine(1);
                eos::ControllerPrint("接线异常" , 1 , 2);
                break;
            }else{
                eos::ClearControllerLine(1);
                eos::ControllerPrint("接线正常" , 1 , 2);
            }
            wait(100,msec);
        }

        eos::ControllerPrint("电量检查..." , 2 , 2);
        wait(1000,msec);
        eos::ClearControllerLine(2);
        if(Brain.Battery.capacity() <= 60){
            eos::ControllerPrint("电量低" , 2 , 2);
        }else{
            eos::ControllerPrint("电量足" , 2 , 2);
        }
        
        eos::ClearControllerScreen();
        eos::ControllerPrint("完成, 即将自动退出" , 2 , 3);
        wait(3000,msec);
        eos::ClearControllerScreen();
        vexSystemExitRequest();
        break;
    }
    default:
        break;
    }
}