#include <chrono>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/float64.hpp"

using namespace std::chrono_literals;

struct TestPhase
{
  std::string name;
  double left_velocity;
  double right_velocity;
};

class RelbotTestNode : public rclcpp::Node
{
public:
  RelbotTestNode()
  : Node("relbottest_6_4_2")
  {
    declare_parameter<double>("setpoint_vel", 2.0);
    declare_parameter<double>("phase_duration", 4.0);
    declare_parameter<double>("publish_rate_hz", 10.0);
    declare_parameter<bool>("sequence_enabled", true);
    declare_parameter<std::string>("test_mode", "straight");

    get_parameter("setpoint_vel", setpoint_vel_);
    get_parameter("phase_duration", phase_duration_);
    get_parameter("publish_rate_hz", publish_rate_hz_);
    get_parameter("sequence_enabled", sequence_enabled_);
    get_parameter("test_mode", test_mode_);

    left_setpoint_pub_ = create_publisher<example_interfaces::msg::Float64>(
      "/input/left_motor/setpoint_vel", 10);
    right_setpoint_pub_ = create_publisher<example_interfaces::msg::Float64>(
      "/input/right_motor/setpoint_vel", 10);

    setup_phases();

    RCLCPP_INFO(get_logger(), "relbottest_6_4_2 node started");
    RCLCPP_INFO(get_logger(), "sequence_enabled=%s test_mode=%s setpoint_vel=%.3f phase_duration=%.3f publish_rate_hz=%.3f",
      sequence_enabled_ ? "true" : "false",
      test_mode_.c_str(),
      setpoint_vel_,
      phase_duration_,
      publish_rate_hz_);

    timer_ = create_wall_timer(
      std::chrono::duration<double>(1.0 / publish_rate_hz_),
      std::bind(&RelbotTestNode::timer_callback, this));

    start_time_ = now();
    last_phase_index_ = std::numeric_limits<size_t>::max();
  }

private:
  void setup_phases()
  {
    phases_ = {
      {"left_only", setpoint_vel_, 0.0},
      {"right_only", 0.0, setpoint_vel_},
      {"straight", setpoint_vel_, setpoint_vel_},
      {"turn_cw", setpoint_vel_, -setpoint_vel_},
      {"turn_ccw", -setpoint_vel_, setpoint_vel_},
      {"stop", 0.0, 0.0}
    };
  }

  const TestPhase & get_test_phase() const
  {
    static const TestPhase left_only{ "left_only", setpoint_vel_, 0.0 };
    static const TestPhase right_only{ "right_only", 0.0, setpoint_vel_ };
    static const TestPhase straight{ "straight", setpoint_vel_, setpoint_vel_ };
    static const TestPhase turn_cw{ "turn_cw", setpoint_vel_, -setpoint_vel_ };
    static const TestPhase turn_ccw{ "turn_ccw", -setpoint_vel_, setpoint_vel_ };
    static const TestPhase stop{ "stop", 0.0, 0.0 };

    if (test_mode_ == "left_only") {
      return left_only;
    }
    if (test_mode_ == "right_only") {
      return right_only;
    }
    if (test_mode_ == "turn_cw") {
      return turn_cw;
    }
    if (test_mode_ == "turn_ccw") {
      return turn_ccw;
    }
    if (test_mode_ == "stop") {
      return stop;
    }
    return straight;
  }

  void publish_setpoints(double left, double right)
  {
    example_interfaces::msg::Float64 left_msg;
    example_interfaces::msg::Float64 right_msg;
    left_msg.data = left;
    right_msg.data = right;
    left_setpoint_pub_->publish(left_msg);
    right_setpoint_pub_->publish(right_msg);
  }

  void timer_callback()
  {
    const TestPhase *current_phase = nullptr;

    if (sequence_enabled_) {
      const auto elapsed = (now() - start_time_).seconds();
      const size_t phase_index = std::min(static_cast<size_t>(elapsed / phase_duration_), phases_.size() - 1);
      current_phase = &phases_[phase_index];
      if (phase_index != last_phase_index_) {
        RCLCPP_INFO(get_logger(), "phase %zu/%zu: %s", phase_index + 1u, phases_.size(), current_phase->name.c_str());
        last_phase_index_ = phase_index;
      }
    } else {
      current_phase = &get_test_phase();
      if (last_phase_index_ == std::numeric_limits<size_t>::max()) {
        RCLCPP_INFO(get_logger(), "running fixed test_mode=%s", current_phase->name.c_str());
        last_phase_index_ = 0;
      }
    }

    publish_setpoints(current_phase->left_velocity, current_phase->right_velocity);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr left_setpoint_pub_;
  rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr right_setpoint_pub_;
  rclcpp::Time start_time_;
  std::vector<TestPhase> phases_;
  size_t last_phase_index_;

  double setpoint_vel_;
  double phase_duration_;
  double publish_rate_hz_;
  bool sequence_enabled_;
  std::string test_mode_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RelbotTestNode>());
  rclcpp::shutdown();
  return 0;
}
