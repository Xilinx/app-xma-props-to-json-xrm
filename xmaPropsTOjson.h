/*       
 * Copyright (C) 2019, Xilinx Inc - All rights reserved
 * XMA Properties To JSON 
 *                                    
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations 
 * under the License.
 */        
#ifndef _XMAPROPSTOJSON_H_
#define _XMAPROPSTOJSON_H_

//#include <string.h>
#include <stdio.h>
#include <syslog.h>
//#include <vector>
//#include <tuple>
//#include <string>

#include "/opt/xilinx/xrm/include/xrm.h"
#include "/opt/xilinx/xrm/include/xrm_error.h"
#include "/opt/xilinx/xrm/include/xrm_limits.h"
#include <xma.h>

#define MAX_CH_SIZE 4096

#ifdef __cplusplus
extern "C" {
#endif

  void convertXmaPropsToJson(void* props, char* funcName, char* jsonJob);

#ifdef __cplusplus
}
#endif

#endif //_XRM_U30_CALC_PERCENT_PLUGIN_HPP_
