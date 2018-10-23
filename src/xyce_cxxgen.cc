/*
 * Verilog-A compiling to C++ codes in Xyce format
 * whose input is from xyce_vacomp elaborator
 */
#include "xyce_vacomp.h"
#include "xyce_cxxgen.h"


void CgenIncludeFiles(string_t& devName, std::ofstream& h_outheader)
{
  h_outheader << "//-------------------------------------------------------------------------" <<std::endl;
  h_outheader << str_format("#ifndef Xyce_N_DEV_{}_h", devName) << std::endl;
  h_outheader << str_format("#define Xyce_N_DEV_{}_h", devName) << std::endl;
  h_outheader << std::endl;
  h_outheader << "#include <Sacado.hpp>" <<std::endl;
  h_outheader << std::endl;
  h_outheader <<"// ----------   Xyce Includes in header ----------" <<std::endl;
  h_outheader <<"#include <N_DEV_Configuration.h>" <<std::endl;
  h_outheader <<"#include <N_DEV_Const.h>" <<std::endl;
  h_outheader <<"#include <N_DEV_DeviceBlock.h>" <<std::endl;
  h_outheader <<"#include <N_DEV_DeviceInstance.h>" <<std::endl;
  h_outheader <<"#include <N_DEV_DeviceModel.h>" <<std::endl;
  h_outheader <<"#include <N_UTL_Math.h>" <<std::endl;
  h_outheader <<"// ---------- Macros Definitions ----------" <<std::endl;
  h_outheader <<"#define KOVERQ        8.61734e-05" <<std::endl;
  h_outheader <<"#define ELEM          1.0e+20" <<std::endl;
  h_outheader <<"#define _VT_ ((ckt->temperature) * KOVERQ)" <<std::endl;
  h_outheader <<"#define _TEMPER_ (ckt->temperature)" <<std::endl;
  h_outheader <<"#define _LIMEXP_(x) ((x)<log(ELIM)? exp(x) : (ELIM*(x) + ELIM - ELIM*log(ELIM)))" <<std::endl;
  h_outheader << std::endl;
}

