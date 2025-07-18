BUILD_DIR = ./build/vfpu
MILL_TOP = vfpu
# OBJ_DIR = $(BUILD_DIR)/OBJ_DIR
TOPNAME = topFMA
MAIN = top.$(TOPNAME)
TOP_V = $(BUILD_DIR)/$(TOPNAME).v
SCALA_FILE = $(shell find ./src/main/ -name '*.scala')

$(TOP_V): $(SCALA_FILE)
	@mkdir -p $(@D)
	mill $(MILL_TOP).runMain $(MAIN) -td $(@D) --output-file $(@F)

verilog: $(TOP_V)

verilog_fadd:
	mill vfpu.runMain race.vpu.exu.laneexu.fp.VerilogFAdd_16_32

clean:
	rm -rf $(BUILD_DIR)

clean_mill:
	rm -rf out

clean_all: clean clean_mill

.PHONY: clean clean_all clean_mill srun run sim verilog