//
//  Controler.h
//  dachuang_car_xcode
//
//  Created by 王晗 on 10/12/16.
//  Copyright © 2016 王晗. All rights reserved.
//

#ifndef RASPI_VISION_CONTROLER__
#define RASPI_VISION_CONTROLER__

#include <raspicam/raspicam_cv.h>
#include <thread>
namespace rv{
	class Controler{
	public:
		void recieve(const cv::Mat& image) {}
	
		static Controler& GetInstance()
		{
			if (sControler == nullptr){
				sControler = new Controler();
			}
			return *sControler;
		}


	private:
		static Controler* sControler;

	};
}

#endif
