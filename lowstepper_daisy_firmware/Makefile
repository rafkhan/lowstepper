# Project Name
TARGET = lowstepper_daisy_firmware

LDFLAGS += -u _printf_float

# Sources
CPP_SOURCES = lowstepper_daisy_firmware.cpp
CPP_SOURCES += lowstepper/LowStepperLfo.cpp
CPP_SOURCES += lowstepper/util.cpp
CPP_SOURCES += lowstepper/AverageBuffer.cpp
CPP_SOURCES += lowstepper/SyncManager.cpp
CPP_SOURCES += hardware/GateInput.cpp
CPP_SOURCES += hardware/LSSwitch.cpp
# CPP_SOURCES += AdditionalFile2.cpp

# Library Locations
LIBDAISY_DIR = ../../DaisyExamples/libdaisy/
DAISYSP_DIR = ../../DaisyExamples/daisysp/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
