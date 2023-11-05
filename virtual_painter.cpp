#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Scalar hsvToBgr(int hueMin, int satMin, int valMin) {
    Mat hsv(1, 1, CV_8UC3, Scalar(hueMin, satMin, valMin));
    Mat bgr;
    cvtColor(hsv, bgr, COLOR_HSV2BGR);
    Vec3b bgrScalar = bgr.at<Vec3b>(0, 0);
    return Scalar(bgrScalar[0], bgrScalar[1], bgrScalar[2]);
}

const vector<vector<int>> colors = {
    {0, 179, 224, 255, 255, 255},
    {40, 91, 139, 255, 255, 255}
};

const vector<Scalar> colorValue = {
    hsvToBgr(40, 91, 139),
    hsvToBgr(0, 179, 224)
};

vector<vector<int>> newPoints;
VideoCapture cam(0);
Mat img;

Point getContours(Mat src) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> contourPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    Point myPoint(0, 0);

    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);
        if (area > 1e3) {
            float perimeter = arcLength(contours[i], true);
            approxPolyDP(contours[i], contourPoly[i], 0.02 * perimeter, true);
            boundRect[i] = boundingRect(contourPoly[i]); 
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;
        }
    }

    return myPoint;
}

vector<vector<int>> getColors(Mat src) {
    Mat srcHsv;
    for (int i = 0; i < colors.size(); i++) {
        cvtColor(src, srcHsv, COLOR_BGR2HSV);
        Scalar lowerB(colors[i][0], colors[i][1], colors[i][2]);
        Scalar upperB(colors[i][3], colors[i][4], colors[i][5]);
        
        Mat mask;
        inRange(srcHsv, lowerB, upperB, mask);
        Point myPoint = getContours(mask);

        if (myPoint.x != 0)
            newPoints.push_back({myPoint.x, myPoint.y, i});
    }

    return newPoints;
}

void drawOnCanvas(Mat &img, vector<vector<int>> newPoints, vector<Scalar> colorValue) {
    for (int i = 0; i < newPoints.size(); i++) {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 8, colorValue[newPoints[i][2]], FILLED);
    }
}

int main() {    
    while (true) {
        cam.read(img);
        newPoints = getColors(img);
        drawOnCanvas(img, newPoints, colorValue);
        imshow("Webcam", img);
        waitKey(1);
    }
}
