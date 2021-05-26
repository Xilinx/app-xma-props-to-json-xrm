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
#include <boost/archive/basic_archive.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_serialization.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "xmaPropsTOjson.h"

namespace pt = boost::property_tree;
using namespace std;

/*------------------------------------------------------------------------------------------------------------------------------------------
 *
 * Convert XMA properties to json job description format
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
void convertXmaPropsToJson(void* props, char* funcName, char* jsonJob)
{

  int channel_load = 0;
  if (strcmp(funcName,"DECODER")==0) 
  {
    XmaDecoderProperties *dec_props;
    dec_props = (XmaDecoderProperties*) props; 

    pt::ptree dec_topTree, dec_resTree,dec_valTree;

    dec_valTree.put("function", funcName);
    dec_valTree.put("format", "H264");
    dec_valTree.put("channel-load", channel_load);
    dec_valTree.put("resolution.input.width", dec_props->width);
    dec_valTree.put("resolution.input.height", dec_props->height);
    dec_valTree.put("resolution.input.frame-rate.num", dec_props->framerate.numerator);
    dec_valTree.put("resolution.input.frame-rate.den", dec_props->framerate.denominator);

    dec_resTree.push_back(std::make_pair("",dec_valTree));

    dec_topTree.add_child("request.parameters.resources",dec_resTree);

    std::stringstream json_str;
    boost::property_tree::write_json(json_str, dec_topTree);
    strncpy(jsonJob, json_str.str().c_str(), XRM_MAX_PLUGIN_FUNC_PARAM_LEN - 1);

    //cout <<"--------Decoder Build json:\n"<<jsonJob<<"\n";

  }
  else if (strcmp(funcName,"SCALER")==0) 
  {
    XmaScalerProperties *scal_props;
    scal_props = (XmaScalerProperties*) props; 

    pt::ptree scal_topTree, scal_resTree, scal_valTree, scal_outTree, scal_subTree;

    scal_valTree.put("function", funcName);
    scal_valTree.put("format", scal_props->input.format);
    scal_valTree.put("channel-load", channel_load);
    scal_valTree.put("resolution.input.width", scal_props->input.width);
    scal_valTree.put("resolution.input.height", scal_props->input.height);
    scal_valTree.put("resolution.input.frame-rate.num", scal_props->input.framerate.numerator);
    scal_valTree.put("resolution.input.frame-rate.den", scal_props->input.framerate.denominator);

    for (int i=0; i < scal_props->num_outputs; ++i) {
       scal_subTree.put("width", scal_props->output[i].width);
       scal_subTree.put("height", scal_props->output[i].height);
       scal_subTree.put("frame-rate.num", scal_props->output[i].framerate.numerator);
       scal_subTree.put("frame-rate.den", scal_props->output[i].framerate.denominator);
       scal_outTree.push_back(std::make_pair("", scal_subTree));
       scal_subTree.clear();
    }

    scal_valTree.add_child("resolution.output",scal_outTree);

    scal_resTree.push_back(std::make_pair("",scal_valTree));

    scal_topTree.add_child("request.parameters.resources",scal_resTree);

    std::stringstream json_str;
    boost::property_tree::write_json(json_str, scal_topTree);
    strncpy(jsonJob, json_str.str().c_str(), XRM_MAX_PLUGIN_FUNC_PARAM_LEN - 1);

    //cout <<"--------Scaler Build json\n"<<jsonJob<<"\n";

  }
  else if (strcmp(funcName,"ENCODER")==0) 
  {
    int lookahead = 0;
    XmaEncoderProperties *enc_props;
    enc_props = (XmaEncoderProperties*) props; 

    pt::ptree enc_topTree, enc_resTree,enc_valTree;

    enc_valTree.put("function", funcName);
    enc_valTree.put("format", "H264");
    enc_valTree.put("channel-load", channel_load);
    enc_valTree.put("lookahead-load", lookahead);
    enc_valTree.put("resolution.input.width", enc_props->width);
    enc_valTree.put("resolution.input.height", enc_props->height);
    enc_valTree.put("resolution.input.frame-rate.num", enc_props->framerate.numerator);
    enc_valTree.put("resolution.input.frame-rate.den", enc_props->framerate.denominator);
    enc_resTree.push_back(std::make_pair("",enc_valTree));

    enc_topTree.add_child("request.parameters.resources",enc_resTree);

    std::stringstream json_str;
    boost::property_tree::write_json(json_str, enc_topTree);
    strncpy(jsonJob, json_str.str().c_str(), XRM_MAX_PLUGIN_FUNC_PARAM_LEN - 1);

    //cout <<"--------Encoder Build json\n"<<jsonJob<<"\n";

  } 
  else if (strcmp(funcName,"LOOKAHEAD")==0) 
  {
    XmaFilterProperties *filter_props;
    filter_props = (XmaFilterProperties*) props; 
    int lookahead = 0;

    pt::ptree filter_topTree, filter_resTree,filter_valTree;

    filter_valTree.put("function", "ENCODER"); 
    filter_valTree.put("format", "H264");
    filter_valTree.put("channel-load", channel_load);
    filter_valTree.put("lookahead-load", lookahead);
    filter_valTree.put("resolution.input.width", filter_props->input.width);
    filter_valTree.put("resolution.input.height", filter_props->input.height);
    filter_valTree.put("resolution.input.frame-rate.num", filter_props->input.framerate.numerator);
    filter_valTree.put("resolution.input.frame-rate.den", filter_props->input.framerate.denominator);
    filter_resTree.push_back(std::make_pair("",filter_valTree));

    filter_topTree.add_child("request.parameters.resources",filter_resTree);

    std::stringstream json_str;
    boost::property_tree::write_json(json_str, filter_topTree);
    strncpy(jsonJob, json_str.str().c_str(), XRM_MAX_PLUGIN_FUNC_PARAM_LEN - 1);
    //cout <<"--------LOOKAHEAD Build json\n"<<jsonJob<<"\n";

  }
}

