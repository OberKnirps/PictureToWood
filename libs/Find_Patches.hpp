//
// Created by fabian on 25.05.21.
//

#ifndef PICTURETOWOOD_FIND_PATCHES_HPP
#define PICTURETOWOOD_FIND_PATCHES_HPP

#include <vector>
#include <functional>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include "opencv2/highgui.hpp"
#include "Cell.hpp"
#include "Picture.hpp"
#include "Patch_List.hpp"
#include <opencv2/saliency.hpp>

long compareFilter(const cell& a, const cell& b);
long compareGray(const cell& a, const cell& b);

void
findMatchingPatches(patch_list &target, std::vector<picture> &source, const int stepX, const int stepY, double cutWidth,
                    const std::function<long(const cell &, const cell &)> &comp);

std::string assembleOutput(patch_list &patches, std::string appendix= "");

void generateCutMap(patch_list &patches, int outputDPI, double cutWidth, std::string outputPath, double textScale,
                    bool flipV, bool flipH);


#endif //PICTURETOWOOD_FIND_PATCHES_HPP
