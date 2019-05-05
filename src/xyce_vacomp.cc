/*
 * Verilog-A compiling to C++ codes in generic format
 * Not considering any simualtor specific trait
 * whose input is from vams AST through D-parser
 */
#include "xyce_vacomp.h"
#include "xyce_cxxgen.h"

static int g_indent_width=0;
static int g_incr_idx=0;
sstrDict va_c_expr_map = {
  {"ln",    "log"},
  {"log",   "log10"},
  {"sqrt",  "sqrt"},
  {"abs",   "fabs"},
  {"begin", "{"},
  {"end",   "}"},
  {"real",   "double"},
  {"integer","int"},
  {"$limit", ""},  //Not impl
  {"$strobe",  "printf"},
  {"$display", "printf"},
  {"$debug",   "printf"},
  {"$fstrobe", "fprintf"},
  {"$fopen",   "fopen"},
  {"$fclose",  "fclose"},
  //Simulator specified fucntions
  {"$vt",      "_VT_"},
  {"$temperature", "_TEMPER_"},
  {"$limexp",  "_LIMEXP_"},
  {"$realtime","_CURRTIME_"}, 
};
std::map < int, string_t > va_c_type_map = {
  {vpiRealVar,    "double"},
  {vpiIntegerVar, "int"},
  {vpiDecConst,   "int"},
  {vpiIntConst,   "int"},
  {vpiRealConst,  "double"},
};
sstrDict va_spice_unit_map = {
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
std::map < int, string_t > va_electrical_type_map = {
  {VA_Flow, "I"},
  {VA_Potential, "V"},
  {VA_Charge, "Q"},
  {VA_Capacitance, "C"},
};

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

//To change string to upper case
void str_toupper(string_t &src)
{
  std::transform(src.begin(), src.end(), src.begin(), toupper);
}

string_t str_toupperC(string_t src)
{
  string_t strg = src;
  std::transform(strg.begin(), strg.end(), strg.begin(), toupper);
  return strg;
}

//To change string to lower case
void str_tolower(string_t &src)
{
  std::transform(src.begin(), src.end(), src.begin(), tolower);
}

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

//remove the tail n chars of the string
void str_remove_tail(string_t& line, const int n)
{
  line.erase(line.size()-n, n); 
}

//get the leading numbers of space char in a string
int str_get_number_first_space(const string_t& line)
{
  if(!line.size())
    return 0;
  unsigned int idx;
  for(idx=0; idx < line.size(); idx++)
  {
    if(line[idx] != ' ')
      return idx;
  }
  return idx;
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
  for(sstrDict::iterator itmap = va_spice_unit_map.begin (); 
        itmap != va_spice_unit_map.end (); ++itmap)
  {
    string_t subkey = itmap->first;
    auto pos=src.find_first_of(subkey);
    if(pos != string_t::npos)
      src.replace(pos, subkey.size(), va_spice_unit_map[subkey]);
  }
}

//replace key `from to `to in src recursively
bool
str_replace_key(string_t& src, const string_t& from, const string_t& to)
{
  bool isReplaced = false;
  auto pos = src.find(from);
  while(pos!= string_t::npos)
  {
    src.replace(pos, from.size(), to);
    pos = src.find(from);
    isReplaced = true;
  }
  return isReplaced;
}

//get the a range variable's plain string,e.g., [4, inf)
string_t 
get_one_range(valueRange& range)
{
  string_t lower_Op = range.lower_Op == vpiGeOp ? "[" : "(";
  string_t higher_Op = range.higher_Op == vpiLeOp ? "]" : ")";
  return lower_Op + range.lower_value + ":" + range.higher_value + higher_Op;
}

strPair
getAnalogFuncArgDef(string_t& analogFuncArgs, 
    sstrVecDict& analogFuncVars, bool isTemplateType)
{
  strVec res = str_split(analogFuncArgs, ',', ' ');
  string_t strArgDef("");
  string_t strFuncVarDef("");
  unsigned int cnt_arg = 0, idx_var=0;
  if(res.size() >0 && res[0] == "input")  //remove the leading keyword `input'
    res.erase(res.begin ());
  
  //search key in map to find the var's type
  for (sstrVecDict::iterator itMap = analogFuncVars.begin (); 
      itMap != analogFuncVars.end (); ++itMap)
  {
    idx_var = 0;
    string_t myType = itMap->first;
    if(isTemplateType)
      myType = string_t(TEMPLATE_TYPE);
    for (strVec::iterator itVar = itMap->second.begin (); itVar != itMap->second.end (); ++itVar)
    {
      if (find_item_container(res, *itVar))
        //if the key matches arg 
      {
        strArgDef += myType + " " + *itVar;
        strArgDef += ",";
        cnt_arg += 1;
      }
      else
        //Not match into vars list
      {
        if(idx_var == 0)
          strFuncVarDef += myType + " ";
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
    str_remove_tail(strArgDef, 1);
  strPair _anaFuncDefs = {strArgDef,strFuncVarDef};
  return _anaFuncDefs;
}

//To obtain or create the depend item of the variable with `varName' and line_No
dependTargInfo& 
getOrCreate_dependTarg_lineNo(int lineNo, string_t& varName, vaElement& vaSpecialItems)
{
  for(auto ivec=vaSpecialItems.m_dependTargMap[varName].begin(); ivec != vaSpecialItems.m_dependTargMap[varName].end(); ++ivec)
  {
    if(ivec->lineNo == lineNo)
      return *ivec;
    else if(vaSpecialItems.m_needMergDependItem && vaSpecialItems.lineNo_ifelse_case != UNDEF)
    {
      return vaSpecialItems.m_dependTargMap[varName].back();
    }
  }
  dependTargInfo _depTarg;
  if(vaSpecialItems.m_needMergDependItem && vaSpecialItems.lineNo_ifelse_case == UNDEF)
  {
    _depTarg.lineNo = lineNo;
    vaSpecialItems.lineNo_ifelse_case = lineNo;
  }
  else
    _depTarg.lineNo = lineNo;
  vaSpecialItems.m_dependTargMap[varName].push_back(_depTarg);
  return vaSpecialItems.m_dependTargMap[varName].back();
}

//insert dependent nodes into container `depItem for a given target variable name
void
insert_depNodes_one_targ(vpiHandle obj, int lineNo, dependTargInfo& depItem, 
    vaElement& vaSpecialItems, int etype=VA_Potential) 
{
  string_t tagName = (string_t) vpi_get_str (vpiName, obj);
  if(key_exists(vaSpecialItems.m_params, tagName)) //ignore model parameter
    return;
  if(key_exists(vaSpecialItems.m_dependTargMap, tagName))
  {
    for(auto ivec=vaSpecialItems.m_dependTargMap[tagName].begin(); ivec != vaSpecialItems.m_dependTargMap[tagName].end(); ++ivec)
    {
      if(ivec->lineNo <= lineNo) //this depend var should appear before current line
      {
        //Don't insert duplicated item!
        insert_vec2vec_unique(depItem.dependNodes, ivec->dependNodes);
        insert_vec2vec_unique(depItem.depend_Branchnodes, ivec->depend_Branchnodes);
        break;
      }
    }
  }
  else if(item_exists(vaSpecialItems.m_moduleNets, tagName))
  {
    //it's a valid node & not exists here, insert it
    //if(!item_exists(depItem.dependNodes, tagName))
    //TODO: need to do that for depend_Branchnodes?
    {
      strPairVec *_thisNodes =  &(depItem.dependNodes);
      if(etype == VA_Flow)
        _thisNodes =  &(depItem.depend_Branchnodes);
      strPair &_nodePair = _thisNodes->back();
      if(!_thisNodes->size() || _nodePair.second != GND)
      {
        _thisNodes->push_back(strPair(tagName,GND));
      }
      else
      {
        assert(_nodePair.second == GND);
        _nodePair.second = tagName;
        //check and remove the redundant item
        item_redundant_remove(*_thisNodes);
      }
    }
  }
  else
  {
    std::cout << str_format("Warn:{} not insert into depend map!\n", tagName);
  }
}

//get all dependent unique nodes inside the arguments of any function call
const intVec funcArgsObjTypes = {vpiArgument, vpiOperand};
void
insert_depNodes_func_args(vpiHandle obj, int lineNo, dependTargInfo& depItem,
    vaElement& vaSpecialItems, int etype=VA_Potential)
{
  for(auto itrType = funcArgsObjTypes.begin(); itrType != funcArgsObjTypes.end(); ++itrType)
  {
    vpiHandle iterator = vpi_iterate (*itrType, obj);
    vpiHandle scan_handle;
    int idx=0, size = vpi_get (vpiSize, iterator);
    int cnt = 1;
    for(idx=0; idx < size; idx++)
    {
      if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
      {
        int _obj_type = (int) vpi_get (vpiType, scan_handle);
        if(_obj_type == xvpiReference)
          insert_depNodes_one_targ(scan_handle, lineNo, depItem, vaSpecialItems, etype);
        else if(_obj_type == vpiConstant)
          continue;
        else
          insert_depNodes_func_args (scan_handle, lineNo, depItem, vaSpecialItems, etype);
      }
    }
  }
}

//insert a depend item for this Lhs if needs
void
insert_depend_item(int lineNo, string_t& varName, vpiHandle objValue, vaElement& vaSpecialItems)
{
  if(!vaSpecialItems.m_needProcessDepend)
    return;

  int _obj_type = (int) vpi_get (vpiType, objValue);
  dependTargInfo &depItem = getOrCreate_dependTarg_lineNo(lineNo, varName, vaSpecialItems);
  string_t _retStr;
  if (_obj_type == vpiBranchProbeFuncCall ||  //V(a,c)
      _obj_type == vpiAnalogFuncCall ||       //V(ti), f(vx)
      _obj_type == vpiFuncCall ||             //diode(v,v2,..)
      _obj_type == vpiAnalogBuiltinFuncCall ||
      _obj_type == vpiSysFuncCall ||
      _obj_type == vpiAnalogSysTaskCall ||
      _obj_type == vpiAnalogFilterFuncCall || //ddt/ddx/...
      _obj_type == vpiAnalogSmallSignalFuncCall || //white|flicker_noise(...)
      _obj_type == vpiAnalogSysFuncCall)
  {
    if(IGNORE_NOISE == 1 && _obj_type == vpiAnalogSmallSignalFuncCall)
      return;
    if(_obj_type != vpiAnalogFilterFuncCall || 
        vaSpecialItems.current_scope == VA_ContribWithFilterFunc)
    {
      int _etype = VA_Potential;
      if(_obj_type == vpiBranchProbeFuncCall)
      {
        string_t tagName = (string_t) vpi_get_str (vpiName, objValue);
        //std::transform(tagName.begin(), tagName.end(), tagName.begin(), toupper);
        str_toupper(tagName);
        if(tagName[0] == 'V')
          _etype = VA_Potential;
        else if(tagName[0] == 'I')
          _etype = VA_Flow;
      }
      insert_depNodes_func_args(objValue, lineNo, depItem, vaSpecialItems,_etype);
    }
    return;
  }
  else if(_obj_type == xvpiReference)
    insert_depNodes_one_targ(objValue, lineNo, depItem, vaSpecialItems);
  else
  {
    //search all Operands in Rhs and get the dependent nodes
    vpiHandle iterator = vpi_iterate (vpiOperand, objValue);
    vpiHandle scan_handle;
    int idx=0, size = vpi_get (vpiSize, iterator);
    int cnt = 1;
    for(idx=0; idx < size; idx++)
    {
      if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
      {
        int _subobj_type = (int)vpi_get (vpiType, scan_handle);
        if(_subobj_type == vpiConstant)  //ignore const var
          continue;
        else if(_subobj_type == xvpiReference)
          insert_depNodes_one_targ(scan_handle, lineNo, depItem, vaSpecialItems);
        else
          insert_depend_item(lineNo, varName, scan_handle, vaSpecialItems);
      }
    }
  }
}

//For parameter stmt, get value and range concat with '='
void 
resolve_block_parameters(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  valueRange _range;
  _range.has_range = false;
  string_t parName = (char *) vpi_get_str (vpiName, obj);
  vpiHandle value_handle = vpi_handle(vpiExpr, obj);
  _range.init_value = str_strip(vpi_resolve_expr_impl (value_handle, vaSpecialItems));

  int cur_const_type = (int) vpi_get (vpiConstType, obj);
  assert(key_exists(va_c_type_map, cur_const_type));
  _range.val_type = va_c_type_map[cur_const_type];
  vpiHandle iterator = vpi_iterate (vpiValueRange, obj);
  if(!iterator)
  {
    vaSpecialItems.m_params[parName] = _range;
    return;
  }
  //processing range `from (a,b)'
  _range.has_range = true;
  vpiHandle range_handle = vpi_scan_index (iterator, 1);
  iterator = vpi_iterate (vpiExpr,range_handle);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1, _obj_type, _OP_type;
  string_t _str;
  assert(size == 2);
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {   
      _obj_type = (int) vpi_get (vpiType, scan_handle);
      _OP_type = (int) vpi_get (vpiOpType,scan_handle);
      _str = (char *) vpi_get_str(vpiDecompile,scan_handle);
      if(_obj_type == vpiOperation)
      {
        if(_OP_type == vpiGeOp || _OP_type == vpiGtOp)
        {
          _range.lower_Op = _OP_type;
          _range.lower_value = _str;
        }
        else if(_OP_type == vpiLeOp || _OP_type == vpiLtOp)
        {
          _range.higher_Op = _OP_type;
          _range.higher_value = _str;
        }
      }
    }
  }
  vaSpecialItems.m_params[parName] = _range;
  assert(::atof(_range.lower_value.c_str()) <= ::atof(_range.higher_value.c_str()));
}

//For begin ... end block
void 
resolve_block_begin(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (vpiStmt, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  retStr += string_t("{\n").insert(0, g_indent_width, ' ');
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
  retStr += string_t("}\n").insert(0, g_indent_width, ' ');
}

//For while(expr) begin ... end block
void
resolve_block_while(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t strCond="";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  retStr += string_t("while(").insert(0, g_indent_width, ' ');
  retStr += strCond;
  retStr += ")\n";
  vpiHandle objWhile_body = vpi_handle(vpiStmt, obj);
  if(objWhile_body)
  {
    //retStr += "\n";
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objWhile_body, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
    //retStr += "\n";
  }
}

//For repeat(expr) block where translates it to while(expr) in C
void
resolve_block_repeat(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t strCond="", incVar=g_loopIncVar, _strtmp;
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  incVar += std::to_string(g_incr_idx++);
  _strtmp = "int " + incVar + "=" + strCond + ";\n";
  _strtmp.insert(0, g_indent_width, ' ');
  retStr += _strtmp;
  _strtmp = "while(" + incVar + "--)\n";
  _strtmp.insert(0, g_indent_width, ' ');
  retStr += _strtmp;
  vpiHandle objWhile_body = vpi_handle(vpiStmt, obj);
  if(objWhile_body)
  {
    //retStr += "\n";
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objWhile_body, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
    //retStr += "\n";
  }
}

//For for-loop block
void
resolve_block_for(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t strCond, strIncr, strInit;
  int _indent_bak = g_indent_width;
  g_indent_width = 0;
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  vpiHandle objIncr = vpi_handle(vpiForIncStmt, obj);
  strIncr = vpi_resolve_expr_impl(objIncr, vaSpecialItems);
  //remove the tail ';'
  if(strIncr[strIncr.size()-1] == ';')
    str_remove_tail(strIncr, 1);

  vpiHandle objInit = vpi_handle(vpiForInitStmt, obj); 
  strInit = vpi_resolve_expr_impl(objInit, vaSpecialItems);
  g_indent_width = _indent_bak;
  retStr += "for(" + strInit + strCond + ";" + strIncr + ")";
  retStr += "\n";
  retStr.insert(0, g_indent_width, ' ');

  vpiHandle objBody = vpi_handle(vpiStmt, obj);
  if(objBody)
  {
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objBody, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
  }
}

//For case statement as below:
//case (test)
// expr-i: <block-i>
// default: <blockn>
//endcase
void
resolve_block_case(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  /* case strunct in vams AST
  obj  vpiCondition
  itr  vpiCaseItem[n] size=n
    obj CaseItem[i]
      itr  vpiExpr 
        obj  vpiExpr[n]
      obj  vpiStmt
    */
  string_t strCond="";
  string_t _tmpStr = "";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  retStr += "switch(" + strCond + "){\n";
  retStr.insert(0, g_indent_width, ' ');

  vpiHandle iterator = vpi_iterate (vpiCaseItem, obj);
  vpiHandle scan_handle, scan_expr;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1, _obj_type;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      _obj_type = (int) vpi_get (vpiType, scan_handle);
      if(_obj_type == vpiCaseItem)
      {
        //handle Expr at expr-i:<expr-i block>
        int cnt1=1;
        vpiHandle itr_expr = vpi_iterate (vpiExpr,scan_handle);
        int size1 = vpi_get (vpiSize, itr_expr);
        for(int idx=0; idx < size1; idx++)
        {
          if((scan_expr = vpi_scan_index (itr_expr, cnt1++)) != NULL)
          {
            _tmpStr = "case " + vpi_resolve_expr_impl (scan_expr, vaSpecialItems) + ":\n";
            _tmpStr.insert(0, g_indent_width, ' ');
            retStr += _tmpStr;
          }
        }
        vpiHandle objCaseBlock = vpi_handle(vpiStmt, scan_handle);
        string_t strline = (char *) vpi_get_str (vpiDecompile, scan_handle);
        if(str_startswith(strline, "default"))
          //here goes to default block
        {
          assert(size1 <= 0);
          _tmpStr = "default:\n";
          _tmpStr.insert(0, g_indent_width, ' ');
          retStr += _tmpStr;
        }
        if(objCaseBlock)
        {
          g_indent_width += INDENT_UNIT;
          _tmpStr = vpi_resolve_expr_impl (objCaseBlock, vaSpecialItems);
          _tmpStr += string_t("break;\n").insert(0, g_indent_width, ' ');
          retStr += _tmpStr;
          g_indent_width -= INDENT_UNIT;
        }
      }
    }
  }
  _tmpStr = "}\n";
  _tmpStr.insert(0, g_indent_width, ' ');
  retStr += _tmpStr;
}

void 
resolve_block_ifelse(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t strCond="", strCondElseIf="";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  if(vaSpecialItems.m_isSrcLinesElseIf>0)
  {
    g_indent_width -= INDENT_UNIT;
    retStr += string_t("else if(").insert(0, g_indent_width, ' ');
    g_indent_width += INDENT_UNIT;
  }
  else
  {
    retStr += string_t("if(").insert(0, g_indent_width, ' ');
  }
  retStr += strCond;
  retStr += ") {\n";
  vpiHandle objIf_body = vpi_handle(vpiStmt, obj);
  if(objIf_body)
  {
    //retStr += "\n";
    int _obj_type = (int) vpi_get (vpiType, objIf_body);
    if(_obj_type != vpiIfElse && vaSpecialItems.m_isSrcLinesElseIf>0)
      vaSpecialItems.m_isSrcLinesElseIf -= 1;
  
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objIf_body, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
    retStr += "\n";
    retStr += string_t("}\n").insert(0, g_indent_width, ' ');
  }

  vpiHandle objCondElseIf = vpi_handle(vpiElseStmt, obj);
  if(objCondElseIf)
  {
    int _obj_type = (int) vpi_get (vpiType, objCondElseIf);
    if(_obj_type == vpiIfElse)
      vaSpecialItems.m_isSrcLinesElseIf += 1;
    else if (_obj_type == vpiBegin && vaSpecialItems.m_isSrcLinesElseIf>0)
      vaSpecialItems.m_isSrcLinesElseIf -= 1;
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
      retStr += string_t("else {\n").insert(0, g_indent_width, ' ');
      retStr += strCondElseIf;
      //_retStr += strCondElseIf.insert(0, g_indent_width, ' ');
      //vaSpecialItems.m_isSrcLinesElseIf -= 1;
      retStr += "\n";
      retStr += string_t("}\n").insert(0, g_indent_width, ' ');
    }
  }
}

//For ddt/ddx/idt operator (Now only support ddt)
void            
resolve_block_analogFilterFunCall(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  //Firstly return 0.0 for the orignal assigment line if it's ddt
  string_t _strName = (char *) vpi_get_str (vpiName, obj);
  //std::transform(_strName.begin(), _strName.end(), _strName.begin(), toupper);
  str_toupper(_strName);
  if(_strName == "DDT")  //Only process ddt
  {
    //strip 'ddt' and resolve its arguments
    vpiHandle iterator = vpi_iterate (vpiArgument, obj);
    vpiHandle scan_handle;
    int idx=0, size = vpi_get (vpiSize, iterator);
    int cnt = 1;
    strVec _args;
    for(idx=0; idx < size; idx++)
    {
      if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
      {
        string_t _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
        _args.push_back(_retStr);
      }
    }
    //retStr = "0.0";
    //str_replace_key(retStr, "ddt", "");
    retStr = str_format("({})",concat_vector2string(_args, ",")); 
  }
  else
  {
    std::cout << "Error Analog Filter Function Call:`" << _strName << "' not supported yet!" <<std::endl;
    vaSpecialItems.retFlag = Ret_ERROR;
    return;
  }
  //set flag
  vaSpecialItems.current_scope = VA_ContribWithFilterFunc;
  vaSpecialItems.objPended = obj;
  return;
}

//For branch definition (alias)
void
resolve_block_branchDef(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  //get positive/negative node name for the branch
  string_t branchName = (char *)vpi_get_str(vpiName, obj);
  vpiHandle objPosNode = vpi_handle(vpiPosNode, obj);
  vpiHandle objNegNode = vpi_handle(vpiNegNode, obj);
  strVec nodes;
  nodes.push_back(branchName);
  nodes.push_back(vpi_resolve_expr_impl (objPosNode, vaSpecialItems));
  nodes.push_back(vpi_resolve_expr_impl (objNegNode, vaSpecialItems));
  retStr = concat_vector2string(nodes, ",");
}

//For V/I probing fucntion call: V(a,c), I(d,s),...
void
resolve_block_branchProbFunCall(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t _strType = (char *) vpi_get_str (vpiName, obj);
  string_t funcName = _strType;
  //std::transform(_strType.begin(), _strType.end(), _strType.begin(), toupper);
  str_toupper(_strType);
  
  vpiHandle iterator = vpi_iterate (vpiArgument, obj);
  vpiHandle scan_handle;
  strVec nodes;
  string_t _retStr;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
      if(_strType == "V")
        nodes.push_back(_retStr);
      else if ( _strType == "I" )
      {
        //resolve the branch name to real nodes
        if(key_exists(vaSpecialItems.m_branches, _retStr))
        {
          strVec _nodes = vaSpecialItems.m_branches[_retStr];
          nodes.insert(nodes.end(), _nodes.begin(), _nodes.end());
        }
        else
          nodes.push_back(_retStr);
      }
      else 
      {
        //here is analog/system function call
        nodes.push_back(_retStr);
      }
    }
  }
  if(_strType == "V" || _strType == "I")
  {
    //retStr = _strType + "prob_" + concat_vector2string(nodes, "_");
    if(nodes.size() == 2)
      retStr = str_format("probeVars[cogendaProbeID_{}_{}_{}]",_strType,nodes[0],nodes[1]);
    else
      retStr = str_format("probeVars[cogendaProbeID_{}_{}_{}]",_strType,nodes[0],GND);
    assert(nodes.size() <= 2);
    if(nodes.size() < 2)
      nodes.push_back(GND);
    strPair _nodePair(nodes[0],nodes[1]);
    if(!item_exists(vaSpecialItems.m_probeConstants[_strType],_nodePair))
      vaSpecialItems.m_probeConstants[_strType].push_back(_nodePair); //TODO avoid the duplicated item
    if( _strType == "I")
    {
      vaSpecialItems.m_nodeContainer.push_back(_nodePair);
    }
  }
  else  //here is analog/system function call
  {
    if(vaSpecialItems.m_isUseTemplateTypeAnalogFunc)
    {
      if(vaSpecialItems.current_scope != VA_AnalogFunctionDef)
        retStr = str_format("AnalogFunctions::{}<{}>({})",funcName,ADVAR_TYPE, concat_vector2string(nodes, ","));
      else
        retStr = str_format("{}<{}>({})",funcName,TEMPLATE_TYPE,concat_vector2string(nodes, ","));
    }
    else
      retStr = str_format("{}({})",funcName,concat_vector2string(nodes, ","));
  }
  return;
}

