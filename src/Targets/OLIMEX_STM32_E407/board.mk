# Define linker script file here
LDSCRIPT= $(PORTLD)/STM32F407xG.ld
MCU  = cortex-m4

# List of all the board related files.
BOARDSRC = Targets/OLIMEX_STM32_E407/board.c

# Required include directories
BOARDINC = Targets/OLIMEX_STM32_E407

# Include required makefiles
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk

USE_FPU = no