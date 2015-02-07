/*****************************************
 * Title: 74HC595 Shift Register Example
 * Description: This code can be used to control multiple 74HC595 shift registers
 * connected to an Arduino. It includes options to use the Output Enable pin for
 * limited PWM control, as well as easy-to-use settings to use any number of shift
 * registers.
 * 
 * GitHub Repo: http://github.com/mwhprojects/Arduino-74HC595
 *****************************************/


///////////////////////////////////////////// Shift Register Code
int latchPin = 2;  // Latch pin
int clockPin = 3;  // Clock pin
int dataPin = 4;  // Data pin
int activeLOW = 0;  // Set to 1 if LEDs are active low.
int numofSRs = 4;  // Number of shift registers. (SR boards have 2 per board.)
int oePin[] = {
  5};  // If you are using any Output Enable pins, add them to this array.

// Initialize data bytes. Add a B00000000 for each shift register.
byte SR[] = {
  B00000000,B00000000,B00000000,B00000000
};

// The shift register function that will output the SR[] bytes to the shift registers.
void SRout() {
  int aa=0;
  int bb=0;

  // Adjust for active LOW LED setup, if applicable.
  if(activeLOW == 1){
    for(int i=0; i<numofSRs; i++){
      SR[i] = ~SR[i];
    }
  }

  /*
    A shift register works by pulsing a clock while changing the logic level on the
   data pin to high or low. Once all of the data has been clocked onto the
   shift registers, the latch pin is then set high to push the data that was
   clocked in onto the parallel output pins.
   */
  digitalWrite(latchPin, 0);  // Set latch pin low.

  // Data is clocked onto the shift register "backwards".
  for(bb = numofSRs-1; bb >= 0; bb--){
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    for (aa=0; aa<=7; aa++)  {
      digitalWrite(clockPin, 0);  // Set clock pin to low.

      /*
        Shift b00000001 to the left and mask it (and it) with the byte on each loop
       of the current for-loop.
       
       If the checked bit is low, the result of the masking will be zero. Otherwise,
       it will be greater than zero, which is a true condition to the if-statement.
       */
      if (SR[bb] & (1<<aa)){
        // If the result of the mask is not/greater than zero, this bit is high.
        digitalWrite(dataPin, 1);
      }
      else{  
        // If the result of the mask is zero, this bit is low.
        digitalWrite(dataPin, 0);
      }

      digitalWrite(clockPin, 1);  // Set clock pin to high to shift the data bit on.
    }

    digitalWrite(clockPin, 0);  // Set the clock pin to low.
  }

  // When all bits have been shifted in, set latch pin to high.
  digitalWrite(latchPin, 1);
}

// Function to set brightness using Output Enable pin.
void setBrightness(int SRselect, int SRsetting){
  analogWrite(oePin[SRselect], 255-SRsetting);
}

///////////////////////////////////////////// SETUP
void setup() {  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for(int cc = 0; cc < sizeof(oePin); cc++){
    pinMode(oePin[cc], OUTPUT);
  }
}

// Some variables to use.
int a, b, c, d;

///////////////////////////////////////////// LOOP
void loop() {  
  // Whenever you change the bits in the bytes, remember to call SRout()!

  // Carnival lights
  for(a=0; a<8; a++){
    setBrightness(0, 225);
    SR[0] = B10010010;
    SR[1] = B01001001;
    SR[2] = B00100100;
    SR[3] = B10010010;
    SRout();
    delay(150);
    SR[0] = B01001001;
    SR[1] = B00100100;
    SR[2] = B10010010;
    SR[3] = B01001001;
    SRout();
    delay(150);
    SR[0] = B00100100;
    SR[1] = B10010010;
    SR[2] = B01001001;
    SR[3] = B00100100;
    SRout();
    delay(150);
  }

  // Fade in and out every other LED
  for(c=0; c<2; c++){
    SR[0] = B10101010;
    SR[1] = B10101010;
    SR[2] = B10101010;
    SR[3] = B10101010;
    SRout();
    for(b=0; b<255; b++){
      setBrightness(0, b);
      delay(3);
    }
    for(b=255; b>0; b--){
      setBrightness(0, b);
      delay(3);
    }

    SR[0] = B01010101;
    SR[1] = B01010101;
    SR[2] = B01010101;
    SR[3] = B01010101;
    SRout();
    for(b=0; b<255; b++){
      setBrightness(0, b);
      delay(3);
    }
    for(b=255; b>0; b--){
      setBrightness(0, b);
      delay(3);
    }
  }

  // Fill up, empty down.
  setBrightness(0, 255);
  SR[0] = 0;
  SR[1] = 0;
  SR[2] = 0;
  SR[3] = 0;
  for(d=0; d<4; d++){
    for(c=0; c<=8; c++){
      SR[d] = 65280 >> c;
      SRout();
      delay(100);
    }
  }
  for(d=0; d<4; d++){
    for(c=1; c<=8; c++){
      SR[d] = 255 >> c;
      SRout();
      delay(100);
    }
  }

}



