#include <opencv2/core.hpp>
class tracker{

public:

  // constructors

  tracker(int maxFreatures);


  // public fucntions

  void addSource(cv::Mat src);

  void addObj(cv::Mat srcObj);

  void addObj(std::vector<cv::Mat> srcObj);


  void changeSource(cv::Mat src);

  void detectSource(std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);

  void detectAllObjects(std::vector<std::vector<cv::KeyPoint>>& kPts, std::vector<cv::Mat>& descriptors);

  void matchAllObjects();


  void detectObject(cv::Mat& Obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors);

  void drawContours(cv::Mat& dst);

  void drawContours(cv::Mat& dst, std::vector<std::vector<cv::Point2f>> cornerPoints);


  void trackFlow(cv::Mat prevImg, cv::Mat nextImg, std::vector<std::vector<cv::Point2f>>& prevPts, std::vector<std::vector<cv::Point2f>>& nextPts, std::vector<std::vector<uchar>>& status);

  void trackConotours(std::vector<std::vector<cv::Point2f>> prevPts, std::vector<std::vector<cv::Point2f>> nextPts, std::vector<std::vector<cv::Point2f>> prevCorners, std::vector<std::vector<cv::Point2f>>& newCorners);


  std::vector<cv::Scalar> colors;
  std::vector<cv::Mat> H;
  std::vector<std::vector<cv::Point2f>> frameCorners;

private:

  int MAX_FEATURES;
  cv::Ptr<cv::SIFT> sift;
  cv::Ptr<cv::BFMatcher> matcher;

  cv::Mat sourceImg;
  cv::Mat colorImg;
  std::vector<cv::KeyPoint> keypointsSrc;
  cv::Mat descriptorsSrc;

  std::vector<cv::Mat> objects;
  std::vector<std::vector<cv::KeyPoint>> keypointsObjs;
  std::vector<cv::Mat> descriptorsObjs;

  std::vector<std::vector<cv::DMatch>> matches;

  std::vector<std::vector<cv::Point2f>> objPts;
  std::vector<std::vector<cv::Point2f>> srcPts;
  std::vector<std::vector<char>> inliers;



};
