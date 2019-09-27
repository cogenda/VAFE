//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "switchcase.h"

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
namespace COGENDAswitchcase {
JacobianStamp Instance::jacStamp;
IdVector Instance::nodeMap;
PairMap Instance::pairToJacStampMap;

    const int Instance::cogendaNodeID_a;
    const int Instance::cogendaNodeID_c;
    const int Instance::cogendaNodeID_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAswitchcase::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAswitchcase::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("CJO",static_cast<double>(-9.999E-12), &COGENDAswitchcase::Instance::CJO);
  p.addPar("IS",static_cast<double>(1.0e-14), &COGENDAswitchcase::Instance::IS);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAswitchcase::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAswitchcase::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("CJO",static_cast<double>(-9.999E-12), &COGENDAswitchcase::Model::CJO);
  p.addPar("IS",static_cast<double>(1.0e-14), &COGENDAswitchcase::Model::IS);
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
    li_branch_ia(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 2;
  numIntVars = 0;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 2;
  leadCurrentF.resize(2);
  leadCurrentQ.resize(2);
  PairVector jacobianElements;

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
    int originalSize = 2;
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
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
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
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_a] += staticContributions[cogendaNodeID_a].val();
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
  if (probeVars.size() != (0))
  {
    probeVars.resize(0);
    staticContributions.resize(2+0);
    dynamicContributions.resize(2+0);
  }

  for (int i=0; i < 2+0 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }


{
  count = ((1)*UNITFAD).val();
  switch(BSIM4dioMod){
  case 0:
    {
      evbs = ((exp((vbs_jct/Nvtms)))*UNITFAD).val();
      T1 = (((BSIM4xjbvs*exp((-(BSIM4bvs+vbs_jct)/Nvtms))))*UNITFAD).val();
      BSIM4gbs = (((((SourceSatCurrent*(evbs+T1))/Nvtms)+_ckt_gmin))*UNITFAD).val();
    }
    break;
  case 1:
    {
      T2 = (((vbs_jct/Nvtms))*UNITFAD).val();
      if((T2 < -1e34)) {
        {
          BSIM4gbs = ((_ckt_gmin)*UNITFAD).val();
        }
      }
          else if((vbs_jct <= BSIM4vjsmFwd)) {
        {
          evbs = ((exp(T2))*UNITFAD).val();
          BSIM4gbs = (((((SourceSatCurrent*evbs)/Nvtms)+_ckt_gmin))*UNITFAD).val();
        }
      }
      else {
        {
          T0 = (((BSIM4IVjsmFwd/Nvtms))*UNITFAD).val();
          BSIM4gbs = (((T0+_ckt_gmin))*UNITFAD).val();
        }
      }
    }
    break;
  case 2:
  case 3:
  case 4:
    {
      if((vbs_jct < BSIM4vjsmRev)) {
        {
          T0 = (((vbs_jct/Nvtms))*UNITFAD).val();
          if((T0 < -1e34)) {
            {
              evbs = ((1e-29)*UNITFAD).val();
              devbs_dvb = ((0.0)*UNITFAD).val();
            }
          }
          else {
            {
              evbs = ((exp(T0))*UNITFAD).val();
              devbs_dvb = (((evbs/Nvtms))*UNITFAD).val();
            }
          }
          T1 = (((evbs-1.0))*UNITFAD).val();
          T2 = (((BSIM4IVjsmRev+(BSIM4SslpRev*(vbs_jct-BSIM4vjsmRev))))*UNITFAD).val();
          BSIM4gbs = (((((devbs_dvb*T2)+(T1*BSIM4SslpRev))+_ckt_gmin))*UNITFAD).val();
          BSIM4cbs = ((((T1*T2)+(_ckt_gmin*vbs_jct)))*UNITFAD).val();
          printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso 
                     type !=1)\n",rgeo);
        }
      }
          else if((vbs_jct <= BSIM4vjsmFwd)) {
        {
          T0 = (((vbs_jct/Nvtms))*UNITFAD).val();
          if((T0 < -1e34)) {
            {
              evbs = ((1e-29)*UNITFAD).val();
              devbs_dvb = ((0.0)*UNITFAD).val();
            }
          }
          else {
            {
              evbs = ((exp(T0))*UNITFAD).val();
              devbs_dvb = (((evbs/Nvtms))*UNITFAD).val();
            }
          }
          T1 = ((((BSIM4bvs+vbs_jct)/Nvtms))*UNITFAD).val();
          if((T1 > 1e34)) {
            {
              T2 = ((1e-29)*UNITFAD).val();
              T3 = ((0.0)*UNITFAD).val();
            }
          }
          else {
            {
              T2 = ((exp(-T1))*UNITFAD).val();
              T3 = (((-T2/Nvtms))*UNITFAD).val();
            }
          }
          BSIM4gbs = ((((SourceSatCurrent*(devbs_dvb-(BSIM4xjbvs*T3)))+_ckt_gmin))*UNITFAD).val();
        }
      }
      else {
        {
          BSIM4gbs = (((BSIM4SslpFwd+_ckt_gmin))*UNITFAD).val();
          BSIM4cbs = ((((BSIM4IVjsmFwd+(BSIM4SslpFwd*(vbs_jct-BSIM4vjsmFwd)))+(_ckt_gmin*vbs_jct)))*UNITFAD).val();
        }
      }
    }
    break;
  default:
    {
      printf("This is a default case with \
                    BSIM4dioMod=%d\n",BSIM4dioMod);
    }
    break;
  }
}
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
#else
  //use the offsets instead of pointers
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
  if(!given("CJO"))
    CJO = -9.999E-12;
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
    CJO(-9.999E-12),
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
    .registerDevice("switchcase", 1)
    .registerModelType("switchcase", 1);
}
} // namespace COGENDAswitchcase
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAswitchcase::registerDevice(); }}; 
Bootstrap COGENDAswitchcase_bootstrap;

