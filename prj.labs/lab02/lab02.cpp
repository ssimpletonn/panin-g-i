#include <opencv2/opencv.hpp>
#include <vector>

int main() {
    cv::Mat img = cv::imread("../data/cross_0256x0256.png");

    //JPEG качество 25%
    std::vector<int> p = {cv::IMWRITE_JPEG_QUALITY, 25};
    cv::imwrite("cross_0256x0256_025.jpeg", img, p);

    cv::Mat img1 = cv::imread("cross_0256x0256_025.jpeg");
    cv::imshow("img", img1);

    //Разделение на 3 канала
    std::vector<cv::Mat> ch(3);
    cv::split(img, ch);

    cv::imshow("ch1", ch[0]);
    cv::imshow("ch2", ch[1]);
    cv::imshow("ch3", ch[2]);
    cv::waitKey(0);
}