/*
 * Verilog-A compiling to C++ codes in generic format
 * Not considering any simualtor specific trait
 * whose input is from vams AST through D-parser
 */
#include "xyce_vacomp.h"

static int g_indent_width=0;
static int g_incr_idx=0;

//For parameter stmt, get value and range concat with '='
void 
resolve_block_parameters(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::vector <std::string> _parValues;
  std::string parName = (char *) vpi_get_str (vpiName, obj);
  _parValues.push_back(parName);
  vpiHandle value_handle = vpi_handle(vpiExpr, obj);
  _parValues.push_back( 
    str_strip(vpi_resolve_expr_impl (value_handle, vaSpecialItems)));
  vpiHandle iterator = vpi_iterate (vpiValueRange, obj);
  if(!iterator)
  {
    retStr = concat_vector2string(_parValues, "=");
    return;
  }
  vpiHandle range_handle = vpi_scan_index (iterator, 1);
  iterator = vpi_iterate (vpiExpr,range_handle);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1, _obj_type, _OP_type;
  std::string range_low, range_high, _str;
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
        if(_OP_type == vpiGeOp)
          range_low = _str;
        else if(_OP_type == vpiLeOp)
          range_high = _str;
      }
    }
  }
  _parValues.push_back(range_low);
  _parValues.push_back(range_high);
  assert(::atof(range_low.c_str()) <= ::atof(range_high.c_str()));
  retStr = concat_vector2string(_parValues, "=");
}

//For begin ... end block
void 
resolve_block_begin(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle iterator = vpi_iterate (vpiStmt, obj);
  vpiHandle scan_handle;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  retStr += std::string("{\n").insert(0, g_indent_width, ' ');
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
  retStr += std::string("}\n").insert(0, g_indent_width, ' ');
}

