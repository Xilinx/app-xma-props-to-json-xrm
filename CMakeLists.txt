# Copyright (C) 2021, Xilinx Inc - All rights reserved
# XMA Properties To JSON 
                                   
# Licensed under the Apache License, Version 2.0 (the "License"). You may
# not use this file except in compliance with the License. A copy of the
# License is located at

#    http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations 
# under the License.

cmake_minimum_required(VERSION 2.8.11)
include (CMakeDependentOption)
set(XMA_PROPS_TO_JSON_PROJ "xilinx-u30-xmapropstojson")
set(XMA_PROPS_TO_JSON_VERSION "1.0.2")

find_program(UNAME_EXEC uname)
find_program(LSB_RELEASE_EXEC lsb_release)

execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
    OUTPUT_VARIABLE LINUX_FLAVOR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (${LINUX_FLAVOR} MATCHES "^(Ubuntu|Debian)")
        # amd64
        execute_process(
        COMMAND dpkg --print-architecture
        OUTPUT_VARIABLE CPACK_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
elseif (${LINUX_FLAVOR} MATCHES "^(RedHat|CentOS|Amazon|Fedora|SUSE)")
        # x86_64
        execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE CPACK_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
else()
        message( SEND_ERROR "Trying to build on incompatible system \"${LINUX_FLAVOR}\"." )
endif()

# Grab OS release short name
execute_process(COMMAND ${LSB_RELEASE_EXEC} -rs
	OUTPUT_VARIABLE LSB_RELEASE_REL_SHORT
	OUTPUT_STRIP_TRAILING_WHITESPACE)

if (${LINUX_FLAVOR} MATCHES "^Amazon")
	set(LSB_RELEASE_REL_SHORT "amzn${LSB_RELEASE_REL_SHORT}")
endif()
set(XMA_PROPS_TO_JSON_PROJ_FULL_NAME "${XMA_PROPS_TO_JSON_PROJ}_${XMA_PROPS_TO_JSON_VERSION}_${LSB_RELEASE_REL_SHORT}-${CPACK_ARCH}")

project(${XMA_PROPS_TO_JSON_PROJ})

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/lib/pkgconfig")

# Linking variables
find_package(PkgConfig REQUIRED)
#pkg_check_modules(XRT REQUIRED xrt)
pkg_check_modules(XMA REQUIRED libxma2api)

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --stats -Bsymbolic")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -DDSA64 -fpermissive")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_FILE_LICENSE_PATH "/usr/share/doc/xilinx/u30")

# Plugin
add_library(xmaPropsTOjson SHARED
	xmaPropsTOjson.cpp
)

include_directories(.)

target_compile_options(xmaPropsTOjson
	PUBLIC	${XMA_CFLAGS}
)

target_include_directories(xmaPropsTOjson
	PUBLIC ${XRT_INCLUDE_DIRS}/xma2
	PUBLIC ${XRT_INCLUDE_DIRS}
)

# Set the location for library installation
install(TARGETS xmaPropsTOjson DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin)
install(FILES ${CPACK_RESOURCE_FILE_LICENSE} CONFIGURATIONS Release RUNTIME DESTINATION ${CPACK_FILE_LICENSE_PATH}/${XMA_PROPS_TO_JSON_PROJ})

# Packaging section
set(CPACK_PACKAGE_NAME ${XMA_PROPS_TO_JSON_PROJ})
set(CPACK_PACKAGE_VERSION ${XMA_PROPS_TO_JSON_VERSION})
set(CPACK_PACKAGE_FILE_NAME ${XMA_PROPS_TO_JSON_PROJ_FULL_NAME})
set(CPACK_PACKAGE_CONTACT "Rohit Athavale <rohit.athavale@xilinx.com>")
set(CPACK_PACKAGE_VENDOR "Xilinx")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Xilinx MPSoC XMA Props To Json Plugin")
set(CPACK_PACKAGE_DESCRIPTION "The Xilinx MPSoC XMA properties to json plugin")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "xrt (>= 2.11), xrm (>= 1.3) ")
set(CPACK_RPM_PACKAGE_REQUIRES "xrt >= 2.11, xrm >= 1.3, ")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/opt" "/opt/xilinx" "/usr/lib" "/usr/lib/pkgconfig" "/usr/share/doc/xilinx" "/usr/share/doc/xilinx/u30")
include(CPack)
