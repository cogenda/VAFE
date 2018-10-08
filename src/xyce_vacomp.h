#ifndef __xyce_vacomp_h
#define __xyce_vacomp_h
#include <vpi_user.h>
#if defined(__XVPI__)
  #include "xvpi.h"
#endif

#ifdef __cplusplus
extern "C" {
extern int verbose;
void CgenHeader(vpiHandle root);
void CgenImplement(vpiHandle root);
int vpi_gen_ccode (vpiHandle obj);
}

#include <assert.h>
#include <string>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
typedef std::string string_t;
typedef std::pair< string_t, string_t > strPair;
typedef std::vector < string_t > strVec;
typedef std::map < string_t, strVec > dictStrVec;
typedef std::map < string_t, string_t > strDict;

const int N_VARS_PER_COL=12;
const int INDENT_UNIT=2;
const string_t g_loopIncVar="__loop_incr_var";

std::map < int, string_t > va_c_type_map = {
  {vpiRealVar,    "double"},
  {vpiIntegerVar, "int"},
};

strDict va_c_expr_map = {
  {"ln",    "log"},
  {"log",   "log10"},
  {"sqrt",  "sqrt"},
  {"abs",   "fabs"},
  {"begin", "{"},
  {"end",   "}"},
  {"real",   "double"},
  {"integer","int"},
  {"$vt",    "_VT_"},
  {"$limit", "_LIMIT_"},
  {"$temperature", "_TEMPER_"},
  {"$strobe", "_STROBE_"},
};

strDict va_spice_unit_map = {
  {"T",     "e12"},
  {"G",     "e9"},
  {"M",     "e6"}, //alias for MEG
  {"K",     "e3"},
  {"k",     "e3"},
  {"m",     "e-3"},
  {"u",     "e-6"},
  {"n",     "e-9"},
  {"p",     "e-12"},
  {"f",     "e-15"},
  {"a",     "e-18"},
};

typedef enum _objSelection {
  toplevel = 0,
  vaPort,
  vaNet,
  vaAnaFun,
  vaVar,
  vaParam,
  vaSysFunCall,
  vaBultinFunCall,
  vaVFunCall,
  vaIFunCall,
  vaContrib,
  vaCondition,
  vaIf,    
  vaElseIf,
  vaElse,  
} objSelect;

const enum_description
  vpi_parameter_prop[] = {
  {vpiName, "vpiName"},
  {vpiType, "vpiOperation"},
  {xvpiValue, "xvpiValue"},
  {vpiParameter,"vpiParameter"}
};

const enum_description
  vpi_port_prop[] = {
  {vpiPort,"vpiPort"},
};

const enum_description
  vpi_condition_prop[] = {
  {vpiCondition,"vpiCondition"},
};

const enum_description
  vpi_Var_prop[] = {
  {vpiVariables, "vpiVariables" },
};

const enum_description
  vpi_net_prop[] = {
  {vpiNet,"vpiNet"},
};

const enum_description
  vpi_anafun_prop[] = {
  {vpiIODecl,"vpiIODecl"},
};

const enum_description
  vpi_If_prop[] = {
  {vpiStmt, "vpiIfElse" },
};

const enum_description
  vpi_ElseIf_prop[] = {
  {vpiElseStmt, "vpiIfElse" },
};

const enum_description
  vpi_Else_prop[] = {
  {vpiElseStmt, "vpiIfElse" },
};

const enum_description
  vpi_srccode_prop[] = {
  {vpiModule,"vpiModule"},
  {vpiBegin,"vpiBegin"},
  {vpiNamedBegin,"vpiNamedBegin"},
  {vpiNamedFork,"vpiNamedFork"},
  {vpiFork,"vpiFork"},
  {vpiCase,"vpiCase"},
  {vpiFor,"vpiFor"},
  {vpiIf,"vpiIf"},
  {vpiIfElse,"vpiIfElse"},
  {vpiRepeat,"vpiRepeat"},
  {vpiCondition, "vpiCondition" },
  {vpiElseStmt, "vpiElseStmt" },
  {vpiForIncStmt, "vpiForIncStmt" },
  {vpiScope, "vpiScope" },
  {vpiUse, "vpiUse" },
  {vpiExpr, "vpiExpr" },
  {vpiPrimitive, "vpiPrimitive" },
  {vpiStmt, "vpiStmt" },
  {vpiAnalogFunction,"vpiAnalogFunction"},
  {vpiFunction,"vpiFunction"},
  {vpiTask,"vpiTask"},
  {vpiAssignment,"vpiAssignment"},
  {vpiLhs,"vpiLhs"},
  {vpiRhs,"vpiRhs"},
  {vpiOperand,"vpiOperand"},
  {vpiArgument,"vpiArgument"},
  {vpiBranch,"vpiBranch"},

};

typedef enum _vaElectricalType {
  VA_Branch = 0,
  VA_Voltage,
  VA_Charge,
  VA_Capacitance
} vaElectricalType;

