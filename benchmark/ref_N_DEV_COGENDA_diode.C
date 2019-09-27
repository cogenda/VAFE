//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "diode.h"

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
namespace COGENDAdiode {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_a;
    const int Instance::cogendaNodeID_c;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_a_c;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAdiode::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAdiode::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("CJO",static_cast<double>(1.0e-14), &COGENDAdiode::Instance::CJO);
  p.addPar("IS",static_cast<double>(1.0e-14), &COGENDAdiode::Instance::IS);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAdiode::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAdiode::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("CJO",static_cast<double>(1.0e-14), &COGENDAdiode::Model::CJO);
  p.addPar("IS",static_cast<double>(1.0e-14), &COGENDAdiode::Model::IS);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("CJO"))
    CJO = model_.CJO;
  if(!given("IS"))
    IS = model_.IS;
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
    li_c(-1),
    li_BRA_a_c(-1),
    li_branch_ia(-1),
    f_a_Equ_BRA_a_c_Var_Ptr(0),
    f_c_Equ_BRA_a_c_Var_Ptr(0),
    f_BRA_a_c_Equ_a_Node_Ptr(0),
    f_BRA_a_c_Equ_c_Node_Ptr(0),
    f_a_Equ_a_Node_Ptr(0),
    f_a_Equ_c_Node_Ptr(0),
    f_c_Equ_a_Node_Ptr(0),
    f_c_Equ_c_Node_Ptr(0),
    q_a_Equ_BRA_a_c_Var_Ptr(0),
    q_c_Equ_BRA_a_c_Var_Ptr(0),
    q_BRA_a_c_Equ_a_Node_Ptr(0),
    q_BRA_a_c_Equ_c_Node_Ptr(0),
    q_a_Equ_a_Node_Ptr(0),
    q_a_Equ_c_Node_Ptr(0),
    q_c_Equ_a_Node_Ptr(0),
    q_c_Equ_c_Node_Ptr(0),
    m_a_Equ_BRA_a_c_VarOffset(-1),
    m_c_Equ_BRA_a_c_VarOffset(-1),
    m_BRA_a_c_Equ_a_NodeOffset(-1),
    m_BRA_a_c_Equ_c_NodeOffset(-1),
    m_a_Equ_a_NodeOffset(-1),
    m_a_Equ_c_NodeOffset(-1),
    m_c_Equ_a_NodeOffset(-1),
    m_c_Equ_c_NodeOffset(-1),
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
  jacobianElements.push_back(IntPair(cogendaNodeID_a,cogendaBRA_ID_a_c));
  jacobianElements.push_back(IntPair(cogendaNodeID_c,cogendaBRA_ID_a_c));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_a_c,cogendaNodeID_a));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_a_c,cogendaNodeID_c));
  jacobianElements.push_back(IntPair(cogendaNodeID_a,cogendaNodeID_a));
  jacobianElements.push_back(IntPair(cogendaNodeID_a,cogendaNodeID_c));
  jacobianElements.push_back(IntPair(cogendaNodeID_c,cogendaNodeID_a));
  jacobianElements.push_back(IntPair(cogendaNodeID_c,cogendaNodeID_c));

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
    int originalSize = 3;
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

  li_a = localLIDVec[nodeMap[cogendaNodeID_a]];
  li_c = localLIDVec[nodeMap[cogendaNodeID_c]];
  li_BRA_a_c = localLIDVec[nodeMap[cogendaBRA_ID_a_c]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_BRA_a_c, getName(), "a_c_branch");
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
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a,cogendaBRA_ID_a_c)];
  m_a_Equ_BRA_a_c_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_c,cogendaBRA_ID_a_c)];
  m_c_Equ_BRA_a_c_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_a_c,cogendaNodeID_a)];
  m_BRA_a_c_Equ_a_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_a_c,cogendaNodeID_c)];
  m_BRA_a_c_Equ_c_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a,cogendaNodeID_a)];
  m_a_Equ_a_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_a,cogendaNodeID_c)];
  m_a_Equ_c_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_c,cogendaNodeID_a)];
  m_c_Equ_a_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_c,cogendaNodeID_c)];
  m_c_Equ_c_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_a_Equ_BRA_a_c_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a,li_BRA_a_c);
  f_c_Equ_BRA_a_c_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_c,li_BRA_a_c);
  f_BRA_a_c_Equ_a_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_a_c,li_a);
  f_BRA_a_c_Equ_c_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_a_c,li_c);
  f_a_Equ_a_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a,li_a);
  f_a_Equ_c_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_a,li_c);
  f_c_Equ_a_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_c,li_a);
  f_c_Equ_c_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_c,li_c);
  q_a_Equ_BRA_a_c_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a,li_BRA_a_c);
  q_c_Equ_BRA_a_c_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_c,li_BRA_a_c);
  q_BRA_a_c_Equ_a_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_a_c,li_a);
  q_BRA_a_c_Equ_c_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_a_c,li_c);
  q_a_Equ_a_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a,li_a);
  q_a_Equ_c_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_a,li_c);
  q_c_Equ_a_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_c,li_a);
  q_c_Equ_c_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_c,li_c);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_a] += staticContributions[cogendaNodeID_a].val();
  (*extData.daeFVectorPtr)[li_c] += staticContributions[cogendaNodeID_c].val();
  (*extData.daeFVectorPtr)[li_BRA_a_c] += staticContributions[cogendaBRA_ID_a_c].val();
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
  (*extData.daeQVectorPtr)[li_c] += dynamicContributions[cogendaNodeID_c].val();
  (*extData.daeQVectorPtr)[li_BRA_a_c] += dynamicContributions[cogendaBRA_ID_a_c].val();
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
    staticContributions.resize(2+1);
    dynamicContributions.resize(2+1);
  }

  for (int i=0; i < 2+1 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_I_a_c] = (*solVectorPtr)[li_BRA_a_c];
  probeVars[cogendaProbeID_I_a_c].diff(cogendaProbeID_I_a_c,2);
  probeVars[cogendaProbeID_V_a_c] = (*solVectorPtr)[li_a] - (*solVectorPtr)[li_c];
  probeVars[cogendaProbeID_V_a_c].diff(cogendaProbeID_V_a_c,2);

