# 
# Tetris
#
# File: Makefile
#
# Description: Makefile to compile and run program. It can generate doxygen documentation.
# 
# Author: Michal Blažek
#

PROJECT_NAME=Tetris
SOURCE_DIR=src/
PROJECT=$(SOURCE_DIR)$(PROJECT_NAME)

DOX_CFG=doxygenConfig
ZIP_NAME=tetris.zip

compile: 
	qmake $(PROJECT).pro -o $(SOURCE_DIR)Makefile
	$(MAKE) -C $(SOURCE_DIR)

all: compile run

run:
	$(PROJECT)

doxygen:
	doxygen $(DOX_CFG)

clean:
	rm -rf $(SOURCE_DIR)obj/*.o $(SOURCE_DIR)Makefile $(PROJECT) $(SOURCE_DIR)moc_* $(SOURCE_DIR)ui_* $(PROJECT).pro.user $(SOURCE_DIR).qmake.stash doc/html doc/latex