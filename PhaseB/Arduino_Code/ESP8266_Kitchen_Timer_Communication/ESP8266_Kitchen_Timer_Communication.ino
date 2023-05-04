String gMsg = "";
volatile unsigned char gISRFlag1   = 0;
unsigned int gReloadTimer1 = (62500 * 4) / 1000; // corresponds to 1 second

/**
  *@brief program setup
  *@return void
*/
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = gReloadTimer1;    // compare match register 1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

/**
  *@brief continuous loop that executes after setup()
  *@return void
*/
void loop() {
  if(gISRFlag1 == 1){
    gISRFlag1 = 0;
    String gMsg = Serial.readStringUntil('\n');
    parseCMD(gMsg);
  }
}

/**
  *@brief takes in a string and turns on and off an led based on the string's value
  *@param msg: string that is meant to represent a command
  *@return void
*/
void parseCMD(String msg){
    if(msg == "$STR"){
      digitalWrite(13, HIGH);
    } else if(msg == "$STP"){
      digitalWrite(13, LOW);
    }
}

/**
  *@brief interrupt for timer 1 that executes every 1s
  *@return void
*/
ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  if(Serial.available() > 0){
    gISRFlag1 = 1;
  }
}
