#include <stdlib.h>
#include <vpi_user.h>

#if defined(__XVPI__)
	#include "xvpi.h"
#endif


#define INDENT_WIDTH 4

static int vpi_object_dump (vpiHandle obj, vpiHandle scope [], int level_max, int level)
{
	struct enum_description {
		PLI_INT32 name;
		char *string;
	};
	static const struct enum_description vpi_object_types [] = {
		/* 1364-1995 */
		/****************************** OBJECT TYPES ******************************/
		{ vpiAlways, "vpiAlways" },
		{ vpiAssignStmt, "vpiAssignStmt" },
		{ vpiAssignment, "vpiAssignment" },
		{ vpiBegin, "vpiBegin" },
		{ vpiCase, "vpiCase" },
		{ vpiCaseItem, "vpiCaseItem" },
		{ vpiConstant, "vpiConstant" },
		{ vpiContAssign, "vpiContAssign" },
		{ vpiDeassign, "vpiDeassign" },
		{ vpiDefParam, "vpiDefParam" },
		{ vpiDelayControl, "vpiDelayControl" },
		{ vpiDisable, "vpiDisable" },
		{ vpiEventControl, "vpiEventControl" },
		{ vpiEventStmt, "vpiEventStmt" },
		{ vpiFor, "vpiFor" },
		{ vpiForce, "vpiForce" },
		{ vpiForever, "vpiForever" },
		{ vpiFork, "vpiFork" },
		{ vpiFuncCall, "vpiFuncCall" },
		{ vpiFunction, "vpiFunction" },
		{ vpiGate, "vpiGate" },
		{ vpiIf, "vpiIf" },
		{ vpiIfElse, "vpiIfElse" },
		{ vpiInitial, "vpiInitial" },
		{ vpiIntegerVar, "vpiIntegerVar" },
		{ vpiInterModPath, "vpiInterModPath" },
		{ vpiIterator, "vpiIterator" },
		{ vpiIODecl, "vpiIODecl" },
		{ vpiMemory, "vpiMemory" },
		{ vpiMemoryWord, "vpiMemoryWord" },
		{ vpiModPath, "vpiModPath" },
		{ vpiModule, "vpiModule" },
		{ vpiNamedBegin, "vpiNamedBegin" },
		{ vpiNamedEvent, "vpiNamedEvent" },
		{ vpiNamedFork, "vpiNamedFork" },
		{ vpiNet, "vpiNet" },
		{ vpiNetBit, "vpiNetBit" },
		{ vpiNullStmt, "vpiNullStmt" },
		{ vpiOperation, "vpiOperation" },
		{ vpiParamAssign, "vpiParamAssign" },
		{ vpiParameter, "vpiParameter" },
		{ vpiPartSelect, "vpiPartSelect" },
		{ vpiPathTerm, "vpiPathTerm" },
		{ vpiPort, "vpiPort" },
		{ vpiPortBit, "vpiPortBit" },
		{ vpiPrimTerm, "vpiPrimTerm" },
		{ vpiRealVar, "vpiRealVar" },
		{ vpiReg, "vpiReg" },
		{ vpiRegBit, "vpiRegBit" },
		{ vpiRelease, "vpiRelease" },
		{ vpiRepeat, "vpiRepeat" },
		{ vpiRepeatControl, "vpiRepeatControl" },
		{ vpiSchedEvent, "vpiSchedEvent" },
		{ vpiSpecParam, "vpiSpecParam" },
		{ vpiSwitch, "vpiSwitch" },
		{ vpiSysFuncCall, "vpiSysFuncCall" },
		{ vpiSysTaskCall, "vpiSysTaskCall" },
		{ vpiTableEntry, "vpiTableEntry" },
		{ vpiTask, "vpiTask" },
		{ vpiTaskCall, "vpiTaskCall" },
		{ vpiTchk, "vpiTchk" },
		{ vpiTchkTerm, "vpiTchkTerm" },
		{ vpiTimeVar, "vpiTimeVar" },
		{ vpiTimeQueue, "vpiTimeQueue" },
		{ vpiUdp, "vpiUdp" },
		{ vpiUdpDefn, "vpiUdpDefn" },
		{ vpiUserSystf, "vpiUserSystf" },
		{ vpiVarSelect, "vpiVarSelect" },
		{ vpiWait, "vpiWait" },
		{ vpiWhile, "vpiWhile" },
		/****************** object types added with 1364-2001 *********************/
		{ vpiAttribute, "vpiAttribute" },
		{ vpiBitSelect, "vpiBitSelect" },
		{ vpiCallback, "vpiCallback" },
		{ vpiDelayTerm, "vpiDelayTerm" },
		{ vpiDelayDevice, "vpiDelayDevice" },
		{ vpiFrame, "vpiFrame" },
		{ vpiGateArray, "vpiGateArray" },
		{ vpiModuleArray, "vpiModuleArray" },
		{ vpiPrimitiveArray, "vpiPrimitiveArray" },
		{ vpiNetArray, "vpiNetArray" },
		{ vpiRange, "vpiRange" },
		{ vpiRegArray, "vpiRegArray" },
		{ vpiSwitchArray, "vpiSwitchArray" },
		{ vpiUdpArray, "vpiUdpArray" },
		{ vpiContAssignBit, "vpiContAssignBit" },
		{ vpiNamedEventArray, "vpiNamedEventArray" },
		{ vpiIndexedPartSelect, "vpiIndexedPartSelect" },
		/* 1364-1995 */
		/******************************** METHODS *********************************/
		/************* methods used to traverse 1 to 1 relationships **************/
		{ vpiCondition, "vpiCondition" },
		{ vpiDelay, "vpiDelay" },
		{ vpiElseStmt, "vpiElseStmt" },
		{ vpiForIncStmt, "vpiForIncStmt" },
		{ vpiForInitStmt, "vpiForInitStmt" },
		{ vpiHighConn, "vpiHighConn" },
		{ vpiLhs, "vpiLhs" },
		{ vpiIndex, "vpiIndex" },
		{ vpiLeftRange, "vpiLeftRange" },
		{ vpiLowConn, "vpiLowConn" },
		{ vpiParent, "vpiParent" },
		{ vpiRhs, "vpiRhs" },
		{ vpiRightRange, "vpiRightRange" },
		{ vpiScope, "vpiScope" },
		{ vpiSysTfCall, "vpiSysTfCall" },
		{ vpiTchkDataTerm, "vpiTchkDataTerm" },
		{ vpiTchkNotifier, "vpiTchkNotifier" },
		{ vpiTchkRefTerm, "vpiTchkRefTerm" },
		/************ methods used to traverse 1 to many relationships ************/
		{ vpiArgument, "vpiArgument" },
		{ vpiBit, "vpiBit" },
		{ vpiDriver, "vpiDriver" },
		{ vpiInternalScope, "vpiInternalScope" },
		{ vpiLoad, "vpiLoad" },
		{ vpiModDataPathIn, "vpiModDataPathIn" },
		{ vpiModPathIn, "vpiModPathIn" },
		{ vpiModPathOut, "vpiModPathOut" },
		{ vpiOperand, "vpiOperand" },
		{ vpiPortInst, "vpiPortInst" },
		{ vpiProcess, "vpiProcess" },
		{ vpiVariables, "vpiVariables" },
		{ vpiUse, "vpiUse" },
		/***** methods which can traverse 1 to 1, or 1 to many relationships ******/
		{ vpiExpr, "vpiExpr" },
		{ vpiPrimitive, "vpiPrimitive" },
		{ vpiStmt, "vpiStmt" },
		/********************* methods added with 1364-2001 ***********************/
		{ vpiActiveTimeFormat, "vpiActiveTimeFormat" },
		{ vpiInTerm, "vpiInTerm" },
		{ vpiInstanceArray, "vpiInstanceArray" },
		{ vpiLocalDriver, "vpiLocalDriver" },
		{ vpiLocalLoad, "vpiLocalLoad" },
		{ vpiOutTerm, "vpiOutTerm" },
		{ vpiPorts, "vpiPorts" },
		{ vpiSimNet, "vpiSimNet" },
		{ vpiTaskFunc, "vpiTaskFunc" },
		{ vpiBaseExpr, "vpiBaseExpr" },
		{ vpiWidthExpr, "vpiWidthExpr" },
		/**************************************************************************/
#if !defined(__VAMS_ENABLE__)
	#warning __VAMS_ENABLE__ not defined: Disabling Verilog/AMS object types.
#else
		/* Verilog/AMS 2.3 -- not all of them are object types/methods? */
		{ vpiAnalogSysFunction, "vpiAnalogSysFunction" },
		{ vpiAnalogSysTask, "vpiAnalogSysTask" },
		{ vpiAnalogTime, "vpiAnalogTime" },
		{ vpiBranch, "vpiBranch" },
		{ vpiChild, "vpiChild" },
		{ vpiDerivative, "vpiDerivative" },
		{ vpiEndFrequency, "vpiEndFrequency" },
		{ vpiEndTime, "vpiEndTime" },
		{ vpiEquationTarget, "vpiEquationTarget" },
		{ vpiExpStrVal, "vpiExpStrVal" },
		{ vpiFlow, "vpiFlow" },
		{ vpiFlowNature, "vpiFlowNature" },
		{ vpiHiConn, "vpiHiConn" },
		{ vpiLoConn, "vpiLoConn" },
		{ vpiNegNode, "vpiNegNode" },
		{ vpiPosNode, "vpiPosNode" },
		{ vpiPotential, "vpiPotential" },
		{ vpiPotentialNature, "vpiPotentialNature" },
		{ vpiPureTransportDelayNo, "vpiPureTransportDelayNo" },
		{ vpiRejectTransientStep, "vpiRejectTransientStep" },
		{ vpiScaledTime, "vpiScaledTime" },
		{ vpiSource, "vpiSource" },
		{ vpiStartFrequency, "vpiStartFrequency" },
		{ vpiStartTime, "vpiStartTime" },
		{ vpiSysAnalogTask, "vpiSysAnalogTask" },
		{ vpiTransientFailConverge, "vpiTransientFailConverge" },
		{ vpiTransientMaxStep, "vpiTransientMaxStep" },
#endif
#if defined(__XVPI__)
		{ vpiAnalogBuiltinFuncCall, "vpiAnalogBuiltinFuncCall" },
		{ vpiAnalogFuncCall, "vpiAnalogFuncCall" },
		{ vpiAnalogFunction, "vpiAnalogFunction" },
		{ vpiAnalogFilterFuncCall, "vpiAnalogFilterFuncCall" },
		{ vpiAnalogSmallSignalFuncCall, "vpiAnalogSmallSignalFuncCall" },
		{ vpiAnalogSysTaskCall, "vpiAnalogSysTaskCall" },
		{ vpiAnalogSysFuncCall, "vpiAnalogSysFuncCall" },
		{ vpiAnalogUserSystf, "vpiAnalogUserSystf" },
		{ vpiAnalysisFuncCall, "vpiAnalysisFuncCall" },
		{ vpiBranchProbeFuncCall, "vpiBranchProbeFuncCall" },
		{ vpiContrib, "vpiContrib" },
		{ vpiDiscipline, "vpiDiscipline" },
		{ vpiNature, "vpiNature" },
		{ vpiNode, "vpiNode" },
		{ vpiPortProbeFuncCall, "vpiPortProbeFuncCall" },
		{ vpiValueRange, "vpiValueRange" },
		{ vpiValueRangeType, "vpiValueRangeType" },
		{ vpiDirect, "vpiDirect" },
		{ vpiGround, "vpiGround" },
		{ vpiUwire, "vpiUwire" },
		{ vpiWreal, "vpiWreal" },
		{ vpiAboveOp, "vpiAboveOp" },
		{ vpiCrossOp, "vpiCrossOp" },
		{ vpiFinalStepOp, "vpiFinalStepOp" },
		{ vpiInitialStepOp, "vpiInitialStepOp" },
		{ vpiTimerOp, "vpiTimerOp" },
		{ vpiRealtimeConst, "vpiRealtimeConst" },
		{ vpiTimeConst, "vpiTimeConst" },
		{ vpiRealtimeVar, "vpiRealtimeVar" },
		{ vpiFrom, "vpiFrom" },
		{ vpiExclude, "vpiExclude" },
		{ xvpiApplication, "xvpiApplication" },
		{ xvpiDimension, "xvpiDimension" },
#endif
	};
	static const struct enum_description vpi_property_types [] = {
		/******************************* PROPERTIES *******************************/
		/************************ generic object properties ***********************/
		{ vpiUndefined, "vpiUndefined" },
		{ vpiType, "vpiType" },
		{ vpiName, "vpiName" },
		{ vpiFullName, "vpiFullName" },
		{ vpiSize, "vpiSize" },
		{ vpiFile, "vpiFile" },
		{ vpiLineNo, "vpiLineNo" },
		/*************************** module properties ****************************/
		{ vpiTopModule, "vpiTopModule" },
		{ vpiCellInstance, "vpiCellInstance" },
		{ vpiDefName, "vpiDefName" },
		{ vpiProtected, "vpiProtected" },
		{ vpiTimeUnit, "vpiTimeUnit" },
		{ vpiTimePrecision, "vpiTimePrecision" },
		{ vpiDefNetType, "vpiDefNetType" },
		{ vpiUnconnDrive, "vpiUnconnDrive" },
		{ vpiDefFile, "vpiDefFile" },
		{ vpiDefLineNo, "vpiDefLineNo" },
		{ vpiDefDelayMode, "vpiDefDelayMode" },
		{ vpiDefDecayTime, "vpiDefDecayTime" },
		/************************ port and net properties *************************/
		{ vpiScalar, "vpiScalar" },
		{ vpiVector, "vpiVector" },
		{ vpiExplicitName, "vpiExplicitName" },
		{ vpiDirection, "vpiDirection" },
		{ vpiConnByName, "vpiConnByName" },
		{ vpiNetType, "vpiNetType" },
		{ vpiExplicitScalared, "vpiExplicitScalared" },
		{ vpiExplicitVectored, "vpiExplicitVectored" },
		{ vpiExpanded, "vpiExpanded" },
		{ vpiImplicitDecl, "vpiImplicitDecl" },
		{ vpiChargeStrength, "vpiChargeStrength" },
		{ vpiArray, "vpiArray" },
		{ vpiPortIndex, "vpiPortIndex" },
		/********************** gate and terminal properties **********************/
		{ vpiTermIndex, "vpiTermIndex" },
		{ vpiStrength0, "vpiStrength0" },
		{ vpiStrength1, "vpiStrength1" },
		{ vpiPrimType, "vpiPrimType" },
		/************** path, path terminal, timing check properties **************/
		{ vpiPolarity, "vpiPolarity" },
		{ vpiDataPolarity, "vpiDataPolarity" },
		{ vpiEdge, "vpiEdge" },
		{ vpiPathType, "vpiPathType" },
		{ vpiTchkType, "vpiTchkType" },
		/************************* expression properties **************************/
		{ vpiOpType, "vpiOpType" },
		{ vpiConstType, "vpiConstType" },
		{ vpiBlocking, "vpiBlocking" },
		{ vpiCaseType, "vpiCaseType" },
		{ vpiNetDeclAssign, "vpiNetDeclAssign" },
		/************** task/function properties **************/
		{ vpiFuncType, "vpiFuncType" },
		{ vpiUserDefn, "vpiUserDefn" },
		{ vpiScheduled, "vpiScheduled" },
		/*********************** properties added with 1364-2001 *******************/
		{ vpiActive, "vpiActive" },
		{ vpiAutomatic, "vpiAutomatic" },
		{ vpiCell, "vpiCell" },
		{ vpiConfig, "vpiConfig" },
		{ vpiConstantSelect, "vpiConstantSelect" },
		{ vpiDecompile, "vpiDecompile" },
		{ vpiDefAttribute, "vpiDefAttribute" },
		{ vpiDelayType, "vpiDelayType" },
		{ vpiIteratorType, "vpiIteratorType" },
		{ vpiLibrary, "vpiLibrary" },
		{ vpiMultiArray, "vpiMultiArray" },
		{ vpiOffset, "vpiOffset" },
		{ vpiResolvedNetType, "vpiResolvedNetType" },
		{ vpiSaveRestartID, "vpiSaveRestartID" },
		{ vpiSaveRestartLocation, "vpiSaveRestartLocation" },
		{ vpiValid, "vpiValid" },
		{ vpiSigned, "vpiSigned" },
		{ vpiLocalParam, "vpiLocalParam" },
		{ vpiModPathHasIfNone, "vpiModPathHasIfNone" },
		{ vpiIndexedPartSelectType, "vpiIndexedPartSelectType" },
		/************* vpi_control() constants (added with 1364-2001) *************/
		{ vpiStop, "vpiStop" },
		{ vpiFinish, "vpiFinish" },
		{ vpiReset, "vpiReset" },
		{ vpiSetInteractiveScope, "vpiSetInteractiveScope" },
#if !defined(__VAMS_ENABLE__)
	#warning __VAMS_ENABLE__ not defined: Disabling Verilog/AMS object types.
#else
		/* Verilog/AMS 2.3 -- which are object types/methods, which properties? */
		{ vpiAnalogSysFunction, "vpiAnalogSysFunction" },
		{ vpiAnalogSysTask, "vpiAnalogSysTask" },
		{ vpiAnalogTime, "vpiAnalogTime" },
		{ vpiBranch, "vpiBranch" },
		{ vpiChild, "vpiChild" },
		{ vpiDerivative, "vpiDerivative" },
		{ vpiEndFrequency, "vpiEndFrequency" },
		{ vpiEndTime, "vpiEndTime" },
		{ vpiEquationTarget, "vpiEquationTarget" },
		{ vpiExpStrVal, "vpiExpStrVal" },
		{ vpiFlow, "vpiFlow" },
		{ vpiFlowNature, "vpiFlowNature" },
		{ vpiHiConn, "vpiHiConn" },
		{ vpiLoConn, "vpiLoConn" },
		{ vpiNegNode, "vpiNegNode" },
		{ vpiPosNode, "vpiPosNode" },
		{ vpiPotential, "vpiPotential" },
		{ vpiPotentialNature, "vpiPotentialNature" },
		{ vpiPureTransportDelayNo, "vpiPureTransportDelayNo" },
		{ vpiRejectTransientStep, "vpiRejectTransientStep" },
		{ vpiScaledTime, "vpiScaledTime" },
		{ vpiSource, "vpiSource" },
		{ vpiStartFrequency, "vpiStartFrequency" },
		{ vpiStartTime, "vpiStartTime" },
		{ vpiSysAnalogTask, "vpiSysAnalogTask" },
		{ vpiTransientFailConverge, "vpiTransientFailConverge" },
		{ vpiTransientMaxStep, "vpiTransientMaxStep" },
#endif
#if defined(__XVPI__)
		{ vpiAnalogBuiltinFuncCall, "vpiAnalogBuiltinFuncCall" },
		{ vpiAnalogFuncCall, "vpiAnalogFuncCall" },
		{ vpiAnalogFunction, "vpiAnalogFunction" },
		{ vpiAnalogFilterFuncCall, "vpiAnalogFilterFuncCall" },
		{ vpiAnalogSmallSignalFuncCall, "vpiAnalogSmallSignalFuncCall" },
		{ vpiAnalogSysTaskCall, "vpiAnalogSysTaskCall" },
		{ vpiAnalogSysFuncCall, "vpiAnalogSysFuncCall" },
		{ vpiAnalogUserSystf, "vpiAnalogUserSystf" },
		{ vpiAnalysisFuncCall, "vpiAnalysisFuncCall" },
		{ vpiBranchProbeFuncCall, "vpiBranchProbeFuncCall" },
		{ vpiContrib, "vpiContrib" },
		{ vpiDiscipline, "vpiDiscipline" },
		{ vpiDomain, "vpiDomain" },
		{ vpiContinuous, "vpiContinuous" },
		{ vpiDiscrete, "vpiDiscrete" },
		{ vpiNature, "vpiNature" },
		{ vpiNode, "vpiNode" },
		{ vpiPortProbeFuncCall, "vpiPortProbeFuncCall" },
		{ vpiValueRange, "vpiValueRange" },
		{ vpiValueRangeType, "vpiValueRangeType" },
		{ vpiDirect, "vpiDirect" },
		{ vpiGround, "vpiGround" },
		{ vpiUwire, "vpiUwire" },
		{ vpiWreal, "vpiWreal" },
		{ vpiAboveOp, "vpiAboveOp" },
		{ vpiCrossOp, "vpiCrossOp" },
		{ vpiFinalStepOp, "vpiFinalStepOp" },
		{ vpiInitialStepOp, "vpiInitialStepOp" },
		{ vpiTimerOp, "vpiTimerOp" },
		{ vpiRealtimeConst, "vpiRealtimeConst" },
		{ vpiTimeConst, "vpiTimeConst" },
		{ vpiRealtimeVar, "vpiRealtimeVar" },
		{ vpiFrom, "vpiFrom" },
		{ vpiExclude, "vpiExclude" },
		{ xvpiValue, "xvpiValue" },
		{ xvpiBoolProp, "xvpiBoolProp" },
		{ xvpiIntProp, "xvpiIntProp" },
		{ xvpiRealProp, "xvpiRealProp" },
		{ xvpiStrProp, "xvpiStrProp" },
		{ xvpiPointerProp, "xvpiPointerProp" },
		{ xvpiApplicationPointer, "xvpiApplicationPointer" },
		{ xvpiCbData, "xvpiCbData" },
		{ xvpiSysTfData, "xvpiSysTfData" },
#endif
	};
	int i;

	if (level >= level_max) {
		vpi_printf("vpi_object_dump(): maximum recursion depth exceeded!\n");
		abort();
	}

	scope[level] = obj;

	if (obj) {
		/* try to dump properties of object */
		for (i=0; i<sizeof(vpi_property_types)/sizeof(vpi_property_types[0]); i++) {
			char *string = (char *) vpi_get_str(vpi_property_types[i].name, obj);
			int integer = (int) vpi_get(vpi_property_types[i].name, obj);
#if defined(__VAMS_ENABLE__)
			double real = vpi_get_real(vpi_property_types[i].name, obj);
#endif
#if defined(__XVPI__)
			void *pointer = xvpi_get_pointer(vpi_property_types[i].name, obj);
			if (pointer != NULL)
				vpi_printf("%*sptr  %s = %p\n", level * INDENT_WIDTH, "", vpi_property_types[i].string, pointer);
#endif
			if (string != NULL) {
				char *s = string;
				vpi_printf("%*sstr  %s = \"", level * INDENT_WIDTH, "", vpi_property_types[i].string);
				while (*s != '\0') {
					switch (*s) {
					case '"': vpi_printf("\\\""); break;
					case '\t': vpi_printf("\\t"); break;
					case '\r': vpi_printf("\\r"); break;
					case '\n': vpi_printf("\\n"); break;
					default: vpi_printf("%c", *s);
					}
					s++;
				}
				vpi_printf("\"\n");
			}
#if defined(__VAMS_ENABLE__)
			if (real != (double) vpiUndefined)
				vpi_printf("%*sreal %s = %g\n", level * INDENT_WIDTH, "", vpi_property_types[i].string, real);
#endif
			if (integer != vpiUndefined)
				vpi_printf("%*sint  %s = %d\n", level * INDENT_WIDTH, "", vpi_property_types[i].string, integer);
		}
	}

	/* try to dump childs as abstract objects in one-to-one or one-to-many relationships. */
	for (i=0; i<sizeof(vpi_object_types)/sizeof(vpi_object_types[0]); i++) {
		vpiHandle handle = vpi_handle(vpi_object_types[i].name, obj);
		vpiHandle iterator = vpi_iterate(vpi_object_types[i].name, obj);
		if (handle) {
			int j;
			for (j=0; j<level; j++) {
				if (scope[j] == handle) {
					vpi_printf("%*sobj  %s (%p) [reference to parent object]\n",
						level * INDENT_WIDTH, "", vpi_object_types[i].string, handle);
					break;
				}
			}
			if (j == level) {
				vpi_printf("%*sobj  %s (%p)\n", level * INDENT_WIDTH, "", vpi_object_types[i].string, handle);
				vpi_object_dump(handle, scope, level_max, level + 1);
			}
			vpi_free_object(handle);
		}
		if (iterator) {
			int idx = 0;
			int size = vpi_get(vpiSize, iterator);
			int type = vpi_get(vpiIteratorType, iterator);
			vpiHandle use = vpi_handle(vpiUse, iterator);
			vpiHandle scan_handle;
			vpi_printf("%*sitr  %s (%p, vpiType %d, vpiSize %d, vpiUse %p)\n",
				level * INDENT_WIDTH, "", vpi_object_types[i].string, iterator, type, size, (void *) use);
			level++;
			scope[level] = iterator;
			while ((scan_handle = vpi_scan(iterator)) != NULL) {
				int j;
				for (j=0; j<level; j++) {
					if (scope[j] == scan_handle) {
						vpi_printf("%*sobj  %s[%d] (%p) [reference to parent object]\n",
							level * INDENT_WIDTH, "", vpi_object_types[i].string, idx, scan_handle);
						break;
					}
				}
				if (j == level) {
					vpi_printf("%*sobj  %s[%d] (%p)\n",
						level * INDENT_WIDTH, "", vpi_object_types[i].string, idx, scan_handle);
					vpi_object_dump(scan_handle, scope, level_max, level + 1);
				}
				idx++;
			}
			level--;
		}
	}

	return 0;
}


