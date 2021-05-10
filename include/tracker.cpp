#include <tracker.h>

//constructors

  tracker::tracker(int maxFreatures, int minMatchCount) {
    MAX_FEATURES = maxFreatures;
    MIN_MATCH_COUNT = minMatchCount;
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create(MAX_FEATURES);
  }
