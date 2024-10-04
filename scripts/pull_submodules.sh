#!/bin/bash
git submodule update --init --recursive
git submodule update --recursive --remote


# git submodule add https://github.com/nasa/PSP.git embedded_software/middleware/NASA-cFS/psp
# git submodule add https://github.com/nasa/ci_lab.git embedded_software/middleware/NASA-cFS/apps/ci_lab
# git submodule add https://github.com/nasa/sample_app.git embedded_software/middleware/NASA-cFS/apps/sample_app
# git submodule add https://github.com/nasa/sch_lab.git embedded_software/middleware/NASA-cFS/apps/sch_lab
# git submodule add https://github.com/nasa/to_lab.git embedded_software/middleware/NASA-cFS/apps/to_lab
# git submodule add https://github.com/nasa/sample_lib.git embedded_software/middleware/NASA-cFS/libs/sample_lib
# git submodule add https://github.com/nasa/cFS-GroundSystem.git embedded_software/middleware/NASA-cFS/tools/GroundSystem
# git submodule add https://github.com/nasa/elf2cfetbl.git embedded_software/middleware/NASA-cFS/tools/elf2cfetbl
