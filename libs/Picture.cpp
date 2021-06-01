//
// Created by fabian on 17.05.21.
//

#include "Picture.hpp"
using namespace cv;

/**
 * Load the Image specified by path into the Picture object.
 * @param path Absolute path or relative path from the working directory.
 * @return Creates a Picture Object containing the colored and grayscale version of the image.
 */
Picture::Picture(char *path) {
    this->loadImg(path);
}

/**
 * Show the colored and grayscale version of the image.
 */
void Picture::show() const {
    if(not img.empty()){
        namedWindow("Image", WINDOW_AUTOSIZE);
        imshow( "Image", this->img);
        waitKey ( 10000);//TODO replace with better solution for waiting
    } else{
        std::cout << "No image to show.";
    }
}

/**
 * Load the Image specified by path into the Picture object, replacing the current images and replacing the name with the new file name.
 * @param path Absolute path or relative path from the working directory.
 */
void Picture::loadImg(char *path) {
    try {
        String file_path = samples::findFile(path);
        this->img = imread(file_path, IMREAD_COLOR);
        this->name = file_path.substr(file_path.find_last_of('/')+1, file_path.find_first_of('.') - file_path.find_last_of('/')-1);
    }catch (const std::exception& e) {
        std::cout << "Could not read image because of:\n" << e.what();
    }
}

/**
 * Cut both versions of the picture into rectangular patches. The patches are contained in patches and patches_gray.
 * @param width Width of the patches in px.
 * @param height Height of the patches in px.
 * @param align Controls where the rectangle containing the patches will start. If the image needs to be croped, this will control where the cropping takes place. E.g TOP_LEFT meaning the Bottom and right boarder will be discarded.
 */
void Picture::cutIntoRect(int width, int height, alignment align) {
    Rect rec;
    rec.width = width;
    rec.height = height;
    int offsetX = 0;
    int offsetY = 0;
    //calculate offset of patches
    switch(align){
        case TOP_LEFT:
            offsetX = 0;
            offsetY = 0;
            break;
        case TOP:
            offsetX = (img.cols % width)/2;
            offsetY = 0;
            break;
        case TOP_RIGHT:
            offsetX = (img.cols % width);
            offsetY = 0;
            break;
        case LEFT:
            offsetX = 0;
            offsetY = (img.rows % height)/2;
            break;
        case CENTER:
            offsetX = (img.cols % width)/2;
            offsetY = (img.rows % height)/2;
            break;
        case RIGHT:
            offsetX = (img.cols % width);
            offsetY = (img.rows % height)/2;
            break;
        case BOTTOM_LEFT:
            offsetX = 0;
            offsetY = (img.rows % height);
            break;
        case BOTTOM:
            offsetX = (img.cols % width)/2;
            offsetY = (img.rows % height);
            break;
        case BOTTOM_RIGHT:
            offsetX = (img.cols % width);
            offsetY = (img.rows % height);
            break;
    }

    std::cout << this->croppingLoss(width,height)*100 << "% of the picture will be lost due to cropping!\n";
    std::cout << "cols:"<< img.cols / width << "\n";
    std::cout << "rows:"<< img.rows / height << "\n";
    //this->croppingAdjust(width,height);
    this->patches = std::vector<std::vector<cell>>(img.cols / width);

    //go from top left to bottom right
    for(int x = 0; offsetX + x*width +width<= img.cols; x++){
        this->patches[x].reserve(img.rows / height);
        for(int y = 0; offsetY + y*height + height <= img.rows; y++){
            rec.x = offsetX + x * width;
            rec.y = offsetY + y * height;

            //create patch and put it into list
            this->patches[x].emplace_back(img(rec));
            //this->patches.back().show();
        }
    }

}


/**
 * Cut both versions of the picture into square patches. The patches are contained in patches and patches.
 * @param width Width and height of the patches in px.
 * @param align Controls where the rectangle containing the patches will start. If the image needs to be croped, this will control where the cropping takes place. E.g TOP_LEFT meaning the Bottom and right boarder will be discarded.
 */
