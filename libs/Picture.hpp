//
// Created by fabian on 17.05.21.
//

#ifndef PICTURETOWOOD_PICTURE_HPP
#define PICTURETOWOOD_PICTURE_HPP

#include <iostream>
#include "opencv2/highgui.hpp"
#include <boost/filesystem.hpp>
#include <opencv2/imgproc.hpp>


struct image_set{
    cv::Mat img;
    cv::Mat img_gray;
    cv::Mat mask;
};

class picture {
    public:
        std::string name;
        std::vector<struct image_set> images;

        explicit picture(char *path);
        void show() const;
        void loadImg(char *path);
        void addRotations(int n);
};


#endif //PICTURETOWOOD_PICTURE_HPP
