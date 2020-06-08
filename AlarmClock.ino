#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int seconds = 59;
int mintues = 59;
int hours = 12;
int tempTime = hours - 12;

int testAlramMintues = 1;
int testAlarmHours = 1;

int melody[] = {660,660,660,510,660,770,380,510,380,320,440,
                 480,450,430,380,660,760,860,700,760,660,520,
                 580,480,510,380,320,440,480,450,430,380,660,
                 760,860,700,760,660,520,580,480,500,760,720,
                 680,620,650,380,430,500,430,500,570,500,760,
                 720,680,620,650,1020,1020,1020,380,500,760,
                 720,680,620,650,380,430,500,430,500,570,585,
                 550,500,380,500,500,500,500,760,720,680,620,
                 650,380,430,500,430,500,570,500,760,720,680,
                 620,650,1020,1020,1020,380,500,760,720,680,
                 620,650,380,430,500,430,500,570,585,550,500,
                 380,500,500,500,500,500,500,500,580,660,500,
                 430,380,500,500,500,500,580,660,870,760,500,
                 500,500,500,580,660,500,430,380,660,660,660,
                 510,660,770,380};
int beats[] = {100,100,100,100,100,100,100,100,100,100,100,80,
               100,100,100,80,50,100,80,50,80,80,80,80,100,100,
               100,100,80,100,100,100,80,50,100,80,50,80,80,80,
               80,100,100,100,100,150,150,100,100,100,100,100,
               100,100,100,100,100,150,200,80,80,80,100,100,100,
               100,100,150,150,100,100,100,100,100,100,100,100,
               100,100,100,100,100,100,100,100,100,150,150,100,
               100,100,100,100,100,100,100,100,100,150,200,80,
               80,80,100,100,100,100,100,150,150,100,100,100,100,
               100,100,100,100,100,100,100,100,100,60,80,60,80,80,
               80,80,80,80,60,80,60,80,80,80,80,80,60,80,60,80,80,
               80,80,80,80,100,100,100,100,100,100,100};
 
int baselineTemp = 0;
int celsius = 0;
int fahrenheit = 0;

int timeFormatButtonState = LOW;
int setAlarmButtonState = LOW;
int switchAlarmButtonState = LOW;
int tempButtonState = LOW; 

boolean alarmEnabled = true;
boolean snoozeAlarm = true;

boolean thermometerEnabled = true;
boolean isCelsius = true;

boolean clockMode = true;
int switchDayTime = 1;

