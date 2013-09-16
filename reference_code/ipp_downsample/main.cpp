
#include <opencv2/opencv.hpp>
#include "ipp.h"
#include "ippi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "common/helpers.h"
#include "helpers_ipp.h"
using namespace std;
using namespace cv;


//from my StackOverflow post: http://stackoverflow.com/questions/13465914/using-opencv-mat-images-with-intel-ipp
void demoIppConvolution(){
    Mat img = imread("./Lena.pgm"); //OpenCV 8U_C3 image
    Mat outImg = img.clone(); //allocate space for convolution results

    int step = img.cols*3; //pitch
    const Ipp32s kernel[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    IppiSize kernelSize = {3,3};
    IppiSize dstRoiSize = {img.cols - kernelSize.width + 1, img.rows - kernelSize.height + 1};
    IppiPoint anchor = {2,2};
    int divisor = 1;

    IppStatus status = ippiFilter_8u_C3R((const Ipp8u*)&img.data[0], step,
                                         (Ipp8u*)&outImg.data[0], step, dstRoiSize,
                                         kernel, kernelSize, anchor, divisor);
    forrestWritePgm(outImg, "Lena_ipp.pgm");  
}

Mat downsampleWithIPP(Mat img, int scale){  
    int inWidth = img.cols;  
    int inHeight = img.rows;  
    int nChannels = img.depth();  
  
    assert(nChannels == 3);  
    assert(img.type() == CV_8UC3);  
  
    int outWidth = round(inWidth * scale);  
    int outHeight = round(inHeight * scale);  
  
    Mat outImg(outWidth, outHeight, CV_8UC3);  
    Ipp8u* pSrc = (Ipp8u*)&img.data[0];  
    Ipp8u* pDst = (Ipp8u*)&outImg.data[0];  
  
    IppiRect srcRect = {0, 0, inWidth, inHeight};  
    IppiRect dstRect = {0, 0, outWidth, outHeight};  
    IppiSize srcSize = {inWidth, inHeight};  
    IppiSize dstSize = {outWidth, outHeight};  
  
    int srcStep = inWidth * nChannels;  
    int dstStep = outWidth * nChannels;  
    IppiPoint dstOffset = {0, 0};  

    int bufsize; 
    IppStatus status = ippiResizeGetBufSize(srcRect, dstRect, nChannels, IPPI_INTER_LINEAR, &bufsize); 
    Ipp8u* pBuffer = (Ipp8u*)ippMalloc(bufsize);  
  
    IppiResizeSpec_32f pspec; //TODO: ippiMalloc this if we have problems    
    IppStatus ippiResizeLinearInit_8u_C3R(IppiSize srcSize,  
                                          IppiSize dstSize,  
                                          &pSpec);  
  
    //http://software.intel.com/sites/products/documentation/doclib/ipp_sa/71/ipp_manual/IPPI/ippi_ch12/functn_ResizeLinear.htm 
    //example: https://github.com/albertoruiz/easyVision/blob/master/packages/imagproc/lib/ImagProc/Ipp/auxIpp.c  
    IppStatus ippiResizeLinear_8u_C3R(pSrc,  
                                      srcStep,  
                                      pDst,  
                                      dstStep,  
                                      dstOffset,  
                                      dstSize,  
                                      ippBorderRepl,
                                      NULL, //borderValue
                                      &pSpec, //might need to do '&pSpec'  
                                      pBuffer /* temporary scratch space */ );  
  
    ippiFree(pBuffer);  
  
    return outImg;  
}


int main (int argc, char **argv){
    demoIppConvolution();

    Mat img = imread("./Lena.pgm"); //OpenCV 8U_C3 image
    int scale = 0.75; //arbitrary

    Mat img_scaled = downsampleWithIPP(img, scale);    

    return 0;
}
