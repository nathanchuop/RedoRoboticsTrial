#include "../include/resource_list.hpp"
#include <libhal-actuator/rc_servo.hpp>
#include <libhal-micromod/micromod.hpp>
#include <libhal-sensor/imu/mpu6050.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/pwm.hpp>
//#include <libhal-expander/pca9685.hpp>


void application(resource_list p_resources){
    
    auto& console = *p_resources.console.value();
    auto& i2c = *p_resources.i2c.value();

    auto& clock = *p_resources.clock.value();
    auto& led = *p_resources.status_led.value();
    using namespace std::chrono_literals;
    
    for (int i = 0; i < 0xFF; i++) {
    auto res = hal::probe(i2c, i);
    }
    hal::sensor::mpu6050 mpu(i2c, 0x68); 
    mpu.power_on();
    
    

    // //expander pca9685
    // hal::expander::pca9685 pca(i2c, 0x40);
    // //pca.get_pwm_channel<1>();

    // auto pwm0 = pca.get_pwm_channel<0>(); //allows you to get the duty cycle?
    // pwm0.frequency(50); //50 Hz
    // //set servo to middle position
    // pwm0.duty_cycle(0.075f); //1.5ms duty cycle


    while(true){
        //Looping forever
        mpu.configure_full_scale(hal::sensor::mpu6050::max_acceleration::g2);


        //flashing lights program
        auto read = mpu.read();
        hal::print<1024>(console, "x: %fg, y: %fg, z: %fg\n", read.x, read.y, read.z);
        led.level(true);
        hal::delay(clock, 500ms);
        led.level(false);
        hal::delay(clock, 500ms);

    }
}