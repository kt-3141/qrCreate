#include <iostream>
#include <fstream>
#include <string>
#include "qrencode.h"
#include "opencv2/opencv.hpp"

int main(void){
	std::string input_str, input_version, input_level;
	std::ofstream ofs("output.csv");
	std::cout << "please input target string" << std::endl;
	std::cin >> input_str;
	int temp;
	do{
		std::cout << "please input target version" << std::endl;
		std::cin >> input_version;
		try{
			temp = stoi(input_version);
		}
		catch(std::invalid_argument& ee){
			std::cout << "please input number" << std::endl;
			std::cin.get();
			std::cin.get();
			return 0;
		}
	}while(temp < 1 || temp > 40);
	do{
		std::cout << "please input error level" << std::endl;
		std::cin >> input_level;
		try{
			temp = stoi(input_level);
		}
		catch(std::invalid_argument& ee){
			std::cout << "please input number" << std::endl;
			std::cin.get();
			std::cin.get();
			return 0;
		}
	}while(temp < 0 || temp > 3);
	QRinput* qrInput = QRinput_new2(stoi(input_version), (QRecLevel)(stoi(input_level)));
	QRinput_append(qrInput, QR_MODE_8, input_str.size(), (const unsigned char*)input_str.c_str());
	QRcode* output = QRcode_encodeInput(qrInput);
	temp = QRinput_getVersion(qrInput);
	if(temp > stoi(input_version)){
		std::cout << "input version is too small. selected version is " << std::to_string(temp) << std::endl;
	}
	else{
		std::cout << "selected version is " << std::to_string(temp) << std::endl;
	}
	cv::Mat img(output->width, output->width, CV_8UC1);
	for(int i = 0; i < output->width; ++i){
		for(int j = 0; j < output->width; ++j){
			ofs << std::to_string(output->data[j * output->width + i] & 0x01) << ",";
			std::cout << std::to_string(output->data[j * output->width + i] & 0x01) << ",";
			if(output->data[j * output->width + i] & 0x01){
				img.at<unsigned char>(j, i) = 0;
			}
			else{
				img.at<unsigned char>(j, i) = 255;
			}
		}
		ofs << std::endl;
		std::cout << std::endl;
	}
	ofs.close();
	cv::resize(img, img, cv::Size(), 500.0/img.cols, 500.0/img.rows);
	cv::imshow("data", img);
	cv::waitKey();
	std::cin.get();
	return 0;
}