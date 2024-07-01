#include <kipr/wombat.h>

// Function declarations for Create3 operations
void forward(int inches, float speed); // Move the robot forward by specified distance at given speed
void drive_until_bumped(); // Drive until a bump sensor is triggered
void rotate(int degrees); // Rotate the robot by specified degrees
void forward_until_line(); // Move forward until a line is detected by the cliff sensors
void forward_until_line_side_sensors(); // Move forward until a line is detected by side cliff sensors
void swipe_poms(); // Perform an arc turn to push poms into an area
void pause_until_button_press(); // Pause program execution until a button press is detected
void light_color(int r, int g, int b, float t); // Set the light ring color to specified RGB values
void align_to_line(); // Align the robot to a line using cliff sensors
void align_to_line_backwards(); //Align the robot to a line using cliff sensors backwards

// Function declarations for arm and claw operations
void slow_servo(int servo_degrees, float speed); // Gradually move a servo to a target position at a specified speed
void arm_up(); // Move the arm up to a predefined position
void arm_down(); // Move the arm down to a predefined position
void arm_grab(); // Position the arm for grabbing cubes
void claw_open(); // Open the claw servo to a predefined position
void claw_close(); // Close the claw servo to a predefined position
void claw_start(); // Move the claw servo to a starting position
void arm_start(); // Move the arm servo to a starting position
void claw_grab_habitat(); // Adjust claw servo to grab habitat elements
void set_arm_position(int target_pos, float speed); // Set the arm servo to a specific position at a specified speed

// Function declarations for main code chunks
void get_multipliers(); // Execute actions related to fetching multipliers
void initialize(); // Perform initial setup and connection to the robot
void pom_area(); // Execute actions related to handling poms in a designated area
void purple_tubes(); // Execute actions related to handling purple tubes

// Global variables
int left_servo_port = 3; // Left servo motor port
int right_servo_port = 2; // Right servo motor port
int claw_port = 1; // Claw servo motor port

double velocity = 0.15; // Velocity parameter for specific operations (e.g., bump detection)

int rangefinder_port = 5; // Port for the rangefinder sensor

int claw_open_degrees = 1045; // Degrees to open the claw
int claw_closed_degrees = 260; // Degrees to close the claw
int claw_start_degrees = 1680; // Degrees to set the claw at starting position
int claw_grab_degrees = 0; // Degrees to grab habitat cubes

int arm_up_degrees = 700; // Degrees to raise the arm
int arm_down_degrees = 200; // Degrees to lower the arm
int arm_grab_position_degrees = 450; // Degrees for the arm to grab cubes
int arm_start_degrees = 550; // Degrees to set the arm at starting position
//NOTE: Values in motor ticks, not degrees.
// Main function
int main() {
    initialize(); // Initialize the robot and perform setup
    while(!side_button()){msleep(1);}
    // Execute sections of the total code
    get_multipliers(); // Fetch multipliers
    //pom_area(); // Execute actions related to poms
    //purple_tubes(); // Handle purple tubes
    return 0;
}

// Function definitions

// Create3 operations
void forward(int inches, float speed) {
    // Move forward by specified distance at given speed
    create3_drive_straight((inches - 1) / 39.37, speed);
    create3_wait();
}

