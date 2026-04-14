#include "project_ppd2.hpp"

ProjectPpd2::ProjectPpd2(uint write_decimator_freq, uint monitor_freq) :
    XenoFrt20Sim(write_decimator_freq, monitor_freq, file, &data_to_be_logged),
    file(1,"./xrf2_logging/PROJECT_PPD2","bin"), // change project name to your project name
    controller()
{
     printf("%s: Constructing rampio\n", __FUNCTION__);
    // Add variables to logger to be logged, has to be done before you can log data
    //logger.addVariable("this_is_a_int", integer);
    //logger.addVariable("this_is_a_double", double_);
    //logger.addVariable("this_is_a_float", float_);
    //logger.addVariable("this_is_a_char", character);
    //logger.addVariable("this_is_a_bool", boolean);
    
    // To infinite run the controller, uncomment line below
    controller.SetFinishTime(0.0);
}

ProjectPpd2::~ProjectPpd2()
{
    
}

int ProjectPpd2::initialising()
{
    // Initialize hardware and logger
    logger.initialise();
    ico_io.init();
    
    // Reset positions to zero
    pos_left = 0.0;
    pos_right = 0.0;

    monitor.printf("The program is initialising ...\n");
    return 1;
}

int ProjectPpd2::initialised()
{
    // Keep the physical syste in a state to be used in the run state
    // Call start() or return 1 to go to run state

    monitor.printf("The program has been initialised.\n");
    return 1;
}

int ProjectPpd2::run()
{
    // Start logger if not already running
    logger.start();

    // 1. CONSTANTS
    const float gear_ratio = 15.58;
    const float counts_per_rev = 1024.0 * gear_ratio * 4.0;
    const float ticks_to_rad = (2.0 * 3.14159) / counts_per_rev;

    // 2. ENCODER PROCESSING (Left Motor - Channel 2)
    int32_t diff_ch2 = sample_data.channel2 - this->last_channel2;

    if (diff_ch2 > 8192)
    {
        diff_ch2 -= 16384;
    }
    else if (diff_ch2 < -8192)
    {
        diff_ch2 += 16384;
    }

    last_channel2 = sample_data.channel2;
    pos_left += diff_ch2 * ticks_to_rad;

    // 3. ENCODER PROCESSING (Right Motor - Channel 1, Inverted)
    int32_t diff_ch1 = -(sample_data.channel1 - this->last_channel1); 

    if (diff_ch1 > 8192)
    {
        diff_ch1 -= 16384;
    }
    else if (diff_ch1 < -8192)
    {
        diff_ch1 += 16384;
    }

    last_channel1 = sample_data.channel1;
    pos_right += diff_ch1 * ticks_to_rad;

    // 4. PREPARE CONTROLLER INPUTS
    u[0] = pos_left;
    u[1] = pos_right;
    u[2] = ros_msg.left_wheel_vel;  // Input from ROS Subscriber
    u[3] = ros_msg.right_wheel_vel; // Input from ROS Subscriber

    // 5. CALCULATE CONTROL EFFORT
    controller.Calculate(u, y);

    // 6. SATURATION AND OUTPUT MAPPING
    double out_left = y[0];
    double out_right = y[1];

    // Clamp Left Motor
    if (out_left < -2047)
    {
        out_left = -2047;
    }
    else if (out_left > 2047)
    {
        out_left = 2047;
    }

    // Clamp Right Motor
    if (out_right < -2047)
    {
        out_right = -2047;
    }
    else if (out_right > 2047)
    {
        out_right = 2047;
    }

    // Assign to hardware actuation channels
    actuate_data.pwm2 = (int16_t)out_left;   // Left motor hardware channel
    actuate_data.pwm1 = (int16_t)-out_right; // Right motor hardware channel (inverted)

    // 7. MONITORING
    monitor.printf("Pos L: %.2f | Pos R: %.2f | PWM L: %.0f | PWM R: %.0f\n", 
                    pos_left, pos_right, out_left, out_right);

    if(controller.IsFinished())
    {
        return 1;
    }

    return 0;
}

int ProjectPpd2::stopping()
{
    logger.stop();
    
    // Stop the motors
    actuate_data.pwm1 = 0;
    actuate_data.pwm2 = 0;

    monitor.printf("Stopping ...\n");
    return 1;
}

int ProjectPpd2::stopped()
{
    monitor.printf("The program has stopped.\n");
    return 0;
}

int ProjectPpd2::pausing()
{
        // Stopping the motors
    actuate_data.pwm1 = 0;
    actuate_data.pwm2 = 0;

    evl_printf("Program is pausing ...\n");
    return 1 ;
}

int ProjectPpd2::paused()
{
    // Keep stopping the motors
    actuate_data.pwm1 = 0;
    actuate_data.pwm2 = 0;

    // keep track of encoders such that when physically moved, they still behave correctly
    last_channel1 = sample_data.channel1;
    last_channel2 = sample_data.channel2;

    monitor.printf("Program has been paused.\n");
    return 0;
}

int ProjectPpd2::error()
{
    // Error detected in the system 
    /monitor.printf("An error occured.\n");
    return 0;
}
