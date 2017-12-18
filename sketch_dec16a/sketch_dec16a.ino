const byte SW = 8;    //按鈕位於Pin8
const byte LED = 12;  //led位於Pin
bool lastState=false,toggle=false; //彈跳狀態
bool checked = false; // 是否已解碼
int code[5] = {3,3,3,3,3};  // 已得摩斯密碼陣列
int code_table[36][5] ={  //摩斯密碼表 0=DOT 2=DASH 3=空值
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
};
char code_chtable[36] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0'}; //密碼to字元轉換
int count  = 0;  // 彈跳計數器
int n=0; // arrary index
double timer =0,spacetimer = 0; // 間隔時間，空白時間

void setup() {
  pinMode(SW,INPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}
void loop()  {
    boolean  b1 = digitalRead(SW);
    if (b1 != lastState){    // 判斷按鈕彈跳
        delay(20);
        boolean b2 = digitalRead(SW);
        if  (b1 == b2){
            lastState = b1;
            count ++;
            if(count==1){
              checked=true;
              Serial.println(spacetimer);
            }
        }
     }
     
     if(count == 1){  // 計算按鈕按下的持續時間
      timer = timer + 0.01;  
     }
     
     if(count==2){   // 完成一次按放動作
      count = 0;  
      toggle = !toggle;
      digitalWrite(LED,toggle);
      Serial.print(timer);
      
      if(timer<=100 && n<6){  //持續時間小於100ms為Dot else為Dash
        code[n++]=0;
        Serial.println(" Dot");
      }
      else{
        code[n++]=1;
        Serial.println(" Dash");
      }
      timer = 0;
      spacetimer=0;
     }
     
     if(count == 0){  // 無動作的空白時間
      spacetimer+= 0.0001;
      
      if(spacetimer>2.5 && checked){
      //解碼
      Serial.println("解碼");
      n=0;
      for(int x=0;x<36;x++){  //搜尋密碼表
        for(int y=0;y<5;y++){
          if(code[y]!=code_table[x][y])  //有不相同就跳下一個字元
            break;
          if(code[y]==code_table[x][y])  //對應密碼成功則n+1;
            n++;
        }
        if(n==5){  //假設五個密碼都對應成功及找到該字元 跳出迴圈
          n=x;
          break;
        }
        else{
          n=0;
        }
      }
      
      Serial.print(code_chtable[n]);  //印出密碼的對應字元
      
      for(int i=0;i<5;i++){  //將讀取密碼陣列回初始值
        Serial.print(code[i]);
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

