#include <kipr/wombat.h>
//Commonly changed values

//Highest score: 230
//Lowest score: 226

//Testing
int with_wait_for_light=1; //SET TO 1 AT GCER!!!!! NOT NOT SET TO 0!!!
int override_with_side_button=1; //Set to 1 for wait for light simulation. If at_gcer is 1, this will be hard-set to 0.
int at_gcer=1; //also set this to 1 for a fail-safe
int starting_height=330; //ORIGINAL VALUE 550 DO NOT OVERWRITE
int with_offset_rotation=1;
int offset_angle=-9;
int with_automatic_position_setup=1;

//Sort
int how_far_to_move_for_second_pom_pyramid=1; //NOTE; this is subtracted by 1 when used in the function
int requires_moving_forward_for_second_pom_pyramid=0; //Controls rotation. If second pyramid does not need forward movement, set to 0. Else set to 1.
int pull_in_poms_step_count = -3;

//Moving over bump to face center tower
int move_over_bump_step_count=11;

//First row cubes
int first_row_cubes_grab_position_incremental_value=120; //87 100
int first_row_cubes_grab_offset=-3; //After dropping off the pom, then the robot rotates back to grab cubes. Rotate extra by this amount.
int first_row_cubes_move_forward_to_drop_pyramid_pom_step_count = 11; //Original 12

//Second row cubes
int second_row_cubes_grab_position_incremental_value=125;
int second_row_cubes_min_rangefinder_distance= 2775; // (new) Minimum distance to close claw (old) Best 2080, V2 2680, V3 2585, V4 2800
int second_row_cubes_max_rangefinder_distance= 2850; //(new) Maximum distance to close claw
int second_row_cubes_uses_rangefinder=0;
int second_row_cubes_step_count=7; //Original 8

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
void pom_claw_open(); //Open the micro-servo claw
void pom_claw_close(); //Close the micro-servo claw
void arm_start(); // Move the arm servo to a starting position
void claw_grab_habitat(); // Adjust claw servo to grab habitat elements
void set_arm_position(int target_pos, float speed); // Set the arm servo to a specific position at a specified speed

int sensor_bumped(); //Get if the front touch sensor is triggered

// Function declarations for main code chunks
void pom_pyramid_multipliers(); // Execute actions related to fetching multipliers to place in the area with the red and green poms
void initialize(); // Perform initial setup and connection to the robot
void pom_pyramid_area(); // Execute actions related to handling poms in a designated area
void pom_pyramid_area_with_sort(); //pom_pyramid_area() with actions related to sorting the poms
void purple_tubes(); // Execute actions related to handling purple tubes
void orange_pom_area(); //Execute actions realted to handling multiplier cubes in the orange pom area


// Global variables
int left_servo_port = 3; // Left servo motor port
int right_servo_port = 2; // Right servo motor port
int claw_port = 1; // Claw servo motor port
int pom_claw_port = 0; //Port for the micro-servo that is located under the main claw

double velocity = 0.15; // Velocity parameter for specific operations (e.g., bump detection)

int rangefinder_port = 5; // Port for the rangefinder sensor
int front_touch_sensor_port = 0; //Port for the front "bumper".

int claw_open_degrees = 900; // Degrees to open the claw, Original value 1045, V2 900
int claw_closed_degrees = 260; // Degrees to close the claw
int claw_start_degrees = 1680; // Degrees to set the claw at starting position
int claw_grab_degrees = 0; // Degrees to grab habitat cubes

int pom_claw_open_position = 0; //Position to open the pom claw
int pom_claw_closed_position = 590; //Position to close the pom claw

int arm_up_degrees = 700; // Degrees to raise the arm
int arm_down_degrees = 80; // Degrees to lower the arm
int arm_grab_position_degrees = 475; // Degrees for the arm to grab cubes Original value 450
int arm_start_degrees = 550; // Degrees to set the arm at starting position
int arm_sort_poms_position = 190; //Original value 150

