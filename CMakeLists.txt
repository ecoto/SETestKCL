PROJECT(Negotiation)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Set executable output path
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# Set source code files
SET(SOURCES "${CMAKE_SOURCE_DIR}/src/Negotiation.cpp" "${CMAKE_SOURCE_DIR}/src/NegotiationRecorder.cpp"  "${CMAKE_SOURCE_DIR}/src/tinyfiledialogs.c")
SET(HEADER_FILES "${CMAKE_SOURCE_DIR}/src/NegotiationRecorder.h"  "${CMAKE_SOURCE_DIR}/src/tinyfiledialogs.h")

# If windows, we need to add the includes and the libs for the pthread library.
IF(WIN32)
    INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/win/")
    LINK_DIRECTORIES( "${CMAKE_SOURCE_DIR}/lib/win/x64/" )
ELSE()
    # If NOT windows, offer the option of using the ncurses library
    OPTION (USE_NCURSES_LIB "Use ncurses library (optional for Linux)" OFF)
    IF(USE_NCURSES_LIB)
        # if we are using the ncurses library, then make sure to find it and use it
        ADD_DEFINITIONS(-DUSE_NCURSES_LIB)
        FIND_PACKAGE(Curses REQUIRED)
        INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})
    ENDIF()
ENDIF()

# Define executable target
ADD_EXECUTABLE(Negotiation  ${SOURCES} ${HEADER_FILES} )

# If Linux, add the linker options for the pthread library
IF("${CMAKE_SYSTEM}" MATCHES "Linux")
    SET( GPP_LINK_FLAGS "-std=c++11 -pthread ")
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GPP_LINK_FLAGS}" )
ENDIF()

# If using ncurses, add the libraries to the target
IF(USE_NCURSES_LIB)
    TARGET_LINK_LIBRARIES(Negotiation ${CURSES_LIBRARIES})
ENDIF()

# if Windows, add the library dependency, and define the INSTALL project to deploy the DLL for pthread in the appropiate directory
IF(WIN32)
    TARGET_LINK_LIBRARIES(Negotiation  "pthreadVC2.lib" )
    INSTALL(FILES "${CMAKE_SOURCE_DIR}/dll/win/x64/pthreadVC2.dll" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/\${CMAKE_INSTALL_CONFIG_NAME}/")
ENDIF()
