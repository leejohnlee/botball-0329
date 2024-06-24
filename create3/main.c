#include <kipr/wombat.h>

//Servo ports
int left_servo_port = 3;
int right_servo_port = 2;
int claw_port = 1;

//Create3 sensor values
double velocity = 0.15; //This is used for better bump detection for alignment for purple tubes

//Sensor ports
int rangefinder_port=5;

//Claw values
int claw_open_degrees = 630;
int claw_closed_degrees = 1600;
int claw_start_degrees = 630;
int claw_grab_degrees = 2047;

//Arm values
int arm_up_degrees = 900;
int arm_down_degrees = 200;
int arm_grab_position_degrees = 425;
int arm_start_degrees = 300;

//Convert meters to inches for Create3
void forward(int inches, float speed);
void drive_until_bumped(); // Used for grabbing second row cubes
void rotate(int degrees); //Used for consistent speeds of rotation and also not needing to always call create3_wait()
void forward_until_line(); //Used for getting to habitat docks
void forward_until_line_side_sensors(); //forward_until_line() but when there is a line under sensors 1 and 2, and it is risky to use those sensors
void swipe_poms(); //Turns create3 in an arc to push poms into area
void pause_until_button_press(); //Debugging for line-by-line evaluation
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
void initialize();
void pom_area();
void purple_tubes();

//NOTE: Speed is in servo ticks per second
//Higher target positions will make the arm travel clockwise
//Lower target positions make the arm travel counterclockwise
void set_arm_position(int target_pos, float speed);

int main() {
    initialize();
    //These are sections of the total code
    get_multipliers();
    pom_area();
    purple_tubes();
    
    //Reset for next test
    arm_start();
    set_servo_position(claw_port, claw_open_degrees);
    return 0;
}
//Code chunks, separate code for testing

void initialize(){ //To clean up int main()
	create3_connect();
    enable_servos();
    arm_start();
    claw_start();
    msleep(250);
    //wait_for_light(0);
    claw_open();
    //msleep(12000);
    shut_down_in(119);
    claw_close();
}
void get_multipliers(){
    msleep(250);

    //Slow down to avoid bump offset and close claw to avoid purple tube
    claw_close();
    forward(38, 30);
    forward(3, 0.5);
    msleep(0.3);

    //turn and approach tower
    create3_rotate_degrees(92, 50);
    create3_wait();

    //Return to default position and go to tower
    arm_grab();
    claw_open();
    forward(10, 0.75);
    create3_wait();
    
    //Align to center tower wall
    float velocity = 0.04;
    while(velocity != 0){
    	create3_velocity_set_components(velocity, 0);
        if(create3_sensor_ir(2)>=100){
        	velocity = 0;
        }
    }
    create3_wait();
    create3_velocity_set_components(0,0);
    forward(3,1);
    create3_velocity_set_components(0,0);
    create3_wait();
    
    //Grab Cubes
    claw_close();
    msleep(750);
    arm_up();
    forward(-0.5,1);
    
    //Not doing rock heap anymore, so first row cubes go to Area 4 (orange poms)
    msleep(250);
    rotate(-90);
    forward(24,1);
    rotate(90);
    forward(1.5,1);
    rotate(45);
    arm_down();
    msleep(500);
    claw_open();
    
    //Grab the second row of cubes
    msleep(100);
    slow_servo(arm_grab_position_degrees + 100, 1); //Second row cubes grab position
    forward(6, 1);
    velocity = 0.05;
    while(velocity != 0){ //Fancy sensor setup
        //Better alignment to center tower walls, using "vertical" rangefinder
    	if(analog(5)>2775){ //best 2080
        	velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
    claw_close();
    msleep(500);
    arm_up();
    forward(-17,1);
    
    //Go to the pom area
    rotate(45);
    forward(12,1);
    forward_until_line();//moves forward to line to reduce step counting, then turns and drops multipliers
}
void pom_area(){
    //Push poms into the area with multipliers
    rotate(-2);
    forward(4,1);
	slow_servo(1024,1);
    claw_close();
    swipe_poms();//swipe_poms() uses arc turning to push poms into area with multipliers
    swipe_poms();
    swipe_poms();
    rotate(35);
    
    //Move back and drop cubes
    forward(-5, 1);
    rotate(10);
    arm_down();
    msleep(250);
    claw_open();
    msleep(250);
    
    //Set arm position, then move to purple tubes
    slow_servo(arm_grab_position_degrees-100,1);
    rotate(70);
    forward(20,1);
    rotate(-90);
    forward(5,1);
}

void purple_tubes(){
    msleep(100);
    while(analog(rangefinder_port)<1750){ //Code for horizontal alignment, taken from old habitat construction code
    	create3_velocity_set_components(0,0.015);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    create3_wait();
    msleep(100);
    /* Removed because the rangefinder is slightly off, so this while loop is just "move forward forever"
    while(analog(rangefinder_port)<2900){ //repurposed code above, used for precise moving back for claw to be able to grip noodle
    	create3_velocity_set_components(0.005,0);
        create3_wait();
    }*/
    rotate(6);
    forward(1.125,1);
    claw_grab_habitat(); //Tight grip on purple noodle
    msleep(500);
    arm_down();
    msleep(100);
    rotate(135);
    forward(6,1);
    rotate(-45);
    arm_up();
    rotate(-45);
    forward(27,5);
    rotate(-90);
    drive_until_bumped();
    forward(0.5,1);
    rotate(90);
    drive_until_bumped();
}

//Other useful functions
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
    slow_servo(25, 1);
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
void drive_until_bumped(){
	double velocity = 0.15;
    while (velocity != 0){
        if (create3_sensor_bump(0)== 1||create3_sensor_bump(1)==1||create3_sensor_bump(2)==1 || create3_sensor_bump(3)==1 || create3_sensor_bump(4)==1){
            velocity = 0;
        }
        create3_velocity_set_components(velocity,0);
    }
    create3_wait();
}
void rotate(int degrees){
	create3_rotate_degrees(degrees,100);
    create3_wait();
    msleep(50);
}
void claw_open(){ set_servo_position(claw_port, claw_open_degrees); }
void claw_close(){ set_servo_position(claw_port, claw_closed_degrees); }
void claw_start(){ set_servo_position(claw_port, claw_start_degrees); }
void claw_grab_habitat(){ set_servo_position(claw_port, claw_grab_degrees); }
void forward_until_line(){
	double velocity = 0.05;
    while (velocity != 0){
        if (create3_sensor_cliff(1)< 2000 ||create3_sensor_cliff(2)< 2000){
            velocity = 0;
        }
    	create3_velocity_set_components (velocity,0);
    }
    create3_wait();
}
void forward_until_line_side_sensors(){
	double velocity = 0.05;
    while (velocity != 0){
        if (create3_sensor_cliff(0)< 2000 ||create3_sensor_cliff(3)< 2000){
            velocity = 0;
        }
    	create3_velocity_set_components (velocity,0);
    }
    create3_wait();
}
void swipe_poms(){
    rotate(-55);
    create3_drive_arc_degrees(0.1524, 55, 0.15);
    create3_wait();
    
    msleep(100);
}
void pause_until_button_press(){
	while(!side_button()){
    	msleep(1);
    }
}
