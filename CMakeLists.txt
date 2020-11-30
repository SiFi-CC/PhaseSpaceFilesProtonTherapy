# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


project(SlicePhaseSpaceProject)

SET(CMAKE_INSTALL_PREFIX /home/home2/institut_3b/kasper/geant4_workdir)
message(STATUS "Install path " ${CMAKE_INSTALL_PREFIX})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS} ${CMAKE_CURRENT_SOURCE_DIR})


message(STATUS "Found CXXCompiler version " ${CMAKE_CXX_COMPILER_VERSION})


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 10.6.2 EXACT REQUIRED ui_all vis_all)
else()
  find_package(Geant4 10.6.2 EXACT REQUIRED)
endif()

message( STATUS "Found Geant4 version " ${Geant4_VERSION} )


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#message(STATUS ${CMAKE_PREFIX_PATH})

find_package( ROOT REQUIRED COMPONENTS Gui Core) 
message( STATUS "Found ROOT version " ${ROOT_VERSION} ) 

include(${ROOT_USE_FILE})

include(makeRootDict)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS} )


#---------------------------------------------------------------------------
#create extra libraries

MAKE_ROOT_DICT(DICTNAME stdfile LINKDEF include/stdLinkDef.h SOURCES src/HistoManager.cc INCLUDES include/stdinclude.hh LIBRARIES ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )
#message( STATUS "RTIMEPATH " ${CMAKE_INSTALL_RPATH} )


add_executable(SPS SPS.cc ${sources} ${headers})
target_link_libraries(SPS stdfile ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )



# when building, don't use the install RPATH already
# (but later on when installing)



#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SPS_SCRIPTS
  macros/vis_SPS.mac
  macros/Run180MeV.mac
  macros/Viewer.mac
  macros/Run6MeV.mac
  macros/Run8MeV.mac
  macros/Run10MeV.mac
  macros/Run12MeV.mac
  macros/Run15MeV.mac
  macros/Run18MeV.mac
  macros/Run20MeV.mac
  macros/Run22MeV.mac
  macros/Run25MeV.mac
  macros/Run27MeV.mac
  macros/Run30MeV.mac
  )
 

foreach(_script ${SPS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(SlicePhaseSpaceProject DEPENDS SPS)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
set_property(TARGET SPS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
install(TARGETS SPS RUNTIME DESTINATION bin/SPS)
install(FILES ${SPS_SCRIPTS} DESTINATION bin/SPS/macros)


