#include "stm8s.h"
#include "led.h"

void sysDelay(char del){
char i;
for(i=0;i<del;i++){nop();}
}
void sys_del_ms(int del_ms)	
{
int i;
int  j;
for(i=0;i<del_ms;i++){
for(j=0;j<200;j++){nop();}}
}

//open drain ver.
void init_LED(){
	PORT_led1_CR2&=~BIT_DC1_mask;
	PORT_led1_CR1&=~BIT_DC1_mask;
	PORT_led1_ODR|=BIT_DC1_mask;
	PORT_led1_DDR|=BIT_DC1_mask;

	PORT_led2_CR2&=~BIT_DC2_mask;
	PORT_led2_CR1&=~BIT_DC2_mask;
	PORT_led2_ODR|=BIT_DC2_mask;
	PORT_led2_DDR|=BIT_DC2_mask;;

	sysDelay(200);
       

}
 //open drain ver.
void startTransmittoinD1(){
	
	PORT_led1_ODR|=BIT_DC1_mask;
	sysDelay(50);
        PORT_led1_ODR&=~BIT_DC1_da;
        sysDelay(50);

}


void startTransmittoinD2(){
	
	PORT_led2_ODR|=BIT_DC2_mask;
	sysDelay(50);
        PORT_led2_ODR&=~BIT_DC2_da;
        sysDelay(50);

}


void stopTransmittoinD1(){

	PORT_led1_ODR&=~BIT_DC1_clk;
        sysDelay(50);
        PORT_led1_ODR&=~BIT_DC1_da;
	sysDelay(50);
        PORT_led1_ODR|=BIT_DC1_clk;
        sysDelay(50);
        PORT_led1_ODR|=BIT_DC1_da;
        sysDelay(50);


}

void stopTransmittoinD2(){

	PORT_led2_ODR&=~BIT_DC2_clk;
        sysDelay(50);
        PORT_led2_ODR&=~BIT_DC2_da;
	sysDelay(50);
        PORT_led2_ODR|=BIT_DC2_clk;
        sysDelay(50);
        PORT_led2_ODR|=BIT_DC2_da;
        sysDelay(50);


}


void sendByteD1(char byte){
	char i;
	sysDelay(50);
	for(i=0;i<8;i++){
		PORT_led1_ODR&=~BIT_DC1_clk;
		nop();sysDelay(50);
                PORT_led1_ODR&=~BIT_DC1_da;
		PORT_led1_ODR|=(byte&1)<<PIN_DC1_da;
		sysDelay(50);
		PORT_led1_ODR|=BIT_DC1_clk;
		byte>>=1;sysDelay(50);
                
	}
        PORT_led1_ODR&=~BIT_DC1_clk;
		nop();sysDelay(50);
        PORT_led1_ODR|=BIT_DC1_clk;
        nop();sysDelay(50);
        
}	


void sendByteD2(char byte){
	char i;
	sysDelay(50);
	for(i=0;i<8;i++){
		PORT_led2_ODR&=~BIT_DC2_clk;
		nop();sysDelay(50);
                PORT_led2_ODR&=~BIT_DC2_da;
		PORT_led2_ODR|=(byte&1)<<PIN_DC2_da;
		sysDelay(50);
		PORT_led2_ODR|=BIT_DC2_clk;
		byte>>=1;sysDelay(50);
                
	}
        PORT_led2_ODR&=~BIT_DC2_clk;
		nop();sysDelay(50);
        PORT_led2_ODR|=BIT_DC2_clk;
        nop();sysDelay(50);
        
}	

void sendCMD_D1(char byte){
	startTransmittoinD1();
	sendByteD1(byte);
	stopTransmittoinD1();
}


void sendDataBunchD1(char* byte){
	char i;
	startTransmittoinD1();
	sendByteD1(CMD_ADDR_0);
	
	for(i=0;i<6;i++){
		sendByteD1(*(byte+i));
		//sendByte(0);
	}
	stopTransmittoinD1();
}


void sendCMD_D2(char byte){
	startTransmittoinD2();
	sendByteD2(byte);
	stopTransmittoinD2();
}


void sendDataBunchD2(char* byte){
	char i;
	startTransmittoinD2();
	sendByteD2(CMD_ADDR_0);
	
	for(i=0;i<6;i++){
		sendByteD2(*(byte+i));
		//sendByte(0);
	}
	stopTransmittoinD2();
}


void printData(char* byte,  char display){
	if(display == 1) {
		startTransmittoinD1();
	  sendCMD_D1(CMD_DATA_W_norm_incADDR);
	  stopTransmittoinD1();
	
	  startTransmittoinD1();
	  sendDataBunchD1(byte);
	  stopTransmittoinD1();
	
	  startTransmittoinD1();
	  sendCMD_D1(CMD_DISP_ON);
	  stopTransmittoinD1();
  } else {
	  startTransmittoinD2();
	  sendCMD_D2(CMD_DATA_W_norm_incADDR);
	  stopTransmittoinD2();
	
	  startTransmittoinD2();
	  sendDataBunchD2(byte);
	  stopTransmittoinD2();
	
		startTransmittoinD2();
	  sendCMD_D2(CMD_DISP_ON);
	  stopTransmittoinD2();
  }
}

void printHigherNumber(int val, char display){
  char data[14]={0x3f,0x06,0x5b,0x4f,0x66,  0x6d,0x7d,0x07,0x7f,0x6f}; /*0-9,(0-9)*/
	 char sendingData[8]	={0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};																														
	char i,d;
	
	if ((val>999)||(val<100)){
		  for(i=0;i<4;i++){
				sendingData[i]=data[13];
		  }
			printData(sendingData, display);
			return;
  } 

  sendingData[3]=data[val%10];
	val /= 10;
	sendingData[2]=0;
	sendingData[1]=data[val%10];
	val /= 10;
	sendingData[0]=data[val%10];

  printData(sendingData, display);
}

void printValue(int val, char leftAlignment, char display){
	char data[14]={0x3f,0x06,0x5b,0x4f,0x66,  0x6d,0x7d,0x07,0x7f,0x6f,/*0-9,(0-9)*/ 0x80,/*'.' (10)*/ 0x00,/*' '(11)*/ 0x08,               /*'_'(12)*/ 0x40};   /* '-'(minus) (13)*/
  char sendingData[8]	={0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};																														
	int valCopy;
	char sign;
	char digCNTR=1;
	char i,d;

	
	if ((val>9999)||(val<-999)){
		  for(i=0;i<4;i++){
				sendingData[i]=data[13];
		  }
			printData(sendingData, display);
			return;
  } 
	if (val<0) {sign=1;val=-val;digCNTR++;}
	else sign=0;
	valCopy=val;
	while(valCopy>=10){
		valCopy/=10;
	  digCNTR++;
	}
	
	
	if(leftAlignment){ //leftAlignment
	
    for(i=digCNTR;i>0;i--){
			d=val%10;
			if((d==0)&&(sign)&&(i==1)) 
			  sendingData[0]=data[13];
			else{
			  sendingData[i-1]=data[d];
			  val/=10;
		  }
		}		

    printData(sendingData, display);
		
		
  } 
	else{//rghtAlignment
	  for(i=4;i>=(5-digCNTR);i--){
			d=val%10;
			if((d==0)&&(sign)&&(i==(5-digCNTR))) 
			  sendingData[4-digCNTR]=data[13];
			else{
			  sendingData[i-1]=data[d];
			  val/=10;
		  }
		}			
    printData(sendingData, display);
	}
}