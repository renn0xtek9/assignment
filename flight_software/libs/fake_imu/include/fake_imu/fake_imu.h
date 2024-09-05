// Copyright 2024 <Maxime Haselbauer>
#ifndef FAKE_IMU_FAKE_IMU_H
#define FAKE_IMU_FAKE_IMU_H

#include <os_abstraction_layer/os_abstraction_layer_interface.h>

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <queue>
#include <string>
/*! \brief IMU data structure
 */
struct ImuData {
  float a_x{0.1F};
  float a_y{0.2F};
  float a_z{0.3F};
  float omega_x{0.4F};
  float omega_y{0.5F};
  float omega_z{0.6F};
  float temperature{25.0F};
  std::chrono::nanoseconds timestamp{};
};

struct ImuDataQueue {
  std::uint32_t dropped_message{0};
  std::queue<ImuData> data_queue{};
};

enum class ImuDriverStatus { OK, BUSY, NO_DATA };

/*! \brief overload steam operator for ImuData*/
inline std::ostream& operator<<(std::ostream& out, const ImuData& data) {
  auto to_time = [&data]() {
    auto epoch = std::chrono::system_clock::time_point(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(data.timestamp));
    std::time_t time = std::chrono::system_clock::to_time_t(epoch);
    std::tm* local_tm = std::localtime(&time);
    return std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
  };

  out << "a_x: " << data.a_x << " a_y: " << data.a_y << " a_z: " << data.a_z << " w_x: " << data.omega_x
      << " w_y: " << data.omega_y << " w_z: " << data.omega_z << " T: " << data.temperature << " time: " << to_time()
      << std::endl;
  return out;
}

/*! \brief Fake an IMU device for usage in Software in the loop context
 */
class FakeImu {
 public:
  /*! \ brief Instatiate fake IMU device
   *\param os_abstraction_layer reference to an operating system abstraction layer.
   */
  explicit FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer);

  /*! \brief Simulate normal operation of the IMU
   *This sends data in the virtual serial device specified by the file descriptor for a given number of seconds
   *\param device_file_path the path to the device file
   *\param duration_ms duration for which data shalll be sent
   */
  void SimulateNormalOperation(const std::string& device_file_path, const std::chrono::milliseconds& duration_ms);

 private:
  /*! \brief Send a byte to the device file
   */
  void SendByte(const std::byte& byte);

  /*! \brief Send IMU data to the device file
   */
  void SendImuData(const ImuData& data);

  int file_descriptor_{-1};

  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
};

#endif  // FAKE_IMU_FAKE_IMU_H
