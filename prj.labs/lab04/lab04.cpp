#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

//TODO: Переделать findConnectedComponents


const std::string path = "lab04_vid";
const int width = 200;
const int height = 150;

std::vector<cv::Mat> resize(std::vector<cv::Mat> imgs) {
    std::vector<cv::Mat> resizedImgs;
    for(int i = 0; i < imgs.size(); i++) {
        cv::Mat resizedImg;
        cv::resize(imgs[i], resizedImg, cv::Size(width, height));
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

    cv::Mat img(height, width * 3, CV_8UC3);
    cv::Rect2d rc = {0, 0, width, height};
    for(int i = 0; i < resizedFrames.size(); i++) {
        resizedFrames[i].copyTo(img(rc));
        rc.x += width;
    }
    cv::imwrite("lab04_vid" + v + "_123.png", img);
    return resizedFrames;
}

std::vector<cv::Mat> grayScale(std::vector<cv::Mat> frames, std::string v) {
    std::vector<cv::Mat> framesGrayScale;
    for(int i = 0; i < frames.size(); i++) {
        cv::Mat frame_grayscale;
        cv::cvtColor(frames[i], frame_grayscale, cv::COLOR_BGR2GRAY); //cv::imread(path + v + "_" + std::to_string(i + 1) + ".png", cv::IMREAD_GRAYSCALE);
        framesGrayScale.push_back(frame_grayscale);
        cv::imwrite(path + v + "_" + std::to_string(i + 1) +  "_grayscale.png", frame_grayscale);
    }
    
    cv::Mat img(height, width * 3, CV_8UC1);
    cv::Rect2d rc = {0, 0, width, height};

    for(int i = 0; i < framesGrayScale.size(); i++) {
        framesGrayScale[i].copyTo(img(rc));
        rc.x += width;
    }
    
    cv::imwrite(path + v + "_123_grayscale.png", img);
    return framesGrayScale;
}

std::vector<cv::Mat> bin(std::vector<cv::Mat> grayImgs, std::string v) {
    std::vector<cv::Mat> framesBin;
    for(int i = 0; i < grayImgs.size(); i++) {
        cv::Mat frameThreshold;
        cv::threshold(grayImgs[i], frameThreshold, 0, 255, cv::THRESH_OTSU);
        framesBin.push_back(frameThreshold);
        cv::imwrite(path + v + "_" + std::to_string(i + 1) +  "_threshold.png", frameThreshold);
    }

    cv::Mat img(height, width * 3, CV_8UC1);
    cv::Rect2d rc = {0, 0, width, height};
    framesBin[0].copyTo(img(rc));
    rc.x += width;
    framesBin[1].copyTo(img(rc));
    rc.x += width;
    framesBin[2].copyTo(img(rc));
    
    cv::imwrite(path + v + "_123_threshold.png", img);
    return framesBin;
}

std::vector<cv::Mat> morph(std::vector<cv::Mat> binImgs, std::string v) {
    std::vector<cv::Mat> morphedFrames;
    for(int i = 0; i < binImgs.size(); i++) {
        cv::Mat morhedImg;
        cv::morphologyEx(binImgs[i], morhedImg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(30, 30)));
        morphedFrames.push_back(morhedImg);
        cv::imwrite(path + v + "_" + std::to_string(i + 1) +  "_morphed.png", morhedImg);
    }

    cv::Mat img(height, width * 3, CV_8UC1);
    cv::Rect2d rc = {0, 0, width, height};
    morphedFrames[0].copyTo(img(rc));
    rc.x += width;
    morphedFrames[1].copyTo(img(rc));
    rc.x += width;
    morphedFrames[2].copyTo(img(rc));
    cv::imwrite(path + v + "_123_morphed.png", img);
    return morphedFrames;
}

