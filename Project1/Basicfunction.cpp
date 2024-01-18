#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
//Ham nay tinh gia tri trung binh cua cac diem anh trong mot khu vuc hcn.
//Tham so đầu vao bao gom ma tran hinh ảnh image, toa độ hang(row), toa do cot(col), và kich thuoc coa khu vực can tinhh trung binh(scale).
//Ham su dung hai vong lap de duyet qua cac diem anh trong khu vuc duoc chi dinh và tinh tong gia trị cua chung.
int calculateAvg(cv::Mat image, int row, int col, int scale)
{
	int sum = 0;
	int pixelCount = 0;
	int r = row, c = col;
	while (r <= row + scale && r < image.rows) {
		while (c <= col + scale && c < image.cols) {
			sum += (int)image.at<uchar>(r, c);
			pixelCount++;
			c++;
		}
		r++;
	}
	return sum / pixelCount;
}
int main(int, char**) {
	std::string path;
	std::cout << "nhap duong dan buc anh:" ;
	std::cin >> path;
	cv::Mat image = cv::imread(path);
	std::string gscale = "$@B%8&WM#*oahkbdpqwmZ0BQLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!1I;;,";
	if (image.empty()) {
		std::cout << "ko load duoc anh";
		return -1;
	}
	cv::imshow("image source", image);
	// xuat buc anh goc
	cv::Mat grayImage;
	cv::Mat image8bit;
	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
	// chuyen buc anh sang anh mua xam bang ham cvtcolor
	cv::imshow("Gray image", grayImage);
	std::ofstream outputFile;
	outputFile.open("ascii.txt");

	int scale = 4;
	for (int i = 0; i < grayImage.rows; i += scale) {
		std::string text = "";
		for (int j = 0; j < grayImage.cols; j += scale) {
			// gray color in range (0, 255)
			int pixel = calculateAvg(grayImage, i, j, scale);
			text +=gscale[ceil((gscale.length() - 1) * pixel / 255)];
		}
		outputFile << text << std::endl;
	}
	cv::waitKey(0);
	return 0;
}
