XRF2 V1.1

 - Change /Command to /XenoCmd, in line with what documentation stated
 - Update 20-Sim template to remove 'main' keyword which didn't do anything
 - Update 20-sim Template20Sim.cpp to have correct constructor
 - Renamed ros_data and xeno_data to ros_msg and xeno_msg
 - Renamed ros_data and xeno_data in CommandBridge, to represent better what they mean
 - Rename Xenomai_state publisher to XenoState, in line with doucmentation
 - Fix permission issue with log files
 - Logging files are generated relative to call location
 - Logging files can auto-generate folder if they don't exist.



XRF2 V1.0

 - Updated Path in CMakeLists.txt to XRF2 instead of long description
 - Updated message folder names