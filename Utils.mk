define link_binary =
	$(CXX) -o $@ $^ $(LDFLAGS)
	echo $(MAKENAME) "Make: binário" $(@F) "ligado ("$(^F)")"
endef

define assemble_object =
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	echo $(MAKENAME) "Make: objeto" $(@F) "montado ("$(^F)")"
endef

define create_dir =
	if ! [ -d $@ ]; then \
	  mkdir $@;          \
	fi
endef

define clean =
	rm -rf $(BIN_DIR)/* $(BUILD_DIR)/*
	echo $(MAKENAME) "Make: limpado os arquivos binários e objetos"
endef
