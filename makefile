# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= OPTIX
ICON ?= icon.png
DESCRIPTION ?= "OPTIX CE 3.0.0"
COMPRESSED ?= YES
ARCHIVED ?= YES
HAS_PRINTF ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# font options
FONTPACKNAME ?= DRSANS
FONTDIR ?= $(SRCDIR)/optix/font

DEPS ?= $(BINDIR)/$(FONTPACKNAME).bin $(BINDIR)/$(FONTPACKNAME).8xv

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

# ----------------------------

$(BINDIR)/$(FONTPACKNAME).bin: $(FONTDIR)/drsans-07-normal.fnt
	$(Q)$(call MKDIR,$(@D))
	$(Q)convfont -o fontpack -N "Dr. Sans" -P "Calculator 1252" \
	-D "A font designed for maximum readability" \
	-V "15 April 2019" \
	-f $(FONTDIR)/drsans-07-normal.fnt -l 8  -x 3 -c 1 -a 1 -b 1 -w normal -s 0 \
	$(BINDIR)/$(FONTPACKNAME).bin
$(BINDIR)/$(FONTPACKNAME).8xv: $(BINDIR)/$(FONTPACKNAME).bin
	$(Q)$(call MKDIR,$(@D))
	$(Q)convbin --iformat bin --input $(BINDIR)/$(FONTPACKNAME).bin --oformat 8xv --output $(BINDIR)/$(FONTPACKNAME).8xv --archive --name $(FONTPACKNAME)

