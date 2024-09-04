// Copyright 2024 <Maxime Haselbauer>

const char kFakeImuSilDeviceFilePath[] = "/tmp/vserial2";
constexpr std::uint16_t IMU_MEASUREMENT_PER_SECONDS{125U};
constexpr std::uint32_t SLEEP_TIME_BETWEEN_MEASUREMENTS_MS{
    static_cast<std::uint32_t>(static_cast<double>(1000) / static_cast<double>(IMU_MEASUREMENT_PER_SECONDS))};
