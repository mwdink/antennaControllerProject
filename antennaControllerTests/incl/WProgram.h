#ifndef WProgram_h
#define WProgram_h

//this is a mock version of WProgram.h


#define HIGH 1
#define LOW  0
#define INPUT 0x0
#define OUTPUT 0x1

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);



#endif