std::vector<cv::Mat> findConnectedComponents(std::vector<cv::Mat> morphedImgs, std::string v) {
    std::vector<cv::Mat> framesWithConnectedComponents;
    for(int i = 0; i < morphedImgs.size(); i++) {
        cv::Mat centroids, stats, res;
        int n = cv::connectedComponentsWithStats(morphedImgs[i], res, stats, centroids);
        std::vector<cv::Vec3b> labels(n);
        int max = 0, imax = 0;
        for (int j = 1; j < n; j++) {
            if (max < stats.at<int>(j, cv::CC_STAT_AREA)) {
                max = stats.at<int>(j, cv::CC_STAT_AREA);
                imax = j;
            }
        }
        for (int j = 0; j < n; j++) {
			labels[j] = cv::Vec3b(0,0,0);
		}
		labels[imax] = cv::Vec3b(255,255,255);
		cv::Mat exit(morphedImgs[i].rows, morphedImgs[i].cols, CV_8UC3);
		for (int a = 0; a < exit.rows; a++) {
			for (int b = 0; b < exit.cols; b++) {
				int label = res.at<int>(a, b);
				cv::Vec3b& pixel = exit.at<cv::Vec3b>(a, b);
				exit.at<cv::Vec3b>(a, b) = labels[label];
			}
		}
        framesWithConnectedComponents.push_back(exit);
    }
    return framesWithConnectedComponents;
}

// std::vector<cv::Mat> dilate(std::vector<cv::Mat> binImgs, std::string v) {
//     std::vector<cv::Mat> morphedFrames;
//     for(int i = 0; i < binImgs.size(); i++) {
//         cv::Mat morhedImg;
//         cv::dilate(binImgs[i], morhedImg, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)));
//         morphedFrames.push_back(morhedImg);
//     }
//     return morphedFrames;
// }

int main() {
    std::string v = "1";
    std::vector<cv::Mat> frames1 = getFrames("../data/money100_1.mp4", v);
    std::vector<cv::Mat> framesBin1 = bin(grayScale(frames1, v), v);
    std::vector<cv::Mat> framesMorph1 = morph(framesBin1, v);
    std::vector<cv::Mat> conCompImgs1 = findConnectedComponents(framesMorph1, v);
    cv::imshow("bin" + v, framesBin1[0]);
    cv::imshow("morph" + v, framesMorph1[0]);
    cv::imshow("connected" + v, conCompImgs1[0]);

    v = "2";
    std::vector<cv::Mat> frames2 = getFrames("../data/money100_2.mp4", v);
    std::vector<cv::Mat> framesBin2 = bin(grayScale(frames2, v),  v);
    std::vector<cv::Mat> framesMorph2 = morph(framesBin2, v);
    std::vector<cv::Mat> conCompImgs2 = findConnectedComponents(framesMorph2, v);
    cv::imshow("bin" + v, framesBin2[0]);
    cv::imshow("morph" + v, framesMorph2[0]);
    cv::imshow("connected" + v, conCompImgs2[0]);


    v = "3";
    std::vector<cv::Mat> frames3 = getFrames("../data/money100_15.MOV", v);
    std::vector<cv::Mat> framesBin3 = bin(grayScale(frames3, v), v);
    std::vector<cv::Mat> framesMorph3 = morph(framesBin3, v);
    std::vector<cv::Mat> conCompImgs3 = findConnectedComponents(framesMorph3, v);
    cv::imshow("bin" + v, framesBin3[0]);
    cv::imshow("morph" + v, framesMorph3[0]);
    cv::imshow("connected" + v, conCompImgs3[0]);

    v = "4";
    std::vector<cv::Mat> frames4 = getFrames("../data/money500_1.mp4", v);
    std::vector<cv::Mat> framesBin4 = bin(grayScale(frames4, v), v);
    std::vector<cv::Mat> framesMorph4 = morph(framesBin4, v);
    std::vector<cv::Mat> conCompImgs4 = findConnectedComponents(framesMorph4, v);
    cv::imshow("bin" + v, framesBin4[0]);
    cv::imshow("morph" + v, framesMorph4[0]);
    cv::imshow("connected" + v, conCompImgs4[0]);

    v = "5";
    std::vector<cv::Mat> frames5 = getFrames("../data/money500_2.mp4", v);
    std::vector<cv::Mat> framesBin5 = bin(grayScale(frames5, v), v);
    std::vector<cv::Mat> framesMorph5 = morph(framesBin5, v);
    std::vector<cv::Mat> conCompImgs5 = findConnectedComponents(framesMorph5, v);
    cv::imshow("bin" + v, framesBin5[0]);
    cv::imshow("morph" + v, framesMorph5[0]);
    cv::imshow("connected" + v, conCompImgs5[0]);


    cv::waitKey(0);
    return 0;
}