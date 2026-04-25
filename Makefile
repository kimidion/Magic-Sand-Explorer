# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=$(realpath ../../..)
endif

PROJECT_AFTER = $(MAKE) copyprojectdata

# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

.PHONY: copyprojectdata
copyprojectdata:
	@echo "Copying project data"
	@mkdir -p bin/data
	@cp -R data/. bin/data/
	@mkdir -p bin/Magic-Sand-Explorer_debug.app/Contents/Resources/data
	@cp -R data/. bin/Magic-Sand-Explorer_debug.app/Contents/Resources/data/
	@mkdir -p bin/Magic-Sand-Explorer.app/Contents/Resources/data
	@cp -R data/. bin/Magic-Sand-Explorer.app/Contents/Resources/data/

RunDebug: copyprojectdata
RunRelease: copyprojectdata
run: copyprojectdata