void
CgenHeaderClassInstance(vaElement& vaModuleEntries, std::ofstream& h_outheader)
{
  int n_collapsible=0;
  int n_whitenoise=0;
  int n_flickernoise=0;
  int n_limitedProbes=0;
  h_outheader << "class Instance : public DeviceInstance"   <<std::endl;
  h_outheader << "{" <<std::endl;
  h_outheader << "  friend class ParametricData<Instance>;" <<std::endl;
  h_outheader << "  friend class Model;"   <<std::endl;
  h_outheader << "  friend struct Traits;" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "  public:"     <<std::endl;
  h_outheader << "    Instance(" <<std::endl;
  h_outheader << "      const Configuration &       configuration,"  <<std::endl;
  h_outheader << "      const InstanceBlock &       instance_block," <<std::endl;
  h_outheader << "      Model &                     model,"          <<std::endl;
  h_outheader << "      const FactoryBlock &        factory_block);" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    ~Instance();" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "private:" <<std::endl;
  h_outheader << "    Instance(const Instance &);"            <<std::endl;
  h_outheader << "    Instance &operator=(const Instance &);" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "public:" <<std::endl;
  h_outheader << "    void registerLIDs( const LocalIdVector & intLIDVecRef," <<std::endl;
  h_outheader << "                       const LocalIdVector & extLIDVecRef );" <<std::endl;
  h_outheader << "    void registerStoreLIDs( const LocalIdVector & stoLIDVecRef );" <<std::endl;
  h_outheader << "    void setupPointers();" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    void loadNodeSymbols(Util::SymbolTable &symbol_table) const;" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    const JacobianStamp & jacobianStamp() const;" <<std::endl;
  h_outheader << "    void registerJacLIDs( const JacobianStamp & jacLIDVec );" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    void registerBranchDataLIDs(const std::vector<int> & branchLIDVecRef);" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    bool processParams();" <<std::endl;
  h_outheader << "    bool updateTemperature ( const double & temp = -999.0 );" <<std::endl;
  h_outheader << "    bool updateIntermediateVars ();"  <<std::endl;
  h_outheader << "    bool updatePrimaryState ();"      <<std::endl;
  h_outheader << "    bool updateSecondaryState ();"    <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    // load functions, residual:"     <<std::endl;
  h_outheader << "    bool loadDAEQVector ();"          <<std::endl;
  h_outheader << "    bool loadDAEFVector ();"          <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);
  h_outheader << "    // load functions, Jacobian:"     <<std::endl;
  h_outheader << "    bool loadDAEdQdx ();"             <<std::endl;
  h_outheader << "    bool loadDAEdFdx ();"             <<std::endl;
  
  if(n_collapsible > 0)
  {
    h_outheader << "    void collapseNodes();" <<std::endl;
  }

  INSERT_EMPTY_LINE(h_outheader);
  if(n_whitenoise >0 || n_flickernoise >0)
  {
    h_outheader << "    int getNumNoiseSources () const;" <<std::endl;
    h_outheader << "    void setupNoiseSources (Xyce::Analysis::NoiseData & noiseData);" <<std::endl;
    h_outheader << "    void getNoiseSources (Xyce::Analysis::NoiseData & noiseData);" <<std::endl;
  }
  INSERT_EMPTY_LINE(h_outheader);
    
  h_outheader << "  private:" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

  h_outheader << "  public:"  <<std::endl;
  h_outheader << "    Model &getModel() {  return model_; }" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

  h_outheader << "  private:" <<std::endl;
  h_outheader << "    Model & model_;   //< Owning Model" <<std::endl;
  //xyceDeclareInstanceVariables  
  //No needs this item
  //xyceDeclareNodeLIDVariables
  h_outheader << "    //Node LID Variables\n";
  for(auto it=vaModuleEntries.m_moduleNets.begin(); it != vaModuleEntries.m_moduleNets.end(); ++it)
  {
    h_outheader << str_format("    int li_{};", *it) <<std::endl;
  }
  //xyceDeclareBranchLIDVariables
  h_outheader << "    //Branch LID Variables\n";
  for(auto it=vaModuleEntries.m_branches.begin(); it != vaModuleEntries.m_branches.end(); ++it)
  {
    h_outheader << str_format("    int li_BRA_{}_{};", it->second[0], it->second[1]) <<std::endl;
  }
  //xyceDeclareLeadBranchLIDVariables
  h_outheader << "    //Lead Branch Variables\n";
  for(auto it=vaModuleEntries.m_modulePorts.begin(); it != vaModuleEntries.m_modulePorts.end(); ++it)
  {
    h_outheader << str_format("    int li_branch_i{};", *it) <<std::endl;
  }
  //xyceDeclareJacobianOffsets
  strVec fNodePtrs, qNodePtrs, mNodeOffsets;
  strPairVec _recodNodeMtrix;

  for(auto it=vaModuleEntries.m_contribs.begin(); 
    it != vaModuleEntries.m_contribs.end(); ++it)
  {
    for(auto node_row=it->nodes.begin(); node_row != it->nodes.end(); ++node_row)
      for(auto node_col=it->depend_nodes.begin(); node_col != it->depend_nodes.end(); ++node_col)
      {
        strPair nodePair = {*node_row, *node_col};
        if(item_exists(_recodNodeMtrix, nodePair))
          continue;
        else
          _recodNodeMtrix.push_back(nodePair);
        //Jacobian  pointers:  double * f|q_bi_Equ_ti_Node_Ptr;
        fNodePtrs.push_back(str_format("    double * f_{}_Equ_{}_Node_Ptr;", *node_row, *node_col));
        qNodePtrs.push_back(str_format("    double * q_{}_Equ_{}_Node_Ptr;", *node_row, *node_col));
        //Jacobian offsets:    int m_bi_Equ_ti_NodeOffset;
        mNodeOffsets.push_back(str_format("    int m_{}_Equ_{}_NodeOffset;", *node_row, *node_col));
      }
  }
  h_outheader << "    //Jacobian pointers\n";
  for(auto it=fNodePtrs.begin(); it != fNodePtrs.end(); ++it)
    h_outheader << *it <<std::endl;
  for(auto it=qNodePtrs.begin(); it != qNodePtrs.end(); ++it)
    h_outheader << *it <<std::endl;
  h_outheader << "    //Jacobian Offsets\n";
  for(auto it=mNodeOffsets.begin(); it != mNodeOffsets.end(); ++it)
    h_outheader << *it <<std::endl;
  
  //xyceDeclareNodeConstants
  h_outheader << "    //Node Constants\n";
  int _idx=0;
  for(auto it=vaModuleEntries.m_moduleNets.begin(); 
      it != vaModuleEntries.m_moduleNets.end(); ++it)
  {
    h_outheader << str_format("    static const int cogendaNodeID_{} = {};", *it, _idx) <<std::endl;
    _idx += 1;
  }
  h_outheader << "    static const int cogendaNodeID_GND = -1;" <<std::endl;
  //xyceDeclareBranchConstants
  h_outheader << "    //Branch Constants\n";
  for(auto it=vaModuleEntries.m_branches.begin(); it != vaModuleEntries.m_branches.end(); ++it)
  {
    h_outheader << str_format("    static const int cogendaBRA_ID_{}_{};", it->second[0], it->second[1]) <<std::endl;
  }

  //xyceDeclareProbeConstants
  //admsProbeID_%(nature)_%(branch/pnode)_%(branch/nnode)"
  //admsProbeID_I_%(pnode)_%(nnode)"
  h_outheader << "    //Probe Constants\n";
  _idx=0;
  {
    strVec _strVecTmp;
    string_t _strItem;
    for(auto it=vaModuleEntries.m_probeConstants.begin(); 
      it != vaModuleEntries.m_probeConstants.end(); ++it)
    {
      //out code: static const int admsProbeID_V_t_ti = 0;
      string_t etype = it->first;
      for(auto itv=it->second.begin(); itv != it->second.end(); ++itv)
      {
        string_t sn_poi=itv->first, sn_neg=itv->second;
        _strItem = str_format("    static const int cogendaProbeID_{}_{}_{}", etype, sn_poi, sn_neg, _idx);
        if(item_exists(_strVecTmp, _strItem))
          continue;
        else
          _strVecTmp.push_back(_strItem);
        h_outheader << str_format("{} = {};", _strItem, _idx) <<std::endl;
        _idx += 1;
      }
    }
  }
  //xyceDeclareLimitedProbeStoreLIDs
  h_outheader << "    //Limited Probe Store LIDs\n";
  //TODO
  //declareCollapsibleBools
  h_outheader << "    //Collapsible Bools\n";
  //TODO
  //xyceDeclareFadArrays
  h_outheader << "    //FadArrays\n";
  h_outheader << " // Arrays to hold probes\n";
  h_outheader << " std::vector < CogendaFadType > probeVars;" <<std::endl;
  h_outheader << " // Arrays to hold contributions\n";
  h_outheader << " // dynamic contributions are differentiated w.r.t time\n";
  h_outheader << " std::vector < CogendaFadType > staticContributions;" <<std::endl;
  h_outheader << " std::vector < CogendaFadType > dynamicContributions;" <<std::endl;
  
  if(n_whitenoise >0 || n_flickernoise >0)
  {
    h_outheader << "    //vectors to hold noise information for communication between methods\n";
    h_outheader << "    std::vector<double> noiseContribsPower;" <<std::endl;
    h_outheader << "    std::vector<double> noiseContribsExponent;" <<std::endl;
  }
  INSERT_EMPTY_LINE(h_outheader);
    
  if(n_limitedProbes >0)
  {
    h_outheader << "    // This array stores the differences between original and limited variables.\n";
    h_outheader << "    std::vector<double> probeDiffs;" <<std::endl;
    h_outheader << "    // These store the Jdxp's for F and Q, respectively\n";
    h_outheader << "    std::vector<double> Jdxp_static;" <<std::endl;
    h_outheader << "    std::vector<double> Jdxp_dynamic;" <<std::endl;
  }
  INSERT_EMPTY_LINE(h_outheader);

  h_outheader << "    // this is what we'll use when any model uses $temperature.  We'll\n";
  h_outheader << "    // set it in updateTemperature, and initialize it to whatever\n";
  h_outheader << "    // is in devOptions when the instance is constructed.\n";
  h_outheader << "    double cogendaTemperature;" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

    // vt at \$temperature;
    //double adms_vt_nom;

  h_outheader << "    // This one is for the annoying bogus \"XyceADMSInstTemp\" parameter\n"; //TODO
  h_outheader << "    // that we need so we can set it from the device manager when there's no\n";
  h_outheader << "    // \"TEMP\" parameter to use\n";
  h_outheader << "    double cogendaInstTemp;" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

  if(n_collapsible == 0)
  {
    h_outheader << "    static JacobianStamp jacStamp;   " <<std::endl;
    h_outheader << "    static IdVector nodeMap;         " <<std::endl;
    h_outheader << "    static PairMap pairToJacStampMap;" <<std::endl;
  }
  else
  {
    h_outheader << "    JacobianStamp jacStamp;   " <<std::endl;
    h_outheader << "    IdVector nodeMap;         " <<std::endl;
    h_outheader << "    PairMap pairToJacStampMap;" <<std::endl;
  }
  INSERT_EMPTY_LINE(h_outheader);

  h_outheader << "    // These instance-owned vectors are for storage of lead current data\n";
  h_outheader << "    std::vector<double> leadCurrentF;" <<std::endl;
  h_outheader << "    std::vector<double> leadCurrentQ;" <<std::endl;
  h_outheader << "    };" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

}

