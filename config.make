################################################################################
# CONFIGURE PROJECT MAKEFILE (optional)
#   This file is where we make project specific configurations.
################################################################################

################################################################################
# OF ROOT
#   The location of your root openFrameworks installation
#       (default) OF_ROOT = ../../.. 
################################################################################
# OF_ROOT = ../../..

################################################################################
# PROJECT ROOT
#   The location of the project - a starting place for searching for files
#       (default) PROJECT_ROOT = . (this directory)
#    
################################################################################
# PROJECT_ROOT = .

################################################################################
# PROJECT SPECIFIC CHECKS
#   This is a project defined section to create internal makefile flags to 
#   conditionally enable or disable the addition of various features within 
#   this makefile.  For instance, if you want to make changes based on whether 
#   GTK is installed, one might test that here and create a variable to check. 
################################################################################
# None

################################################################################
# PROJECT EXTERNAL SOURCE PATHS
#   These are fully qualified paths that are not within the PROJECT_ROOT folder.
#   Like source folders in the PROJECT_ROOT, these paths are subject to 
#   exlclusion via the PROJECT_EXLCUSIONS list.
#
#     (default) PROJECT_EXTERNAL_SOURCE_PATHS = (blank) 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_EXTERNAL_SOURCE_PATHS = 

################################################################################
# PROJECT EXCLUSIONS
#   These makefiles assume that all folders in your current project directory 
#   and any listed in the PROJECT_EXTERNAL_SOURCH_PATHS are are valid locations
#   to look for source code. The any folders or files that match any of the 
#   items in the PROJECT_EXCLUSIONS list below will be ignored.
#
#   Each item in the PROJECT_EXCLUSIONS list will be treated as a complete 
#   string unless teh user adds a wildcard (%) operator to match subdirectories.
#   GNU make only allows one wildcard for matching.  The second wildcard (%) is
#   treated literally.
#
#      (default) PROJECT_EXCLUSIONS = (blank)
#
#		Will automatically exclude the following:
#
#			$(PROJECT_ROOT)/bin%
#			$(PROJECT_ROOT)/obj%
#			$(PROJECT_ROOT)/%.xcodeproj
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_EXCLUSIONS =

################################################################################
# PROJECT LINKER FLAGS
#	These flags will be sent to the linker when compiling the executable.
#
#		(default) PROJECT_LDFLAGS = -Wl,-rpath=./libs
#
#   Note: Leave a leading space when adding list items with the += operator
#
# Currently, shared libraries that are needed are copied to the 
# $(PROJECT_ROOT)/bin/libs directory.  The following LDFLAGS tell the linker to
# add a runtime path to search for those shared libraries, since they aren't 
# incorporated directly into the final executable application binary.
################################################################################
LIBFREENECT2_ROOT = $(shell cd $(PROJECT_ROOT) && pwd)/deps/libfreenect2/install

PROJECT_LDFLAGS += -L$(LIBFREENECT2_ROOT)/lib
PROJECT_LDFLAGS += -lfreenect2
PROJECT_LDFLAGS += -Wl,-rpath,$(LIBFREENECT2_ROOT)/lib

################################################################################
# PROJECT DEFINES
#   Create a space-delimited list of DEFINES. The list will be converted into 
#   CFLAGS with the "-D" flag later in the makefile.
#
#		(default) PROJECT_DEFINES = (blank)
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_DEFINES = 

################################################################################
# PROJECT CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   project.  These CFLAGS will be used IN ADDITION TO the PLATFORM_CFLAGS 
#   defined in your platform specific core configuration files. These flags are
#   presented to the compiler BEFORE the PROJECT_OPTIMIZATION_CFLAGS below. 
#
#		(default) PROJECT_CFLAGS = (blank)
# 
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
PROJECT_CFLAGS += -I$(LIBFREENECT2_ROOT)/include

################################################################################
# PROJECT CXXFLAGS
#   This is a list of fully qualified CXXFLAGS required when compiling for this 
#   project. These flags will be used IN ADDITION TO the PLATFORM_CXXFLAGS 
#   defined in your platform specific core configuration files. These flags are
#   presented to the compiler BEFORE the PROJECT_OPTIMIZATION_CFLAGS below. 
#
#		(default) PROJECT_CXXFLAGS = (blank)
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_CXXFLAGS =

################################################################################
# PROJECT OPTIMIZATION CFLAGS
#   These flags will be appended to the end of the CFLAGS command string and 
#   thus override any previous flags. They should be used carefully, if at all.
#
#		(default) PROJECT_OPTIMIZATION_CFLAGS = (blank)
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_OPTIMIZATION_CFLAGS = 

################################################################################
# ADDONS
#   Add the libs that need to be compiled. By default of_v0.9.3 will compile 
#   all addons that are found in the addons folder. If you want to compile a 
#   subset, specify them here. You can also use the Project Generator to create
#   a project with specific addons.
#
#		(default) PROJECT_ADDONS = (all addons)
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_ADDONS = 

################################################################################
# INTERNAL EXCLUSIONS
#   This is a project defined section to exclude internal addons from 
#   compilation. This is especially useful if you want to use an external 
#   version of an addon that is included in the OF distribution.
#
#   For instance, if you want to use your own version of ofxOpenCv you can 
#   add it to the PROJECT_EXTERNAL_SOURCE_PATHS and add ofxOpenCv to the 
#   PROJECT_INTERNAL_EXCLUSIONS.
#
#		(default) PROJECT_INTERNAL_EXCLUSIONS = (none)
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
# PROJECT_INTERNAL_EXCLUSIONS =
