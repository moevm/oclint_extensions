.ONESHELL:

default: help

GCC_EXAMPLES = $(wildcard examples/ex-*)
MAKE_EXAMPLES = $(wildcard examples/make/ex-*)
.PHONY: $(GCC_EXAMPLES) $(MAKE_EXAMPLES)

$(GCC_EXAMPLES):
	examples/test-gcc.sh $@

$(MAKE_EXAMPLES):
	examples/test-makefile.sh $@

########
# HELP #
########

help:
	@echo "=== TARGETS FOR BUILD/INSTALL/UNINSTALL ==="
	@echo "    install          # installs everyting listed below"
	@echo "    install_oclint"
	@echo "    install_rules"
	@echo "    install_deps"
	@echo "    uninstall_oclint"
	@echo "    uninstall_rules"
	@echo "    build_oclint"
	@echo "    build_rules"
	@echo "    clean"
	@echo "=== TARGETS FOR RUNNING EXAMPLES ==="
	@for x in $(GCC_EXAMPLES); do echo "    $$x"; done;
	@for x in $(MAKE_EXAMPLES); do echo "    $$x"; done;
	
########
# DEPS #
########

.PHONY: install_deps
install_deps:
	apt install -y ninja-build cmake bear g++

##########
# OCLINT #
##########

.PHONY: build_oclint install_oclint uninstall_oclint

# clone repo (oclint directory)
oclint:
	OCLINT_22_02_COMMIT = d776db51c8574df406b2b0dc1b43b0b9b2d86d34
	
	git clone https://github.com/oclint/oclint.git oclint
	cd oclint
	git reset --hard $OCLINT_22_02_COMMIT
	cd ..
	
oclint/build/oclint-release/bin/oclint:
	cd oclint/oclint-scripts
	./make
	
build_oclint: oclint oclint/build/oclint-release/bin/oclint

install_oclint: build_oclint
	cp -r oclint/build/oclint-release/lib/* /usr/local/lib/
	cp -r oclint/build/oclint-release/bin/* /usr/local/bin/
	
uninstall_oclint:
	rm -rf /usr/local/bin/oclint
	rm -rf /usr/local/lib/oclint
	
#########
# RULES #
#########

.PHONY: build_rules install_rules uninstall_rules

# build dir
build:
	mkdir build
	cd build
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../
	cd ..
	
build_rules: build build_oclint
	cd build
	cmake --build .
	cp compile_commands.json ../
	cd ..

install_rules: build_rules
	cd build
	cmake --install .
	cd ..

uninstall_rules:
	xargs rm < build/install_manifest.txt

###########
# GENERAL #
###########

.PHONY: install clean

install: install_deps install_oclint install_rules

clean:
	rm -rf build
