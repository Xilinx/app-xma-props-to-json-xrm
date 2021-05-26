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
project(xmaPropsTOjson)
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/lib/pkgconfig")

# Linking variables
find_package(PkgConfig REQUIRED)
#pkg_check_modules(XRT REQUIRED xrt)
pkg_check_modules(XMA REQUIRED libxma2api)

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --stats -Bsymbolic")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O1 -g  -DDSA64 -fpermissive")

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

# Packaging section
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Rohit Athavale <rohit.athavale@xilinx.com>")
set(CPACK_PACKAGE_VENDOR "Xilinx")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Xilinx MPSoC XMA Props To Json Plugin")
set(CPACK_PACKAGE_DESCRIPTION "The Xilinx MPSoC XMA properties to json plugin")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "xrt (>= 2.10.37) ")
set(CPACK_RPM_PACKAGE_DEPENDS "xrt >= 2.10.37 ")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/opt" "/opt/xilinx" "/usr/lib" "/usr/lib/pkgconfig")
include(CPack)
