#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <armadillo>
#include <opencv2/opencv.hpp>
#include "recover.h"

const int n = 255;
const int lambda = 50;
const int samples = 500;


int weight(int z) {
    if(z < 256/2) {
        return z + 1;
    } else {
        return 256 - z;
    }
}


std::vector<arma::vec> gsolve(std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> imgs) {
    int pixels = std::get<1>(imgs);
    int photos = std::get<2>(imgs);
    std::vector<std::tuple<cv::Mat, double>> img_mats = std::get<0>(imgs);
    auto rows = samples * photos + n;
    auto cols = samples + n + 1;

    std::srand(std::time(nullptr));
    std::vector<int> random_index;
    for (int i = 0; i != samples; i++)
        random_index.push_back(std::rand() % pixels);

    std::vector<arma::mat> As;
    std::vector<arma::vec> bs;
    std::vector<arma::vec> gs;

    std::cout << "build zeros matrix for A and B to solve x from Ax = B." << std::endl;
    std::cout << "A is " << rows << " * " << cols << std::endl;
    std::cout << "B is " << rows << std::endl;

    for(int c = 0 ; c != 3 ; c++){
        auto tmp1 = arma::zeros<arma::mat>(rows, cols);
        auto tmp2 = arma::zeros<arma::vec>(rows);
        As.push_back(tmp1);
        bs.push_back(tmp2);
    }

    std::cout << "filling the subject matrix." << std::endl;

    for(int channel = 0 ; channel != 3 ; channel++){
        int k = 0;
        for(int i = 0 ; i != samples ; i++ ) {
            for(int j = 0 ; j != photos ; j++) {
                cv::Mat img = std::get<0>(img_mats[j]);
                int sampled_index = random_index[i];
                int z = img.data[img.channels() * sampled_index + channel];
                int wz = weight(z);
                As[channel](k, z) = wz;
                As[channel](k, 256 + i) = -wz;

                bs[channel](k) = wz * std::log(1/std::get<1>(img_mats[j]));

                k++;
            }
        }

        As[channel](k, 128) = 1;
        k++;

        for(int i = 1 ; i != 255 ; i++) {
            As[channel](k, i - 1) = lambda * weight(i);
            As[channel](k, i) = -2 * lambda * weight(i);
            As[channel](k, i + 1) = lambda * weight(i);
            k++;
        }
        std::cout << "solving the subject for channel: " << channel + 1 << "" << std::endl;
        gs.push_back(arma::solve(As[channel], bs[channel]));
        std::cout << "solved the subject for channel: " << channel + 1 << "" << std::endl;
    }

    return gs;

}

cv::Mat buildHDR(std::vector<std::tuple<cv::Mat, double>> imgs, std::vector<arma::vec> gs) {

    auto rows = std::get<0>(imgs[0]).rows;
    auto cols = std::get<0>(imgs[0]).cols;
    cv::Mat hdr_img(rows, cols, CV_32FC3);

    //cause the RGB model's pixels values are not linearly dependent,
    //so the channels will response different irradiances(Ei) after solving regression
    //to aviod the chromaticity losts in human vision, we might need some adjustment for human vision.
    //and also, the sensors/post-processing might cause the non-linear change between pixel values and irrandiance in the different exposure setting.

    for(int i = 0 ; i != rows ; i++) {
        for(int j = 0 ; j != cols ; j++) {
            double irrad_value[3] = {0, 0, 0};
            double weights_sum = 0;
            //double weights_sum[3] = {0, 0, 0};

            for(auto it = imgs.begin() ; it != imgs.end() ; it++) {
                double exposure = std::get<1>(*it);
                cv::Mat img = std::get<0>(*it);
                double weights = 0;
                //double weights[3] = {0, 0, 0};

                for(int channel = 0 ; channel != 3 ; channel++) {
                    int pixel_value = img.at<cv::Vec3b>(i, j)[channel];
                    weights += weight(pixel_value);
                    //weights[channel] = weight(pixel_value);
                }
                weights /= 3;

                for(int channel = 0 ; channel != 3 ; channel++) {
                    int pixel_value = img.at<cv::Vec3b>(i, j)[channel];
                    //irrad_value[channel] += weights[channel] * (gs[channel][pixel_value] - std::log(1/exposure));
                    irrad_value[channel] += weights * (gs[channel][pixel_value] - std::log(1/exposure));
                    //weights_sum[channel] += weights[channel];
                }

                weights_sum += weights;
            }

            for(int channel = 0 ; channel != 3 ; channel++) {
                hdr_img.at<cv::Vec3f>(i, j)[channel] = std::exp(irrad_value[channel] / weights_sum);
                //hdr_img.at<cv::Vec3f>(i, j)[channel] = std::exp(irrad_value[channel] / weights_sum[channel]);
            }

        }
    }
//    hdr_img = hdr_img + cv::Scalar(-1,-1,-1);
    return hdr_img;
}

cv::Mat recover(std::tuple<std::vector<std::tuple<cv::Mat, double>>, int, int> imgs) {
    std::cout << "getting the radiance maps from gslove." << std::endl;
    std::vector<arma::vec> gs = gsolve(imgs);
    cv::Mat HDR_img = buildHDR(std::get<0>(imgs), gs);
    return HDR_img;
}
