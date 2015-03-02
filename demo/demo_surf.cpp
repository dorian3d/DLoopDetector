/**
 * File: demo_surf.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: demo application of DLoopDetector
 * License: see the LICENSE.txt file
 */

#include <iostream>
#include <vector>
#include <string>

// DLoopDetector and DBoW2
#include <DBoW2/DBoW2.h> // defines Surf64Vocabulary
#include "DLoopDetector.h" // defines Surf64LoopDetector
#include <DUtilsCV/DUtilsCV.h> // defines macros CVXX

// OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
#if CV24
#include <opencv2/nonfree/features2d.hpp>
#endif

// Demo
#include "demoDetector.h"

using namespace DLoopDetector;
using namespace DBoW2;
using namespace std;

// ----------------------------------------------------------------------------

static const char *VOC_FILE = "./resources/surf64_k10L6.voc.gz";
static const char *IMAGE_DIR = "./resources/images";
static const char *POSE_FILE = "./resources/pose.txt";
static const int IMAGE_W = 640; // image size
static const int IMAGE_H = 480;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/// This functor extracts SURF64 descriptors in the required format
class SurfExtractor: public FeatureExtractor<FSurf64::TDescriptor>
{
public:
  /** 
   * Extracts features from an image
   * @param im image
   * @param keys keypoints extracted
   * @param descriptors descriptors extracted
   */
  virtual void operator()(const cv::Mat &im, 
    vector<cv::KeyPoint> &keys, vector<vector<float> > &descriptors) const;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int main()
{
  // prepares the demo
  demoDetector<Surf64Vocabulary, Surf64LoopDetector, FSurf64::TDescriptor> 
    demo(VOC_FILE, IMAGE_DIR, POSE_FILE, IMAGE_W, IMAGE_H);

  try 
  {  
    // run the demo with the given functor to extract features
    SurfExtractor extractor;
    demo.run("SURF64", extractor);
  }
  catch(const std::string &ex)
  {
    cout << "Error: " << ex << endl;
  }

  return 0;
}

// ----------------------------------------------------------------------------

void SurfExtractor::operator() (const cv::Mat &im, 
  vector<cv::KeyPoint> &keys, vector<vector<float> > &descriptors) const
{
  // extract surfs with opencv
  static cv::SURF surf_detector(400);
  
  surf_detector.extended = 0;
  
  keys.clear(); // opencv 2.4 does not clear the vector
  vector<float> plain;
  surf_detector(im, cv::Mat(), keys, plain);
  
  // change descriptor format
  const int L = surf_detector.descriptorSize();
  descriptors.resize(plain.size() / L);

  unsigned int j = 0;
  for(unsigned int i = 0; i < plain.size(); i += L, ++j)
  {
    descriptors[j].resize(L);
    std::copy(plain.begin() + i, plain.begin() + i + L, descriptors[j].begin());
  }
}

// ----------------------------------------------------------------------------

