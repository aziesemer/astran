#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=llvm-g++
CXX=llvm-g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/icpdfrm.o \
	${OBJECTDIR}/src/wxfp.o \
	${OBJECTDIR}/src/pathfinder2.o \
	${OBJECTDIR}/src/autocell2.o \
	${OBJECTDIR}/src/designmng.o \
	${OBJECTDIR}/src/cellnetlst.o \
	${OBJECTDIR}/src/spice.o \
	${OBJECTDIR}/src/wxrules.o \
	${OBJECTDIR}/src/draw.o \
	${OBJECTDIR}/src/wxabout.o \
	${OBJECTDIR}/src/icpdApp.o \
	${OBJECTDIR}/src/wxautocell.o \
	${OBJECTDIR}/src/wxPreferences.o \
	${OBJECTDIR}/src/pathfinder.o \
	${OBJECTDIR}/src/placer.o \
	${OBJECTDIR}/src/rules.o \
	${OBJECTDIR}/src/gds.o \
	${OBJECTDIR}/src/lef.o \
	${OBJECTDIR}/src/router.o \
	${OBJECTDIR}/src/circuit.o \
	${OBJECTDIR}/src/clayout.o \
	${OBJECTDIR}/src/compaction.o \
	${OBJECTDIR}/src/wxinterface.o \
	${OBJECTDIR}/src/cif.o \
	${OBJECTDIR}/src/wxcircuit.o \
	${OBJECTDIR}/src/verilog.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 `wx-config --cppflags` 
CXXFLAGS=-m32 `wx-config --cppflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk build/bin/Astran.app/Contents/MacOS/Astran

build/bin/Astran.app/Contents/MacOS/Astran: ${OBJECTFILES}
	${MKDIR} -p build/bin/Astran.app/Contents/MacOS
	${LINK.cc} `wx-config --libs` -o build/bin/Astran.app/Contents/MacOS/Astran  ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/icpdfrm.o: src/icpdfrm.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/icpdfrm.o src/icpdfrm.cpp

${OBJECTDIR}/src/wxfp.o: src/wxfp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxfp.o src/wxfp.cpp

${OBJECTDIR}/src/pathfinder2.o: src/pathfinder2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/pathfinder2.o src/pathfinder2.cpp

${OBJECTDIR}/src/autocell2.o: src/autocell2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/autocell2.o src/autocell2.cpp

${OBJECTDIR}/src/designmng.o: src/designmng.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/designmng.o src/designmng.cpp

${OBJECTDIR}/src/cellnetlst.o: src/cellnetlst.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/cellnetlst.o src/cellnetlst.cpp

${OBJECTDIR}/src/spice.o: src/spice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/spice.o src/spice.cpp

${OBJECTDIR}/src/wxrules.o: src/wxrules.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxrules.o src/wxrules.cpp

${OBJECTDIR}/src/draw.o: src/draw.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/draw.o src/draw.cpp

${OBJECTDIR}/src/wxabout.o: src/wxabout.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxabout.o src/wxabout.cpp

${OBJECTDIR}/src/icpdApp.o: src/icpdApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/icpdApp.o src/icpdApp.cpp

${OBJECTDIR}/src/wxautocell.o: src/wxautocell.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxautocell.o src/wxautocell.cpp

${OBJECTDIR}/src/wxPreferences.o: src/wxPreferences.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxPreferences.o src/wxPreferences.cpp

${OBJECTDIR}/src/pathfinder.o: src/pathfinder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/pathfinder.o src/pathfinder.cpp

${OBJECTDIR}/src/placer.o: src/placer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/placer.o src/placer.cpp

${OBJECTDIR}/src/rules.o: src/rules.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/rules.o src/rules.cpp

${OBJECTDIR}/src/gds.o: src/gds.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gds.o src/gds.cpp

${OBJECTDIR}/src/lef.o: src/lef.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/lef.o src/lef.cpp

${OBJECTDIR}/src/router.o: src/router.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/router.o src/router.cpp

${OBJECTDIR}/src/circuit.o: src/circuit.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/circuit.o src/circuit.cpp

${OBJECTDIR}/src/clayout.o: src/clayout.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/clayout.o src/clayout.cpp

${OBJECTDIR}/src/compaction.o: src/compaction.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/compaction.o src/compaction.cpp

${OBJECTDIR}/src/wxinterface.o: src/wxinterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxinterface.o src/wxinterface.cpp

${OBJECTDIR}/src/cif.o: src/cif.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/cif.o src/cif.cpp

${OBJECTDIR}/src/wxcircuit.o: src/wxcircuit.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxcircuit.o src/wxcircuit.cpp

${OBJECTDIR}/src/verilog.o: src/verilog.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/verilog.o src/verilog.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} build/bin/Astran.app/Contents/MacOS/Astran

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
