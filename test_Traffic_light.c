#include<reg52.h>
#include<absacc.h>
#define uint unsigned int
#define uchar unsigned char
//引入单片机编程所需的头文件

#define COM8155 XBYTE[0xff20]
#define PA XBYTE[0xff21]
#define PB XBYTE[0xff22]
#define PC XBYTE[0xff22]
//定义8155拓展的PA、PB、PC的地址
#define w1 0xfe;
#define w2 0xfd;
#define w3 0xfb;
#define w4 0xf7;
#define w5 0xef;
#define w6 0xdf;
//宏定义绑定灯和管脚
sbit EWyellow=P1^0;
sbit EWgreen=P1^1;
sbit EWred=P1^2;
sbit NSyellow=P1^4;
sbit NSgreen=P1^5;
sbit NSred=P1^6;
sbit Pedestrian1=P1^3;// 人行提示灯1
sbit Pedestrian2=P1^7;
sbit J=P3^6;// 儿童通行按钮
//定义函数
uchar  code      table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint num1,num2,last1,shi1,last2,shi2;
void count();
void count1();
void flash1();
void flash2();
void normal();
//延迟设定和循环
void delay(uint z)
{
 uint x,y;
 for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}
//主函数，负责判定儿童通过，高优先级
void main()
{
COM8155=0x43;
while(1)
 if(J==0)
 {NSred=0;EWred=0;}
//如果儿童通过输入低电平，则不进入正常循环，直接中断，NEWS均亮红灯
 else
   {normal();}
}
//倒计时函数1，在没有行人通过情况下的南北、东西方向信号灯计时控制
void count()
 {
 uint i=167;
 while(i--)
 {
        PA=w1;  PB=table[last1];    delay(1);   PA=0;
        PA=w2;  PB=table[shi1]; delay(1);   PA=0;
        PA=w3;  PB=table[last2];    delay(1);   PA=0;
        PA=w4;  PB=table[shi2]; delay(1);   PA=0;
        PA=w5;  PB=0x3f;        delay(1);   PA=0;
        PA=w6;  PB=0x3f;        delay(1);   PA=0;
 }
 }
//倒计时函数2，在经过第一个信号灯变换阶段之后加入，提示行人信号灯的等待时间
void count1()
 {
 uint i=167;
 while(i--)
 {
        PA=w1;  PB=table[last1];    delay(1);   PA=0;
        PA=w2;  PB=table[shi1]; delay(1);   PA=0;
        PA=w3;  PB=table[last2];    delay(1);   PA=0;
        PA=w4;  PB=table[shi2]; delay(1);   PA=0;
        PA=w5;  PB=table[last2];    delay(1);   PA=0;
        PA=w6;  PB=table[shi2]; delay(1);   PA=0;
 }
 }
//正常运行状态，列出所有可能的情况，通过判定跳转到不同阶段运行
void normal()
{
 num1=20;num2=42;
 while(num1)
 {if(J==0)
 {  NSred=0;    NSyellow=1;     NSgreen=1;  
    EWred=0;    EWyellow=1;     EWgreen=1;
    Pedestrian1=1; Pedestrian2=1;}//均为黄灯，闪烁
 else
 {if(num1>2)
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=1;    EWyellow=1;     EWgreen=0;  
    Pedestrian1=1; Pedestrian2=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    count();    num1--;     num2--;}//东西方向红灯，南北方向绿灯，行人红灯
     else
   {NSred=0;    NSyellow=1;     NSgreen=1; 
    EWred=1;    EWyellow=1;     EWgreen=0;
    Pedestrian1=1; Pedestrian2=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    flash1();   num1--;     num2--;}
 }
 }// 东西方向绿灯 20s （最后两秒闪烁） 南北方向红灯 20s
 num1=2;    num2=22;
 while(num1)
 {if(J==0)
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1; 
    Pedestrian1=1; Pedestrian2=1;}//均为黄灯，闪烁
 else
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=1;    EWyellow=0;     EWgreen=1;
    Pedestrian1=1; Pedestrian2=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    count();    num1--;     num2--;}
 } // 东西方向黄灯 2s. 南北方向红灯
 num1=42;   num2=20;
 while(num2)
 {if(J==0)
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1; 
    Pedestrian1=1; Pedestrian2=1;}
 else
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1;  
    Pedestrian1=0; Pedestrian2=0;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    count1();   num1--;     num2--;}
 }// 东西方向红灯 20s, 南北方向红灯 20s, 行人通过
 num1=22;num2=20;
 while(num2)
 {if(J==0)
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1; 
    Pedestrian1=1; Pedestrian2=1;}
 else
 {if(num2>2)
   {NSred=1;    NSyellow=1;     NSgreen=0;
    EWred=0;    EWyellow=1;     EWgreen=1;
    Pedestrian=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    count();    num1--;     num2--;}
   else
   {NSred=1;    NSyellow=1;     NSgreen=0;
    EWred=0;    EWyellow=1;     EWgreen=1;
    Pedestrian1=1; Pedestrian2=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    flash2();   num1--;     num2--;}
 }
 }// 东西方向红灯 20s, 南北方向绿灯 20s （最后两秒闪烁）
 num1=2;num2=2;
 while(num2)
 {if(J==0)
 {  NSred=0;    NSyellow=1;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1;
    Pedestrian1=1; Pedestrian2=1;}
 else
 {  NSred=1;    NSyellow=0;     NSgreen=1;
    EWred=0;    EWyellow=1;     EWgreen=1;
    Pedestrian1=1; Pedestrian2=1;
    last1=num1%10;  shi1=num1/10;   last2=num2%10;  shi2=num2/10;
    count();    num1--;     num2--;}
 }// 东西方向红灯 2s, 南北方向黄灯 2s
}
//东西方向控制换灯前的闪烁
void flash1()
{
uint i=170;
 while(i--)
 {
 if((i%18)==0)EWgreen=!EWgreen;
                PA=w1;  PB=table[last1];    delay(1);   PA=0;
                PA=w2;  PB=table[shi1]; delay(1);   PA=0;
                PA=w3;  PB=table[last2];    delay(1);   PA=0;
                PA=w4;  PB=table[shi2]; delay(1);   PA=0;
                PA=w5;  PB=0x3f;        delay(1);   PA=0;
                PA=w6;  PB=0x3f;        delay(1);   PA=0;
 }
}
//南北方向控制换灯前的闪烁
void flash2()
{
uint i=170;
 while(i--)
 {
 if((i%18)==0)NSgreen=!NSgreen;
                PA=w1;  PB=table[last1];    delay(1);PA=0;
                PA=w2;  PB=table[shi1]; delay(1);PA=0;
                PA=w3;  PB=table[last2];    delay(1);PA=0;
                PA=w4;  PB=table[shi2]; delay(1);PA=0;
                PA=w5;  PB=0x3f;        delay(1);PA=0;
                PA=w6;  PB=0x3f;        delay(1);PA=0;
 }
}
