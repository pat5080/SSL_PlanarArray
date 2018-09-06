/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2010, Kyoto University and Honda Motor Co.,Ltd. All rights reserved.
 *
 * HARK was developed by researchers in Okuno Laboratory at the Kyoto University and
 * Honda Research Institute Japan Co.,Ltd.
 */

#ifndef RETRIEVEDATA_H
#define RETRIEVEDATA_H

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "tf/transform_datatypes.h"
#include "hark_msgs/HarkSource.h"
#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include <tf/transform_listener.h>


#include "hark_msgs/HarkFeature.h"
#include "hark_msgs/HarkFeatureVal.h"
#include "hark_msgs/HarkSourceVal.h"
#include "hark_msgs/HarkSrcFeature.h"
#include "hark_msgs/HarkSrcFeatureVal.h"
//#include "HarkRosGlobals.h" --> No such directory error
//#include "RecogSource.h" --> No such directory error
#include <math.h>

#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/image_encodings.h"
#include "nav_msgs/Odometry.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "geometry_msgs/PoseArray.h"
#include <visualization_msgs/Marker.h>

#include <sstream>
#include <iostream>
#include <string>

#include <thread>
#include <chrono>
#include <deque>
#include <mutex>
#include <random>
#include <vector>

using namespace std;


class RetrieveData
{
public:
    RetrieveData(ros::NodeHandle nh); //!< Constructor passing in node handle
    ~RetrieveData()
    {
    }

    void separateThread();  //!< Thread containing the ros::ok (main loop of program)

    void coorCallback(const hark_msgs::HarkSource::ConstPtr& msg); //!< HarkSource callback function - pushes azimuth & elevation data into a deque.


private:


    ros::NodeHandle nh_;    //!< Node handle
    ros::Subscriber sub1_;  //!< Subscriber 1

    int count_; //!< Counter for node execution iterations
    //int data_buffer_num = 1000;

    struct DataBuffer
    {
        std::deque<hark_msgs::HarkSource> deque_harksource;
        //std::deque<ros::Time> timeStampDeq;
        std::mutex buffer_mutex_;   //!< Mutex will be used to lock and unlock data members of struct
    };
    DataBuffer buffer; //!< DataBuffer object
};

#endif // RETRIEVEDATA_H
