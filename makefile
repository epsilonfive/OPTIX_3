# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= OPTIX
ICON ?= icon.png
DESCRIPTION ?= "OPTIX CE 3.0.0"
COMPRESSED ?= YES
ARCHIVED ?= YES
HAS_PRINTF ?= NO

CFLAGS ?= -Wall -Wextra -Os
CXXFLAGS ?= -Wall -Wextra -Os

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
