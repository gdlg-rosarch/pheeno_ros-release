#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include "command_line_parser.h"
#include "sensor_listener.h"

double randomTurn()
{
  return rand() % 10 + 1 <= 5 ? -0.07 : 0.07;
}

void avoidObstacles(std::vector<double> values, double& linear, double& angular, double random_turn_value)
{
  double range_to_avoid = 20.0;
  if (values[0] < range_to_avoid)
  {
    if (std::abs((values[2] - values[3])) < 5.0 ||
        (values[2] > range_to_avoid && values[3] > range_to_avoid))
    {
      linear = 0.0;
      angular = randomTurn();
    }

    if (values[2] < values[3])
    {
      linear = 0.0;
      angular = -0.07;  // Turn Left
    }
    else
    {
      linear = 0.0;
      angular = 0.07;  // Turn Right
    }
  }
  else if (values[4] < range_to_avoid && values[5] < range_to_avoid)
  {
    linear = 0.0;
    angular = randomTurn();
  }
  else if (values[4] < range_to_avoid)
  {
    linear = 0.0;
    angular = -0.07;  // Turn Left
  }
  else if (values[5] < range_to_avoid)
  {
    linear = 0.0;
    angular = 0.07;  // Turn Right
  }
  else if (values[2] < range_to_avoid)
  {
    linear = 0;
    angular = -0.07;  // Turn Left
  }
  else if (values[3] < range_to_avoid)
  {
    linear = 0;
    angular = 0.07;  // Turn Right
  }
  else
  {
    linear = 0.0;
    angular = random_turn_value;
  }
}

void avoidObstacles(std::vector<double> values, double& linear, double& angular)
{
  double range_to_avoid = 20.0;
  if (values[0] < range_to_avoid)
  {
    if (std::abs((values[2] - values[3])) < 5.0 ||
        (values[2] > range_to_avoid && values[3] > range_to_avoid))
    {
      linear = 0.0;
      angular = randomTurn();
    }

    if (values[2] < values[3])
    {
      linear = 0.0;
      angular = -0.07;  // Turn Left
    }
    else
    {
      linear = 0.0;
      angular = 0.07;  // Turn Right
    }
  }
  else if (values[4] < range_to_avoid && values[5] < range_to_avoid)
  {
    linear = 0.0;
    angular = randomTurn();
  }
  else if (values[4] < range_to_avoid)
  {
    linear = 0.0;
    angular = -0.07;  // Turn Left
  }
  else if (values[5] < range_to_avoid)
  {
    linear = 0.0;
    angular = 0.07;  // Turn Right
  }
  else if (values[2] < range_to_avoid)
  {
    linear = 0.0;
    angular = -0.07;  // Turn Left
  }
  else if (values[3] < range_to_avoid)
  {
    linear = 0.0;
    angular = 0.07;  // Turn Right
  }
  else
  {
    linear = 0.05;  // Move Straight
    angular = 0.0;
  }
}

int main(int argc, char **argv) {
  // Initial Variables
  std::string pheeno_name;

  // Parse inputs
  CommandLineParser cml_parser(argc, argv);

  // Parse input arguments for Pheeno name.
  if (cml_parser["-n"])
  {
    std::string pheeno_number = cml_parser("-n");
    pheeno_name = "/pheeno_" + pheeno_number;
  }
  else
  {
    ROS_ERROR("Need to provide Pheeno number!");
  }


  // Initializing ROS node
  ros::init(argc, argv, "obstacle_avoidance_node");

  // Created a node handle object and sensor listener
  ros::NodeHandle node_obj;
  SensorListener sensor_listener;

  // Create Publishers and Subscribers
  ros::Publisher pub_cmd_vel = node_obj.advertise<geometry_msgs::Twist>(pheeno_name + "/cmd_vel", 100);
  ros::Subscriber sub_ir_center = node_obj.subscribe(pheeno_name + "/scan_center", 10, &SensorListener::irSensorCenterCallback, &sensor_listener);
  ros::Subscriber sub_ir_back = node_obj.subscribe(pheeno_name + "/scan_back", 10, &SensorListener::irSensorBackCallback, &sensor_listener);
  ros::Subscriber sub_ir_right = node_obj.subscribe(pheeno_name + "/scan_right", 10, &SensorListener::irSensorRightCallback, &sensor_listener);
  ros::Subscriber sub_ir_left = node_obj.subscribe(pheeno_name + "/scan_left", 10, &SensorListener::irSensorLeftCallback, &sensor_listener);
  ros::Subscriber sub_ir_cr = node_obj.subscribe(pheeno_name + "/scan_cr", 10, &SensorListener::irSensorCRightCallback, &sensor_listener);
  ros::Subscriber sub_ir_cl = node_obj.subscribe(pheeno_name + "/scan_cl", 10, &SensorListener::irSensorCLeftCallback, &sensor_listener);

  // ROS Rate loop
  ros::Rate loop_rate(10);

  // Variables before loop
  double saved_time = ros::Time::now().toSec();
  double current_duration;
  double turn_direction = randomTurn();
  double linear = 0.0;
  double angular = 0.0;
  geometry_msgs::Twist cmd_vel_msg;

  while (ros::ok())
  {
    // Find current duration of motion.
    current_duration = ros::Time::now().toSec() - saved_time;

    if (current_duration <= 2.0) {
      avoidObstacles(sensor_listener.ir_sensor_values_, linear, angular, turn_direction);
      cmd_vel_msg.linear.x = linear;
      cmd_vel_msg.angular.z = angular;

    } else if (current_duration < 5.0) {
      avoidObstacles(sensor_listener.ir_sensor_values_, linear, angular);
      cmd_vel_msg.linear.x = linear;
      cmd_vel_msg.angular.z = angular;

    } else {
      // Reset Variables
      saved_time = ros::Time::now().toSec();
      turn_direction = randomTurn();
    }

    // Publish, Spin, and Sleep
    pub_cmd_vel.publish(cmd_vel_msg);
    ros::spinOnce();
    loop_rate.sleep();

  }
}