typedef enum _vaState {
  VA_ModuleTopBlock = 0, //0: toplevel exclued module block & other function
  VA_ModuleInitBlock,    //1: @(initial_step)
  VA_AnalogFunctionDef,  //2: analog func block 
  VA_ContribStmt,        //3: Y <+ X_expr stmt (w/o ddt/ddx/...)
                         //4: Y <+ X_expr+ddt/ddx/idt(...)
  VA_ContribWithFilterFunc 
} vaState;

typedef enum _returnFlag {
  Ret_NORMAL = 0,   //0: normal infor
  Ret_WARN,         //1: warnings 
  Ret_ERROR,        //2: Error including not implemented features, failures, etc 
  Ret_FATAL,        //3: show stoppers, fatal error 
} returnFlag;

typedef struct _contribElement {
  vaElectricalType etype;
  string_t contrib_lhs;
  string_t contrib_rhs;
  strVec nodes;
}contribElement;

typedef struct _vaElement {
  dictStrVec m_analogFuncVars;
  dictStrVec m_moduleVars;
  dictStrVec m_params;
  dictStrVec m_branches;
  strVec m_resolvedInitStepCcodes;    
  strVec m_resolvedCcodes;    
  strVec m_modulePorts;    
  strVec m_moduleNets;    
  string_t m_analogFuncArgDef;
  string_t m_moduleArgDef;
  strVec m_analogFuncNames;
  std::vector < contribElement > m_contribs;
  //current handling va code scope 
  vaState current_scope;  
  vpiHandle objPended;
  bool m_isSrcLinesElseIf;    
  returnFlag retFlag;
} vaElement;


std::map < objSelect, std::pair<size_t, const enum_description *> > objSelMap = {
  {toplevel,{sizeof(vpi_srccode_prop)/sizeof(vpi_srccode_prop[0]),    vpi_srccode_prop}},
  {vaPort,  {sizeof(vpi_port_prop)/sizeof(vpi_port_prop[0]),          vpi_port_prop}},
  {vaNet,   {sizeof(vpi_net_prop)/sizeof(vpi_net_prop[0]),            vpi_net_prop}},
  {vaAnaFun,{sizeof(vpi_anafun_prop)/sizeof(vpi_anafun_prop[0]),      vpi_anafun_prop}},
  {vaVar,   {sizeof(vpi_Var_prop)/sizeof(vpi_Var_prop[0]),            vpi_Var_prop}},
  {vaParam, {sizeof(vpi_parameter_prop)/sizeof(vpi_parameter_prop[0]),vpi_parameter_prop}},
  {vaCondition, {sizeof(vpi_condition_prop)/sizeof(vpi_condition_prop[0]),vpi_condition_prop}},
  {vaIf,    {sizeof(vpi_If_prop)/sizeof(vpi_If_prop[0]),vpi_If_prop}},
  {vaElseIf,{sizeof(vpi_ElseIf_prop)/sizeof(vpi_ElseIf_prop[0]),vpi_ElseIf_prop}},
  {vaElse,  {sizeof(vpi_Else_prop)/sizeof(vpi_Else_prop[0]),vpi_Else_prop}},
};

std::pair <size_t, const enum_description *>& 
getObjSelInfo(objSelect objSel)
{
  return objSelMap[objSel];
}

//search Ta in T which is typex <Ta>
template<typename T, typename Ta> 
bool find_item_container(T& container, Ta& item)
{
  if (container.size () == 0)
    return false;
  typename T::iterator it = find(container.begin(),container.end(),item);
  if (it == container.end())
    return false;
  return true;
}

//search key in map-like container
template <typename T, typename Key>
bool key_exists(const T& container, const Key& key)
{
    return (container.find(key) != std::end(container));
}

//search item in vector-like container
template <typename T, typename Key>
bool item_exists(const T& vcontainer, const Key& x)
{
  if(vcontainer.empty())
    return false;
  if(std::find(vcontainer.begin(), vcontainer.end(), x) != vcontainer.end()) 
    return true;
  else 
    return false;
}

//To check if a std::string `src stars with `targ
bool
str_startswith(const string_t& src, const string_t& targ)
{
if(src.substr(0, targ.size()) == targ)
  return true;
else
  return false;
}

string_t 
str_strip(const string_t s, const string_t chars=" ", int mode=0)
  //mode =0: trip both side; 1 left side; 2 right side
{
  size_t begin = 0;
  size_t end = s.size()-1;
  if(mode == 0 || mode == 1)
  {
    for(; begin < s.size(); begin++)
      if(chars.find_first_of(s[begin]) == string_t::npos)
        break;
  }
  if(mode == 0 || mode == 2)
  {
    for(; end > begin; end--)
      if(chars.find_first_of(s[end]) == string_t::npos)
        break;
  }
  return s.substr(begin, end-begin+1);
}

//split a string into a vector by token1 and token2
strVec
str_split(const string_t& line, const char token1, const char token2)
{
  strVec subArray;
  int len = line.length();
  for (int j = 0, k = 0; j < len; j++) {
      if (line[j] == token1 || line[j] == token2) {
          string_t ch = line.substr(k, j - k);
          k = j+1;
          if(ch.size()>0)
            subArray.push_back(ch);
      }
      if (j == len - 1) {
          string_t ch = line.substr(k, j - k+1);
          if(ch.size()>0)
            subArray.push_back(ch);
      }
  }
  return subArray;
}

