#ifndef RASPI_VISION_CONTROLER__
#define RASPI_VISION_CONTROLER__
#include <wiringPi.h>
#include <raspicam/raspicam_cv.h>
#include <thread>
namespace rv{
	class Controler{
	public:
		void recieve(const cv::Mat& image) {
			// static bool isSet=false;
			// if(!isSet){
			// 	this->imageRows = image.rows;
			// 	this->imageCols = image.cols;
			// 	targetPos=this->imageCols/2;
			// 	line[0]=imageRows/2;
			// 	line[1]=imageRows/2+3;
			// 	isSet=true;
			// }
			// turnServo( calcServoAngle( image,line[0],line[1] ) );
			
			
		}
		//the set functions
		void setServoPin(int _pin){
			servoPin=_pin;
			pinMode (servoPin, OUTPUT);//设置为pwm
		}
		
		void setLine(int line1,int line2){
			line[0]=line1;
			line[1]=line2;
		}
		
		void setPID(double kp,double ki,double kd){
			pid_kp=kp;
			pid_ki=ki;
			pid_kd=kd;
		}
		void setMaxAngle(int angle){
			this->maxAngle=angle;
		}
		void setTargetPos(int _targetPos){
			targetPos=_targetPos;
		}
		//the get functions
		int getServoPin(){
			return this->servoPin;
		}
		
		static Controler& GetInstance()
		{
			if (sControler == nullptr){
				sControler = new Controler();
			}
			return *sControler; 
		} 
		
		~Controler(){
			delete pwmThread;
		}
		
	private:
		static Controler* sControler;
		Controler():servoPin(1){
			//set up hardware
			wiringPiSetup() ;
    		pinMode (servoPin, OUTPUT);//设置为pwm
			pwmThread=new std::thread(run,this);
			pwmThread->detach();
		}
		std::thread *pwmThread;
		int line[2]={0};
		//use pwm to control the servo
		int pwmHigh=0;
		int pwmLow=20000;
		//image size
		int imageCols;
		int imageRows;
		
		//the pos we want to get to
		int targetPos=0;
		
		int maxAngle=45;
		
		//servo output pin
		int servoPin=1;
		
		int threshold=100;
		
		//the value of p,i,d
		double pid_kp=0.5;
		double pid_ki=0.001;
		double pid_kd=0.5;
		
		static void run(Controler* that);
		//计算白线中间位置
		int findLineMidPos(const cv::Mat& image,int line);
		
		int calcServoAngle(const cv::Mat& image,int preLine,int line);
		
		void turnServo(int angle);
		
		int PID(int pos);
		
		
		
	};
}

#endif

