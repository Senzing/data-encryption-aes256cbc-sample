
project(G2_ENCRYPTION_PLUGINS C) 
cmake_minimum_required(VERSION 2.8)


# Where do we install?
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist)
set(INSTALL_LIB_DIR "/lib" CACHE INTERNAL
    "Default directory for installing libraries." )
set(INSTALL_INCLUDE_DIR "/include" CACHE INTERNAL
    "Default directory for installing source code header files." )

#
# We want details... not summaries
#
set(CMAKE_VERBOSE_MAKEFILE "ON")


message(STATUS "BUILD_TYPE:          [${CMAKE_BUILD_TYPE}]")
message(STATUS "SYSTEM_NAME:         [${CMAKE_SYSTEM_NAME}]")
message(STATUS "SYSTEM_PROCESSOR:    [${CMAKE_SYSTEM_PROCESSOR}]")
message(STATUS "SYSTEM_VERSION:      [${CMAKE_SYSTEM_VERSION}]")
message(STATUS "HOST_SYSTEM:         [${CMAKE_HOST_SYSTEM}]")
message(STATUS "SYSTEM:              [${CMAKE_SYSTEM}]")

#
# Determine Linux Distribution and OS Version
#
set(LINUX_PLATFORM "" CACHE
    STRING "The fully specified platform, for example REDHAT5 or SUSE10."
    FORCE )

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" )
  set(RAW_LINUX_DISTRIBUTION)
  set(LINUX_DISTRIBUTION)
  set(LINUX_VERSION)

  # First figure out the linux distribution on this build machine
  exec_program(lsb_release ARGS -is OUTPUT_VARIABLE RAW_LINUX_DISTRIBUTION)

  if ( ${RAW_LINUX_DISTRIBUTION} MATCHES "^SUSE" )
    set(LINUX_DISTRIBUTION "SUSE")
  endif ()

  if ( ${RAW_LINUX_DISTRIBUTION} MATCHES "^RedHat" )
    set(LINUX_DISTRIBUTION "REDHAT")
  endif ()

  if ( ${RAW_LINUX_DISTRIBUTION} MATCHES "^CentOS" )
    set(LINUX_DISTRIBUTION "CENTOS")
  endif ()

  if ( ${RAW_LINUX_DISTRIBUTION} MATCHES "^AmazonAMI" )
    set(LINUX_DISTRIBUTION "AMAZON")
  endif ()

  message(STATUS "LINUX DISTRIBUTION:  [${LINUX_DISTRIBUTION}]")

  # Now, get the OS Version
  exec_program(lsb_release ARGS -rs OUTPUT_VARIABLE LINUX_VERSION)
  message(STATUS "LINUX VERSION:       [${LINUX_VERSION}]")

  # Treat Ubuntu and Debian linux as if they were Redhat 5
  # RJR: Cannot do this anymore, because Power8 also runs the Ubuntu OS.
  # Different arch - need to take processor into account.
  if (("${RAW_LINUX_DISTRIBUTION}" STREQUAL "Ubuntu" OR "${RAW_LINUX_DISTRIBUTION}"
      STREQUAL "Debian") AND "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    set(LINUX_DISTRIBUTION "REDHAT")
    set(LINUX_VERSION "5")
  endif ()


  #Ubuntu on Power 8 Arch.
  if ("${RAW_LINUX_DISTRIBUTION}" STREQUAL "Ubuntu" AND "${CMAKE_SYSTEM_PROCESSOR}"
STREQUAL "ppc64le")
        set (LINUX_DISTRIBUTION "UBUNTU_PWR8_V")
        string(REPLACE "." ";" VERSION_LIST ${LINUX_VERSION})
        list(GET VERSION_LIST 0 LINUX_VERSION)
  endif ()

#Redhat on Power 8 Arch.
  if ("${LINUX_DISTRIBUTION}" STREQUAL "REDHAT" AND "${CMAKE_SYSTEM_PROCESSOR}"
  STREQUAL "ppc64le")
        set (LINUX_DISTRIBUTION "REDHAT_PWR8_V")
        string(REPLACE "." ";" VERSION_LIST ${LINUX_VERSION})
        list(GET VERSION_LIST 0 LINUX_VERSION)
  endif ()

  # Treat Ubuntu and Debian linux as if they were Redhat 5
  if ( "${RAW_LINUX_DISTRIBUTION}" STREQUAL "Ubuntu" OR
       "${RAW_LINUX_DISTRIBUTION}" STREQUAL "Debian" )
    set(LINUX_DISTRIBUTION "REDHAT")
    set(LINUX_VERSION "5")
  endif ( "${RAW_LINUX_DISTRIBUTION}" STREQUAL "Ubuntu" OR
          "${RAW_LINUX_DISTRIBUTION}" STREQUAL "Debian" )
          
  #Ubuntu on Power 8 Arch.
  if ("${RAW_LINUX_DISTRIBUTION}" STREQUAL "Ubuntu" AND "${CMAKE_SYSTEM_PROCESSOR}"
  STREQUAL "ppc64le")
    set (LINUX_DISTRIBUTION "UBUNTU_PWR8_V")
    string(REPLACE "." ";" VERSION_LIST ${LINUX_VERSION})
    list(GET VERSION_LIST 0 LINUX_VERSION)
  endif ()

   #Redhat on Power 8 Arch.
  if ("${LINUX_DISTRIBUTION}" STREQUAL "REDHAT" AND "${CMAKE_SYSTEM_PROCESSOR}"
  STREQUAL "ppc64le")
    set (LINUX_DISTRIBUTION "REDHAT_PWR8_V")
    string(REPLACE "." ";" VERSION_LIST ${LINUX_VERSION})
    list(GET VERSION_LIST 0 LINUX_VERSION)
   endif ()


  # Output platform messages
  message (STATUS "LINUX FLAVOR:        [${RAW_LINUX_DISTRIBUTION}]")
  message (STATUS "LINUX VERSION:       [${LINUX_VERSION}]")

  # Finally, cruft up the full linux platform name.
  set(LINUX_PLATFORM ${LINUX_DISTRIBUTION}${LINUX_VERSION})
  message(STATUS "LINUX PLATFORM:      [${LINUX_PLATFORM}]")
endif ()


####################################
## Find required library paths
####################################


set(OPENSSL_VERSION 1.0.2o)
if ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  set(OPENSSL_VERSION 1.0.2l)
endif ()
if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  set(USE_OPENSSL TRUE)
endif ()
if ( "${USE_OPENSSL}" STREQUAL "TRUE" )
  find_path(OPENSSL_ROOT "include/openssl/rsa.h"
	/usr/local/Cellar/openssl/1.0.2o_1
          NO_DEFAULT_PATH
	  DOC "Path to OPENSSL installation" )
endif ()


message(STATUS "OPENSSL_ROOT:        [${OPENSSL_ROOT}]")


#############################################
### Global Platform Compiler Definitions
#############################################

#################
# WINDOWS
#################
if ( WIN32 )
  add_definitions(-DWIN32API)
  add_definitions(-DNOMINMAX)
  add_definitions(/wd4091)
  add_definitions(/wd4251)
  add_definitions(/wd4800)
  add_definitions(/wd4996)
  add_definitions(/WX)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /EHsc")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /GS /RTC1" )
