#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat mosaic(cv::Mat& img) {
    cv::Mat mosaic(512, 512, CV_8UC3);
    mosaic = 0;
    cv::Rect2d rec = {0, 0, 256, 256};
    
    std::vector<cv::Mat> ch(3);
    cv::split(img, ch);
    cv::Mat zeros = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    
    std::vector<cv::Mat> B = {ch[0], zeros, zeros};
    std::vector<cv::Mat> G = {zeros, ch[1], zeros};
    std::vector<cv::Mat> R = {zeros, zeros, ch[2]};

    img.copyTo(mosaic(rec));
    rec.y += 256;
    cv::merge(G, img);
    img.copyTo(mosaic(rec));
    rec.x += 256;
    cv::merge(B, img);
    img.copyTo(mosaic(rec));
    rec.y -= 256;
    cv::merge(R, img);
    img.copyTo(mosaic(rec));

    cv::merge(ch, img);

    return mosaic;
}

// cv::Mat getHistogram(cv::Mat& img) {

// }


int main() {
    cv::Mat img = cv::imread("../data/cross_0256x0256.png");

    //JPEG качество 25%
    std::vector<int> p = {cv::IMWRITE_JPEG_QUALITY, 25};
    cv::imwrite("cross_0256x0256_025.jpeg", img, p);
    cv::Mat img1 = cv::imread("cross_0256x0256_025.jpeg");

    //Мозаика
    cv::Mat mosaic1 = mosaic(img);
    cv::Mat mosaic2 = mosaic(img1);

    cv::imshow("m1", mosaic1);
    cv::imshow("m2", mosaic2);

    cv::imwrite("cross_0256x0256_png_channels.png", mosaic1);
    cv::imwrite("cross_0256x0256_jpg_channels.png", mosaic2);

    //Гистограммы
    //cv::Mat histograms;
    
    //hist1 = getHistogram(img);
    //hist2 = getHistogram(img1);

    //cv::vconcat(hist1, hist2, histograms);
    //cv::imshow("histograms", histograms);

    cv::waitKey(0);
}