// Copyright 2024 <Maxime Haselbauer>
#include <uart_imu_driver/driver.h>

namespace uart_imu {
Driver::Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
               std::queue<messages::ImuData>& imu_data_queue,
               std::mutex& queue_mutex,
               const std::string device_file_path)
    : os_layer_{os_abastrction_layer},
      imu_data_queue_{imu_data_queue},
      queue_mutex_{queue_mutex},
      device_file_path_{device_file_path} {
}
}  // namespace uart_imu
