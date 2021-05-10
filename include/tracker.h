#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>


class tracker{

public:

  // constructors

  tracker(int maxFreatures, int minMatchCount);


  // public fucntions

  void addSource(cv::Mat& src);

  void addObj(cv::Mat& srcObj);

  void addObj(std::vector<cv::Mat>& srcObj);


  void changeSource(cv::Mat& src);

  void detectSource(std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);

  void detectAllObjects(std::vector<std::vector<cv::KeyPoint>>& kPts, std::vector<cv::Mat>& descriptors);

  void matchAllObjects();


  void detectObject(cv::Mat& Obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);



private:

  int MAX_FEATURES;
  int MIN_MATCH_COUNT;
  cv::Ptr<cv::SIFT> sift;
  cv::Ptr<cv::BFMatcher> matcher;

  cv::Mat sourceImg;
  std::vector<cv::KeyPoint> keypointsSrc;
  cv::Mat descriptorsSrc;

  std::vector<cv::Mat> objects;
  std::vector<std::vector<cv::KeyPoint>> keypointsObjs;
  std::vector<cv::Mat> descriptorsObjs;


};
