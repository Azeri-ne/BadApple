#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <mmsystem.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "winmm.lib")

void checkForOpenCV()
{
    if(cv::getCPUTickCount == 0)
    {
        std::cout << "OpenCV is not detected." << '\n';
    }
    else
    {
        std::cout << "OpenCV is detected." << '\n';
    }
}

cv::VideoCapture openVid(std::string &filePath)
{
    cv::VideoCapture bapple{filePath};
    if(!bapple.isOpened())
    {
        std::cout << "The file cannot be accessed." << '\n';
        exit(-1);
    }
    else
    {
        std::cout << "File is accessed." << '\n';
    }

    return bapple;
}

char brightness2Ascii(int brightness, std::string asciiCharset)
{
    int index(brightness * asciiCharset.size() / 256);
    return asciiCharset[index];
}

int main()
{
    checkForOpenCV();

    std::string filePath{R"(C:\Users\Harold.DESKTOP-UJ6F4M4\Videos\video.mp4)"};
    cv::VideoCapture bapple{openVid(filePath)};

    double fps{bapple.get(cv::CAP_PROP_FPS)};
    std::cout << fps << '\n';

    std::vector<cv::Mat> totalFrames{}; // preloadVidFrames();
    cv::Mat frame{};

    while(bapple.read(frame))
    {
        int reductionFactor{12};
        cv::Mat resizedFrame{};
        cv::resize(frame, resizedFrame, cv::Size(480 / reductionFactor, 360 / reductionFactor));

        totalFrames.push_back(frame.clone());
    }
    bapple.release();

    for(const auto &frame:totalFrames)
    {
        std::string asciiArt{};
        for(int i{0}; i < frame.rows; i++)
        {
            for(int j{0}; j < frame.cols; j++)
            {
                const std::string asciiCharset{" .'-=*%#@"};
                int brightness{frame.at<uchar>(i, j)};
                char pixelChar{brightness2Ascii(brightness, asciiCharset)};
                asciiArt += pixelChar;
            }
            asciiArt += '\n';
        }
        // system("cls");
        std::cout << asciiArt;
        cv::waitKey(fps);

    }

    return 0;
}