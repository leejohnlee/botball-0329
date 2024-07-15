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
void align_to_line_backwards(); // Align the robot to a line using cliff sensors backwards

// Function declarations for arm and claw operations
void slow_servo(int servo_degrees, float speed); // Gradually move a servo to a target position at a specified speed
void arm_up(); // Move the arm up to a predefined position
void arm_down(); // Move the arm down to a predefined position
void arm_grab(); // Position the arm for grabbing cubes
void claw_open(); // Open the claw servo to a predefined position
void claw_close(); // Close the claw servo to a predefined position
void claw_start(); // Move the claw servo to a starting position
void pom_claw_open(); // Open the micro-servo claw
void pom_claw_close(); // Close the micro-servo claw
void arm_start(); // Move the arm servo to a starting position
void claw_grab_habitat(); // Adjust claw servo to grab habitat elements

int sensor_bumped(); // Get if the front touch sensor is triggered

// Function declarations for main code chunks
void initialize(); // Perform initial setup and connection to the robot
void pom_pyramid_area_with_sort(); // Execute actions related to sorting the poms in a designated area
void pom_pyramid_multipliers_with_switch(); // Execute actions related to fetching multipliers and flipping the switch
void orange_pom_area(); // Execute actions related to handling multiplier cubes in the orange pom area

// Global variables
int left_servo_port = 3; // Left servo motor port
int right_servo_port = 2; // Right servo motor port
int claw_port = 1; // Claw servo motor port
int pom_claw_port = 0; // Port for the micro-servo that is located under the main claw

double velocity = 0.15; // Velocity parameter for specific operations (e.g., bump detection)

int rangefinder_port = 5; // Port for the rangefinder sensor
int front_touch_sensor_port = 0; // Port for the front "bumper"

int claw_open_degrees = 900; // Degrees to open the claw, Original value 1045, V2 900
int claw_closed_degrees = 260; // Degrees to close the claw
int claw_start_degrees = 1680; // Degrees to set the claw at starting position
int claw_grab_degrees = 0; // Degrees to grab habitat cubes

int pom_claw_open_position = 0; // Position to open the pom claw
int pom_claw_closed_position = 780; // Position to close the pom claw

int arm_up_degrees = 700; // Degrees to raise the arm
int arm_down_degrees = 80; // Degrees to lower the arm
int arm_grab_position_degrees = 475; // Degrees for the arm to grab cubes Original value 450
int arm_start_degrees = 550; // Degrees to set the arm at starting position
int arm_sort_poms_position = 190; // Original value 150

// Main function
int main() {
    create3_connect();
    create3_drive_straight(0, 0.1);
    create3_wait();
    light_color(0, 255, 0, 0.25);
    initialize(); // Initialize the robot and perform setup

    // Execute sections of the total code
    pom_pyramid_area_with_sort();
    light_color(50, 50, 255, 1);
    pom_pyramid_multipliers_with_switch();
    light_color(50, 255, 50, 1);
    orange_pom_area();
    light_color(50, 255, 50, 1);

    return 0;
}

// Initialization function
void initialize() {
    // Perform initial setup and connection to the robot
    printf("Connection to the Create 3\n");
    create3_power_on();
    pause_until_button_press();
    printf("Play button pressed, initiating the robot.\n");
    create3_safe();
    msleep(100);
    enable_servos();
    msleep(100);
    arm_start();
    claw_start();
    pom_claw_open();
    printf("Initialization complete.\n");
}

// Pom pyramid area with sort
void pom_pyramid_area_with_sort() {
    forward(40, 0.2);
    rotate(-90);
    forward(16, 0.1);
    forward_until_line_side_sensors();
    align_to_line();
    arm_down();
    msleep(100);
    claw_open();
    msleep(100);
    arm_grab();
    msleep(200);
    claw_close();
    msleep(400);
    arm_up();
    rotate(90);
    create3_drive_straight(0.05, 0.1);
    msleep(200);
    create3_drive_straight(-0.05, 0.1);
    rotate(90);
    forward_until_line();
    align_to_line();
    rotate(90);
    forward(19, 0.075);
    rotate(90);
    claw_open();
    arm_down();
    arm_start();
    rotate(-90);
    forward(14, 0.05);
    forward_until_line();
    align_to_line();
    arm_grab();
    claw_open();
    forward(5, 0.1);
    arm_down();
    claw_close();
    arm_sort_poms_position();
    arm_grab();
    forward_until_line();
    align_to_line();
    rotate(90);
    forward(5, 0.1);
    claw_open();
    arm_start();
    forward(5, 0.1);
}

// Pom pyramid multipliers with switch
void pom_pyramid_multipliers_with_switch() {
    create3_drive_straight(-0.09, 0.05);
    align_to_line();
    claw_open();
    rotate(90);
    forward(18, 0.05);
    forward_until_line();
    align_to_line();
    rotate(90);
    forward(20, 0.1);
    rotate(90);
    claw_open();
    forward_until_line_side_sensors();
    align_to_line();
    rotate(-90);
    forward(9, 0.05);
    claw_close();
    arm_down();
    forward(10, 0.1);
    create3_drive_arc_degrees(0.1, 180, 0.1);
    create3_drive_arc_degrees(0.1, -180, 0.1);
    create3_drive_straight(0.1, 0.1);
    claw_open();
    arm_up();
    rotate(-90);
    forward(5, 0.1);
    forward_until_line_side_sensors();
    align_to_line();
    forward(6, 0.1);
    claw_close();
    forward(5, 0.1);
    rotate(-90);
    create3_drive_straight(0.1, 0.1);
    arm_grab();
    claw_open();
}

