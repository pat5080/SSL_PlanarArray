#include "retrievedata.h"


RetrieveData::RetrieveData(ros::NodeHandle nh)
    : nh_(nh), it_(nh)
{
    sub1_ = nh_.subscribe("/HarkSource", 1000, &RetrieveData::coorCallback, this); //!< ROS subscriber to HarkSource

    //ros::NodeHandle pn("~");

    count_ = 0;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "hark");

    ros::NodeHandle nh;

    std::shared_ptr<RetrieveData> gc(new RetrieveData(nh));
    std::thread t(&RetrieveData::separateThread, gc);

    ros::spin();

    ros::shutdown();
    t.join();

    return 0;

}

void RetrieveData::coorCallback(const hark_msgs::HarkSource::ConstPtr& msg) //!< Callback function for HarkSource
{
      //if(enable_debug)
      //ROS_INFO_STREAM("HarkSource received [" << msg->count << "] [thread=" << boost::this_thread::get_id() << "]");
      hark_msgs::HarkSource value_harksource;
      value_harksource.header.frame_id = msg->header.frame_id;
      value_harksource.header.stamp = msg->header.stamp;
      value_harksource.count = msg->count;
      value_harksource.exist_src_num = msg->exist_src_num;
      value_harksource.src.resize(0);
      for(int i = 0; i < msg->exist_src_num; i++){
        hark_msgs::HarkSourceVal HarkSourceValMsg;
        HarkSourceValMsg.id    = msg->src[i].id;
        HarkSourceValMsg.power = msg->src[i].power;
        HarkSourceValMsg.x     = msg->src[i].x;
        HarkSourceValMsg.y     = msg->src[i].y;
        HarkSourceValMsg.z     = msg->src[i].z;
        HarkSourceValMsg.azimuth   = msg->src[i].azimuth;
        HarkSourceValMsg.elevation = msg->src[i].elevation;
        value_harksource.src.push_back(HarkSourceValMsg);
      }

      static ros::Time latest_timestamp = value_harksource.header.stamp;
      ros::Time timestamp = value_harksource.header.stamp;

      if(latest_timestamp != timestamp)
      {
          buffer.buffer_mutex_.lock();
          buffer.deque_harksource.push_back(value_harksource);

          if(buffer.deque_harksource.size() > 1)
          {
            buffer.deque_harksource.pop_front();
          }
          buffer.buffer_mutex_.unlock();
      }
}

void RetrieveData::separateThread(){
    //The below loop runs until ros is shutdown, to ensure this thread does not remain
    // a zombie thread
    //The loop locks the buffer, checks the size
    //And then pulls items: the pose and timer_t
    // You can contemplate whether these could have been combined into one

    ros::Rate rate_limiter(0.1);

    double azimuth;
    double elevation;
    //double x;
    //double y;
    //double z;
    ros::Time timeCoor = ros::Time::now();;


    while (ros::ok()) {


        int deqSz = -1;

        buffer.buffer_mutex_.lock();
        deqSz = buffer.deque_harksource.size();
        if (deqSz > 0){
            hark_msgs::HarkSource value_HarkSource = buffer.deque_harksource.front();

            hark_msgs::HarkSourceVal HarkSourceValMsg = value_HarkSource.src.front();
            azimuth = HarkSourceValMsg.azimuth;
            elevation = HarkSourceValMsg.elevation;
            timeCoor = value_HarkSource.header.stamp;

            buffer.deque_harksource.pop_front();
        }
        buffer.buffer_mutex_.unlock();

        std::cout << timeCoor << " Azimuth: " << azimuth << " Elevation: " << elevation << std::endl;
        //std::cout << "Translation " << " x: " << x << " y: " << y << " z: " << z << " Yaw: " << yaw << std::endl;

        rate_limiter.sleep();
        }
}
