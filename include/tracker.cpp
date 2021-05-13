#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <tracker.h>

//constructors

  tracker::tracker(int maxFeatures, int minMatchCount) {
    MAX_FEATURES = maxFeatures;
    MIN_MATCH_COUNT = minMatchCount;
    sift = cv::SIFT::create();
    matcher = cv::BFMatcher::create();
  }


  // fucntions

  void tracker::addSource(cv::Mat src) {
    colorImg = src.clone();
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    sourceImg = src.clone();
  }

  void tracker::addObj(cv::Mat srcObj) {
    cv::cvtColor(srcObj, srcObj, cv::COLOR_BGR2GRAY);
    objects.push_back(srcObj);
  }

  void tracker::addObj(std::vector<cv::Mat> srcObj) {
    cv::Mat temp;
    for(size_t i(0); i<srcObj.size(); i++) {
      cv::cvtColor(srcObj[i], temp, cv::COLOR_BGR2GRAY);
      objects.push_back(temp);
    }
  }


  void tracker::changeSource(cv::Mat src) {
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
        std::cout << "No objects to detect found!" << std::endl;
    }
  }

  void tracker::detectObject(cv::Mat& obj, std::vector<cv::KeyPoint>& kPts, cv::Mat& descriptors) {
    sift -> detectAndCompute(obj, cv::Mat(), kPts, descriptors);
    objects.push_back(obj);
    keypointsObjs.push_back(kPts);
    descriptorsObjs.push_back(descriptors);
  }

  void tracker::matchAllObjects() {

    const float ratio_thresh = .8f;
    descriptorsSrc.convertTo(descriptorsSrc, CV_32F);
    std::vector<std::vector<cv::DMatch>> matchesTemp;
    std::vector<cv::DMatch> goodTemp;

    for(size_t i(0); i<objects.size(); i++) {
      goodTemp.clear();
      matchesTemp.clear();

      descriptorsObjs[i].convertTo(descriptorsObjs[i], CV_32F);
      matcher->knnMatch(descriptorsObjs[i], descriptorsSrc, matchesTemp, 2);

      // applying the Lowe's ratio test to find good mathes
      for(size_t j(0); j<matchesTemp.size(); j++) {

        if(matchesTemp[j][0].distance < ratio_thresh*matchesTemp[j][1].distance) {
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
      for(size_t j(0); j<matches[i].size(); j++) {

        srcTemp.push_back(keypointsSrc[matches[i][j].trainIdx].pt);
        objTemp.push_back(keypointsObjs[i][matches[i][j].queryIdx].pt);
      }
      // calculating the homography and getting the mask for inliers
      std::vector<char> inliersTemp(objTemp.size(), 0);
      cv::Mat homography = findHomography(objTemp, srcTemp, inliersTemp, cv::RANSAC, 3);

      std::cout << "Total features matched: " << srcTemp.size() << std::endl;
      // discarding outliers
      for (int i = 0; i < inliersTemp.size(); i++) {


        if((int) inliersTemp[i] == 0) {
          objTemp.erase(objTemp.begin()+i);
          srcTemp.erase(srcTemp.begin()+i);
        }
      }

      std::cout << "Total features without outliers matched: " << srcTemp.size() << std::endl;


      inliers.push_back(inliersTemp);

      objPts.push_back(objTemp);
      srcPts.push_back(srcTemp);

      H.push_back(homography);
    }
    // get the keypoints from the good matches
    cv::Mat matchedImg;
    for(size_t i(0); i<objects.size(); i++) {
      std::cout << "keypointsSource: " << keypointsSrc.size() << std::endl;
      std::cout << "keypointsObject: " << keypointsObjs[i].size() << std::endl;
      std::cout << "MatchesSize: " << matches[i].size() << std::endl;
      drawMatches(objects[i], keypointsObjs[i], sourceImg, keypointsSrc, matches[i], matchedImg, cv::Scalar((i%2+1)*255, (i%3)*255, (i/2)*255), cv::Scalar::all(-1), inliers[i]);
      imshow("match", matchedImg);
      cv::waitKey();
    }
  }


    void tracker::drawContours(cv::Mat& dst) {

      dst = colorImg.clone();
      for (size_t i(0); i<objects.size(); i++) {

        std::vector<cv::Point2f> objCorner(4);
        objCorner[0] = cv::Point2f(0,0);
        objCorner[1] = cv::Point2f((float) objects[i].cols, 0);
        objCorner[2] = cv::Point2f((float) objects[i].cols, (float) objects[i].rows);
        objCorner[3] = cv::Point2f(0, (float) objects[i].rows);

        std::vector<cv::Point2f> frameCorners(4);
        cv::perspectiveTransform(objCorner, frameCorners, H[i]);

        cv::line(dst, frameCorners[0], frameCorners[1], cv::Scalar((i%2+1)*255, (i%3)*255, (i/2)*255),2);
        cv::line(dst, frameCorners[1], frameCorners[2], cv::Scalar((i%2+1)*255, (i%3)*255, (i/2)*255),2);
        cv::line(dst, frameCorners[2], frameCorners[3], cv::Scalar((i%2+1)*255, (i%3)*255, (i/2)*255),2);
        cv::line(dst, frameCorners[3], frameCorners[0], cv::Scalar((i%2+1)*255, (i%3)*255, (i/2)*255),2);
      }
      cv::imshow("test", dst);
      cv::waitKey();
    }

    void tracker::trackFlow(cv::Mat prevImg, cv::Mat nextImg, std::vector<std::vector<cv::Point2f>> prevPts, std::vector<std::vector<cv::Point2f>>& nextPts, std::vector<std::vector<uchar>>& status) {

      if(prevPts.empty()) {
        prevPts = srcPts;
      }

      std::vector<cv::Point2f> nextTemp;
      std::vector<uchar> statusTemp;
      cv::Mat err;

      for (size_t i = 0; i < prevPts.size(); i++) {
        cv::calcOpticalFlowPyrLK(prevImg, nextImg, prevPts[i], nextTemp, statusTemp, err);

        std::cout << "Points tracked -> " << nextTemp.size() << '\n';

        for (size_t j = 0; j < nextTemp.size(); j++) {
          if (((int) statusTemp[j])==0) {
            nextTemp.erase(nextTemp.begin()+i);
          }
        }
        std::cout << "Total points preserved for object " << i << ": " << cv::countNonZero(statusTemp) << '\n';//nextTemp.size()
        nextPts.push_back(nextTemp);
      }
    }


























    //--------------------------------------------
