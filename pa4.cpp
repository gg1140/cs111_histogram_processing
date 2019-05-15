#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * All functions are written for single channel grayscale images
 */

/*
 * Input: Mat object from opencv lib
 * Notes:
 *  - uchar/char will not be printing numbers but as ASCII characters
 */
template <typename T>
void printMat(Mat& img) {
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            cout << "|" << img.at<T>(i, j);
        }
        cout << endl;
    }
}

/*
 * input: gray images
 * output: histogram in pdf
 */
Mat FindPDF(Mat I){

    Mat pdf = Mat::zeros(256, 1, CV_32FC1);

    for (int i = 0; i < I.rows; i++){
        for (int j = 0; j < I.cols; j++){

            int intensity = (int) I.at<uchar>(i, j);
            pdf.at<float>(intensity, 0) += 1.0;
        }
    }

    for (int i = 0; i < pdf.rows; i++) {

        pdf.at<float>(i, 0) /= I.total();;
    }

    return pdf;
}

/*
 * input: probability density function
 * output: cumulative density function
 */
Mat FindCDF(Mat pdf){

    Mat cdf(256, 1, CV_32FC1);
    float cumulative_sum = 0.0;

    for (int i = 0; i < pdf.total(); i++){

        cumulative_sum += pdf.at<float>(i, 0);
        cdf.at<float>(i, 0) = cumulative_sum;
    }

    return cdf;
}

/*
 * input: cumulative density funtion of input images
 * output: mapping array for linear distribution
 */
Mat FindEqualMapping(Mat cdf){

    Mat target(256, 1, CV_32FC1);

    for (int i = 0; i < 256; i++){
        // Fill in target cdf
        target.at<float>(i, 0) = ((float)i + 1) / target.total();
    }

    Mat mapping(256, 1, CV_8UC1);

    for (int i = 0; i < 256; i++){
        // Find closet target[j] to cdf[i]
        int map_val = 0;
        float closest_diff = 2.0;

        for (int j = 0; j < 256; j++){

            float diff = cdf.at<float>(i, 0) - target.at<float>(j, 0);
            float abs_diff = abs(diff);

            if (abs_diff < closest_diff) {
                map_val = j;
                closest_diff = abs_diff;
            }
            else if (abs_diff > closest_diff) {
                break;
            }
        }
        // mapping[i] = j
        mapping.at<uchar>(i, 0) = (uchar)map_val;
    }

    return mapping;
}

/*
 * input: soure image, mapping array
 * output: new image with remapped values
 */
Mat ApplyEqualization(Mat I, Mat mapping){

    Mat Output(I.rows, I.cols, CV_8UC1);

    for (int i = 0; i < I.rows; i++){
        for (int j = 0; j < I.cols; j++){
            // Set the output[i,j]
            int intensity = (int) I.at<uchar>(i, j);
            Output.at<uchar>(i, j) = mapping.at<uchar>(intensity, 0);
        }
    }

    return Output;
}

/*
 * Input: source image
 * Output: new image after histogram equalization
 */
Mat HistogramEqualization(Mat I) {

    Mat currPDF = FindPDF(I);
    Mat currCDF = FindCDF(currPDF);
    Mat eqMap = FindEqualMapping(currCDF);
    Mat output = ApplyEqualization(I, eqMap);

    return output;
}

Mat Resize(Mat I, float s){

    int orig_x = I.cols;
    int orig_y = I.rows;

    int tar_x = orig_x * s;
    int tar_y = orig_y * s;

    // Query points
    Mat X(tar_y, tar_x, CV_32FC1);
    Mat Y(tar_y, tar_x, CV_32FC1);

    // Setting the query points
    for (int i = 0; i < tar_y; i++){

        for (int j = 0; j < tar_x; j++){

            // Set X[i,j] and Y[i,j]
        }
    }


    // Output image
    Mat Output(tar_y, tar_x, CV_8UC1);

    // Performing the interpolation
    for (int i = 0; i < tar_y; i++){

        for (int j = 0; j < tar_x; j++){

            // Set Output[i,j] using X[i,j] and Y[i,j]
        }
    }

    return Output;
}

int main(int argc, char** argv) {

    Mat img = imread(argv[1], IMREAD_GRAYSCALE);
    Mat new_img = HistogramEqualization(img);
    imwrite(argv[2], new_img);

    return 0;
}
