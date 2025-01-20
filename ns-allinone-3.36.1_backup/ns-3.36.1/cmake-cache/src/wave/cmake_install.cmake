# Install script for directory: /home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "default")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/lib/libns3.36.1-wave-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so"
         OLD_RPATH "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-wave-default.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/helper/wave-bsm-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/helper/wave-bsm-stats.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/helper/wave-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/helper/wave-mac-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/helper/wifi-80211p-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/bsm-application.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/channel-coordinator.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/channel-manager.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/channel-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/default-channel-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/higher-tx-tag.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/ocb-wifi-mac.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/vendor-specific-action.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/vsa-manager.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/wave-frame-exchange-manager.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/wave/model/wave-net-device.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/include/ns3/wave-module.h"
    )
endif()