//NOTE: Values in motor ticks, not degrees.
// Main function
int main() {
    create3_connect();
    //Testing parameters
    if(with_automatic_position_setup){
    	rotate(-offset_angle);
    }
    arm_start_degrees=starting_height;
    if(at_gcer){
    	with_wait_for_light=1;
        override_with_side_button=0;
    }
    
    //Full run
    create3_drive_straight(0,0.1);
    create3_wait();
    light_color(0,255,0,0.25);
    initialize(); // Initialize the robot and perform setup
    
    // Execute sections of the total code
    pom_pyramid_area_with_sort();
    light_color(50,50,255,1);
    pom_pyramid_multipliers();
    orange_pom_area();
    light_color(50,255,50,1);
    
    //Not needed
    /*pom_pyramid_area(); //Execute actions related to the red and green pom area
    pom_pyramid_multipliers(); //Execute actions related to placing multipliers in that area
    orange_pom_area(); //Execute actions related to the orange pom area, such as placing multipliers and pushing orange poms
    purple_tubes(); //Execute actions related to handling the purple tubes*/
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
    
    //way to stop so momentum does not offset robot
    create3_drive_straight(0,0);
    create3_wait();
    //create3_rotate_degrees(1, 1);
    //create3_wait();
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
    slow_servo(arm_down_degrees+50, 1); // Assuming 25 degrees is the lower position
}

