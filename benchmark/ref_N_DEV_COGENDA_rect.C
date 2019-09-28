//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "rect.h"

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
namespace COGENDArect {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_acp;
    const int Instance::cogendaNodeID_acn;
    const int Instance::cogendaNodeID_dcp;
    const int Instance::cogendaNodeID_dcn;
    const int Instance::cogendaNodeID_dcpp;
    const int Instance::cogendaNodeID_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDArect::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDArect::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("I0",static_cast<double>(1e-6), &COGENDArect::Instance::I0);
  p.addPar("C",static_cast<double>(1e-12), &COGENDArect::Instance::c);
  p.addPar("R",static_cast<double>(100), &COGENDArect::Instance::r);
  p.addPar("VTH",static_cast<double>(0.0257), &COGENDArect::Instance::vth);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDArect::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDArect::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("I0",static_cast<double>(1e-6), &COGENDArect::Model::I0);
  p.addPar("C",static_cast<double>(1e-12), &COGENDArect::Model::c);
  p.addPar("R",static_cast<double>(100), &COGENDArect::Model::r);
  p.addPar("VTH",static_cast<double>(0.0257), &COGENDArect::Model::vth);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("I0"))
    I0 = model_.I0;
  if(!given("C"))
    c = model_.c;
  if(!given("R"))
    r = model_.r;
  if(!given("VTH"))
    vth = model_.vth;
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
    li_acp(-1),
    li_acn(-1),
    li_dcp(-1),
    li_dcn(-1),
    li_dcpp(-1),
    li_branch_iacp(-1),
    li_branch_iacn(-1),
    li_branch_idcp(-1),
    li_branch_idcn(-1),
    f_acp_Equ_acp_Node_Ptr(0),
    f_acp_Equ_dcpp_Node_Ptr(0),
    f_dcpp_Equ_acp_Node_Ptr(0),
    f_dcpp_Equ_dcpp_Node_Ptr(0),
    f_acn_Equ_acn_Node_Ptr(0),
    f_acn_Equ_dcpp_Node_Ptr(0),
    f_dcpp_Equ_acn_Node_Ptr(0),
    f_dcn_Equ_dcn_Node_Ptr(0),
    f_dcn_Equ_acp_Node_Ptr(0),
    f_acp_Equ_dcn_Node_Ptr(0),
    f_dcn_Equ_acn_Node_Ptr(0),
    f_acn_Equ_dcn_Node_Ptr(0),
    f_dcpp_Equ_dcp_Node_Ptr(0),
    f_dcp_Equ_dcpp_Node_Ptr(0),
    f_dcp_Equ_dcp_Node_Ptr(0),
    f_dcpp_Equ_dcn_Node_Ptr(0),
    f_dcn_Equ_dcpp_Node_Ptr(0),
    f_dcp_Equ_dcn_Node_Ptr(0),
    f_dcn_Equ_dcp_Node_Ptr(0),
    q_acp_Equ_acp_Node_Ptr(0),
    q_acp_Equ_dcpp_Node_Ptr(0),
    q_dcpp_Equ_acp_Node_Ptr(0),
    q_dcpp_Equ_dcpp_Node_Ptr(0),
    q_acn_Equ_acn_Node_Ptr(0),
    q_acn_Equ_dcpp_Node_Ptr(0),
    q_dcpp_Equ_acn_Node_Ptr(0),
    q_dcn_Equ_dcn_Node_Ptr(0),
    q_dcn_Equ_acp_Node_Ptr(0),
    q_acp_Equ_dcn_Node_Ptr(0),
    q_dcn_Equ_acn_Node_Ptr(0),
    q_acn_Equ_dcn_Node_Ptr(0),
    q_dcpp_Equ_dcp_Node_Ptr(0),
    q_dcp_Equ_dcpp_Node_Ptr(0),
    q_dcp_Equ_dcp_Node_Ptr(0),
    q_dcpp_Equ_dcn_Node_Ptr(0),
    q_dcn_Equ_dcpp_Node_Ptr(0),
    q_dcp_Equ_dcn_Node_Ptr(0),
    q_dcn_Equ_dcp_Node_Ptr(0),
    m_acp_Equ_acp_NodeOffset(-1),
    m_acp_Equ_dcpp_NodeOffset(-1),
    m_dcpp_Equ_acp_NodeOffset(-1),
    m_dcpp_Equ_dcpp_NodeOffset(-1),
    m_acn_Equ_acn_NodeOffset(-1),
    m_acn_Equ_dcpp_NodeOffset(-1),
    m_dcpp_Equ_acn_NodeOffset(-1),
    m_dcn_Equ_dcn_NodeOffset(-1),
    m_dcn_Equ_acp_NodeOffset(-1),
    m_acp_Equ_dcn_NodeOffset(-1),
    m_dcn_Equ_acn_NodeOffset(-1),
    m_acn_Equ_dcn_NodeOffset(-1),
    m_dcpp_Equ_dcp_NodeOffset(-1),
    m_dcp_Equ_dcpp_NodeOffset(-1),
    m_dcp_Equ_dcp_NodeOffset(-1),
    m_dcpp_Equ_dcn_NodeOffset(-1),
    m_dcn_Equ_dcpp_NodeOffset(-1),
    m_dcp_Equ_dcn_NodeOffset(-1),
    m_dcn_Equ_dcp_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 4;
  numIntVars = 1;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 4;
  leadCurrentF.resize(4);
  leadCurrentQ.resize(4);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_acp,cogendaNodeID_acp));
  jacobianElements.push_back(IntPair(cogendaNodeID_acp,cogendaNodeID_dcpp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcpp,cogendaNodeID_acp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcpp));
  jacobianElements.push_back(IntPair(cogendaNodeID_acn,cogendaNodeID_acn));
  jacobianElements.push_back(IntPair(cogendaNodeID_acn,cogendaNodeID_dcpp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcpp,cogendaNodeID_acn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcn,cogendaNodeID_dcn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcn,cogendaNodeID_acp));
  jacobianElements.push_back(IntPair(cogendaNodeID_acp,cogendaNodeID_dcn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcn,cogendaNodeID_acn));
  jacobianElements.push_back(IntPair(cogendaNodeID_acn,cogendaNodeID_dcn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcp,cogendaNodeID_dcpp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcp,cogendaNodeID_dcp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcn,cogendaNodeID_dcpp));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcp,cogendaNodeID_dcn));
  jacobianElements.push_back(IntPair(cogendaNodeID_dcn,cogendaNodeID_dcp));

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
    int originalSize = 5;
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

  li_acp = localLIDVec[nodeMap[cogendaNodeID_acp]];
  li_acn = localLIDVec[nodeMap[cogendaNodeID_acn]];
  li_dcp = localLIDVec[nodeMap[cogendaNodeID_dcp]];
  li_dcn = localLIDVec[nodeMap[cogendaNodeID_dcn]];
  li_dcpp = localLIDVec[nodeMap[cogendaNodeID_dcpp]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_dcpp, getName(), "dcpp");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_iacp, getName(), "BRANCH_D1");
    addBranchDataNode( symbol_table, li_branch_iacn, getName(), "BRANCH_D2");
    addBranchDataNode( symbol_table, li_branch_idcp, getName(), "BRANCH_D3");
    addBranchDataNode( symbol_table, li_branch_idcn, getName(), "BRANCH_D4");
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
        li_branch_iacp = branchLIDVecRef[i++];
        li_branch_iacn = branchLIDVecRef[i++];
        li_branch_idcp = branchLIDVecRef[i++];
        li_branch_idcn = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acp,cogendaNodeID_acp)];
  m_acp_Equ_acp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acp,cogendaNodeID_dcpp)];
  m_acp_Equ_dcpp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcpp,cogendaNodeID_acp)];
  m_dcpp_Equ_acp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcpp)];
  m_dcpp_Equ_dcpp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acn,cogendaNodeID_acn)];
  m_acn_Equ_acn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acn,cogendaNodeID_dcpp)];
  m_acn_Equ_dcpp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcpp,cogendaNodeID_acn)];
  m_dcpp_Equ_acn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcn,cogendaNodeID_dcn)];
  m_dcn_Equ_dcn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcn,cogendaNodeID_acp)];
  m_dcn_Equ_acp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acp,cogendaNodeID_dcn)];
  m_acp_Equ_dcn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcn,cogendaNodeID_acn)];
  m_dcn_Equ_acn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_acn,cogendaNodeID_dcn)];
  m_acn_Equ_dcn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcp)];
  m_dcpp_Equ_dcp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcp,cogendaNodeID_dcpp)];
  m_dcp_Equ_dcpp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcp,cogendaNodeID_dcp)];
  m_dcp_Equ_dcp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcpp,cogendaNodeID_dcn)];
  m_dcpp_Equ_dcn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcn,cogendaNodeID_dcpp)];
  m_dcn_Equ_dcpp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcp,cogendaNodeID_dcn)];
  m_dcp_Equ_dcn_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dcn,cogendaNodeID_dcp)];
  m_dcn_Equ_dcp_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_acp_Equ_acp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acp,li_acp);
  f_acp_Equ_dcpp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acp,li_dcpp);
  f_dcpp_Equ_acp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcpp,li_acp);
  f_dcpp_Equ_dcpp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcpp);
  f_acn_Equ_acn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acn,li_acn);
  f_acn_Equ_dcpp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acn,li_dcpp);
  f_dcpp_Equ_acn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcpp,li_acn);
  f_dcn_Equ_dcn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcn,li_dcn);
  f_dcn_Equ_acp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcn,li_acp);
  f_acp_Equ_dcn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acp,li_dcn);
  f_dcn_Equ_acn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcn,li_acn);
  f_acn_Equ_dcn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_acn,li_dcn);
  f_dcpp_Equ_dcp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcp);
  f_dcp_Equ_dcpp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcp,li_dcpp);
  f_dcp_Equ_dcp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcp,li_dcp);
  f_dcpp_Equ_dcn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcn);
  f_dcn_Equ_dcpp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcn,li_dcpp);
  f_dcp_Equ_dcn_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcp,li_dcn);
  f_dcn_Equ_dcp_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dcn,li_dcp);
  q_acp_Equ_acp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acp,li_acp);
  q_acp_Equ_dcpp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acp,li_dcpp);
  q_dcpp_Equ_acp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcpp,li_acp);
  q_dcpp_Equ_dcpp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcpp);
  q_acn_Equ_acn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acn,li_acn);
  q_acn_Equ_dcpp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acn,li_dcpp);
  q_dcpp_Equ_acn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcpp,li_acn);
  q_dcn_Equ_dcn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcn,li_dcn);
  q_dcn_Equ_acp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcn,li_acp);
  q_acp_Equ_dcn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acp,li_dcn);
  q_dcn_Equ_acn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcn,li_acn);
  q_acn_Equ_dcn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_acn,li_dcn);
  q_dcpp_Equ_dcp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcp);
  q_dcp_Equ_dcpp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcp,li_dcpp);
  q_dcp_Equ_dcp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcp,li_dcp);
  q_dcpp_Equ_dcn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcpp,li_dcn);
  q_dcn_Equ_dcpp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcn,li_dcpp);
  q_dcp_Equ_dcn_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcp,li_dcn);
  q_dcn_Equ_dcp_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dcn,li_dcp);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_acp] += staticContributions[cogendaNodeID_acp].val();
  (*extData.daeFVectorPtr)[li_acn] += staticContributions[cogendaNodeID_acn].val();
  (*extData.daeFVectorPtr)[li_dcp] += staticContributions[cogendaNodeID_dcp].val();
  (*extData.daeFVectorPtr)[li_dcn] += staticContributions[cogendaNodeID_dcn].val();
  (*extData.daeFVectorPtr)[li_dcpp] += staticContributions[cogendaNodeID_dcpp].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_iacp] = leadCurrentF[cogendaNodeID_acp];
          leadF[li_branch_iacn] = leadCurrentF[cogendaNodeID_acn];
          leadF[li_branch_idcp] = leadCurrentF[cogendaNodeID_dcp];
          leadF[li_branch_idcn] = leadCurrentF[cogendaNodeID_dcn];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_acp] += dynamicContributions[cogendaNodeID_acp].val();
  (*extData.daeQVectorPtr)[li_acn] += dynamicContributions[cogendaNodeID_acn].val();
  (*extData.daeQVectorPtr)[li_dcp] += dynamicContributions[cogendaNodeID_dcp].val();
  (*extData.daeQVectorPtr)[li_dcn] += dynamicContributions[cogendaNodeID_dcn].val();
  (*extData.daeQVectorPtr)[li_dcpp] += dynamicContributions[cogendaNodeID_dcpp].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_iacp] = leadCurrentQ[cogendaNodeID_acp];
          leadQ[li_branch_iacn] = leadCurrentQ[cogendaNodeID_acn];
          leadQ[li_branch_idcp] = leadCurrentQ[cogendaNodeID_dcp];
          leadQ[li_branch_idcn] = leadCurrentQ[cogendaNodeID_dcn];
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
  if (probeVars.size() != (7))
  {
    probeVars.resize(7);
    staticContributions.resize(5+0);
    dynamicContributions.resize(5+0);
  }

  for (int i=0; i < 5+0 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_V_acp_dcpp] = (*solVectorPtr)[li_acp] - (*solVectorPtr)[li_dcpp];
  probeVars[cogendaProbeID_V_acp_dcpp].diff(cogendaProbeID_V_acp_dcpp,7);
  probeVars[cogendaProbeID_V_acn_dcpp] = (*solVectorPtr)[li_acn] - (*solVectorPtr)[li_dcpp];
  probeVars[cogendaProbeID_V_acn_dcpp].diff(cogendaProbeID_V_acn_dcpp,7);
  probeVars[cogendaProbeID_V_dcn_acp] = (*solVectorPtr)[li_dcn] - (*solVectorPtr)[li_acp];
  probeVars[cogendaProbeID_V_dcn_acp].diff(cogendaProbeID_V_dcn_acp,7);
  probeVars[cogendaProbeID_V_dcn_acn] = (*solVectorPtr)[li_dcn] - (*solVectorPtr)[li_acn];
  probeVars[cogendaProbeID_V_dcn_acn].diff(cogendaProbeID_V_dcn_acn,7);
  probeVars[cogendaProbeID_V_dcpp_dcp] = (*solVectorPtr)[li_dcpp] - (*solVectorPtr)[li_dcp];
  probeVars[cogendaProbeID_V_dcpp_dcp].diff(cogendaProbeID_V_dcpp_dcp,7);
  probeVars[cogendaProbeID_V_dcpp_dcn] = (*solVectorPtr)[li_dcpp] - (*solVectorPtr)[li_dcn];
  probeVars[cogendaProbeID_V_dcpp_dcn].diff(cogendaProbeID_V_dcpp_dcn,7);
  probeVars[cogendaProbeID_V_dcp_dcn] = (*solVectorPtr)[li_dcp] - (*solVectorPtr)[li_dcn];
  probeVars[cogendaProbeID_V_dcp_dcn].diff(cogendaProbeID_V_dcp_dcn,7);

