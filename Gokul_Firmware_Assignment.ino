#define LM35 A0
const int LED = 13;
/*
16 Mhz Arduno frequency.1/16 Mhz is one count
Timer 1 is of 16 bit so using the registers and prescale
2^16 is 65536-2^15+2^14.....1
for 1 sec with prescale of 256 
1=1/16000000*(256)*62500
*/
//for 250 and 500ms values
const uint16_t tmr_reg_250ms = 15625;
const uint16_t tmr_reg_500ms = 31250;
bool status = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  //Turning LED Off
  digitalWrite(LED,LOW);
  TCCR1A = 0;
  //setting pre-scale to 256
  TCCR1B |= (1<<CS12);
  TCCR1B &= ~(1<<CS11);
  TCCR1B &= ~(1<<CS10);

  //seting threshlod value for registers
  TCNT1 = 0;
  //setting value to be compared
  OCR1A = tmr_reg_250ms;
  //setting timer mode to compare mode
  TIMSK1 = (1 << OCIE1A);
  //enable all interrupts
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  float LM35_val = analogRead(LM35);
  float temp = (LM35_val * 500 ) / 1023 ;
  Serial.println(temp);
  if( temp <=  30 )
  {
    //blink led at 250ms
    OCR1A = tmr_reg_250ms;
  }
  else
  {
    //blink led at 500ms
    OCR1A = tmr_reg_500ms;
  }

}
//using timer1 compare mode isr
ISR(TIMER1_COMPA_vect)
{
  //reset timer value
  TCNT1 = 0;
  status = !status;
  digitalWrite(LED,status);
}