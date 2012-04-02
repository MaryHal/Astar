# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := astar_test

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

astar_test: 
	@echo "==== Building astar_test ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f astar_test.make

clean:
	@${MAKE} --no-print-directory -C . -f astar_test.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo "   debug32"
	@echo "   release32"
	@echo "   debug64"
	@echo "   release64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   astar_test"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"