endif ()


#################
# macOS gcc SPECIFIC
#################
if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_TR1_TUPLE=0 -Wall -Werror ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror ")
  set(LINK_FLAGS "${LINK_FLAGS} -flat_namespace  -multiply_defined error ")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -fPIC ")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-ms-compat ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility-ms-compat ")

  set(CMAKE_CXX_FLAGS_DEBUG "-Og -ggdb -fno-omit-frame-pointer " CACHE STRING "" FORCE)

  if ( "${PROFILE}" STREQUAL "TRUE" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg ")
  endif ()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")

  if ( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "86$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${CMAKE_SYSTEM_PROCESSOR}")
  endif ()

  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -gline-tables-only " CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -gline-tables-only " CACHE STRING "" FORCE)

endif ()

#################
# Linux gcc SPECIFIC
#################
if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -ansi")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -ansi")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -fPIC ")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-pragmas -D_GLIBCXX_USE_CXX11_ABI=0 ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden ")

  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 -fno-omit-frame-pointer " CACHE STRING "" FORCE)

  if ( "${PROFILE}" STREQUAL "TRUE" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg ")
  endif ()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")

  if ( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "86$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${CMAKE_SYSTEM_PROCESSOR}")
  endif ()

  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g " CACHE STRING "" FORCE)

endif ()

# add local flags
# Local flags may be added either through environment variables
# or by including something like:
#  -DLOCAL_CXX_FLAGS:string="..."
# to the CMake command line.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOCAL_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LOCAL_C_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LOCAL_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${LOCAL_C_FLAGS_DEBUG}")


########################################################################################
# SET INCLUDE DIRECTORIES
########################################################################################  


########################################################################################
# SET LIBRARY PATHS
########################################################################################

if ( USE_OPENSSL )
  include_directories(${OPENSSL_ROOT}/include)
  link_directories(${OPENSSL_ROOT}/lib)
endif ()


#############################################
### Misc Variable Definitions
#############################################
if ( WIN32 )
  set(OBJECT_SUFFIX .obj)
else ()
  set(OBJECT_SUFFIX .o)
endif ()

#############################################
### Linking setup
#############################################

include_directories(${CMAKE_SOURCE_DIR}/senzing-data-encryption-specification/src)

if ( "${USE_OPENSSL}" STREQUAL "TRUE" )
  if ( WIN32 )
    set (OPENSSL_LIBS libeay32MD ssleay32MD)
  else ()
    set (OPENSSL_LIBS 4758cca aep atalla capi chil cswift gmp gost libeay32 nuron padlock ssleay32 sureware ubsec)
  endif ()
endif ()

if ( WIN32 )
  set(LINK_FLAGS ${LINK_FLAGS} " /FORCE:MULTIPLE /STACK:2048000")
  set(OSCORELIBS advapi32 comsupp comsuppw ws2_32 DbgHelp ${OPENSSL_LIBS})
else ()
  if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" )
    set(OSCORELIBS crypto ssl)
  else ()
    set(OSCORELIBS rt pthread dl crypto ssl)
  endif ()
endif ()


#############################################
### Recursive work on subdirectories
#############################################

add_subdirectory(encryptionPlugins)

# testing binary
enable_testing()

add_executable(testPlainTextPlugin testPlainTextPlugin.c)
target_link_libraries(testPlainTextPlugin g2EncryptDataClearText)
add_test(NAME testPlainTextPlugin COMMAND testPlainTextPlugin)

add_executable(testEncPlugin testEncPlugin.c)
target_link_libraries(testEncPlugin g2EncryptDataAES256CBC)
add_test(NAME testEncPlugin COMMAND testEncPlugin)

