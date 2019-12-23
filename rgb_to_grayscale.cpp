#include <opencv2/opencv.hpp>

using namespace cv;

int main( int argc, char** argv )
{
    char* imageName = argv[1];
    Mat image;
    image = imread( imageName, IMREAD_COLOR );
    if( argc != 2 || !image.data )
    {
        printf( " No image data \n " );
        return -1;
    }
    Mat gray_image;

    cvtColor( image, gray_image, COLOR_BGR2GRAY );

    

    namedWindow( imageName, WINDOW_NORMAL );
    namedWindow( "Gray image", WINDOW_NORMAL );

    Mat imageS;
    Mat gray_imageS;

    resize(image, imageS, Size(), 0.1, 0.1 );
    resize(gray_image, gray_imageS, Size(), 0.1, 0.1);

    //Save the video
    //imwrite( "../../images/Gray_Image.jpg", gray_imageS );

    imshow( imageName, imageS );
    imshow( "Gray image", gray_imageS );
    waitKey(0);
    return 0;
}