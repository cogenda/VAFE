//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "switchx.h"

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
namespace COGENDAswitchx {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_p;
    const int Instance::cogendaNodeID_n;
    const int Instance::cogendaNodeID_c;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_p_n;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAswitchx::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAswitchx::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("R",static_cast<double>(1000), &COGENDAswitchx::Instance::r);
  p.addPar("VTH",static_cast<double>(0.7), &COGENDAswitchx::Instance::vth);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAswitchx::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAswitchx::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("R",static_cast<double>(1000), &COGENDAswitchx::Model::r);
  p.addPar("VTH",static_cast<double>(0.7), &COGENDAswitchx::Model::vth);
}

/* class Instance member functions */
bool Instance::processParams () {
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
    li_p(-1),
    li_n(-1),
    li_c(-1),
    li_BRA_p_n(-1),
    li_branch_ip(-1),
    li_branch_in(-1),
    li_branch_ic(-1),
    f_p_Equ_BRA_p_n_Var_Ptr(0),
    f_n_Equ_BRA_p_n_Var_Ptr(0),
    f_BRA_p_n_Equ_p_Node_Ptr(0),
    f_BRA_p_n_Equ_n_Node_Ptr(0),
    f_BRA_p_n_Equ_BRA_p_n_Var_Ptr(0),
    q_p_Equ_BRA_p_n_Var_Ptr(0),
    q_n_Equ_BRA_p_n_Var_Ptr(0),
    q_BRA_p_n_Equ_p_Node_Ptr(0),
    q_BRA_p_n_Equ_n_Node_Ptr(0),
    q_BRA_p_n_Equ_BRA_p_n_Var_Ptr(0),
    m_p_Equ_BRA_p_n_VarOffset(-1),
    m_n_Equ_BRA_p_n_VarOffset(-1),
    m_BRA_p_n_Equ_p_NodeOffset(-1),
    m_BRA_p_n_Equ_n_NodeOffset(-1),
    m_BRA_p_n_Equ_BRA_p_n_VarOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 3;
  numIntVars = 1;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 3;
  leadCurrentF.resize(3);
  leadCurrentQ.resize(3);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_p,cogendaBRA_ID_p_n));
  jacobianElements.push_back(IntPair(cogendaNodeID_n,cogendaBRA_ID_p_n));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_p_n,cogendaNodeID_p));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_p_n,cogendaNodeID_n));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_p_n,cogendaBRA_ID_p_n));

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
    int originalSize = 4;
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

  li_p = localLIDVec[nodeMap[cogendaNodeID_p]];
  li_n = localLIDVec[nodeMap[cogendaNodeID_n]];
  li_c = localLIDVec[nodeMap[cogendaNodeID_c]];
  li_BRA_p_n = localLIDVec[nodeMap[cogendaBRA_ID_p_n]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_BRA_p_n, getName(), "p_n_branch");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_ip, getName(), "BRANCH_D1");
    addBranchDataNode( symbol_table, li_branch_in, getName(), "BRANCH_D2");
    addBranchDataNode( symbol_table, li_branch_ic, getName(), "BRANCH_D3");
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
        li_branch_ip = branchLIDVecRef[i++];
        li_branch_in = branchLIDVecRef[i++];
        li_branch_ic = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_p,cogendaBRA_ID_p_n)];
  m_p_Equ_BRA_p_n_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_n,cogendaBRA_ID_p_n)];
  m_n_Equ_BRA_p_n_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_p_n,cogendaNodeID_p)];
  m_BRA_p_n_Equ_p_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_p_n,cogendaNodeID_n)];
  m_BRA_p_n_Equ_n_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_p_n,cogendaBRA_ID_p_n)];
  m_BRA_p_n_Equ_BRA_p_n_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_p_Equ_BRA_p_n_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_p,li_BRA_p_n);
  f_n_Equ_BRA_p_n_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_n,li_BRA_p_n);
  f_BRA_p_n_Equ_p_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_p);
  f_BRA_p_n_Equ_n_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_n);
  f_BRA_p_n_Equ_BRA_p_n_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_BRA_p_n);
  q_p_Equ_BRA_p_n_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_p,li_BRA_p_n);
  q_n_Equ_BRA_p_n_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_n,li_BRA_p_n);
  q_BRA_p_n_Equ_p_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_p);
  q_BRA_p_n_Equ_n_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_n);
  q_BRA_p_n_Equ_BRA_p_n_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_p_n,li_BRA_p_n);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_p] += staticContributions[cogendaNodeID_p].val();
  (*extData.daeFVectorPtr)[li_n] += staticContributions[cogendaNodeID_n].val();
  (*extData.daeFVectorPtr)[li_c] += staticContributions[cogendaNodeID_c].val();
  (*extData.daeFVectorPtr)[li_BRA_p_n] += staticContributions[cogendaBRA_ID_p_n].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_ip] = leadCurrentF[cogendaNodeID_p];
          leadF[li_branch_in] = leadCurrentF[cogendaNodeID_n];
          leadF[li_branch_ic] = leadCurrentF[cogendaNodeID_c];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_p] += dynamicContributions[cogendaNodeID_p].val();
  (*extData.daeQVectorPtr)[li_n] += dynamicContributions[cogendaNodeID_n].val();
  (*extData.daeQVectorPtr)[li_c] += dynamicContributions[cogendaNodeID_c].val();
  (*extData.daeQVectorPtr)[li_BRA_p_n] += dynamicContributions[cogendaBRA_ID_p_n].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_ip] = leadCurrentQ[cogendaNodeID_p];
          leadQ[li_branch_in] = leadCurrentQ[cogendaNodeID_n];
          leadQ[li_branch_ic] = leadCurrentQ[cogendaNodeID_c];
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
  if (probeVars.size() != (2))
  {
    probeVars.resize(2);
    staticContributions.resize(3+1);
    dynamicContributions.resize(3+1);
  }

  for (int i=0; i < 3+1 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_I_p_n] = (*solVectorPtr)[li_BRA_p_n];
  probeVars[cogendaProbeID_I_p_n].diff(cogendaProbeID_I_p_n,2);
  probeVars[cogendaProbeID_V_c_GND] = (*solVectorPtr)[li_c];
  probeVars[cogendaProbeID_V_c_GND].diff(cogendaProbeID_V_c_GND,2);

