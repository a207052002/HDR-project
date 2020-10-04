#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#include "load_img.h"
#include "recover.h"
#include "tonemapping.h"
#include "tonemapping_l.h"

int main(int argc, char *argv[]){
    std::string list_file(argv[1]);
    list_file += "/";
    std::cout << "loading images with the list." << std::endl;
    auto imgs = load_img(list_file);
    auto hdr_img = recover(imgs);

    cv::imwrite(list_file + "hdr_result.hdr", hdr_img);
    auto tonemap_img = tone_mapping(hdr_img);
    cv::imwrite(list_file + "ton_mapping.jpg", tonemap_img);

    return 0;
}
