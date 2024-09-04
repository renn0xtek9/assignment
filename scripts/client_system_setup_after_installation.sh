#!/bin/bash
set -euxo pipefail
sudo usermod -aG dialout "$USER"
