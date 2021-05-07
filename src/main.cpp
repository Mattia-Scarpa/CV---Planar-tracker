#include <iostream>

using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;



void detectObj() {
  ;
}



int main(int argc, char const *argv[]) {

  // Defining Image path
  vector<string> dataPaths {"../data/objects/*.jpg", "../data/objects/*.png"};
  vector<string> imagesPath;
  vector<string> imagesPath_temp;
  for (size_t  i(0); i < dataPaths.size(); ++i) {
    glob(dataPaths[i], imagesPath_temp);
    for (size_t j(0); j < imagesPath_temp.size(); ++j) {
            imagesPath.push_back(imagesPath_temp[j]);
    }
  }

  if (imagesPath.size() < 1) {
    cout << "No compatible images found in folder ../data/ \nAdd image in the specified folder then rerun the application (available format: .jpg, .png)" << endl;
    return 0;
  }

  cout << "A total of " << imagesPath.size() << "images objects to detect has been found!" << endl;

  /*for (size_t i(0); i < imagesPath.size(); i++) {
    cout << i << ": " << imagesPath[i].substr(7) << endl;
  }*/

  dataPaths = {"../data/*.avi", "../data/*.mov", "../data/*.mp4"}

  cv::VideoCapture cap("video.avi");
  bool detected = true; // chek if objects has been detected from the first frame

  if(cap.isOpened()) { // check if we succeeded
    for(;;) {
      Mat frame;
      cap >> frame;

      cout << "I am doing something" << endl;
      if (detected) {
        detectObj();
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
