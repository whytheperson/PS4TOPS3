.SUFFIXES:

ifeq ($(strip $(PSL1GHT)),)
$(error "PSL1GHT is not defined")
endif

TITLE       := PS3 2D Port Test
APPID       := DCPORT001
CONTENTID   := UP0001-DCPORT001_00-0000000000000000
ICON0       := $(CURDIR)/ICON0.PNG

include $(PSL1GHT)/ppu_rules


TARGET      := ps3_port_test
BUILD       := build

SOURCES     := source
DATA        := data
SHADERS     := shaders
INCLUDES    := include


LIBS := \
    -ltiny3d \
    -lrsx \
    -lsimdmath \
    -lgcm_sys \
    -lio \
    -lsysutil \
    -lrt \
    -llv2 \
    -lsysmodule \
    -lm


CFLAGS := \
    -O2 \
    -Wall \
    -Wextra \
    -mcpu=cell \
    $(MACHDEP) \
    $(INCLUDE)

CXXFLAGS := $(CFLAGS)

LDFLAGS := \
    $(MACHDEP) \
    -Wl,-Map,$(notdir $@).map


LIBDIRS :=


ifneq ($(BUILD),$(notdir $(CURDIR)))


export OUTPUT := $(CURDIR)/$(TARGET)


export VPATH := \
    $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
    $(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
    $(foreach dir,$(SHADERS),$(CURDIR)/$(dir))


export DEPSDIR := $(CURDIR)/$(BUILD)


CFILES := \
    $(foreach dir,$(SOURCES), \
    $(notdir $(wildcard $(dir)/*.c)))


CPPFILES := \
    $(foreach dir,$(SOURCES), \
    $(notdir $(wildcard $(dir)/*.cpp)))


sFILES := \
    $(foreach dir,$(SOURCES), \
    $(notdir $(wildcard $(dir)/*.s)))


SFILES := \
    $(foreach dir,$(SOURCES), \
    $(notdir $(wildcard $(dir)/*.S)))


BINFILES := \
    $(foreach dir,$(DATA), \
    $(notdir $(wildcard $(dir)/*.bin)))


ifeq ($(strip $(CPPFILES)),)

export LD := $(CC)

else

export LD := $(CXX)

endif


export OFILES := \
    $(addsuffix .o,$(BINFILES)) \
    $(CPPFILES:.cpp=.o) \
    $(CFILES:.c=.o) \
    $(sFILES:.s=.o) \
    $(SFILES:.S=.o)


export INCLUDE := \
    $(foreach dir,$(INCLUDES), \
    -I$(CURDIR)/$(dir)) \
    $(LIBPSL1GHT_INC) \
    -I$(CURDIR)/$(BUILD) \
    -I$(PORTLIBS)/include


export LIBPATHS := \
    $(LIBPSL1GHT_LIB) \
    -L$(PORTLIBS)/lib


.PHONY: $(BUILD) clean run pkg


$(BUILD):

	@[ -d $@ ] || mkdir -p $@

	@$(MAKE) \
		--no-print-directory \
		-C $(BUILD) \
		-f $(CURDIR)/Makefile


clean:

	@echo clean...

	@rm -fr \
		$(BUILD) \
		*.elf \
		*.self \
		*.pkg \
		EBOOT.BIN \
		PARAM.SFO


run:

	ps3load $(OUTPUT).self


pkg: $(BUILD) $(OUTPUT).pkg



else


DEPENDS := $(OFILES:.o=.d)


$(OUTPUT).self: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES)


%.bin.o: %.bin

	@echo $(notdir $<)

	@$(bin2o)


-include $(DEPENDS)


endif