// Orange pom area
void orange_pom_area() {
    forward(20, 0.05);
    forward_until_line();
    align_to_line();
    rotate(90);
    forward(15, 0.1);
    forward_until_line();
    align_to_line();
    rotate(90);
    forward(5, 0.1);
    arm_down();
    claw_close();
    arm_up();
    rotate(180);
    forward(10, 0.1);
    create3_drive_arc_degrees(0.1, 90, 0.1);
    create3_drive_arc_degrees(0.1, -90, 0.1);
    create3_drive_straight(0.1, 0.1);
    claw_open();
    arm_up();
    rotate(-90);
    forward(5, 0.1);
    forward_until_line();
    align_to_line();
    forward(6, 0.1);
    claw_close();
    forward(5, 0.1);
    rotate(-90);
    create3_drive_straight(0.1, 0.1);
    arm_grab();
    claw_open();
}

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
            create3_sensor_bump(4) == 1 || create3_sensor_bump(5) == 1) {
            create3_drive_straight(0, velocity);
        } else {
            create3_drive_straight(velocity, velocity);
        }
    }
}

void rotate(int degrees) {
    // Rotate the robot by specified degrees
    double rotation_speed = 0.2;
    if (degrees > 0) {
        create3_drive_arc_degrees(rotation_speed, degrees, rotation_speed);
    } else {
        create3_drive_arc_degrees(rotation_speed, -degrees, -rotation_speed);
    }
}

void forward_until_line() {
    // Move forward until a line is detected by the cliff sensors
    while (create3_cliff_sensor(0) > 1000 || create3_cliff_sensor(1) > 1000 ||
           create3_cliff_sensor(2) > 1000 || create3_cliff_sensor(3) > 1000) {
        create3_drive_straight(0.05, 0.05);
    }
    create3_drive_straight(0, 0);
}

void forward_until_line_side_sensors() {
    // Move forward until a line is detected by side cliff sensors
    while (create3_cliff_sensor(1) > 1000 || create3_cliff_sensor(2) > 1000) {
        create3_drive_straight(0.05, 0.05);
    }
    create3_drive_straight(0, 0);
}

void swipe_poms() {
    // Perform an arc turn to push poms into an area
    create3_drive_arc_degrees(0.1, 360, 0.1);
    create3_wait();
}

void pause_until_button_press() {
    // Pause program execution until a button press is detected
    while (!button_press()) {
        msleep(10);
    }
}

void light_color(int r, int g, int b, float t) {
    // Set the light ring color to specified RGB values
    create3_set_leds(r, g, b);
    msleep((int)(t * 1000));
}

void align_to_line() {
    // Align the robot to a line using cliff sensors
    while (create3_cliff_sensor(1) > 1000 || create3_cliff_sensor(2) > 1000) {
        create3_drive_straight(-0.05, -0.05);
    }
    create3_drive_straight(0, 0);
}

void align_to_line_backwards() {
    // Align the robot to a line using cliff sensors backwards
    while (create3_cliff_sensor(1) > 1000 || create3_cliff_sensor(2) > 1000) {
        create3_drive_straight(0.05, 0.05);
    }
    create3_drive_straight(0, 0);
}

// Arm and claw operations
void slow_servo(int servo_degrees, float speed) {
    // Gradually move a servo to a target position at a specified speed
    float current_position = get_servo_position(0);
    float difference = (servo_degrees - current_position);
    float time = abs(difference) / speed;
    for (int i = 0; i < time; i++) {
        set_servo_position(0, current_position + difference / time);
        msleep(1);
    }
}

void arm_up() {
    // Move the arm up to a predefined position
    set_servo_position(left_servo_port, arm_up_degrees);
    msleep(400);
}

void arm_down() {
    // Move the arm down to a predefined position
    set_servo_position(left_servo_port, arm_down_degrees);
    msleep(400);
}

void arm_grab() {
    // Position the arm for grabbing cubes
    set_servo_position(left_servo_port, arm_grab_position_degrees);
    msleep(400);
}

void claw_open() {
    // Open the claw servo to a predefined position
    set_servo_position(claw_port, claw_open_degrees);
    msleep(400);
}

void claw_close() {
    // Close the claw servo to a predefined position
    set_servo_position(claw_port, claw_closed_degrees);
    msleep(400);
}

void claw_start() {
    // Move the claw servo to a starting position
    set_servo_position(claw_port, claw_start_degrees);
    msleep(400);
}

void pom_claw_open() {
    // Open the micro-servo claw
    set_servo_position(pom_claw_port, pom_claw_open_position);
    msleep(400);
}

void pom_claw_close() {
    // Close the micro-servo claw
    set_servo_position(pom_claw_port, pom_claw_closed_position);
    msleep(400);
}

void arm_start() {
    // Move the arm servo to a starting position
    set_servo_position(left_servo_port, arm_start_degrees);
    msleep(400);
}

void claw_grab_habitat() {
    // Adjust claw servo to grab habitat elements
    set_servo_position(claw_port, claw_grab_degrees);
    msleep(400);
}

// Function to check if the front touch sensor is triggered
int sensor_bumped() {
    // Get if the front touch sensor is triggered
    return digital(front_touch_sensor_port);
}
