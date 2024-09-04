#!/bin/bash
set -euxo pipefail
# shellcheck disable=SC2034
DEBIAN_FRONTEND=noninteractive
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
sudo apt-get update
sudo apt-get install -y cmake git texlive-base texlive-science texlive-latex-extra biber doxygen graphviz makefile2graph plantuml
python3 -m pip install -r requirements.txt
