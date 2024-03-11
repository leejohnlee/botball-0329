#include <kipr/wombat.h>
 //Port configurations
int left_servo_port = 3;
int right_servo_port = 2;
int claw_port = 1;

//Servo rotation configurations
int claw_open_degrees = 630;
int claw_closed_degrees = 1600;
int arm_up_degrees = 1024;
int arm_down_degrees = 200;
int arm_grab_position_degrees = 400;
int arm_start_degrees=300;
//Important functions
//Convert meters to inches for Create3
void forward(int inches, float speed);

//Less jerky motion for arm
//Needed syntax: 
//void slow_servo(int left_port, int_right_port, int servo_degrees, int_speed);
void slow_servo(int servo_degrees, float speed);
//Arm and claw functions
void arm_up();
void arm_down();
void arm_grab();
void claw_open();
void claw_close();
void claw_start();
void arm_start();
//LESS THAN 67 FOR IR SENSOR APPROACH CENTER TOWER
int main() {
    //Initialize
    create3_connect();
    enable_servos();
    arm_start();
    //*
    claw_open();
    msleep(250);
    //*/
    //arm_down();
    //arm_up();
    //wait_for_light(port);
    //Toggle
	//*
    //Move out of starting zone, forward until line
    forward(12, 1);
    while (create3_sensor_cliff(0) > 2180 && create3_sensor_cliff(3) > 2180) {
        create3_velocity_set_components(0.25, 0);
        create3_wait();
    }
    forward(-0.5,0.75);
    //turn and approach tower
    create3_rotate_degrees(90, 50);
    create3_wait();
    //Set arm to GRAB position, open claw
    arm_grab();
    claw_open();
    //Maybe use the IR sensors to get accurate measurment?
    //forward(14.75, 0.75);
    forward(16,0.75);
    create3_wait();
    //Grab Cubes
    claw_close();
    msleep(750);
    arm_up();
    //turn, approach rock heap
    msleep(250);
    create3_rotate_degrees(-90, 50);
    create3_wait();
    forward(35, 1);
    msleep(250);
    //Turn to lower arm into rock heap
    create3_rotate_degrees(70, 50);
    create3_wait();
    //Lower arm, drop cubes
    slow_servo(0,1);
    claw_open();
    msleep(500);
    arm_start();
    claw_start();
    //*/
    create3_rotate_degrees(90,50);
    create3_wait();
    for
    return 0;
}
//inverse is 2048-value
void forward(int inches, float speed) {
    create3_drive_straight((inches - 1) / 39.37, speed);
    create3_wait();
};

void arm_up() {
    slow_servo(arm_up_degrees, 1);
}

void slow_servo(int servo_degrees, float speed){
	int current_degrees=get_servo_position(left_servo_port);
    while(current_degrees>servo_degrees){
    	current_degrees-=speed;
        set_servo_position(right_servo_port,current_degrees);
        set_servo_position(left_servo_port,current_degrees);
        msleep(speed);
    }
    while(current_degrees<servo_degrees){
    	current_degrees+=speed;
        set_servo_position(left_servo_port,current_degrees);
        set_servo_position(right_servo_port,current_degrees);
        msleep(speed);
    }
}

void arm_down() {
    slow_servo(arm_down_degrees, 1);
}

void arm_grab() {
    slow_servo(arm_grab_position_degrees, 1);
}

void claw_open() {
    set_servo_position(claw_port, claw_open_degrees);
}

void claw_close() {
    set_servo_position(claw_port, claw_closed_degrees);
 
}
void claw_start(){
	set_servo_position(claw_port, 0);
}
void arm_start(){
	slow_servo(arm_start_degrees,1);
}
