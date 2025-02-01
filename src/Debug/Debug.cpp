#include "Debug.h"

namespace debug {
    const std::string windowName = XorStr("AimAheadWnd");
    cv::Mat sharedImage;
    std::mutex sharedImageMutex;
    const int cordinateOffset = 640 / 2.0;

	void init() {
#ifndef __DEBUG__
        return;
#endif


        cv::Mat blankImage(640, 640, CV_8UC3, cv::Scalar(0, 0, 0)); 

        cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE); 
        cv::resizeWindow(windowName, 640, 640);

        sharedImage = blankImage.clone();

        HWND hwnd = utils::getHwndFromTitle(windowName);
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        while (true) {
            try {
                if (!sharedImage.empty()) {
                    if (!sharedImageMutex.try_lock()) {
                        continue;
                    }
                    cv::imshow(windowName, sharedImage);
                    sharedImageMutex.unlock();
                }
                else {
                    std::cout << XorStr("Error: Cannot display an empty image.") << std::endl;
                }
            }
            catch (const cv::Exception& e) {
                std::cout << XorStr("OpenCV Error: ") << e.what() << std::endl;
            }
            cv::waitKey(1);
        }

        cv::destroyWindow(windowName);
	}

    void drawBox(cv::Mat& image, int minX, int minY, int maxX, int maxY, const cv::Scalar& color, int thickness = 2) {
        cv::Point topLeft(minX, minY);
        cv::Point bottomRight(maxX, maxY);

        cv::rectangle(image, topLeft, bottomRight, color, thickness);
    }




    void update_image(cv::Mat cpuMat) {
#ifndef __DEBUG__
        return;
#endif /
        if (!sharedImageMutex.try_lock()) {
            return;
        }
        sharedImage = cpuMat;
        sharedImageMutex.unlock();
    }

    void draw_boxes(std::vector<BoundingBox>& boxes) {
#ifndef __DEBUG__
        return;
#endif /

        for (int i = 0; i < boxes.size(); i++) {
            BoundingBox box = boxes[i];
            cv::Scalar boxColor(230, 30, 30);
            drawBox(sharedImage, box.xmin, box.ymin, box.xmax, box.ymax, boxColor, 1);
        }
    }

    void draw_target_location(std::pair<double, double> cords) {
#ifndef __DEBUG__
        return;
#endif /

        int thickness = cv::FILLED;
        cv::Scalar color(5, 5, 230);
        int radius = 5;
        cv::Point center(cords.first * 5 + cordinateOffset, cords.second * 5 + cordinateOffset);
        cv::circle(sharedImage, center, radius, color, thickness);
    }

    void draw_target_prediction(std::pair<double, double> oldCords, std::pair<double, double> newCords) {
#ifndef __DEBUG__
        return;
#endif /
        cv::Scalar color(5, 230, 5);
        double sens = 1/0.2;

        oldCords.first = oldCords.first * sens + cordinateOffset;
        oldCords.second = oldCords.second * sens + cordinateOffset;
        newCords.first = newCords.first * sens + cordinateOffset;
        newCords.second = newCords.second * sens + cordinateOffset;

        cv::Point startPoint(oldCords.first, oldCords.second);
        cv::Point endPoint(newCords.first, newCords.second);
        cv::line(sharedImage, startPoint, endPoint, color, 2);
        cv::circle(sharedImage, endPoint, 5, color, cv::FILLED);
    }
}