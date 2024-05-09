#include <iostream>
#include <vector>
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
    Sleep(1500);
}

cv::VideoCapture openVid()
{
    std::string filePath{R"(C:\Users\Harold.DESKTOP-UJ6F4M4\Videos\video.mp4)"};
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
    Sleep(1500);

    return bapple;
}

void showFileInfo(cv::VideoCapture bapple, int width, int height)
{
    std::cout << "Resolution: " << width << " x " << height << '\n';
    Sleep(1500);
}

void preloadFrame(cv::VideoCapture bapple, std::vector<cv::Mat> &totalFrames, cv::Mat &frame)
{
    std::cout << "Creating empty frame..." << '\n';
    while(bapple.read(frame))
    {
        totalFrames.push_back(frame.clone());
    }
    bapple.release();
    std::cout << "Empty frame is created!" << '\n';
    Sleep(1500);
}

char brightness2Ascii(int brightness, std::string asciiCharset)
{
    int index(brightness * asciiCharset.size() / 256);
    return asciiCharset[index];
}

void populateFrame(cv::Mat &frame, cv::Mat &grayFrame, std::vector<cv::Mat> &totalFrames, std::vector<std::string> &asciiFrames, int completeFrameAmount, int frameIndex, int width, int height, int aspectRatio)
{
    std::cout << "Populating empty frame with strings..." << '\n';
    Sleep(1500);

    for(const auto &frame : totalFrames)
    {
        std::string asciiArt{};

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        cv::resize(grayFrame, grayFrame, cv::Size(2 * width / aspectRatio, height / aspectRatio), 0, 0);

        asciiArt.clear();

        for(int i{0}; i < grayFrame.rows; i++)
        {
            for(int j{0}; j < grayFrame.cols; j++)
            {
                const std::string asciiCharset{".-=*%#@"};
                int brightness{grayFrame.at<uchar>(i, j)};
                char pixelChar{brightness2Ascii(brightness, asciiCharset)};
                asciiArt += pixelChar;
            }
            asciiArt += '\n';
        }
        asciiFrames.push_back(asciiArt);
        frameIndex++;


        std::cout << frameIndex << " out of " << completeFrameAmount << " frames processed." << std::flush << '\n';
    }

    std::cout << "Population done!" << '\n';
}

void playAscii(std::vector<std::string> &asciiFrames)
{
    PlaySound(TEXT(R"(C:\Users\Harold.DESKTOP-UJ6F4M4\Music\bad_apple.wav)"), NULL, SND_FILENAME | SND_ASYNC);
    for(int i{0}; i < asciiFrames.size(); i++)
    {
        std::cout << asciiFrames[i] << std::flush;
        cv::waitKey(29);
    }
}

int main()
{
    checkForOpenCV();

    cv::VideoCapture bapple{openVid()};

    int width(bapple.get(cv::CAP_PROP_FRAME_WIDTH));
    int height(bapple.get(cv::CAP_PROP_FRAME_HEIGHT));

    bapple.get(cv::CAP_PROP_FPS);

    showFileInfo(bapple, width, height);

    std::vector<cv::Mat> totalFrames{};
    cv::Mat frame{};
    preloadFrame(bapple, totalFrames, frame);

    int aspectRatio{12};
    cv::Mat grayFrame{};
    std::vector<std::string> asciiFrames{};
    int completeFrameAmount(totalFrames.size());
    int frameIndex{0};

    populateFrame(frame, grayFrame, totalFrames, asciiFrames, completeFrameAmount, frameIndex, width, height, aspectRatio);

    system("pause");

    playAscii(asciiFrames);

    return 0;
}