//For any function call: builtin, system, analog function call, etc
void 
resolve_block_anyFunCall(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  string_t anyFuncName = (char *) vpi_get_str (vpiName, obj);
  bool is_func_limit = false, is_system_var = false;
  if(key_exists(va_c_expr_map, anyFuncName))
  {
    retStr=va_c_expr_map[anyFuncName];
    if(retStr[0]=='_' and retStr.back()=='_') {
      //for sysmtem variable, don't insert ()
      is_system_var = true;
    }
    else {
      retStr += "(";
    }
    if(anyFuncName.find("$limit",0) != string_t::npos)
      is_func_limit = true;
  }else{
    if(anyFuncName[0] == '$')
    {
      std::cout << "Error Not supported system function: `" << anyFuncName << "'" <<std::endl;
      vaSpecialItems.retFlag = Ret_ERROR;
    }
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
      if(is_func_limit) //for $limit, do nothing and return the 1st arg
      {
        retStr += ")";
        break;
      }
      if(idx != size-1)
        retStr += ",";
      else
        if(!is_system_var)
          retStr += ")";
    }
  }
  //special handling for $strobe() etc calling
  if(vpiAnalogSysTaskCall == vpi_get (vpiType, obj))
  {
    retStr += ";";
    retStr.insert(0, g_indent_width, ' ');
  }
}