byte none[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte am[] = {
  B00100,
  B01110,
  B01010,
  B00000,
  B01010,
  B01110,
  B01010,
  B00000
};

byte pm[] = {
  B00100,
  B01110,
  B01000,
  B00000,
  B01010,
  B01110,
  B01010,
  B00000
};

byte bell[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

byte thermo[] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

void configureLCD(){
  //Setting up LCD grid
  lcd.setCursor(6, 0); lcd.print(":");
  lcd.setCursor(9, 0); lcd.print(":");
  
  lcd.createChar(0, none);
  lcd.createChar(1, pm);
  lcd.createChar(2, am);
  lcd.createChar(3, bell);
  lcd.createChar(4, thermo);

  //Formating clock 
  if (hours <= 9) {
    lcd.setCursor(4, 0); lcd.print("0");
    lcd.setCursor(5, 0); lcd.print(hours);
  }
  else if (hours > 9) {
    lcd.setCursor(4, 0); lcd.print(hours);
  }

  if (mintues <= 9) {
    lcd.setCursor(7, 0); lcd.print("0");
    lcd.setCursor(8, 0); lcd.print(mintues);
  }
  else if (mintues > 9) {
    lcd.setCursor(7, 0); lcd.print(mintues);
  }


  if (seconds <= 9) {
    lcd.setCursor(10, 0); lcd.print("0");
    lcd.setCursor(11, 0); lcd.print(seconds);
  }
  else if (seconds > 9) {
    lcd.setCursor(10, 0); lcd.print(seconds);
  }
  
  //Checking DayTime status
  if (switchDayTime == 0){
    lcd.setCursor(12, 0);
    lcd.write(byte(1));
  }
  else if (switchDayTime == 1){
    lcd.setCursor(12, 0);
    lcd.write(byte(2));
  }
  else if (switchDayTime == 2){
    lcd.setCursor(12, 0);
    lcd.write(byte(0));
  }
  
  //Checking Alarm status
  if(alarmEnabled){
  	lcd.setCursor(3, 0);
    lcd.write(byte(3));
  }
  else if(!alarmEnabled){
    lcd.setCursor(3, 0);
    lcd.write(byte(0));
  }
  
  //Checking Thermometer
  if(thermometerEnabled){
    lcd.setCursor(5, 1);
    if(isCelsius){
    	lcd.write(byte(4));
    	lcd.setCursor(7, 1); lcd.print(celsius);
        if(celsius <= 100){
          lcd.write(byte(0));
          lcd.setCursor(10, 1);lcd.print("C");
      }
    }
    else if(!isCelsius){
      	lcd.write(byte(4));
    	lcd.setCursor(7, 1); lcd.print(fahrenheit);
        if(celsius <= 100){
          lcd.write(byte(0));
          lcd.setCursor(10, 1);lcd.print("F");
      }
    }
  }
}

//Switching the clock system from am/pm to 24
void switchClockSystem(boolean selectMode){
  if(seconds >= 60){
   mintues++;
   seconds =0;   
  }
  
  if(mintues >= 60){
   hours++;
   tempTime++;
   mintues=0;
  }
  
  if(!clockMode){
    switchDayTime = 2;
  	if(hours >= 24){
  	 	hours = 0; 
      	tempTime = 0; 
 	 }
  }
  else if(clockMode){
    if(hours > 12){
      if(tempTime < 12){
      	switchDayTime = 1;
        hours = 1; 
        tempTime = 1; 
      }
      else{
        switchDayTime = 0;
      }
    }
  }
}

//Alarm clock code
void alarmClock(int alarmMintues, int alarmHours){
  if(mintues == alarmMintues && hours == alarmHours){
    if(snoozeAlarm == true){
      alarmMintues =+ 5;
      Serial.println(alarmMintues);
    }
    else{
      Serial.println("Alarm Detected!");
      buzzerSound();
    }
  }
  else{
  	//Serial.println("Alarm Not Detected!");
    noTone(6);
  }
}

//Alarm clock sound
void buzzerSound(){
  for (int note = 0; note < 156; note++) {

    int beat = beats[note];
    tone(6, melody[note], beat);

    int tempBeat[] = {150,300,300,100,300,550,575,450,
                      400,500,300,330,150,300,200,200,
                      150,300,150,350,300,150,150,500,
                      450,400,500,300,330,150,300,200,
                      200,150,300,150,350,300,150,150,
                      500,300,100,150,150,300,300,150,
                      150,300,150,100,220,300,100,150,
                      150,300,300,300,150,300,300,300,
                      100,150,150,300,300,150,150,300,
                      150,100,420,450,420,360,300,300,
                      150,300,300,100,150,150,300,300,
                      150,150,300,150,100,220,300,100,
                      150,150,300,300,300,150,300,300,
                      300,100,150,150,300,300,150,150,
                      300,150,100,420,450,420,360,300,
                      300,150,300,150,300,350,150,350,
                      150,300,150,600,150,300,350,150,
                      150,550,325,600,150,300,350,150,
                      350,150,300,150,600,150,300,300,
                      100,300,550,575};
    delay(tempBeat[note]);
  }
  noTone(6);
}

//Thermometer code
void thermometer(){
  baselineTemp = 40;
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);
  fahrenheit = ((celsius * 9) / 5 + 32);
}

//Button control code
void buttons(){
  timeFormatButtonState = digitalRead(13);
  setAlarmButtonState = digitalRead(10);
  switchAlarmButtonState = digitalRead(9);
  tempButtonState = digitalRead(8); 
  
  if(timeFormatButtonState == HIGH){
    if(clockMode){
      clockMode = false;
      switchDayTime = 0;
    }
    else{
      clockMode = true;
      switchDayTime = 1;
    }
  }
  else if(switchAlarmButtonState == HIGH){
    if(alarmEnabled){
      alarmEnabled = false;
    }
    else if(!alarmEnabled){
      alarmEnabled = true;
    }
    else{
      snoozeAlarm = true;
    }
  }
  else if(tempButtonState == HIGH){
    if(isCelsius){
      isCelsius = false;
    }
    else{
      isCelsius = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  //Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  //Set up the Thermometer
  pinMode(A0, INPUT);
  
  //Set up the Buzzer
  pinMode(6,OUTPUT);
  
  //Setup the buttons
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(13, INPUT);
  
  cli();//disable global interrupts
  
   //T1
  TCCR1A = 0; // Clear registers and timer value
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 3037;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12); //CTC
  TCCR1B |= (1 << CS12) | (1 << CS10);  //Prescaler is 1024
  TIMSK1 |= (1 << OCIE1A); //Enable
  
  sei();//Allow global interrupts
}

void loop() {  
  configureLCD();
  
  switchClockSystem(clockMode);
  
  alarmClock(testAlramMintues, testAlarmHours);
  
  thermometer();
  
  buttons();
}

ISR(TIMER1_COMPA_vect){
  seconds++;
}
 