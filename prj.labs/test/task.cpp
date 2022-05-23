#include <opencv2/opencv.hpp>
#include <cmath>

int main() {
    cv::Mat img(300, 450, CV_8UC1);
    cv::Rect2d rc = {0, 0, 150, 150};
    for(int i = 0; i < 3; i++) {
        img(rc) = 127*i;
        rc.y += 150;
        img(rc) = (3 - i - 1)*127;
        rc.x += 150;
        rc.y -= 150;
    }
    img({150, 150, 150, 150}) = 0;
    img({300, 150, 150, 150}) = 127;

    for(int i = 0; i < 3; i++) {
        cv::circle(img, cv::Point(75 * (i*2 + 1), 225), 75, 127*i, -1);
    }
    cv::circle(img, cv::Point(75, 75), 75, 255, -1);
    cv::circle(img, cv::Point(225, 75), 75, 0, -1);
    cv::circle(img, cv::Point(375, 75), 75, 127, -1);

    cv::Mat img1;
    cv::Mat img2;

    float ker[9]{1, 0, -1, 2, 0, -2, 1, 0, -1};
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    float ker1[9]{1, 2, 1, 0, 0, 0, -1, -2, -1};
    cv::Mat kernel1(3, 3, CV_32FC1, ker1);

    cv::filter2D(img, img1, -1, kernel);
    cv::filter2D(img, img2, -1, kernel1);

    cv::Mat geomean(300, 450, CV_8UC1);

    for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            geomean.at<uchar>(i, j) =  std::sqrt(img1.at<uchar>(i, j) * img2.at<uchar>(i, j));
        }
    }

    cv::imwrite("task_I.png", img);
    cv::imwrite("task_I1.png", img1);
    cv::imwrite("task_I2.png", img2);
    cv::imwrite("task_geomean.png", geomean);

    cv::imshow("img", img);
    cv::imshow("img1", img1);
    cv::imshow("img2", img2);
    cv::imshow("geomean", geomean);
    cv::waitKey(0);
    return 0;
}


// 1 0 -1
// 2 0 -2
// 1 0 -1

// 1 2 1
// 0 0 0
// -1 -2 -1