//For any unary, binary & Ternary OP, e.g., Ternary OP: c? a:b
void 
resolve_block_operation(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (vpiOperand, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  int opType = (int) vpi_get (vpiOpType, obj);
  if(size >1)
    retStr="(";
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      //unary OPs
      if(opType == vpiMinusOp)
        retStr += "-";
      else if(opType == vpiNotOp)
        retStr += "!";
      retStr += vpi_resolve_expr_impl (scan_handle, vaSpecialItems);
      retStr = str_strip(retStr, " ", 2);  //strip the right space chars
      if(idx != size-1) {
      //binary OPs
        if(opType == vpiAddOp)
          retStr += "+";
        else if(opType == vpiSubOp)
          retStr += "-";
        else if(opType == vpiDivOp)
          retStr += "/";
        else if(opType == vpiMultOp)
          retStr += "*";
        else if(opType == vpiModOp)
          retStr += "%";
        else if(opType == vpiGtOp)
          retStr += " > ";
        else if(opType == vpiGeOp)
          retStr += " >= ";
        else if(opType == vpiLtOp)
          retStr += " < ";
        else if(opType == vpiLeOp)
          retStr += " <= ";
        else if(opType == vpiLogAndOp)
          retStr += " && ";
        else if(opType == vpiLogOrOp || opType == vpiBitOrOp)
          retStr += " || ";
        else if(opType == vpiEqOp)
          retStr += " == ";
        else if(opType == vpiNeqOp)
          retStr += " != ";
        //ternary OPs
        else if(opType == vpiConditionOp)
        {
          if(idx == 0)
            retStr += " ? ";
          else if(idx == 1)
            retStr += " : ";
        }
      }
    }             
  }
  if(size >1)
    retStr += ")";  
}

