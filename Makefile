BUILD_DIR = ./build/vfpu
MILL_TOP = vfpu
TOPNAME = top
CHISEL_MAIN = top.$(TOPNAME)
TOP_V = $(BUILD_DIR)/$(TOPNAME).v
SCALA_FILE = $(shell find ./src/main/ -name '*.scala')

VERILATOR = verilator
VERILATOR_FLAGS +=  -MMD --trace --build -cc --exe \
	                                 -O3 --x-assign fast --x-initial fast -report-unoptflat
VERILATOR_FLAGS += --timescale 1us/1us
VERILATOR_FLAGS += -j 28

$(TOP_V): $(SCALA_FILE)
	@mkdir -p $(@D)
	mill $(MILL_TOP).runMain $(CHISEL_MAIN) -td $(@D) --output-file $(@F)

verilog: $(TOP_V)

verilog_fadd:
	mill vfpu.runMain race.vpu.exu.laneexu.fp.VerilogFAdd_16_32

vcd ?= 1
ifeq ($(vcd), 1)
    CFLAGS += -DVCD
endif

# C flags
SOFTFLOAT_DIR = $(abspath ./src/test/csrc/softfloat)
INC_PATH += $(abspath ./src/test/csrc/include)
INC_PATH += $(SOFTFLOAT_DIR)/include
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) $(CFLAGS_SIM) -DTOP_NAME="V$(TOPNAME)"
LDFLAGS += $(SOFTFLOAT_DIR)/lib/softfloat.a

# source file
VSRCS = $(TOP_V)
CSRCS = $(shell find $(abspath ./src/test/csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

OBJ_DIR = $(BUILD_DIR)/OBJ_DIR
BIN = $(BUILD_DIR)/$(TOPNAME)
NPC_EXEC := $(BIN)

$(BIN): $(VSRCS) $(CSRCS) $(shell find ./src/test/csrc/include -name "*.h")
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) -top $(TOPNAME) $(VSRCS) $(CSRCS) \
	$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
	--Mdir $(OBJ_DIR) -o $(abspath $(BIN))

run: $(BIN)
	@echo "------------ RUN --------------"
	$(NPC_EXEC)

clean:
	rm -rf $(BUILD_DIR)

clean_mill:
	rm -rf out

clean_all: clean clean_mill

.PHONY: clean clean_all clean_mill srun run sim verilog