void
CgenHeaderClassModel(vaElement& vaModuleEntries, std::ofstream& h_outheader)
{
  h_outheader << "class Model : public DeviceModel" <<std::endl;
  h_outheader << "{" <<std::endl;
  h_outheader << "    typedef std::vector<Instance *> InstanceVector;" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "    friend class ParametricData<Model>;" <<std::endl;
  h_outheader << "    friend class Instance;" <<std::endl;
  h_outheader << "    friend struct Traits;" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "  public:" <<std::endl;
  h_outheader << "    Model(" <<std::endl;
  h_outheader << "      const Configuration &       configuration," <<std::endl;
  h_outheader << "      const ModelBlock &          model_block," <<std::endl;
  h_outheader << "      const FactoryBlock &        factory_block);" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "    ~Model();" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "private:" <<std::endl;
  h_outheader << "    Model(const Model &);" <<std::endl;
  h_outheader << "    Model &operator=(const Model &);" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "public:" <<std::endl;
  h_outheader << "    virtual void forEachInstance(DeviceInstanceOp &op) const /* override */;" <<std::endl;
  h_outheader << "    virtual std::ostream &printOutInstances(std::ostream &os) const;" <<std::endl;
  h_outheader << "    bool processParams();" <<std::endl;
  h_outheader << "    bool processInstanceParams();" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "  private:" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "  public:" <<std::endl;
  h_outheader << "    void addInstance(Instance *instance)" <<std::endl;
  h_outheader << "    {" <<std::endl;
  h_outheader << "      instanceContainer.push_back(instance);" <<std::endl;
  h_outheader << "    }" <<std::endl;
  INSERT_EMPTY_LINE(h_outheader);

  h_outheader << "  private:" <<std::endl;
  h_outheader << "    std::vector<Instance*> instanceContainer;" <<std::endl;
  h_outheader << "" <<std::endl;
  h_outheader << "  private:" <<std::endl;

  //TODO: how to handle XyceADMSInstTemp gracefully?
    // This one is for the annoying bogus "XyceADMSInstTemp" parameter
    // that we need so we can set it from the device manager when there's no
    // "TEMP" model parameter to use
  h_outheader << "    double cogendaModTemp;" <<std::endl;
// Begin verilog Model Variables
  h_outheader << "//   Model Parameters" <<std::endl;
  for(auto it=vaModuleEntries.m_params.begin(); it != vaModuleEntries.m_params.end(); ++it)
  {
    h_outheader << str_format("    {} {};", it->second.val_type, it->first) <<std::endl;
  }
  h_outheader << "};" <<std::endl;
}

