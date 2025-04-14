#include "mbed.h"
#include "DoorAccess.h"

// Define keypad pin connections
#define ROW0 PTC8
#define ROW1 PTA5
#define ROW2 PTA4
#define ROW3 PTA12
#define COL0 PTD3
#define COL1 PTA2
#define COL2 PTA1

#define GREEN_LED LED1
#define RED_LED LED2

int main() {
    
    DoorAccess doorAccess(ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, GREEN_LED, RED_LED);
    doorAccess.run(); 
}