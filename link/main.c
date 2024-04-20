

#include <kipr/wombat.h>

void move_forward(int ms);
void move_backwards(int ms);
void turn_right(int ms);
void turn_left(int ms);
void mav_move_forward(int ms);
void mav_move_backwards(int ms);
void mav_turn_right(int ms);
void mav_turn_left(int ms);

int main() {
    //starting
    wait_for_light(0);
    shut_down_in(119);
    turn_left(170);

    //start 1st straight
    move_forward(4600);
   
    //turn right
    turn_right(1700);

    //drive on bump
    move_forward(1700);
    
    //turn left off of bump
    turn_left(1700);
    
    //towards 2nd to last rock
    move_forward(2450);
    
    //turn left towards the rock heap
    turn_left(1500);
    
    //drive into the rock heap
    move_forward(3300);
    
    //curve the rocks into rock heap more securly
    turn_right(500);
    
    //turn back to original position
    turn_left(500);

    //move out of icreate's path
    move_backwards(4300);
    
    motor(0,-100);
    msleep(1250);
    ao();

}

//~~~~ FUNCTIONS ~~~~//

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
