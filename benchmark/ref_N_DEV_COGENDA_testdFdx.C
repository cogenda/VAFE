//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "testdFdx.h"

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
namespace COGENDAtestdFdx {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_qceb;
    const int Instance::cogendaNodeID_emitter;
    const int Instance::cogendaNodeID_drain;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_qceb_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAtestdFdx::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAtestdFdx::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAtestdFdx::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAtestdFdx::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
}

/* class Instance member functions */
bool Instance::processParams () {
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
    li_qceb(-1),
    li_emitter(-1),
    li_drain(-1),
    li_BRA_qceb_GND(-1),
    li_branch_ia(-1),
    f_qceb_Equ_BRA_qceb_GND_Var_Ptr(0),
    f_BRA_qceb_GND_Equ_qceb_Node_Ptr(0),
    f_BRA_qceb_GND_Equ_emitter_Node_Ptr(0),
    f_BRA_qceb_GND_Equ_drain_Node_Ptr(0),
    q_qceb_Equ_BRA_qceb_GND_Var_Ptr(0),
    q_BRA_qceb_GND_Equ_qceb_Node_Ptr(0),
    q_BRA_qceb_GND_Equ_emitter_Node_Ptr(0),
    q_BRA_qceb_GND_Equ_drain_Node_Ptr(0),
    m_qceb_Equ_BRA_qceb_GND_VarOffset(-1),
    m_BRA_qceb_GND_Equ_qceb_NodeOffset(-1),
    m_BRA_qceb_GND_Equ_emitter_NodeOffset(-1),
    m_BRA_qceb_GND_Equ_drain_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 2;
  numIntVars = 2;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 2;
  leadCurrentF.resize(2);
  leadCurrentQ.resize(2);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_qceb,cogendaBRA_ID_qceb_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_qceb));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_emitter));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_drain));

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

  li_qceb = localLIDVec[nodeMap[cogendaNodeID_qceb]];
  li_emitter = localLIDVec[nodeMap[cogendaNodeID_emitter]];
  li_drain = localLIDVec[nodeMap[cogendaNodeID_drain]];
  li_BRA_qceb_GND = localLIDVec[nodeMap[cogendaBRA_ID_qceb_GND]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_qceb, getName(), "qceb");
  addInternalNode(symbol_table, li_emitter, getName(), "emitter");
  addInternalNode(symbol_table, li_drain, getName(), "drain");
  addInternalNode(symbol_table, li_BRA_qceb_GND, getName(), "qceb_GND_branch");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_ia, getName(), "BRANCH_D1");
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
        li_branch_ia = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_qceb,cogendaBRA_ID_qceb_GND)];
  m_qceb_Equ_BRA_qceb_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_qceb)];
  m_BRA_qceb_GND_Equ_qceb_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_emitter)];
  m_BRA_qceb_GND_Equ_emitter_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_drain)];
  m_BRA_qceb_GND_Equ_drain_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_qceb_Equ_BRA_qceb_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_qceb,li_BRA_qceb_GND);
  f_BRA_qceb_GND_Equ_qceb_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_qceb);
  f_BRA_qceb_GND_Equ_emitter_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_emitter);
  f_BRA_qceb_GND_Equ_drain_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_drain);
  q_qceb_Equ_BRA_qceb_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_qceb,li_BRA_qceb_GND);
  q_BRA_qceb_GND_Equ_qceb_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_qceb);
  q_BRA_qceb_GND_Equ_emitter_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_emitter);
  q_BRA_qceb_GND_Equ_drain_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_drain);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_qceb] += staticContributions[cogendaNodeID_qceb].val();
  (*extData.daeFVectorPtr)[li_emitter] += staticContributions[cogendaNodeID_emitter].val();
  (*extData.daeFVectorPtr)[li_drain] += staticContributions[cogendaNodeID_drain].val();
  (*extData.daeFVectorPtr)[li_BRA_qceb_GND] += staticContributions[cogendaBRA_ID_qceb_GND].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_ia] = leadCurrentF[cogendaNodeID_a];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_qceb] += dynamicContributions[cogendaNodeID_qceb].val();
  (*extData.daeQVectorPtr)[li_emitter] += dynamicContributions[cogendaNodeID_emitter].val();
  (*extData.daeQVectorPtr)[li_drain] += dynamicContributions[cogendaNodeID_drain].val();
  (*extData.daeQVectorPtr)[li_BRA_qceb_GND] += dynamicContributions[cogendaBRA_ID_qceb_GND].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_ia] = leadCurrentQ[cogendaNodeID_a];
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
  if (probeVars.size() != (3))
  {
    probeVars.resize(3);
    staticContributions.resize(3+1);
    dynamicContributions.resize(3+1);
  }

  for (int i=0; i < 3+1 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_I_qceb_GND] = (*solVectorPtr)[li_BRA_qceb_GND];
  probeVars[cogendaProbeID_I_qceb_GND].diff(cogendaProbeID_I_qceb_GND,3);
  probeVars[cogendaProbeID_V_emitter_drain] = (*solVectorPtr)[li_emitter] - (*solVectorPtr)[li_drain];
  probeVars[cogendaProbeID_V_emitter_drain].diff(cogendaProbeID_V_emitter_drain,3);
  probeVars[cogendaProbeID_V_qceb_GND] = (*solVectorPtr)[li_qceb];
  probeVars[cogendaProbeID_V_qceb_GND].diff(cogendaProbeID_V_qceb_GND,3);

{
  ved = probeVars[cogendaProbeID_V_emitter_drain];
//V-contrib...
  staticContributions[cogendaBRA_ID_qceb_GND] += (probeVars[cogendaProbeID_V_qceb_GND]+ved);
}
staticContributions[cogendaNodeID_qceb] += probeVars[cogendaProbeID_I_qceb_GND];
staticContributions[cogendaBRA_ID_qceb_GND] -= (*solVectorPtr)[li_qceb];
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 2 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 3; unCtNode++) {
      if (nodeMap[unCtNode] < 2 && nodeMap[unCtNode] != -1 ) {
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
  (*f_qceb_Equ_BRA_qceb_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  (*f_BRA_qceb_GND_Equ_qceb_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_qceb_GND);
  (*f_BRA_qceb_GND_Equ_emitter_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  (*f_BRA_qceb_GND_Equ_drain_Node_Ptr) +=  -staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  (*f_BRA_qceb_GND_Equ_qceb_Node_Ptr) +=  -1;
#else
  //use the offsets instead of pointers
  dFdx[li_qceb][m_qceb_Equ_BRA_qceb_GND_VarOffset] +=  +staticContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_qceb_NodeOffset] +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_qceb_GND);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_emitter_NodeOffset] +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_drain_NodeOffset] +=  -staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_qceb_NodeOffset] +=  -1;
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_qceb_Equ_BRA_qceb_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
#else
  //use the offsets instead of pointers
  dQdx[li_qceb][m_qceb_Equ_BRA_qceb_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
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
    .registerDevice("testdFdx", 1)
    .registerModelType("testdFdx", 1);
}
} // namespace COGENDAtestdFdx
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAtestdFdx::registerDevice(); }}; 
Bootstrap COGENDAtestdFdx_bootstrap;