//For while(expr) begin ... end block
void
resolve_block_while(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string strCond="";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  retStr += std::string("while(").insert(0, g_indent_width, ' ');
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
resolve_block_repeat(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string strCond="", incVar=g_loopIncVar, _strtmp;
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
resolve_block_for(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string strCond, strIncr, strInit;
  int _indent_bak = g_indent_width;
  g_indent_width = 0;
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  vpiHandle objIncr = vpi_handle(vpiForIncStmt, obj);
  strIncr = vpi_resolve_expr_impl(objIncr, vaSpecialItems);
  //remove the tail ';'
  if(strIncr[strIncr.size()-1] == ';')
    strIncr.erase(strIncr.size()-1, 1); 

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
resolve_block_case(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  /* case strunct in vams AST
  obj  vpiCondition
  itr  vpiCaseItem[n] size=n
    obj CaseItem[i]
      itr  vpiExpr 
        obj  vpiExpr[n]
      obj  vpiStmt
    */
  std::string strCond="";
  std::string _tmpStr = "";
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
        std::string strline = (char *) vpi_get_str (vpiDecompile, scan_handle);
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
          _tmpStr += std::string("break;\n").insert(0, g_indent_width, ' ');
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
resolve_block_ifelse(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string strCond="", strCondElseIf="";
  vpiHandle objCond = vpi_handle(vpiCondition, obj);
  strCond = vpi_resolve_expr_impl (objCond, vaSpecialItems);
  if(vaSpecialItems.m_isSrcLinesElseIf)
  {
    g_indent_width -= INDENT_UNIT;
    retStr += std::string("else if(").insert(0, g_indent_width, ' ');
    g_indent_width += INDENT_UNIT;
  }
  else
  {
    retStr += std::string("if(").insert(0, g_indent_width, ' ');
  }
  retStr += strCond;
  retStr += ")\n";
  vpiHandle objIf_body = vpi_handle(vpiStmt, obj);
  if(objIf_body)
  {
    //retStr += "\n";
    g_indent_width += INDENT_UNIT;
    retStr += vpi_resolve_expr_impl (objIf_body, vaSpecialItems);
    g_indent_width -= INDENT_UNIT;
    //retStr += "\n";
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
      retStr += std::string("else\n").insert(0, g_indent_width, ' ');
      retStr += strCondElseIf;
      //_retStr += strCondElseIf.insert(0, g_indent_width, ' ');
      vaSpecialItems.m_isSrcLinesElseIf = false;
    }
  }
}

//For ddt/ddx/idt operator (Now only support ddt)
void            
resolve_block_analogFilterFunCall(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  //Firstly return 0.0 for the orignal assigment line
  std::string _strName = (char *) vpi_get_str (vpiName, obj);
  std::transform(_strName.begin(), _strName.end(), _strName.begin(), toupper);
  if(_strName == "DDT")  //Only process ddt
    retStr = "0.0";
  else
    return;
  //set flag
  vaSpecialItems.current_scope = VA_ContribWithFilterFunc;
  vaSpecialItems.objPended = obj;
  return;
}

//For V/I probing fucntion call: V(a,c), I(d,s),...
void            
resolve_block_branchProbFunCall(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string _strType = (char *) vpi_get_str (vpiName, obj);
  std::transform(_strType.begin(), _strType.end(), _strType.begin(), toupper);
  assert(_strType == "V" || _strType == "I");
  
  vpiHandle iterator = vpi_iterate (vpiArgument, obj);
  vpiHandle scan_handle;
  std::vector<std::string> nodes;
  std::string _retStr;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
      nodes.push_back(_retStr);
    }
  }
  retStr = _strType + "prob_" + concat_vector2string(nodes, "_");
}

//For any function call: builtin, system, analog function call, etc
void 
resolve_block_anyFunCall(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  std::string anyFuncName = (char *) vpi_get_str (vpiName, obj);
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
  //special handling for $strobe() etc calling
  if(vpiAnalogSysTaskCall == vpi_get (vpiType, obj))
  {
    retStr += ";";
    retStr.insert(0, g_indent_width, ' ');
  }
}

//For any unary, binary & Ternary OP, e.g., Ternary OP: c? a:b
void 
resolve_block_operation(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
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
resolve_block_assign(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objLhs = vpi_handle(vpiLhs, obj);
  vpiHandle objRhs = vpi_handle(vpiRhs, obj);
  std::string _strLhs = vpi_resolve_expr_impl (objLhs, vaSpecialItems);
  std::string _strRhs = vpi_resolve_expr_impl (objRhs, vaSpecialItems);
  if( find_item_container(vaSpecialItems.m_analogFuncNames, _strLhs))
    //replace the 'function-name = Rhs' as 'return Rhs;'
    retStr += "return " + _strRhs + ";";
  else
    retStr += _strLhs   + " = "   +  _strRhs + ";";
  retStr.insert(0, g_indent_width, ' ');
}


//For analog I/V contribtion expression x<+...
void 
resolve_block_contrib(vpiHandle obj, std::string& retStr, vaElement& vaSpecialItems)
{
  vpiHandle objRhs = vpi_handle(vpiRhs, obj);
  std::string _strRhs = vpi_resolve_expr_impl (objRhs, vaSpecialItems);
  vpiHandle objLhs = vpi_handle(vpiBranch, obj);
  vaElectricalType _etype;
  std::string _strType = (char *) vpi_get_str (vpiName, objLhs);
  std::transform(_strType.begin(), _strType.end(), _strType.begin(), toupper);
  if(_strType == "V")
    _etype = VA_Voltage;
  else if(_strType == "I")
    _etype = VA_Branch;
  else
    assert(0);
  
  vpiHandle iterator = vpi_iterate (vpiArgument, objLhs);
  vpiHandle scan_handle;
  contribElement _contrib;
  std::vector<std::string> nodes;
  std::string _retStr;
  int idx=0, size = vpi_get (vpiSize, iterator);
  int cnt = 1;
  for(idx=0; idx < size; idx++)
  {
    if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
    {
      _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
      nodes.push_back(_retStr);
    }
  }
  std::string _strLhs = _strType + "contrib_" + concat_vector2string(nodes, "_");
  retStr = _strLhs + "=" + _strRhs + ";";
  retStr.insert(0, g_indent_width, ' ');
  _contrib.etype = _etype;
  _contrib.contrib_lhs=_strLhs;
  _contrib.contrib_rhs=_strRhs;
  _contrib.nodes = nodes;
  vaSpecialItems.m_contribs.push_back(_contrib); 
  //check if there is ddt/ddx
  if(vaSpecialItems.current_scope == VA_ContribWithFilterFunc)
  {
    assert(vaSpecialItems.objPended);
    vpiHandle iterator = vpi_iterate (vpiArgument, vaSpecialItems.objPended);
    vpiHandle scan_handle;
    int idx=0, size = vpi_get (vpiSize, iterator);
    int cnt = 1;
    std::vector<std::string> _args;
    _strType = (char *) vpi_get_str (vpiName, vaSpecialItems.objPended);
    std::transform(_strType.begin(), _strType.end(), _strType.begin(), toupper);
    assert(_strType == "DDT");
    for(idx=0; idx < size; idx++)
    {
      if((scan_handle = vpi_scan_index (iterator, cnt++)) != NULL)
      {
        _retStr = vpi_resolve_expr_impl (scan_handle, vaSpecialItems);     
        _args.push_back(_retStr);
      }
    }
    _strLhs  = "Q_ddt_";
    _strLhs += "contrib_" + concat_vector2string(nodes, "_");
    _strRhs  = concat_vector2string(_args, "_");
    _retStr += _strLhs + "=" + _strRhs + ";";
    _retStr.insert(0, g_indent_width, ' ');
    retStr  += "\n" + _retStr;
    _contrib.etype = VA_Charge;
    _contrib.contrib_lhs=_strLhs;
    _contrib.contrib_rhs=_strRhs;
    _contrib.nodes = nodes;
    vaSpecialItems.m_contribs.push_back(_contrib);     
    //restore the state
    vaSpecialItems.objPended = 0;
    vaSpecialItems.current_scope = VA_ModuleTopBlock;
  }
}

//Main C code generation for VA statements
std::string
vpi_resolve_expr_impl (vpiHandle obj, vaElement &vaSpecialItems)
{
  std::string _retStr="";
  while (obj)
    {
      int cur_obj_type = (int) vpi_get (vpiType, obj);
      if(cur_obj_type != vpiModule && cur_obj_type != vpiAnalogFunction)
      {
        std::string strline = (char *) vpi_get_str (vpiDecompile, obj);
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
            return strline;
          }
          else if(cur_obj_type == vpiFuncCall)
          {
            //TODO handle HDL func call ...
            return strline;
          }
          else if(cur_obj_type == vpiContrib)
          {
            //handle VA contribution <+ ...
            resolve_block_contrib(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiAnalogFilterFuncCall)
            //TODO ddt/ddx/idt
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
            resolve_block_case(obj, _retStr, vaSpecialItems);
          }
          else if(cur_obj_type == vpiCondition)
          {
            //already handled at other higher level stmt ...
            std::cout << "vpiCondition here..." << std::endl;
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
  vaSpecialItems.m_isSrcLinesElseIf = false;
  vaSpecialItems.objPended = 0;
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
        std::string anlogFuncName = (char *)vpi_get_str (vpiName, obj_scan);
        std::string anlogFuncType = (char *)vpi_get_str (vpiFuncType, obj_scan);
        if(anlogFuncType == "real" or anlogFuncType == "integer") //TODO for more types
          anlogFuncType = va_c_expr_map[anlogFuncType];
        vaSpecialItems.m_analogFuncNames.push_back(anlogFuncName);

        //get the arguments of AnalogFunction
        std::vector <std::string> analogFuncIOdef;
        set_vec_iteration_by_name(obj_scan, vpiIODecl, 
            analogFuncIOdef, vaSpecialItems);

        set_map_iteration_by_name(obj_scan, vpiVariables, 
            vaSpecialItems.m_analogFuncVars, vaSpecialItems);
        std::string s_analogFuncIOdef= concat_vector2string(analogFuncIOdef,",");
        std::pair <std::string, std::string> anaFuncDefs = 
          getAnalogFuncArgDef(s_analogFuncIOdef, vaSpecialItems.m_analogFuncVars);

        //get function definition: <type> <func_name>(args list)
        vaSpecialItems.m_analogFuncArgDef = anlogFuncType + " " + anlogFuncName 
          + "(" + anaFuncDefs.first + ")";
        vaSpecialItems.m_resolvedCcodes.push_back(vaSpecialItems.m_analogFuncArgDef);
        vaSpecialItems.m_resolvedCcodes.push_back("{\n");

        //get function variables inside: <type1> vars list
        std::string _funcVars=anaFuncDefs.second;
        vaSpecialItems.m_resolvedCcodes.push_back(_funcVars);

        //get other vpiStmt with analog function block
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
  vpi_resolve_srccode_impl(obj, vaSpecialEntries);
  std::cout << "=====Final C codes=====" << std::endl;
  for (std::vector < std::string >::iterator it = vaSpecialEntries.m_resolvedCcodes.begin (); 
      it != vaSpecialEntries.m_resolvedCcodes.end (); ++it)
  {
    std::cout << *it;
    if((*it)[(*it).size()-1] != '\n')
      std::cout << std::endl;
  }
  std::map < std::string, std::vector<std::string> > mpars = vaSpecialEntries.m_params;
  for (std::map < std::string, std::vector<std::string> >::iterator it = mpars.begin (); it != mpars.end (); ++it)
    {
      //resolve reference parameter's value
      if (mpars.find (it->second[0]) != mpars.end ())
	{
          std::string key = it->second[0];
	  mpars[it->first] = mpars[it->second[0]];
	  std::cout << "key: " << it-> first << " value : " << it->second[0]
            << " (resolved from " << key << ")" << std::endl;
	}
      else
      {
        std::cout << "key: " << it->first << " value: " << it->second[0];
        if(it->second.size() == 3)
          std::cout << " with range [" + it->second[1] + ":" + it->second[2] + "]" << std::endl;
        else
          std::cout << std::endl;
      }
    }

  return 0;
}

void CgenIncludeFiles()
{
std::cout << "//-------------------------------------------------------------------------" <<std::endl;
std::cout << "#include <Xyce_config.h>" <<std::endl;
std::cout << std::endl;
std::cout <<"// ---------- Standard Includes ----------" <<std::endl;
std::cout << std::endl;
std::cout <<"// ----------   Xyce Includes   ----------" <<std::endl;
std::cout <<"#include <N_DEV_Const.h>" <<std::endl;
std::cout <<"#include <N_DEV_DeviceOptions.h>" <<std::endl;
std::cout <<"#include <N_DEV_ExternData.h>" <<std::endl;
std::cout <<"#include <N_DEV_MOSFET1.h>" <<std::endl;
std::cout <<"#include <N_DEV_MOSFET_B4.h>" <<std::endl;
std::cout <<"#include <N_DEV_MatrixLoadData.h>" <<std::endl;
std::cout <<"#include <N_DEV_Message.h>" <<std::endl;
std::cout <<"#include <N_DEV_SolverState.h>" <<std::endl;
std::cout <<"#include <N_ERH_ErrorMgr.h>" <<std::endl;
std::cout <<"#include <N_LAS_Matrix.h>" <<std::endl;
std::cout <<"#include <N_LAS_Vector.h>" <<std::endl;
std::cout <<"#include <N_UTL_FeatureTest.h>" <<std::endl;
std::cout <<"#include <N_UTL_MachDepParams.h>" <<std::endl;
std::cout <<"#include <N_UTL_Math.h>" <<std::endl;

}

void
CgenHeader (vpiHandle root)
{
  std::cout << "This is a C++ header file !" << std::endl;
  CgenIncludeFiles();
  PLI_INT32 vpiObj = vpiModule;
  vpiHandle obj = vpi_iterator_by_index (root, vpiObj);
  vpiHandle topHandle = vpi_handle (vpiObj, obj);
  vpi_gen_ccode (topHandle);
}


void
CgenImplement (vpiHandle root)
{
  std::cout << "This is a C++ Main file of Implementation!" << std::endl;
}
