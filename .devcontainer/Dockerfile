# Base image
FROM mcr.microsoft.com/devcontainers/base:jammy

COPY setup_build_environment_on_ubuntu_22.sh .
COPY setup_documentation_environment_on_ubuntu_22.sh .
COPY requirements.txt .
RUN chmod +x setup_build_environment_on_ubuntu_22.sh
RUN chmod +x setup_documentation_environment_on_ubuntu_22.sh
RUN ./setup_build_environment_on_ubuntu_22.sh
RUN ./setup_documentation_environment_on_ubuntu_22.sh

WORKDIR /workspace

CMD ["/bin/bash"]
