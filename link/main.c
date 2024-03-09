#include <kipr/wombat.h>

void move_forward(int ms);
void move_backwards(int ms);
void turn_right(int ms);
void turn_left(int ms);

int main() {
    //starting
    wait_for_light(0);
    turn_left(170);

    //start 1st straight
    move_forward(5000);
   
    //turn right
    turn_right(1860);

    //drive on bump
    move_forward(1900);
    
    //turn left off of bump
    turn_left(1800);
    
    //towards 2nd to last rock
    move_forward(2600);
    
    //turn left towards the rock heap
    turn_left(1500);
    
    //drive into the rock heap
    move_forward(3500);

    //move out of icreate's path
    move_backwards(4500);

}

//~~~~ FUNCTIONS ~~~~//

//move forward
void move_forward(int ms) {
    motor(0, 100);
    motor(1, 100);
    msleep(ms);
    ao();
}

//go backwards
void move_backwards(int ms) {
    motor(0,-100);
    motor(1,-100);
    msleep(ms);
    ao();
}


//turn right
void turn_right(int ms) {
    motor(1, 100);
    msleep(ms);
    ao();

}

//turn left  
void turn_left(int ms) {
    motor(0, 100);
    msleep(ms);

}

