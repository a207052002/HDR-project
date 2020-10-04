#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "load_img.h"


//(vector of (img, exposure time), img size, amount of photos)
std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> load_img(std::string files_list) {
    std::ifstream list_file(files_list + "img_list.txt");
    if(list_file.fail()) {
        std::cerr << "reading the txt file is failed." << std::endl;
    }


    int img_size;
    int rows = 0;
    int cols = 0;
    std::vector<std::tuple<cv::Mat, double>> imgs;

    std::string tmp_line;
    std::stringstream ss;


    while(std::getline(list_file, tmp_line)) {
        std::string img_file_name;
        double exposure;

        ss << tmp_line;
        std::cout << tmp_line << std::endl;
        ss >> img_file_name;
        std::cout << "reading img: " << img_file_name << std::endl;
        ss >> exposure;
        std::cout << "reading img exp: " << exposure << std::endl;

        ss.clear();
        ss.str("");

        cv::Mat img;
        //std::cout << "reading img: " << img_file_name << std::endl;
        img = cv::imread((files_list + img_file_name).c_str(), CV_LOAD_IMAGE_COLOR);
        if(!img.data) {
            std::cerr << "reading img is failed" << std::endl;
            break;
        }
        rows = img.rows;
        cols = img.cols;
        img_size = img.rows * img.cols;
        imgs.push_back(std::make_tuple(img, exposure));
    }
    std::cout << "the img size is: " << rows << " * " << cols << std::endl;
    std::cout << "so the total pixels amount is: " << rows * cols << std::endl;
    std::cout << "sorting imgs with exposure time." << std::endl;
    std::sort(imgs.begin(), imgs.end(), [](const std::tuple<cv::Mat, double> &img1, const std::tuple<cv::Mat, double> &img2) {
        return std::get<double>(img1) < std::get<double>(img2);
    });

    return std::make_tuple(imgs, img_size, imgs.size());
}
    