{
  on = cogenda_ternary_op<CogendaFadType>((probeVars[cogendaProbeID_V_c_GND] > this->vth),1,0);
  if(on) {
//V-contrib...
    staticContributions[cogendaBRA_ID_p_n] += (probeVars[cogendaProbeID_I_p_n]*this->r);
  }
  else {
//I-contrib...
    staticContributions[cogendaNodeID_p] += 0;
    staticContributions[cogendaNodeID_n] -= 0;
  }
}
staticContributions[cogendaNodeID_p] += probeVars[cogendaProbeID_I_p_n];
staticContributions[cogendaNodeID_n] -= probeVars[cogendaProbeID_I_p_n];
staticContributions[cogendaBRA_ID_p_n] -= probeVars[cogendaProbeID_I_p_n];
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 3 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 3; unCtNode++) {
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
  (*f_p_Equ_BRA_p_n_Var_Ptr) +=  +staticContributions[cogendaNodeID_p].dx(cogendaProbeID_I_p_n);
  (*f_n_Equ_BRA_p_n_Var_Ptr) +=  +staticContributions[cogendaNodeID_n].dx(cogendaProbeID_I_p_n);
  (*f_BRA_p_n_Equ_p_Node_Ptr) +=  -1;
  (*f_BRA_p_n_Equ_n_Node_Ptr) +=  +1;
  (*f_BRA_p_n_Equ_BRA_p_n_Var_Ptr) +=  +staticContributions[cogendaBRA_ID_p_n].dx(cogendaProbeID_I_p_n);
#else
  //use the offsets instead of pointers
  dFdx[li_p][m_p_Equ_BRA_p_n_VarOffset] +=  +staticContributions[cogendaNodeID_p].dx(cogendaProbeID_I_p_n);
  dFdx[li_n][m_n_Equ_BRA_p_n_VarOffset] +=  +staticContributions[cogendaNodeID_n].dx(cogendaProbeID_I_p_n);
  dFdx[li_BRA_p_n][m_BRA_p_n_Equ_p_NodeOffset] +=  -1;
  dFdx[li_BRA_p_n][m_BRA_p_n_Equ_n_NodeOffset] +=  +1;
  dFdx[li_BRA_p_n][m_BRA_p_n_Equ_BRA_p_n_VarOffset] +=  +staticContributions[cogendaBRA_ID_p_n].dx(cogendaProbeID_I_p_n);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_p_Equ_BRA_p_n_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_p].dx(cogendaProbeID_I_p_n);
  (*q_n_Equ_BRA_p_n_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_n].dx(cogendaProbeID_I_p_n);
  (*q_BRA_p_n_Equ_BRA_p_n_Var_Ptr) +=  +dynamicContributions[cogendaBRA_ID_p_n].dx(cogendaProbeID_I_p_n);
#else
  //use the offsets instead of pointers
  dQdx[li_p][m_p_Equ_BRA_p_n_VarOffset] +=  +dynamicContributions[cogendaNodeID_p].dx(cogendaProbeID_I_p_n);
  dQdx[li_n][m_n_Equ_BRA_p_n_VarOffset] +=  +dynamicContributions[cogendaNodeID_n].dx(cogendaProbeID_I_p_n);
  dQdx[li_BRA_p_n][m_BRA_p_n_Equ_BRA_p_n_VarOffset] +=  +dynamicContributions[cogendaBRA_ID_p_n].dx(cogendaProbeID_I_p_n);
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
  if(!given("R"))
    r = 1000;
  if(!given("VTH"))
    vth = 0.7;
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
    r(1000),
    vth(0.7)
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
    .registerDevice("switchx", 1)
    .registerModelType("switchx", 1);
}
} // namespace COGENDAswitchx
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAswitchx::registerDevice(); }}; 
Bootstrap COGENDAswitchx_bootstrap;

