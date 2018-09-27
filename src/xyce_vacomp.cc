#include "xyce_vacomp.h"
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
using namespace std;
using std::string;
bool verbose = true;
//bool verbose = false;
static int g_indent_width=0;
static int INDENT_UNIT=2;


map < int, string > va_c_type_map = {
  {vpiRealVar,    "double"},
  {vpiIntegerVar, "int"},
};

map < string, string > va_c_expr_map = {
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
//  {vpiVariables, "vpiVariables" },
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

map < objSelect, pair<size_t, const enum_description *> > objSelMap = {
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

pair <size_t, const enum_description *> & getObjSelInfo(objSelect objSel)
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
bool key_exists(T& container, Key& key)
{
    return (container.find(key) != std::end(container));
}

//split a string into a vector by token1 and token2
vector<string> string_split(string line, const char token1, const char token2){
    vector<string> subArray;
    int len = line.length();
    for (int j = 0, k = 0; j < len; j++) {
        if (line[j] == token1 || line[j] == token2) {
            string ch = line.substr(k, j - k);
            k = j+1;
            if(ch.size()>0)
              subArray.push_back(ch);
        }
        if (j == len - 1) {
            string ch = line.substr(k, j - k+1);
            if(ch.size()>0)
              subArray.push_back(ch);
        }
    }
    return subArray;
}

pair <string, string> getAnalogFuncArgDef( string analogFuncArgs, map < string, vector < string > > &analogFuncVars)
{
  vector <string> res = string_split(analogFuncArgs, ',', ' ');
  string strArgDef("");
  string strFuncVarDef("");
  unsigned int cnt_arg = 0, idx_var=0;
  if(res.size() >0 && res[0] == "input")  //remove the leading keyword `input'
    res.erase(res.begin ());
  
  //search key in map to find the var's type
  for (map < string, vector <string> >::iterator itMap = analogFuncVars.begin (); 
      itMap != analogFuncVars.end (); ++itMap)
  {
    idx_var = 0;
    for (vector < string >::iterator itVar = itMap->second.begin (); itVar != itMap->second.end (); ++itVar)
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
  pair <string, string> _anaFuncDefs = {strArgDef,strFuncVarDef};
  return _anaFuncDefs;
}

template<typename T> 
void setModuleArgDef(T& moduleArgsDef,  map<T, vector<T> >& moduleVars)
{
  unsigned NperCol=10, i=0;
  for(typename map<T, vector<T> >::iterator itmap = moduleVars.begin (); 
        itmap != moduleVars.end (); ++itmap)
  {
    moduleArgsDef += itmap->first;
    for(typename vector<T>::iterator itv=itmap->second.begin (); 
        itv != itmap->second.end (); ++itv,i++)
    {
      if(i < NperCol)
        moduleArgsDef += " " + *itv;
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

typedef struct _vaElement {
  map < string, vector < string > > m_analogFuncVars;
  map < string, vector < string > > m_moduleVars;
  map < string, vector < string > > m_params;
  vector < string > m_srcLines;    
  bool m_isSrcLinesElseIf;    
  set < int > m_srcLineNoRec;    
  vector < string > m_resolvedCcodes;    
  vector < string > m_modulePorts;    
  vector < string > m_moduleNets;    
  string m_analogFuncArgDef;
  string m_moduleArgDef;
  vector < string > m_analogFuncNames;
  //current handling va code scope 0: toplevel exclued module block & other function 
  //1: module block 2: analog func block 3: If-Else block
  int current_scope;  
} vaElement;

string
vpi_resolve_expr_impl (vpiHandle obj, vaElement &vaSpecialItems);

void 
resolve_block_begin(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (vpiStmt, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  retStr += string("{\n").insert(0, g_indent_width, ' ');
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      g_indent_width += INDENT_UNIT;
      retStr += vpi_resolve_expr_impl (scan_handle, vaSpecialItems);
      g_indent_width -= INDENT_UNIT;
    }
    retStr += "\n";
  }
  retStr += string("}\n").insert(0, g_indent_width, ' ');
}

void 
resolve_block_ifelse(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  string strCond="", strCondElseIf="";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  if(vaSpecialItems.m_isSrcLinesElseIf)
  {
    g_indent_width -= INDENT_UNIT;
    retStr += string("else if(").insert(0, g_indent_width, ' ');
    g_indent_width += INDENT_UNIT;
  }
  else
  {
    retStr += string("if(").insert(0, g_indent_width, ' ');
  }
  retStr += strCond;
  retStr += ")";
  vpiHandle objIf_body = vpi_handle(vpiStmt, obj);
  if(objIf_body)
  {
    retStr += "\n";
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objIf_body, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
    retStr += "\n";
  }

  vpiHandle objCondElseIf = vpi_handle(vpiElseStmt, obj);
  if(objCondElseIf)
  {
    int _obj_type = (int) vpi_get (vpiType, objCondElseIf);
    vaSpecialItems.m_isSrcLinesElseIf = true;
    if(_obj_type != vpiIfElse)
      g_indent_width += INDENT_UNIT;
    strCondElseIf = vpi_resolve_expr_impl (objCondElseIf, vaSpecialItems);
    if(_obj_type != vpiIfElse)
      g_indent_width -= INDENT_UNIT;
    if(_obj_type == vpiIfElse)
      //If/Else if block
    {
      retStr += strCondElseIf.insert(0, g_indent_width, ' ');
    }
    else
      //Else block
    {
      retStr += string("else\n").insert(0, g_indent_width, ' ');
      retStr += strCondElseIf;
      //_retStr += strCondElseIf.insert(0, g_indent_width, ' ');
      vaSpecialItems.m_isSrcLinesElseIf = false;
    }
  }
}

//For any function call: builtin, system, analog function call, etc
void 
resolve_block_anyFunCall(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  string anyFuncName = (char *) vpi_get_str (vpiName, obj);
  if(key_exists(va_c_expr_map, anyFuncName))
  {
    retStr=va_c_expr_map[anyFuncName] + "(";
  }else{
    retStr=anyFuncName + "(";
  }
  vpiHandle iterator = vpi_iterate (vpiArgument, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      retStr += vpi_resolve_expr_impl (scan_handle, vaSpecialItems);                
      if(idx != size-1)
        retStr += ",";
      else
        retStr += ")";
    }
  }
}

void 
resolve_block_operation(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (vpiOperand, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  int opType = (int) vpi_get (vpiOpType, obj);
  retStr="(";
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      retStr += vpi_resolve_expr_impl (scan_handle, vaSpecialItems);
      if(idx != size-1) {
        //TODO more OPs
        if(opType == vpiAddOp)
          retStr += "+";
        else if(opType == vpiSubOp)
          retStr += "-";
        else if(opType == vpiDivOp)
          retStr += "/";
        else if(opType == vpiMultOp)
          retStr += "*";
        else if(opType == vpiGtOp)
          retStr += " > ";
        else if(opType == vpiLtOp)
          retStr += " < ";
        else if(opType == vpiLogAndOp)
          retStr += " && ";
        else if(opType == vpiLogOrOp)
          retStr += " || ";
      }
    }             
  }
  retStr += ")";  
}

void 
resolve_block_assign(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objLhs = vpi_handle(vpiLhs, obj);
  vpiHandle objRhs = vpi_handle(vpiRhs, obj);
  string _strLhs = vpi_resolve_expr_impl (objLhs, vaSpecialItems);
  string _strRhs = vpi_resolve_expr_impl (objRhs, vaSpecialItems);
  if( find_item_container(vaSpecialItems.m_analogFuncNames, _strLhs))
    //replace the 'function-name = Rhs' as 'return Rhs;'
    retStr += "return " + _strRhs + ";";
  else
    retStr += _strLhs   + " = "   +  _strRhs + ";";
  retStr.insert(0, g_indent_width, ' ');
}

void 
resolve_block_contrib(vpiHandle obj, string& retStr, vaElement& vaSpecialItems)
{
  //TODO handle <+ ...
  cout << "**dbg vpiContrib here.." << endl;
  retStr.insert(0, g_indent_width, ' ');
}

string
vpi_resolve_expr_impl (vpiHandle obj, vaElement &vaSpecialItems)
{
  string _retStr="";
  while (obj)
    {
      int cur_obj_type = (int) vpi_get (vpiType, obj);
      if(cur_obj_type != vpiModule && cur_obj_type != vpiAnalogFunction)
      {
        string strline = (char *) vpi_get_str (vpiDecompile, obj);
        _retStr = strline;
	if (strline.size()) 
        {
          assert(cur_obj_type != vpiIterator);
          if(cur_obj_type == vpiAssignment)
          {
            resolve_block_assign(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiOperation)
          {
            resolve_block_operation(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiAnalogBuiltinFuncCall 
               || cur_obj_type == vpiAnalogFuncCall
               || cur_obj_type == vpiSysFuncCall)
          {
            resolve_block_anyFunCall(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiIfElse)
          {
            resolve_block_ifelse(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiBegin)
          {
            resolve_block_begin(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == xvpiReference || cur_obj_type == vpiFuncType)
          {
            if(key_exists(va_c_expr_map, strline))
              return va_c_expr_map[strline];
            else
              return strline;
          }
          else if(cur_obj_type == vpiConstant)
            return strline;

          else if(cur_obj_type == vpiPort || cur_obj_type == vpiNet)
          {
            return strline;
          }
          else if(cur_obj_type == vpiParameter)
          {
            //TODO should check vpiExpr and do more elaberation
            return strline;
          }
          else if(cur_obj_type == vpiIODecl)
          {
            return strline;
          }
          else if(cur_obj_type == vpiRealVar || cur_obj_type == vpiIntegerVar)
            //For various vpiVariables types
            //TODO add more types
          {
            return strline;
          }
          else if(cur_obj_type == vpiFuncCall)
          {
            //TODO handle V(a,c) ...
            return strline;
          }
          else if(cur_obj_type == vpiContrib)
          {
            //TODO handle <+ ...
            resolve_block_contrib(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiRepeat)
          {
            //TODO handle repeat-loop ...
            cout << "**dbg vpiRepeat here.." << endl;
          }
          else if(cur_obj_type == vpiWhile)
          {
            //TODO handle while-loop ...
            cout << "**dbg vpiWhile here.." << endl;
          }
          else if(cur_obj_type == vpiFor)
          {
            //TODO handle For-loop ...
            cout << "**dbg vpiFor here.." << endl;
          }

          else if(cur_obj_type == vpiCondition)
          {
            cout << "**dbg vpiCondition here.." << endl;
          }

          else if(cur_obj_type == vpiCase)
          {
            cout << "**dbg vpiCase here.." << endl;
          }

	  if (verbose)
	  {
            cout << "Out src: " << _retStr << endl;
	  }
        }
      } 
      break;
    } //end of obj
  return _retStr;
}

template<typename T>  //container: vector <Tx>
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
      string _retString = vpi_resolve_expr_impl(scan_handle, vaSpecialItems);
      container.push_back (_retString);
    }
  }  
}

//join the list/vector with div into a string, container: T <string>
template<typename T>
string concat_vector2string(T& vec, const string div)
{
  string _retStr;
  for(typename T::iterator it=vec.begin(); it != vec.end(); ++it)
  {
    _retStr += *it;
    if(it != vec.end())
      _retStr += div;
  }
  return _retStr;
}

template<typename T1, typename T2> //container: map <T1, T2>
void set_map_iteration_by_name(vpiHandle obj, int iter_type, map<T1,T2>& container, vaElement &vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (iter_type, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {

      string _retString = vpi_resolve_expr_impl(scan_handle, vaSpecialItems);
      int _type = (int) vpi_get (vpiType, scan_handle);
      switch(_type) {
        //TODO add more types
      case vpiRealVar:
      case vpiIntegerVar:
        container[va_c_type_map[_type]].push_back(_retString);
        break;
      case vpiParameter:
        {
          vector<string> _params = string_split(_retString,'=',' ');
          container[_params[0]].push_back(_params[1]);
        }
        break;
      default:
        cout << "No supported type!" << endl;
        break;
      }
    }
  }  
}

int
vpi_resolve_srccode_impl (vpiHandle root, vaElement &vaSpecialItems)
{
  vpiHandle obj;
  int cur_obj_type = (int) vpi_get (vpiType, root);
  if(cur_obj_type == vpiModule)
    obj = root;
  else if(cur_obj_type == vpiIterator)
  {
    obj = vpi_handle (vpiModule, root);
    cur_obj_type = (int) vpi_get (vpiType, obj);
    assert(cur_obj_type == vpiModule);
  }
  else
    assert(0);
  //get the port and all nodes info
  set_vec_iteration_by_name(obj, vpiPort, vaSpecialItems.m_modulePorts, vaSpecialItems);
  set_vec_iteration_by_name(obj, vpiNet, vaSpecialItems.m_moduleNets, vaSpecialItems);

  //get the parameters and stored in a map
  set_map_iteration_by_name(obj, vpiParameter, vaSpecialItems.m_params, vaSpecialItems);
  
  //get module level variable definition lines
  set_map_iteration_by_name(obj, vpiVariables, vaSpecialItems.m_moduleVars, vaSpecialItems);
  setModuleArgDef(vaSpecialItems.m_moduleArgDef, vaSpecialItems.m_moduleVars);
  vaSpecialItems.m_resolvedCcodes.push_back(vaSpecialItems.m_moduleArgDef);
  
  //get other vpiStmt
  vpiHandle objStmt_itr = vpi_iterate(vpiStmt, obj);
  int size = vpi_get (vpiSize, objStmt_itr);
  int cnt = 1;
  vpiHandle obj_scan;
  for(int idx=0; idx < size; idx++)
  {
    if((obj_scan = vpi_scan_index (objStmt_itr, cnt++)) != NULL)
    {
      vaSpecialItems.m_resolvedCcodes.push_back(
        vpi_resolve_expr_impl (obj_scan, vaSpecialItems));
    }
  }
  //get AnalogFunction block
  objStmt_itr = vpi_iterate(vpiAnalogFunction, obj);
  size = vpi_get (vpiSize, objStmt_itr);
  cnt = 1;
  for(int idx=0; idx < size; idx++)
  {
    if((obj_scan = vpi_scan_index (objStmt_itr, cnt++)) != NULL)
    {
      int _obj_type = (int) vpi_get (vpiType, obj_scan);
      if( _obj_type == vpiAnalogFunction)
      {
        vaSpecialItems.m_resolvedCcodes.push_back("/*starts of Analog function*/");
        //get the function name with reture type
        string anlogFuncName = (char *)vpi_get_str (vpiName, obj_scan);
        string anlogFuncType = (char *)vpi_get_str (vpiFuncType, obj_scan);
        if(anlogFuncType == "real" or anlogFuncType == "integer") //TODO for more types
          anlogFuncType = va_c_expr_map[anlogFuncType];
        vaSpecialItems.m_analogFuncNames.push_back(anlogFuncName);

        //get the arguments of AnalogFunction
        vector <string> analogFuncIOdef;
        set_vec_iteration_by_name(obj_scan, vpiIODecl, analogFuncIOdef, vaSpecialItems);

        set_map_iteration_by_name(obj_scan, vpiVariables, vaSpecialItems.m_analogFuncVars, vaSpecialItems);
        pair <string, string> anaFuncDefs = getAnalogFuncArgDef(concat_vector2string(analogFuncIOdef,","), vaSpecialItems.m_analogFuncVars);
        //get function definition: <type> <func_name>(args list)
        vaSpecialItems.m_analogFuncArgDef = anlogFuncType + " " + anlogFuncName 
          + "(" + anaFuncDefs.first + ")";
        vaSpecialItems.m_resolvedCcodes.push_back(vaSpecialItems.m_analogFuncArgDef);
        vaSpecialItems.m_resolvedCcodes.push_back("{\n");
        //get function variables inside: <type1> vars list
        string _funcVars=anaFuncDefs.second;
        vaSpecialItems.m_resolvedCcodes.push_back(_funcVars);

        //get other vpiStmt
        vpiHandle objStmt = vpi_handle(vpiStmt, obj_scan);
        vpiHandle objStmt_itr = vpi_iterate(vpiStmt, objStmt);
        int _size = vpi_get (vpiSize, objStmt_itr);
        int _cnt = 1;
        vpiHandle obj_scan_func;
        for(int idx=0; idx < _size; idx++)
        {
          if((obj_scan_func = vpi_scan_index (objStmt_itr, _cnt++)) != NULL)
          {
            vaSpecialItems.m_resolvedCcodes.push_back(
              vpi_resolve_expr_impl (obj_scan_func, vaSpecialItems));
          }
        }
        vaSpecialItems.m_resolvedCcodes.push_back("}\n/*ends of Analog function*/");
      }
    }
  }
  return 0;
}

int
vpi_gen_ccode (vpiHandle obj)
{
  // process src lines
  vaElement vaSpecialEntries;
  vaSpecialEntries.current_scope = 0;
  vaSpecialEntries.m_isSrcLinesElseIf = false;
  vpi_resolve_srccode_impl(obj, vaSpecialEntries);
  cout << "=====Final C codes=====" << endl;
  for (vector < string >::iterator it = vaSpecialEntries.m_resolvedCcodes.begin (); 
      it != vaSpecialEntries.m_resolvedCcodes.end (); ++it)
  {
    cout << *it;
    if((*it)[(*it).size()-1] != '\n')
      cout << endl;
  }
  map < string, vector<string> > mpars = vaSpecialEntries.m_params;
  for (map < string, vector<string> >::iterator it = mpars.begin (); it != mpars.end (); ++it)
    {
      //resolve reference parameter's value
      if (mpars.find (it->second[0]) != mpars.end ())
	{
          string key = it->second[0];
	  mpars[it->first] = mpars[it->second[0]];
	  cout << "key: " << it-> first << " value : " << it->second[0]
            << " (resolved from " << key << ")" << endl;
	}
      else
        cout << "key: " << it->first << " value: " << it->second[0] << endl;
    }

  //clean the container
  return 0;
}

void CgenIncludeFiles()
{
cout << "//-------------------------------------------------------------------------" <<endl;
cout << "#include <Xyce_config.h>" <<endl;
cout << endl;
cout <<"// ---------- Standard Includes ----------" <<endl;
cout << endl;
cout <<"// ----------   Xyce Includes   ----------" <<endl;
cout <<"#include <N_DEV_Const.h>" <<endl;
cout <<"#include <N_DEV_DeviceOptions.h>" <<endl;
cout <<"#include <N_DEV_ExternData.h>" <<endl;
cout <<"#include <N_DEV_MOSFET1.h>" <<endl;
cout <<"#include <N_DEV_MOSFET_B4.h>" <<endl;
cout <<"#include <N_DEV_MatrixLoadData.h>" <<endl;
cout <<"#include <N_DEV_Message.h>" <<endl;
cout <<"#include <N_DEV_SolverState.h>" <<endl;
cout <<"#include <N_ERH_ErrorMgr.h>" <<endl;
cout <<"#include <N_LAS_Matrix.h>" <<endl;
cout <<"#include <N_LAS_Vector.h>" <<endl;
cout <<"#include <N_UTL_FeatureTest.h>" <<endl;
cout <<"#include <N_UTL_MachDepParams.h>" <<endl;
cout <<"#include <N_UTL_Math.h>" <<endl;

}

void
CgenHeader (vpiHandle root)
{
  cout << "This is a C++ header file !" << endl;
  CgenIncludeFiles();
  PLI_INT32 vpiObj = vpiModule;
  vpiHandle obj = vpi_iterator_by_index (root, vpiObj);
  vpiHandle topHandle = vpi_handle (vpiObj, obj);
  vpi_gen_ccode (topHandle);
}


void
CgenImplement (vpiHandle root)
{
  cout << "This is a C++ Main file of Implementation!" << endl;
}