returnFlag
CgenHeader (vaElement& vaModuleEntries, string_t& fheaderName)
{
  std::ofstream h_outheader;
  h_outheader.open(fheaderName.c_str(),std::ofstream::out);
  if(!h_outheader)
  {
    std::cout << str_format("file {}: cannot open to write!\n",fheaderName);
    return Ret_FATAL;
  }
    
  string_t moduleName = vaModuleEntries.m_moduleName;
  int numberProbes = vaModuleEntries.m_moduleNets.size();
  int numNodes = vaModuleEntries.m_modulePorts.size();

  CgenIncludeFiles(moduleName, h_outheader);
  //main header content goes here
  h_outheader << "namespace Xyce {" << std::endl;
  h_outheader << "namespace Device {" << std::endl;
  h_outheader << str_format("namespace COGENDA_{} {", moduleName) << std::endl;
  h_outheader << "// This typedef is for our automatic differentiation:" << std::endl;
  h_outheader << str_format("typedef Sacado::Fad::SFad<double,{}> CogendaFadType;", numberProbes) << std::endl;
  h_outheader << std::endl;
  h_outheader << "class Model;" << std::endl;
  h_outheader << "class Instance;" << std::endl;
  h_outheader << std::endl;

  //struct DeviceTraits
  h_outheader << "struct Traits: public DeviceTraits<Model, Instance>" << std::endl;
  h_outheader << "{"<< std::endl;
  h_outheader << str_format("  static const char *name() {return \"COGENDA {}\";}",moduleName)<< std::endl;
  h_outheader << str_format("  static const char *deviceTypeName() {return \"{} level 1\";}",moduleName)<< std::endl;
  h_outheader << std::endl;
  h_outheader << str_format("  static int numNodes() {return {};}",numNodes)<< std::endl;
  h_outheader << "  static bool modelRequired() {return true;}"<< std::endl;
  h_outheader << "  static bool isLinearDevice() {return false;}"<< std::endl;
  h_outheader << std::endl;
  h_outheader << "  static Device *factory(const Configuration &configuration, const FactoryBlock &factory_block);" << std::endl;
  h_outheader << "  static void loadModelParameters(ParametricData<Model> &model_parameters);"<< std::endl;
  h_outheader << "  static void loadInstanceParameters(ParametricData<Instance> &instance_parameters);"<< std::endl;
  h_outheader << "};"<< std::endl;
  //class Instance 
  CgenHeaderClassInstance(vaModuleEntries,h_outheader);
  //class Model
  CgenHeaderClassModel(vaModuleEntries,h_outheader);
  h_outheader << "void registerDevice();"<< std::endl;
  h_outheader << str_format("} // namespace COGENDA_{}", moduleName) << std::endl;
  h_outheader << "} // namespace Device"<< std::endl;
  h_outheader << "} // namespace Xyce"<< std::endl;
  h_outheader << str_format("#endif //Xyce_N_DEV_{}_h", moduleName) << std::endl;
  return Ret_NORMAL;
}

