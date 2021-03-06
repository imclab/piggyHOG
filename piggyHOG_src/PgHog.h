#ifndef __PgHog_H__
#define __PgHog_H__
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "PgHogContainer.h"
#include "common/helpers.h"

using namespace std;
using namespace cv;

//fast HOG feature extraction
// complies with the UOC-TTI Deformable Parts Model HOG extractor

class PgHog{

    public:

        PgHog(); //precompute tables, stuff like that 
        ~PgHog();

        PgHogContainer* extract_HOG_oneScale(Mat img, int spatialBinSize); //in voc-release5, spatialBinSize is typically 4 or 8 pixels

        //compute HOGs at many scales of downsampling
        vector<PgHogContainer*> extract_HOG_pyramid(Mat img, int padx, int pady);

        //compute the gradient and magnitude at one image location, store the results in gradImg and magImg
        //out of the 3 RGB channels, use the gradient that has the highest magnitude
        void gradient(int x, int y, Mat img, Mat &gradImg, Mat &magImg);

        //compute one HOG cell, storing the cell in hogResult
        void hogCell(int hogX, int hogY, Mat &oriImg, Mat &magImg, PgHogContainer* hogResult);

        //pool contrast-sensitive (0-360) into contrast-insensitive (0-180), store in hogResult
        void hogCell_unsigned(int hogX, int hogY, PgHogContainer* hogResult);

        //compute the norm of one hog cell (sum up its 'energy'), store it in normImg
        void hogCell_gradientEnergy(int hogX, int hogY, PgHogContainer* hogResult, Mat &normImg);

        //void hogBlock(...) //compute HOG block location at one x,y location (indexing into HOG cells)
        void hogBlock_normalize(int hogX, int hogY, PgHogContainer* hogResult, Mat normImg);
        
        //add binary place features around padded border of a HOG image
        void hogPlaceFeatures_border(PgHogContainer* hogResult);

    private:
        //arctan lookup table 
        float ATAN2_TABLE[512][512]; //I'd like to make this 'static', but the compiler gives me 'undefined reference' error

};

#endif

