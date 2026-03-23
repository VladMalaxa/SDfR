//==============================================================
// Filename    : relbot_sequence_controller.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the sequence controller function used for the control of
// of the RelBot. Inside the SteerRelbot::calculate_velocity() function, the finite state machine for the turtle movements 
// is defined, with each value of the int "selection" (line 38) being tied to a specific trajectory.
//==============================================================

#include "steering.hpp"
 
SteerRelbot::SteerRelbot() : Node("steer_relbot") {
    RCLCPP_INFO(this->get_logger(), "Init");

    // initialize topics
    create_topics();
    RCLCPP_INFO(this->get_logger(), "Created Topics");

    // initialize timer
    timer_ = this->create_wall_timer(std::chrono::duration<double>(1/DEFAULT_SETPOINT_STREAM),
                                     std::bind(&SteerRelbot::timer_callback, this));
}

void SteerRelbot::create_topics() {
    left_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/left_motor/setpoint_vel", 1);

    right_wheel_topic_ = this->create_publisher<example_interfaces::msg::Float64>(
        "/input/right_motor/setpoint_vel", 1);
}

void SteerRelbot::calculate_velocity() {    
    /* Change the code here: */

    //Change this int in order to select the different modes: 
    // 1 - straight line / 2 - circle / 3 - sharp turn / 4 - square /
    int selection = 2;

    //Obtain inital time
    static rclcpp::Time starting_time = this->get_clock()->now();
    static bool initialized = false;
    static int current_selection = 0;

    //Reset timer in case of selection change or in inital moment
    if (!initialized || current_selection != selection) {
        starting_time = this->get_clock()->now();
        if (starting_time.seconds() > 0.0) {
            initialized = true;
            current_selection = selection;
            RCLCPP_INFO(this->get_logger(), "Current selection: %d", selection);
        }
        return;
    }

    // Calculate time since the start of the selection execution
    double t = (this->get_clock()->now() - starting_time).seconds();
   
    //First case: Straight line
    if ((selection == 1) && (t<5))
    {
    left_velocity = -10;
    right_velocity = 10;
    }
    //Stop after 5 seconds moving straight.
    else if ((selection == 1) && (t>=5))
    {
    left_velocity = 0;
    right_velocity = 0;
    }
    
    //Second case: Movement in a circle.
    if ((selection == 2)&&(t<15))
    {
    left_velocity = -9;
    right_velocity = 10;
    }
    //Stop movement after 15 seconds.
    else if((selection == 2)&&(t>=15))
    {
    left_velocity = 0;
    right_velocity = 0;       
    }

    //Third case: Straight line and 90 degree turn: Start with straight line
    if ((selection == 3)&&(t<5))
    {   
    left_velocity = -10;
    right_velocity = 10;
    }

    //Turn to the left on place 90 degrees
    else if ((selection == 3)&&(t>=5)&&(t<5.635))
    {  
    left_velocity = 0;
    right_velocity = 10;
    }

    //Countinue straight for 5 more seconds
    else if ((selection == 3)&&(t>=5.635)&&(t<10.635))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Stop the turtle
    else if ((selection == 3)&&(t>=10.635))
    {   
    left_velocity = 0;
    right_velocity = 0;
    }


    //Fourth case: Square trajectory
    if ((selection == 4)&&(t<5))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Turn to the left on place 90 degrees
    else if ((selection == 4)&&(t>=5)&&(t<5.635))
    {  
    left_velocity = 0;
    right_velocity = 10;
    }

    //Countinue straight for 5 more seconds
    else if ((selection == 4)&&(t>=5.635)&&(t<10.635))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Second turn to the left on place 90 degrees
    else if ((selection == 4)&&(t>=10.635)&&(t<11.27))
    {  
    left_velocity = 0;
    right_velocity = 10;
    }
    //Countinue straight for 5 more seconds
    else if ((selection == 4)&&(t>=11.27)&&(t<16.27))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Third turn to the left on place 90 degrees
    else if ((selection == 4)&&(t>=16.27)&&(t<16.905))
    {  
    left_velocity = 0;
    right_velocity = 10;
    }

    //Countinue straight for 5 more seconds
    else if ((selection == 4)&&(t>=16.905)&&(t<21.905))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Last turn to the left on place 90 degrees
    else if ((selection == 4)&&(t>=21.905)&&(t<22.54))
    {  
    left_velocity = 0;
    right_velocity = 10;
    }

    //Countinue straight for 5 more seconds
    else if ((selection == 4)&&(t>=22.54)&&(t<27.54))
    {    
    left_velocity = -10;
    right_velocity = 10;
    }

    //Stop the turtle
    else if ((selection == 4)&&(t>=27.54))
    {   
    left_velocity = 0;
    right_velocity = 0;
    }

    if ((selection >= 5)||(selection<1))
    {
    RCLCPP_ERROR(this->get_logger(), "Not a valid selection");
    }
    /* End of your algorithm */
}

void SteerRelbot::timer_callback() {
    // calculate velocity
    calculate_velocity();

    // publish velocity to simulator
    example_interfaces::msg::Float64 left_wheel;
    left_wheel.data = left_velocity;
    example_interfaces::msg::Float64 right_wheel;
    right_wheel.data = right_velocity;
    left_wheel_topic_->publish(left_wheel);
    right_wheel_topic_->publish(right_wheel);
}

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SteerRelbot>());
    rclcpp::shutdown();
    return 0;
}