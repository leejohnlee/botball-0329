#include <kipr/wombat.h>
void by();
void move_forward(int ms);
void move_backwards(int ms);
void turn_right(int ms);
void turn_left(int ms);
void mav_move_forward(int ms);
void mav_move_backwards(int ms);
void mav_turn_right(int ms);
void mav_turn_left(int ms);
void flip_switch();
void bye();
void hi();
void go();
void drive_straight(float dist_inches, int speed);
void shimmy();
int left_motor_port=0;
int right_motor_port=1;
int switch_arm=2;
int right_arm_up=1150; //port # 1
int right_arm_down=60; //port # 1
int left_arm_up=780; //port # 0
int left_arm_down=1911; //port # 0
int main() {
    //starting
    enable_servos();
    set_servo_position(right_motor_port,right_arm_up);
    msleep(500);
    set_servo_position(left_motor_port,left_arm_up);
    msleep(500);
    set_servo_position(switch_arm,0);
    msleep(500);
    //exit(1);
    //wait_for_light();
    msleep(500);
    set_servo_position(0,left_arm_down);
    msleep(500);
    shut_down_in(119);
    turn_left(170);

    //start 1st straight
    set_servo_position(1,1150);
    move_forward(4600);
   
    //turn right
    turn_right(1700);

    //drive on bump
    set_servo_position(1,60);
    move_forward(1700);
    
    //turn left off of bump
    turn_left(1700);
    
    //shimmy
    //shimmy();
    //shimmyshimmyahhshimmyahhshimmyahhh
    
    //towards 2nd to last rock
    move_forward(2450);
    
    //turn left towards the rock heap
    turn_left(1500);
    move_forward(2400);
    set_servo_position(0,left_arm_up);
    set_servo_position(1,right_arm_up);
    
    //drive into the rock heap
    move_forward(1000);
    
    //curve the rocks into rock heap more securly
    //turn_right(500);
    //turn back to original position
    //turn_left(500);
    ao();
    //july 5 thing that dongho did
      motor(0,-100);
    motor(1,-100);
    msleep(2300);
    ao();
    motor(1,-50);
    msleep(1400);
    ao();
    by();
    set_servo_position(1,right_arm_down);
    set_servo_position(0,left_arm_down);
    ao();
    motor(1,50);
    msleep(200);
    motor(0,-50);
    msleep(200);
    motor(0,70);
    motor(1,70);
    msleep(2500);
    motor(0,-50);
    motor(1,-50);
    msleep(500);
    set_servo_position(0,left_arm_up);
    motor(0,25);
    motor(1,-75);
    msleep(400);
    set_servo_position(1,400);
    motor(0,25);
    motor(1,-75);
    msleep(1100);
    //go back from rock heap so that the link is in the middle white area
    motor(1,-100);
    motor(0,-100);
    msleep(2300);
    set_servo_position(0,left_arm_down);
    //turns 180 degrees
    
    motor(1,-50);
    motor(0,50);
    msleep(2610);
    //starts going back until the link senses black tape
	hi();
    motor(0,100);
    motor(1,100);
    //goes foward so that the front sensor won't be sensing black
    motor(0,50);
    motor(1,50);
    msleep(7000);
    ao();
    //start to go back until front sensor senses black
    bye();
    motor(1,-50);
    motor(0,-50);
    ao();
    //link will crash to the switch
   // set_servo_position(0,2047);
    motor(0,-50);
    msleep(350);
    motor(1,-100);
    motor(0,-100);
    msleep(2500);
    //turns 90
    motor(0,-50);
    motor(1,50);
    msleep(1500);
    set_servo_position(2,0);
    msleep(200);
    ao();
    //forward and then back
    motor(0,50);
    motor(1,50);
    msleep(3000);
    ao();
    
    while(analog(2)<2900){
    	motor(0,-30);
        motor(1,-30);
    }
    //flip the switch
    drive_straight(0.5,1000);
    set_servo_position(2,0);
    msleep(200);
    set_servo_position(2,1500);
    msleep(500);
    set_servo_position(2,0);
    ao();
}

//~~~~ FUNCTIONS ~~~~//
//turn after black line
//void turnafterblackline (int color) {
   // if (analog(1)<300){
     //what the bot does if the sensor senses WHITE }
   // else{
    //if (analog(1)>2800){
            //what the bot does if the sensor senses BLACK }
    //




//flip switch (rotate servo)
void flip_switch() {
    set_servo_position(0,1526);
    
    
}
//move forward
void move_forward(int ms) {
    motor(0, 100);
    motor(1, 100);
    msleep(ms);
    ao();
}

void mav_move_forward(int ms) {
    //mav(0, )
    //mav(1, )
    msleep(ms);
    ao();
}

//move backwards
void move_backwards(int ms) {
    motor(0,-100);
    motor(1,-100);
    msleep(ms);
    ao();
}

void mav_move_backwards(int ms) {
    //mav(0, )
    ///mav(1, )
    msleep(ms);
    ao();
}

//turn right
void turn_right(int ms) {
    motor(1, 100);
    msleep(ms);
    ao();
}

void mav_turn_right(int ms) {
    //mav(1, );
    msleep(ms);
    ao();    
}

//turn left  
void turn_left(int ms) {
    motor(0, 100);
    msleep(ms);    
}

void mav_turn_left(int ms) {
    //mav(0, );
    msleep(ms);
}


void hi(){
	while(analog(1)<2900){
    	motor(0,50);
    	motor(1,50);
        ao();
    }
    msleep(400);
    while(analog(2)<2900){
    	motor(1,50);
        motor(0,0);
    }
}
void bye(){
    while(analog(1)<2900){
    	motor(0,-50);
        motor(1,-50);
    }
    motor(0,50);
    msleep(500);
    while(analog(2)<2900){
    	motor(0,40);
    }
    ao();
    motor(0,-50);
    motor(1,-50);
    msleep(1250);
    ao();
    while(analog(1) < 2900){
        motor(0,-35);
    }
    ao();
}
void go(){
    while (analog(1) > 2900){
        motor(1,30);
    }
}
void shimmy(){
    motor(1,50);
    msleep(500);
    ao();
    motor(0,50);
    msleep(1000);
    ao();
    motor(1,50);
    msleep(500);
    ao();
}
void drive_straight(float dist_inches, int speed){
    clear_motor_position_counter(left_motor_port);
    clear_motor_position_counter(right_motor_port);
    mav(left_motor_port,-1);
    mav(right_motor_port,-1);
    msleep(10);
    mav(right_motor_port, speed);
    mav(left_motor_port, speed);
	//mrp(right_motor_port,speed,dist_inches*200);
    //mrp(left_motor_port,speed,dist_inches*200);
    if(dist_inches>0){
        while(gmpc(left_motor_port)<dist_inches*200||gmpc(right_motor_port)<dist_inches*200){
            msleep(1);
        }
    }
    else{
        while(gmpc(left_motor_port)>dist_inches*200||gmpc(right_motor_port)>dist_inches*200){
    		msleep(1);
    	}
    }
    mav(left_motor_port,-1);
    mav(right_motor_port,-1);
    msleep(5);
}
void by(){
    while(analog(1)<2900){
    	motor(0,50);
        motor(1,50);
        ao();
    }
    while(analog(2)<2900){
        motor(1,50);
    }
}
