#ifndef JSON_HANDLER_H_
#define JSON_HANDLER_H_

#include <bits/stdc++.h>    // for std::unordered_set
#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *
#include <unordered_map>    // for std::unordered_map

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp> 

using std::unordered_map;
using std::string;

namespace rakan {

class JsonHandler {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Constructors and destructors
  /////////////////////////////////////////////////////////////////////////////

  // Default constructor. Creates no internal json object and can be
  // used to create json objects.
  JsonHandler() {}

  // Takes in a JSON file to read contents for user.
  //
  // Arguments:
  //  - contents: The name of the json file to be interpreted.
  JsonHandler(char* contents, size_t size);

  // Default destructor.
  ~JsonHandler();

  /////////////////////////////////////////////////////////////////////////////
  // Accessors
  /////////////////////////////////////////////////////////////////////////////

  string GetState() const { return state_; }

  string GetJobID() const { return job_ID_; }

  unordered_map<uint32_t, uint32_t> GetMap() const { return map_; }

  double GetAlpha() const { return _alpha; }

  double GetBeta() const { return _beta; }

  double GetGamma() const { return gamma_; }
  
  double GetEta() const { return eta_; }

 private:
  // State name
  string state_;
  // Identifier for the job
  string job_ID_;

  // Mapping of the following format: Map<PrecinctID, DistrictID>
  unordered_map<uint32_t, uint32_t> map_;


  // Metric parameters.
  double alpha_;
  double beta_;
  double gamma_;
  double eta_;
};        // class JsonHandler

}         // namespace rakan

#endif    // JSON_HANDLER_H_
