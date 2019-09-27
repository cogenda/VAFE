//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "vco.h"

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
namespace COGENDAvco {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_in;
    const int Instance::cogendaNodeID_out;
    const int Instance::cogendaNodeID_GND;
    const int Instance::cogendaBRA_ID_out_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAvco::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAvco::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("FMAX",static_cast<double>((2*this->fmin)), &COGENDAvco::Instance::fmax);
  p.addPar("FMIN",static_cast<double>(1e6), &COGENDAvco::Instance::fmin);
  p.addPar("JITTER",static_cast<double>(0e-12), &COGENDAvco::Instance::jitter);
  p.addPar("TT",static_cast<double>((0.01/this->fmax)), &COGENDAvco::Instance::tt);
  p.addPar("TTOL",static_cast<double>((1e-6/this->fmax)), &COGENDAvco::Instance::ttol);
  p.addPar("VH",static_cast<double>(1e-3), &COGENDAvco::Instance::vh);
  p.addPar("VL",static_cast<double>(-1e-15), &COGENDAvco::Instance::vl);
  p.addPar("VMAX",static_cast<double>((this->vmin+1)), &COGENDAvco::Instance::vmax);
  p.addPar("VMIN",static_cast<double>(0e-18), &COGENDAvco::Instance::vmin);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAvco::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAvco::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("FMAX",static_cast<double>((2*this->fmin)), &COGENDAvco::Model::fmax);
  p.addPar("FMIN",static_cast<double>(1e6), &COGENDAvco::Model::fmin);
  p.addPar("JITTER",static_cast<double>(0e-12), &COGENDAvco::Model::jitter);
  p.addPar("TT",static_cast<double>((0.01/this->fmax)), &COGENDAvco::Model::tt);
  p.addPar("TTOL",static_cast<double>((1e-6/this->fmax)), &COGENDAvco::Model::ttol);
  p.addPar("VH",static_cast<double>(1e-3), &COGENDAvco::Model::vh);
  p.addPar("VL",static_cast<double>(-1e-15), &COGENDAvco::Model::vl);
  p.addPar("VMAX",static_cast<double>((this->vmin+1)), &COGENDAvco::Model::vmax);
  p.addPar("VMIN",static_cast<double>(0e-18), &COGENDAvco::Model::vmin);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("FMAX"))
    fmax = model_.fmax;
  if(!given("FMIN"))
    fmin = model_.fmin;
  if(!given("JITTER"))
    jitter = model_.jitter;
  if(!given("TT"))
    tt = model_.tt;
  if(!given("TTOL"))
    ttol = model_.ttol;
  if(!given("VH"))
    vh = model_.vh;
  if(!given("VL"))
    vl = model_.vl;
  if(!given("VMAX"))
    vmax = model_.vmax;
  if(!given("VMIN"))
    vmin = model_.vmin;
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
    li_in(-1),
    li_out(-1),
    li_BRA_out_GND(-1),
    li_branch_iout(-1),
    f_out_Equ_BRA_out_GND_Var_Ptr(0),
    f_BRA_out_GND_Equ_out_Node_Ptr(0),
    q_out_Equ_BRA_out_GND_Var_Ptr(0),
    q_BRA_out_GND_Equ_out_Node_Ptr(0),
    m_out_Equ_BRA_out_GND_VarOffset(-1),
    m_BRA_out_GND_Equ_out_NodeOffset(-1),
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
  jacobianElements.push_back(IntPair(cogendaNodeID_out,cogendaBRA_ID_out_GND));
  jacobianElements.push_back(IntPair(cogendaBRA_ID_out_GND,cogendaNodeID_out));

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

  li_in = localLIDVec[nodeMap[cogendaNodeID_in]];
  li_out = localLIDVec[nodeMap[cogendaNodeID_out]];
  li_BRA_out_GND = localLIDVec[nodeMap[cogendaBRA_ID_out_GND]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
  /*The initial_step block stars...*/
  seed = ((-561)*UNITFAD).val();  /*The initial_step block ends...*/
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  addInternalNode(symbol_table, li_BRA_out_GND, getName(), "out_GND_branch");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_iout, getName(), "BRANCH_D1");
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
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_out,cogendaBRA_ID_out_GND)];
  m_out_Equ_BRA_out_GND_VarOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaBRA_ID_out_GND,cogendaNodeID_out)];
  m_BRA_out_GND_Equ_out_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_out_Equ_BRA_out_GND_Var_Ptr = dFdxMatPtr->returnRawEntryPointer(li_out,li_BRA_out_GND);
  f_BRA_out_GND_Equ_out_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_BRA_out_GND,li_out);
  q_out_Equ_BRA_out_GND_Var_Ptr = dQdxMatPtr->returnRawEntryPointer(li_out,li_BRA_out_GND);
  q_BRA_out_GND_Equ_out_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_BRA_out_GND,li_out);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_in] += staticContributions[cogendaNodeID_in].val();
  (*extData.daeFVectorPtr)[li_out] += staticContributions[cogendaNodeID_out].val();
  (*extData.daeFVectorPtr)[li_BRA_out_GND] += staticContributions[cogendaBRA_ID_out_GND].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_iout] = leadCurrentF[cogendaNodeID_out];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_in] += dynamicContributions[cogendaNodeID_in].val();
  (*extData.daeQVectorPtr)[li_out] += dynamicContributions[cogendaNodeID_out].val();
  (*extData.daeQVectorPtr)[li_BRA_out_GND] += dynamicContributions[cogendaBRA_ID_out_GND].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
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

  probeVars[cogendaProbeID_I_out_GND] = (*solVectorPtr)[li_BRA_out_GND];
  probeVars[cogendaProbeID_I_out_GND].diff(cogendaProbeID_I_out_GND,2);
  probeVars[cogendaProbeID_V_in_GND] = (*solVectorPtr)[li_in];
  probeVars[cogendaProbeID_V_in_GND].diff(cogendaProbeID_V_in_GND,2);

