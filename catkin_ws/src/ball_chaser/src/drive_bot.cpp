#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "geometry_msgs/Twist.h"
//#include <std_msgs/Float64.h>

// Publisher Motor Commands
ros::Publisher motor_command_publisher;

// This callback function executes whenever a safe_move service is requested
bool handle_command_robot_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res)
{

    ROS_INFO("DriveToTargetRequest received - j1:%1.2f, j2:%1.2f", (float)req.linear_x, (float)req.angular_z);

    // Publish  to the robot
    geometry_msgs::Twist motor_command;

    motor_command.linear.x = req.linear_x;
    motor_command.angular.z= req.angular_z;

    motor_command_publisher.publish(motor_command);

    // Wait 3 seconds for robot to settle
    ros::Duration(3).sleep();

    // Return a response message
    res.msg_feedback = "Bot velocities set - linear: " + std::to_string(req.linear_x) + " , angular: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize the drive_bot node and create a handle to it
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    // Define publishers to publish Twist messages on cmd_vel topic
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // Define a safe_move service with a handle_safe_move_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_command_robot_request);
    ROS_INFO("Ready to send robot driving commands");

    // Handle ROS communication events
    ros::spin();

    return 0;
}