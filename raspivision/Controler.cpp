#include "Controler.h"
#include <wiringPi.h>
#include <unistd.h>
#include <iostream>
#define ON_BLACK 0
#define ON_WHITE 1
#define BLACK_UNKNOWN 2   //黑色待定
#define WHITE_UNKNOWN 3   //白色待定
#define WHITE_NOISE_WHIDTH 3
#define BLACK_NOISE_WHIDTH 3
namespace rv{
	Controler* Controler::sControler = nullptr;
	
	void Controler::run(Controler* that){
		int pin=that->servoPin;
		while(true){
			digitalWrite(pin,HIGH);
			usleep(that->pwmHigh);
			digitalWrite(pin,LOW);
			usleep(that->pwmLow); 
		}
	}
	
	int Controler::findLineMidPos(const cv::Mat& image,int line){
		int threshold=100;
		int i=0;
		//状态
		int state=0;
		//待定白块宽度
		int unknownWhite=0;
		//待定黑块宽度
		int unknownBlack=0;
		//线 左边界
		int left_border=0;
		//线 右边界
		int right_border=0;
		
		//detect the left border
		for(i=0,state=ON_BLACK;i<imageCols;i++){
			//if on white pixel
			
			if(image.at<uchar>(line,i) >threshold){
				switch(state){
					case ON_BLACK:
						left_border=i;
						state=WHITE_UNKNOWN;
						unknownWhite=0;
						break;
					case BLACK_UNKNOWN:
						
						state=WHITE_UNKNOWN;
						break;
					case WHITE_UNKNOWN:
						unknownWhite++;
						//连续白块宽度大于阈值，说明之前找到左边界
						if(unknownWhite>WHITE_NOISE_WHIDTH){
							goto END_LEFT_DETECTING;
						}
						break;
				
				}
			}
			//if on black pixel
			else{
				switch(state){
					case WHITE_UNKNOWN:
						unknownBlack=1;
						state=BLACK_UNKNOWN;
					case BLACK_UNKNOWN:
						unknownBlack++;
						if(unknownBlack>BLACK_NOISE_WHIDTH){
							state=ON_BLACK;
						}
						break;
				}
		
			} 

		}
		END_LEFT_DETECTING:
		//detect the right_border
		for(i=left_border,state=ON_WHITE;i<imageCols;i++){
			//if on black pixel
			if(image.at<uchar>(line,i) < threshold){
				switch (state){
					case ON_WHITE:
						state=BLACK_UNKNOWN;
						unknownBlack=1;
						right_border=i;
						break;
					case WHITE_UNKNOWN:
						state=BLACK_UNKNOWN;
						break;
					case BLACK_UNKNOWN:
						unknownBlack++;
						if(unknownBlack>BLACK_NOISE_WHIDTH){
							goto END_RIGHT_DETECTING;
						}
						break;
				}
			}
			//if on white pixel
			else{
				switch(state){
					case BLACK_UNKNOWN:
						state=WHITE_UNKNOWN;
						unknownWhite=1;
						break;
					case WHITE_UNKNOWN:
						unknownWhite++;
						if(unknownWhite>WHITE_NOISE_WHIDTH){
							state=ON_WHITE;
						}
						break;
				}
			}
		}
		END_RIGHT_DETECTING:
		
		return (left_border+right_border)/2;
	}
	
	int Controler::calcServoAngle(const cv::Mat& image,int preLine,int line){
		int prePos=findLineMidPos(image,preLine);
		int linePos=findLineMidPos(image,line);
		if(prePos<5){
			return -this->maxAngle;
		}
		else if(prePos>imageCols-5){
			return this->maxAngle;
		}
		else{
			return PID( (prePos+linePos)/2 );
		}
	}
	
	void Controler::turnServo(int angle){
		//舵机控制使用周期为 20ms 占空比0.025～0.125 
		pwmHigh=1500+angle*11.1;
		pwmLow=20000-pwmHigh;
	}
	
	
	//pid algorithm
	int Controler::PID(int pos){
		static int integral=0;
		static int lastPos=-1;
		int error=this->targetPos  - pos;
		int diff=0;
		int u=0;
		integral+=error;
		
		if(lastPos!=-1){
			diff=pos-lastPos;
			lastPos=pos;
		}
		else{
			diff=0;
			lastPos=pos;
		}
		u=pid_kp*error+pid_ki*integral+pid_kd*diff;
		
		if(u < -this->maxAngle){
			u=-maxAngle;
		}
		else if(u > this->maxAngle){
			u=maxAngle;
		}
		return u;  
	}
		
}