void CgenIncludeFilesCxx(string_t& devName, std::ofstream& h_outCxx)
{
  h_outCxx << "//-------------------------------------------------------------------------" <<std::endl;
  h_outCxx <<"// ----------   Xyce Includes in Cxx ----------" <<std::endl;
  h_outCxx <<"#include <Xyce_config.h>" <<std::endl;
  h_outCxx <<str_format("#include \"N_DEV_COGENDA_{}.h\"", devName) <<std::endl;
  INSERT_EMPTY_LINE(h_outCxx);
  h_outCxx <<"#include <N_DEV_Const.h>" <<std::endl;
  h_outCxx <<"#include <N_DEV_DeviceOptions.h>" <<std::endl;
  h_outCxx <<"#include <N_DEV_DeviceMaster.h>" <<std::endl;
  h_outCxx <<"#include <N_DEV_ExternData.h>" <<std::endl;
  h_outCxx <<"#include <N_DEV_SolverState.h>" <<std::endl;
  h_outCxx <<"#include <N_DEV_Message.h>" <<std::endl;
  INSERT_EMPTY_LINE(h_outCxx);
  h_outCxx <<"#include <N_LAS_Matrix.h>" <<std::endl;
  h_outCxx <<"#include <N_LAS_Vector.h>" <<std::endl;
  INSERT_EMPTY_LINE(h_outCxx);
  h_outCxx <<"  #include <N_ANP_NoiseData.h>" <<std::endl;

  h_outCxx <<"#include <N_UTL_FeatureTest.h>" <<std::endl;
  h_outCxx <<"#if defined(HAVE_UNORDERED_MAP)" <<std::endl;
  h_outCxx <<"#include <unordered_map>" <<std::endl;
  h_outCxx <<"using std::unordered_map;" <<std::endl;
  h_outCxx <<"#elif defined(HAVE_TR1_UNORDERED_MAP)" <<std::endl;
  h_outCxx <<"#include <tr1/unordered_map>" <<std::endl;
  h_outCxx <<"using std::tr1::unordered_map;" <<std::endl;
  h_outCxx <<"#else" <<std::endl;
  h_outCxx <<"#error neither unordered_map or tr1/unordered_map found!" <<std::endl;
  h_outCxx <<"#endif" <<std::endl;
  INSERT_EMPTY_LINE(h_outCxx);
}

