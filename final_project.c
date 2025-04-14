#include <8051.h>
#include "Keypad4x4.h" //if scan nothing, return 120

#define RELOAD (65536 - 1000) // timer0 interrupt counter

typedef struct 
{
    unsigned char note_num; // which note
    short duration; // pressing time 
} Recorded_note;

unsigned char time_accelerate = 0; // accelerate time
unsigned char pause = 0; // The world!
unsigned char count_0 = 0; 
unsigned char key = 13, old_key = 13;
unsigned char play_index = 0; //play the note of thwe index
unsigned char rec_ptr = 0; // recording the note of the pointer
unsigned char mode = 0; // 0: change tone on click 2: Record 3: Play Record
unsigned char beat = 13; // current beat of melody

const unsigned short note[] = {63628, 63731, 63836, 63928, 64021, 64103, 64185, 64261, 64331, 64400, 64436, 64524, 64582, 65535};// C4, C4#, D4, D4#/E4b, E4, F4, F4#, G4, G4#, A4, A4#, B4, C5, PAUSE


void delay(unsigned char t){
    for(int i = 0; i < t; i++)
        for(int j = 0; j < 100; j++);
}

const unsigned char light_num[] = { //7-seg light 
    0b11000000,
    0b11111001,
    0b10100100,
    0b10110000,
    0b10011001,
    0b10010010,
    0b10000010,
    0b11111000,
    0b10000000,
    0b10010000,
    0b10001000,
    0b00000000,
    0b10100111,
    0b01000000,
    0b00101111,
    0b10001110,
};

__xdata Recorded_note record[100];//record array

__xdata unsigned short tmp_duration[100] = {0};

void display(unsigned char n){
    P0 = light_num[n];
}

void timer0_isr (void) __interrupt (1) __using (3) // scan 100 times per second
{   
    // ------ timer 0 ------
    TH0 = (char)(RELOAD >> 8);
    TL0 = (char)RELOAD;

    count_0 ++;
    if(count_0 >= 10){
        if(mode == 1){
            record[rec_ptr].duration++; //every 0.01 second recording once
        }else if (mode == 2){
            if(!pause){
                if(time_accelerate)
                    record[play_index].duration -= 4; //every 0.01 second play a note
                else
                    record[play_index].duration -=2;
            }
        }
        
        count_0 = 0;
        key = Keypad4x4();
    }
}

void timer1_isr (void) __interrupt (3) __using (2) // speaker frequency
{   
    // ------ timer 1 ------
    TH1 = (char)(note[beat] >> 8);
    TL1 = (char)(note[beat]);
    P2_3 = !P2_3;
}

void main(void){

    EA = 1; // enable interrupt
    TMOD = 0b00010001; // all timer mode 1

    // ------ timer 0 ------ 
    TH0 = (char)(RELOAD >> 8);
    TL0 = (char)RELOAD;
    TR0 = 1; // Timer0 run
    ET0 = 1; // Timer0 interrupt enable

    // ------ timer 1 ------ 
    TH1 = (char)(RELOAD >> 8);
    TL1 = (char)(RELOAD);
    TR1 = 1; // Timer1 run
    ET1 = 1; // Timer1 interrupt enable

    P0 = 0x00; //7-seg a,b,c,d,e,f,g
    P1 = 0x00; //keyboard
    P2 = 0x00; // 7-seg D1,D2,D3,D4, speaker

    for(unsigned char i = 0; i < 100; i++){ //initialize the record array
        record[i].note_num = 13;
        record[i].duration = 1;

    }

    while(1){
        
        delay(15);

        if(key == 14){ //switch mode
            mode++;
            old_key = 120;
            key = old_key;
            play_index = 0;
            for(unsigned char j = 0; j < rec_ptr; j++)
                tmp_duration[j] = record[j].duration;
            if(mode > 2){
                mode = 0;
                rec_ptr = 0;
                pause = 0;
                for(unsigned char i = 0; i < 100; i++){ //initialize the record array
                    record[i].note_num = 13;
                    record[i].duration = 1;
                }
            }
        }

        if(key == 13 && mode == 2)
            time_accelerate = ~time_accelerate;

        if(key == 15)
            pause = ~pause;
        
        
        if(mode == 0){ // keep playing until next button pressed mode
            if (key != old_key && key != 120)
                old_key = key;
            if(old_key <= 13)
                beat = old_key;
            else 
                beat = 13;
            
            
            display(beat);

        }else if(mode == 1){ // record mode
            
            display(14); //only display r.
            if(key != old_key){ 
                rec_ptr++;
                if(key == 120)
                    key = 13;
                record[rec_ptr].note_num = key;
                old_key = key;
                beat = old_key;
            }

        }else if (mode == 2){ //play mode
            if(!pause){

                if(play_index > rec_ptr){
                    for(unsigned char i = 0; i < rec_ptr; i++)
                        record[i].duration = tmp_duration[i];
                    play_index = 0;
                }

                if(record[play_index].duration < 0)
                    play_index ++; 
                
                    

                beat = record[play_index].note_num;
                
                if(beat < 13)
                    display(beat);
                else
                    display(15);
            }
        }
    }
}