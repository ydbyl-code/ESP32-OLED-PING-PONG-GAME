#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,-1,5,4);

int LB = 9;

int I1 = 20;
int I2 = 21;
int I3 = 45;
int I4 = 1;

int Pk = 10;
int P1X = 60;
int P1Y = 60;
int P2X = 60;
int P2Y = 4;
int Pv = 5;

int Ck = 3;
int Cx = 60;
int Cy = 32;
int CXv = 1;
int CYv = 1;
int Cxmode = 0;
int Cymode = 1;

int Pmode = 0;

int score = 0;

void setup() {

    u8g2.begin();

    pinMode(LB, OUTPUT);

    pinMode(I1, INPUT_PULLDOWN);
    pinMode(I2, INPUT_PULLDOWN);
    pinMode(I3, INPUT_PULLDOWN);
    pinMode(I4, INPUT_PULLDOWN);
}

void loop() 
{

    digitalWrite(LB, HIGH);

    //游戏计算：

    if(Pmode == 0)
    {  
        score = 0;
        if(digitalRead(I3) == HIGH) 
        {
            delay(100);
            if(digitalRead(I3) == HIGH) 
            {
                Pmode = 1;
                digitalWrite(LB, LOW);
                delay(50);
                digitalWrite(LB, HIGH);
            }
        }
    }
    if(Pmode == 1 || Pmode == 2)
    {
        if(Cxmode == 0)
        {
            Cx -= CXv;
            if(Cx <= Ck) 
            {
                Cx = Ck;
                Cxmode = 1;
            }
        }
        if(Cxmode == 1)
        {
            Cx += CXv;
            if(Cx >= 128-Ck) 
            {
                Cx = 128-Ck;
                Cxmode = 0;
            }
        }
        if(Cymode == 0)
        {
            Cy -= CYv;

            if(Cy <= P2Y+Ck)
            {
                if(Cx >= P2X+Pk || Cx <= P2X-Pk) 
                {
                    digitalWrite(LB, LOW);
                    delay(50);
                    digitalWrite(LB, HIGH);
                    Pmode = 4;
                }
                else
                {   
                    Cy = P2Y+Ck;
                    Cymode = 1;
                    digitalWrite(LB, LOW);
                    score+=1;
                    delay(10);
                    digitalWrite(LB, HIGH);
                }
            }

        }
        if(Cymode == 1)
        {
            Cy += CYv;
            if(Cy >= P1Y-Ck)
            {
                if(Cx >= P1X+Pk || Cx <= P1X-Pk) 
                {
                    digitalWrite(LB, LOW);
                    delay(50);
                    digitalWrite(LB, HIGH);
                    Pmode = 4;
                }
                else
                {
                    Cy = P1Y-Ck;
                    Cymode = 0;
                    digitalWrite(LB, LOW);
                    score+=1;
                    delay(10);
                    digitalWrite(LB, HIGH);
                }
            }
        }
    }
    if (Pmode == 1)
    {
        if(digitalRead(I1) == HIGH) 
        {
            P1X -= Pv;
            if(P1X <= Pk) 
            {
                P1X = Pk;
            } 
        }    
        if(digitalRead(I2) == HIGH) 
        {
            P1X += Pv;
            if(P1X >= 128-Pk) 
            {
                P1X = 128-Pk;
            }
        }
        if(digitalRead(I4) == HIGH) 
        {
            delay(50);
            if(digitalRead(I4) == HIGH) 
            {
                Pmode = 2;
                digitalWrite(LB, LOW);
                delay(30);
                digitalWrite(LB, HIGH);
            }
        }
    }
    if (Pmode == 2)
    {
        if(digitalRead(I1) == HIGH) 
        {
            P2X -= Pv;
            if(P2X <= Pk) 
            {
                P2X = Pk;
            }
        }    
        if(digitalRead(I2) == HIGH) 
        {
            P2X += Pv;
            if(P2X >= 128-Pk) 
            {
                P2X = 128-Pk;
            }
        }
        if(digitalRead(I4) == HIGH) 
        {
            delay(100);
            if(digitalRead(I4) == HIGH)
            {
                Pmode = 1;
                digitalWrite(LB, LOW);
                delay(50);
                digitalWrite(LB, HIGH);
            }
        }
    }
    if(Pmode == 4)
    {
        if(digitalRead(I3) == HIGH) 
        {
            delay(100);
            if(digitalRead(I3) == HIGH) 
            {
                P1X = 60;
                P1Y = 60;
                P2X = 60;
                P2Y = 4;
                Cx = 60;
                Cy = 32;
                Pmode = 0;
                Cymode = 1;
                digitalWrite(LB, LOW);
                delay(50);
                digitalWrite(LB, HIGH);
            }
        }
    }

    //游戏渲染：
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    u8g2.drawLine(P1X+Pk, P1Y, P1X-Pk, P1Y);
    u8g2.drawLine(P2X+Pk, P2Y, P2X-Pk, P2Y);

    u8g2.drawDisc(Cx, Cy, Ck);

    if(Pmode == 1) {
        u8g2.drawStr(0, 10, "Player 1");
    }
    if(Pmode == 2) {
        u8g2.drawStr(0, 10, "Player 2");
    }  
    if(Pmode == 0) {
        u8g2.drawStr(20, 40, "PING-PONG GAME");
    }  
    if(Pmode == 4) {
        u8g2.drawStr(40, 40, "GAME OVER");
    }  

    u8g2.drawStr(0, 20, "score:");
    u8g2.drawStr(40, 20, String(score).c_str());

    u8g2.sendBuffer();
    delay(16);

}
