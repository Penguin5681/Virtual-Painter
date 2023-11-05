# Virtual Painter Using OpenCV C++ 

**Overview:**
Virtual Painter is an OpenCV-based C++ project that allows you to draw on a canvas using colored objects in front of a webcam. This project detects predefined colors (in this case, green and orange) and tracks their movement, enabling you to paint on a digital canvas in real-time.

**Usage:**
1. Clone or download the project repository.
2. Make sure you have OpenCV installed and configured in your C++ development environment.
3. Compile the project using a C++ compiler (Make sure to configure it using CMake).
4. Run the compiled executable.

**Code Overview:**

```cpp
// Include necessary libraries
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

// Define color ranges for detection
const vector<vector<int>> colors = {
    {0, 179, 224, 255, 255, 255},   // orange (kinda)
    {40, 91, 139, 255, 255, 255}    // green
};

// Initialize a video capture object
VideoCapture cam(0);
Mat img;

// Main loop for real-time painting
signed main() {
    while (true) {
        cam.read(img);
        newPoints = get_colors(img);
        draw_on_canvas(img, newPoints, colorValue);
        imshow("Webcam", img);
        waitKey(1);
    }
}
```

**How it Works:**

1. The project captures video from the webcam in real-time.
2. It detects and tracks objects of predefined colors (green and orange) using OpenCV. However you can add more colors using the HSV values.
3. The detected objects' positions are used to draw on the digital canvas.

**Adding more Colors:**
```cpp
    // values from the function go to colors vector
    void color_picker() {
        VideoCapture cap(0);
        Mat image;
        Mat image_HSV, mask;


        int hue_min = 0; int sat_min = 0; int val_min = 0;
        int hue_max = 19; int sat_max = 240; int val_max = 255;

        namedWindow("Trackbars", (640, 200));

        createTrackbar("Hue Min", "Trackbars", &hue_min, 179);
        createTrackbar("Sat Min", "Trackbars", &sat_min, 179);
        createTrackbar("Val Min", "Trackbars", &val_min, 255);
        createTrackbar("Hue Max", "Trackbars", &hue_max, 255);
        createTrackbar("Sat Max", "Trackbars", &sat_max, 255);
        createTrackbar("Val Max", "Trackbars", &val_max, 255);

        while (true) {
            cap.read(image);
            cvtColor(image, image_HSV, COLOR_BGR2HSV);
            inRange(image_HSV, Scalar(hue_min, sat_min, val_min), Scalar(hue_max,       sat_max, val_max), mask);
            cap.read(image);
            cout << "{" << hue_min << ", " << sat_min << ", " << val_min << ", " << hue_max << ", " << sat_max << ", " << val_max << "}" << endl;
            imshow("Original", image);
            imshow("Original HSV", image_HSV);
            imshow("Masked", mask);
            waitKey(1);
        }
    }

    // convert the hsv into bgr format and put into colorValue vector
    Scalar hsvToBgr(int hueMin, int satMin, int valMin) {
        Mat hsv(1, 1, CV_8UC3, Scalar(hueMin, satMin, valMin));
        Mat bgr;

        cvtColor(hsv, bgr, COLOR_HSV2BGR);

        Vec3b bgrScalar = bgr.at<Vec3b>(0, 0);
        return Scalar(bgrScalar[0], bgrScalar[1], bgrScalar[2]);
    }
``` 

**Color Detection:**

- The `get_colors` function converts the captured video frames to the HSV color space and then applies predefined color ranges to create masks for the specified colors.
- It finds contours in the masks to detect the objects and their positions.

**Drawing on Canvas:**

- The `draw_on_canvas` function takes the detected object positions and draws circles on the canvas using the corresponding color.