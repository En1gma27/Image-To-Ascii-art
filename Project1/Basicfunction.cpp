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
void processFrame(cv::Mat frame, std::string gscale, int scale, std::ofstream& outputFile) {
    cv::Mat grayImage;
    cv::cvtColor(frame, grayImage, cv::COLOR_BGR2GRAY);

    for (int i = 0; i < grayImage.rows; i += scale) {
        std::string text = "";
        for (int j = 0; j < grayImage.cols; j += scale) {
            int pixel = calculateAvg(grayImage, i, j, scale);
            text += gscale[ceil((gscale.length() - 1) * pixel / 255)];
        }
        outputFile << text << std::endl;
        std::cout << text << std::endl; // Hiển thị ra console
    }
    outputFile << "\n";
    std::cout << "\n";
}
int main(int, char**) {
    int mode;
    std::cout << "Chon che do: (1: Anh, 2: Video): ";
    std::cin >> mode;

    std::string gscale = "$@B%8&WM#*oahkbdpqwmZ0BQLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!1I;;,";
    int scale = 4;

    if (mode == 1) {
        // Xử lý ảnh
        std::string path;
        std::cout << "Nhap duong dan buc anh: ";
        std::cin >> path;

        cv::Mat image = cv::imread(path);
        if (image.empty()) {
            std::cout << "Khong the load anh." << std::endl;
            return -1;
        }

        std::ofstream outputFile("ascii_image.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Khong the mo file ascii_image.txt." << std::endl;
            return -1;
        }

        processFrame(image, gscale, scale, outputFile);
        outputFile.close();

        cv::imshow("Image Source", image);
        cv::waitKey(0);
    } else if (mode == 2) {
        // Xử lý video
        std::string path;
        std::cout << "Nhap duong dan video: ";
        std::cin >> path;

        cv::VideoCapture video(path);
        if (!video.isOpened()) {
            std::cerr << "Khong the mo video." << std::endl;
            return -1;
        }

        std::ofstream outputFile("ascii_video.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Khong the mo file ascii_video.txt." << std::endl;
            return -1;
        }

        cv::Mat frame;
        while (true) {
            video >> frame;
            if (frame.empty()) break;

            processFrame(frame, gscale, scale, outputFile);

            cv::imshow("Video Source", frame);
            if (cv::waitKey(30) >= 0) break; // Nhấn phím bất kỳ để dừng
        }

        outputFile.close();
        video.release();
        cv::destroyAllWindows();
    } else {
        std::cerr << "Che do khong hop le!" << std::endl;
        return -1;
    }

    return 0;
}
