#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "tonemapping_l.h"

const double gamma_value = 1.5;


cv::Mat tone_mapping_l(cv::Mat hdr_image) {
  cv::Mat n_hdr = hdr_image;
  int rows = hdr_image.rows;
  int cols = hdr_image.cols;

  double min_v = std::numeric_limits<double>::infinity();
  double max_v = -std::numeric_limits<double>::infinity();

  for (int i = 0; i != rows; ++i) {
    for (int j = 0; j != cols; ++j) {
      for (int channel = 0; channel != 3; ++channel) {
        min_v = std::min(min_v, static_cast<double>(n_hdr.at<cv::Vec3f>(i, j)[channel]));
        max_v = std::max(max_v, static_cast<double>(n_hdr.at<cv::Vec3f>(i, j)[channel]));
      }
    }
  }
  std::cout << "max: " << max_v << std::endl;
  std::cout << "min: " << min_v << std::endl;

  for(int i = 0 ; i != rows ; i++) {
    for(int j = 0 ; j != cols ; j++) {
      for(int channel = 0 ; channel !=3 ; channel++) {
        double p = static_cast<double>(n_hdr.at<cv::Vec3f>(i, j)[channel]);
        n_hdr.at<cv::Vec3f>(i, j)[channel] = p / (1 + p);
      } 
    }
  }

  for(int i = 0 ; i != rows ; i++) {
    for(int j = 0 ; j != cols ; j++) {
      for(int channel = 0 ; channel != 3 ; channel++) {
        double p = static_cast<double>(n_hdr.at<cv::Vec3f>(i, j)[channel]);
        n_hdr.at<cv::Vec3f>(i, j)[channel] = std::pow(p, gamma_value);
      }
    }
  }

  return n_hdr * 255;
}
