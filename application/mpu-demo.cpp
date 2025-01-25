#include "../include/resource_list.hpp"
#include <libhal-actuator/rc_servo.hpp>
#include <libhal-micromod/micromod.hpp>
#include <libhal-sensor/imu/mpu6050.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/pwm.hpp>


/* Steps for trial project from arduino code

Outside Loop
1) connect i2c, servo, and mpu6050 (gyroscope&accelerometer) to arduino (in this case, our lpc/stm)

Inside Loop
2) read mpu values
- request 14 bytes of data 
- store first accelerometer values (3), then temperature (2), then gyro values (3) as 16 bit integers. Each requires you to take 8 bits, shift it, then take another 8 bits.
- convert accel values from LSB to m/s^2, gyro to deg/s

3) adjust the servo accordingly 
- for gyroscope: change in pitch angle = (ydireciton deg/s) * (change in time), change in roll angle = (xdireciton deg/s) * (change in time)
         pitchgryo = pitchinitial + change in pitch, rollgyro = rollinitial + change in roll angle
- for accelerometer (used for drift corrections for errors in gyroscope): pitch angle = arctan(ax / sqrt[ay^2 + az^2]) , roll angle = arctan(-ay / sqrt[ax^2 + az^2])
- combine the two: change the servo by the following angles: ptich = k(pitchgryo) + (1-k)(pitchaccel), roll = k(rollgyro) + (1-k)(rollaccel)
- adjust the servo to point in these new pitch and roll direcitons

4) Delay the loop
*/












void application(resource_list p_resources){
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();
    auto& clock = *p_resources.clock.value();
    using namespace std::chrono_literals;
    
    hal::sensor::mpu6050 mpu(i2c); 

    hal::actuator::rc_servo::settings rc_settings = {
        /// @brief PWM signal frequency. Check the documentation for the RC servo to
        /// determine what range of frequencies can be used with it.
        .frequency = 50,
        /// @brief The physical minimum angle that the servo shaft can move to
        .min_angle = 0,
        /// @brief The physical maximum angle that the servo shaft can move to
        .max_angle = 90,
        /// @brief The minimum pulse width in microseconds that maps to the minimum
        /// angle of the servo
        .min_microseconds = 1000,
        /// @brief The maximum pulse width in microseconds that maps to the maximum
        /// angle of the servo.
        .max_microseconds = 2000,
    };


    hal::actuator::rc_servo servo(*p_resources.pwm.value(), rc_settings);

    


    //Setup
    servo.position(/*hal::degrees*/ 45); //starting position at the middle

    while(true){
        //Looping forever
        
        /*flashing lights program
        auto read = mpu.read();
        hal::print<1024>(console, "x: %fg, y: %fg, z: %fg", read.x, read.y, read.z);
        hal::delay(clock, 100ms);
        */

        auto result = mpu.read();
        auto accX = result.x;
        auto accY = result.y;
        auto accZ = result.z;


    }
}