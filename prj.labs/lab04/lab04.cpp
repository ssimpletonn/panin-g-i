#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

std::vector<cv::Mat> resize(std::vector<cv::Mat> imgs) {
    std::vector<cv::Mat> resizedImgs;
    for(int i = 0; i < imgs.size(); i++) {
        cv::Mat resizedImg;
        cv::resize(imgs[i], resizedImg, cv::Size(), 0.25, 0.25);
        resizedImgs.push_back(resizedImg);
    }
    return resizedImgs;
}

std::vector<cv::Mat> getFrames(std::string videoPath, std::string v) {
    cv::VideoCapture vc(videoPath);
    std::vector<cv::Mat> frames;
    while(true) {
        cv::Mat frame;
        vc >> frame;
        if(frame.empty()){
            break;
        }
        else {
            frames.push_back(frame);            
        }
    }
    cv::Mat frame1 = frames[(frames.size()*2)/5 - 1];
    cv::Mat frame2 = frames[(frames.size()*3)/5 - 1];
    cv::Mat frame3 = frames[(frames.size()*4)/5 - 1];
    std::vector<cv::Mat> resizedFrames = resize({frame1, frame2, frame3});

    cv::imwrite("lab04_frame" + v + "_1.png", resizedFrames[0]);
    cv::imwrite("lab04_frame"+ v + "_2.png", resizedFrames[1]);
    cv::imwrite("lab04_frame" + v + "_3.png", resizedFrames[2]);
    return resizedFrames;
}

std::vector<cv::Mat> grayScale(std::vector<cv::Mat> frames, std::string path, std::string v) {

    std::vector<cv::Mat> framesGrayScale;
    for(int i = 0; i < frames.size(); i++) {
        cv::Mat frame_grayscale = cv::imread(path + v + "_" + std::to_string(i + 1) + ".png", cv::IMREAD_GRAYSCALE);
        framesGrayScale.push_back(frame_grayscale);
        cv::imwrite(path + v + "_" + std::to_string(i + 1) +  "_grayscale.png", frame_grayscale);
    }
    return framesGrayScale;
}

std::vector<cv::Mat> bin(std::vector<cv::Mat> grayImgs, std::string path, std::string v) {
    std::vector<cv::Mat> framesBin;
    for(int i = 0; i < grayImgs.size(); i++) {
        cv::Mat frameThreshold;
        cv::threshold(grayImgs[i], frameThreshold, 0, 255, cv::THRESH_OTSU);
        framesBin.push_back(frameThreshold);
        cv::imwrite(path + v + "_" + std::to_string(i + 1) +  "_threshold.png", frameThreshold);
    }
    return framesBin;
}

std::vector<cv::Mat> morph(std::vector<cv::Mat> binImgs, std::string path, std::string v) {
    std::vector<cv::Mat> morphedFrames;
    for(int i = 0; i < binImgs.size(); i++) {
        cv::Mat erodedImg;
        cv::morphologyEx(binImgs[i], erodedImg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)));
        morphedFrames.push_back(erodedImg);
    }
    return morphedFrames;
}

std::vector<cv::Mat> dilate(std::vector<cv::Mat> binImgs, std::string path, std::string v) {
    std::vector<cv::Mat> morphedFrames;
    for(int i = 0; i < binImgs.size(); i++) {
        cv::Mat erodedImg;
        cv::dilate(binImgs[i], erodedImg, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)));
        morphedFrames.push_back(erodedImg);
    }
    return morphedFrames;
}

int main() {
    std::string v = "1";
    std::vector<cv::Mat> frames1 = getFrames("../data/money100_1.mp4", v);
    std::vector<cv::Mat> framesBin1 = bin(grayScale(frames1, "lab04_frame", v), "lab04_frame", v);
    std::vector<cv::Mat> framesMorph1 = morph(framesBin1, "lab04_frame", v);
    cv::imshow("bin1", framesBin1[0]);
    cv::imshow("morph1", framesMorph1[0]);

    v = "2";
    std::vector<cv::Mat> frames2 = getFrames("../data/money100_2.mp4", v);
    std::vector<cv::Mat> framesBin2 = bin(grayScale(frames2, "lab04_frame", v), "lab04_frame", v);
    std::vector<cv::Mat> framesMorph2 = morph(framesBin2, "lab04_frame", v);
    cv::imshow("bin2", framesBin2[0]);
    cv::imshow("morph2", framesMorph2[0]);

    v = "3";
    std::vector<cv::Mat> frames3 = getFrames("../data/money100_3.mp4", v);
    std::vector<cv::Mat> framesBin3 = bin(grayScale(frames3, "lab04_frame", v), "lab04_frame", v);
    std::vector<cv::Mat> framesMorph3 = dilate(framesBin3, "lab04_frame", v);
    cv::imshow("bin3", framesBin3[0]);
    cv::imshow("morph3", framesMorph3[0]);

    v = "4";
    std::vector<cv::Mat> frames4 = getFrames("../data/money500_1.mp4", v);
    std::vector<cv::Mat> framesBin4 = bin(grayScale(frames4, "lab04_frame", v), "lab04_frame", v);
    std::vector<cv::Mat> framesMorph4 = morph(framesBin4, "lab04_frame", v);
    cv::imshow("bin4", framesBin4[0]);
    cv::imshow("morph4", framesMorph4[0]);

    v = "5";
    std::vector<cv::Mat> frames5 = getFrames("../data/money500_2.mp4", v);
    std::vector<cv::Mat> framesBin5 = bin(grayScale(frames5, "lab04_frame", v), "lab04_frame", v);
    std::vector<cv::Mat> framesMorph5 = morph(framesBin5, "lab04_frame", v);
    cv::imshow("bin4", framesBin5[0]);
    cv::imshow("morph4", framesMorph5[0]);


    cv::waitKey(0);
    return 0;
}