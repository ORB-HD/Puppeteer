
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)

PROJECT(QtPropertyBrowser)

OPTION(QtPropertyBrowser_BUILD_EXAMPLES "Build examples" OFF)

#-----------------------------------------------------------------------------
# Output directories.
#
SET(output_dir ${QtPropertyBrowser_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
# Qt
#
FIND_PACKAGE (Qt5Widgets REQUIRED)
FIND_PACKAGE (Qt5Core)
 
INCLUDE_DIRECTORIES (${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${Qt5Widgets_INCLUDE_DIRS})

#-----------------------------------------------------------------------------
# Subdirectories
#
ADD_SUBDIRECTORY(src)
IF(QtPropertyBrowser_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF()

#-----------------------------------------------------------------------------
# Generate QtPropertyBrowserConfig file
#
CONFIGURE_FILE(UseQtPropertyBrowser.cmake.in
               ${QtPropertyBrowser_BINARY_DIR}/UseQtPropertyBrowser.cmake COPYONLY)

INCLUDE(GenerateQtPropertyBrowserConfig.cmake)
