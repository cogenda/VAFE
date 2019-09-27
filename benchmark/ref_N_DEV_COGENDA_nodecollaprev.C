//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "nodecollaprev.h"

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
namespace COGENDAnodecollaprev {

    const int Instance::cogendaNodeID_a;
    const int Instance::cogendaNodeID_a1;
    const int Instance::cogendaNodeID_c;
    const int Instance::cogendaNodeID_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAnodecollaprev::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAnodecollaprev::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("A",static_cast<double>(1e-12), &COGENDAnodecollaprev::Instance::A);
  p.addPar("Q",static_cast<int>(1), &COGENDAnodecollaprev::Instance::Q);
  p.addPar("R",static_cast<double>(1000e3), &COGENDAnodecollaprev::Instance::R);
  p.addPar("A0",static_cast<double>(0.1), &COGENDAnodecollaprev::Instance::a0);
  p.addPar("A2",static_cast<double>(0.01), &COGENDAnodecollaprev::Instance::a2);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAnodecollaprev::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAnodecollaprev::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("A",static_cast<double>(1e-12), &COGENDAnodecollaprev::Model::A);
  p.addPar("Q",static_cast<int>(1), &COGENDAnodecollaprev::Model::Q);
  p.addPar("R",static_cast<double>(1000e3), &COGENDAnodecollaprev::Model::R);
  p.addPar("A0",static_cast<double>(0.1), &COGENDAnodecollaprev::Model::a0);
  p.addPar("A2",static_cast<double>(0.01), &COGENDAnodecollaprev::Model::a2);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("A"))
    A = model_.A;
  if(!given("Q"))
    Q = model_.Q;
  if(!given("R"))
    R = model_.R;
  if(!given("A0"))
    a0 = model_.a0;
  if(!given("A2"))
    a2 = model_.a2;
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
    li_a(-1),
    li_a1(-1),
    li_c(-1),
    li_branch_ia(-1),
    f_a1_Equ_a1_Node_Ptr(0),
    f_a1_Equ_c_Node_Ptr(0),
    f_c_Equ_a1_Node_Ptr(0),
    f_c_Equ_c_Node_Ptr(0),
    f_a_Equ_a_Node_Ptr(0),
    f_a_Equ_a1_Node_Ptr(0),
    f_a1_Equ_a_Node_Ptr(0),
    q_a1_Equ_a1_Node_Ptr(0),
    q_a1_Equ_c_Node_Ptr(0),
    q_c_Equ_a1_Node_Ptr(0),
    q_c_Equ_c_Node_Ptr(0),
    q_a_Equ_a_Node_Ptr(0),
    q_a_Equ_a1_Node_Ptr(0),
    q_a1_Equ_a_Node_Ptr(0),
    m_a1_Equ_a1_NodeOffset(-1),
    m_a1_Equ_c_NodeOffset(-1),
    m_c_Equ_a1_NodeOffset(-1),
    m_c_Equ_c_NodeOffset(-1),
    m_a_Equ_a_NodeOffset(-1),
    m_a_Equ_a1_NodeOffset(-1),
    m_a1_Equ_a_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 2;
  numIntVars = 1;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 2;
  leadCurrentF.resize(2);
  leadCurrentQ.resize(2);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_a1,cogendaNodeID_a1));
  jacobianElements.push_back(IntPair(cogendaNodeID_a1,cogendaNodeID_c));
  jacobianElements.push_back(IntPair(cogendaNodeID_c,cogendaNodeID_a1));
  jacobianElements.push_back(IntPair(cogendaNodeID_c,cogendaNodeID_c));
  jacobianElements.push_back(IntPair(cogendaNodeID_a,cogendaNodeID_a));
  jacobianElements.push_back(IntPair(cogendaNodeID_a,cogendaNodeID_a1));
  jacobianElements.push_back(IntPair(cogendaNodeID_a1,cogendaNodeID_a));

  setDefaultParams();
  setParams(instance_block.params);

  if (!given("XYCEADMSINSTTEMP"))
    cogendaInstTemp=getDeviceOptions().temp.getImmutableValue<double>();

  updateDependentParameters();

  processParams();
  initInternalVars();

  PairVector collapsedNodes;
  collapseNodes();
  if (collapseNode_a1) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_a1,cogendaNodeID_a));

  if (jacStamp.empty())
  {
    int originalSize = 3;
    computeJacStampAndMaps(jacobianElements,collapsedNodes,jacStamp,nodeMap,pairToJacStampMap,originalSize);
  }
}

  void Instance::collapseNodes() {
  collapseNode_a1 = false;
if((this->R < 1e-2)) {
{
  collapseNode_a1 = true;
  numIntVars--;
}
}
else {
{
}
}
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

  li_a = localLIDVec[nodeMap[cogendaNodeID_a]];
  li_a1 = localLIDVec[nodeMap[cogendaNodeID_a1]];
  li_c = localLIDVec[nodeMap[cogendaNodeID_c]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  if (!collapseNode_a1)
    addInternalNode(symbol_table, li_a1, getName(), "a1");
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
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a1,cogendaNodeID_a1)];
  m_a1_Equ_a1_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a1,cogendaNodeID_c)];
  m_a1_Equ_c_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_c,cogendaNodeID_a1)];
  m_c_Equ_a1_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_c,cogendaNodeID_c)];
  m_c_Equ_c_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a,cogendaNodeID_a)];
  m_a_Equ_a_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a,cogendaNodeID_a1)];
  m_a_Equ_a1_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a1,cogendaNodeID_a)];
  m_a1_Equ_a_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_a1_Equ_a1_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a1,li_a1);
  f_a1_Equ_c_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a1,li_c);
  f_c_Equ_a1_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_c,li_a1);
  f_c_Equ_c_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_c,li_c);
  f_a_Equ_a_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a,li_a);
  f_a_Equ_a1_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a,li_a1);
  f_a1_Equ_a_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a1,li_a);
  q_a1_Equ_a1_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a1,li_a1);
  q_a1_Equ_c_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a1,li_c);
  q_c_Equ_a1_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_c,li_a1);
  q_c_Equ_c_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_c,li_c);
  q_a_Equ_a_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a,li_a);
  q_a_Equ_a1_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a,li_a1);
  q_a1_Equ_a_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a1,li_a);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_a] += staticContributions[cogendaNodeID_a].val();
  (*extData.daeFVectorPtr)[li_a1] += staticContributions[cogendaNodeID_a1].val();
  (*extData.daeFVectorPtr)[li_c] += staticContributions[cogendaNodeID_c].val();
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
  (*extData.daeQVectorPtr)[li_a] += dynamicContributions[cogendaNodeID_a].val();
  (*extData.daeQVectorPtr)[li_a1] += dynamicContributions[cogendaNodeID_a1].val();
  (*extData.daeQVectorPtr)[li_c] += dynamicContributions[cogendaNodeID_c].val();
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
  if (probeVars.size() != (2))
  {
    probeVars.resize(2);
    staticContributions.resize(3+0);
    dynamicContributions.resize(3+0);
  }

  for (int i=0; i < 3+0 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_V_a1_c] = (*solVectorPtr)[li_a1] - (*solVectorPtr)[li_c];
  probeVars[cogendaProbeID_V_a1_c].diff(cogendaProbeID_V_a1_c,2);
  probeVars[cogendaProbeID_V_a_a1] = (*solVectorPtr)[li_a] - (*solVectorPtr)[li_a1];
  probeVars[cogendaProbeID_V_a_a1].diff(cogendaProbeID_V_a_a1,2);

