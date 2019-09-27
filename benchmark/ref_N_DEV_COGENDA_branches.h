//-------------------------------------------------------------------------
#ifndef Xyce_N_DEV_branches_h
#define Xyce_N_DEV_branches_h

#include <Sacado.hpp>

// ----------   Xyce Includes in header ----------
#include <N_DEV_Configuration.h>
#include <N_DEV_Const.h>
#include <N_DEV_DeviceBlock.h>
#include <N_DEV_DeviceInstance.h>
#include <N_DEV_DeviceModel.h>
#include <N_UTL_Math.h>
// ---------- Macros Definitions ----------
#define KOVERQ        8.61734e-05
#define P_CELSIUS0    273.15
using std::max;
using std::min;
#define ELIM          1.0e+20
#define _VT0_(T) ((T) * KOVERQ)
#define _VT_ cogenda_vt_nom
#define _TEMPER_ cogendaTemperature
#define _LIMEXP_(x) ((x)<log(ELIM)? exp(x) : (ELIM*(x) + ELIM - ELIM*log(ELIM)))
#define _CURRTIME_ (getSolverState().currTime_)
#define analysis_noise (getSolverState().noiseFlag)
#define analysis_dc (getSolverState().dcsweepFlag || getSolverState().dcopFlag)
#define analysis_tran (getSolverState().transientFlag)

namespace Xyce {
namespace Device {
namespace COGENDAbranches {
// This typedef is for our automatic differentiation:
typedef Sacado::Fad::SFad<double,14> CogendaFadType;

class Model;
class Instance;
const CogendaFadType UNITFAD=CogendaFadType(1.0);
template<typename ArgIn>
static ArgIn cogenda_ternary_op(const bool cond,const ArgIn  &ifTrue, const ArgIn  &ifFalse){
 if (cond)
  return ifTrue;
 else
  return ifFalse;
}
template<typename ArgIn>
static ArgIn cogenda_ternary_op(const bool cond,const ArgIn  &ifTrue, const double &ifFalse){
 if (cond)
  return ifTrue;
 else
  return ArgIn(ifFalse);
}
template<typename ArgIn>
static ArgIn cogenda_ternary_op(const bool cond,const double &ifTrue, const ArgIn  &ifFalse){
 if (cond)
  return ArgIn(ifTrue);
 else
  return ifFalse;
}
template<typename ArgIn>
static ArgIn cogenda_ternary_op(const bool cond,const double &ifTrue, const double &ifFalse){
 if (cond)
  return ArgIn(ifTrue);
 else
  return ArgIn(ifFalse);
}

struct Traits: public DeviceTraits<Model, Instance>
{
  static const char *name() {return "COGENDA branches";}
  static const char *deviceTypeName() {return "branches level 1";}

  static int numNodes() {return 4;}
  static bool modelRequired() {return true;}
  static bool isLinearDevice() {return false;}

  static Device *factory(const Configuration &configuration, const FactoryBlock &factory_block);
  static void loadModelParameters(ParametricData<Model> &model_parameters);
  static void loadInstanceParameters(ParametricData<Instance> &instance_parameters);
};
class Instance : public DeviceInstance
{
  friend class ParametricData<Instance>;
  friend class Model;
  friend struct Traits;

  public:
    Instance(
      const Configuration &       configuration,
      const InstanceBlock &       instance_block,
      Model &                     model,
      const FactoryBlock &        factory_block);

    ~Instance();

private:
    Instance(const Instance &);
    Instance &operator=(const Instance &);

public:
    void registerLIDs( const LocalIdVector & intLIDVecRef,
                       const LocalIdVector & extLIDVecRef );
    void registerStoreLIDs( const LocalIdVector & stoLIDVecRef );
    void setupPointers();

    void loadNodeSymbols(Util::SymbolTable &symbol_table) const;

    const JacobianStamp & jacobianStamp() const;
    void registerJacLIDs( const JacobianStamp & jacLIDVec );

    void registerBranchDataLIDs(const std::vector<int> & branchLIDVecRef);

    bool processParams();
    void initInternalVars();
    bool updateTemperature ( const double & temp = -999.0 );
    bool updateIntermediateVars ();
    bool updatePrimaryState ();
    bool updateSecondaryState ();

    // load functions, residual:
    bool loadDAEQVector ();
    bool loadDAEFVector ();

    // load functions, Jacobian:
    bool loadDAEdQdx ();
    bool loadDAEdFdx ();
    void collapseNodes();


  private:

  public:
    Model &getModel() {  return model_; }

  private:
    Model & model_;   //< Owning Model
    //Node LID Variables
    int li_anode;
    int li_gate;
    int li_cathode;
    int li_nsat;
    int li_qceb;
    int li_mucinv;
    int li_drain;
    int li_emitter;
    int li_dvdgdt;
    int li_dvbcdt;
    //Branch LID Variables
    int li_BRA_gate_cathode;
    int li_BRA_dvdgdt_emitter;
    int li_BRA_dvbcdt_GND;
    int li_BRA_qceb_GND;
    int li_BRA_drain_cathode;
    int li_BRA_nsat_mucinv;
    int li_BRA_mucinv_GND;
    //Lead Branch Variables
    int li_branch_ianode;
    int li_branch_igate;
    int li_branch_icathode;
    int li_branch_insat;
    //Jacobian pointers
    double * f_anode_Equ_anode_Node_Ptr;
    double * f_anode_Equ_emitter_Node_Ptr;
    double * f_emitter_Equ_anode_Node_Ptr;
    double * f_emitter_Equ_emitter_Node_Ptr;
    double * f_gate_Equ_BRA_gate_cathode_Var_Ptr;
    double * f_BRA_gate_cathode_Equ_gate_Node_Ptr;
    double * f_cathode_Equ_BRA_gate_cathode_Var_Ptr;
    double * f_BRA_gate_cathode_Equ_cathode_Node_Ptr;
    double * f_BRA_gate_cathode_Equ_BRA_gate_cathode_Var_Ptr;
    double * f_gate_Equ_gate_Node_Ptr;
    double * f_gate_Equ_cathode_Node_Ptr;
    double * f_cathode_Equ_gate_Node_Ptr;
    double * f_cathode_Equ_cathode_Node_Ptr;
    double * f_drain_Equ_dvdgdt_Node_Ptr;
    double * f_gate_Equ_dvdgdt_Node_Ptr;
    double * f_emitter_Equ_qceb_Node_Ptr;
    double * f_drain_Equ_qceb_Node_Ptr;
    double * f_dvdgdt_Equ_BRA_dvdgdt_emitter_Var_Ptr;
    double * f_emitter_Equ_BRA_dvdgdt_emitter_Var_Ptr;
    double * f_BRA_dvdgdt_emitter_Equ_drain_Node_Ptr;
    double * f_BRA_dvdgdt_emitter_Equ_gate_Node_Ptr;
    double * f_BRA_dvdgdt_emitter_Equ_dvdgdt_Node_Ptr;
    double * f_BRA_dvdgdt_emitter_Equ_emitter_Node_Ptr;
    double * f_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr;
    double * f_BRA_dvbcdt_GND_Equ_drain_Node_Ptr;
    double * f_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr;
    double * f_BRA_dvbcdt_GND_Equ_emitter_Node_Ptr;
    double * f_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr;
    double * f_qceb_Equ_BRA_qceb_GND_Var_Ptr;
    double * f_BRA_qceb_GND_Equ_qceb_Node_Ptr;
    double * f_BRA_qceb_GND_Equ_emitter_Node_Ptr;
    double * f_BRA_qceb_GND_Equ_drain_Node_Ptr;
    double * f_nsat_Equ_BRA_nsat_mucinv_Var_Ptr;
    double * f_mucinv_Equ_BRA_nsat_mucinv_Var_Ptr;
    double * f_BRA_nsat_mucinv_Equ_nsat_Node_Ptr;
    double * f_BRA_nsat_mucinv_Equ_mucinv_Node_Ptr;
    double * f_drain_Equ_BRA_drain_cathode_Var_Ptr;
    double * f_cathode_Equ_BRA_drain_cathode_Var_Ptr;
    double * f_BRA_nsat_mucinv_Equ_BRA_drain_cathode_Var_Ptr;
    double * f_BRA_drain_cathode_Equ_BRA_drain_cathode_Var_Ptr;
    double * f_mucinv_Equ_BRA_mucinv_GND_Var_Ptr;
    double * f_BRA_mucinv_GND_Equ_mucinv_Node_Ptr;
    double * q_anode_Equ_anode_Node_Ptr;
    double * q_anode_Equ_emitter_Node_Ptr;
    double * q_emitter_Equ_anode_Node_Ptr;
    double * q_emitter_Equ_emitter_Node_Ptr;
    double * q_gate_Equ_BRA_gate_cathode_Var_Ptr;
    double * q_BRA_gate_cathode_Equ_gate_Node_Ptr;
    double * q_cathode_Equ_BRA_gate_cathode_Var_Ptr;
    double * q_BRA_gate_cathode_Equ_cathode_Node_Ptr;
    double * q_BRA_gate_cathode_Equ_BRA_gate_cathode_Var_Ptr;
    double * q_gate_Equ_gate_Node_Ptr;
    double * q_gate_Equ_cathode_Node_Ptr;
    double * q_cathode_Equ_gate_Node_Ptr;
    double * q_cathode_Equ_cathode_Node_Ptr;
    double * q_drain_Equ_dvdgdt_Node_Ptr;
    double * q_gate_Equ_dvdgdt_Node_Ptr;
    double * q_emitter_Equ_qceb_Node_Ptr;
    double * q_drain_Equ_qceb_Node_Ptr;
    double * q_dvdgdt_Equ_BRA_dvdgdt_emitter_Var_Ptr;
    double * q_emitter_Equ_BRA_dvdgdt_emitter_Var_Ptr;
    double * q_BRA_dvdgdt_emitter_Equ_drain_Node_Ptr;
    double * q_BRA_dvdgdt_emitter_Equ_gate_Node_Ptr;
    double * q_BRA_dvdgdt_emitter_Equ_dvdgdt_Node_Ptr;
    double * q_BRA_dvdgdt_emitter_Equ_emitter_Node_Ptr;
    double * q_dvbcdt_Equ_BRA_dvbcdt_GND_Var_Ptr;
    double * q_BRA_dvbcdt_GND_Equ_drain_Node_Ptr;
    double * q_BRA_dvbcdt_GND_Equ_cathode_Node_Ptr;
    double * q_BRA_dvbcdt_GND_Equ_emitter_Node_Ptr;
    double * q_BRA_dvbcdt_GND_Equ_dvbcdt_Node_Ptr;
    double * q_qceb_Equ_BRA_qceb_GND_Var_Ptr;
    double * q_BRA_qceb_GND_Equ_qceb_Node_Ptr;
    double * q_BRA_qceb_GND_Equ_emitter_Node_Ptr;
    double * q_BRA_qceb_GND_Equ_drain_Node_Ptr;
    double * q_nsat_Equ_BRA_nsat_mucinv_Var_Ptr;
    double * q_mucinv_Equ_BRA_nsat_mucinv_Var_Ptr;
    double * q_BRA_nsat_mucinv_Equ_nsat_Node_Ptr;
    double * q_BRA_nsat_mucinv_Equ_mucinv_Node_Ptr;
    double * q_drain_Equ_BRA_drain_cathode_Var_Ptr;
    double * q_cathode_Equ_BRA_drain_cathode_Var_Ptr;
    double * q_BRA_nsat_mucinv_Equ_BRA_drain_cathode_Var_Ptr;
    double * q_BRA_drain_cathode_Equ_BRA_drain_cathode_Var_Ptr;
    double * q_mucinv_Equ_BRA_mucinv_GND_Var_Ptr;
    double * q_BRA_mucinv_GND_Equ_mucinv_Node_Ptr;
    //Jacobian Offsets
    int m_anode_Equ_anode_NodeOffset;
    int m_anode_Equ_emitter_NodeOffset;
    int m_emitter_Equ_anode_NodeOffset;
    int m_emitter_Equ_emitter_NodeOffset;
    int m_gate_Equ_BRA_gate_cathode_VarOffset;
    int m_BRA_gate_cathode_Equ_gate_NodeOffset;
    int m_cathode_Equ_BRA_gate_cathode_VarOffset;
    int m_BRA_gate_cathode_Equ_cathode_NodeOffset;
    int m_BRA_gate_cathode_Equ_BRA_gate_cathode_VarOffset;
    int m_gate_Equ_gate_NodeOffset;
    int m_gate_Equ_cathode_NodeOffset;
    int m_cathode_Equ_gate_NodeOffset;
    int m_cathode_Equ_cathode_NodeOffset;
    int m_drain_Equ_dvdgdt_NodeOffset;
    int m_gate_Equ_dvdgdt_NodeOffset;
    int m_emitter_Equ_qceb_NodeOffset;
    int m_drain_Equ_qceb_NodeOffset;
    int m_dvdgdt_Equ_BRA_dvdgdt_emitter_VarOffset;
    int m_emitter_Equ_BRA_dvdgdt_emitter_VarOffset;
    int m_BRA_dvdgdt_emitter_Equ_drain_NodeOffset;
    int m_BRA_dvdgdt_emitter_Equ_gate_NodeOffset;
    int m_BRA_dvdgdt_emitter_Equ_dvdgdt_NodeOffset;
    int m_BRA_dvdgdt_emitter_Equ_emitter_NodeOffset;
    int m_dvbcdt_Equ_BRA_dvbcdt_GND_VarOffset;
    int m_BRA_dvbcdt_GND_Equ_drain_NodeOffset;
    int m_BRA_dvbcdt_GND_Equ_cathode_NodeOffset;
    int m_BRA_dvbcdt_GND_Equ_emitter_NodeOffset;
    int m_BRA_dvbcdt_GND_Equ_dvbcdt_NodeOffset;
    int m_qceb_Equ_BRA_qceb_GND_VarOffset;
    int m_BRA_qceb_GND_Equ_qceb_NodeOffset;
    int m_BRA_qceb_GND_Equ_emitter_NodeOffset;
    int m_BRA_qceb_GND_Equ_drain_NodeOffset;
    int m_nsat_Equ_BRA_nsat_mucinv_VarOffset;
    int m_mucinv_Equ_BRA_nsat_mucinv_VarOffset;
    int m_BRA_nsat_mucinv_Equ_nsat_NodeOffset;
    int m_BRA_nsat_mucinv_Equ_mucinv_NodeOffset;
    int m_drain_Equ_BRA_drain_cathode_VarOffset;
    int m_cathode_Equ_BRA_drain_cathode_VarOffset;
    int m_BRA_nsat_mucinv_Equ_BRA_drain_cathode_VarOffset;
    int m_BRA_drain_cathode_Equ_BRA_drain_cathode_VarOffset;
    int m_mucinv_Equ_BRA_mucinv_GND_VarOffset;
    int m_BRA_mucinv_GND_Equ_mucinv_NodeOffset;
    //Node Constants
    static const int cogendaNodeID_anode = 0;
    static const int cogendaNodeID_gate = 1;
    static const int cogendaNodeID_cathode = 2;
    static const int cogendaNodeID_nsat = 3;
    static const int cogendaNodeID_qceb = 4;
    static const int cogendaNodeID_mucinv = 5;
    static const int cogendaNodeID_drain = 6;
    static const int cogendaNodeID_emitter = 7;
    static const int cogendaNodeID_dvdgdt = 8;
    static const int cogendaNodeID_dvbcdt = 9;
    static const int cogendaNodeID_GND = -1;
    //Branch Constants
    static const int cogendaBRA_ID_gate_cathode = 10;
    static const int cogendaBRA_ID_dvdgdt_emitter = 11;
    static const int cogendaBRA_ID_dvbcdt_GND = 12;
    static const int cogendaBRA_ID_qceb_GND = 13;
    static const int cogendaBRA_ID_drain_cathode = 14;
    static const int cogendaBRA_ID_nsat_mucinv = 15;
    static const int cogendaBRA_ID_mucinv_GND = 16;
    //Probe Constants
    static const int cogendaProbeID_I_gate_cathode = 0;
    static const int cogendaProbeID_I_dvdgdt_emitter = 1;
    static const int cogendaProbeID_I_dvbcdt_GND = 2;
    static const int cogendaProbeID_I_qceb_GND = 3;
    static const int cogendaProbeID_I_drain_cathode = 4;
    static const int cogendaProbeID_I_nsat_mucinv = 5;
    static const int cogendaProbeID_I_mucinv_GND = 6;
    static const int cogendaProbeID_V_drain_gate = 7;
    static const int cogendaProbeID_V_gate_cathode = 8;
    static const int cogendaProbeID_V_drain_cathode = 9;
    static const int cogendaProbeID_V_anode_emitter = 10;
    static const int cogendaProbeID_V_emitter_drain = 11;
    static const int cogendaProbeID_V_dvdgdt_GND = 12;
    static const int cogendaProbeID_V_qceb_GND = 13;
    //Limited Probe Store LIDs
    //Collapsible Bools
    //FadArrays
 // Arrays to hold probes
 std::vector < CogendaFadType > probeVars;
 // Arrays to hold contributions
 // dynamic contributions are differentiated w.r.t time
 std::vector < CogendaFadType > staticContributions;
 std::vector < CogendaFadType > dynamicContributions;


