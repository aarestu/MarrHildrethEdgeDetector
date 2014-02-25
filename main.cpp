#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void marrEdge(const Mat src, Mat& result, int besarKernel, double delta);
void LoGKernelGenerator(Mat &result, int besarKernel, double delta);
void rgb2gray(const Mat src, Mat &result);

int main(int argc, char *argv[])
{
    Mat src = imread("D:\\Project\\C++\\CitraDigital\\lena.jpg");

    rgb2gray(src, src);

    Mat edge;

    marrEdge(src, edge, 9, 1.6);

    namedWindow("asli");
    imshow("asli", src);

    namedWindow("MarrHildrethEdgeDetector");
    imshow("MarrHildrethEdgeDetector", edge);

    waitKey(0);

}

void marrEdge(const Mat src, Mat& result, int besarKernel, double delta)
{

    //inisialisasi kernel LoG
    Mat kernel;
    LoGKernelGenerator( kernel, besarKernel, delta);

    int filterOffset = besarKernel / 2;

    Mat laplacian =  (Mat_<double>(src.rows - filterOffset * 2, src.cols - filterOffset*2));



    result = Mat::zeros(src.rows - filterOffset*2, src.cols - filterOffset*2, src.type());

    double sumLaplacian;

    for(int ysrc = filterOffset; ysrc < src.rows - filterOffset; ++ysrc){
        for(int xsrc = filterOffset; xsrc < src.cols - filterOffset; ++xsrc){

            sumLaplacian = 0;

            for(int ykernel = -filterOffset; ykernel <= filterOffset; ++ykernel){
                for(int xkernel = -filterOffset; xkernel <= filterOffset; ++xkernel){

                    sumLaplacian += src.at<uchar>(ysrc + ykernel, xsrc + xkernel) * kernel.at<double>(filterOffset + ykernel, filterOffset + xkernel);
                }
            }

            laplacian.at<double>(ysrc - filterOffset, xsrc - filterOffset) = sumLaplacian;

        }
    }


    //zero crossing
    for(int y = 1; y < result.rows - 1; ++y){
        for(int x = 1; x < result.cols - 1; ++x){

            result.at<uchar>(y,x) = 255;
            if(laplacian.at<double>(y - 1,x) * laplacian.at<double>(y + 1,x) < 0){
                result.at<uchar>(y,x) = 0;
            }
            if(laplacian.at<double>(y, x - 1) * laplacian.at<double>(y, x + 1) < 0){
                result.at<uchar>(y,x) = 0;
            }
            if(laplacian.at<double>(y + 1, x - 1) * laplacian.at<double>(y - 1, x + 1) < 0){
                result.at<uchar>(y,x) = 0;
            }
            if(laplacian.at<double>(y - 1, x - 1) * laplacian.at<double>(y + 1, x + 1) < 0){
                result.at<uchar>(y,x) = 0;
            }
        }
    }
}

//laplacian of Gaussian (LoG)
void LoGKernelGenerator(Mat &result, int besarKernel, double delta)
{
    int kernelRadius = besarKernel / 2;
    result = Mat_<double>(besarKernel, besarKernel);


    for(int filterY = - kernelRadius; filterY <= kernelRadius; filterY++){
        for(int filterX = - kernelRadius; filterX <= kernelRadius; filterX++){

            result.at<double>(filterY + kernelRadius, filterX + kernelRadius) =
                    exp(-( ( pow(filterX, 2)  + pow( filterY, 2 )  ) / ( pow(delta, 2) * 2) ))
                    * ( ( ( pow(filterX, 2)  + pow( filterY, 2 ) - 2 * pow(delta, 2) ) /  (2 * pow(delta, 4) ) ));

        }
    }

    //cout<< result;
}


void rgb2gray(const Mat src, Mat &result)
{
    CV_Assert(src.depth() != sizeof(uchar)); //harus 8 bit

    result = Mat::zeros(src.rows, src.cols, CV_8UC1); //buat matrik 1 chanel
    uchar data;

    if(src.channels() == 3){

        for( int i = 0; i < src.rows; ++i)
            for( int j = 0; j < src.cols; ++j )
            {
                data = (uchar)(((Mat_<Vec3b>) src)(i,j)[0] * 0.0722 + ((Mat_<Vec3b>) src)(i,j)[1] * 0.7152 + ((Mat_<Vec3b>) src)(i,j)[2] * 0.2126);

                result.at<uchar>(i,j) = data;
            }


    }else{

        result = src;
    }

}
