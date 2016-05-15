#include "ImageProcess.h"
#include <cmath>
#include <omp.h>
namespace rv{
	
	
	/******************************************************************
	*  using otsu algorithm to automatic compute binaryzation threshold
	*
	*	@param mImage : input image(include types: RGB, Gray) 
	*	
	*	@author Created by Han Wang. SCU  on 2016/5/9. 
	*	@return binaryzation threshold
	*
	*******************************************************************/
	int Otsu(cv::Mat& mImage) {
	
	}
	
	
	/******************************************************************
	*  using otsu algorithm to automatic compute binaryzation threshold
	*
	*	@param histogram : gray image histogram
	*	@param height 
	*	@param width  
	*	
	*	@author Created by Han Wang. SCU  on 2016/5/9. 
	*	@return binaryzation threshold
	*
	*******************************************************************/
	int Otsu(float histogram[256],const int height,const int width)        
	{        
		//normalize histogram        
		int size = height * width;  
		
		for(int i = 0; i < 256; ++i)      
		{        
			histogram[i] = histogram[i] / size;        
		}        
	  
		//average pixel value        
		float avgValue=0;        
		for(int i=0; i < 256; i++)      
		{        
			avgValue += i * histogram[i];       
		}         
	  
		int threshold;          
		float maxVariance=0;        
		float w = 0, u = 0;        
		
		for(int i = 0; i < 256; i++)       
		{        
			w += histogram[i];  
			u += i * histogram[i];      
	  
			float t = avgValue * w - u;        
			float variance = t * t / (w * (1 - w) );        
			
			if(variance > maxVariance)       
			{        
				maxVariance = variance;        
				threshold = i;        
			}        
		}        
	  
		return threshold;   
	}
	
	
	/******************************************************************
	*	input image that is processed by sqrt Binary alogrithm
	*
	*	@param mImage 
	*	
	*	@author Created by Han Wang. SCU  on 2016/5/9
	*
	*******************************************************************/
	void sqrtBinary(cv::Mat& mImage){
		cv::Mat Red_Channel; 
		cv::Mat Green_Channel; 
		cv::Mat Blue_Channel; 	
		std::vector<cv::Mat> channels;  
		
		
		float r = 0.0;
		float g = 0.0;
		float b = 0.0;
		
		float area = (int)mImage.rows * (int)mImage.cols;
		
		cv::split(mImage,channels);  
		
		Red_Channel = channels.at(2); 
		Green_Channel = channels.at(1);
		Blue_Channel = channels.at(0);
		
		cv::MatIterator_<uchar> it, end;
		
		for (it = Red_Channel.begin<uchar>(), end = Red_Channel.end<uchar>(); it != end;++it)
			r+=(float)(*it);
		r/=area;
		
		for (it = Green_Channel.begin<uchar>(), end = Green_Channel.end<uchar>(); it != end;++it) 
			g+=(float)(*it);	
		g/=area;
		
		for (it = Blue_Channel.begin<uchar>(), end = Blue_Channel.end<uchar>(); it != end;++it) 
			b+=(float)(*it);
		b/=area;
			
			
		cv::Mat grayImage((int)mImage.rows,(int)mImage.cols,CV_8UC1,cv::Scalar::all(0));
		
		for ( int i=0 ; i< mImage.rows;++i)
		{
			for( int j=0;j<mImage.cols;++j)
			{
				grayImage.at<uchar>(i,j) = (std::sqrt(g*b)*(float)mImage.at<cv::Vec3b>(i,j)[2]+              
											std::sqrt(r*b)*(float)mImage.at<cv::Vec3b>(i,j)[1]+  	 		
											std::sqrt(r*g)*(float)mImage.at<cv::Vec3b>(i,j)[0])/ 	 		
											(std::sqrt(g*b)+std::sqrt(r*b)+std::sqrt(r*g));
			}
		}
		
		//printf("************************************\n");
		//printf("r :%f\n",r);
		//printf("g :%f\n",g);
		//printf("b :%f\n",b);
		
		float Gray_hist[256]={0}; 
		
		for (it = grayImage.begin<uchar>(), end = grayImage.end<uchar>(); it != end;++it) {
			Gray_hist[(int)(*it)]++;
		}
		
		int level = Otsu(Gray_hist,(int)mImage.rows,(int)mImage.cols);
		threshold(grayImage, mImage, level, 255, CV_THRESH_BINARY);
	
	}
	
	/******************************************************************
	*	input image that is processed by square Binary alogrithm
	*
	*	@param mImage 
	*	
	*	@author Created by Han Wang. SCU  on 2016/5/9
	*
	*******************************************************************/
	void squareBinary(cv::Mat& mImage){
		cv::Mat Red_Channel; 
		cv::Mat Green_Channel; 
		cv::Mat Blue_Channel; 	
		std::vector<cv::Mat> channels;  
		
		
		float r = 0.0;
		float g = 0.0;
		float b = 0.0;
		
		float area = (int)mImage.rows * (int)mImage.cols;
		
		cv::split(mImage,channels);  
		
		#pragma omp prallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					Red_Channel = channels.at(2); 
				}
				#pragma omp section
				{
					Green_Channel = channels.at(1);
				}
				#pragma omp section
				{
					Blue_Channel = channels.at(0);
				}
			}
		}
		
		#pragma omp prallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					for (cv::MatIterator_<uchar> it = Red_Channel.begin<uchar>(),end = Red_Channel.end<uchar>(); it != end;++it)
						r+=(float)(*it);
					r/=area;
				}
				#pragma omp section
				{
					for (cv::MatIterator_<uchar> it = Green_Channel.begin<uchar>(),end = Green_Channel.end<uchar>(); it != end;++it) 
						g+=(float)(*it);	
					g/=area;
				}
				#pragma omp section
				{
					for (cv::MatIterator_<uchar> it = Blue_Channel.begin<uchar>(), end = Blue_Channel.end<uchar>(); it != end;++it) 
						b+=(float)(*it);
					b/=area;
				}
			}
		}	
			
		cv::Mat grayImage((int)mImage.rows,(int)mImage.cols,CV_8UC1,cv::Scalar::all(0));
		
		#pragma omp parallel for
		for ( int i=0 ; i< mImage.rows;++i)
		{
			for( int j=0;j<mImage.cols;++j)
			{
				grayImage.at<uchar>(i,j) = (g*b*g*b*(float)mImage.at<cv::Vec3b>(i,j)[2]+              
											r*b*r*b*(float)mImage.at<cv::Vec3b>(i,j)[1]+  	 		
											r*g*r*g*(float)mImage.at<cv::Vec3b>(i,j)[0])/ 	 		
											(g*b*g*b+r*b*r*b+r*g*r*g);
			}
		}
		
		float Gray_hist[256]={0}; 
		
		for (cv::MatIterator_<uchar> it = grayImage.begin<uchar>(),end = grayImage.end<uchar>(); it != end;++it) {
			Gray_hist[(int)(*it)]++;
		}
		
		int level = Otsu(Gray_hist,(int)mImage.rows,(int)mImage.cols);
		threshold(grayImage, mImage, level, 255, CV_THRESH_BINARY);
	
	}
	
}



