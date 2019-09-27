//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "contrib.h"

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
namespace COGENDAcontrib {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_anode;
    const int Instance::cogendaNodeID_gate;
    const int Instance::cogendaNodeID_cathode;
    const int Instance::cogendaNodeID_nsat;
    const int Instance::cogendaNodeID_qceb;
    const int Instance::cogendaNodeID_mucinv;
    const int Instance::cogendaNodeID_drain;
    const int Instance::cogendaNodeID_emitter;
    const int Instance::cogendaNodeID_dvdgdt;
    const int Instance::cogendaNodeID_dvbcdt;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_dvdgdt_GND;
    const int Instance::cogendaBRA_ID_dvbcdt_GND;
    const int Instance::cogendaBRA_ID_qceb_GND;
    const int Instance::cogendaBRA_ID_nsat_GND;
    const int Instance::cogendaBRA_ID_mucinv_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAcontrib::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAcontrib::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAcontrib::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAcontrib::Model::cogendaModTemp)
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
    li_anode(-1),
    li_gate(-1),
    li_cathode(-1),
    li_nsat(-1),
    li_qceb(-1),
    li_mucinv(-1),
    li_drain(-1),
    li_emitter(-1),
    li_dvdgdt(-1),
    li_dvbcdt(-1),
    li_BRA_dvdgdt_GND(-1),
    li_BRA_dvbcdt_GND(-1),
    li_BRA_qceb_GND(-1),
    li_BRA_nsat_GND(-1),
    li_BRA_mucinv_GND(-1),
    li_branch_ianode(-1),
    li_branch_igate(-1),
    li_branch_icathode(-1),
    li_branch_insat(-1),
    f_gate_Equ_gate_Node_Ptr(0),
    f_gate_Equ_cathode_Node_Ptr(0),
    f_cathode_Equ_gate_Node_Ptr(0),
    f_cathode_Equ_cathode_Node_Ptr(0),
    f_drain_Equ_dvdgdt_Node_Ptr(0),
    f_gate_Equ_dvdgdt_Node_Ptr(0),
    f_emitter_Equ_qceb_Node_Ptr(0),
    f_drain_Equ_qceb_Node_Ptr(0),
    f_anode_Equ_anode_Node_Ptr(0),
    f_anode_Equ_emitter_Node_Ptr(0),
    f_emitter_Equ_anode_Node_Ptr(0),
    f_emitter_Equ_emitter_Node_Ptr(0),
    f_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr(0),
    f_BRA_dvdgdt_GND_Equ_drain_Node_Ptr(0),
    f_BRA_dvdgdt_GND_Equ_gate_Node_Ptr(0),
    f_BRA_dvdgdt_GND_Equ_dvdgdt_Node_Ptr(0),
    f_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr(0),
    f_BRA_dvbcdt_GND_Equ_drain_Node_Ptr(0),
    f_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr(0),
    f_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr(0),
    f_qceb_Equ_BRA_qceb_GND_Var_Ptr(0),
    f_BRA_qceb_GND_Equ_qceb_Node_Ptr(0),
    f_BRA_qceb_GND_Equ_emitter_Node_Ptr(0),
    f_BRA_qceb_GND_Equ_drain_Node_Ptr(0),
    f_nsat_Equ_BRA_nsat_GND_Var_Ptr(0),
    f_BRA_nsat_GND_Equ_nsat_Node_Ptr(0),
    f_mucinv_Equ_BRA_mucinv_GND_Var_Ptr(0),
    f_BRA_mucinv_GND_Equ_mucinv_Node_Ptr(0),
    q_gate_Equ_gate_Node_Ptr(0),
    q_gate_Equ_cathode_Node_Ptr(0),
    q_cathode_Equ_gate_Node_Ptr(0),
    q_cathode_Equ_cathode_Node_Ptr(0),
    q_drain_Equ_dvdgdt_Node_Ptr(0),
    q_gate_Equ_dvdgdt_Node_Ptr(0),
    q_emitter_Equ_qceb_Node_Ptr(0),
    q_drain_Equ_qceb_Node_Ptr(0),
    q_anode_Equ_anode_Node_Ptr(0),
    q_anode_Equ_emitter_Node_Ptr(0),
    q_emitter_Equ_anode_Node_Ptr(0),
    q_emitter_Equ_emitter_Node_Ptr(0),
    q_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr(0),
    q_BRA_dvdgdt_GND_Equ_drain_Node_Ptr(0),
    q_BRA_dvdgdt_GND_Equ_gate_Node_Ptr(0),
    q_BRA_dvdgdt_GND_Equ_dvdgdt_Node_Ptr(0),
    q_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr(0),
    q_BRA_dvbcdt_GND_Equ_drain_Node_Ptr(0),
    q_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr(0),
    q_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr(0),
    q_qceb_Equ_BRA_qceb_GND_Var_Ptr(0),
    q_BRA_qceb_GND_Equ_qceb_Node_Ptr(0),
    q_BRA_qceb_GND_Equ_emitter_Node_Ptr(0),
    q_BRA_qceb_GND_Equ_drain_Node_Ptr(0),
    q_nsat_Equ_BRA_nsat_GND_Var_Ptr(0),
    q_BRA_nsat_GND_Equ_nsat_Node_Ptr(0),
    q_mucinv_Equ_BRA_mucinv_GND_Var_Ptr(0),
    q_BRA_mucinv_GND_Equ_mucinv_Node_Ptr(0),
    m_gate_Equ_gate_NodeOffset(-1),
    m_gate_Equ_cathode_NodeOffset(-1),
    m_cathode_Equ_gate_NodeOffset(-1),
    m_cathode_Equ_cathode_NodeOffset(-1),
    m_drain_Equ_dvdgdt_NodeOffset(-1),
    m_gate_Equ_dvdgdt_NodeOffset(-1),
    m_emitter_Equ_qceb_NodeOffset(-1),
    m_drain_Equ_qceb_NodeOffset(-1),
    m_anode_Equ_anode_NodeOffset(-1),
    m_anode_Equ_emitter_NodeOffset(-1),
    m_emitter_Equ_anode_NodeOffset(-1),
    m_emitter_Equ_emitter_NodeOffset(-1),
    m_dvdgdt_Equ_BRA_dvdgdt_GND_VarOffset(-1),
    m_BRA_dvdgdt_GND_Equ_drain_NodeOffset(-1),
    m_BRA_dvdgdt_GND_Equ_gate_NodeOffset(-1),
    m_BRA_dvdgdt_GND_Equ_dvdgdt_NodeOffset(-1),
    m_dvbcdt_Equ_BRA_dvbcdt_GND_VarOffset(-1),
    m_BRA_dvbcdt_GND_Equ_drain_NodeOffset(-1),
    m_BRA_dvbcdt_GND_Equ_cathode_NodeOffset(-1),
    m_BRA_dvbcdt_GND_Equ_dvbcdt_NodeOffset(-1),
    m_qceb_Equ_BRA_qceb_GND_VarOffset(-1),
    m_BRA_qceb_GND_Equ_qceb_NodeOffset(-1),
    m_BRA_qceb_GND_Equ_emitter_NodeOffset(-1),
    m_BRA_qceb_GND_Equ_drain_NodeOffset(-1),
    m_nsat_Equ_BRA_nsat_GND_VarOffset(-1),
    m_BRA_nsat_GND_Equ_nsat_NodeOffset(-1),
    m_mucinv_Equ_BRA_mucinv_GND_VarOffset(-1),
    m_BRA_mucinv_GND_Equ_mucinv_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 4;
  numIntVars = 11;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 4;
  leadCurrentF.resize(4);
  leadCurrentQ.resize(4);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_gate,cogendaNodeID_gate));
  jacobianElements.push_back(IntPair(cogendaNodeID_gate,cogendaNodeID_cathode));
  jacobianElements.push_back(IntPair(cogendaNodeID_cathode,cogendaNodeID_gate));
  jacobianElements.push_back(IntPair(cogendaNodeID_cathode,cogendaNodeID_cathode));
  jacobianElements.push_back(IntPair(cogendaNodeID_drain,cogendaNodeID_dvdgdt));
  jacobianElements.push_back(IntPair(cogendaNodeID_gate,cogendaNodeID_dvdgdt));
  jacobianElements.push_back(IntPair(cogendaNodeID_emitter,cogendaNodeID_qceb));
  jacobianElements.push_back(IntPair(cogendaNodeID_drain,cogendaNodeID_qceb));
  jacobianElements.push_back(IntPair(cogendaNodeID_anode,cogendaNodeID_anode));
  jacobianElements.push_back(IntPair(cogendaNodeID_anode,cogendaNodeID_emitter));
  jacobianElements.push_back(IntPair(cogendaNodeID_emitter,cogendaNodeID_anode));
  jacobianElements.push_back(IntPair(cogendaNodeID_emitter,cogendaNodeID_emitter));
  jacobianElements.push_back(IntPair(cogendaNodeID_dvdgdt,cogendaBRA_ID_dvdgdt_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_drain));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_gate));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_dvdgdt));
  jacobianElements.push_back(IntPair(cogendaNodeID_dvbcdt,cogendaBRA_ID_dvbcdt_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_drain));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_cathode));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_dvbcdt));
  jacobianElements.push_back(IntPair(cogendaNodeID_qceb,cogendaBRA_ID_qceb_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_qceb));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_emitter));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_drain));
  jacobianElements.push_back(IntPair(cogendaNodeID_nsat,cogendaBRA_ID_nsat_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_nsat_GND,cogendaNodeID_nsat));
  jacobianElements.push_back(IntPair(cogendaNodeID_mucinv,cogendaBRA_ID_mucinv_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_mucinv_GND,cogendaNodeID_mucinv));

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
    int originalSize = 15;
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

  li_anode = localLIDVec[nodeMap[cogendaNodeID_anode]];
  li_gate = localLIDVec[nodeMap[cogendaNodeID_gate]];
  li_cathode = localLIDVec[nodeMap[cogendaNodeID_cathode]];
  li_nsat = localLIDVec[nodeMap[cogendaNodeID_nsat]];
  li_qceb = localLIDVec[nodeMap[cogendaNodeID_qceb]];
  li_mucinv = localLIDVec[nodeMap[cogendaNodeID_mucinv]];
  li_drain = localLIDVec[nodeMap[cogendaNodeID_drain]];
  li_emitter = localLIDVec[nodeMap[cogendaNodeID_emitter]];
  li_dvdgdt = localLIDVec[nodeMap[cogendaNodeID_dvdgdt]];
  li_dvbcdt = localLIDVec[nodeMap[cogendaNodeID_dvbcdt]];
  li_BRA_dvdgdt_GND = localLIDVec[nodeMap[cogendaBRA_ID_dvdgdt_GND]];
  li_BRA_dvbcdt_GND = localLIDVec[nodeMap[cogendaBRA_ID_dvbcdt_GND]];
  li_BRA_qceb_GND = localLIDVec[nodeMap[cogendaBRA_ID_qceb_GND]];
  li_BRA_nsat_GND = localLIDVec[nodeMap[cogendaBRA_ID_nsat_GND]];
  li_BRA_mucinv_GND = localLIDVec[nodeMap[cogendaBRA_ID_mucinv_GND]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
  /*The initial_step block stars...*/
  {
    ibp = ((1e-12)*UNITFAD).val();
    rb = ((1.0)*UNITFAD).val();
    imult = ((1.0)*UNITFAD).val();
  }
  /*The initial_step block ends...*/
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_qceb, getName(), "qceb");
  addInternalNode(symbol_table, li_mucinv, getName(), "mucinv");
  addInternalNode(symbol_table, li_drain, getName(), "drain");
  addInternalNode(symbol_table, li_emitter, getName(), "emitter");
  addInternalNode(symbol_table, li_dvdgdt, getName(), "dvdgdt");
  addInternalNode(symbol_table, li_dvbcdt, getName(), "dvbcdt");
  addInternalNode(symbol_table, li_BRA_dvdgdt_GND, getName(), "dvdgdt_GND_branch");
  addInternalNode(symbol_table, li_BRA_dvbcdt_GND, getName(), "dvbcdt_GND_branch");
  addInternalNode(symbol_table, li_BRA_qceb_GND, getName(), "qceb_GND_branch");
  addInternalNode(symbol_table, li_BRA_nsat_GND, getName(), "nsat_GND_branch");
  addInternalNode(symbol_table, li_BRA_mucinv_GND, getName(), "mucinv_GND_branch");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_ianode, getName(), "BRANCH_D1");
    addBranchDataNode( symbol_table, li_branch_igate, getName(), "BRANCH_D2");
    addBranchDataNode( symbol_table, li_branch_icathode, getName(), "BRANCH_D3");
    addBranchDataNode( symbol_table, li_branch_insat, getName(), "BRANCH_D4");
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
        li_branch_ianode = branchLIDVecRef[i++];
        li_branch_igate = branchLIDVecRef[i++];
        li_branch_icathode = branchLIDVecRef[i++];
        li_branch_insat = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gate,cogendaNodeID_gate)];
  m_gate_Equ_gate_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gate,cogendaNodeID_cathode)];
  m_gate_Equ_cathode_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_cathode,cogendaNodeID_gate)];
  m_cathode_Equ_gate_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_cathode,cogendaNodeID_cathode)];
  m_cathode_Equ_cathode_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_drain,cogendaNodeID_dvdgdt)];
  m_drain_Equ_dvdgdt_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gate,cogendaNodeID_dvdgdt)];
  m_gate_Equ_dvdgdt_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_emitter,cogendaNodeID_qceb)];
  m_emitter_Equ_qceb_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_drain,cogendaNodeID_qceb)];
  m_drain_Equ_qceb_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_anode,cogendaNodeID_anode)];
  m_anode_Equ_anode_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_anode,cogendaNodeID_emitter)];
  m_anode_Equ_emitter_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_emitter,cogendaNodeID_anode)];
  m_emitter_Equ_anode_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_emitter,cogendaNodeID_emitter)];
  m_emitter_Equ_emitter_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dvdgdt,cogendaBRA_ID_dvdgdt_GND)];
  m_dvdgdt_Equ_BRA_dvdgdt_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_drain)];
  m_BRA_dvdgdt_GND_Equ_drain_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_gate)];
  m_BRA_dvdgdt_GND_Equ_gate_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvdgdt_GND,cogendaNodeID_dvdgdt)];
  m_BRA_dvdgdt_GND_Equ_dvdgdt_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dvbcdt,cogendaBRA_ID_dvbcdt_GND)];
  m_dvbcdt_Equ_BRA_dvbcdt_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_drain)];
  m_BRA_dvbcdt_GND_Equ_drain_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_cathode)];
  m_BRA_dvbcdt_GND_Equ_cathode_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_dvbcdt_GND,cogendaNodeID_dvbcdt)];
  m_BRA_dvbcdt_GND_Equ_dvbcdt_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_qceb,cogendaBRA_ID_qceb_GND)];
  m_qceb_Equ_BRA_qceb_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_qceb)];
  m_BRA_qceb_GND_Equ_qceb_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_emitter)];
  m_BRA_qceb_GND_Equ_emitter_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_qceb_GND,cogendaNodeID_drain)];
  m_BRA_qceb_GND_Equ_drain_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_nsat,cogendaBRA_ID_nsat_GND)];
  m_nsat_Equ_BRA_nsat_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_nsat_GND,cogendaNodeID_nsat)];
  m_BRA_nsat_GND_Equ_nsat_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_mucinv,cogendaBRA_ID_mucinv_GND)];
  m_mucinv_Equ_BRA_mucinv_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_mucinv_GND,cogendaNodeID_mucinv)];
  m_BRA_mucinv_GND_Equ_mucinv_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_gate_Equ_gate_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gate,li_gate);
  f_gate_Equ_cathode_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gate,li_cathode);
  f_cathode_Equ_gate_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_cathode,li_gate);
  f_cathode_Equ_cathode_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_cathode,li_cathode);
  f_drain_Equ_dvdgdt_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_drain,li_dvdgdt);
  f_gate_Equ_dvdgdt_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gate,li_dvdgdt);
  f_emitter_Equ_qceb_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_emitter,li_qceb);
  f_drain_Equ_qceb_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_drain,li_qceb);
  f_anode_Equ_anode_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_anode,li_anode);
  f_anode_Equ_emitter_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_anode,li_emitter);
  f_emitter_Equ_anode_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_emitter,li_anode);
  f_emitter_Equ_emitter_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_emitter,li_emitter);
  f_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dvdgdt,li_BRA_dvdgdt_GND);
  f_BRA_dvdgdt_GND_Equ_drain_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_drain);
  f_BRA_dvdgdt_GND_Equ_gate_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_gate);
  f_BRA_dvdgdt_GND_Equ_dvdgdt_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_dvdgdt);
  f_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dvbcdt,li_BRA_dvbcdt_GND);
  f_BRA_dvbcdt_GND_Equ_drain_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_drain);
  f_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_cathode);
  f_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_dvbcdt);
  f_qceb_Equ_BRA_qceb_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_qceb,li_BRA_qceb_GND);
  f_BRA_qceb_GND_Equ_qceb_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_qceb);
  f_BRA_qceb_GND_Equ_emitter_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_emitter);
  f_BRA_qceb_GND_Equ_drain_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_drain);
  f_nsat_Equ_BRA_nsat_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_nsat,li_BRA_nsat_GND);
  f_BRA_nsat_GND_Equ_nsat_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_nsat_GND,li_nsat);
  f_mucinv_Equ_BRA_mucinv_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_mucinv,li_BRA_mucinv_GND);
  f_BRA_mucinv_GND_Equ_mucinv_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_mucinv_GND,li_mucinv);
  q_gate_Equ_gate_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gate,li_gate);
  q_gate_Equ_cathode_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gate,li_cathode);
  q_cathode_Equ_gate_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_cathode,li_gate);
  q_cathode_Equ_cathode_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_cathode,li_cathode);
  q_drain_Equ_dvdgdt_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_drain,li_dvdgdt);
  q_gate_Equ_dvdgdt_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gate,li_dvdgdt);
  q_emitter_Equ_qceb_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_emitter,li_qceb);
  q_drain_Equ_qceb_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_drain,li_qceb);
  q_anode_Equ_anode_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_anode,li_anode);
  q_anode_Equ_emitter_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_anode,li_emitter);
  q_emitter_Equ_anode_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_emitter,li_anode);
  q_emitter_Equ_emitter_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_emitter,li_emitter);
  q_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dvdgdt,li_BRA_dvdgdt_GND);
  q_BRA_dvdgdt_GND_Equ_drain_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_drain);
  q_BRA_dvdgdt_GND_Equ_gate_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_gate);
  q_BRA_dvdgdt_GND_Equ_dvdgdt_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvdgdt_GND,li_dvdgdt);
  q_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dvbcdt,li_BRA_dvbcdt_GND);
  q_BRA_dvbcdt_GND_Equ_drain_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_drain);
  q_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_cathode);
  q_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_dvbcdt_GND,li_dvbcdt);
  q_qceb_Equ_BRA_qceb_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_qceb,li_BRA_qceb_GND);
  q_BRA_qceb_GND_Equ_qceb_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_qceb);
  q_BRA_qceb_GND_Equ_emitter_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_emitter);
  q_BRA_qceb_GND_Equ_drain_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_qceb_GND,li_drain);
  q_nsat_Equ_BRA_nsat_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_nsat,li_BRA_nsat_GND);
  q_BRA_nsat_GND_Equ_nsat_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_nsat_GND,li_nsat);
  q_mucinv_Equ_BRA_mucinv_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_mucinv,li_BRA_mucinv_GND);
  q_BRA_mucinv_GND_Equ_mucinv_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_mucinv_GND,li_mucinv);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_anode] += staticContributions[cogendaNodeID_anode].val();
  (*extData.daeFVectorPtr)[li_gate] += staticContributions[cogendaNodeID_gate].val();
  (*extData.daeFVectorPtr)[li_cathode] += staticContributions[cogendaNodeID_cathode].val();
  (*extData.daeFVectorPtr)[li_nsat] += staticContributions[cogendaNodeID_nsat].val();
  (*extData.daeFVectorPtr)[li_qceb] += staticContributions[cogendaNodeID_qceb].val();
  (*extData.daeFVectorPtr)[li_mucinv] += staticContributions[cogendaNodeID_mucinv].val();
  (*extData.daeFVectorPtr)[li_drain] += staticContributions[cogendaNodeID_drain].val();
  (*extData.daeFVectorPtr)[li_emitter] += staticContributions[cogendaNodeID_emitter].val();
  (*extData.daeFVectorPtr)[li_dvdgdt] += staticContributions[cogendaNodeID_dvdgdt].val();
  (*extData.daeFVectorPtr)[li_dvbcdt] += staticContributions[cogendaNodeID_dvbcdt].val();
  (*extData.daeFVectorPtr)[li_BRA_dvdgdt_GND] += staticContributions[cogendaBRA_ID_dvdgdt_GND].val();
  (*extData.daeFVectorPtr)[li_BRA_dvbcdt_GND] += staticContributions[cogendaBRA_ID_dvbcdt_GND].val();
  (*extData.daeFVectorPtr)[li_BRA_qceb_GND] += staticContributions[cogendaBRA_ID_qceb_GND].val();
  (*extData.daeFVectorPtr)[li_BRA_nsat_GND] += staticContributions[cogendaBRA_ID_nsat_GND].val();
  (*extData.daeFVectorPtr)[li_BRA_mucinv_GND] += staticContributions[cogendaBRA_ID_mucinv_GND].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_ianode] = leadCurrentF[cogendaNodeID_anode];
          leadF[li_branch_igate] = leadCurrentF[cogendaNodeID_gate];
          leadF[li_branch_icathode] = leadCurrentF[cogendaNodeID_cathode];
          leadF[li_branch_insat] = leadCurrentF[cogendaNodeID_nsat];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_anode] += dynamicContributions[cogendaNodeID_anode].val();
  (*extData.daeQVectorPtr)[li_gate] += dynamicContributions[cogendaNodeID_gate].val();
  (*extData.daeQVectorPtr)[li_cathode] += dynamicContributions[cogendaNodeID_cathode].val();
  (*extData.daeQVectorPtr)[li_nsat] += dynamicContributions[cogendaNodeID_nsat].val();
  (*extData.daeQVectorPtr)[li_qceb] += dynamicContributions[cogendaNodeID_qceb].val();
  (*extData.daeQVectorPtr)[li_mucinv] += dynamicContributions[cogendaNodeID_mucinv].val();
  (*extData.daeQVectorPtr)[li_drain] += dynamicContributions[cogendaNodeID_drain].val();
  (*extData.daeQVectorPtr)[li_emitter] += dynamicContributions[cogendaNodeID_emitter].val();
  (*extData.daeQVectorPtr)[li_dvdgdt] += dynamicContributions[cogendaNodeID_dvdgdt].val();
  (*extData.daeQVectorPtr)[li_dvbcdt] += dynamicContributions[cogendaNodeID_dvbcdt].val();
  (*extData.daeQVectorPtr)[li_BRA_dvdgdt_GND] += dynamicContributions[cogendaBRA_ID_dvdgdt_GND].val();
  (*extData.daeQVectorPtr)[li_BRA_dvbcdt_GND] += dynamicContributions[cogendaBRA_ID_dvbcdt_GND].val();
  (*extData.daeQVectorPtr)[li_BRA_qceb_GND] += dynamicContributions[cogendaBRA_ID_qceb_GND].val();
  (*extData.daeQVectorPtr)[li_BRA_nsat_GND] += dynamicContributions[cogendaBRA_ID_nsat_GND].val();
  (*extData.daeQVectorPtr)[li_BRA_mucinv_GND] += dynamicContributions[cogendaBRA_ID_mucinv_GND].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_ianode] = leadCurrentQ[cogendaNodeID_anode];
          leadQ[li_branch_igate] = leadCurrentQ[cogendaNodeID_gate];
          leadQ[li_branch_icathode] = leadCurrentQ[cogendaNodeID_cathode];
          leadQ[li_branch_insat] = leadCurrentQ[cogendaNodeID_nsat];
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
  if (probeVars.size() != (12))
  {
    probeVars.resize(12);
    staticContributions.resize(10+5);
    dynamicContributions.resize(10+5);
  }

  for (int i=0; i < 10+5 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_I_dvdgdt_GND] = (*solVectorPtr)[li_BRA_dvdgdt_GND];
  probeVars[cogendaProbeID_I_dvdgdt_GND].diff(cogendaProbeID_I_dvdgdt_GND,12);
  probeVars[cogendaProbeID_I_dvbcdt_GND] = (*solVectorPtr)[li_BRA_dvbcdt_GND];
  probeVars[cogendaProbeID_I_dvbcdt_GND].diff(cogendaProbeID_I_dvbcdt_GND,12);
  probeVars[cogendaProbeID_I_qceb_GND] = (*solVectorPtr)[li_BRA_qceb_GND];
  probeVars[cogendaProbeID_I_qceb_GND].diff(cogendaProbeID_I_qceb_GND,12);
  probeVars[cogendaProbeID_I_nsat_GND] = (*solVectorPtr)[li_BRA_nsat_GND];
  probeVars[cogendaProbeID_I_nsat_GND].diff(cogendaProbeID_I_nsat_GND,12);
  probeVars[cogendaProbeID_I_mucinv_GND] = (*solVectorPtr)[li_BRA_mucinv_GND];
  probeVars[cogendaProbeID_I_mucinv_GND].diff(cogendaProbeID_I_mucinv_GND,12);
  probeVars[cogendaProbeID_V_drain_gate] = (*solVectorPtr)[li_drain] - (*solVectorPtr)[li_gate];
  probeVars[cogendaProbeID_V_drain_gate].diff(cogendaProbeID_V_drain_gate,12);
  probeVars[cogendaProbeID_V_gate_cathode] = (*solVectorPtr)[li_gate] - (*solVectorPtr)[li_cathode];
  probeVars[cogendaProbeID_V_gate_cathode].diff(cogendaProbeID_V_gate_cathode,12);
  probeVars[cogendaProbeID_V_drain_cathode] = (*solVectorPtr)[li_drain] - (*solVectorPtr)[li_cathode];
  probeVars[cogendaProbeID_V_drain_cathode].diff(cogendaProbeID_V_drain_cathode,12);
  probeVars[cogendaProbeID_V_anode_emitter] = (*solVectorPtr)[li_anode] - (*solVectorPtr)[li_emitter];
  probeVars[cogendaProbeID_V_anode_emitter].diff(cogendaProbeID_V_anode_emitter,12);
  probeVars[cogendaProbeID_V_emitter_drain] = (*solVectorPtr)[li_emitter] - (*solVectorPtr)[li_drain];
  probeVars[cogendaProbeID_V_emitter_drain].diff(cogendaProbeID_V_emitter_drain,12);
  probeVars[cogendaProbeID_V_dvdgdt_GND] = (*solVectorPtr)[li_dvdgdt];
  probeVars[cogendaProbeID_V_dvdgdt_GND].diff(cogendaProbeID_V_dvdgdt_GND,12);
  probeVars[cogendaProbeID_V_qceb_GND] = (*solVectorPtr)[li_qceb];
  probeVars[cogendaProbeID_V_qceb_GND].diff(cogendaProbeID_V_qceb_GND,12);

