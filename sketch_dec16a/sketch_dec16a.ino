#include <LiquidCrystal.h> 
#include "song.h"
const byte speakerPin=8;
  const byte SW = 10;    //按鈕位於Pin10
const byte LED = 9;  //led位於Pin
bool lastState=false; //彈跳狀態
bool checked = false; // 是否已解碼
byte code[5] = {3,3,3,3,3};  // 已得摩斯密碼陣列
const byte code_table[38][5] ={  //摩斯密碼表 0=DOT 2=DASH 3=空值
  0,1,3,3,3, //A
  1,0,0,0,3, //B
  1,0,1,0,3, //C
  1,0,0,3,3, //D
  0,3,3,3,3, //E
  0,0,1,0,3, //F
  1,1,0,3,3, //G
  0,0,0,0,3, //H
  0,0,3,3,3, //I
  0,1,1,1,3, //J
  1,0,1,3,3, //K
  0,1,0,0,3, //L
  1,1,3,3,3, //M
  1,0,3,3,3, //N
  1,1,1,3,3, //O
  0,1,1,0,3, //P
  1,1,0,1,3, //Q
  0,1,0,3,3, //R
  0,0,0,3,3, //S
  1,3,3,3,3, //T
  0,0,1,3,3, //U
  0,0,0,1,3, //V
  0,1,1,3,3, //W
  1,0,0,1,3, //X
  1,0,1,1,3, //Y
  1,1,0,0,3, //Z
  0,1,1,1,1, //1
  0,0,1,1,1, //2
  0,0,0,1,1, //3
  0,0,0,0,1, //4
  0,0,0,0,0, //5
  1,0,0,0,0, //6
  1,1,0,0,0, //7
  1,1,1,0,0, //8
  1,1,1,1,0, //9
  1,1,1,1,1, //0
  1,1,1,1,3,
  0,0,1,1,3,
};
const char code_chtable[37] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0',' '}; //密碼to字元轉換
char msg[16]={'0'};
char music[4][7]={"SPR","MX","SEE","E"};
byte count  = 0;  // 彈跳計數器
byte n=0,decon=0; // arrary index
double timer =0,spacetimer = 0; // 間隔時間，空白時間
LiquidCrystal lcd(12, 11, 5, 4, 3, 1);



void setup() {
  pinMode(SW,INPUT);
  pinMode(LED,OUTPUT);
  //Serial.begin(9600);
  lcd.begin(16,2);
}
void loop()  {
    boolean  b1 = digitalRead(SW);
    digitalWrite(LED,b1);//配合開關亮暗以便  之後光敏電阻判斷
    if (b1 != lastState){    // 判斷按鈕彈跳 
        delay(20);
        boolean b2 = digitalRead(SW);
        if  (b1 == b2){
            lastState = b1;
            count ++;
            if(count==1){
              checked=true;
              //Serial.println(spacetimer);
            }
        }
     }
     if(count == 1){  // 計算按鈕按下的持續時間
      timer = timer + 0.01;  
     }
     if(count==2){   // 完成一次按放動作
        count = 0;  
        //Serial.print(timer);
        if(timer<=100 && n<5){  //持續時間小於100ms為Dot else為Dash
            code[n]=0;
            Serial.println(" Dot");
            lcd.setCursor(n,1);
            lcd.print("0");
            n++;
        }
        else if(timer>13&&n<5)
        {
            code[n]=1;
            Serial.println(" Dash");
            lcd.setCursor(n,1);
            lcd.print("1");
            n++;
        }
        
        timer = 0;
        spacetimer=0;
     }
     
     if(count == 0){  // 無動作的空白時間
          spacetimer += 0.0001;
          
          if(spacetimer>2.5 && checked){ //無動作一段時間後判定為單字
              //解碼
              Serial.print("decode :  ");
              char ch = morse_decode();
              msg[decon-1] = ch;
              lcd.print( ch );
              for(int i=0;i<5;i++){  //將讀取密碼陣列回初始值
                  code[i]=3;
              }
          
              //解碼
              n=0;
              checked=false;
          }
     }
     
     
     if(spacetimer>10000){  //避免spacetimer溢出
        spacetimer = 4;
     }
}

char morse_decode() //解碼function
{
      lcd.setCursor(0,1);
      lcd.print("                 ");
      lcd.setCursor(decon++,0);
      if(decon == 16)
        decon = 0;
      n=0;
      byte x;
      for(x=0;x<=37;x++){  //搜尋密碼表
        for(int y=0;y<5;y++){
          if(code[y]!=code_table[x][y])  //有不相同就跳下一個字元
            break;
          if(code[y]==code_table[x][y])  //對應密碼成功則n+1;
            n++;
        }
        if(n==5){  //假設五個密碼都對應成功及找到該字元 跳出迴圈
          if(x==36)
          {
            lcd.setCursor(0,0);
            lcd.print("                 ");
            decon = 0;
            break;
          }
          else if(x==37)
          {
            play_music();
            lcd.clear();
            break;
          }
          n=0;
          return code_chtable[x];
        }
        n=0;
      }
      return code_chtable[36];
}
void play_music()
{
  byte play_number=9;
  for(byte i=0;i<4;i++)
  {
    byte n=0;
    for(byte j=0;j < strlen(music[i]); j++)
    {
      if(msg[j]==music[i][j])
        n++;
    }
    if(n==strlen(music[i])){
      play_number=i;
      break;
    }
  }
  if(play_number == 0)
  {
    int length = sizeof(spring_notes);
    loop_play(500,spring_notes,spring_beats,length);
  }
  if(play_number == 1)
  {
    int length = sizeof(mx_notes);
    loop_play(600,mx_notes,mx_beats,length);
  }
  if(play_number == 2){
    int length = sizeof(t_notes);
    loop_play(600,t_notes,t_beats,length);
  }
  /*if(play_number == 3){
    int length = sizeof(e_notes);
    loop_play(700,e_notes,e_beats,length);
  }*/
}

void loop_play(int tempo, char *notes, float *beats,int length)
{
    digitalWrite(LED,HIGH);
    for (int i = 0; i < length; i++) {
        if(digitalRead(SW))
          break;
        // 如果是空白的話，不撥放音樂
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
      } else {
        // 呼叫 palyNote() 這個 function，將音符轉換成訊號讓蜂鳴器發聲
        playNote(speakerPin,notes[i], beats[i] * tempo);
      } 
      // 每個音符之間的間隔，這邊設定的長短會有連音 or 段音的效果
      delay(tempo/10); 
    }
    digitalWrite(LED,LOW);
    
}

void playNote(int OutputPin, char note, unsigned long duration) {
   // 音符字元與對應的頻率由兩個矩陣表示
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' ,'D','E','F','G','B','A'};
  int tones[] = { 261, 294, 330, 349, 392, 440, 494, 523, 587, 659, 700, 786,990,882};
  // 播放音符對應的頻率
  for (byte i = 0; i < 14; i++) {
    if (names[i] == note) {
      tone(OutputPin,tones[i], duration);
  //下方的 delay() 及 noTone ()，測試過後一定要有這兩行，整體的撥放出來的東西才不會亂掉，可能是因為 Arduino 送出tone () 頻率後會馬上接著執行下個指令，不會等聲音播完，導致撥出的聲音混合而亂掉
      delay(duration);
      noTone(OutputPin);
    }
  }
}

