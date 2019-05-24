#ifndef __xyce_vacomp_h
#define __xyce_vacomp_h
#include <vpi_user.h>
#if defined(__XVPI__)
  #include "xvpi.h"
#endif

#ifdef __cplusplus
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

#define IGNORE_NOISE 1
#define INSERT_EMPTY_LINE(h) h << std::endl
#define UNDEF -99999
#define UNFILLED "__UNFILLED__"
#define GND "GND"
#define MODEL_DOT "this->"
//#define MODEL_DOT "model_."
#define TEMPLATE_TYPE "GenericT"
#define ADVAR_TYPE "CogendaFadType"

struct _dependTargInfo;
struct _valueRange;
typedef std::string string_t;
typedef std::pair< string_t, string_t > strPair;
typedef std::pair< int, int > intPair;
typedef std::vector < string_t > strVec;
typedef std::vector < strPair > strPairVec;
typedef std::vector < int > intVec;
typedef std::vector < struct _dependTargInfo > dependVec;
typedef std::map < string_t, strVec > sstrVecDict;
typedef std::map < string_t, string_t > sstrDict;
typedef std::map < string_t, struct _valueRange> paramDict;
typedef std::map < string_t, strPairVec> sstrPairVecDict;

const unsigned int N_VARS_PER_COL=12;
const unsigned int INDENT_UNIT=2;
const string_t g_loopIncVar="__loop_incr_var";

extern std::map < int, string_t > va_c_type_map;
extern std::map < int, string_t > va_electrical_type_map;
extern sstrDict va_c_expr_map;
extern sstrDict va_spice_unit_map;

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
  VA_UNKNOWN = -1,
  VA_Flow = 0,
  VA_Potential,
  VA_Charge,
  VA_Capacitance,
  VA_Dynamic,
  VA_Static,
} vaElectricalType;

typedef enum _vaState {
  VA_ModuleTopBlock = 0, //0: toplevel exclued module block & other function
  VA_ModuleInitBlock,    //1: @(initial_step)
  VA_AnalogFunctionDef,  //2: analog func block 
  VA_ContribStmt,        //3: Y <+ X_expr stmt (w/o ddt/ddx/...)
                         //4: Y <+ X_expr+ddt/ddx/idt(...)
  VA_ContribWithFilterFunc, 
  VA_ContribSkipFilterFunc, //5:Only process Y <+ X_expr and skip the Q-part
} vaState;

typedef enum _returnFlag {
  Ret_NORMAL = 0,   //0: normal infor
  Ret_WARN,         //1: warnings 
  Ret_ERROR,        //2: Error including not implemented features, failures, etc 
  Ret_FATAL,        //3: show stoppers, fatal error 
} returnFlag;

typedef struct _contribElement {
  vaElectricalType etype;
  vaElectricalType rhs_etype;  //v(a,b)<+rhs, rhs's etype: ddt->dynamic else ->static
  bool isInsertedLhsNodes;
  int lineNo;
  string_t contrib_lhs;
  string_t contrib_rhs;
  strVec nodes;         //a,c for I(a,c) <+ v(a,c)*2 + v(b,c)
  //rhs dependent voltage node pair
  strPairVec depend_nodes;  //[{a,b},{b,c}] for above contrib
  //rhs dependent branch node pair
  strPairVec depend_Branchnodes;  //[{a,b},{b,c}] for contrib: i|v(a,c)<+rx*i(a,b)+ry*i(b,c)
  //rhs dependent node pair for dynamic part
}contribElement;

typedef struct _dependTargInfo {
  int lineNo;
  //strVec dependNodes;
  strPairVec dependNodes;
  //add it to save branch dependency like I(a,b)  for this lhs
  strPairVec depend_Branchnodes;
}dependTargInfo;

typedef struct _valueRange {
  string_t init_value;
  string_t val_type; //integer, real, etc
  int lower_Op;      //vpiGeOp, vpiGtOp
  string_t lower_value;
  int higher_Op;     //vpiLeOp, vpiLtOp
  string_t higher_value;
  bool has_range;
}valueRange;