//For assignment statment lhs=rhs
void 
resolve_block_assign(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objLhs = vpi_handle(vpiLhs, obj);
  vpiHandle objRhs = vpi_handle(vpiRhs, obj);
  string_t _strLhs = vpi_resolve_expr_impl (objLhs, vaSpecialItems);
  string_t _strRhs = vpi_resolve_expr_impl (objRhs, vaSpecialItems);
  //insert a depend item for this Lhs if needs
  int lineNo = (int) vpi_get (vpiLineNo, obj);
  insert_depend_item(lineNo, _strLhs, objRhs, vaSpecialItems);
  if( find_item_container(vaSpecialItems.m_analogFuncNames, _strLhs))
    //replace the 'function-name = Rhs' as 'return Rhs;'
    retStr += "return " + _strRhs + ";";
  else
    retStr += _strLhs   + " = "   +  _strRhs + ";";
  retStr.insert(0, g_indent_width, ' ');
}

//For Event control which starts @(...) begin ... end
void
resolve_block_eventControl(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  int _OP_type = (int) vpi_get (vpiOpType, objCond);
  if(_OP_type == vpiInitialStepOp)
  {
    vpiHandle objStmt = vpi_handle(vpiStmt, obj);  
    string_t _retStr="/*The initial_step block stars...*/\n";
    _retStr.insert(0, g_indent_width, ' ');
    retStr = _retStr;
    _retStr = vpi_resolve_expr_impl (objStmt, vaSpecialItems);
    retStr += _retStr;
    _retStr="/*The initial_step block ends...*/\n";
    _retStr.insert(0, g_indent_width, ' ');
    retStr += _retStr;
    vaSpecialItems.m_resolvedInitStepCcodes.push_back(retStr);
    retStr = ""; //not insert into module container
    return;
  }
  else
  {
    std::cout << "Error Not supported event expression:\n`" << retStr << "'" <<std::endl;
    vaSpecialItems.retFlag = Ret_ERROR;
    retStr = "";
    return;
  }
}
//For analog I/V contribtion expression x<+...
void 
resolve_block_contrib(vpiHandle obj, string_t& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objRhs = vpi_handle(vpiRhs, obj);
  //clean the node containor firstly
  vaSpecialItems.m_nodeContainer.clear();
  string_t _strRhs = vpi_resolve_expr_impl (objRhs, vaSpecialItems);
  vpiHandle objLhs = vpi_handle(vpiBranch, obj);
  int lineNo = (int) vpi_get (vpiLineNo, obj);
  vaElectricalType _etype;
  string_t _strType = (char *) vpi_get_str (vpiName, objLhs);
  //std::transform(_strType.begin(), _strType.end(), _strType.begin(), toupper);
  str_toupper(_strType);
  if(_strType == va_electrical_type_map[VA_Potential])
    _etype = VA_Potential;
  else if(_strType == va_electrical_type_map[VA_Flow])
    _etype = VA_Flow;
  else
    assert(0);
  
  vpiHandle iterator = vpi_iterate (vpiArgument, objLhs);
  vpiHandle scan_handle;
  contribElement _contrib;
  strVec nodes;
  string_t _retStr;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
      //make sure the args in V/I(arg1,args,..) within Module Net set
      assert(item_exists(vaSpecialItems.m_moduleNets,_retStr) 
          || key_exists(vaSpecialItems.m_branches, _retStr));
      
      //resolve the branch name to real nodes
      if(key_exists(vaSpecialItems.m_branches, _retStr))
      {
        strVec _nodes = vaSpecialItems.m_branches[_retStr];
        for(strVec::iterator it=_nodes.begin(); it != _nodes.end(); ++it)
          nodes.push_back(*it);
      }
      else
        nodes.push_back(_retStr);
    }
  }
  if(!vaSpecialItems.objPended)
  {
    string_t _strLhs = _strType + "contrib_" + concat_vector2string(nodes, "_");
    string_t _keytemp = "vpiAnalogSmallSignalFuncCall", _extrComment="";
    if(str_replace_key(_strRhs, _keytemp, "0.0"))
      _extrComment = "//smallSignalFuncCall ignored here.";
    retStr = str_format("{}+={}; {}",_strLhs,_strRhs,_extrComment);
    retStr.insert(0, g_indent_width, ' ');
    _contrib.etype = _etype;
    _contrib.lineNo = lineNo;
    _contrib.contrib_lhs=_strLhs;
    _contrib.contrib_rhs=_strRhs;
    _contrib.nodes = nodes;
    _contrib.rhs_etype = VA_Static;
    insert_depend_item(lineNo, _strLhs, objRhs, vaSpecialItems);
    _contrib.depend_nodes = vaSpecialItems.m_dependTargMap[_strLhs].back().dependNodes;
    _contrib.depend_Branchnodes = vaSpecialItems.m_dependTargMap[_strLhs].back().depend_Branchnodes;
    if(vaSpecialItems.m_nodeContainer.size())
    {
      _contrib.depend_Branchnodes.insert(_contrib.depend_Branchnodes.end(), vaSpecialItems.m_nodeContainer.begin(), vaSpecialItems.m_nodeContainer.end());
      insert_vec2vec_unique(vaSpecialItems.m_branchLIDs, vaSpecialItems.m_nodeContainer);
      //vaSpecialItems.m_branchLIDs.insert(vaSpecialItems.m_branchLIDs.end(),vaSpecialItems.m_nodeContainer.begin(), vaSpecialItems.m_nodeContainer.end());
    }
    vaSpecialItems.m_contribs.push_back(_contrib); 

    if(_strType == "V")
    {
      //record the branch LIDs (BRA item)
      if(nodes.size() == 2) {
        strPair _nodePair(nodes[0],nodes[1]);
        if(!item_exists(vaSpecialItems.m_branchLIDs,_nodePair))
          vaSpecialItems.m_branchLIDs.push_back(_nodePair);
        if(!item_exists(vaSpecialItems.m_probeConstants["I"],_nodePair))
          vaSpecialItems.m_probeConstants["I"].push_back(_nodePair); // avoid the duplicated item
      }
      else if(nodes.size() == 1) {
        strPair _nodePair(nodes[0],GND);
        if(!item_exists(vaSpecialItems.m_branchLIDs,_nodePair))
          vaSpecialItems.m_branchLIDs.push_back(_nodePair);
        if(!item_exists(vaSpecialItems.m_probeConstants["I"],_nodePair))
          vaSpecialItems.m_probeConstants["I"].push_back(_nodePair); // avoid the duplicated item
      }
      else
        assert(0);      
    }
    
  }
  else // vaSpecialItems.objPended
  {
    //vaSpecialItems.current_scope = VA_ContribSkipFilterFunc;
  //if(vaSpecialItems.objPended)
  //  vaSpecialItems.current_scope = VA_ContribWithFilterFunc;
  //check if there is ddt/ddx
    assert(vaSpecialItems.current_scope == VA_ContribWithFilterFunc && vaSpecialItems.objPended);
    //assert(vaSpecialItems.objPended);
    vpiHandle iterator = vpi_iterate (vpiArgument, vaSpecialItems.objPended);
    vpiHandle scan_handle;
    int idx=0, size = vpi_get (vpiSize, iterator);
    int cnt = 1;
    strVec _args;
    string_t _strLhs;
    string_t _strType2 = (char *) vpi_get_str (vpiName, vaSpecialItems.objPended);
    //std::transform(_strType2.begin(), _strType2.end(), _strType2.begin(), toupper);
    str_toupper(_strType2);
    assert(_strType2 == "DDT");
    for(idx=0; idx < size; idx++)
    {
      if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
      {
        _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
        _args.push_back(_retStr);
      }
    }
    if(_strType == "V")
    {
      _strLhs = _strType + "contrib_";
      _contrib.etype = _etype;
      //record the branch LIDs
      if(nodes.size() == 2) {
        strPair _nodePair(nodes[0],nodes[1]);
        if(!item_exists(vaSpecialItems.m_branchLIDs,_nodePair))
          vaSpecialItems.m_branchLIDs.push_back(_nodePair);
        if(!item_exists(vaSpecialItems.m_probeConstants["I"],_nodePair))
          vaSpecialItems.m_probeConstants["I"].push_back(_nodePair); // avoid the duplicated item
      }
      else if(nodes.size() == 1) {
        strPair _nodePair(nodes[0],GND);
        if(!item_exists(vaSpecialItems.m_branchLIDs,_nodePair))
          vaSpecialItems.m_branchLIDs.push_back(_nodePair);
        if(!item_exists(vaSpecialItems.m_probeConstants["I"],_nodePair))
          vaSpecialItems.m_probeConstants["I"].push_back(_nodePair); // avoid the duplicated item
      }
      else
        assert(0);      
    }
    else
    {
      _strLhs = "Qcontrib_";
      _contrib.etype = VA_Charge;
    }
    _strLhs += concat_vector2string(nodes, "_");
    //_strRhs = concat_vector2string(_args, "_");
    _retStr = _strLhs + "=" + _strRhs + ";"; //TODO why not += ?
    _retStr.insert(0, g_indent_width, ' ');
    //retStr  += "\n" + _retStr;
    retStr  += _retStr;
    
    //_contrib.etype = VA_Charge;
    _contrib.contrib_lhs=_strLhs;
    _contrib.contrib_rhs=_strRhs;
    _contrib.nodes = nodes;
    _contrib.rhs_etype = VA_Dynamic;
    insert_depend_item(lineNo, _strLhs, vaSpecialItems.objPended, vaSpecialItems);
    _contrib.depend_nodes = vaSpecialItems.m_dependTargMap[_strLhs].back().dependNodes;
    _contrib.depend_Branchnodes = vaSpecialItems.m_dependTargMap[_strLhs].back().depend_Branchnodes;
    if(vaSpecialItems.m_nodeContainer.size())
    {
      _contrib.depend_Branchnodes.insert(_contrib.depend_Branchnodes.end(), vaSpecialItems.m_nodeContainer.begin(), vaSpecialItems.m_nodeContainer.end());
      insert_vec2vec_unique(vaSpecialItems.m_branchLIDs, vaSpecialItems.m_nodeContainer);
      //vaSpecialItems.m_branchLIDs.insert(vaSpecialItems.m_branchLIDs.end(),vaSpecialItems.m_nodeContainer.begin(), vaSpecialItems.m_nodeContainer.end());
    }
    vaSpecialItems.m_contribs.push_back(_contrib);     
    //restore the state
    vaSpecialItems.objPended = 0;
    vaSpecialItems.current_scope = VA_ModuleTopBlock;
  }
}

