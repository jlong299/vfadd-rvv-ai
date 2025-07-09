TOP = top
MAIN = top.topMain
BUILD_DIR = ./build/top
OBJ_DIR = $(BUILD_DIR)/OBJ_DIR
TOPNAME = top
TOP_V = $(BUILD_DIR)/$(TOPNAME).v

SCALA_FILE = $(shell find ./src/main/ -name '*.scala')

$(TOP_V): $(SCALA_FILE)
	@mkdir -p $(@D)
	mill $(TOP).runMain $(MAIN) -td $(@D) --output-file $(@F)

verilog: $(TOP_V)

clean:
	rm -rf $(BUILD_DIR)

clean_mill:
	rm -rf out

clean_all: clean clean_mill

.PHONY: clean clean_all clean_mill srun run sim verilog