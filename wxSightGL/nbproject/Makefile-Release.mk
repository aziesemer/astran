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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/app.o \
	${OBJECTDIR}/src/basicglpane.o \
	${OBJECTDIR}/src/interface/interface.o \
	${OBJECTDIR}/src/interface/layeritem.o \
	${OBJECTDIR}/src/sight/camera.o \
	${OBJECTDIR}/src/sight/fileformats/BinaryFileReader.o \
	${OBJECTDIR}/src/sight/fileformats/gds.o \
	${OBJECTDIR}/src/sight/io_files.o \
	${OBJECTDIR}/src/sight/label.o \
	${OBJECTDIR}/src/sight/layer.o \
	${OBJECTDIR}/src/sight/model.o \
	${OBJECTDIR}/src/sight/opengl.o \
	${OBJECTDIR}/src/sight/rectangle.o \
	${OBJECTDIR}/src/sight/repository.o \
	${OBJECTDIR}/src/sight/ruler.o \
	${OBJECTDIR}/src/sight/sight.o \
	${OBJECTDIR}/src/sight/states/drag.o \
	${OBJECTDIR}/src/sight/states/lbl.o \
	${OBJECTDIR}/src/sight/states/line.o \
	${OBJECTDIR}/src/sight/states/rulerst.o \
	${OBJECTDIR}/src/sight/states/select.o \
	${OBJECTDIR}/src/sight/states/sqr.o \
	${OBJECTDIR}/src/sight/states/wire.o \
	${OBJECTDIR}/src/sight/txGpu.o \
	${OBJECTDIR}/src/sight/wire_line.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`wx-config --static=no --debug=no --cppflags` 
CXXFLAGS=`wx-config --static=no --debug=no --cppflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`wx-config --static=no --debug=no --libs --gl-libs` -lGL -lGLU -lglut -lGLEW  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wxsightgl

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wxsightgl: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wxsightgl ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/app.o: src/app.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/app.o src/app.cpp

${OBJECTDIR}/src/basicglpane.o: src/basicglpane.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/basicglpane.o src/basicglpane.cpp

${OBJECTDIR}/src/interface/interface.o: src/interface/interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/interface
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/interface/interface.o src/interface/interface.cpp

${OBJECTDIR}/src/interface/layeritem.o: src/interface/layeritem.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/interface
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/interface/layeritem.o src/interface/layeritem.cpp

${OBJECTDIR}/src/sight/camera.o: src/sight/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/camera.o src/sight/camera.cpp

${OBJECTDIR}/src/sight/fileformats/BinaryFileReader.o: src/sight/fileformats/BinaryFileReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/fileformats
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/fileformats/BinaryFileReader.o src/sight/fileformats/BinaryFileReader.cpp

${OBJECTDIR}/src/sight/fileformats/gds.o: src/sight/fileformats/gds.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/fileformats
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/fileformats/gds.o src/sight/fileformats/gds.cpp

${OBJECTDIR}/src/sight/io_files.o: src/sight/io_files.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/io_files.o src/sight/io_files.cpp

${OBJECTDIR}/src/sight/label.o: src/sight/label.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/label.o src/sight/label.cpp

${OBJECTDIR}/src/sight/layer.o: src/sight/layer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/layer.o src/sight/layer.cpp

${OBJECTDIR}/src/sight/model.o: src/sight/model.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/model.o src/sight/model.cpp

${OBJECTDIR}/src/sight/opengl.o: src/sight/opengl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/opengl.o src/sight/opengl.cpp

${OBJECTDIR}/src/sight/rectangle.o: src/sight/rectangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/rectangle.o src/sight/rectangle.cpp

${OBJECTDIR}/src/sight/repository.o: src/sight/repository.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/repository.o src/sight/repository.cpp

${OBJECTDIR}/src/sight/ruler.o: src/sight/ruler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/ruler.o src/sight/ruler.cpp

${OBJECTDIR}/src/sight/sight.o: src/sight/sight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/sight.o src/sight/sight.cpp

${OBJECTDIR}/src/sight/states/drag.o: src/sight/states/drag.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/drag.o src/sight/states/drag.cpp

${OBJECTDIR}/src/sight/states/lbl.o: src/sight/states/lbl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/lbl.o src/sight/states/lbl.cpp

${OBJECTDIR}/src/sight/states/line.o: src/sight/states/line.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/line.o src/sight/states/line.cpp

${OBJECTDIR}/src/sight/states/rulerst.o: src/sight/states/rulerst.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/rulerst.o src/sight/states/rulerst.cpp

${OBJECTDIR}/src/sight/states/select.o: src/sight/states/select.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/select.o src/sight/states/select.cpp

${OBJECTDIR}/src/sight/states/sqr.o: src/sight/states/sqr.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/sqr.o src/sight/states/sqr.cpp

${OBJECTDIR}/src/sight/states/wire.o: src/sight/states/wire.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight/states
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/states/wire.o src/sight/states/wire.cpp

${OBJECTDIR}/src/sight/txGpu.o: src/sight/txGpu.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/txGpu.o src/sight/txGpu.cpp

${OBJECTDIR}/src/sight/wire_line.o: src/sight/wire_line.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/sight
	${RM} $@.d
	$(COMPILE.cc) -O2 `wx-config --static=no --debug=no --cflags --gl-libs`   -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sight/wire_line.o src/sight/wire_line.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wxsightgl

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