void genInstMemberFunc(vaElement& vaModuleEntries, std::ofstream& h_outCxx)
{
  h_outCxx <<"/* class Instance member functions */\n";
  h_outCxx << "  //bool Instance::processParams()\n";
  h_outCxx << "  //Instance::Instance(){...}\n";
  h_outCxx << "  //Instance::~Instance()\n";
  h_outCxx << "  //void Instance::registerLIDs\n";
  h_outCxx << "  //void Instance::loadNodeSymbols\n";
  h_outCxx << "  //void Instance::registerStoreLIDs\n";
  h_outCxx << "  //void Instance::registerBranchDataLIDs\n";
  h_outCxx << "  //const JacobianStamp & Instance::jacobianStamp() const\n";
  h_outCxx << "  //void Instance::registerJacLIDs\n";
  h_outCxx << "  //void Instance::setupPointers\n";
  h_outCxx << "  //bool Instance::loadDAEFVector\n";
  h_outCxx << "  //bool Instance::loadDAEQVector\n";
  h_outCxx << "  //bool Instance::updatePrimaryState()\n";
  h_outCxx << "  //bool Instance::updateSecondaryState()\n";
  h_outCxx << "  //bool Instance::updateIntermediateVars\n";
  h_outCxx << "  //int Instance::getNumNoiseSources\n";
  h_outCxx << "  //void Instance::setupNoiseSources\n";
  h_outCxx << "  //void Instance::getNoiseSources\n";
  h_outCxx << "  //bool Instance::loadDAEdFdx\n";
  h_outCxx << "  //bool Instance::loadDAEdQdx\n";
  h_outCxx << "  //bool Instance::updateTemperature\n";
  //
}

void 
genModelMemberFunc(vaElement& vaModuleEntries, std::ofstream& h_outCxx)
{
  h_outCxx <<"/* class Model member functions */\n";
  h_outCxx << "  //bool Model::processParams\n";
  h_outCxx << "  //bool Model::processInstanceParams\n";
  h_outCxx << "  //Model::Model(..){...}\n";
  h_outCxx << "  //Model::~Model\n";
  h_outCxx << "  //std::ostream &Model::printOutInstances\n";
  h_outCxx << "  //void Model::forEachInstance()\n";
}

