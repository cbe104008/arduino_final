const byte SW = 8;    //按鈕位於Pin8
const byte LED = 12;  //led位於Pin
bool lastState=false,toggle=false; //彈跳狀態
bool code[6];  // 摩斯密碼陣列
bool checked = false; // 是否已解碼
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
      
      if(timer<=100){  //持續時間小於100ms為Dot else為Dash
        code[n++]=0;
        Serial.println(" Dot");}
      else{
        code[n++]=1;
        Serial.println(" Dash");}
        
      timer = 0;
      spacetimer=0;
     }
     
     if(count == 0){  // 無動作的空白時間
      spacetimer+= 0.0001;
      if(spacetimer>2.5 && checked){
      //解碼
      Serial.print("解碼");
      checked=false;
      //解碼
      }
     }
     if(spacetimer>10000){  //避免spacetimer溢出
      spacetimer = 4;
     }
}
