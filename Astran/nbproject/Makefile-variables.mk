#
# Generated - do not edit!
#
# NOCDDL
#
CND_BASEDIR=`pwd`
CND_BUILDDIR=build
CND_DISTDIR=dist

# Debug configuration
CND_PLATFORM_Debug=$(shell uname)
CND_ARTIFACT_DIR_Debug=build/bin/Astran.app/Contents/MacOS
CND_ARTIFACT_NAME_Debug=Astran
CND_ARTIFACT_PATH_Debug=build/bin/Astran.app/Contents/MacOS/Astran
CND_PACKAGE_DIR_Debug=dist/Debug/$(CND_PLATFORM_Release)/package
CND_PACKAGE_NAME_Debug=astran.tar
CND_PACKAGE_PATH_Debug=dist/Debug/$(CND_PLATFORM_Release)/package/astran.tar

# Release configuration
CND_PLATFORM_Release=$(shell uname)
CND_ARTIFACT_DIR_Release=build/bin/Astran.app/Contents/MacOS
CND_ARTIFACT_NAME_Release=Astran
CND_ARTIFACT_PATH_Release=build/bin/Astran.app/Contents/MacOS/Astran
CND_PACKAGE_DIR_Release=dist/Release/$(CND_PLATFORM_Release)/package
CND_PACKAGE_NAME_Release=astran.tar
CND_PACKAGE_PATH_Release=dist/Release/$(CND_PLATFORM_Release)/package/astran.tar
#
# include compiler specific variables
#
# dmake command
ROOT:sh = test -f nbproject/private/Makefile-variables.mk || \
	(mkdir -p nbproject/private && touch nbproject/private/Makefile-variables.mk)
#
# gmake command
.PHONY: $(shell test -f nbproject/private/Makefile-variables.mk || (mkdir -p nbproject/private && touch nbproject/private/Makefile-variables.mk))
#
include nbproject/private/Makefile-variables.mk