{
//I-contrib...
  staticContributions[cogendaNodeID_acp] += (this->I0*(exp((probeVars[cogendaProbeID_V_acp_dcpp]/this->vth))-1.0));
  staticContributions[cogendaNodeID_dcpp] -= (this->I0*(exp((probeVars[cogendaProbeID_V_acp_dcpp]/this->vth))-1.0));
//I-contrib...
  staticContributions[cogendaNodeID_acn] += (this->I0*(exp((probeVars[cogendaProbeID_V_acn_dcpp]/this->vth))-1.0));
  staticContributions[cogendaNodeID_dcpp] -= (this->I0*(exp((probeVars[cogendaProbeID_V_acn_dcpp]/this->vth))-1.0));
//I-contrib...
  staticContributions[cogendaNodeID_dcn] += (this->I0*(exp((probeVars[cogendaProbeID_V_dcn_acp]/this->vth))-1.0));
  staticContributions[cogendaNodeID_acp] -= (this->I0*(exp((probeVars[cogendaProbeID_V_dcn_acp]/this->vth))-1.0));
//I-contrib...
  staticContributions[cogendaNodeID_dcn] += (this->I0*(exp((probeVars[cogendaProbeID_V_dcn_acn]/this->vth))-1.0));
  staticContributions[cogendaNodeID_acn] -= (this->I0*(exp((probeVars[cogendaProbeID_V_dcn_acn]/this->vth))-1.0));
//I-contrib...
  staticContributions[cogendaNodeID_dcpp] += (probeVars[cogendaProbeID_V_dcpp_dcp]/this->r);
  staticContributions[cogendaNodeID_dcp] -= (probeVars[cogendaProbeID_V_dcpp_dcp]/this->r);
//Q-contrib...
  dynamicContributions[cogendaNodeID_dcpp] += (this->c*(probeVars[cogendaProbeID_V_dcpp_dcn]));
  dynamicContributions[cogendaNodeID_dcn] -= (this->c*(probeVars[cogendaProbeID_V_dcpp_dcn]));
//Q-contrib...
  dynamicContributions[cogendaNodeID_dcp] += (this->c*(probeVars[cogendaProbeID_V_dcp_dcn]));
  dynamicContributions[cogendaNodeID_dcn] -= (this->c*(probeVars[cogendaProbeID_V_dcp_dcn]));
}
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 4 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 5; unCtNode++) {
      if (nodeMap[unCtNode] < 4 && nodeMap[unCtNode] != -1 ) {
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
  (*f_acn_Equ_acn_Node_Ptr) +=  +staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_acn_dcpp)  -staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_dcn_acn);
  (*f_acn_Equ_dcn_Node_Ptr) +=  +staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_dcn_acn);
  (*f_acn_Equ_dcpp_Node_Ptr) +=  -staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_acn_dcpp);
  (*f_acp_Equ_acp_Node_Ptr) +=  +staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_acp_dcpp)  -staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_dcn_acp);
  (*f_acp_Equ_dcn_Node_Ptr) +=  +staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_dcn_acp);
  (*f_acp_Equ_dcpp_Node_Ptr) +=  -staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_acp_dcpp);
  (*f_dcn_Equ_acn_Node_Ptr) +=  -staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acn);
  (*f_dcn_Equ_acp_Node_Ptr) +=  -staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acp);
  (*f_dcn_Equ_dcn_Node_Ptr) +=  +staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acp)  +staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acn);
  (*f_dcp_Equ_dcp_Node_Ptr) +=  -staticContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcpp_dcp);
  (*f_dcp_Equ_dcpp_Node_Ptr) +=  +staticContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcpp_dcp);
  (*f_dcpp_Equ_acn_Node_Ptr) +=  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acn_dcpp);
  (*f_dcpp_Equ_acp_Node_Ptr) +=  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acp_dcpp);
  (*f_dcpp_Equ_dcp_Node_Ptr) +=  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcp);
  (*f_dcpp_Equ_dcpp_Node_Ptr) +=  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acp_dcpp)  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acn_dcpp)  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcp);
#else
  //use the offsets instead of pointers
  dFdx[li_acn][m_acn_Equ_acn_NodeOffset] +=  +staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_acn_dcpp)  -staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_dcn_acn);
  dFdx[li_acn][m_acn_Equ_dcn_NodeOffset] +=  +staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_dcn_acn);
  dFdx[li_acn][m_acn_Equ_dcpp_NodeOffset] +=  -staticContributions[cogendaNodeID_acn].dx(cogendaProbeID_V_acn_dcpp);
  dFdx[li_acp][m_acp_Equ_acp_NodeOffset] +=  +staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_acp_dcpp)  -staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_dcn_acp);
  dFdx[li_acp][m_acp_Equ_dcn_NodeOffset] +=  +staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_dcn_acp);
  dFdx[li_acp][m_acp_Equ_dcpp_NodeOffset] +=  -staticContributions[cogendaNodeID_acp].dx(cogendaProbeID_V_acp_dcpp);
  dFdx[li_dcn][m_dcn_Equ_acn_NodeOffset] +=  -staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acn);
  dFdx[li_dcn][m_dcn_Equ_acp_NodeOffset] +=  -staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acp);
  dFdx[li_dcn][m_dcn_Equ_dcn_NodeOffset] +=  +staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acp)  +staticContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcn_acn);
  dFdx[li_dcp][m_dcp_Equ_dcp_NodeOffset] +=  -staticContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcpp_dcp);
  dFdx[li_dcp][m_dcp_Equ_dcpp_NodeOffset] +=  +staticContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcpp_dcp);
  dFdx[li_dcpp][m_dcpp_Equ_acn_NodeOffset] +=  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acn_dcpp);
  dFdx[li_dcpp][m_dcpp_Equ_acp_NodeOffset] +=  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acp_dcpp);
  dFdx[li_dcpp][m_dcpp_Equ_dcp_NodeOffset] +=  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcp);
  dFdx[li_dcpp][m_dcpp_Equ_dcpp_NodeOffset] +=  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acp_dcpp)  -staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_acn_dcpp)  +staticContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcp);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_dcn_Equ_dcn_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcpp_dcn)  -dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcp_dcn);
  (*q_dcn_Equ_dcp_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcp_dcn);
  (*q_dcn_Equ_dcpp_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcpp_dcn);
  (*q_dcp_Equ_dcn_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcp_dcn);
  (*q_dcp_Equ_dcp_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcp_dcn);
  (*q_dcpp_Equ_dcn_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcn);
  (*q_dcpp_Equ_dcpp_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcn);
