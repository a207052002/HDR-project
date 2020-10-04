#ifndef RECOVER
#define RECOVER

#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <armadillo>
#include <opencv2/opencv.hpp>

int weight(int z);
std::vector<arma::vec> gslove(std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> imgs);
cv::Mat buildHDR(std::vector<std::tuple<cv::Mat, double>> imgs, std::vector<arma::vec> gs);
cv::Mat recover(std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> imgs);

#endif