void 
genDeviceTraits(vaElement& vaModuleEntries, std::ofstream& h_outCxx)
{
  string_t moduleName = vaModuleEntries.m_moduleName;
  h_outCxx <<"/* class Traits member functions */\n";
  h_outCxx <<"  //void Traits::loadInstanceParameters() \n";
  h_outCxx <<str_format("void Traits::loadInstanceParameters(ParametricData<COGENDA_{}::Instance> &p)",moduleName)<<std::endl;
  h_outCxx <<"{\n";
  h_outCxx <<"  // This kludge is to force us always to have an instance parameter\n";
  h_outCxx <<"  // that the device manager can set to the temperature, even if we have\n";
  h_outCxx <<"  // no \"TEMP\".\n";
  h_outCxx <<str_format("  p.addPar(\"XYCE_COGENDA_INST_TEMP\", 0.0, &COGENDA_{}::Instance::cogendaInstTemp)",moduleName)<<std::endl;
  h_outCxx <<"    .setExpressionAccess(NO_DOC)\n";
  h_outCxx <<"    .setUnit(U_DEGK)\n";
  h_outCxx <<"    .setCategory(CAT_TEMP)\n";
  h_outCxx <<"    .setDescription(\"Internally set parameter for device instance temperature\");\n";
  //set all normal VA parameters as instance paramsters
  for(auto it=vaModuleEntries.m_params.begin(); it != vaModuleEntries.m_params.end(); ++it)
  {
    h_outCxx << str_format("  p.addPar(\"{}\",static_cast<{}>({}), &COGENDA_{}::Instance:{});", it->first, it->second.val_type, it->second.init_value, moduleName, it->first) <<std::endl;
  }
  h_outCxx <<"}\n\n";
  
  h_outCxx <<"  //void Traits::loadModelParameters()\n";
  h_outCxx <<str_format("void Traits::loadModelParameters(ParametricData<COGENDA_{}::Model> &p)",moduleName)<<std::endl;
  h_outCxx <<"{\n";
  h_outCxx <<str_format("  p.addPar(\"XYCE_COGENDA_MOD_TEMP\", 0.0, &COGENDA_{}::Model::cogendaModTemp)",moduleName)<<std::endl;
  h_outCxx <<"    .setExpressionAccess(NO_DOC)\n";
  h_outCxx <<"    .setUnit(U_DEGK)\n";
  h_outCxx <<"    .setCategory(CAT_TEMP)\n";
  h_outCxx <<"    .setDescription(\"Internally set parameter for device model temperature\");\n";
  //set all normal VA parameters as model paramsters
  for(auto it=vaModuleEntries.m_params.begin(); it != vaModuleEntries.m_params.end(); ++it)
  {
    h_outCxx << str_format("  p.addPar(\"{}\",static_cast<{}>({}), &COGENDA_{}::Model:{});", it->first, it->second.val_type, it->second.init_value, moduleName, it->first) <<std::endl;
  }
  h_outCxx <<"}\n\n";
  
  h_outCxx <<"  //Device *Traits::factory()\n";
  h_outCxx << "Device *Traits::factory(const Configuration &configuration, const FactoryBlock &factory_block)" <<std::endl;
  h_outCxx << "  {" <<std::endl;
  h_outCxx << "    return new DeviceMaster<Traits>(configuration, factory_block, factory_block.solverState_, factory_block.deviceOptions_);" <<std::endl;
  h_outCxx << "}" <<std::endl;

  h_outCxx << "void registerDevice()" <<std::endl;
  h_outCxx << "{" <<std::endl;
  h_outCxx << "  Config<Traits>::addConfiguration()" <<std::endl; 
  h_outCxx << str_format("    .registerDevice(\"{}\", 1)", moduleName) <<std::endl; 
  h_outCxx << str_format("    .registerModelType(\"{}\", 1);", moduleName) <<std::endl;
  h_outCxx << "}" <<std::endl;
}

returnFlag
CgenImplement (vaElement& vaModuleEntries, string_t& fCxxName)
{
  std::ofstream h_outCxx;
  h_outCxx.open(fCxxName.c_str(),std::ofstream::out);
  string_t moduleName = vaModuleEntries.m_moduleName;
  CgenIncludeFilesCxx(moduleName, h_outCxx);
  if(!h_outCxx)
  {
    std::cout << str_format("file {}: cannot open to write!\n",fCxxName);
    return Ret_FATAL;
  }  
  h_outCxx << "//The C++ Main part of Model Implementation Starts here" << std::endl;
  h_outCxx <<"namespace Xyce {" << std::endl;
  h_outCxx <<"namespace Device {" << std::endl;
  h_outCxx << str_format("namespace COGENDA_{} {", moduleName) << std::endl;
  h_outCxx <<"JacobianStamp Instance::jacStamp;" << std::endl;
  h_outCxx <<"IdVector Instance::nodeMap;" << std::endl;
  h_outCxx <<"PairMap Instance::pairToJacStampMap;" << std::endl;
  INSERT_EMPTY_LINE(h_outCxx);

  for(auto it=vaModuleEntries.m_moduleNets.begin(); 
      it != vaModuleEntries.m_moduleNets.end(); ++it)
  {
    h_outCxx << str_format("    const int Instance::cogendaNodeID_{};", *it) <<std::endl;
  }
  h_outCxx << "    const int Instance::cogendaNodeID_GND;" <<std::endl;  
  INSERT_EMPTY_LINE(h_outCxx);
  // Additional IDs for branch equations
  //TODO?
  genDeviceTraits(vaModuleEntries, h_outCxx);
  genInstMemberFunc(vaModuleEntries, h_outCxx);
  genModelMemberFunc(vaModuleEntries, h_outCxx);
 
  h_outCxx << str_format("} // namespace COGENDA_{}\n",moduleName);
  h_outCxx << "} // namespace Device\n";
  h_outCxx << "} // namespace Xyce\n";
  return Ret_NORMAL;
}

