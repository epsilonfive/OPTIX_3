# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= OPTIX
ICON ?= icon.png
DESCRIPTION ?= "OPTIX CE 3.0.0"
COMPRESSED ?= NO
ARCHIVED ?= YES
HAS_PRINTF ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
