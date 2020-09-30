# Project Name
TARGET = Uexkull

# Sources
CPP_SOURCES = main.cpp

# Library Locations
LIBDAISY_DIR = ./libdaisy
DAISYSP_DIR = ./DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile