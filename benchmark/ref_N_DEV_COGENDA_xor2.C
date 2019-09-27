//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "xor2.h"

#include <N_DEV_Const.h>
#include <N_DEV_DeviceOptions.h>
#include <N_DEV_DeviceMaster.h>
#include <N_DEV_ExternData.h>
#include <N_DEV_SolverState.h>
#include <N_DEV_Message.h>

#include <N_LAS_Matrix.h>
#include <N_LAS_Vector.h>

  #include <N_ANP_NoiseData.h>
#include <N_UTL_FeatureTest.h>
#if defined(HAVE_UNORDERED_MAP)
#include <unordered_map>
using std::unordered_map;
#elif defined(HAVE_TR1_UNORDERED_MAP)
#include <tr1/unordered_map>
using std::tr1::unordered_map;
#else
#error neither unordered_map or tr1/unordered_map found!
#endif

//The C++ Main part of Model Implementation Starts here
namespace Xyce {
namespace Device {
namespace COGENDAxor2 {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_in1;
    const int Instance::cogendaNodeID_in2;
    const int Instance::cogendaNodeID_out;
    const int Instance::cogendaNodeID_out2;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_out2_GND;
    const int Instance::cogendaBRA_ID_out_out2;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAxor2::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAxor2::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("VHIGH",static_cast<double>(5.0), &COGENDAxor2::Instance::vhigh);
  p.addPar("VLOW",static_cast<double>(0.0), &COGENDAxor2::Instance::vlow);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAxor2::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAxor2::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("VHIGH",static_cast<double>(5.0), &COGENDAxor2::Model::vhigh);
  p.addPar("VLOW",static_cast<double>(0.0), &COGENDAxor2::Model::vlow);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("VHIGH"))
    vhigh = model_.vhigh;
  if(!given("VLOW"))
    vlow = model_.vlow;
  updateTemperature(cogendaInstTemp);
  return true;
}

Instance::Instance(
  const Configuration & configuration,
  const InstanceBlock & instance_block,
  Model &               model,
  const FactoryBlock &  factory_block)
  : DeviceInstance(instance_block, configuration.getInstanceParameters(), factory_block),
    model_(model),
    li_in1(-1),
    li_in2(-1),
    li_out(-1),
    li_out2(-1),
    li_BRA_out2_GND(-1),
    li_BRA_out_out2(-1),
    li_branch_iin1(-1),
    li_branch_iin2(-1),
    li_branch_iout(-1),
    f_out2_Equ_BRA_out2_GND_Var_Ptr(0),
    f_BRA_out2_GND_Equ_in1_Node_Ptr(0),
    f_BRA_out2_GND_Equ_in2_Node_Ptr(0),
    f_BRA_out2_GND_Equ_out2_Node_Ptr(0),
    f_GND_Equ_BRA_out2_GND_Var_Ptr(0),
    f_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr(0),
    f_out_Equ_BRA_out_out2_Var_Ptr(0),
    f_out2_Equ_BRA_out_out2_Var_Ptr(0),
    f_BRA_out_out2_Equ_out_Node_Ptr(0),
    f_BRA_out_out2_Equ_out2_Node_Ptr(0),
    f_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr(0),
    q_out2_Equ_BRA_out2_GND_Var_Ptr(0),
    q_BRA_out2_GND_Equ_in1_Node_Ptr(0),
    q_BRA_out2_GND_Equ_in2_Node_Ptr(0),
    q_BRA_out2_GND_Equ_out2_Node_Ptr(0),
    q_GND_Equ_BRA_out2_GND_Var_Ptr(0),
    q_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr(0),
    q_out_Equ_BRA_out_out2_Var_Ptr(0),
    q_out2_Equ_BRA_out_out2_Var_Ptr(0),
    q_BRA_out_out2_Equ_out_Node_Ptr(0),
    q_BRA_out_out2_Equ_out2_Node_Ptr(0),
    q_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr(0),
    m_out2_Equ_BRA_out2_GND_VarOffset(-1),
    m_BRA_out2_GND_Equ_in1_NodeOffset(-1),
    m_BRA_out2_GND_Equ_in2_NodeOffset(-1),
    m_BRA_out2_GND_Equ_out2_NodeOffset(-1),
    m_GND_Equ_BRA_out2_GND_VarOffset(-1),
    m_BRA_out2_GND_Equ_BRA_out2_GND_VarOffset(-1),
    m_out_Equ_BRA_out_out2_VarOffset(-1),
    m_out2_Equ_BRA_out_out2_VarOffset(-1),
    m_BRA_out_out2_Equ_out_NodeOffset(-1),
    m_BRA_out_out2_Equ_out2_NodeOffset(-1),
    m_BRA_out_out2_Equ_BRA_out_out2_VarOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 3;
  numIntVars = 3;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 3;
  leadCurrentF.resize(3);
  leadCurrentQ.resize(3);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_out2,cogendaBRA_ID_out2_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_in1));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_in2));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_out2));
  jacobianElements.push_back(IntPair(cogendaNodeID_GND,cogendaBRA_ID_out2_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out2_GND,cogendaBRA_ID_out2_GND));
  jacobianElements.push_back(IntPair(cogendaNodeID_out,cogendaBRA_ID_out_out2));
  jacobianElements.push_back(IntPair(cogendaNodeID_out2,cogendaBRA_ID_out_out2));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out_out2,cogendaNodeID_out));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out_out2,cogendaNodeID_out2));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out_out2,cogendaBRA_ID_out_out2));

  setDefaultParams();
  setParams(instance_block.params);

  if (!given("XYCEADMSINSTTEMP"))
    cogendaInstTemp=getDeviceOptions().temp.getImmutableValue<double>();

  updateDependentParameters();

  processParams();
  initInternalVars();

  PairVector collapsedNodes;
  collapseNodes();

  if (jacStamp.empty())
  {
    int originalSize = 6;
    computeJacStampAndMaps(jacobianElements,collapsedNodes,jacStamp,nodeMap,pairToJacStampMap,originalSize);
  }
}

  void Instance::collapseNodes() {
}

  Instance::~Instance(){}

  void Instance::registerLIDs( const LocalIdVector & intLIDVecRef,
                               const LocalIdVector & extLIDVecRef)
{
  AssertLIDs(intLIDVecRef.size() == numIntVars);
  AssertLIDs(extLIDVecRef.size() == numExtVars);
  LocalIdVector localLIDVec;

  intLIDVec = intLIDVecRef;
  extLIDVec = extLIDVecRef;
  localLIDVec.resize(numExtVars+numIntVars);
  for (int localNodeIndex=0;localNodeIndex<numExtVars;++localNodeIndex)
  {
    localLIDVec[localNodeIndex]=extLIDVec[localNodeIndex];
  }
  for (int localNodeIndex=numExtVars;localNodeIndex<numExtVars+numIntVars;++localNodeIndex)
  {
    localLIDVec[localNodeIndex]=intLIDVec[localNodeIndex-numExtVars];
  }

  li_in1 = localLIDVec[nodeMap[cogendaNodeID_in1]];
  li_in2 = localLIDVec[nodeMap[cogendaNodeID_in2]];
  li_out = localLIDVec[nodeMap[cogendaNodeID_out]];
  li_out2 = localLIDVec[nodeMap[cogendaNodeID_out2]];
  li_BRA_out2_GND = localLIDVec[nodeMap[cogendaBRA_ID_out2_GND]];
  li_BRA_out_out2 = localLIDVec[nodeMap[cogendaBRA_ID_out_out2]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_out2, getName(), "out2");
  addInternalNode(symbol_table, li_BRA_out2_GND, getName(), "out2_GND_branch");
  addInternalNode(symbol_table, li_BRA_out_out2, getName(), "out_out2_branch");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_iin1, getName(), "BRANCH_D1");
    addBranchDataNode( symbol_table, li_branch_iin2, getName(), "BRANCH_D2");
    addBranchDataNode( symbol_table, li_branch_iout, getName(), "BRANCH_D3");
  }
}

void Instance::registerStoreLIDs( const LocalIdVector & stoLIDVecRef)
{
  AssertLIDs(stoLIDVecRef.size() == getNumStoreVars());
}

void Instance::registerBranchDataLIDs(const std::vector<int> & branchLIDVecRef)
{
  AssertLIDs(branchLIDVecRef.size() == getNumBranchDataVars());
  if (loadLeadCurrent)
  {    
        int i = 0;
        li_branch_iin1 = branchLIDVecRef[i++];
        li_branch_iin2 = branchLIDVecRef[i++];
        li_branch_iout = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_out2,cogendaBRA_ID_out2_GND)];
  m_out2_Equ_BRA_out2_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_in1)];
  m_BRA_out2_GND_Equ_in1_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_in2)];
  m_BRA_out2_GND_Equ_in2_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out2_GND,cogendaNodeID_out2)];
  m_BRA_out2_GND_Equ_out2_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_GND,cogendaBRA_ID_out2_GND)];
  m_GND_Equ_BRA_out2_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out2_GND,cogendaBRA_ID_out2_GND)];
  m_BRA_out2_GND_Equ_BRA_out2_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_out,cogendaBRA_ID_out_out2)];
  m_out_Equ_BRA_out_out2_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_out2,cogendaBRA_ID_out_out2)];
  m_out2_Equ_BRA_out_out2_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out_out2,cogendaNodeID_out)];
  m_BRA_out_out2_Equ_out_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out_out2,cogendaNodeID_out2)];
  m_BRA_out_out2_Equ_out2_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out_out2,cogendaBRA_ID_out_out2)];
  m_BRA_out_out2_Equ_BRA_out_out2_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_out2_Equ_BRA_out2_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_out2,li_BRA_out2_GND);
  f_BRA_out2_GND_Equ_in1_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_in1);
  f_BRA_out2_GND_Equ_in2_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_in2);
  f_BRA_out2_GND_Equ_out2_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_out2);
  f_GND_Equ_BRA_out2_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_GND,li_BRA_out2_GND);
  f_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_BRA_out2_GND);
  f_out_Equ_BRA_out_out2_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_out,li_BRA_out_out2);
  f_out2_Equ_BRA_out_out2_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_out2,li_BRA_out_out2);
  f_BRA_out_out2_Equ_out_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_out);
  f_BRA_out_out2_Equ_out2_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_out2);
  f_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_BRA_out_out2);
  q_out2_Equ_BRA_out2_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_out2,li_BRA_out2_GND);
  q_BRA_out2_GND_Equ_in1_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_in1);
  q_BRA_out2_GND_Equ_in2_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_in2);
  q_BRA_out2_GND_Equ_out2_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_out2);
  q_GND_Equ_BRA_out2_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_GND,li_BRA_out2_GND);
  q_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out2_GND,li_BRA_out2_GND);
  q_out_Equ_BRA_out_out2_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_out,li_BRA_out_out2);
  q_out2_Equ_BRA_out_out2_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_out2,li_BRA_out_out2);
  q_BRA_out_out2_Equ_out_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_out);
  q_BRA_out_out2_Equ_out2_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_out2);
  q_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out_out2,li_BRA_out_out2);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_in1] += staticContributions[cogendaNodeID_in1].val();
  (*extData.daeFVectorPtr)[li_in2] += staticContributions[cogendaNodeID_in2].val();
  (*extData.daeFVectorPtr)[li_out] += staticContributions[cogendaNodeID_out].val();
  (*extData.daeFVectorPtr)[li_out2] += staticContributions[cogendaNodeID_out2].val();
  (*extData.daeFVectorPtr)[li_BRA_out2_GND] += staticContributions[cogendaBRA_ID_out2_GND].val();
  (*extData.daeFVectorPtr)[li_BRA_out_out2] += staticContributions[cogendaBRA_ID_out_out2].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_iin1] = leadCurrentF[cogendaNodeID_in1];
          leadF[li_branch_iin2] = leadCurrentF[cogendaNodeID_in2];
          leadF[li_branch_iout] = leadCurrentF[cogendaNodeID_out];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_in1] += dynamicContributions[cogendaNodeID_in1].val();
  (*extData.daeQVectorPtr)[li_in2] += dynamicContributions[cogendaNodeID_in2].val();
  (*extData.daeQVectorPtr)[li_out] += dynamicContributions[cogendaNodeID_out].val();
  (*extData.daeQVectorPtr)[li_out2] += dynamicContributions[cogendaNodeID_out2].val();
  (*extData.daeQVectorPtr)[li_BRA_out2_GND] += dynamicContributions[cogendaBRA_ID_out2_GND].val();
  (*extData.daeQVectorPtr)[li_BRA_out_out2] += dynamicContributions[cogendaBRA_ID_out_out2].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_iin1] = leadCurrentQ[cogendaNodeID_in1];
          leadQ[li_branch_iin2] = leadCurrentQ[cogendaNodeID_in2];
          leadQ[li_branch_iout] = leadCurrentQ[cogendaNodeID_out];
  }
  return true;
}

  //bool Instance::updatePrimaryState()
bool Instance::updatePrimaryState()
{
  bool bsuccess = true;
  initInternalVars();
  bsuccess = updateIntermediateVars();
  return bsuccess;
}

  //bool Instance::updateSecondaryState()
bool Instance::updateSecondaryState()
{
  bool bsuccess = true;
  return bsuccess;
}

  //bool Instance::updateIntermediateVars
bool Instance::updateIntermediateVars()
{
  bool bsuccess=true;
  Linear::Vector * solVectorPtr = extData.nextSolVectorPtr;
  // set the sizes of the Fad arrays:
  if (probeVars.size() != (4))
  {
    probeVars.resize(4);
    staticContributions.resize(4+2);
    dynamicContributions.resize(4+2);
  }

  for (int i=0; i < 4+2 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_I_out2_GND] = (*solVectorPtr)[li_BRA_out2_GND];
  probeVars[cogendaProbeID_I_out2_GND].diff(cogendaProbeID_I_out2_GND,4);
  probeVars[cogendaProbeID_I_out_out2] = (*solVectorPtr)[li_BRA_out_out2];
  probeVars[cogendaProbeID_I_out_out2].diff(cogendaProbeID_I_out_out2,4);
  probeVars[cogendaProbeID_V_in1_GND] = (*solVectorPtr)[li_in1];
  probeVars[cogendaProbeID_V_in1_GND].diff(cogendaProbeID_V_in1_GND,4);
  probeVars[cogendaProbeID_V_in2_GND] = (*solVectorPtr)[li_in2];
  probeVars[cogendaProbeID_V_in2_GND].diff(cogendaProbeID_V_in2_GND,4);

{
  b1 = cogenda_ternary_op<CogendaFadType>((probeVars[cogendaProbeID_V_in1_GND] >= (this->vhigh/2.0)),1,0);
  b2 = cogenda_ternary_op<CogendaFadType>((probeVars[cogendaProbeID_V_in2_GND] >= (this->vhigh/2.0)),1,0);
//V-contrib...
  staticContributions[cogendaBRA_ID_out2_GND] += cogenda_ternary_op<CogendaFadType>((b1
//V-contrib...
  staticContributions[cogendaBRA_ID_out2_GND] += (1.0e+3*probeVars[cogendaProbeID_I_out2_GND]);
//V-contrib...
  staticContributions[cogendaBRA_ID_out_out2] += probeVars[cogendaProbeID_I_out_out2];
}
staticContributions[cogendaNodeID_out2] += probeVars[cogendaProbeID_I_out2_GND];
staticContributions[cogendaBRA_ID_out2_GND] -= (*solVectorPtr)[li_out2];
staticContributions[cogendaNodeID_out] += probeVars[cogendaProbeID_I_out_out2];
staticContributions[cogendaNodeID_out2] -= probeVars[cogendaProbeID_I_out_out2];
staticContributions[cogendaBRA_ID_out_out2] -= (*solVectorPtr)[li_out]-(*solVectorPtr)[li_out2];
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 3 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 4; unCtNode++) {
      if (nodeMap[unCtNode] < 3 && nodeMap[unCtNode] != -1 ) {
        leadCurrentF[nodeMap[unCtNode]] += staticContributions[unCtNode].val();
        leadCurrentQ[nodeMap[unCtNode]] += dynamicContributions[unCtNode].val();
      }
    }
  }
  return true;
}

  //bool Instance::loadDAEdFdx //Load the dFdx static jacobian matrix
bool Instance::loadDAEdFdx()
{
  bool bsuccess = true;
  Linear::Matrix & dFdx = *(extData.dFdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*f_out2_Equ_BRA_out2_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out2_GND);
  (*f_BRA_out2_GND_Equ_in1_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_V_in1_GND);
  (*f_BRA_out2_GND_Equ_in2_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_V_in2_GND);
  (*f_BRA_out2_GND_Equ_out2_Node_Ptr) +=  -1;
  (*f_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr) +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_I_out2_GND);
  (*f_out_Equ_BRA_out_out2_Var_Ptr) +=  +staticContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_out2);
  (*f_out2_Equ_BRA_out_out2_Var_Ptr) +=  +staticContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out_out2);
  (*f_BRA_out_out2_Equ_out_Node_Ptr) +=  -1;
  (*f_BRA_out_out2_Equ_out2_Node_Ptr) +=  +1;
  (*f_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr) +=  +staticContributions[cogendaBRA_ID_out_out2].dx(cogendaProbeID_I_out_out2);
#else
  //use the offsets instead of pointers
  dFdx[li_out2][m_out2_Equ_BRA_out2_GND_VarOffset] +=  +staticContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out2_GND);
  dFdx[li_BRA_out2_GND][m_BRA_out2_GND_Equ_in1_NodeOffset] +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_V_in1_GND);
  dFdx[li_BRA_out2_GND][m_BRA_out2_GND_Equ_in2_NodeOffset] +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_V_in2_GND);
  dFdx[li_BRA_out2_GND][m_BRA_out2_GND_Equ_out2_NodeOffset] +=  -1;
  dFdx[li_BRA_out2_GND][m_BRA_out2_GND_Equ_BRA_out2_GND_VarOffset] +=  +staticContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_I_out2_GND);
  dFdx[li_out][m_out_Equ_BRA_out_out2_VarOffset] +=  +staticContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_out2);
  dFdx[li_out2][m_out2_Equ_BRA_out_out2_VarOffset] +=  +staticContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out_out2);
  dFdx[li_BRA_out_out2][m_BRA_out_out2_Equ_out_NodeOffset] +=  -1;
  dFdx[li_BRA_out_out2][m_BRA_out_out2_Equ_out2_NodeOffset] +=  +1;
  dFdx[li_BRA_out_out2][m_BRA_out_out2_Equ_BRA_out_out2_VarOffset] +=  +staticContributions[cogendaBRA_ID_out_out2].dx(cogendaProbeID_I_out_out2);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_out2_Equ_BRA_out2_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out2_GND);
  (*q_BRA_out2_GND_Equ_BRA_out2_GND_Var_Ptr) +=  +dynamicContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_I_out2_GND);
  (*q_out_Equ_BRA_out_out2_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_out2);
  (*q_out2_Equ_BRA_out_out2_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out_out2);
  (*q_BRA_out_out2_Equ_BRA_out_out2_Var_Ptr) +=  +dynamicContributions[cogendaBRA_ID_out_out2].dx(cogendaProbeID_I_out_out2);
#else
  //use the offsets instead of pointers
  dQdx[li_out2][m_out2_Equ_BRA_out2_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out2_GND);
  dQdx[li_BRA_out2_GND][m_BRA_out2_GND_Equ_BRA_out2_GND_VarOffset] +=  +dynamicContributions[cogendaBRA_ID_out2_GND].dx(cogendaProbeID_I_out2_GND);
  dQdx[li_out][m_out_Equ_BRA_out_out2_VarOffset] +=  +dynamicContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_out2);
  dQdx[li_out2][m_out2_Equ_BRA_out_out2_VarOffset] +=  +dynamicContributions[cogendaNodeID_out2].dx(cogendaProbeID_I_out_out2);
  dQdx[li_BRA_out_out2][m_BRA_out_out2_Equ_BRA_out_out2_VarOffset] +=  +dynamicContributions[cogendaBRA_ID_out_out2].dx(cogendaProbeID_I_out_out2);
#endif
  return bsuccess;
}

bool Instance::updateTemperature(const double & temperatureTemp)
{
  cogendaTemperature = temperatureTemp;
  cogenda_vt_nom = _VT0_(temperatureTemp);
  return true;
}
/* class Model member functions */
  //bool Model::processParams
  bool Model::processParams()
  {
  if(!given("VHIGH"))
    vhigh = 5.0;
  if(!given("VLOW"))
    vlow = 0.0;
}

  //bool Model::processInstanceParams
