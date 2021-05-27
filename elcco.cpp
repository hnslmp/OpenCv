#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;

Mat image; Mat imageS; Mat result; Mat canny_output; Mat imgHSV; Mat imgThresholded1; Mat imgThresholded2;
Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

int thresh = 100;

int max_thresh = 255;
int min_area = 600;
RNG rng(12345);
int largest_area=0;
int largest_contour_index=0;


const char* window_name1 = "Threshold 1";
const char* window_name2 = "Threshold 2";

void contour_callback (int, void*);

int main (int argc, char** argv){
    
    VideoCapture cap(4);
    cap.get(10); 
    
	    // Check if camera opened successfully
	    if(!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	    }

    //First Parameter
    int iLowH1 = 26;
    int iHighH1 = 156;
    int iLowS1 = 255; 
    int iHighS1 = 255;
    int iLowV1 = 83;
    int iHighV1 = 175;

    //Second Parameter
    int iLowH2 = 0;
    int iHighH2 = 255;
    int iLowS2 = 0; 
    int iHighS2 = 255;
    int iLowV2 = 0;
    int iHighV2 = 255;

    //Create Windows
    namedWindow(window_name1, CV_WINDOW_AUTOSIZE);
    namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
    namedWindow("Original", CV_WINDOW_AUTOSIZE);

    //Create Thresholding Trackbar
    cvCreateTrackbar("LowH", window_name1, &iLowH1, 255); //Hue (0 - 255)
    cvCreateTrackbar("HighH", window_name1, &iHighH1, 255);
    cvCreateTrackbar("LowS", window_name1, &iLowS1, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", window_name1, &iHighS1, 255);
    cvCreateTrackbar("LowV", window_name1, &iLowV1, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", window_name1, &iHighV1, 255);

    cvCreateTrackbar("LowH", window_name2, &iLowH2, 255); //Hue (0 - 255)
    cvCreateTrackbar("HighH", window_name2, &iHighH2, 255);
    cvCreateTrackbar("LowS", window_name2, &iLowS2, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", window_name2, &iHighS2, 255);
    cvCreateTrackbar("LowV", window_name2, &iLowV2, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", window_name2, &iHighV2, 255);

    time_t current_time;
    time_t previous_time;
    time(&previous_time);
    time(&current_time);

    while(1){
        time(&current_time);
        cap.read(image);
        // flip(image, image, +1);

        if (image.empty())
            break;

        //resize(image, image, Size(), 0.1, 0.1 );

        cvtColor (image,imgHSV, CV_BGR2HSV);
        blur( imgHSV, imgHSV, Size(3,3) );

        //Threshold the image
        inRange(imgHSV, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1);
        inRange(imgHSV, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2);

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 


        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        

        //Find Contours of shape
        Canny(imgThresholded1, canny_output, thresh, thresh*2, 3);
        findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        //drawContours( image, contours, largest_contour_index, color, 2, 8, hierarchy, 0, Point() );
        
        // if(current_time - previous_time <= 2){
        //     continue;
        // }
        // else {
        //     cout << "Print" << endl;
        //     previous_time = current_time;
        //     imshow (window_name1, imgThresholded1);
        //     imshow (window_name2, imgThresholded2);
        //     imshow ("Original",image);
        //     imwrite("Thresh1.jpg", imgThresholded1);
        //     imwrite("Thresh2.jpg", imgThresholded2);   
        // }
        
        int x = 0;
        string filename = "elcco" + x;
        x=x+1;
        //imwrite(filename, imgThresholded);
        
        imshow (window_name1, imgThresholded1);
        imshow (window_name2, imgThresholded2);
        imshow ("Original",image);

        // Press  ESC on keyboard to exit
        char c=(char)waitKey(25);
        if(c==27) break;
        
    }
  // When everything done, release the video capture object
  cap.release();
  // Closes all the frames
  destroyAllWindows();
  return 0;
}