{
  vdg = probeVars[cogendaProbeID_V_drain_gate];
  vgs = probeVars[cogendaProbeID_V_gate_cathode];
  vds = probeVars[cogendaProbeID_V_drain_cathode];
  vae = probeVars[cogendaProbeID_V_anode_emitter];
  ved = probeVars[cogendaProbeID_V_emitter_drain];
  qcgs = (cgs*vgs);
  icdg = (cgd*probeVars[cogendaProbeID_V_dvdgdt_GND]);
//Q-contrib...
  dynamicContributions[cogendaNodeID_gate] += (qcgs);
  dynamicContributions[cogendaNodeID_cathode] -= (qcgs);
//I-contrib...
  staticContributions[cogendaNodeID_drain] += icdg;
  staticContributions[cogendaNodeID_gate] -= icdg;
//Q-contrib...
  dynamicContributions[cogendaNodeID_drain] += ((imos+imult)+(qcdsj));
  dynamicContributions[cogendaNodeID_cathode] -= ((imos+imult)+(qcdsj));
//I-contrib...
  staticContributions[cogendaNodeID_emitter] += ic;
  staticContributions[cogendaNodeID_cathode] -= ic;
//Q-contrib...
  dynamicContributions[cogendaNodeID_emitter] += (ibp+(probeVars[cogendaProbeID_V_qceb_GND]));
  dynamicContributions[cogendaNodeID_drain] -= (ibp+(probeVars[cogendaProbeID_V_qceb_GND]));
//I-contrib...
  staticContributions[cogendaNodeID_anode] += (vae/rb);
  staticContributions[cogendaNodeID_emitter] -= (vae/rb);
//V-contrib...
  dynamicContributions[cogendaBRA_ID_dvdgdt_GND] += (vdg);
//V-contrib...
  dynamicContributions[cogendaBRA_ID_dvbcdt_GND] += (vds);
//V-contrib...
  staticContributions[cogendaBRA_ID_qceb_GND] += ((probeVars[cogendaProbeID_V_qceb_GND]-vebj)+ved);
//V-contrib...
  staticContributions[cogendaBRA_ID_nsat_GND] += nsat1;
//V-contrib...
  staticContributions[cogendaBRA_ID_mucinv_GND] += mucinv1;
}
staticContributions[cogendaNodeID_dvdgdt] += probeVars[cogendaProbeID_I_dvdgdt_GND];
staticContributions[cogendaBRA_ID_dvdgdt_GND] -= (*solVectorPtr)[li_dvdgdt];
staticContributions[cogendaNodeID_dvbcdt] += probeVars[cogendaProbeID_I_dvbcdt_GND];
staticContributions[cogendaBRA_ID_dvbcdt_GND] -= (*solVectorPtr)[li_dvbcdt];
staticContributions[cogendaNodeID_qceb] += probeVars[cogendaProbeID_I_qceb_GND];
staticContributions[cogendaBRA_ID_qceb_GND] -= (*solVectorPtr)[li_qceb];
staticContributions[cogendaNodeID_nsat] += probeVars[cogendaProbeID_I_nsat_GND];
staticContributions[cogendaBRA_ID_nsat_GND] -= (*solVectorPtr)[li_nsat];
staticContributions[cogendaNodeID_mucinv] += probeVars[cogendaProbeID_I_mucinv_GND];
staticContributions[cogendaBRA_ID_mucinv_GND] -= (*solVectorPtr)[li_mucinv];
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 4 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 10; unCtNode++) {
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
  (*f_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_dvdgdt].dx(cogendaProbeID_I_dvdgdt_GND);
  (*f_BRA_dvdgdt_GND_Equ_dvdgdt_Node_Ptr) +=  -1;
  (*f_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_dvbcdt].dx(cogendaProbeID_I_dvbcdt_GND);
  (*f_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr) +=  -1;
  (*f_qceb_Equ_BRA_qceb_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  (*f_BRA_qceb_GND_Equ_qceb_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_qceb_GND);
  (*f_BRA_qceb_GND_Equ_emitter_Node_Ptr) +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  (*f_BRA_qceb_GND_Equ_drain_Node_Ptr) +=  -staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  (*f_BRA_qceb_GND_Equ_qceb_Node_Ptr) +=  -1;
  (*f_nsat_Equ_BRA_nsat_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_nsat].dx(cogendaProbeID_I_nsat_GND);
  (*f_BRA_nsat_GND_Equ_nsat_Node_Ptr) +=  -1;
  (*f_mucinv_Equ_BRA_mucinv_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_mucinv].dx(cogendaProbeID_I_mucinv_GND);
  (*f_BRA_mucinv_GND_Equ_mucinv_Node_Ptr) +=  -1;
  (*f_anode_Equ_anode_Node_Ptr) +=  +staticContributions[cogendaNodeID_anode].dx(cogendaProbeID_V_anode_emitter);
  (*f_anode_Equ_emitter_Node_Ptr) +=  -staticContributions[cogendaNodeID_anode].dx(cogendaProbeID_V_anode_emitter);
  (*f_drain_Equ_dvdgdt_Node_Ptr) +=  +staticContributions[cogendaNodeID_drain].dx(cogendaProbeID_V_dvdgdt_GND);
  (*f_emitter_Equ_anode_Node_Ptr) +=  +staticContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_anode_emitter);
  (*f_emitter_Equ_emitter_Node_Ptr) +=  -staticContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_anode_emitter);
  (*f_gate_Equ_dvdgdt_Node_Ptr) +=  +staticContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_dvdgdt_GND);
