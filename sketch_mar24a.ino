#include <LiquidCrystal.h>
#include <pitches.h>
#define CLEAR lcd.clear();
#define PRINT(x) lcd.print(x);

  LiquidCrystal lcd(7,6,5,4,3,2);
  //LEDS
  int redL =19;
  int blueL =18;
  int yellowL=17;
  int greenL=16;

  //BUTTONS
  int redB =8;
  int blueB=9;
  int yellowB=10;
  int greenB=11;
  
const  int red=1;
const  int blue=2;
const  int yellow=3;
const  int green=4;

  int sound = 13;
  int redS[] = {N_C5}; //Plays C Note
  int blueS[] = {N_E5}; //Plays E Note
  int yellowS[] = {N_G5}; //Plays G Note
  int greenS[] = {N_B5}; //Plays B Note

  int level;  //set difficulty of the game
  int Time(300);
  int game[10]; //  //contain a random music
  int player[10]; //contain the player guess
  int button; //ust to catach button press
  int turn =1; //number ot tunes to play
  int win;  //check win or loose
  int G; //for starting a new game

//catch a button
int prss(){
  int temp=0;
    if(digitalRead(redB)==LOW) temp=red;
    if(digitalRead(blueB)==LOW) temp=blue;
    if(digitalRead(yellowB)==LOW) temp=yellow;
    if(digitalRead(greenB)==LOW) temp=green;
  return temp;
}

//play sound & light
void play_tone(int led, int sund[],int dur){
      tone(sound,sund[0],dur); 
      digitalWrite(led,HIGH);
      delay(dur);
      digitalWrite(led,LOW);  
      delay(150); 
  }

//convert button to sound
void play_one(int num, int dur){
    switch (num){
    case (red):
      play_tone(redL,redS,dur);
      break;
    case (blue):
      play_tone(blueL,blueS,dur);
      break;
    case (yellow):
      play_tone(yellowL,yellowS,dur);
      break;
    case (green):
      play_tone(greenL,greenS,dur);
      break;      
  }
}

//playing all with lights
void play_all(int dur){
  for(int i=0;i<level;i++)
    switch (game[i]){
    case (red):
      play_one(game[i],dur);
      break;
    case (blue):
      play_one(game[i],dur);
      break;
    case (yellow):
      play_one(game[i],dur);
      break;
    case (green):
      play_one(game[i],dur);
      break;      
  }
}  

//reseting the game
void start_game(){
  int i;  
  for(i=1;i<5;i++)     //reset board
    play_one(i,300);
  for(i=4;i>0;i--)
    play_one(i,300);
  for(i=0;i<10;i++)
    game[i]=player[i]=0;
    
 for(int i=0;i<level;i++)  //seting randon tunes
     game[i]=random(1, 5);    
}

void game_over(){
  CLEAR
  lcd.setCursor(2,0);
  PRINT("YOU LOOSE!!")
  delay(750);
  lcd.setCursor(2,1);
  PRINT("GAME OVER!!")
  for(int j=0;j<2;j++){
   for (int i=redL;i>=greenL;i--){
    digitalWrite(i,HIGH);
    delay(500);
    digitalWrite(i,LOW);
   }
  }
}

void winner(){
  CLEAR
  lcd.setCursor(3,0);
  PRINT("NICE JOB!")
  lcd.setCursor(3,1);
  PRINT("YOU WON!!")
  for(int j=0;j<10;j++){
    for(int i=redL;i>=blueL;i--){
      digitalWrite(i,HIGH);
       digitalWrite(i-2,HIGH);
      delay(Time);
      digitalWrite(i,LOW);
      digitalWrite(i-2,LOW);  
    }
  }
}

//set difficulty of the game
void set_level(){
  CLEAR
  lcd.setCursor(2,0);
  PRINT("CHOOSE LEVEL")
  lcd.setCursor(0,1);
  PRINT("R-3,B-5,Y-8,G-10")
  level=0;
  while(level==0){
    if(digitalRead(redB)==LOW) {level=3; play_one(red,300);}
    if(digitalRead(blueB)==LOW) {level=5; play_one(blue,300);}
    if(digitalRead(yellowB)==LOW) {level=8; play_one(yellow,300);}
    if(digitalRead(greenB)==LOW) {level=10; play_one(green,300);}
  }
  CLEAR
  lcd.setCursor(4,0);
  PRINT(level)
  PRINT(" NOTES!")
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  lcd.begin(16,2); 
  G=1;
  turn=1;
  win=0;      //reseting flgs
  button=0;
  for (int i=16;i<=19;i++){
    for(int note=0;note<1;note++)
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  for(int i=8;i<=11;i++)
    pinMode(i, INPUT_PULLUP);
}

void loop() {
  int i;
  // put your main code here, to run repeatedly:
 G=turn=1;
 button=win=0;
 
 
  while(G){
    set_level();
    CLEAR
    PRINT("SETTING THE GAME")
    lcd.setCursor(0,1);
    PRINT("PLEASE WAIT...")
    delay(500);
    start_game();
    CLEAR
    PRINT("THAT'S THE" )
    lcd.setCursor(5,1);
    PRINT("GOAL!!!")
    delay(1500);
    play_all(Time);
    delay(1000);
    do{
      CLEAR
      PRINT("TRY TO REMEMBER")
      lcd.setCursor(0,1);
      PRINT(i+1)
      PRINT(" TONES:")
      delay(1500);
      for(i=0;i<turn;i++)
      play_one(game[i],Time); //play till the new tone //playing the tunes
      CLEAR
      PRINT("IT'S YOUR TURN:")
       for(i=0;i<turn;i++){
        button=0;
        while(button==0){                 //catching one prees on a button
          button=prss();
        }
        player[i]=button;
        play_one(button,300);
        if (player[i]!=game[i]) {G=0; win=-1; break;}
         else if(turn==level) {G=0; win=1;}
        delay(Time);
       }
     turn++;
    }while(win==0);
  }
    if(win==-1)
      game_over();
    else if(win==1) winner();  
    CLEAR
    lcd.setCursor(3,0);
    if (win==-1)
      PRINT("YOU'VE LOST!!!")
    else
      PRINT("YO'VE WON!!!")
    lcd.setCursor(0,1);
    PRINT("AGAIN? (ANY KEY)")
    button=0;
    while(button==0)
      button=prss();
    play_one(button,300);
    delay(1000);
    G=turn=1;
    button=win=0;
}