//Main C code generation for VA statements
string_t
vpi_resolve_expr_impl (vpiHandle obj, vaElement &vaSpecialItems)
{
  string_t _retStr="";
  while (obj)
    {
      int cur_obj_type = (int) vpi_get (vpiType, obj);
      if(cur_obj_type != vpiModule && cur_obj_type != vpiAnalogFunction)
      {
        string_t strline = (char *) vpi_get_str (vpiDecompile, obj);
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
               || cur_obj_type == vpiSysFuncCall
               || cur_obj_type == vpiAnalogSysTaskCall   //$strobe()       
               || cur_obj_type == vpiAnalogSysFuncCall)
          {
            resolve_block_anyFunCall(obj, _retStr, vaSpecialItems);
          }
          else if (cur_obj_type == vpiBranchProbeFuncCall  //V(a,c)
               || cur_obj_type == vpiAnalogFuncCall)       //V(id)
            //For V(a,c), I(a,c),etc
          {
            resolve_block_branchProbFunCall(obj, _retStr, vaSpecialItems);
          }
          else if (cur_obj_type == vpiBranch)   //branch defintion
          {
            resolve_block_branchDef(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiAnalogSmallSignalFuncCall) //white|flicker_noise(...)
          {
            _retStr = "vpiAnalogSmallSignalFuncCall";
          }
          else if(cur_obj_type == vpiIfElse)
          {
            vaSpecialItems.m_needMergDependItem = true;
            resolve_block_ifelse(obj, _retStr, vaSpecialItems);
            vaSpecialItems.m_needMergDependItem = false;
            vaSpecialItems.lineNo_ifelse_case = UNDEF;
          }
          else if(cur_obj_type == vpiBegin)
          {
            resolve_block_begin(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == xvpiReference || cur_obj_type == vpiFuncType)
          {
            if(key_exists(va_c_expr_map, strline))
            {
              assert(!key_exists(vaSpecialItems.m_params, strline));
              return va_c_expr_map[strline];
            }
            else
            {
            if(key_exists(vaSpecialItems.m_params, strline))
                return MODEL_DOT + strline;
              return strline;
            }
          }
          else if(cur_obj_type == vpiConstant)
          {
            _retStr = strline;
            int cur_const_type = (int) vpi_get (vpiConstType, obj);
            if(cur_const_type == vpiRealConst)
              str_convert_unit(_retStr);
          }

          else if(cur_obj_type == vpiPort || cur_obj_type == vpiNet)
          {
            return strline;
          }
          else if(cur_obj_type == vpiParameter)
          {
            //For parameter stmt
            resolve_block_parameters(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiIODecl)
          {
            return strline;
          }
          else if(cur_obj_type == vpiRealVar || cur_obj_type == vpiIntegerVar)
            //For various vpiVariables types
            //TODO add more types
          {
            //check if it's an array
            if(strline.find('[') != string_t::npos && strline.find(']') != string_t::npos)
            {
              std::cout << "Error array variable: " << strline <<" not supported yet!" <<std::endl;
              vaSpecialItems.retFlag = Ret_ERROR;
            }
            return strline;
          }
          else if(cur_obj_type == vpiFuncCall)
          {
            //TODO handle HDL func call ... e.g., V(in)
            resolve_block_branchProbFunCall(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiContrib)
          {
            //handle VA contribution <+ ...
            resolve_block_contrib(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiAnalogFilterFuncCall)
            //handle analog OP ddt/ddx/idt
          {
            _retStr = strline;
            resolve_block_analogFilterFunCall(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiRepeat)
          {
            //handle repeat-loop ...
            resolve_block_repeat(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiWhile)
          {
            //handle while-loop ...
            resolve_block_while(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiFor)
          {
            //handle For-loop ...
            resolve_block_for(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiCase)
          {
            //handle Case stmt ...
            vaSpecialItems.m_needMergDependItem = true;
            resolve_block_case(obj, _retStr, vaSpecialItems);
            vaSpecialItems.m_needMergDependItem = false;
            vaSpecialItems.lineNo_ifelse_case = UNDEF;
          }
          else if(cur_obj_type == vpiCondition)
          {
            //already handled at other higher level stmt ...
            std::cout << "vpiCondition here..." << std::endl;
          }
          else if(cur_obj_type == vpiEventControl)
          {
            //handle @(initial_step) ... block
            _retStr = strline;
            resolve_block_eventControl(obj, _retStr, vaSpecialItems);
          }

	  if (verbose)
	  {
            std::cout << "Out src: " << _retStr << std::endl;
	  }
        }
      } 
      break;
    } //end of obj
  return _retStr;
}

//Main procedure for VA module to C code generation
int
vpi_resolve_srccode_impl (vpiHandle root, vaElement &vaSpecialItems)
{
  vaSpecialItems.current_scope = VA_ModuleTopBlock;
  vaSpecialItems.m_isSrcLinesElseIf = 0;
  vaSpecialItems.objPended = 0;
  vaSpecialItems.retFlag = Ret_NORMAL;
  vaSpecialItems.m_needMergDependItem = false;
  vaSpecialItems.lineNo_ifelse_case = UNDEF;
  vaSpecialItems.m_isUseTemplateTypeAnalogFunc = true;
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

  //process AnalogFunction block
  vaSpecialItems.current_scope = VA_AnalogFunctionDef;
  vaSpecialItems.m_needProcessDepend = false;
  vpiHandle obj_scan, objStmt_itr = vpi_iterate(vpiAnalogFunction, obj);
  int size = vpi_get (vpiSize, objStmt_itr);
  int cnt = 1;
  for(int idx=0; idx < size; idx++)
  {
    if((obj_scan = vpi_scan_index (objStmt_itr, cnt++)) != NULL)
    {
      int _obj_type = (int) vpi_get (vpiType, obj_scan);
      if( _obj_type == vpiAnalogFunction)
      {
        //get the function name with reture type
        string_t anlogFuncName = (char *)vpi_get_str (vpiName, obj_scan);
        string_t anlogFuncType = (char *)vpi_get_str (vpiFuncType, obj_scan);
        vaSpecialItems.m_resolvedAnaFunCcodes.push_back(str_format("/*starts of Analog function {}*/",anlogFuncName));
        if(anlogFuncType == "real" or anlogFuncType == "integer") //TODO for more types
        {
          if(!vaSpecialItems.m_isUseTemplateTypeAnalogFunc)
            anlogFuncType = va_c_expr_map[anlogFuncType];
        }
        vaSpecialItems.m_analogFuncNames.push_back(anlogFuncName);

        //get the arguments of AnalogFunction
        strVec analogFuncIOdef;
        sstrVecDict analogFuncVars;
        set_vec_iteration_by_name(obj_scan, vpiIODecl, 
            analogFuncIOdef, vaSpecialItems);

        set_map_iteration_by_name(obj_scan, vpiVariables, 
            analogFuncVars, vaSpecialItems);
        string_t s_analogFuncIOdef= concat_vector2string(analogFuncIOdef,",");
        strPair anaFuncDefs = 
          getAnalogFuncArgDef(s_analogFuncIOdef, analogFuncVars, vaSpecialItems.m_isUseTemplateTypeAnalogFunc);

        //get function definition: <type> <func_name>(args list)
        string_t analogFuncArgDef;
        if(vaSpecialItems.m_isUseTemplateTypeAnalogFunc)
          analogFuncArgDef = str_format("template<typename {}> {} ",TEMPLATE_TYPE,TEMPLATE_TYPE)
            + anlogFuncName + "(" + anaFuncDefs.first + ")";
        else
          analogFuncArgDef = anlogFuncType + " " + anlogFuncName 
          + "(" + anaFuncDefs.first + ")";
        vaSpecialItems.m_resolvedAnaFunCcodes.push_back(analogFuncArgDef);
        vaSpecialItems.m_resolvedAnaFunCcodes.push_back("{\n");

        //get function variables inside: <type1> vars list
        string_t _funcVars=anaFuncDefs.second;
        vaSpecialItems.m_resolvedAnaFunCcodes.push_back(_funcVars);

        //get other vpiStmt with analog function block
        vpiHandle objStmt = vpi_handle(vpiStmt, obj_scan);
        vpiHandle objStmt_itr1 = vpi_iterate(vpiStmt, objStmt);
        int _size = vpi_get (vpiSize, objStmt_itr1);
        int _cnt = 1;
        vpiHandle obj_scan_func;
        for(int idx=0; idx < _size; idx++)
        {
          if((obj_scan_func = vpi_scan_index (objStmt_itr1, _cnt++)) != NULL)
          {
            vaSpecialItems.m_resolvedAnaFunCcodes.push_back(
              vpi_resolve_expr_impl (obj_scan_func, vaSpecialItems));
          }
        }
        vaSpecialItems.m_resolvedAnaFunCcodes.push_back(str_format("}\n/*ends of Analog function {}*/",anlogFuncName));
      }
    }
  }
  //process module block
  vaSpecialItems.current_scope = VA_ModuleTopBlock;
  vaSpecialItems.m_needProcessDepend = true;
  vaSpecialItems.m_moduleName = (char *) vpi_get_str(vpiName, obj);
  //get the port and all nodes info
  set_vec_iteration_by_name(obj, vpiPort, vaSpecialItems.m_modulePorts, vaSpecialItems);
  set_vec_iteration_by_name(obj, vpiNet, vaSpecialItems.m_moduleNets, vaSpecialItems);

  //get the parameters and stored in a map
  set_map_iteration_by_name(obj, vpiParameter, vaSpecialItems.m_params, vaSpecialItems);
  
  //get module level variable definition lines
  set_map_iteration_by_name(obj, vpiVariables, vaSpecialItems.m_moduleVars, vaSpecialItems);
  setModuleArgDef(vaSpecialItems.m_moduleArgDef, vaSpecialItems.m_moduleVars);
  //Don't insert the model header into the main C codes
  //vaSpecialItems.m_resolvedCcodes.push_back(vaSpecialItems.m_moduleArgDef);
 
  //get branch definition if there are some
  //itr vpiBranch -> obj vpiBranch[n] ->vpiPosNode & vpiNegNode
  set_map_iteration_by_name(obj, vpiBranch, vaSpecialItems.m_branches, vaSpecialItems);

  //get other vpiStmt
  objStmt_itr = vpi_iterate(vpiStmt, obj);
  size = vpi_get (vpiSize, objStmt_itr);
  cnt = 1;
  for(int idx=0; idx < size; idx++)
  {
    if((obj_scan = vpi_scan_index (objStmt_itr, cnt++)) != NULL)
    {
      //both code into this container
      vaSpecialItems.m_resolvedCcodes.push_back(
        vpi_resolve_expr_impl (obj_scan, vaSpecialItems));
    }
  }
  return 0;
}

void
vpi_gen_ccode (vpiHandle obj, vaElement& vaSpecialEntries)
{
  // process src lines
  vpi_resolve_srccode_impl(obj, vaSpecialEntries);
  if (!verbose)
    return;
  std::cout << "Info: Final C codes begin..........." << std::endl;
  for(unsigned int idx=0; idx<2; idx++)
  {
    strVec *_codeVec;
    if(idx)
      _codeVec = &vaSpecialEntries.m_resolvedCcodes;
    else
      _codeVec = &vaSpecialEntries.m_resolvedAnaFunCcodes;
    for (strVec::iterator it = _codeVec->begin (); 
        it != _codeVec->end (); ++it)
    {
      std::cout << *it;
      if((*it)[(*it).size()-1] != '\n')
        std::cout << std::endl;
    }
  }
  paramDict mpars = vaSpecialEntries.m_params;
  for (paramDict::iterator it = mpars.begin (); it != mpars.end (); ++it)
    {
      //resolve reference parameter's value
      if (mpars.find (it->second.init_value) != mpars.end ())
	{
          string_t key = it->second.init_value;
	  mpars[it->first] = mpars[key];
	  std::cout << "parameter: " << it-> first << " value : " << it->second.init_value
            << " (resolved from " << key << ")" << std::endl;
	}
      else
      {
        std::cout << "parameter: " << it->first << " value: " << it->second.init_value;
        if(it->second.has_range)
        {
          string_t lower_Op = it->second.lower_Op == vpiGeOp ? "[" : "(";
          string_t higher_Op = it->second.higher_Op == vpiLeOp ? "]" : ")";
          std::cout << str_format(" with range {}",lower_Op) + it->second.lower_value + ":" + it->second.higher_value + higher_Op << std::endl;
        }
        else
          std::cout << std::endl;
      }
    }
  if(vaSpecialEntries.retFlag > 1)
    std::cout << "Info: VA compiling failed!" << std::endl;
  std::cout << "Info: Final C codes end............." << std::endl;
  return;
}


void
CxxGenFiles (vpiHandle root)
{
  vaElement vaModuleEntries;
  PLI_INT32 vpiObj = vpiModule;
  vpiHandle obj = vpi_iterator_by_index (root, vpiObj);
  vpiHandle topHandle = vpi_handle (vpiObj, obj);
  vpi_gen_ccode (topHandle, vaModuleEntries);
  string_t fHppName = str_format("{}.h", vaModuleEntries.m_moduleName);
  string_t fCxxName = str_format("{}.C", vaModuleEntries.m_moduleName);
  returnFlag retH,retC;
  retH = CgenHeader(vaModuleEntries, fHppName);
  retC = CgenImplement(vaModuleEntries, fCxxName);
  if(retH > 1 || retC > 1)
    std::cout << "Info: Generate Xyce model code failed!" << std::endl;
}