{
  if((this->R < 1e-2)) {
    {
//V-contrib...
//ZERO_For_Collapsed_Node
//I-contrib...
      staticContributions[cogendaNodeID_a1] += ((probeVars[cogendaProbeID_V_a1_c]/this->R)*2);
      staticContributions[cogendaNodeID_c] -= ((probeVars[cogendaProbeID_V_a1_c]/this->R)*2);
    }
  }
  else {
    {
//I-contrib...
      staticContributions[cogendaNodeID_a] += (probeVars[cogendaProbeID_V_a_a1]/(this->R*2));
      staticContributions[cogendaNodeID_a1] -= (probeVars[cogendaProbeID_V_a_a1]/(this->R*2));
//I-contrib...
      staticContributions[cogendaNodeID_a1] += ((probeVars[cogendaProbeID_V_a1_c]/this->R)*2);
      staticContributions[cogendaNodeID_c] -= ((probeVars[cogendaProbeID_V_a1_c]/this->R)*2);
    }
  }
}
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
  (*f_a_Equ_a_Node_Ptr) +=  +staticContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_a1);
  (*f_a_Equ_a1_Node_Ptr) +=  -staticContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_a1);
  (*f_a1_Equ_a_Node_Ptr) +=  +staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a_a1);
  (*f_a1_Equ_a1_Node_Ptr) +=  +staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a1_c)  -staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a_a1);
  (*f_a1_Equ_c_Node_Ptr) +=  -staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a1_c);
  (*f_c_Equ_a1_Node_Ptr) +=  +staticContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a1_c);
  (*f_c_Equ_c_Node_Ptr) +=  -staticContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a1_c);
#else
  //use the offsets instead of pointers
  dFdx[li_a][m_a_Equ_a_NodeOffset] +=  +staticContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_a1);
  dFdx[li_a][m_a_Equ_a1_NodeOffset] +=  -staticContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_a1);
  dFdx[li_a1][m_a1_Equ_a_NodeOffset] +=  +staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a_a1);
  dFdx[li_a1][m_a1_Equ_a1_NodeOffset] +=  +staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a1_c)  -staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a_a1);
  dFdx[li_a1][m_a1_Equ_c_NodeOffset] +=  -staticContributions[cogendaNodeID_a1].dx(cogendaProbeID_V_a1_c);
  dFdx[li_c][m_c_Equ_a1_NodeOffset] +=  +staticContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a1_c);
  dFdx[li_c][m_c_Equ_c_NodeOffset] +=  -staticContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a1_c);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
#else
  //use the offsets instead of pointers
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
  if(!given("A"))
    A = 1e-12;
  else if ( !(A >= 0))
    UserError0(*this) << "Model Parameter value " << A << " out of range [0:inf)";
  if(!given("Q"))
    Q = 1;
  if(!given("R"))
    R = 1000e3;
  if(!given("A0"))
    a0 = 0.1;
  if(!given("A2"))
    a2 = 0.01;
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
    A(1e-12),
    Q(1),
    R(1000e3),
    a0(0.1),
    a2(0.01)
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
    .registerDevice("nodecollaprev", 1)
    .registerModelType("nodecollaprev", 1);
}
} // namespace COGENDAnodecollaprev
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAnodecollaprev::registerDevice(); }}; 
Bootstrap COGENDAnodecollaprev_bootstrap;