{
  vdio = (probeVars[cogendaProbeID_V_a_c]);
  idio = ((this->IS*(cogenda_ternary_op<CogendaFadType>(((vdio/_VT_) < log(1e20)),exp((vdio/_VT_)),((((1e20*vdio)/_VT_)+1e20)-(1e20*log(1e20))))-1))*log(vdio));
//V-contrib...
  staticContributions[cogendaBRA_ID_a_c] += (vdio+0.001);
  if((vdio < 0.5)) {
    {
      qdio = ((0.5*this->CJO)*(1-sqrt((1-vdio))));
      idio = (1.2e-9*log((qdio*3.499)));
    }
  }
  else {
    {
      qdio = (this->CJO*((2.0*(1.0-sqrt(0.5)))+((sqrt(2.0)/2.0)*(((vdio*vdio)+vdio)-(3.0/4.0)))));
    }
  }
  rand = ((5)*UNITFAD).val();
  while(rand)
    {
      count = (((rand+1))*UNITFAD).val();
      rand = (((rand-1))*UNITFAD).val();
    }
  while(rand)
    rand = (((rand-1))*UNITFAD).val();
  for(j = ((2)*UNITFAD).val();(j < 22);j = (((j+2))*UNITFAD).val())
    total = (((count+j))*UNITFAD).val();
  int __loop_incr_var0=10;
  while(__loop_incr_var0--)
    {
      rand = (((rand+1))*UNITFAD).val();
      total = (((total+i))*UNITFAD).val();
    }
//Q-contrib...
  dynamicContributions[cogendaNodeID_a] += (qdio);
  dynamicContributions[cogendaNodeID_c] -= (qdio);
}
staticContributions[cogendaNodeID_a] += probeVars[cogendaProbeID_I_a_c];
staticContributions[cogendaNodeID_c] -= probeVars[cogendaProbeID_I_a_c];
staticContributions[cogendaBRA_ID_a_c] -= probeVars[cogendaProbeID_I_a_c];
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 2 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 2; unCtNode++) {
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
  (*f_a_Equ_BRA_a_c_Var_Ptr) +=  +staticContributions[cogendaNodeID_a].dx(cogendaProbeID_I_a_c);
  (*f_c_Equ_BRA_a_c_Var_Ptr) +=  +staticContributions[cogendaNodeID_c].dx(cogendaProbeID_I_a_c);
  (*f_BRA_a_c_Equ_a_Node_Ptr) +=  -1;
  (*f_BRA_a_c_Equ_c_Node_Ptr) +=  +1;
#else
  //use the offsets instead of pointers
  dFdx[li_a][m_a_Equ_BRA_a_c_VarOffset] +=  +staticContributions[cogendaNodeID_a].dx(cogendaProbeID_I_a_c);
  dFdx[li_c][m_c_Equ_BRA_a_c_VarOffset] +=  +staticContributions[cogendaNodeID_c].dx(cogendaProbeID_I_a_c);
  dFdx[li_BRA_a_c][m_BRA_a_c_Equ_a_NodeOffset] +=  -1;
  dFdx[li_BRA_a_c][m_BRA_a_c_Equ_c_NodeOffset] +=  +1;
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_a_Equ_BRA_a_c_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_I_a_c);
  (*q_c_Equ_BRA_a_c_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_I_a_c);
  (*q_a_Equ_a_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_c);
  (*q_a_Equ_c_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_c);
  (*q_c_Equ_a_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a_c);
  (*q_c_Equ_c_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a_c);
#else
  //use the offsets instead of pointers
  dQdx[li_a][m_a_Equ_BRA_a_c_VarOffset] +=  +dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_I_a_c);
  dQdx[li_c][m_c_Equ_BRA_a_c_VarOffset] +=  +dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_I_a_c);
  dQdx[li_a][m_a_Equ_a_NodeOffset] +=  +dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_c);
  dQdx[li_a][m_a_Equ_c_NodeOffset] +=  -dynamicContributions[cogendaNodeID_a].dx(cogendaProbeID_V_a_c);
  dQdx[li_c][m_c_Equ_a_NodeOffset] +=  +dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a_c);
  dQdx[li_c][m_c_Equ_c_NodeOffset] +=  -dynamicContributions[cogendaNodeID_c].dx(cogendaProbeID_V_a_c);
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
  if(!given("CJO"))
    CJO = 1.0e-14;
  if(!given("IS"))
    IS = 1.0e-14;
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
    CJO(1.0e-14),
    IS(1.0e-14)
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
    .registerDevice("diode", 1)
    .registerModelType("diode", 1);
}
} // namespace COGENDAdiode
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAdiode::registerDevice(); }}; 
Bootstrap COGENDAdiode_bootstrap;