void Picture::cutIntoSquares(int width, alignment align) {
    this->cutIntoRect(width, width,align);
}


/**
 * Calculates the percentage of image area that will be lost, if the image is cut into patches of size width x height.
 * @param width Width of the patches in px.
 * @param height Height of the patches in px.
 * @return Percentage of image area that would be lost, using these dimensions in range from 0-1.
 */
double Picture::croppingLoss(int width, int height) const {
    int newWidth;
    int newHeight;

    newHeight = img.rows - img.rows % height;
    newWidth = img.cols - img.cols % width;

    return 1.0 - (double(newWidth*newHeight) / double(img.cols*img.rows));
}

/**
 * Adjusts the parameters to minimize the loss due to cropping.
 * @param width Width of the patches in px. Will be changed if a better value is found.
 * @param height Height of the patches in px. Will be changed if a better value is found.
 * @param keepRatio Specifics if the ratio between width and height should be kept the same or not.
 * @return Writes return values into width and height.
 */
void Picture::croppingAdjust(int &width, int &height, bool keepRatio) {
    if(keepRatio) {
        double ratio = (double) width / (double) height;
        double loss = this->croppingLoss(width, height);
        double minLoss = loss;
        int originalWidth = width;
        int newWidth;
        int newHeight;
        for (int x = -20; x <= 20; x++) {
            newWidth = originalWidth + x;
            newHeight = newWidth / ratio;
            loss = this->croppingLoss(newWidth, newHeight);
            if (loss < minLoss) {
                width = newWidth;
                height = newHeight;
                minLoss = loss;
            }
        }
        if (width != originalWidth) {
            std::cout << "Better size for current Ratio in a 20 Pixel range found for: width = " << width
                      << "| height = " << height << " with a loss of " << minLoss * 100 << "%";
        }
    }else{
     //TODO
    }
}

/**
* Cut both versions of the picture into square patches. The patches are contained in patches
* @param cols The number of columns the picture will be divided into.
* @param align Controls where the rectangle containing the patches will start. If the image needs to be cropped, this will control where the cropping takes place. E.g TOP_LEFT meaning the Bottom and right boarder will be discarded.
*/
void Picture::cutIntoGrid(int cols, alignment align) {
    int width = img.cols/cols;
    int height = width;
    //this->croppingAdjust(width,height); //TODO fix adjust for small(under 20px) patches
    this->cutIntoSquares(width,align);
}

/**
* Cut both versions of the picture into rectangular patches. The patches are contained in patches
* @param cols The number of columns the picture will be divided into.
* @param rows The number of rows the picture will be divided into.
* @param align Controls where the rectangle containing the patches will start. If the image needs to be cropped, this will control where the cropping takes place. E.g TOP_LEFT meaning the Bottom and right boarder will be discarded.
*/
void Picture::cutIntoGrid(int cols, int rows, alignment align) {
    int width = img.cols/cols;
    int height = img.rows/rows;
    this->croppingAdjust(width,height);
    this->cutIntoRect(width,height,align);
}

/**
* Saves the patches into the specified directory.
* @param path Directory name or relative directory path.
*/
void Picture::save_patches(const std::string& path) {
    //checks if the output directory exist or creates
    std::string output_path = boost::filesystem::current_path().string()+ "/" + path;
    if(not boost::filesystem::exists(output_path))
        boost::filesystem::create_directory(output_path);

    //creates a sub directory for the image or clears an existing one
    output_path += "/" + this->name;
    if(boost::filesystem::exists(output_path)){
        boost::filesystem::remove_all(output_path);
        boost::filesystem::create_directory(output_path);
    }else{
        boost::filesystem::create_directory(output_path);
    }

    //writes the images
    for(int x = 0; x < patches.size(); x++) {
        for (int y = 0; y < patches[x].size(); y++) {
            imwrite(output_path + "/" + "n" + std::to_string(x) + "," + std::to_string(y) + ".tiff", patches[x][y].img);
            imwrite(output_path + "/" + "g" + std::to_string(x) + "," + std::to_string(y) + ".tiff", patches[x][y].img_gray);
        }
    }
}

