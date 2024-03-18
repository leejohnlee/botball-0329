#include <kipr/wombat.h>
 void front(int ms) {
       //motor(0,100);
      // motor(1,100);
     mav(0,2000);
     mav(1,2000);
       msleep(ms);
   }
int main()
{
    
   // motor(0,100);
    mav(0,2000);
    msleep(170);
    ao();
    
    front(5000);
    ao();
    
    //motor(1,100);
    mav(1,2000);
    msleep(1860);
    ao();
    
    front(1900);
    ao();
    
    //motor(0,100);
    mav(0,2000);
    msleep(1800);
    ao();
    
    front(2600);
    ao();
    
    //motor(0,100);
    mav(0,2000);
    msleep(1500);
    ao();
    
    front(3500);
    ao();
    //motor(0,-100);
    //motor(1,-115);
    //msleep(4500);
}
