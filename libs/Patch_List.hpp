//
// Created by fabian on 07.06.21.
//

#ifndef PICTURETOWOOD_PATCH_LIST_HPP
#define PICTURETOWOOD_PATCH_LIST_HPP


#include "Cell.hpp"
#include "Pattern.hpp"

enum alignment{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT
};

enum style{
    RECTANGLE,
    GRID

};

struct patch {
    cell target; //patch that should be approximated
    cell source; //patch that approximates the target patch
};

class patch_list {
public:
    std::vector<std::vector<patch>> patches;


    patch_list(picture &p, Pattern &ptrn, alignment align=CENTER);
    void save_patches(const std::string& path);

    cv::Size size;
    cv::Point offset;
    cv::Mat shape;
    Pattern *pattern;
    int _stepX = -1;
    int _stepY = -1;

    /** LEGACY CODE
    patch_list(picture &p, int x, int y, style style=RECTANGLE, alignment align=CENTER);
    patch_list(picture &p, cv::Mat &shape, alignment align=CENTER);
    patch_list(picture &p, cv::Mat &shape, int stepX, int stepY, alignment align=CENTER);
    void cutIntoShape(picture &p, alignment align);
    double croppingLoss(int width, int height) const;
    void croppingAdjust(int &width, int &height, bool keepRatio = true);
     **/
};


#endif //PICTURETOWOOD_PATCH_LIST_HPP
