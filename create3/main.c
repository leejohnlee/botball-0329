#include <kipr/wombat.h>

//Servo ports
int left_servo_port = 3;
int right_servo_port = 2;
int claw_port = 1;

//Sensor ports
int rangefinder_port=5;

//Claw values
int claw_open_degrees = 630;
int claw_closed_degrees = 1600;

//Arm values
int arm_up_degrees = 700;
int arm_down_degrees = 200;
int arm_grab_position_degrees = 425;
int arm_start_degrees = 300;

//Convert meters to inches for Create3
void forward(int inches, float speed);

//Less jerky motion for arm
//NOTE: Speed is in servo ticks per second
void slow_servo(int servo_degrees, float speed);

//Arm and claw functions
void arm_up();
void arm_down();
void arm_grab();
void claw_open();
void claw_close();
void claw_start();
void arm_start();
void claw_grab_habitat();

//Code chunks
void get_multipliers();

//NOTE: Speed is in servo ticks per second
//Higher target positions will make the arm travel clockwise
//Lower target positions make the arm travel counterclockwise
void set_arm_position(int target_pos, float speed);

int main() {
    //Initialize
    create3_connect();
    enable_servos();
    arm_start();
    claw_start();
    msleep(250);
    wait_for_light(0);
    claw_close();
    msleep(12000);
    shut_down_in(119);
    
    
    claw_close();
    arm_start();
    
    //This is a section of the total code
    get_multipliers();
    
    //To the habitat construction
    forward(36,1);
    
    //Precision alignment to habitat construction dock
    create3_rotate_degrees(-20,0.5);
    while(analog(rangefinder_port)<2000){
    	create3_velocity_set_components(0,0.075);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    while(analog(rangefinder_port)<2890){
    	create3_velocity_set_components(0.075,0);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    //Grab habitat tube(red pool noodle)
    claw_close();
    msleep(500);
    
    //*Move forward because the arm uses angular movement, and the tube needs linear movement(Prevent create from flipping)
    create3_velocity_set_components(0.1,0);
    create3_wait();
    arm_up();
    
    //turn to push poms
    create3_rotate_degrees(90,10);
    create3_wait();
    create3_rotate_degrees(-90,10);
    create3_wait();
    msleep(250);
    
    //Back up and drop tube
    forward(-5,1);
    create3_rotate_degrees(70,50);
    create3_wait();
    forward(5,1);
    msleep(250);
    set_arm_position(0,1);
    claw_open();
    msleep(500);
    
    //next round init
    arm_start();
    set_servo_position(claw_port, 0);
    return 0;
}

void forward(int inches, float speed) {
    //inches - 1 is to compensate for the small movement the Create makes at the end
    create3_drive_straight((inches - 1) / 39.37, speed);
    create3_wait();
};
//Move arm up
void arm_up() {
    slow_servo(arm_up_degrees, 1);
}

void slow_servo(int servo_degrees, float speed) {
    int current_degrees = get_servo_position(left_servo_port);
    while (current_degrees > servo_degrees) {
        current_degrees -= speed;
        set_servo_position(right_servo_port, current_degrees);
        set_servo_position(left_servo_port, current_degrees);
        msleep(speed);
    }
    while (current_degrees < servo_degrees) {
        current_degrees += speed;
        set_servo_position(left_servo_port, current_degrees);
        set_servo_position(right_servo_port, current_degrees);
        msleep(speed);
    }
}

void arm_down() {
    slow_servo(arm_down_degrees, 1);
}
//Arm position to grab 1st row cubes
void arm_grab() {
    slow_servo(arm_grab_position_degrees, 1);
}
//Set arm to the starting position
void arm_start() {
    slow_servo(arm_start_degrees, 1);
}

void set_arm_position(int target_pos, float speed) {
    slow_servo(target_pos, speed);
}

void claw_open() {
    set_servo_position(claw_port, claw_open_degrees);
}

void claw_close() {
    set_servo_position(claw_port, claw_closed_degrees);

}
//Set claw to starting position
void claw_start() {
    set_servo_position(claw_port, 0);
}
void claw_grab_habitat() {
    set_servo_position(claw_port, 1400);
}
//Code chunks, separate code for testing
void get_multipliers(){
    msleep(250);

    //Slow down to avoid bump offset and close claw to avoid purple tube
    claw_close();
    forward(37, 3);
    forward(3, 0.5);
    msleep(0.3);

    //turn and approach tower
    create3_rotate_degrees(92, 50);
    create3_wait();

    //Return to default position and go to tower
    arm_start();
    arm_grab();
    claw_open();
    forward(10, 0.75);
    create3_wait();
    
    //Align to center tower wall
    while(analog(rangefinder_port)<2200){
    	create3_velocity_set_components(0.075,0);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    create3_wait();
    
    //Grab Cubes
    claw_close();
    msleep(750);
    arm_up();
    
    //Back up to push poms into Area 4
    forward(-2,1);
    create3_wait();
    
    //go to rock heap
    msleep(250);
    create3_rotate_degrees(-90, 50);
    create3_wait();
    forward(35, 1);
    msleep(250);

    //Turn and drop cubes
    //60
    create3_rotate_degrees(80, 50);
    create3_wait();
    slow_servo(0, 1);
    claw_open();
    msleep(500);
    arm_grab();
    claw_start();

    //Turn and approach tower to grab second row cubes
    //110
    create3_rotate_degrees(90, 50);
    create3_wait();
    forward(17, 1);
    create3_rotate_degrees(-45, 50);
    create3_wait();
    arm_grab();
    forward(1, 1);

    //Grab second row cubes
    slow_servo(arm_grab_position_degrees + 90, 1);
    forward(3.0, 1);
    
    //Align to center tower wall
    while(analog(rangefinder_port)<2375){
    	create3_velocity_set_components(0.075,0);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    create3_wait();
    claw_close();
    msleep(750);
    arm_up();

    //Back up and drop cubes in designated area
    forward(-12, 1);
    set_arm_position(0,1);
    msleep(500);
    claw_open();
	msleep(250);
    arm_start();
    
    //Turn and head toward habitat construction
    forward(-2, 1);
    create3_rotate_degrees(42, 50);
}
