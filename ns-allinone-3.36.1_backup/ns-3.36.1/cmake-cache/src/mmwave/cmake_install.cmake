# Install script for directory: /home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/lib/libns3.36.1-mmwave-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so"
         OLD_RPATH "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.36.1-mmwave-default.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-phy-trace.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-point-to-point-epc-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-bearer-stats-calculator.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mc-stats-calculator.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/core-network-stats-calculator.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-bearer-stats-connector.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/helper/mmwave-mac-trace.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-net-device.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-enb-net-device.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-ue-net-device.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-phy.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-enb-phy.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-ue-phy.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-spectrum-phy.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-spectrum-value-helper.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-interference.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-chunk-processor.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-phy-mac-common.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-control-messages.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-spectrum-signal-parameters.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-radio-bearer-tag.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-amc.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac-sched-sap.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac-csched-sap.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-phy-sap.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-enb-mac.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-ue-mac.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-rrc-protocol-ideal.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-lte-rrc-protocol-real.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac-pdu-header.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-mac-pdu-tag.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-harq-phy.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-flex-tti-mac-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-flex-tti-maxweight-mac-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-flex-tti-maxrate-mac-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-flex-tti-pf-mac-scheduler.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-propagation-loss-model.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mc-ue-net-device.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-component-carrier.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-component-carrier-ue.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-component-carrier-enb.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-no-op-component-carrier-manager.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/mmwave-beamforming-model.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/beamforming-codebook.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/file-beamforming-codebook.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-error-model.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-lte-mi-error-model.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-cc-t1.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-cc-t2.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-cc.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-error-model.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-ir-t1.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-ir-t2.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-ir.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-t1.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/src/mmwave/model/error-model/mmwave-eesm-t2.h"
    "/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/build/include/ns3/mmwave-module.h"
    )
endif()

