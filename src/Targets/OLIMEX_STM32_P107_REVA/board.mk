# Define linker script file here
LDSCRIPT= $(PORTLD)/STM32F107xC.ld
MCU  = cortex-m3

# List of all the board related files.
BOARDSRC = Targets/OLIMEX_STM32_P107_REVA/board.c

# Required include directories
BOARDINC =Targets/OLIMEX_STM32_P107_REVA

# Include required makefiles
include $(CHIBIOS)/os/hal/platforms/STM32F1xx/platform.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F1xx/port.mk