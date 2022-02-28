#include <opencv2/opencv.hpp>


int main() {
  double gamma = 2.3;
  cv::Mat img(180, 768, CV_8UC1);
  //draw dummy image
  img = 0;
  cv::Rect2d rc = {0, 0, 768, 60 };
  cv::rectangle(img, rc, { 100 }, 1);
  rc.y += rc.height;
  cv::rectangle(img, rc, { 250 }, 1);
  rc.y += rc.height;
  cv::rectangle(img, rc, { 150 }, 1);
  //save result


//От черного к белому
  for(int i = 0; i < 768; i++){
    for(int k = 0; k < 60; k++){
      img.at<uchar>(k,i) = i/3;
    }
  }

//Гамма-коррекция pow
  unsigned int start_time =  clock();
  rc = {0, 0, 768, 60};
  cv::Rect2d rc1 = {0, 60, 768, 60 };
  img.convertTo(img, CV_64F);
  cv::pow(img(rc)/255, gamma, img(rc1));
  img(rc1) *= 255;
  img.convertTo(img, CV_8UC1);
  unsigned int end_time = clock();
  unsigned int td1 = end_time - start_time; 

//Гамма-коррекция(прямое обращение)
  start_time =  clock();
  for(int i = 0; i < 768; i++){
    for(int k = 0; k < 60; k++){
      img.at<uchar>(k + 120,i) = cv::saturate_cast<uchar>(cv::pow(img.at<uchar>(k,i)/255., gamma) * 255);
    }
  }
  end_time = clock();
  unsigned int td2 = end_time - start_time;

  std::cout << "1 method: " << td1 << "ms" << '\n' << "2 method: " << td2 << "ms";
  cv::imshow("img", img );

  cv::waitKey(0);

  cv::imwrite("lab01.png", img);
  return 0;
}