#else
  //use the offsets instead of pointers
  dQdx[li_dcn][m_dcn_Equ_dcn_NodeOffset] +=  -dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcpp_dcn)  -dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcp_dcn);
  dQdx[li_dcn][m_dcn_Equ_dcp_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcp_dcn);
  dQdx[li_dcn][m_dcn_Equ_dcpp_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dcn].dx(cogendaProbeID_V_dcpp_dcn);
  dQdx[li_dcp][m_dcp_Equ_dcn_NodeOffset] +=  -dynamicContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcp_dcn);
  dQdx[li_dcp][m_dcp_Equ_dcp_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dcp].dx(cogendaProbeID_V_dcp_dcn);
  dQdx[li_dcpp][m_dcpp_Equ_dcn_NodeOffset] +=  -dynamicContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcn);
  dQdx[li_dcpp][m_dcpp_Equ_dcpp_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dcpp].dx(cogendaProbeID_V_dcpp_dcn);
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
  if(!given("I0"))
    I0 = 1e-6;
  if(!given("C"))
    c = 1e-12;
  if(!given("R"))
    r = 100;
  if(!given("VTH"))
    vth = 0.0257;
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
    I0(1e-6),
    c(1e-12),
    r(100),
    vth(0.0257)
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
    .registerDevice("rect", 1)
    .registerModelType("rect", 1);
}
} // namespace COGENDArect
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDArect::registerDevice(); }}; 
Bootstrap COGENDArect_bootstrap;

