#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <unistd.h>

using namespace std::chrono_literals;


/*
class OdomSubscriber : public rclcpp::Node {
public:
  OdomSubscriber(std::string odom_topic_name) : Node("odom_subscriber") {

    subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
        odom_topic_name, 10,
        std::bind(&OdomSubscriber::topic_callback, this,
                  std::placeholders::_1));
  }

private:
  void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Odometry=['%f','%f','%f']",
                msg->pose.pose.position.x, msg->pose.pose.position.y,
                msg->pose.pose.position.z);
  }
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};
*/


class SlowTimer : public rclcpp::Node {
public:
  SlowTimer(float sleep_timer) : Node("slow_timer_subscriber") {
    this->wait_time = sleep_timer;
    timerSlow_ = this->create_wall_timer(
        500ms, std::bind(&SlowTimer::timerSlow_callback, this));
  }

private:
  void timerSlow_callback() {
    sleep(this->wait_time);
    RCLCPP_INFO(this->get_logger(), "TICK SLOW");
  }
  rclcpp::TimerBase::SharedPtr timerSlow_;
  float wait_time;
};

class FastTimer : public rclcpp::Node {
public:
  FastTimer(float sleep_timer_fast) : Node("fast_timer_subscriber") {
    this->wait_time_fast = sleep_timer_fast;
    timerFast_ = this->create_wall_timer(
        100ms, std::bind(&FastTimer::timerFast_callback, this));
  }

private:
  void timerFast_callback() {
    sleep(this->wait_time_fast);
    RCLCPP_INFO(this->get_logger(), "TICK FAST");
  }
  rclcpp::TimerBase::SharedPtr timerFast_;
  float wait_time_fast;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);

   /* Instantiate the odometry subscriber node
  std::shared_ptr<OdomSubscriber> odom_subs_node =
      std::make_shared<OdomSubscriber>("/box_bot_1/odom");
   */
  // Instantiate the timer  node node
  float sleep_time = 3.0;
  std::shared_ptr<SlowTimer> slow_timer_node =
      std::make_shared<SlowTimer>(sleep_time);

    // Instantiate the timer  node node
  float sleep_time_fast = 1.0;
  std::shared_ptr<FastTimer> fast_timer_node =
      std::make_shared<FastTimer>(sleep_time_fast);

  //RCLCPP_INFO(odom_subs_node->get_logger(), "odom_subs_node INFO...");
  RCLCPP_INFO(slow_timer_node->get_logger(), "slow_timer_node INFO...");
  RCLCPP_INFO(fast_timer_node->get_logger(), "fast_timer_node INFO...");

  rclcpp::executors::MultiThreadedExecutor executor;
    
  // Add the odometry subscriber node to the executor
  executor.add_node(fast_timer_node);
  // Add the timer node to the executor
  executor.add_node(slow_timer_node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}

