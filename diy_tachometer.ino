#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

volatile unsigned int count;
unsigned int rpm;
bool too_slow = false;

void setup() {
  pinMode(2,INPUT);
  lcd.init();
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(2),fallingISR,FALLING);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |=(1<<CS12);//Set prescaler 256
  TIMSK1 |=(1<<TOIE1);//Set timer1 in overflow mode
  interrupts();
}

void loop() {
  rpm = 60*(62500/count);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(rpm);

  if(too_slow==true){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TOO SLOW!");
  }
}

void fallingISR(){
  TCCR1B &= ~(1 << CS12);//stop timer
  count = TCNT1;//load the count
  TCNT1 = 0;//reset the timer
}

ISR(TIMER1_OVF_vect){
  too_slow = true;
}
