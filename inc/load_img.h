#ifndef LOAD_IMG
#define LOAD_IMG
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <opencv2/opencv.hpp>

std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> load_img(std::string files_list);

#endif