#else
  //use the offsets instead of pointers
  dFdx[li_dvdgdt][m_dvdgdt_Equ_BRA_dvdgdt_GND_VarOffset] +=  +staticContributions[cogendaNodeID_dvdgdt].dx(cogendaProbeID_I_dvdgdt_GND);
  dFdx[li_BRA_dvdgdt_GND][m_BRA_dvdgdt_GND_Equ_dvdgdt_NodeOffset] +=  -1;
  dFdx[li_dvbcdt][m_dvbcdt_Equ_BRA_dvbcdt_GND_VarOffset] +=  +staticContributions[cogendaNodeID_dvbcdt].dx(cogendaProbeID_I_dvbcdt_GND);
  dFdx[li_BRA_dvbcdt_GND][m_BRA_dvbcdt_GND_Equ_dvbcdt_NodeOffset] +=  -1;
  dFdx[li_qceb][m_qceb_Equ_BRA_qceb_GND_VarOffset] +=  +staticContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_qceb_NodeOffset] +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_qceb_GND);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_emitter_NodeOffset] +=  +staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_drain_NodeOffset] +=  -staticContributions[cogendaBRA_ID_qceb_GND].dx(cogendaProbeID_V_emitter_drain);
  dFdx[li_BRA_qceb_GND][m_BRA_qceb_GND_Equ_qceb_NodeOffset] +=  -1;
  dFdx[li_nsat][m_nsat_Equ_BRA_nsat_GND_VarOffset] +=  +staticContributions[cogendaNodeID_nsat].dx(cogendaProbeID_I_nsat_GND);
  dFdx[li_BRA_nsat_GND][m_BRA_nsat_GND_Equ_nsat_NodeOffset] +=  -1;
  dFdx[li_mucinv][m_mucinv_Equ_BRA_mucinv_GND_VarOffset] +=  +staticContributions[cogendaNodeID_mucinv].dx(cogendaProbeID_I_mucinv_GND);
  dFdx[li_BRA_mucinv_GND][m_BRA_mucinv_GND_Equ_mucinv_NodeOffset] +=  -1;
  dFdx[li_anode][m_anode_Equ_anode_NodeOffset] +=  +staticContributions[cogendaNodeID_anode].dx(cogendaProbeID_V_anode_emitter);
  dFdx[li_anode][m_anode_Equ_emitter_NodeOffset] +=  -staticContributions[cogendaNodeID_anode].dx(cogendaProbeID_V_anode_emitter);
  dFdx[li_drain][m_drain_Equ_dvdgdt_NodeOffset] +=  +staticContributions[cogendaNodeID_drain].dx(cogendaProbeID_V_dvdgdt_GND);
  dFdx[li_emitter][m_emitter_Equ_anode_NodeOffset] +=  +staticContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_anode_emitter);
  dFdx[li_emitter][m_emitter_Equ_emitter_NodeOffset] +=  -staticContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_anode_emitter);
  dFdx[li_gate][m_gate_Equ_dvdgdt_NodeOffset] +=  +staticContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_dvdgdt_GND);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_dvdgdt_Equ_BRA_dvdgdt_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_dvdgdt].dx(cogendaProbeID_I_dvdgdt_GND);
  (*q_BRA_dvdgdt_GND_Equ_drain_Node_Ptr) +=  +dynamicContributions[cogendaBRA_ID_dvdgdt_GND].dx(cogendaProbeID_V_drain_gate);
  (*q_BRA_dvdgdt_GND_Equ_gate_Node_Ptr) +=  -dynamicContributions[cogendaBRA_ID_dvdgdt_GND].dx(cogendaProbeID_V_drain_gate);
  (*q_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_dvbcdt].dx(cogendaProbeID_I_dvbcdt_GND);
  (*q_BRA_dvbcdt_GND_Equ_drain_Node_Ptr) +=  +dynamicContributions[cogendaBRA_ID_dvbcdt_GND].dx(cogendaProbeID_V_drain_cathode);
  (*q_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr) +=  -dynamicContributions[cogendaBRA_ID_dvbcdt_GND].dx(cogendaProbeID_V_drain_cathode);
  (*q_qceb_Equ_BRA_qceb_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  (*q_nsat_Equ_BRA_nsat_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_nsat].dx(cogendaProbeID_I_nsat_GND);
  (*q_mucinv_Equ_BRA_mucinv_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_mucinv].dx(cogendaProbeID_I_mucinv_GND);
  (*q_cathode_Equ_cathode_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_cathode].dx(cogendaProbeID_V_gate_cathode);
  (*q_cathode_Equ_gate_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_cathode].dx(cogendaProbeID_V_gate_cathode);
  (*q_drain_Equ_qceb_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_drain].dx(cogendaProbeID_V_qceb_qceb);
  (*q_emitter_Equ_qceb_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_qceb_qceb);
  (*q_gate_Equ_cathode_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_gate_cathode);
  (*q_gate_Equ_gate_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_gate_cathode);
#else
  //use the offsets instead of pointers
  dQdx[li_dvdgdt][m_dvdgdt_Equ_BRA_dvdgdt_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_dvdgdt].dx(cogendaProbeID_I_dvdgdt_GND);
  dQdx[li_BRA_dvdgdt_GND][m_BRA_dvdgdt_GND_Equ_drain_NodeOffset] +=  +dynamicContributions[cogendaBRA_ID_dvdgdt_GND].dx(cogendaProbeID_V_drain_gate);
  dQdx[li_BRA_dvdgdt_GND][m_BRA_dvdgdt_GND_Equ_gate_NodeOffset] +=  -dynamicContributions[cogendaBRA_ID_dvdgdt_GND].dx(cogendaProbeID_V_drain_gate);
  dQdx[li_dvbcdt][m_dvbcdt_Equ_BRA_dvbcdt_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_dvbcdt].dx(cogendaProbeID_I_dvbcdt_GND);
  dQdx[li_BRA_dvbcdt_GND][m_BRA_dvbcdt_GND_Equ_drain_NodeOffset] +=  +dynamicContributions[cogendaBRA_ID_dvbcdt_GND].dx(cogendaProbeID_V_drain_cathode);
  dQdx[li_BRA_dvbcdt_GND][m_BRA_dvbcdt_GND_Equ_cathode_NodeOffset] +=  -dynamicContributions[cogendaBRA_ID_dvbcdt_GND].dx(cogendaProbeID_V_drain_cathode);
  dQdx[li_qceb][m_qceb_Equ_BRA_qceb_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_qceb].dx(cogendaProbeID_I_qceb_GND);
  dQdx[li_nsat][m_nsat_Equ_BRA_nsat_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_nsat].dx(cogendaProbeID_I_nsat_GND);
  dQdx[li_mucinv][m_mucinv_Equ_BRA_mucinv_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_mucinv].dx(cogendaProbeID_I_mucinv_GND);
  dQdx[li_cathode][m_cathode_Equ_cathode_NodeOffset] +=  -dynamicContributions[cogendaNodeID_cathode].dx(cogendaProbeID_V_gate_cathode);
  dQdx[li_cathode][m_cathode_Equ_gate_NodeOffset] +=  +dynamicContributions[cogendaNodeID_cathode].dx(cogendaProbeID_V_gate_cathode);
  dQdx[li_drain][m_drain_Equ_qceb_NodeOffset] +=  -dynamicContributions[cogendaNodeID_drain].dx(cogendaProbeID_V_qceb_qceb);
  dQdx[li_emitter][m_emitter_Equ_qceb_NodeOffset] +=  -dynamicContributions[cogendaNodeID_emitter].dx(cogendaProbeID_V_qceb_qceb);
  dQdx[li_gate][m_gate_Equ_cathode_NodeOffset] +=  -dynamicContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_gate_cathode);
  dQdx[li_gate][m_gate_Equ_gate_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gate].dx(cogendaProbeID_V_gate_cathode);
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
    .registerDevice("contrib", 1)
    .registerModelType("contrib", 1);
}
} // namespace COGENDAcontrib
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAcontrib::registerDevice(); }}; 
Bootstrap COGENDAcontrib_bootstrap;

