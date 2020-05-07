
#include "Json_Handler.h"

using std::unordered_map;
using std::string;

namespace rakan {

JsonHandler::JsonHandler(char* contents, size_t size) {
  string converted = convertToString(contents + 1, size - 1);
  vector<string> result; 
  // Find all of the simple values first

  // Be sure to possible to change the array indexing if it looks different
  size_t pos = contents.find("GUID");
  string guid = "";
  while (converted[pos + 6] != ' ') {
    guid += converted[pos + 6];
    pos++;
  }

  pos = contents.find("State");
  string state = "";
  while (converted[pos + 7] != ' ') {
    state += converted[pos + 7];
    pos++;
  }
  pos = contents.find("alpha");
  string alpha = "";
  while (converted[pos + 7] != ' ') {
    alpha += converted[pos + 7];
    pos++;
  }
  alpha_ = 
  pos = contents.find("beta");
  string beta = "";
  while (converted[pos + 6] != ' ') {
    beta += converted[pos + 7];
    pos++;
  }
  pos = contents.find("gamma");
  string gamma = "";
  while (converted[pos + 7] != ' ') {
    gamma += converted[pos + 7];
    pos++;
  }
  pos = contents.find("eta");
  string eta = "";
  while (converted[pos + 5] != ' ') {
    eta += converted[pos + 7];
    pos++;
  }
  boost::split(result, input, boost::is_any_of("")); 

  for (int i = 0; i < result.size(); i++) 
    cout << result[i] << endl; 
  return 0;
}

string convertToString(char* contents, size_t size) 
{ 
    size_t i; 
    string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + contents[i]; 
    } 
    return s; 
} 
}     // namespace rakan
