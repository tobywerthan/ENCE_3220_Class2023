#define BUTTON_1  2
#define BUTTON_2  3
#define GREEN_LED 4
#define RED_LED   5
#define BUZZER    6

#define DATA      9
#define LATCH     8
#define CLOCK     7

#define DIGIT_4   10
#define DIGIT_3   11
#define DIGIT_2   12
#define DIGIT_1   13

bool gTimerStart = false;
bool gBuzzerActive = false;

unsigned char gHexValTable[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

volatile unsigned char gCountOne = 0;
volatile unsigned char gCountTwo = 3;
volatile unsigned char gCountThree = 0;
volatile unsigned char gCountFour = 0;

/**
  *@brief Program setup
  *@return void
*/
void setup() {
  // LEDs Pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button Pins
  pinMode(BUTTON_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), buttonOneISR, RISING);
  pinMode(BUTTON_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), buttonTwoISR, RISING);

  // Buzer Pins
  pinMode(BUZZER, OUTPUT);

  // 7-Seg display
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  // 7-Seg display
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  // Shift Register Pins
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);

  // timer 1 setup
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 62500;            // compare match register 16MHz/256
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

  dispOn(10);
  dispOn(11);
  dispOn(12);
  dispOn(13);
}

/**
  *@brief Activates the buzzer sequence when the count reaches zero
  *@return void
*/
void activeBuzzer()
{
  gBuzzerActive = true;
  unsigned char i;
  unsigned char sleep_time = 1; // ms
  
  for(i=0;i<100;i++)
   {
    digitalWrite(BUZZER,HIGH);
    delay(sleep_time);//wait for 1ms
    digitalWrite(BUZZER,LOW);
    delay(sleep_time);//wait for 1ms
    }
}

/**
  *@brief Program setup
  *@param num: the value to display
  *@param digit: the pin number of the digit to display
  *@return void
*/
void display(unsigned char num, int digit)
{
  dispOn(digit);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, gHexValTable[num]);
  digitalWrite(LATCH, HIGH);
}

/**
  *@brief displays each digit with a delay of 2ms between
  *@return void
*/
void displayNumbers()
{ 
  display(gCountOne, 10); 
  delay(2);
  display(gCountTwo, 11);
  delay(2);
  display(gCountThree, 12); 
  delay(2);
  display(gCountFour, 13);
  delay(2);
}

/**
  *@brief turns on a specific digit
  *@param digit: pin of the digit to turn on
  *@return void
*/
void dispOn(int digit)
{
  dispOff();
  digitalWrite(digit, LOW);
}

/**
  *@brief turns off the display
  *@return void
*/
void dispOff()
{
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, HIGH);
}

/**
  *@brief resets the count to 30 seconds
  *@return void
*/
void resetTime(){
  gCountOne = 0;
  gCountTwo = 3;
  gCountThree = 0;
  gCountFour = 0;
}

/**
  *@brief interrupt for the button that increments the count
  *@return void
*/
void buttonOneISR()
{
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
  
  buttonState = digitalRead(BUTTON_1);
  digitalWrite(RED_LED, buttonState);

  if(gCountOne > 8){
    gCountOne = 0;
    if(gCountTwo > 8){
      gCountTwo = 0;
      if(gCountThree > 8){
        gCountThree = 0;
        if(gCountFour > 8){
          gCountFour = 0;
        } else{
          gCountFour++;
        }
      } else{
        gCountThree++;
      }
    }else{
      gCountTwo++; 
    }
    
  }else{
    gCountOne++;
  }
}

/**
  *@brief interrupt for the button that pauses and resets the timer
  *@return void
*/
void buttonTwoISR()
{ 
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
   
  buttonState = digitalRead(BUTTON_2);
  digitalWrite(GREEN_LED, buttonState);

  if(gBuzzerActive){
    gBuzzerActive = false;
    resetTime();
  }

  gTimerStart = !gTimerStart;
}

/**
  *@brief interrupt for timer 1 that occurs every second to decrement the count
  *@return void
*/
ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  if(gTimerStart){
    if((gCountTwo == 0) && (gCountOne > 0)){
      gCountOne--;    
    }else if((gCountTwo > 0) && (gCountOne > 0)){
      gCountOne--;
    }
    if((gCountOne == 0) && (gCountTwo > 0)){
      gCountTwo--;
      gCountOne = 9;
    }
    if((gCountThree > 0) && (gCountTwo == 0) && (gCountOne == 0)) {
      gCountThree--; 
      gCountTwo = 9;
    }
    if((gCountFour > 0) && (gCountThree == 0) && (gCountTwo == 0) && (gCountOne == 0)){
      gCountFour--; 
      gCountThree = 9;
    }
  }

}

/**
  *@brief Continuous loop that executes after setup()
  *@return void
*/
void loop() {  
  if(gCountOne == 0 && gCountTwo == 0 && gCountThree == 0 && gCountFour == 0){
    activeBuzzer();
  }
  displayNumbers();
}
