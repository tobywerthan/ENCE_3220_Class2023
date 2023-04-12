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

bool start = false;
bool buzzerActive = false;

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

// variables for keeping track of each digits value
volatile unsigned char count1 = 0;
volatile unsigned char count2 = 3;
volatile unsigned char count3 = 0;
volatile unsigned char count4 = 0;

/**
 * @brief Setup
 * @param 
 * @return  
 */
void setup() {
  // LEDs Pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button Pins
  pinMode(BUTTON_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), Button_1_ISR, RISING);
  pinMode(BUTTON_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), Button_2_ISR, RISING);

  // Buzer Pins
  pinMode(BUZZER, OUTPUT);

  // 7-Seg Display
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  // 7-Seg Display
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

  disp_on(10);
  disp_on(11);
  disp_on(12);
  disp_on(13);
}
/**
 * @brief Turn On Buzzer
 * @param
 * @return
 */
void Active_Buzzer()
{
  buzzerActive = true;
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
 * @brief Shifts bits through shift register 
 * @param num
 * @param digit  
 * @return  
 */
void Display(unsigned char num, int digit)
{
  disp_on(digit);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[num]);
  digitalWrite(LATCH, HIGH);
}
/**
 * @brief Displays each digit 
 * @param 
 * @return  
 */
void Display_Numbers()
{ 
  Display(count1, 10); 
  delay(2);
  Display(count2, 11);
  delay(2);
  Display(count3, 12); 
  delay(2);
  Display(count4, 13);
  delay(2);
}
/**
 * @brief Turns on a specific digit
 * @param digit
 * @return  
 */
void disp_on(int digit)
{
  disp_off();
  digitalWrite(digit, LOW);
}
/**
 * @brief Turns off all digits
 * @param 
 * @return  
 */
void disp_off()
{
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, HIGH);
}
/**
 * @brief Resets the timer to 30 seconds
 * @param
 * @return  
 */
void ResetTime(){
  count1 = 0;
  count2 = 3;
  count3 = 0;
  count4 = 0;
}
/**
 * @brief Button 1 interrupt handler (increments the count)
 * @param
 * @return  
 */
void Button_1_ISR()
{
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
  
  buttonState = digitalRead(BUTTON_1);
  digitalWrite(RED_LED, buttonState);

  if(count1 > 8){
    count1 = 0;
    if(count2 > 8){
      count2 = 0;
      if(count3 > 8){
        count3 = 0;
        if(count4 > 8){
          count4 = 0;
        } else{
          count4++;
        }
      } else{
        count3++;
      }
    }else{
      count2++; 
    }
    
  }else{
    count1++;
  }
}
/**
 * @brief Button 2 interrupt handler (pauses the timer, also acts as a reset)
 * @param digit
 * @return  
 */
void Button_2_ISR()
{ 
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
   
  buttonState = digitalRead(BUTTON_2);
  digitalWrite(GREEN_LED, buttonState);

  if(buzzerActive){
    buzzerActive = false;
    ResetTime();
  }

  start = !start;
}
/**
 * @brief Timer 1 interrupt handler (decrements the timer)
 * @param digit
 * @return  
 */
ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  if(start){
    if(count2 == 0 && count1 > 0){
      count1--;    
    }else if(count2 > 0 && count1 > 0){
      count1--;
    }
    if(count1 == 0 && count2 > 0){
      count2--;
      count1 = 9;
    }
    if(count3 > 0 && count2 == 0 && count1 == 0) {
      count3--; 
      count2 = 9;
    }
    if(count4 > 0 && count3 == 0 && count2 == 0 && count1 == 0){
      count4--; 
      count3 = 9;
    }
  }

}
/**
 * @brief Loop
 * @param
 * @return  
 */
void loop() {
    
  if(count1 == 0 && count2 == 0 && count3 == 0 && count4 == 0){
    Active_Buzzer();
  }
  Display_Numbers();
}
