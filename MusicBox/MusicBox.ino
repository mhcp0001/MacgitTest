#include <pitches.h> // < > で囲む

#define PINSP 8  //圧電スピーカーのピン番号
#define BEAT 30 //音の長さを指定
#define PRESS 250

const int led_pin = 7;
const int C_pin = 1;
const int D_pin = 2;
const int E_pin = 3;
const int F_pin = 4;
const int G_pin = 5;

int C_value = 0;
int D_value = 0;
int E_value = 0;
int F_value = 0;
int G_value = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );
  // initialize digital pin 8 as an output. 
  pinMode(led_pin, OUTPUT);
}

int mode = 0;

void loop() {
 // put your main code here, to run repeatedly:
  C_value = analogRead( C_pin );
  D_value = analogRead( D_pin );
  E_value = analogRead( E_pin );
  F_value = analogRead( F_pin );
  G_value = analogRead( G_pin );

  analogWrite(led_pin, C_value/4 );
  Serial.println( byte(C_value/4) );

  if(C_value<PRESS){
    tone(PINSP,NOTE_C6,BEAT) ;  // ド
     //delay(BEAT*2) ;
  }else if(D_value<PRESS){
    tone(PINSP,NOTE_D6,BEAT) ;  // レ
    //delay(BEAT*2) ;
  }else if(E_value<PRESS){
    tone(PINSP,NOTE_E6,BEAT) ;  // ミ
    //delay(BEAT*2) ;
  }else if(F_value<PRESS){
    tone(PINSP,NOTE_F6,BEAT) ;  // ファ
    //delay(BEAT*2) ;
  }else if(G_value<PRESS){
    tone(PINSP,NOTE_G6,BEAT) ;  // ソ
    //delay(BEAT*2) ;
  }

  if(mode == 1){
    
  }

}
