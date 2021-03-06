#include <opencv2/opencv.hpp>
#include <cmath>

int main() {
    cv::Mat img = cv::imread("../data/cross_0256x0256.png");
    cv::Mat imggray = cv::imread("../data/cross_0256x0256.png", cv::IMREAD_GRAYSCALE);
    cv::imshow("img", img);
    cv::imshow("imggray", imggray);
    cv::imwrite("lab03_rgb.png", img);
    cv::imwrite("lab03_gre.png", imggray);

    cv::Mat func(512, 512, CV_8UC1);
    func = 255;

    cv::Mat lut(1, 256, CV_8U);
    

    //Функция преобразования яркости
    for(int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = cv::saturate_cast<uchar>((1 - std::sin(std::exp(i/256.))) * 256);
    }

    //Визуализация функции
    cv::line(func, cv::Point(0,0), cv::Point(0,511), 0, 1, 0);
    cv::line(func, cv::Point(0,511), cv::Point(511,511), 0, 1, 0);
    cv::line(func, cv::Point(0,3), cv::Point(3,3), 0, 1, 0);
    cv::putText(func, "(0,255)", cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(0, 0, 0));
    cv::putText(func, "(0,0)", cv::Point(10, 500), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(0, 0, 0));
    cv::putText(func, "(255,0)", cv::Point(480, 500), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(0, 0, 0));

    for(int i = 0; i < 512; i++) {
        cv::line(
            func, 
            cv::Point((i - 1) * 2, 512 - lut.at<uchar>(i - 1) * 2), 
            cv::Point(i * 2, 512 - lut.at<uchar>(i) * 2),
            0, 1, 0
        );
    }
    
    cv::imshow("func", func);
    cv::imwrite("lab03_viz_func.png", func);

    //LUT
    cv::Mat imgres;
    cv::Mat imggrayres;
    cv::LUT(img, lut, imgres);
    cv::LUT(imggray, lut, imggrayres);

    cv::imshow("imgres", imgres);
    cv::imshow("imggrayres", imggrayres);
    cv::imwrite("lab03_gre_res.png", imggrayres);
    cv::imwrite("lab03_rgb_res.png", imgres);
    
    
    cv::waitKey(0);
    return 0;
}