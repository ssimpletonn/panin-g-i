## Работа 2. Исследование каналов и JPEG-сжатия
автор: Панин Г. И.
дата: 2022-02-21T21:11:13

url: https://github.com/ssimpletonn/panin-g-i/tree/main/prj.labs/lab01

### Задание
1. В качестве тестового использовать изображение data/cross_0256x0256.png
2. Сохранить тестовое изображение в формате JPEG с качеством 25%.
3. Используя cv::merge и cv::split сделать "мозаику" с визуализацией каналов для исходного тестового изображения и JPEG-версии тестового изображения
- левый верхний - трехканальное изображение
- левый нижний - монохромная (черно-зеленая) визуализация канала G
- правый верхний - монохромная (черно-красная) визуализация канала R
- правый нижний - монохромная (черно-синяя) визуализация канала B
4. Результы сохранить для вставки в отчет
5. Сделать мозаику из визуализации гистограммы для исходного тестового изображения и JPEG-версии тестового изображения, сохранить для вставки в отчет.

### Результаты

![](cross_0256x0256_025.jpg)
Рис. 1. Тестовое изображение после сохранения в формате JPEG с качеством 25%

![](cross_0256x0256_png_channels.png)
Рис. 2. Визуализация каналов исходного тестового изображения

![](cross_0256x0256_jpg_channels.png)
Рис. 3. Визуализация каналов JPEG-версии тестового изображения

![](cross_0256x0256_hists.png)
Рис. 3. Визуализация гистограм исходного и JPEG-версии тестового изображения

### Текст программы

```cpp
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
```
