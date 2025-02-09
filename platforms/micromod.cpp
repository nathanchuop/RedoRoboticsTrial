// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-micromod/micromod.hpp>
#include <libhal-util/bit_bang_i2c.hpp>
#include <resource_list.hpp>


void initialize_platform(resource_list& p_list)
{
  using namespace hal::literals;
  p_list.reset = +[]() { hal::micromod::v1::reset(); },

  hal::micromod::v1::initialize_platform();

  p_list.status_led = &hal::micromod::v1::led();
  p_list.clock = &hal::micromod::v1::uptime_clock();
  p_list.console = &hal::micromod::v1::console(hal::buffer<128>);
  p_list.i2c = &hal::micromod::v1::i2c();

  p_list.g0_sda = &hal::micromod::v1::output_g0();
  p_list.g1_scl = &hal::micromod::v1::output_g1();
  p_list.g0_sda.value()->configure({.resistor = hal::pin_resistor::pull_up,
    .open_drain = true});
  p_list.g1_scl.value()->configure({.resistor = hal::pin_resistor::pull_up,
    .open_drain = true});

  static hal::bit_bang_i2c::pins bit_bang_pins{
    .sda = p_list.g0_sda.value(),
    .scl = p_list.g1_scl.value(),
  };
  static hal::bit_bang_i2c bit_bang_i2c(bit_bang_pins, *p_list.clock.value());
  p_list.i2c = &bit_bang_i2c;
  // p_list.pwm = &hal::micromod::v1::pwm0();

}
