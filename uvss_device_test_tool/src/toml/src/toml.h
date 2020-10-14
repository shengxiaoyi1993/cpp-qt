/* 简单的toml文件读写
 *
 * 注意：
 * 不支持数组
 * 不支持多层对象
 *
 * 一个可以的范例：
 * product = 1
 * remain_days = 60
 *
 * 允许以“#”开头的注释
 *
 * 支持的数据类型：int,double,bool,string
 *
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Toml {
 public:
  Toml(string file);
  ~Toml();

  int Get(string key, int &v);
  int Get(string key, long int &v);
  int Get(string key, double &v);
  int Get(string key, bool &v);
  int Get(string key, string &v);

  void Set(string key, int v);
  void Set(string key, long int v);
  void Set(string key, double v);
  void Set(string key, bool v);
  void Set(string key, string v);
  void Set(string key, const char *v);

  string toString();
  int Save(string file);

 private:
  map<string, string> map_of_data;
  bool Exist(string key);
};
