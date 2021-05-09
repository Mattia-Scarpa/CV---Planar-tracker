#include <iostream>

using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;


int MAX_FEATURES = 1000;
int MIN_MATCH_COUNT = 10;

/* void detectObj(): takes an image src where to detect the objects contained in obj vectos. Bot src either objs vector must be in BGR color space */

void detectObj(Mat& src, vector<Mat>& objs) {

  // Convert given image from BGR color space to gray level
  Mat sourceImg = src.clone();
  
  cvtColor(sourceImg, sourceImg, COLOR_BGR2GRAY);

  // creating the SIFT objects
  Ptr<SIFT> sift = SIFT::create(MAX_FEATURES);

  // detecting keypoints for source image
  vector<KeyPoint> keypointsSrc;
  Mat descriptorsSrc;
  sift -> detectAndCompute(sourceImg, Mat(), keypointsSrc, descriptorsSrc);
  cout << keypointsSrc[0].pt << endl;

  vector<vector<KeyPoint>> keypointsObjs;
  vector<Mat> descriptorsObjs;
  for (size_t i(0); i < objs.size(); i++) {

  }

}



int main(int argc, char const *argv[]) {

  // Defining Image path
  vector<string> dataPaths {"../data/objects/*.jpg", "../data/objects/*.png"};
  vector<string> imagesPath;
  vector<string> path_temp;
  for (size_t  i(0); i < dataPaths.size(); ++i) {
    glob(dataPaths[i], path_temp);
    for (size_t j(0); j < path_temp.size(); ++j) {
            imagesPath.push_back(path_temp[j]);
    }
  }

  if (imagesPath.size() < 1) {
    cout << "No compatible images found in folder ../data/ \nAdd image in the specified folder then rerun the application (available format: .jpg, .png)" << endl;
    return 0;
  }

  cout << "A total of " << imagesPath.size() << " objects images to detect has been found!" << endl;

  for (size_t i(0); i < imagesPath.size(); i++) {
    cout << i << ": " << imagesPath[i].substr(7) << endl;
  }


  vector<string> videosPath;
  dataPaths = {"../data/*.avi", "../data/*.mov", "../data/*.mp4"};
  path_temp.clear();

  for (size_t  i(0); i < dataPaths.size(); ++i) {
    glob(dataPaths[i], path_temp);
    for (size_t j(0); j < path_temp.size(); ++j) {
            videosPath.push_back(path_temp[j]);
    }
  }

  cout << "A total of " << videosPath.size() << " objects images to detect has been found!" << endl;

  vector<Mat> objects;
  for (size_t i(0); i < videosPath.size(); i++) {
    cout << i << ": " << videosPath[i].substr(7) << endl;
    objects.push_back(imread(videosPath[i]));

  }


  cv::VideoCapture cap(videosPath[0]);
  bool detected = true; // chek if objects has been detected from the first frame

  if(cap.isOpened()) { // check if we succeeded
    for(;;) {
      Mat frame;
      cap >> frame;
      if (!frame.empty()) {
        imshow("win", frame);
      }
      waitKey(1);

      //cout << "I am doing something" << endl;
      if (detected) {
        detectObj(frame, objects);
        detected = false;
        // detecting object given in image objects
      }
    }
  }
  else {
    cout << "No video of the correct format found" << endl;
  }

  return 0;
}
