#include "toml.h"
#include<memory>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "../../cppgc/cppgc.h"

std::string &Trim(std::string &s) {
  if (s.empty()) {
    return s;
  }

  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

std::vector<std::string> string_split(const  std::string& s, const std::string& delim)
{
    std::vector<std::string> elems;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    while (pos < len)
    {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0)
        {
            elems.push_back(s.substr(pos, len - pos));
            break;
        }
        elems.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
    return elems;
}

string& string_replace(string& str, const string& to_replaced, const string& newchars)
{
    for(string::size_type pos(0); pos != string::npos; pos += newchars.length())
    {
        pos = str.find(to_replaced,pos);
        if(pos!=string::npos)
           str.replace(pos,to_replaced.length(),newchars);
        else
            break;
    }
    return   str;
}

Toml::Toml(string file) {
  FILE *fp = fopen(file.data(), "rb");

  std::cout <<"read toml file:" << file << std::endl;

  if (!fp) {
    cout << "error for open " << file << endl;
    return;
  }

  fseek(fp, 0,SEEK_END);
  size_t len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  AutoPtr buff(len+1);

  std::cout << "toml len = " << len << std::endl;

  char *s_char = (char *)(buff.Ptr());

  fread(s_char, len, 1, fp);
  fclose(fp);

  string sall = string(s_char);

  std::cout << "sall = " << sall << std::endl;

  string_replace(sall, "\r", "");

  std::vector<std::string> s_list = string_split(sall, "\n");

  for(int i = 0;i < s_list.size();i++) {
    std::string s = s_list[i];
    size_t index_of_j = s.find("#");

    if (index_of_j != string::npos) {
      continue;
    }

    int index_of_d = s.find_first_of("=");

    if (index_of_d != string::npos) {
      string key = s.substr(0, index_of_d);
      string value = s.substr(index_of_d + 1);
      key = Trim(key);
      value = Trim(value);
      map_of_data[key] = value;
    }
  }



}

Toml::~Toml() {}

int Toml::Get(string key, int &v) {
  int count = map_of_data.count(key);
  if (count == 0) {
    v = 0;
    return -1;
  }

  string value = map_of_data[key];
  
  try {
      v = std::stoi(value);
  } catch (...) {
      v = 0;
      return 0;
  }

  return 0;
}

int Toml::Get(string key, long int &v) {
  int count = map_of_data.count(key);
  if (count == 0) {
    v = 0;
    return -1;
  }

  string value = map_of_data[key];
  v = std::stol(value);
  return 0;
}

int Toml::Get(string key, double &v) {
  int count = map_of_data.count(key);
  if (count == 0) {
    v = 0;
    return -1;
  }

  string value = map_of_data[key];
  v = std::stof(value);
  return 0;
}

int Toml::Get(string key, bool &v) {
  int count = map_of_data.count(key);

  if (count == 0) {
    v = false;
    return -1;
  }

  string value = map_of_data[key];

  v = false;

  if (value == "true") {
    v = true;
  }

  return 0;
}

int Toml::Get(string key, string &v) {
  int count = map_of_data.count(key);

  if (count == 0) {
    v = "";
    return -1;
  }

  string value = map_of_data[key];

  if (value.find_first_of("\"") == 0 &&
      value.find_last_of("\"") == value.length() - 1) {
    v = value.substr(1, value.length() - 2);
  } else {
    v = "";
  }
  return 0;
}

void Toml::Set(string key, int v) { map_of_data[key] = std::to_string(v); }

void Toml::Set(string key, long int v) { map_of_data[key] = std::to_string(v); }

void Toml::Set(string key, double v) { map_of_data[key] = std::to_string(v); }

void Toml::Set(string key, bool v) { map_of_data[key] = v ? "true" : "false"; }

void Toml::Set(string key, string v) { map_of_data[key] = "\"" + v + "\""; }

void Toml::Set(string key, const char *v) {
  string value = v;
  map_of_data[key] = "\"" + value + "\"";
}

string Toml::toString() {
  string result = "";
  map<string, string>::iterator it;
  it = map_of_data.begin();

  while (it != map_of_data.end()) {
    // it->first;
    // it->second;
    result += it->first + " = " + it->second + "\n";

    it++;
  }

  return result;
}

int Toml::Save(string file) {
  ofstream fp(file, ios::out);
  if (!fp) {
    return -1;
  }

  fp << toString() << endl;

  fp.close();

  return 0;
}