    // this is what we'll use when any model uses $temperature.  We'll
    // set it in updateTemperature, and initialize it to whatever
    // is in devOptions when the instance is constructed.
    double cogendaTemperature;
    double cogenda_vt_nom;

    // This one is for the annoying bogus "XycecogendaInstTemp" parameter
    // that we need so we can set it from the device manager when there's no
    // "TEMP" parameter to use
    double cogendaInstTemp;
//   Model Parameters also as instance params
    double cgd;
    double cgs;
  // Local variables extend to global vars in Instance 
  CogendaFadType vdg;
  CogendaFadType vgs;
  CogendaFadType vds;
  CogendaFadType vae;
  CogendaFadType ved;
  CogendaFadType qcgs;
  double qcdsj;
  double imult;
  double imos;
  CogendaFadType icdg;
  double ibp;
  double ic;
  double rb;
  double vebj;
  double nsat1;
  double mucinv1;

    static JacobianStamp jacStamp;   
    static IdVector nodeMap;         
    static PairMap pairToJacStampMap;

    // These instance-owned vectors are for storage of lead current data
    std::vector<double> leadCurrentF;
    std::vector<double> leadCurrentQ;
};

class Model : public DeviceModel
{
    typedef std::vector<Instance *> InstanceVector;

    friend class ParametricData<Model>;
    friend class Instance;
    friend struct Traits;

  public:
    Model(
      const Configuration &       configuration,
      const ModelBlock &          model_block,
      const FactoryBlock &        factory_block);

    ~Model();

private:
    Model(const Model &);
    Model &operator=(const Model &);

public:
    virtual void forEachInstance(DeviceInstanceOp &op) const /* override */;
    virtual std::ostream &printOutInstances(std::ostream &os) const;
    bool processParams();
    bool processInstanceParams();

  private:

  public:
    void addInstance(Instance *instance)
    {
      instanceContainer.push_back(instance);
    }

  private:
    std::vector<Instance*> instanceContainer;

  private:
    double cogendaModTemp;
//   Model Parameters
    double cgd;
    double cgs;
};
namespace AnalogFunctions{
} //namespace AnalogFunctions
void registerDevice();
} // namespace COGENDAbranches
} // namespace Device
} // namespace Xyce
#endif //Xyce_N_DEV_branches_h