static int end_of_compile_cb (s_cb_data *cb_data)
{
	vpiHandle scope [1000];
	vpi_printf("---------- VPI object dump at end_of_compile_cb() time -----------\n");
	vpi_object_dump(NULL, scope, sizeof(scope)/sizeof(scope[0]), 0);
	vpi_printf("--------------------- end of VPI object dump ---------------------\n");
	return 0;
}


static void register_cb (void)
{
	s_cb_data cb_data = {
		.reason = cbEndOfCompile,
		.cb_rtn = end_of_compile_cb,
	};
	vpi_register_cb(&cb_data);

	//vpiHandle scope [1000];
	//vpi_printf("------------- VPI object dump at register_cb() time --------------\n");
	//vpi_object_dump(NULL, scope, sizeof(scope)/sizeof(scope[0]), 0);
	//vpi_printf("--------------------- end of VPI object dump ---------------------\n");
}

void (*vlog_startup_routines []) (void) = {
	register_cb,
	NULL
};

void test_dump_vpi_obj(const int max_depth, void *root)
{
        vpiHandle scope [max_depth];
	vpi_printf("---------- VPI object dump For debugging -----------\n");
	vpi_object_dump((vpiHandle)root, scope, max_depth, 0);
	vpi_printf("--------------------- end of VPI object dump ---------------------\n");
}
