#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

Mat image; Mat imageS; Mat result; Mat canny_output;
Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

int thresh = 100;
int max_thresh = 255;
int min_area = 600;
int roi_y = 0;
RNG rng(12345);

int count_red = 0;

string window_name = "Shape Detection";

void contour_callback (int, void*);

int main (int argc, char** argv){

  char* imageName = argv[1];
  image = imread(imageName, 1);

  //resize(image, image, Size(), 0.1, 0.1 );
  cvtColor (image,imageS, CV_BGR2GRAY);
  blur( imageS, imageS, Size(3,3) );
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;

  //Find Contours of shape
  Canny(imageS, canny_output, thresh, thresh*2, 3);
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  for (int i=0; i < contours.size();i++ ){
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

    //Find approx of the polyshape
    vector<Point> approx;
    vector<Point> cnt = contours.at(i);
    double area = contourArea(cnt);
    approxPolyDP(cnt, approx, 5, true);
  
    //Find Center of Contour
    Moments m = moments(approx,true);
    Point p(m.m10/m.m00, m.m01/m.m00);
    int x = p.x;
    int y = p.y;

    int min_x = 9999;

    //Make a region of interest 
    if (area > min_area && y > roi_y){
      
      //Count circle
      if(7 <= approx.size() <= 20){
        count_red += 1;
        
        //Find the most left circle
        if (x < min_x){
          min_x = x;
        }
      } 
    }  
  }
  
  cout << count_red;

  namedWindow(window_name, CV_WINDOW_AUTOSIZE);
  imshow (window_name, canny_output);
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  
  waitKey(0);
  return 0;

}