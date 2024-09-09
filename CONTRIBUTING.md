# CONTRIBUTING

# Folder structure

```bash
.
├── cmake_modules # All cmake modules
├── documentation # All project documentation
│   ├── development_process # documentation of build and development process
│   ├── doxygen
│   ├── requirements_engineering # engineering and requirement documentation
├── flight_software # All source code
│   ├── applications # All applications
│   │   ├── flight_software_application # Actual flight software application
│   │   └── software_in_the_loop # Software in the loop application
│   ├── libs # All libraries
│   │   ├── embedded_software # Only libraries for embedded software
│   │   │   ├── serializer
│   │   │   └── uart_imu_driver # UART IMU driver
│   │   │       ├── include # Public header files
│   │   │       ├── src # C++ files
│   │   │       └── tests # Unit tests
│   │   ├── ground_software # Only libraries for test and non embedded
│   │   │   └── fake_imu
│   │   └── os_abstraction_layer
│   └── system_interfaces # All interfaces between applications and libraries
│       ├── communication_protocols # Constants definitions for communication protocols
│       ├── deployment_configuration # All configuration files for deployment definitions
│       ├── display_messages # Only for testing and developement purpose
│       └── messages # All messages exchanges between applications and libraries
└── scripts # Shell scripts
├── Makefile # Main Makefile that orchestrate the build process
├── README.md # General project description
├── CONTRIBUTING.md # How-tos for developer
```


## How-to
### Setup
- Install dependencies:
It is recomended to use the VSCode with the `.devcontainer` and "Reopen in Container feature"
If you want to develop natively on an Ubuntu 22 distribution, you can run
```bash
./.devcontainer/setup_build_environment_on_ubuntu_22.sh
./.devcontainer/setup_documentation_environment_on_ubuntu_22.sh
```

### Develop
- Clean the repository:

```bash
make clean
```

- Build all artifacts

```bash
make all
```

- Format all files of the repository using relevant `pre-commit` hook
```bash
make format
```

- Configure the project
```bash
make configure
```

- Run all tests
```bash
make test
```

- Run all code quality checks
```bash
make quality
```

- Run all code performance checks
```bash
make validate
```

### Generate artifacts

- Generate a debian archive that contains all the project documentation.
```bash
make documentation
```

- Create a debian package that contains the release build binary.
```bash
make packaging
```
