# include "Keypad4x4.h"

char Keypad4x4(void){
    
    // Scan
    char tempR;
    for (char i = 0; i < 4; i++) {        // scan y-axis

        P1 = ~(0x10 << i);
        
        for (char j = 0; j < 4; j++) {    // scan x-axis
            tempR = P1 & 0x0F;
            tempR = tempR >> j;

            if (tempR % 2 == 0)
                return (i*4 + j);

        }
    }

    return 120; // scan nothing
}