#include <pitches.h> // < > で囲む
#include <Wire.h>
#include "rgb_lcd.h"

#define PINSP 8  //圧電スピーカーのピン番号
#define BEAT 30 //音の長さを指定
#define PRESS 1000

rgb_lcd lcd;
const int buttonPin = 7;
const int C_pin = 1;
const int D_pin = 2;
const int E_pin = 3;
const int F_pin = 4;
const int G_pin = 5;
const int Bee[2][8]={{G_pin,F_pin,E_pin,D_pin,E_pin,F_pin,D_pin,C_pin},
{0,600,1200,2400,2700,3000,3300,3600}};
int play_notes[2][8];

unsigned long time_m = 0;
unsigned long push_time = 0;
int push = 0;
int C_value = 0;
int D_value = 0;
int E_value = 0;
int F_value = 0;
int G_value = 0;

boolean mode = 0;
boolean fullcombo=true;
int buttonState=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize digital pin 8 as an output. 
  pinMode(buttonPin, INPUT);
}


void loop() {
 // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  C_value = analogRead( C_pin );
  D_value = analogRead( D_pin );
  E_value = analogRead( E_pin );
  F_value = analogRead( F_pin );
  G_value = analogRead( G_pin );

  //analogWrite(led_pin, C_value/4 );
  //Serial.println( byte(C_value/4) );
  time_m=millis();

  if (buttonState == HIGH) {
    mode = 1;
  } 
  if(C_value<PRESS){
    tone(PINSP,NOTE_C6,BEAT) ;  // ド
    if(push == 0){
      Serial.print("NOTE_C pushed:");
      Serial.println(time_m);
      push = C_pin;
      
    }
     //delay(BEAT*2) ;
  }else if(D_value<PRESS){
    tone(PINSP,NOTE_D6,BEAT) ;  // レ
    if(push == 0){
      Serial.print("NOTE_D pushed:");
      Serial.println(time_m);
      push = D_pin;
    }
    //delay(BEAT*2) ;
  }else if(E_value<PRESS){
    tone(PINSP,NOTE_E6,BEAT) ;  // ミ
    if(push == 0){
      Serial.print("NOTE_E pushed:");
      Serial.println(time_m);
      push = E_pin;
    }
    //delay(BEAT*2) ;
  }else if(F_value<PRESS){
    tone(PINSP,NOTE_F6,BEAT) ;  // ファ
    if(push == 0){
      Serial.print("NOTE_F pushed:");
      Serial.println(time_m);
      push = F_pin;
    }
    //delay(BEAT*2) ;
  }else if(G_value<PRESS){
    tone(PINSP,NOTE_G6,BEAT) ;  // ソ
    if(push == 0){
      Serial.print("NOTE_G pushed:");
      Serial.println(time_m);
      push = G_pin;
    }
    //delay(BEAT*2) ;
  }else if(push!=0){
    Serial.print("Button relese:");
    Serial.println(time_m);
    push = 0; 
  }

  if(mode == true){
    game();
  }
}

void game(){
  static boolean start = false;
  static int note_number = 0;
  static int start_time = 0;
  static boolean long_note = false;
  
  if(start == false){ //開始時メトロノーム音、スタート時の時刻
    Serial.print("Game Start!:");
    Serial.println(time_m);
    lcd.print("Game Start!:");
    lcd.setCursor(12, 0);
    delay(595);
    tone(PINSP,NOTE_G2,100) ;  // ソ
    lcd.print("1");
    Serial.println("1");
    delay(595);
    tone(PINSP,NOTE_G2,100) ;  // ソ
    lcd.print("2");
    Serial.println("2");
    delay(595);
    tone(PINSP,NOTE_G2,100) ;  // ソ
    lcd.print("3");
    Serial.println("3");
    delay(595);
    tone(PINSP,NOTE_G2,100) ;  // ソ
    lcd.print("4");
    Serial.println("4");
    delay(590);
    start_time = millis();
    start = true;
    lcd.setCursor(0, 0);
    lcd.print("                ");
    }else if(push!=0 && long_note==false){ //押した瞬間の処理(どれをいつ押したか)
    play_notes[0][note_number]=push;
    play_notes[1][note_number]=time_m-start_time-Bee[1][note_number]; 
    long_note=true;
    note_number++;
  }else if(push==0){ //鍵盤から指を離した際の処理
    long_note=false;
  }

  if(note_number == 8){ //終了処理
    Serial.print("start_time:");
    Serial.println(start_time);
    music(judge(start_time));
    Serial.println("play_notes[0]:");
    for(int i=0;i<8;i++){
      Serial.println(play_notes[0][i]);
    }
    Serial.println("play_notes[1]:");
    for(int i=0;i<8;i++){
      Serial.println(play_notes[1][i]);
    }

    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    start = false;
    fullcombo=true;
    note_number = 0;
    mode = false;
  }
}

