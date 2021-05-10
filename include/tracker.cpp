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

    const float ratio_thresh = 0.9f;
    descriptorsSrc.convertTo(descriptorsSrc, CV_32F);
    std::vector<std::vector<cv::DMatch>> matchesTemp;
    std::vector<cv::DMatch> goodTemp;

    for(size_t i(0); i<objects.size(); i++) {
      goodTemp.clear();

      descriptorsObjs[i].convertTo(descriptorsObjs[i], CV_32F);
      matcher->knnMatch(descriptorsObjs[i], descriptorsSrc, matchesTemp, 2);

      // applying the Lowe's ratio test to find good mathes
      for(size_t j(0); j<matchesTemp.size(); j++) {
        if(matchesTemp[j][0].distance < ratio_thresh*matchesTemp[j][0].distance) {
          goodTemp.push_back(matchesTemp[j][0]);
        }
      }
      matches.push_back(goodTemp);
    }

    // ensuring all vector points are cleared
    objPts.clear();
    srcPts.clear();

    std::vector<cv::Point2f> objTemp;
    std::vector<cv::Point2f> srcTemp;
    for(size_t i(0); i<objects.size(); i++) {
      objTemp.clear();
      srcTemp.clear();
      for(size_t j(0); j<matches.size(); j++) {
        srcTemp.push_back(keypointsSrc[matches[i][j].trainIdx].pt);
        objTemp.push_back(keypointsObjs[i][matches[i][j].queryIdx].pt);
      }
      objPts.push_back(objTemp);
      srcPts.push_back(srcTemp);
    }



    // get the keypoints from the good matches
    std::string wName = "match ";
    cv::Mat matchedImg;
    for(size_t i(0); i<objects.size(); i++) {
      drawMatches(objects[i], keypointsObjs[i], sourceImg, keypointsSrc, matches[i], matchedImg, Scalar(0, 255, 0));
      imshow(wName+i, matchedImg);
    }

  }


    void tracker::detectObject(cv::Mat& obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors) {
      sift -> detectAndCompute(obj, cv::Mat(), kPts, descriptors);
      objects.push_back(obj);
      keypointsObjs.push_back(kPts);
      descriptorsObjs.push_back(descriptors);
    }
