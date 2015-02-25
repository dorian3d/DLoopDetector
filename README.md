DLoopDetector
=============

##Overview

DLoopDetector is an open source C++ library to detect loops in a sequence of images collected by a mobile robot. It implements the algorithm presented in GalvezTRO12, based on a bag-of-words database created from image local descriptors, and temporal and geometrical constraints. The current implementation includes versions to work with SURF64 and BRIEF descriptors. DLoopDetector is based on the DBoW2 library, so that it can work with any other type of descriptor with little effort.

DLoopDetector requires OpenCV and the `boost::dynamic_bitset` class in order to use the BRIEF version.

DLoopDetector has been tested on several real datasets, yielding an execution time of ~9 ms to detect a loop a in a sequence with more than 19000 images (without considering the feature extraction). When BRIEF descriptors were used, the feature extraction and the loop detection were performed in 16 ms on average.

##Citing

If you use this software in an academic work, please cite:

    @ARTICLE{GalvezTRO12,
      author={G\'alvez-L\'opez, Dorian and Tard\'os, J. D.},
      journal={IEEE Transactions on Robotics},
      title={Bags of Binary Words for Fast Place Recognition in Image Sequences},
      year={2012},
      month={October},
      volume={28},
      number={5},
      pages={1188--1197},
      doi={10.1109/TRO.2012.2197158},
      ISSN={1552-3098}
    }

## Install and usage notes

DLoopDetector requires OpenCV and the `boost::dynamic_bitset` class in order to use the BRIEF version. You can install Boost by typing:

    $ sudo apt-get install libboost-dev


To check how to use DLoopDetector, compile the demo applications. This demo includes the DLoopDetector classes, bag-of-words vocabularies with 10^6 words to use with SURF64 or BRIEF features, a small collection of images of the Bicocca 2009-02-25b dataset of the Rawseeds FP-6 project and a demo application to find loops in these images.

Note that the demo applications require some external resources that are downloaded automatically when they are activated in CMake. The resource file is 374MB so it may take a while to download.

You can run either demo_surf or demo_brief. When the demo is running, you should see a window with the current image, a window with the trajectory of the robot and the status of the detection process in the console:
![Loop detector](http://dorian3d.github.com/other/images/dloop.png)

The main functionality of the demo is written in the `demoDetector.h` class. Check it to see how to change the parameters of the loop detector.