typedef struct _vaElement {
  sstrVecDict m_moduleVars;
  paramDict m_params;
  sstrVecDict m_branches;
  strPairVec m_branchLIDs;   //for v(a,b) <+ ...
  strVec m_resolvedInitStepCcodes;    
  strVec m_resolvedCcodes;    
  strVec m_resolvedAnaFunCcodes;    
  strVec m_modulePorts;    
  strVec m_moduleNets;    
  string_t m_moduleArgDef;
  strVec m_analogFuncNames;
  std::vector < contribElement > m_contribs;
  std::map < string_t, dependVec > m_dependTargMap;
  sstrPairVecDict m_probeConstants;
  //temp stored rhs branch nodes infor stored for BRA item resolution
  strPairVec m_nodeContainer; 
  string_t m_moduleName;
  //current handling va code scope 
  vaState current_scope;  
  vpiHandle objPended;
  int m_isSrcLinesElseIf;    
  bool m_needProcessDepend;
  bool m_processedInitStepBlock; //flag to precesse @(initial_step) begin ... end
  bool m_isUseTemplateTypeAnalogFunc; //use C++ template function in *.h file for AnalogFunctions
  //merge the depend node list when a LHS within a switch-case or if-else block
  bool m_needMergDependItem;
  int lineNo_ifelse_case;
  returnFlag retFlag;
} vaElement;


extern std::map < objSelect, std::pair<size_t, const enum_description *> > objSelMap;

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

//remove the redundant item in vector-like container
template <typename T>
void item_redundant_remove(std::vector<T>& vcontainer)
{
  if(vcontainer.empty())
    return;
  std::set<T> s(vcontainer.cbegin(), vcontainer.cend());
  vcontainer = std::vector<T>(s.cbegin(), s.cend());  
}

//return the subvector = vector[istart:istart+len]
template <typename T>
std::vector<T> get_subvector(std::vector<T> &vec, int istart, int len)
{
  typename std::vector<T>::const_iterator first = vec.begin()+istart;
  typename std::vector<T>::const_iterator last  = first + len;
  typename std::vector<T> cut_vector(first, last);
  return cut_vector;
}

//To change string to upper case
void str_toupper(string_t &src);
string_t str_toupperC(string_t src);

//To change string to lower case
void str_tolower(string_t &src);

//To check if a std::string `src stars with `targ
bool
str_startswith(const string_t& src, const string_t& targ);

//string strip, mode =0: trip both side; 1 left side; 2 right side
string_t 
str_strip(const string_t s, const string_t chars, int mode);

//replace key `from to `to in src recursively
bool
str_replace_key(string_t& src, const string_t& from, const string_t& to);

//split a string into a vector by token1 and token2
strVec
str_split(const string_t& line, const char token1, const char token2);

//remove the tail n chars of the string
void str_remove_tail(string_t& line, const int n);

//get the leading numbers of space char in a string
int str_get_number_first_space(const string_t& line);

void str_convert_unit(string_t& src);

//find if the variable has the depend node in a map
bool has_depend_nodes(string_t key, dependVec& depVector);

string_t 
get_one_range(valueRange& range);

int
findContribItemWithLhs(string_t lhs,vaElement& vaSpecialItems);

strPair
getAnalogFuncArgDef(string_t& analogFuncArgs, 
    sstrVecDict& analogFuncVars, bool isTemplateType);

template<typename T>
void swapAB(T& a, T& b)
{
  T c;
  c = a; a = b; b = c;
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

//extend container by targContainer with unique element
template<typename T> //container: vector <Tx>
void insert_vec2vec_unique(T& container, T& targContainer)
{
  for(auto it=targContainer.begin(); it != targContainer.end(); ++it)
  {
    if(!item_exists(container, *it))
      container.push_back(*it);
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
      case vpiParameter:  //Useless codes, using the next `set_map_iteration_by_name
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

template<typename T1> //container: map <T1, valueRange>
void set_map_iteration_by_name(vpiHandle obj, int iter_type, std::map<T1,valueRange>& container, vaElement &vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (iter_type, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      int _type = (int) vpi_get (vpiType, scan_handle);
      assert(_type == vpiParameter);
      string_t _retString = vpi_resolve_expr_impl(scan_handle, vaSpecialItems);
    }
  }  
}

extern "C" {
extern int verbose;
returnFlag CgenHeader(vaElement& vaElem, string_t& fheaderName);
returnFlag CgenImplement(vaElement& vaElem, string_t& fheaderName);
void vpi_gen_ccode (vpiHandle obj, vaElement& vaElem);
void CxxGenFiles (vpiHandle root);
}

#else
void CxxGenFiles (vpiHandle root);
#endif

#endif

