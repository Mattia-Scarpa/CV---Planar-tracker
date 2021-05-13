#include <iostream>

using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;

#include <tracker.h>


int MAX_FEATURES = 1000;
int MIN_MATCH_COUNT = 10;



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

  vector<Mat> objects;

  cout << "A total of " << imagesPath.size() << " objects images to detect has been found!" << endl;
  for (size_t i(0); i < imagesPath.size(); i++) {
    cout << i << ": " << imagesPath[i].substr(7) << endl;
    objects.push_back(imread(imagesPath[i]));
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


  for (size_t i(0); i < videosPath.size(); i++) {
    cout << i << ": " << videosPath[i].substr(7) << endl;
  }




  cv::VideoCapture cap(videosPath[0]);
  bool detected = true; // chek if objects has been detected from the first frame
  tracker* track = new tracker(MAX_FEATURES, MIN_MATCH_COUNT);
  char k;

  if(cap.isOpened()) { // check if we succeeded

    Mat prevFrame;
    vector<vector<Point2f>> prevPts, nextPts;
    vector<vector<uchar>> status;
    for(;;) {
      Mat frame;
      cap >> frame;
      if (frame.empty()) {
        //imshow("win", frame);
        break;
      }
      k = waitKey(1);



      if (detected) {
        detected = false;
        track->addSource(frame);
        track->addObj(objects);
        // detecting object given in image objects
        vector<KeyPoint> frameKeypoints;
        Mat frameDescriptors;
        vector<vector<KeyPoint>> objectsKeypoints;
        vector<Mat> objectsDescriptors;
        track->detectSource(frameKeypoints,frameDescriptors);
        track->detectAllObjects(objectsKeypoints,objectsDescriptors);
        track->matchAllObjects();

        Mat contours;
        track->drawContours(contours);
      }
      else {
        status.clear();
        //std::vector<cv::Point2f> prevPts, std::vector<cv::Point2f>& nextPts, std::vector<uchar>& status
        track->trackFlow(prevFrame, frame, prevPts, nextPts, status);

        //-------------------------------------------------------------
        vector<KeyPoint> kpts;
        for (size_t k = 0; k < nextPts.size(); k++) {
          for (int i = 0; i<nextPts[k].size(); i++){
            KeyPoint kpt = KeyPoint(nextPts[k][i], (float) 3);
            kpts.push_back(kpt);
          }
        }

         Mat test;
        drawKeypoints(frame, kpts, test);
        imshow("testdisperato", test);
        waitKey(1);
        prevPts = nextPts;
        nextPts.clear();
      }

      prevFrame = frame.clone();

      if(k!=-1) {
        break;
      }

    }
  }
  else {
    cout << "No video of the correct format found" << endl;
  }

  return 0;
}