void
str_convert_unit(string_t& src)
{
  for(strDict::iterator itmap = va_spice_unit_map.begin (); 
        itmap != va_spice_unit_map.end (); ++itmap)
  {
    string_t subkey = itmap->first;
    int pos=src.find_first_of(subkey);
    if(pos != (int)string_t::npos)
      src.replace(pos, subkey.size(), va_spice_unit_map[subkey]);
  }
}

strPair
getAnalogFuncArgDef(string_t& analogFuncArgs, 
    dictStrVec& analogFuncVars)
{
  strVec res = str_split(analogFuncArgs, ',', ' ');
  string_t strArgDef("");
  string_t strFuncVarDef("");
  unsigned int cnt_arg = 0, idx_var=0;
  if(res.size() >0 && res[0] == "input")  //remove the leading keyword `input'
    res.erase(res.begin ());
  
  //search key in map to find the var's type
  for (dictStrVec::iterator itMap = analogFuncVars.begin (); 
      itMap != analogFuncVars.end (); ++itMap)
  {
    idx_var = 0;
    for (strVec::iterator itVar = itMap->second.begin (); itVar != itMap->second.end (); ++itVar)
    {
      if (find_item_container(res, *itVar))
        //if the key matches arg 
      {
        strArgDef += itMap->first + " " + *itVar;
        strArgDef += ",";
        cnt_arg += 1;
      }
      else
        //Not match into vars list
      {
        if(idx_var == 0)
          strFuncVarDef += itMap->first + " ";
        strFuncVarDef += *itVar;
        if (itVar != itMap->second.end()-1)
          strFuncVarDef += ",";
        else
          strFuncVarDef += ";\n";
        idx_var += 1;
      }
    }
  }
  assert(cnt_arg == res.size());
  //remove the tail ',' in args list
  if(strArgDef[strArgDef.size()-1] == ',')
    strArgDef.erase(strArgDef.size()-1, 1); 
  strPair _anaFuncDefs = {strArgDef,strFuncVarDef};
  return _anaFuncDefs;
}

template<typename T> 
void setModuleArgDef(T& moduleArgsDef,  std::map<T, std::vector<T> >& moduleVars)
{
  unsigned int i=0;
  for(typename std::map<T, std::vector<T> >::iterator itmap = moduleVars.begin (); 
        itmap != moduleVars.end (); ++itmap)
  {
    moduleArgsDef += itmap->first + " ";
    i = 0;
    for(typename std::vector<T>::iterator itv=itmap->second.begin (); 
        itv != itmap->second.end (); ++itv,i++)
    {
      if(i < N_VARS_PER_COL)
      {
        if(i != 0)
          moduleArgsDef += ", ";
        moduleArgsDef += *itv;
      }
      else
      {
        moduleArgsDef += ";\n" + itmap->first + " " + *itv;
        i=0;
      }
    }
    moduleArgsDef += ";\n";
  }
}

template<typename T> 
void clean_container(T& container)
{
  container.clear();
}

//join the list/vector with div into a string, container: T <string>
template<typename T>
string_t concat_vector2string(T& vec, const string_t div=",")
{
  string_t _retStr;
  for(typename T::iterator it=vec.begin(); it != vec.end(); ++it)
  {
    _retStr += *it;
    if(it != vec.end()-1)
      _retStr += div;
  }
  return _retStr;
}

string_t
vpi_resolve_expr_impl (vpiHandle obj, vaElement& vaSpecialItems);

template<typename T> //container: vector <Tx>
void set_vec_iteration_by_name(vpiHandle obj, int iter_type, T& container, vaElement &vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (iter_type, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      string_t _retString = vpi_resolve_expr_impl(scan_handle, vaSpecialItems);
      container.push_back (_retString);
    }
  }  
}


template<typename T1, typename T2> //container: map <T1, T2>
void set_map_iteration_by_name(vpiHandle obj, int iter_type, std::map<T1,T2>& container, vaElement &vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (iter_type, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {

      string_t _retString = vpi_resolve_expr_impl(scan_handle, vaSpecialItems);
      int _type = (int) vpi_get (vpiType, scan_handle);
      switch(_type) {
        //TODO add more types
      case vpiRealVar:
      case vpiIntegerVar:
        container[va_c_type_map[_type]].push_back(_retString);
        break;
      case vpiParameter:
        {
          strVec _params = str_split(_retString,'=',' ');
          for(strVec::iterator it=_params.begin()+1; it != _params.end(); ++it)
            container[_params[0]].push_back(*it);
        }
        break;
      case vpiBranch:
        {
          strVec _params = str_split(_retString,',',' ');
          for(strVec::iterator it=_params.begin()+1; it != _params.end(); ++it)
            container[_params[0]].push_back(*it);
        }
        break;
      default:
        std::cout << "No supported type!" << std::endl;
        break;
      }
    }
  }  
}

#else
void CgenHeader( vpiHandle root);
void CgenImplement( vpiHandle root);
int vpi_gen_ccode (vpiHandle obj);
#endif

#endif
