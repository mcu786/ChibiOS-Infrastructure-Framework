MODULEPATH=Modules
ACTIVEMODULES=$(MODULEPATH)/activeModules

# Find modules in the module directory and include modules specified in 
# activeModules
#MODULENAMES:=$(shell  find $(MODULEPATH) -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | grep -x -f $(ACTIVEMODULES))

MODULENAMES:=$(shell cat  $(ACTIVEMODULES) | sed -e '/^%/d' | uniq)
MODULEDIRS:=$(patsubst %, $(MODULEPATH)/%/, $(MODULENAMES))


# Only find module source files if at least one module is selected
ifneq ($(MODULENAMES),)
# Find the C files in the module directory including subdirectories!
CSOURCES:=$(shell find $(MODULEDIRS) -name "*.c")
CPPSOURCES:=$(shell find $(MODULEDIRS) -name "*.cpp")


# Add all infos to the global build settings
CSRC += $(CSOURCES)
CPPSRC += $(CPPSOURCES)
#INCDIR += $(MODULEPATH) $(MODULEDIRS)

$(info Activated modules are: $(MODULENAMES))

# Include all sub-makefiles. This might be used to build external modules
include $(shell find $(MODULEDIRS) -name "additional.mk")

else

$(warning No modules activated!)

endif