int judge(int s_time){
  unsigned int error=0;
  for(int i=0;i<8;i++){
    error+=abs(play_notes[1][i]);
    if(play_notes[0][i]!=Bee[0][i]){
      error += 600;
      fullcombo=false;
      Serial.println("MISS");
    }
  }

  Serial.print("Error:");
  Serial.println(error);
  float errp = (float)error/4800;
  Serial.print("ErrorP:");
  Serial.println(errp);
  float per = 1-errp;
  if(per<0){
    per=0;
  }
  Serial.print("%:");
  Serial.println(per*100);
  lcd.setCursor(10, 1); 
  lcd.print(per*100);
  lcd.print("%");
  lcd.setCursor(0, 0);
  if(per>=0.95){
    Serial.println("RANK:S");
    lcd.print("  S RANK CLEAR!!");
    return 6; //RANK S
  }else if(per>=0.90){
    Serial.println("RANK:AAA");
    lcd.print("AAA RANK CLEAR!!");
    return 5; //RANK AAA
  }else if(per>=0.85){
    Serial.println("RANK:AA");
    lcd.print(" AA RANK CLEAR!!");
    return 4;  //RANK AA
  }else if(per>=0.80){
    Serial.println("RANK:A");
    lcd.print("  A RANK CLEAR!!");
    return 3;  //RANK A
  }else if(per>=0.70){
    Serial.println("RANK:B");
    lcd.print("  B RANK CLEAR!!");
    return 2;  //RANK B
  }else if(per>=0.60){
    Serial.println("RANK:C");
    lcd.print("  C RANK CLEAR!!");
    return 1;  //RANK C
  }else{
    Serial.println("RANK:F");
    lcd.print("  FAILED");
    return 0;  //RANK F
  }
}

void music(int music){
  if(fullcombo==true){
    delay(195);
    lcd.setCursor(0, 1);
    Serial.println("FULLCOMBO");
    tone(PINSP,NOTE_E6,100) ;
    delay(95);
    tone(PINSP,NOTE_G6,100) ;
    delay(95);
    tone(PINSP,NOTE_C7,100) ;
    delay(95);
  }
  delay(1200);
  switch(music){
    case 6:
      tone(PINSP,NOTE_C6,100) ;
      delay(145);
      tone(PINSP,NOTE_D6,100) ;
      delay(145);
      tone(PINSP,NOTE_E6,100) ;
      delay(145);
      tone(PINSP,NOTE_F7,100) ;
      delay(145);
      tone(PINSP,NOTE_G6,100) ;
      delay(145);
      tone(PINSP,NOTE_A6,100) ;
      delay(145);
      tone(PINSP,NOTE_B6,100) ;
      delay(145);
      tone(PINSP,NOTE_C7,100) ;
      delay(145);
      tone(PINSP,NOTE_D7,100) ;
      delay(145);
      tone(PINSP,NOTE_E7,100) ;
      delay(145);
      tone(PINSP,NOTE_F7,100) ;
      delay(145);
      tone(PINSP,NOTE_G7,100) ;
      delay(145);
      tone(PINSP,NOTE_A7,100) ;
      delay(145);
      tone(PINSP,NOTE_B7,100) ;
      delay(145);
      tone(PINSP,NOTE_C8,100) ;
      delay(145);
      break;

    case 5:

//      break;
    case 4:

//      break;    

    case 3:

//      break;
    case 2:

//      break;
    case 1:
      tone(PINSP,NOTE_C6,100) ;
      delay(195);
      tone(PINSP,NOTE_E6,100) ;
      delay(195);
      tone(PINSP,NOTE_G6,100) ;
      delay(195);
      tone(PINSP,NOTE_C7,100) ;
      delay(195);      
      tone(PINSP,NOTE_C6,100) ;
      delay(195);
      tone(PINSP,NOTE_E6,100) ;
      delay(195);
      tone(PINSP,NOTE_G6,100) ;
      delay(195);
      tone(PINSP,NOTE_C7,100) ;
      delay(195);
      break;
    case 0:
      tone(PINSP,NOTE_G6,50) ;
      delay(45);
      tone(PINSP,NOTE_F6,50) ;
      delay(45);
      tone(PINSP,NOTE_E6,50) ;
      delay(45);
      tone(PINSP,NOTE_D6,50) ;
      delay(45);
      tone(PINSP,NOTE_C6,50) ;
      delay(45);
      tone(PINSP,NOTE_B5,50) ;
      delay(45);
      tone(PINSP,NOTE_A5,50) ;
      delay(45);
      tone(PINSP,NOTE_G5,50) ;
      delay(45);
      tone(PINSP,NOTE_F5,50) ;
      delay(45);
      tone(PINSP,NOTE_E5,50) ;
      delay(45);
      tone(PINSP,NOTE_D5,50) ;
      delay(45);
      tone(PINSP,NOTE_C5,50) ;
      delay(45);
      tone(PINSP,NOTE_B4,50) ;
      delay(45);
      tone(PINSP,NOTE_A4,50) ;
      delay(45);
      tone(PINSP,NOTE_G4,50) ;
      delay(45);
      tone(PINSP,NOTE_F4,50) ;
      delay(45);
      tone(PINSP,NOTE_E4,50) ;
      delay(45);
      tone(PINSP,NOTE_D4,50) ;
      delay(45);
      tone(PINSP,NOTE_C4,50) ;
      delay(45);
      tone(PINSP,NOTE_B3,50) ;
      delay(45);
      tone(PINSP,NOTE_A3,500) ;
      delay(495);
  }
  
}

