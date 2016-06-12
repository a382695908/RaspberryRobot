#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

#include <opencv2/imgproc/imgproc.hpp>

extern std::ostream& log_;

namespace rv{

	int Otsu(cv::Mat& mImage);

	//Using Ostsu algorithm to get binary threshold
	int Otsu(float histogram[256],const int height,const int width);

	//Using sqrt algorithm to get binary images
	void sqrtBinary(cv::Mat& mImage);

	//Using square algorithm to get binary images
	void squareBinary(cv::Mat& mImage);

}

#endif /*#define _IMAGE_PROCESS_H_*/