bool Model::processInstanceParams()
{
  std::vector<Instance*>::iterator iter;
  std::vector<Instance*>::iterator first = instanceContainer.begin();
  std::vector<Instance*>::iterator last  = instanceContainer.end();
  for (iter=first; iter!=last; ++iter)
  {
    (*iter)->processParams();
  }
  return true;
}

  //Model::Model(..){...}
Model::Model(
  const Configuration & configuration,
  const ModelBlock &    model_block,
  const FactoryBlock &  factory_block)
  : DeviceModel(model_block, configuration.getModelParameters(), factory_block),
    vhigh(5.0),
    vlow(0.0)
{
  setDefaultParams();
  setModParams(model_block.params);
  if (!given("XYCEADMSMODTEMP"))
    cogendaModTemp=getDeviceOptions().temp.getImmutableValue<double>();
  updateDependentParameters();
  processParams();
}

  //Model::~Model
Model::~Model()
{
  std::vector<Instance*>::iterator iterI;
  std::vector<Instance*>::iterator firstI = instanceContainer.begin ();
  std::vector<Instance*>::iterator lastI  = instanceContainer.end ();
  for (iterI = firstI; iterI != lastI; ++iterI)
    delete (*iterI);
}

  //std::ostream &Model::printOutInstances
std::ostream &Model::printOutInstances(std::ostream &os) const
{
  std::vector<Instance*>::const_iterator iter;
  std::vector<Instance*>::const_iterator first = instanceContainer.begin();
  std::vector<Instance*>::const_iterator last  = instanceContainer.end();
  int i;
  os << std::endl;
  os << "  name   model name  Parameters" << std::endl;
  for (i=0, iter=first; iter!=last; ++iter, ++i)
  {
    os << "  " << i << ": " << (*iter)->getName() << "      ";
    os << getName();
    os << std::endl;
    os << std::endl;
  }
  os << std::endl;
  return os;
}

  //void Model::forEachInstance()
void Model::forEachInstance(DeviceInstanceOp &op) const 
{
  for (std::vector<Instance *>::const_iterator it = instanceContainer.begin(); it != instanceContainer.end(); ++it)
    op(*it);
}

/* class Traits member functions */
  //Device *Traits::factory()
Device *Traits::factory(const Configuration &configuration, const FactoryBlock &factory_block)
  {
    return new DeviceMaster<Traits>(configuration, factory_block, factory_block.solverState_, factory_block.deviceOptions_);
}
void registerDevice()
{
  Config<Traits>::addConfiguration()
    .registerDevice("xor2", 1)
    .registerModelType("xor2", 1);
}
} // namespace COGENDAxor2
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAxor2::registerDevice(); }}; 
Bootstrap COGENDAxor2_bootstrap;

