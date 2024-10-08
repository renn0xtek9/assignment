// Copyright 2024 <Maxime Haselbauer>
/*! \file imu_driver_status.h
 *\brief ImuDriverStatus message definition
 */
#ifndef MESSAGES_IMU_DRIVER_STATUS_H
#define MESSAGES_IMU_DRIVER_STATUS_H

namespace messages {

/*! \brief Enum class for the status of the IMU driver.
 * Sys-Req: TS2
 */
enum class ImuDriverStatus { OK, BUSY, NO_DATA };

}  // namespace messages

#endif  //
