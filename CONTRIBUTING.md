# CONTRIBUTING

# Folder structure

```bash
.
├── CONTRIBUTING.md # Main documentation for Developers
├── README.md # Short project description
├── Makefile # Main makefile. Orchestrate the various build (using CMake)
├── .devcontainer
│   ├── ... # Docker file and scripts for the development container
├── .github
│   ├── ... # GitHub Continuous Integration files.
├── CMakeLists.txt
├── cmake_modules
│   ├── ... # all cmake modules needed in the CMakeLists
├── conanfile.txt
├── conan_profiles
│   └── ... # Conan build profiles.
├── desktop_client # Client (x86) application
│   ├── application
│   │   ├── .. # Main files
│   ├── CMakeLists.txt
│   ├── libs
│   │   ├── ... # Libraries
│   └── system_interfaces
│       ├── ... # System interfaces (describe data exchange between subsystems)
├── documentation
│   ├── development_process
│   │   ├── ... # Documentation of development process
│   │   └── software_build_process
│   │       ├── ... # Documentation of build process itself
│   ├── doxygen
│   │   ├── ... # code documentation
│   ├── requirements_engineering
│   │   ├── ... # Latex document for requirements engineering
│   └── resources
│       ├── ... # other documentation resources
├── License.txt
├── scripts
│   └── ... # additional script for developers
```


## How-to
### Setup
- Install dependencies:
It is recomended to use the VSCode with the `.devcontainer`.
If you want to defvelop natively on an Ubuntu 22 distribution, you can run
```bash
./.devcontainer/setup_build_environment_on_ubuntu_22.sh
./.devcontainer/setup_documentation_environment_on_ubuntu_22.sh
```

### Develop
- Clean the repository:

```bash
make clean
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