void arm_grab() {
    // Position the arm for grabbing cubes
    slow_servo(arm_grab_position_degrees, 1);
}
void pom_claw_open(){
	set_servo_position(pom_claw_port, pom_claw_open_position);
}
void pom_claw_close(){
	set_servo_position(pom_claw_port, pom_claw_closed_position);
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


int sensor_bumped(){
	return digital(front_touch_sensor_port);
}
// Main code chunks
void initialize() {
    // Perform initial setup and connection to the robot
    enable_servos();
    claw_start(); //open claw first, so it does not get pushed onto the habitat docks
    arm_start();
    
    pom_claw_open();
    msleep(250);
    //rotate(0.5);
    if(with_wait_for_light){
        if(override_with_side_button){
        	pause_until_button_press();
        }
        else{
    		wait_for_light(0);
        }
    }
    slow_servo(550,1);
    if(with_offset_rotation){
    	rotate(offset_angle);
    }
    //claw_open();
    //msleep(12000);
    shut_down_in(119);
    //claw_close();
}
void pom_pyramid_area_with_sort(){
    //New starting position code
    forward(4,1);
    rotate(-45);
    align_to_line();
    claw_close();
    //Old starting position code
    /*
	claw_close();
    align_to_line();
    */
    arm_up();
    forward(5, 30);
    rotate(60);
    forward(3,1);
    slow_servo(arm_sort_poms_position-30, 1);
    msleep(500);
    pom_claw_close();
    msleep(1000);
    arm_up();
    forward(-2,1);
    slow_servo(arm_down_degrees+50,1);
    forward(8,1);
    forward(-6,1);
    arm_up();
	rotate(-60);
    forward(9,1);
    rotate(180); //-60
    arm_down();
    pom_claw_open();
    arm_up();
    if(requires_moving_forward_for_second_pom_pyramid){
        rotate(-180); //-111
        forward(how_far_to_move_for_second_pom_pyramid,1);
        rotate(69);
    }
    else{
    	rotate(-111);
    }
    //rotate(70);
    forward(4,1);
    slow_servo(arm_sort_poms_position-30, 1);
    //rotate(3);
    msleep(500);
    pom_claw_close();
    msleep(500);
    //rotate(-3);
    arm_up();
    //rotate(30);
    //forward(2,1);
    //arm_down();
    //pom_claw_open();
    //arm_up();
    forward(2,1);
    arm_down();
    forward(pull_in_poms_step_count,1);
    arm_up();
    rotate(-68);
    forward(move_over_bump_step_count,1);
}

void pom_pyramid_multipliers(){
    forward(3, 0.25);

    // Turn and approach tower
    create3_rotate_degrees(90, 50);
    create3_wait();
	slow_servo(arm_grab_position_degrees+first_row_cubes_grab_position_incremental_value, 1);
    claw_start();
    forward(first_row_cubes_move_forward_to_drop_pyramid_pom_step_count,0.75);
    rotate(45);
    //forward(3,1);
    arm_down();
    msleep(500);
    pom_claw_open();
    msleep(500);
    arm_grab();
    //forward(-1,1);
    rotate(-45+first_row_cubes_grab_offset);
    claw_open();
    float velocity = 0.015;
    while (velocity != 0) {
        if (create3_sensor_ir(2) >= 900){
        	velocity = 0;
            create3_velocity_set_components(velocity, 0);
            light_color(255,255,0,0.5);
        }
        if(analog(5) > 2080) { //Original value 2750, V2 2550
            velocity = 0;
            create3_velocity_set_components(velocity, 0);
            light_color(255,0,255,0.5);
        }
        create3_velocity_set_components(velocity, 0);
    }
    create3_wait();
    //Experimental code
    create3_velocity_set_components(0.1,0);
    create3_wait();
    msleep(100);
    create3_velocity_set_components(0,0);
    create3_wait();
    rotate(2);
    claw_close();
    msleep(100);
    forward(-1,0.75);
    arm_up();
    forward(3,1);
    msleep(100);

    rotate(150);
    arm_down();
    msleep(500);
    claw_open();
    msleep(250);
    arm_up();
    rotate(120);
	//End
   
    arm_up();
    forward(20,1);
    rotate(90);
    forward(0,1);
    align_to_line();
    rotate(45);
    msleep(100);
}
void orange_pom_area(){
    slow_servo(arm_grab_position_degrees + second_row_cubes_grab_position_incremental_value, 1); // Second row cubes grab position, original value 135
    if(second_row_cubes_uses_rangefinder){
        forward(7, 1);
        velocity = 0.015;
        while (velocity != 0) { // Fancy sensor setup
            // Better alignment to center tower walls, using "vertical" rangefinder
            if (analog(5) > second_row_cubes_min_rangefinder_distance && analog(5) < second_row_cubes_max_rangefinder_distance) {
                velocity = 0;
            }
            if(analog(5) > second_row_cubes_max_rangefinder_distance){
                velocity = -0.0125;
            }
            create3_velocity_set_components(velocity, 0);
        }
        create3_wait();
        create3_velocity_set_components(0,0);
        create3_wait();
        forward(2,1);
    }
    else{
    	forward(second_row_cubes_step_count,1);
    }
    claw_grab_habitat();
    msleep(500);
    arm_up();
    //Changeable values
    //msleep(5000);
    //***
    forward(-12,10);
    arm_down();
    msleep(500);
    claw_open();
    msleep(500);
    arm_up();
    /*rotate(70);
    forward(10,1);
    rotate(50); //[V2 30], [V3 50]
    //forward(-3,1);
    align_to_line();
    rotate(-90);
    align_to_line();
    forward(-5,1);
    rotate(50);
    //forward(0,1);
    slow_servo(arm_grab_position_degrees + 5, 1);
    claw_grab_habitat();
    for (int i = 0; i < 5; i += 1) {
        slow_servo(arm_grab_position_degrees - (i*80), 1);
        forward(0.125, 1);
    }
    arm_up();*/
    //forward(0,1);
    //rotate(-85);
    //forward(9,30);
}
void purple_tubes(){
    rotate(90);
    drive_until_bumped();
    forward(0,1);
    rotate(17);
    forward(1.5,1);
    slow_servo(arm_down_degrees+5,1);
    msleep(500);
    set_servo_position(claw_port,325);
    for(int i=0;i<2000;i++){
    	create3_velocity_set_components(-0.01,0);
   		create3_wait();
        msleep(1);
    }
    create3_velocity_set_components(0,0);
    create3_wait();
    msleep(500);
    arm_up();
    /*//Purple Noodle Code
    velocity = -0.015;
    while (velocity != 0) { // Code for horizontal alignment, taken from old habitat construction code
        if ((analog(rangefinder_port) > 1350)) {
            velocity = 0;
        }
        create3_velocity_set_components(0, velocity);
    }
    create3_wait();
    //rotate(-2); //Removed because over-compensation for create3 movements
    slow_servo(arm_grab_position_degrees + 5, 1);
    forward(2, 1);
    while (!(create3_sensor_ir(2) >= 27)) { //[V2 40], [V3 3]
        create3_velocity_set_components(0.015, 0);
        create3_wait();
    }
    claw_grab_habitat();
    for (int i = 0; i < 5; i += 1) {
        slow_servo(arm_grab_position_degrees - (i*80), 1);
        forward(0.125, 1);
    }
    arm_up();
    rotate(100);
    drive_until_bumped();
	rotate(5);
    arm_down();
    msleep(500);
    claw_open();
    msleep(500);
    arm_up();*/
}
