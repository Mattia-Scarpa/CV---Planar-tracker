#include <tracker.h>

//constructors

  tracker::tracker(int maxFreatures, int minMatchCount) {
    MAX_FEATURES = maxFreatures;
    MIN_MATCH_COUNT = minMatchCount;
    sift = cv::SIFT::create(MAX_FEATURES);
    matcher = cv::BFMatcher::create();
  }


  // fucntions

  void tracker::addSource(cv::Mat& src) {
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    sourceImg = src.clone();
  }

  void tracker::addObj(cv::Mat& srcObj) {
    objects.push_back(srcObj);
  }

  void tracker::addObj(std::vector<cv::Mat>& srcObj) {
    for(size_t i(0); i<srcObj.size(); i++) {
      objects.push_back(srcObj[i]);
    }
  }


  void tracker::changeSource(cv::Mat& src) {
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    sourceImg = src.clone();
  }

  void tracker::detectSource(std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors) {
    sift -> detectAndCompute(sourceImg, cv::Mat(), keypointsSrc, descriptorsSrc);
    kPts = keypointsSrc;
    descriptors = descriptorsSrc;
  }

  void tracker::detectAllObjects(std::vector<std::vector<cv::KeyPoint>>& kPts, std::vector<cv::Mat>& descriptors) {
    if (!objects.size() <= 0) {
      for (size_t i(0); i < objects.size(); i++) {
        std::vector<cv::KeyPoint> keypointsTemp;
        cv::Mat descriptorTemp;
        sift -> detectAndCompute(objects[i], cv::Mat(), keypointsTemp, descriptorTemp);
        keypointsObjs.push_back(keypointsTemp);
        descriptorsObjs.push_back(descriptorTemp);
      }
    }
    else {
        std::cout << "No objects to detect features found!" << std::endl;
    }
  }

  void tracker::matchAllObjects() {

  }


    void tracker::detectObject(cv::Mat& obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors) {
      sift -> detectAndCompute(obj, cv::Mat(), kPts, descriptors);
      objects.push_back(obj);
      keypointsObjs.push_back(kPts);
      descriptorsObjs.push_back(descriptors);
    }
