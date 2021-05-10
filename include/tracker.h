#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>


class tracker{

public:

  // constructors

  tracker(int maxFreatures, int minMatchCount);


  // public fucntions

  void addSource(cv::Mat& src);

  void addObj(cv::Mat& srcObj);

  void addObj(std::vector<cv::Mat>& srcObj);


  void detectSource();

  void detectObject(cv::Mat& Obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);

  void matchObject(std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);


  void detectAllObjects(std::vector<std::vector<cv::KeyPoint>>& kPts, std::vector<cv::Mat>& descriptors);

  void matchAllObject(std::vector<std::vector<cv::KeyPoint>>& kPts, std::vector<cv::Mat>& descriptors);



private:

  void clearSource();

  int MAX_FEATURES;
  int MIN_MATCH_COUNT;
  cv::Ptr<cv::SIFT> sift;

  cv::Mat sourceImg;
  std::vector<cv::KeyPoint> keypointsSrc;
  cv::Mat descriptorsSrc;

  std::vector<cv::Mat> objs;
  std::vector<std::vector<cv::KeyPoint>> keypointsObjs;
  std::vector<cv::Mat> descriptorsObjs;

};
