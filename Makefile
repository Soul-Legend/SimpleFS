include Config.mk
include Utils.mk

vpath %.cc $(SRC_DIR)

ifdef INCL_DIR
	CXXFLAGS += -I$(INCL_DIR) -I$(INCL_DIR)/gui
endif

COMMON_OBJS = $(patsubst %,$(BUILD_DIR)/%.o,$(MODULES))
GUI_OBJS = $(patsubst %,$(BUILD_DIR)/gui/%.o,$(GUI_MODULES))

all: bin/shell bin/gui

.PHONY: clean

$(BIN_DIR)/shell: $(BUILD_DIR)/shell.o $(COMMON_OBJS) | $(BIN_DIR)
	@$(link_binary)

$(BIN_DIR)/gui: $(BUILD_DIR)/gui.o $(COMMON_OBJS) $(GUI_OBJS) | $(BIN_DIR)
	@$(link_binary)

$(BUILD_DIR)/%.o: %.cc | $(BUILD_DIR) $(BUILD_DIR)/gui
	@$(assemble_object)

# Cria os diretórios não existentes.
$(BUILD_DIR) $(BUILD_DIR)/gui $(BIN_DIR):
	@$(create_dir)

clean:
	@$(clean)
