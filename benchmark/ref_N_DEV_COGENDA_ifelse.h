//-------------------------------------------------------------------------
#ifndef Xyce_N_DEV_ifelse_h
#define Xyce_N_DEV_ifelse_h

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
namespace COGENDAifelse {
// This typedef is for our automatic differentiation:
typedef Sacado::Fad::SFad<double,4> CogendaFadType;

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
  static const char *name() {return "COGENDA ifelse";}
  static const char *deviceTypeName() {return "ifelse level 1";}

  static int numNodes() {return 2;}
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
    int li_a;
    int li_c;
    int li_b;
    //Branch LID Variables
    int li_BRA_a_c;
    int li_BRA_a_b;
    int li_BRA_a_GND;
    //Lead Branch Variables
    int li_branch_ia;
    //Jacobian pointers
    double * f_a_Equ_BRA_a_b_Var_Ptr;
    double * f_b_Equ_BRA_a_b_Var_Ptr;
    double * f_BRA_a_b_Equ_a_Node_Ptr;
    double * f_BRA_a_b_Equ_b_Node_Ptr;
    double * f_a_Equ_BRA_a_c_Var_Ptr;
    double * f_c_Equ_BRA_a_c_Var_Ptr;
    double * f_BRA_a_b_Equ_BRA_a_c_Var_Ptr;
    double * f_BRA_a_c_Equ_BRA_a_c_Var_Ptr;
    double * f_BRA_a_b_Equ_BRA_a_b_Var_Ptr;
    double * f_BRA_a_c_Equ_a_Node_Ptr;
    double * f_BRA_a_c_Equ_c_Node_Ptr;
    double * f_a_Equ_BRA_a_GND_Var_Ptr;
    double * f_BRA_a_GND_Equ_a_Node_Ptr;
    double * f_a_Equ_a_Node_Ptr;
    double * f_a_Equ_c_Node_Ptr;
    double * f_c_Equ_a_Node_Ptr;
    double * f_c_Equ_c_Node_Ptr;
    double * q_a_Equ_BRA_a_b_Var_Ptr;
    double * q_b_Equ_BRA_a_b_Var_Ptr;
    double * q_BRA_a_b_Equ_a_Node_Ptr;
    double * q_BRA_a_b_Equ_b_Node_Ptr;
    double * q_a_Equ_BRA_a_c_Var_Ptr;
    double * q_c_Equ_BRA_a_c_Var_Ptr;
    double * q_BRA_a_b_Equ_BRA_a_c_Var_Ptr;
    double * q_BRA_a_c_Equ_BRA_a_c_Var_Ptr;
    double * q_BRA_a_b_Equ_BRA_a_b_Var_Ptr;
    double * q_BRA_a_c_Equ_a_Node_Ptr;
    double * q_BRA_a_c_Equ_c_Node_Ptr;
    double * q_a_Equ_BRA_a_GND_Var_Ptr;
    double * q_BRA_a_GND_Equ_a_Node_Ptr;
    double * q_a_Equ_a_Node_Ptr;
    double * q_a_Equ_c_Node_Ptr;
    double * q_c_Equ_a_Node_Ptr;
    double * q_c_Equ_c_Node_Ptr;
    //Jacobian Offsets
    int m_a_Equ_BRA_a_b_VarOffset;
    int m_b_Equ_BRA_a_b_VarOffset;
    int m_BRA_a_b_Equ_a_NodeOffset;
    int m_BRA_a_b_Equ_b_NodeOffset;
    int m_a_Equ_BRA_a_c_VarOffset;
    int m_c_Equ_BRA_a_c_VarOffset;
    int m_BRA_a_b_Equ_BRA_a_c_VarOffset;
    int m_BRA_a_c_Equ_BRA_a_c_VarOffset;
    int m_BRA_a_b_Equ_BRA_a_b_VarOffset;
    int m_BRA_a_c_Equ_a_NodeOffset;
    int m_BRA_a_c_Equ_c_NodeOffset;
    int m_a_Equ_BRA_a_GND_VarOffset;
    int m_BRA_a_GND_Equ_a_NodeOffset;
    int m_a_Equ_a_NodeOffset;
    int m_a_Equ_c_NodeOffset;
    int m_c_Equ_a_NodeOffset;
    int m_c_Equ_c_NodeOffset;
    //Node Constants
    static const int cogendaNodeID_a = 0;
    static const int cogendaNodeID_c = 1;
    static const int cogendaNodeID_b = 2;
    static const int cogendaNodeID_GND = -1;
    //Branch Constants
    static const int cogendaBRA_ID_a_c = 3;
    static const int cogendaBRA_ID_a_b = 4;
    static const int cogendaBRA_ID_a_GND = 5;
    //Probe Constants
    static const int cogendaProbeID_I_a_c = 0;
    static const int cogendaProbeID_I_a_b = 1;
    static const int cogendaProbeID_I_a_GND = 2;
    static const int cogendaProbeID_V_a_c = 3;
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
    double CJO;
    double IS;
  // Local variables extend to global vars in Instance 
  double vdio;
  double idio;
  double qdio;
  double vnew;
  double r1;
  double eww;
  double vold;
  double vtemp;
  double arg;
  double vcrit;

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
    double CJO;
    double IS;
};
namespace AnalogFunctions{
} //namespace AnalogFunctions
void registerDevice();
} // namespace COGENDAifelse
} // namespace Device
} // namespace Xyce
#endif //Xyce_N_DEV_ifelse_h