{
  freq = ((((probeVars[cogendaProbeID_V_in_GND]-this->vmin)*(this->fmax-this->fmin))/(this->vmax-this->vmin))+this->fmin);
  if((freq > this->fmax)) {
    freq = this->fmax;
  }
  if((freq < this->fmin)) {
    freq = this->fmin;
  }
  freq = (freq/(1+(dT*freq)));
  if(((freq%2) != 0)) {
    {
      this->fmax = ((1.0)*UNITFAD).val();
      this->vmax = ((1.0)*UNITFAD).val();
      dT = (2.0*max(((freq-1.0)/2.0),0.0));
      this->vmin = dT;
    }
  }
  else {
    {
      if((phase == 1)) {
        {
          this->fmax = ((2.0)*UNITFAD).val();
          dT = (2.0*max(((freq/2.0)-1.0),0.0));
          this->vmax = ((0.0)*UNITFAD).val();
          this->vmin = freq;
        }
      }
      else {
        {
          this->fmax = ((0.0)*UNITFAD).val();
          dT = freq;
          this->vmax = ((2.0)*UNITFAD).val();
          this->vmin = (2.0*max(((freq/2.0)-1.0),0.0));
        }
      }
    }
  }
  phase = ((((2*3.14159265358979323846)*idtmod(freq, 0.0, 1.0, -0.5)))*UNITFAD).val();
  (0.05/freq));
//V-contrib...
  staticContributions[cogendaBRA_ID_out_GND] += transition(n
  printf("T0,T1=",T0,T1);
  printf("Temperature effect has caused cjsw to be negative. \
              Cjsw is clamped to zero.\n");
  printf("ps,pd,ad,as(given):",BSIM4Pseff,BSIM4sourcePerimeter,BSIM4sourcePerimeterGiven,BSIM4Pdeff,BSIM4drainPerimeter,BSIM4drainPerimeterGiven,BSIM4Adeff,BSIM4drainArea,BSIM4drainAreaGiven,BSIM4Aseff,BSIM4sourceArea,BSIM4sourceAreaGiven);
}
staticContributions[cogendaNodeID_out] += probeVars[cogendaProbeID_I_out_GND];
staticContributions[cogendaBRA_ID_out_GND] -= (*solVectorPtr)[li_out];
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
  (*f_out_Equ_BRA_out_GND_Var_Ptr) +=  +staticContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_GND);
  (*f_BRA_out_GND_Equ_out_Node_Ptr) +=  -1;
#else
  //use the offsets instead of pointers
  dFdx[li_out][m_out_Equ_BRA_out_GND_VarOffset] +=  +staticContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_GND);
  dFdx[li_BRA_out_GND][m_BRA_out_GND_Equ_out_NodeOffset] +=  -1;
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_out_Equ_BRA_out_GND_Var_Ptr) +=  +dynamicContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_GND);
#else
  //use the offsets instead of pointers
  dQdx[li_out][m_out_Equ_BRA_out_GND_VarOffset] +=  +dynamicContributions[cogendaNodeID_out].dx(cogendaProbeID_I_out_GND);
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
  if(!given("FMAX"))
    fmax = (2*this->fmin);
  else if ( !(fmax > fmin))
    UserError0(*this) << "Model Parameter value " << fmax << " out of range (fmin:inf)";
  if(!given("FMIN"))
    fmin = 1e6;
  else if ( !(fmin > 0))
    UserError0(*this) << "Model Parameter value " << fmin << " out of range (0:inf)";
  if(!given("JITTER"))
    jitter = 0e-12;
  else if ( !(jitter >= 0 && jitter < 0.25/fmax ))
    UserError0(*this) << "Model Parameter value " << jitter << " out of range [0:0.25/fmax)";
  if(!given("TT"))
    tt = (0.01/this->fmax);
  else if ( !(tt > 0))
    UserError0(*this) << "Model Parameter value " << tt << " out of range (0:inf)";
  if(!given("TTOL"))
    ttol = (1e-6/this->fmax);
  else if ( !(ttol > 0 && ttol < 1/fmax ))
    UserError0(*this) << "Model Parameter value " << ttol << " out of range (0:1/fmax)";
  if(!given("VH"))
    vh = 1e-3;
  if(!given("VL"))
    vl = -1e-15;
  if(!given("VMAX"))
    vmax = (this->vmin+1);
  else if ( !(vmax > vmin))
    UserError0(*this) << "Model Parameter value " << vmax << " out of range (vmin:inf)";
  if(!given("VMIN"))
    vmin = 0e-18;
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
    fmax((2*this->fmin)),
    fmin(1e6),
    jitter(0e-12),
    tt((0.01/this->fmax)),
    ttol((1e-6/this->fmax)),
    vh(1e-3),
    vl(-1e-15),
    vmax((this->vmin+1)),
    vmin(0e-18)
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
    .registerDevice("vco", 1)
    .registerModelType("vco", 1);
}
} // namespace COGENDAvco
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAvco::registerDevice(); }}; 
Bootstrap COGENDAvco_bootstrap;