void drive_until_bumped() {
    // Drive until a bump sensor is triggered
    double velocity = 0.075;
    while (velocity != 0) {
        if (create3_sensor_bump(0) == 1 || create3_sensor_bump(1) == 1 ||
            create3_sensor_bump(2) == 1 || create3_sensor_bump(3) == 1 ||
            create3_sensor_bump(4) == 1) {
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
}

void rotate(int degrees) {
    // Rotate the robot by specified degrees
    create3_rotate_degrees(degrees, 150); //Original value 50
    create3_wait();
    msleep(50);
}

void forward_until_line() {
    // Move forward until a line is detected by the cliff sensors
    double velocity = 0.05;
    while (velocity != 0) {
        if (create3_sensor_cliff(1) < 2000 || create3_sensor_cliff(2) < 2000) {
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
}

void forward_until_line_side_sensors() {
    // Move forward until a line is detected by side cliff sensors
    double velocity = 0.05;
    while (velocity != 0) {
        if (create3_sensor_cliff(0) < 2000 || create3_sensor_cliff(3) < 2000) {
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
}

void swipe_poms() {
    // Perform an arc turn to push poms into an area
    rotate(-55);
    create3_drive_arc_degrees(0.1524, 55, 10);
    create3_wait();
    create3_velocity_set_components(0,0);
    msleep(100);
}

void pause_until_button_press() {
    // Pause program execution until a button press is detected
    while (!side_button()) {
        msleep(1);
    }
}

void light_color(int r, int g, int b, float t) {
    // Set the light ring color to specified RGB values
    Create3LedColor green;
    green.r = r;
    green.g = g;
    green.b = b;
    Create3Lightring lightring;
    lightring.led0 = green;
    lightring.led1 = green;
    lightring.led2 = green;
    lightring.led3 = green;
    lightring.led4 = green;
    lightring.led5 = green;
    create3_led_animation(Create3BlinkLights, lightring, t);
    create3_wait();
}

void align_to_line() {
    // Align the robot to a line using cliff sensors
    while (1) {
        if (create3_sensor_cliff(0) <= 2195) {
            create3_velocity_set_components(0.025, 0.3);
        }
        if (create3_sensor_cliff(3) <= 2195) {
            create3_velocity_set_components(0.025, -0.3);
        }
        if ((create3_sensor_cliff(0) <= 2195) && (create3_sensor_cliff(3) <= 2195)) {
            break;
        }
        if ((create3_sensor_cliff(0) >= 2195) && (create3_sensor_cliff(3) >= 2195)) {
            create3_velocity_set_components(0.1, 0);
        }
    }

    create3_wait();
    create3_rotate_degrees(1, 1);
    create3_wait();
}

void align_to_line_backwards(){
    // Align the robot to a line using cliff sensors
    while (1) {
        if (create3_sensor_cliff(0) <= 2195) {
            create3_velocity_set_components(-0.025, -0.3);
        }
        if (create3_sensor_cliff(3) <= 2195) {
            create3_velocity_set_components(-0.025, 0.3);
        }
        if ((create3_sensor_cliff(0) <= 2195) && (create3_sensor_cliff(3) <= 2195)) {
            break;
        }
        if ((create3_sensor_cliff(0) >= 2195) && (create3_sensor_cliff(3) >= 2195)) {
            create3_velocity_set_components(-0.1, 0);
        }
    }

    create3_wait();
    create3_rotate_degrees(-1, 1);
    create3_wait();
}
// Arm and claw operations

void slow_servo(int servo_degrees, float speed) {
    // Gradually move a servo to a target position at a specified speed
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

void arm_up() {
    // Move the arm up to a predefined position
    slow_servo(arm_up_degrees, 1);
}

void arm_down() {
    // Move the arm down to a predefined position
    slow_servo(25, 1); // Assuming 25 degrees is the lower position
}

void arm_grab() {
    // Position the arm for grabbing cubes
    slow_servo(arm_grab_position_degrees, 1);
}

void claw_open() {
    // Open the claw servo to a predefined position
    set_servo_position(claw_port, claw_open_degrees);
}

void claw_close() {
    // Close the claw servo to a predefined position
    set_servo_position(claw_port, claw_grab_degrees); //I am DONE with the claw half-closing and dropping the multipliers
}

void claw_start() {
    // Move the claw servo to a starting position
    set_servo_position(claw_port, claw_start_degrees);
}

void arm_start() {
    // Move the arm servo to a starting position
    slow_servo(arm_start_degrees, 1);
}

void claw_grab_habitat() {
    // Adjust claw servo to grab habitat elements
    set_servo_position(claw_port, claw_grab_degrees);
}

void set_arm_position(int target_pos, float speed) {
    // Set the arm servo to a specific position at a specified speed
    slow_servo(target_pos, speed);
}

// Main code chunks
void initialize() {
    // Perform initial setup and connection to the robot
    create3_connect();
    enable_servos();
    arm_start();
    claw_start();
    msleep(250);
    // wait_for_light(0);
    //claw_open();
    //msleep(12000);
    //shut_down_in(119);
    //claw_close();
}
void get_multipliers() { //[NOTE] Changed this so in the delay when the Link crosses the center bump, 
    //the Create3 pushes the poms into a different area, but still can get points form that
    // Fetch multipliers
    msleep(250);

    // Slow down to avoid bump offset and close claw to avoid purple tube
    
    claw_close();
    align_to_line();
    arm_up();
    //[BEGIN_CHANGES]
    forward(5, 30); // Original value 38
    rotate(60);
    forward(4,10);
    arm_down();
    forward(-4,10);
    arm_up();
    forward(1.25,1);
    rotate(-60);
    forward(9,1);
    rotate(70);
    forward(5,10);
    arm_down();
    forward(-3,10);
    arm_up();
    rotate(-70);
    
    //msleep(18000); //18 second delay
    //forward(10,30);
    //[END_CHANGES]
    align_to_line(); // Eliminate error
    forward(3, 0.25);

    // Turn and approach tower
    create3_rotate_degrees(92, 50);
    create3_wait();
	slow_servo(arm_grab_position_degrees+50, 1);
    claw_start();
    // Execute sections of the total code
        float velocity = 0.05;
    while (velocity != 0) {
        if (create3_sensor_ir(2) >= 900){
        	velocity = 0;
            create3_velocity_set_components(velocity, 0);
            light_color(255,255,0,1);
        }
        if(analog(5) > 2800) {
            velocity = 0;
            create3_velocity_set_components(velocity, 0);
            light_color(255,0,255,1);
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
    claw_close();
    msleep(500);
    arm_up();
    msleep(500);
    /*// Return to default position and go to tower
    slow_servo(arm_grab_position_degrees + 50, 1);
    set_servo_position(claw_port, 700);
    forward(10, 0.75);
    create3_wait();
	drive_until_bumped();
    
     Align to center tower wall
    float velocity = 0.025;
    while (velocity != 0) {
        if (create3_sensor_ir(2) >= 90 || analog(5) > 2700) {
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();*
    float velocity = 0.08;
    while (velocity != 0) { // Fancy sensor setup
        // Better alignment to center tower walls, using "vertical" rangefinder
        if (analog(5) > 2700) { // Best 2080, V2 2680
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_velocity_set_components(0, 0);
    //forward(3, 1);
    //create3_velocity_set_components(0, 0);
    create3_wait();
	msleep(500);
    // Grab cubes
    claw_close();
    msleep(750);
    arm_up();*/
    rotate(150);
    arm_down();
    msleep(500);
    claw_open();
    msleep(250);
    arm_up();
    rotate(-150);
    forward(0.5, 1);
    rotate(-90);
    msleep(1000);
    forward(24, 1);
    rotate(90);
    align_to_line();
    forward(-1,1);
    rotate(45);
    msleep(100);
    slow_servo(arm_grab_position_degrees + 150, 1); // Second row cubes grab position, original value 125
    forward(8, 1);
    velocity = 0.08;
    while (velocity != 0) { // Fancy sensor setup
        // Better alignment to center tower walls, using "vertical" rangefinder
        if (analog(5) > 2780) { // Best 2080, V2 2680
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
    create3_velocity_set_components(0,0);
    create3_wait();
    forward(2,1);
    claw_grab_habitat();
    msleep(500);
    arm_up();
    forward(-12,10);
    arm_down();
    msleep(500);
    claw_open();
    msleep(500);
    arm_up();
    
	forward(-3,10);
    rotate(80);
    align_to_line();
    rotate(-90);
    forward(9,30);
    //Purple Noodle Code
    velocity = -0.015;
    while (velocity != 0) { // Code for horizontal alignment, taken from old habitat construction code
        if ((analog(rangefinder_port) > 1300) || (/*Sensor code*/0)) {
            velocity = 0;
        }
        create3_velocity_set_components(0, velocity);
    }
    create3_wait();
    //rotate(-2); //Removed because over-compensation for create3 movements
    slow_servo(arm_grab_position_degrees + 5, 1);
    forward(2, 1);
    while (!(create3_sensor_ir(2) >= 20)) {
        create3_velocity_set_components(0.015, 0);
        create3_wait();
    }
    claw_grab_habitat();
    for (int i = 0; i < 5; i += 1) {
        slow_servo(arm_grab_position_degrees - (i*80), 1);
        forward(0.125, 1);
    }
    arm_up();
    rotate(135);
    drive_until_bumped();
    /* Removed because this part of the code is obsolete, but is still
	here because it can be referenced.
    forward(-15,30);
    rotate(90);
    arm_down();
    msleep(250);
    claw_open();
    msleep(500);
    arm_up();
    rotate(-135);
    forward(10,10);
    align_to_line();*/
	/*
    // Not doing rock heap anymore, so first row cubes go to Area 4 (orange poms)
    msleep(250);
    forward(0, 1);
    rotate(-45); //-90
    arm_down();
    msleep(500);
    claw_open();
    msleep(500);
    arm_up();
    rotate(-45);
    msleep(1000);
    forward(24, 1);
    rotate(90);
    align_to_line();
    forward(-1,1);
    rotate(45);
    //arm_down();
    //msleep(500);
    //claw_open();

    // Grab the second row of cubes
    msleep(100);
    slow_servo(arm_grab_position_degrees + 150, 1); // Second row cubes grab position, original value 125
    forward(8, 1);
    velocity = 0.08;
    while (velocity != 0) { // Fancy sensor setup
        // Better alignment to center tower walls, using "vertical" rangefinder
        if (analog(5) > 2680) { // Best 2080
            velocity = 0;
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
    claw_grab_habitat();
    msleep(500);
    arm_up();
    //forward(-17, 1);
    align_to_line_backwards();

    // Go to the pom area
    forward(-6,1);
    rotate(87);
    forward(11, 10);
    align_to_line(); // Moves forward to line to reduce step counting, then turns and drops multipliers*/
}

void pom_area() {
    // Execute actions related to handling poms in a designated area
    // Push poms into the area with multipliers
    slow_servo(1024, 1);
    swipe_poms(); // swipe_poms() uses arc turning to push poms into area with multipliers
    swipe_poms();
    swipe_poms();
    rotate(35);

    // Move back and drop cubes
    forward(0, 1);
    rotate(15); // Original 10
    arm_down();
    msleep(250);
    claw_open();
    msleep(250);

    // Set arm position, then move to purple tubes
    slow_servo(arm_grab_position_degrees + 275, 1);
    rotate(75); // Original 70
    forward(30, 1); // Original 20, 2nd original 25
    rotate(-90); // Original -90, 2nd original 45, 3rd original -80
    // forward(5,1);
}

void purple_tubes() {
    // Execute actions related to handling purple tubes
    forward(2,10);
    velocity = -0.015;
    while (velocity != 0) { // Code for horizontal alignment, taken from old habitat construction code
        if ((analog(rangefinder_port) > 1300) || (/*Sensor code*/0)) {
            velocity = 0;
        }
        create3_velocity_set_components(0, velocity);
    }
    create3_wait();
    //rotate(-2); //Removed because over-compensation for create3 movements
    slow_servo(arm_grab_position_degrees + 5, 1);
    forward(2, 1);
    while (!(create3_sensor_ir(2) >= 15)) {
        create3_velocity_set_components(0.015, 0);
        create3_wait();
    }
    claw_grab_habitat();
    for (int i = 0; i < 5; i += 1) {
        slow_servo(arm_grab_position_degrees - (i*80), 1);
        forward(0.125, 1);
    }
    msleep(250);
    //arm_grab();
    rotate(45);
    align_to_line();
    msleep(250);
    arm_up();
    drive_until_bumped();
    arm_grab();
    forward(0,1);
    rotate(90);
    drive_until_bumped();
    forward(0.875,1);
    msleep(500);
    rotate(-27.5);
    slow_servo(180,1);
    msleep(500);
    /*velocity = -0.015; //Removed because the randefinder gets confused
    while (velocity != 0) { // Code for horizontal alignment, taken from old habitat construction code
        if ((analog(rangefinder_port) > 2700)) {
            velocity = 0;
        }
        create3_velocity_set_components(0, velocity);
    }*/
    create3_wait();
    msleep(250);
    drive_until_bumped();
    msleep(250);
    velocity = 0.015;
    while (velocity != 0) { // Code for horizontal alignment, taken from old habitat construction code
        if ((analog(rangefinder_port) > 2700)) {
            velocity = 0;
        }
        create3_velocity_set_components(0, velocity);
    }
    msleep(500);
    rotate(-2);
    msleep(500);
	create3_velocity_set_components(-0.03,0);
    create3_wait();
    msleep(300);
    create3_velocity_set_components(0,0);
    create3_wait();
    msleep(500);
    claw_open();
    /*msleep(100);
    while(analog(rangefinder_port)<1750){ //Code for horizontal alignment, taken from old habitat construction code
        create3_velocity_set_components(0,0.015);
        create3_wait();
    }
    create3_velocity_set_components(0,0);
    create3_wait();
    msleep(100);
    * Removed because the rangefinder is slightly off, so this while loop is just "move forward forever"
    while(analog(rangefinder_port)<2900){ //repurposed code above, used for precise moving back for claw to be able to grip noodle
        create3_velocity_set_components(0.005,0);
        create3_wait();
    }
    rotate(6);
    forward(1.125,1);
    claw_grab_habitat(); // Tight grip on purple noodle
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
    drive_until_bumped();*/
}
