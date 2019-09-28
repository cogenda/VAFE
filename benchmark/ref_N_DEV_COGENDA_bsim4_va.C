//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "bsim4_va.h"

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
namespace COGENDAbsim4_va {

    const int Instance::cogendaNodeID_d;
    const int Instance::cogendaNodeID_g;
    const int Instance::cogendaNodeID_s;
    const int Instance::cogendaNodeID_b;
    const int Instance::cogendaNodeID_di;
    const int Instance::cogendaNodeID_si;
    const int Instance::cogendaNodeID_gi;
    const int Instance::cogendaNodeID_gm;
    const int Instance::cogendaNodeID_bi;
    const int Instance::cogendaNodeID_sbulk;
    const int Instance::cogendaNodeID_dbulk;
    const int Instance::cogendaNodeID_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAbsim4_va::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAbsim4_va::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("_CKT_GMIN",static_cast<double>(1e-12), &COGENDAbsim4_va::Instance::_ckt_gmin);
  p.addPar("_MIN",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::_min);
  p.addPar("A0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::a0);
  p.addPar("A1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::a1);
  p.addPar("A2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::a2);
  p.addPar("ACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::acde);
  p.addPar("ACNQSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::acnqsmod);
  p.addPar("AD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ad);
  p.addPar("ADOS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ados);
  p.addPar("AF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::af);
  p.addPar("AGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::agidl);
  p.addPar("AGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::agisl);
  p.addPar("AGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ags);
  p.addPar("AIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigbacc);
  p.addPar("AIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigbinv);
  p.addPar("AIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigc);
  p.addPar("AIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigd);
  p.addPar("AIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigs);
  p.addPar("AIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::aigsd);
  p.addPar("ALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::alpha0);
  p.addPar("ALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::alpha1);
  p.addPar("AS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::as);
  p.addPar("AT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::at);
  p.addPar("B0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::b0);
  p.addPar("B1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::b1);
  p.addPar("BDOS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bdos);
  p.addPar("BETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::beta0);
  p.addPar("BG0SUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bg0sub);
  p.addPar("BGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bgidl);
  p.addPar("BGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bgisl);
  p.addPar("BIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigbacc);
  p.addPar("BIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigbinv);
  p.addPar("BIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigc);
  p.addPar("BIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigd);
  p.addPar("BIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigs);
  p.addPar("BIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bigsd);
  p.addPar("BINUNIT",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::binunit);
  p.addPar("BVD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bvd);
  p.addPar("BVS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::bvs);
  p.addPar("CAPMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::capmod);
  p.addPar("CDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cdsc);
  p.addPar("CDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cdscb);
  p.addPar("CDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cdscd);
  p.addPar("CF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cf);
  p.addPar("CGBO",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgbo);
  p.addPar("CGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgdl);
  p.addPar("CGDO",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgdo);
  p.addPar("CGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgidl);
  p.addPar("CGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgisl);
  p.addPar("CGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgsl);
  p.addPar("CGSO",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cgso);
  p.addPar("CIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigbacc);
  p.addPar("CIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigbinv);
  p.addPar("CIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigc);
  p.addPar("CIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigd);
  p.addPar("CIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigs);
  p.addPar("CIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cigsd);
  p.addPar("CIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cit);
  p.addPar("CJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjd);
  p.addPar("CJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjs);
  p.addPar("CJSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjswd);
  p.addPar("CJSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjswgd);
  p.addPar("CJSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjswgs);
  p.addPar("CJSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cjsws);
  p.addPar("CKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ckappad);
  p.addPar("CKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ckappas);
  p.addPar("CLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::clc);
  p.addPar("CLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::cle);
  p.addPar("CVCHARGEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::cvchargemod);
  p.addPar("DELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::delta);
  p.addPar("DELVTO",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::delvto);
  p.addPar("DIOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::diomod);
  p.addPar("DLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dlc);
  p.addPar("DLCIG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dlcig);
  p.addPar("DLCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dlcigd);
  p.addPar("DMCG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dmcg);
  p.addPar("DMCGT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dmcgt);
  p.addPar("DMCI",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dmci);
  p.addPar("DMDG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dmdg);
  p.addPar("DROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::drout);
  p.addPar("DSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dsub);
  p.addPar("DTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dtox);
  p.addPar("DVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt0);
  p.addPar("DVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt0w);
  p.addPar("DVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt1);
  p.addPar("DVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt1w);
  p.addPar("DVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt2);
  p.addPar("DVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvt2w);
  p.addPar("DVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp0);
  p.addPar("DVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp1);
  p.addPar("DVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp2);
  p.addPar("DVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp3);
  p.addPar("DVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp4);
  p.addPar("DVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dvtp5);
  p.addPar("DWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dwb);
  p.addPar("DWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dwc);
  p.addPar("DWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dwg);
  p.addPar("DWJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::dwj);
  p.addPar("EASUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::easub);
  p.addPar("EF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ef);
  p.addPar("EGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::egidl);
  p.addPar("EGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::egisl);
  p.addPar("EIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::eigbinv);
  p.addPar("EM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::em);
  p.addPar("EOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::eot);
  p.addPar("EPSRGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::epsrgate);
  p.addPar("EPSROX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::epsrox);
  p.addPar("EPSRSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::epsrsub);
  p.addPar("ETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::eta0);
  p.addPar("ETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::etab);
  p.addPar("EU",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::eu);
  p.addPar("FGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::fgidl);
  p.addPar("FGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::fgisl);
  p.addPar("FNOIMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::fnoimod);
  p.addPar("FPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::fprout);
  p.addPar("GAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::gamma1);
  p.addPar("GAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::gamma2);
  p.addPar("GBMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::gbmin);
  p.addPar("GEOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::geomod);
  p.addPar("GIDLMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::gidlmod);
  p.addPar("IGBMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::igbmod);
  p.addPar("IGCMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::igcmod);
  p.addPar("IJTHDFWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ijthdfwd);
  p.addPar("IJTHDREV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ijthdrev);
  p.addPar("IJTHSFWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ijthsfwd);
  p.addPar("IJTHSREV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ijthsrev);
  p.addPar("JSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jsd);
  p.addPar("JSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jss);
  p.addPar("JSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jswd);
  p.addPar("JSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jswgd);
  p.addPar("JSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jswgs);
  p.addPar("JSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jsws);
  p.addPar("JTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtsd);
  p.addPar("JTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtss);
  p.addPar("JTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtsswd);
  p.addPar("JTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtsswgd);
  p.addPar("JTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtsswgs);
  p.addPar("JTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtssws);
  p.addPar("JTWEFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::jtweff);
  p.addPar("K1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::k1);
  p.addPar("K2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::k2);
  p.addPar("K2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::k2we);
  p.addPar("K3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::k3);
  p.addPar("K3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::k3b);
  p.addPar("KETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::keta);
  p.addPar("KF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kf);
  p.addPar("KGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kgidl);
  p.addPar("KGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kgisl);
  p.addPar("KT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kt1);
  p.addPar("KT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kt1l);
  p.addPar("KT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kt2);
  p.addPar("KU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ku0);
  p.addPar("KU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ku0we);
  p.addPar("KVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kvsat);
  p.addPar("KVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kvth0);
  p.addPar("KVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::kvth0we);
  p.addPar("L",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::l);
  p.addPar("LA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::la0);
  p.addPar("LA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::la1);
  p.addPar("LA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::la2);
  p.addPar("LACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lacde);
  p.addPar("LAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lagidl);
  p.addPar("LAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lagisl);
  p.addPar("LAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lags);
  p.addPar("LAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigbacc);
  p.addPar("LAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigbinv);
  p.addPar("LAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigc);
  p.addPar("LAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigd);
  p.addPar("LAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigs);
  p.addPar("LAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::laigsd);
  p.addPar("LALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lalpha0);
  p.addPar("LALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lalpha1);
  p.addPar("LAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lambda);
  p.addPar("LAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lat);
  p.addPar("LB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lb0);
  p.addPar("LB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lb1);
  p.addPar("LBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbeta0);
  p.addPar("LBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbgidl);
  p.addPar("LBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbgisl);
  p.addPar("LBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigbacc);
  p.addPar("LBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigbinv);
  p.addPar("LBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigc);
  p.addPar("LBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigd);
  p.addPar("LBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigs);
  p.addPar("LBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lbigsd);
  p.addPar("LC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lc);
  p.addPar("LCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcdsc);
  p.addPar("LCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcdscb);
  p.addPar("LCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcdscd);
  p.addPar("LCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcf);
  p.addPar("LCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcgdl);
  p.addPar("LCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcgidl);
  p.addPar("LCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcgisl);
  p.addPar("LCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcgsl);
  p.addPar("LCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigbacc);
  p.addPar("LCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigbinv);
  p.addPar("LCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigc);
  p.addPar("LCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigd);
  p.addPar("LCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigs);
  p.addPar("LCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcigsd);
  p.addPar("LCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcit);
  p.addPar("LCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lckappad);
  p.addPar("LCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lckappas);
  p.addPar("LCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lclc);
  p.addPar("LCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lcle);
  p.addPar("LDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldelta);
  p.addPar("LDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldrout);
  p.addPar("LDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldsub);
  p.addPar("LDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt0);
  p.addPar("LDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt0w);
  p.addPar("LDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt1);
  p.addPar("LDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt1w);
  p.addPar("LDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt2);
  p.addPar("LDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvt2w);
  p.addPar("LDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp0);
  p.addPar("LDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp1);
  p.addPar("LDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp2);
  p.addPar("LDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp3);
  p.addPar("LDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp4);
  p.addPar("LDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldvtp5);
  p.addPar("LDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldwb);
  p.addPar("LDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ldwg);
  p.addPar("LEFFEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::leffeot);
  p.addPar("LEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::legidl);
  p.addPar("LEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::legisl);
  p.addPar("LEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::leigbinv);
  p.addPar("LETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::leta0);
  p.addPar("LETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::letab);
  p.addPar("LEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::leu);
  p.addPar("LEVEL",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::level);
  p.addPar("LFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lfgidl);
  p.addPar("LFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lfgisl);
  p.addPar("LFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lfprout);
  p.addPar("LGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lgamma1);
  p.addPar("LGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lgamma2);
  p.addPar("LINT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lint);
  p.addPar("LK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lk1);
  p.addPar("LK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lk2);
  p.addPar("LK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lk2we);
  p.addPar("LK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lk3);
  p.addPar("LK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lk3b);
  p.addPar("LKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lketa);
  p.addPar("LKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkgidl);
  p.addPar("LKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkgisl);
  p.addPar("LKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkt1);
  p.addPar("LKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkt1l);
  p.addPar("LKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkt2);
  p.addPar("LKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lku0);
  p.addPar("LKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lku0we);
  p.addPar("LKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkvth0);
  p.addPar("LKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lkvth0we);
  p.addPar("LL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ll);
  p.addPar("LLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llambda);
  p.addPar("LLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llc);
  p.addPar("LLN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lln);
  p.addPar("LLODKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llodku0);
  p.addPar("LLODVTH",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llodvth);
  p.addPar("LLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llp);
  p.addPar("LLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llpe0);
  p.addPar("LLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::llpeb);
  p.addPar("LMAX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lmax);
  p.addPar("LMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lmin);
  p.addPar("LMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lminv);
  p.addPar("LMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lminvcv);
  p.addPar("LMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lmoin);
  p.addPar("LNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lndep);
  p.addPar("LNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnfactor);
  p.addPar("LNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lngate);
  p.addPar("LNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnigbacc);
  p.addPar("LNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnigbinv);
  p.addPar("LNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnigc);
  p.addPar("LNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnoff);
  p.addPar("LNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnsd);
  p.addPar("LNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lnsub);
  p.addPar("LNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lntox);
  p.addPar("LODETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lodeta0);
  p.addPar("LODK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lodk2);
  p.addPar("LP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lp);
  p.addPar("LPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpclm);
  p.addPar("LPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpdiblc1);
  p.addPar("LPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpdiblc2);
  p.addPar("LPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpdiblcb);
  p.addPar("LPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpdits);
  p.addPar("LPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpditsd);
  p.addPar("LPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpe0);
  p.addPar("LPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpeb);
  p.addPar("LPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lphin);
  p.addPar("LPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpigcd);
  p.addPar("LPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpoxedge);
  p.addPar("LPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lprt);
  p.addPar("LPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lprwb);
  p.addPar("LPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lprwg);
  p.addPar("LPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpscbe1);
  p.addPar("LPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpscbe2);
  p.addPar("LPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lpvag);
  p.addPar("LRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lrdsw);
  p.addPar("LRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lrdw);
  p.addPar("LRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lrgidl);
  p.addPar("LRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lrgisl);
  p.addPar("LRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lrsw);
  p.addPar("LTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lteta0);
  p.addPar("LTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ltnfactor);
  p.addPar("LTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ltvfbsdoff);
  p.addPar("LTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ltvoff);
  p.addPar("LTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ltvoffcv);
  p.addPar("LU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lu0);
  p.addPar("LUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lua);
  p.addPar("LUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lua1);
  p.addPar("LUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lub);
  p.addPar("LUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lub1);
  p.addPar("LUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::luc);
  p.addPar("LUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::luc1);
  p.addPar("LUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lucs);
  p.addPar("LUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lucste);
  p.addPar("LUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lud);
  p.addPar("LUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lud1);
  p.addPar("LUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lup);
  p.addPar("LUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lute);
  p.addPar("LVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvbm);
  p.addPar("LVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvbx);
  p.addPar("LVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvfb);
  p.addPar("LVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvfbcv);
  p.addPar("LVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvfbsdoff);
  p.addPar("LVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvoff);
  p.addPar("LVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvoffcv);
  p.addPar("LVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvsat);
  p.addPar("LVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvth0);
  p.addPar("LVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lvtl);
  p.addPar("LW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lw);
  p.addPar("LW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lw0);
  p.addPar("LWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lwc);
  p.addPar("LWL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lwl);
  p.addPar("LWLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lwlc);
  p.addPar("LWN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lwn);
  p.addPar("LWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lwr);
  p.addPar("LXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lxj);
  p.addPar("LXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lxn);
  p.addPar("LXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lxrcrg1);
  p.addPar("LXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lxrcrg2);
  p.addPar("LXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::lxt);
  p.addPar("MINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::minv);
  p.addPar("MINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::minvcv);
  p.addPar("MJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjd);
  p.addPar("MJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjs);
  p.addPar("MJSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjswd);
  p.addPar("MJSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjswgd);
  p.addPar("MJSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjswgs);
  p.addPar("MJSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::mjsws);
  p.addPar("MOBMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::mobmod);
  p.addPar("MOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::moin);
  p.addPar("MTRLCOMPATMOD",static_cast<double>(-9999999), &COGENDAbsim4_va::Instance::mtrlcompatmod);
  p.addPar("MTRLMOD",static_cast<double>(-9999999), &COGENDAbsim4_va::Instance::mtrlmod);
  p.addPar("NDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ndep);
  p.addPar("NF",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::nf);
  p.addPar("NFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nfactor);
  p.addPar("NGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ngate);
  p.addPar("NGCON",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ngcon);
  p.addPar("NI0SUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ni0sub);
  p.addPar("NIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nigbacc);
  p.addPar("NIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nigbinv);
  p.addPar("NIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nigc);
  p.addPar("NJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njd);
  p.addPar("NJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njs);
  p.addPar("NJTS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njts);
  p.addPar("NJTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njtsd);
  p.addPar("NJTSSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njtssw);
  p.addPar("NJTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njtsswd);
  p.addPar("NJTSSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njtsswg);
  p.addPar("NJTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::njtsswgd);
  p.addPar("NOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::noff);
  p.addPar("NOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::noia);
  p.addPar("NOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::noib);
  p.addPar("NOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::noic);
  p.addPar("NRD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nrd);
  p.addPar("NRS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nrs);
  p.addPar("NSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nsd);
  p.addPar("NSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::nsub);
  p.addPar("NTNOI",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ntnoi);
  p.addPar("NTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ntox);
  p.addPar("OFF",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::off);
  p.addPar("PA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pa0);
  p.addPar("PA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pa1);
  p.addPar("PA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pa2);
  p.addPar("PACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pacde);
  p.addPar("PAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pagidl);
  p.addPar("PAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pagisl);
  p.addPar("PAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pags);
  p.addPar("PAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigbacc);
  p.addPar("PAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigbinv);
  p.addPar("PAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigc);
  p.addPar("PAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigd);
  p.addPar("PAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigs);
  p.addPar("PAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::paigsd);
  p.addPar("PALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::palpha0);
  p.addPar("PALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::palpha1);
  p.addPar("PARAMCHK",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::paramchk);
  p.addPar("PAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pat);
  p.addPar("PB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pb0);
  p.addPar("PB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pb1);
  p.addPar("PBD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbd);
  p.addPar("PBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbeta0);
  p.addPar("PBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbgidl);
  p.addPar("PBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbgisl);
  p.addPar("PBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigbacc);
  p.addPar("PBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigbinv);
  p.addPar("PBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigc);
  p.addPar("PBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigd);
  p.addPar("PBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigs);
  p.addPar("PBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbigsd);
  p.addPar("PBS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbs);
  p.addPar("PBSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbswd);
  p.addPar("PBSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbswgd);
  p.addPar("PBSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbswgs);
  p.addPar("PBSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pbsws);
  p.addPar("PCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcdsc);
  p.addPar("PCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcdscb);
  p.addPar("PCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcdscd);
  p.addPar("PCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcf);
  p.addPar("PCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcgdl);
  p.addPar("PCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcgidl);
  p.addPar("PCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcgisl);
  p.addPar("PCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcgsl);
  p.addPar("PCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigbacc);
  p.addPar("PCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigbinv);
  p.addPar("PCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigc);
  p.addPar("PCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigd);
  p.addPar("PCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigs);
  p.addPar("PCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcigsd);
  p.addPar("PCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcit);
  p.addPar("PCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pckappad);
  p.addPar("PCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pckappas);
  p.addPar("PCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pclc);
  p.addPar("PCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pcle);
  p.addPar("PCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pclm);
  p.addPar("PD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pd);
  p.addPar("PDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdelta);
  p.addPar("PDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdiblc1);
  p.addPar("PDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdiblc2);
  p.addPar("PDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdiblcb);
  p.addPar("PDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdits);
  p.addPar("PDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pditsd);
  p.addPar("PDITSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pditsl);
  p.addPar("PDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdrout);
  p.addPar("PDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdsub);
  p.addPar("PDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt0);
  p.addPar("PDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt0w);
  p.addPar("PDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt1);
  p.addPar("PDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt1w);
  p.addPar("PDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt2);
  p.addPar("PDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvt2w);
  p.addPar("PDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp0);
  p.addPar("PDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp1);
  p.addPar("PDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp2);
  p.addPar("PDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp3);
  p.addPar("PDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp4);
  p.addPar("PDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdvtp5);
  p.addPar("PDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdwb);
  p.addPar("PDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pdwg);
  p.addPar("PEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pegidl);
  p.addPar("PEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pegisl);
  p.addPar("PEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::peigbinv);
  p.addPar("PERMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::permod);
  p.addPar("PETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::peta0);
  p.addPar("PETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::petab);
  p.addPar("PEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::peu);
  p.addPar("PFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pfgidl);
  p.addPar("PFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pfgisl);
  p.addPar("PFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pfprout);
  p.addPar("PGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pgamma1);
  p.addPar("PGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pgamma2);
  p.addPar("PHIG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::phig);
  p.addPar("PHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::phin);
  p.addPar("PIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pigcd);
  p.addPar("PK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pk1);
  p.addPar("PK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pk2);
  p.addPar("PK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pk2we);
  p.addPar("PK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pk3);
  p.addPar("PK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pk3b);
  p.addPar("PKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pketa);
  p.addPar("PKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkgidl);
  p.addPar("PKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkgisl);
  p.addPar("PKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkt1);
  p.addPar("PKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkt1l);
  p.addPar("PKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkt2);
  p.addPar("PKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pku0);
  p.addPar("PKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pku0we);
  p.addPar("PKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkvth0);
  p.addPar("PKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pkvth0we);
  p.addPar("PLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::plambda);
  p.addPar("PLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::plp);
  p.addPar("PLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::plpe0);
  p.addPar("PLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::plpeb);
  p.addPar("PMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pminv);
  p.addPar("PMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pminvcv);
  p.addPar("PMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pmoin);
  p.addPar("PNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pndep);
  p.addPar("PNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnfactor);
  p.addPar("PNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pngate);
  p.addPar("PNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnigbacc);
  p.addPar("PNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnigbinv);
  p.addPar("PNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnigc);
  p.addPar("PNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnoff);
  p.addPar("PNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnsd);
  p.addPar("PNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pnsub);
  p.addPar("PNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pntox);
  p.addPar("POXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::poxedge);
  p.addPar("PPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppclm);
  p.addPar("PPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppdiblc1);
  p.addPar("PPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppdiblc2);
  p.addPar("PPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppdiblcb);
  p.addPar("PPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppdits);
  p.addPar("PPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppditsd);
  p.addPar("PPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pphin);
  p.addPar("PPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppigcd);
  p.addPar("PPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppoxedge);
  p.addPar("PPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pprt);
  p.addPar("PPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pprwb);
  p.addPar("PPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pprwg);
  p.addPar("PPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppscbe1);
  p.addPar("PPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppscbe2);
  p.addPar("PPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ppvag);
  p.addPar("PRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prdsw);
  p.addPar("PRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prdw);
  p.addPar("PRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prgidl);
  p.addPar("PRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prgisl);
  p.addPar("PRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prsw);
  p.addPar("PRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prt);
  p.addPar("PRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prwb);
  p.addPar("PRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::prwg);
  p.addPar("PS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ps);
  p.addPar("PSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pscbe1);
  p.addPar("PSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pscbe2);
  p.addPar("PTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pteta0);
  p.addPar("PTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ptnfactor);
  p.addPar("PTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ptvfbsdoff);
  p.addPar("PTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ptvoff);
  p.addPar("PTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ptvoffcv);
  p.addPar("PU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pu0);
  p.addPar("PUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pua);
  p.addPar("PUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pua1);
  p.addPar("PUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pub);
  p.addPar("PUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pub1);
  p.addPar("PUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::puc);
  p.addPar("PUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::puc1);
  p.addPar("PUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pucs);
  p.addPar("PUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pucste);
  p.addPar("PUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pud);
  p.addPar("PUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pud1);
  p.addPar("PUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pup);
  p.addPar("PUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pute);
  p.addPar("PVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvag);
  p.addPar("PVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvbm);
  p.addPar("PVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvbx);
  p.addPar("PVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvfb);
  p.addPar("PVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvfbcv);
  p.addPar("PVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvfbsdoff);
  p.addPar("PVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvoff);
  p.addPar("PVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvoffcv);
  p.addPar("PVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvsat);
  p.addPar("PVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvth0);
  p.addPar("PVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pvtl);
  p.addPar("PW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pw0);
  p.addPar("PWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pwr);
  p.addPar("PXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pxj);
  p.addPar("PXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pxn);
  p.addPar("PXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pxrcrg1);
  p.addPar("PXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pxrcrg2);
  p.addPar("PXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::pxt);
  p.addPar("RBDB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbdb);
  p.addPar("RBDBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbdbx0);
  p.addPar("RBDBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbdby0);
  p.addPar("RBODYMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::rbodymod);
  p.addPar("RBPB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpb);
  p.addPar("RBPBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbx0);
  p.addPar("RBPBXL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbxl);
  p.addPar("RBPBXNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbxnf);
  p.addPar("RBPBXW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbxw);
  p.addPar("RBPBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpby0);
  p.addPar("RBPBYL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbyl);
  p.addPar("RBPBYNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbynf);
  p.addPar("RBPBYW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpbyw);
  p.addPar("RBPD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpd);
  p.addPar("RBPD0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpd0);
  p.addPar("RBPDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpdl);
  p.addPar("RBPDNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpdnf);
  p.addPar("RBPDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpdw);
  p.addPar("RBPS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbps);
  p.addPar("RBPS0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbps0);
  p.addPar("RBPSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpsl);
  p.addPar("RBPSNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpsnf);
  p.addPar("RBPSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbpsw);
  p.addPar("RBSB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsb);
  p.addPar("RBSBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsbx0);
  p.addPar("RBSBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsby0);
  p.addPar("RBSDBXL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbxl);
  p.addPar("RBSDBXNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbxnf);
  p.addPar("RBSDBXW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbxw);
  p.addPar("RBSDBYL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbyl);
  p.addPar("RBSDBYNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbynf);
  p.addPar("RBSDBYW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rbsdbyw);
  p.addPar("RDSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::rdsmod);
  p.addPar("RDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rdsw);
  p.addPar("RDSWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rdswmin);
  p.addPar("RDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rdw);
  p.addPar("RDWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rdwmin);
  p.addPar("RGATEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::rgatemod);
  p.addPar("RGEOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::rgeomod);
  p.addPar("RGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rgidl);
  p.addPar("RGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rgisl);
  p.addPar("RNOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rnoia);
  p.addPar("RNOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rnoib);
  p.addPar("RNOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rnoic);
  p.addPar("RSH",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rsh);
  p.addPar("RSHG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rshg);
  p.addPar("RSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rsw);
  p.addPar("RSWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::rswmin);
  p.addPar("SA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sa);
  p.addPar("SAREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::saref);
  p.addPar("SB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sb);
  p.addPar("SBREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sbref);
  p.addPar("SC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sc);
  p.addPar("SCA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sca);
  p.addPar("SCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::scb);
  p.addPar("SCC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::scc);
  p.addPar("SCREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::scref);
  p.addPar("SD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::sd);
  p.addPar("STETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::steta0);
  p.addPar("STK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::stk2);
  p.addPar("TBGASUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tbgasub);
  p.addPar("TBGBSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tbgbsub);
  p.addPar("TCJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tcj);
  p.addPar("TCJSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tcjsw);
  p.addPar("TCJSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tcjswg);
  p.addPar("TEMPEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tempeot);
  p.addPar("TEMPMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::tempmod);
  p.addPar("TETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::teta0);
  p.addPar("TKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tku0);
  p.addPar("TNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnfactor);
  p.addPar("TNJTS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjts);
  p.addPar("TNJTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjtsd);
  p.addPar("TNJTSSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjtssw);
  p.addPar("TNJTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjtsswd);
  p.addPar("TNJTSSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjtsswg);
  p.addPar("TNJTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnjtsswgd);
  p.addPar("TNOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnoia);
  p.addPar("TNOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnoib);
  p.addPar("TNOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnoic);
  p.addPar("TNOIMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::tnoimod);
  p.addPar("TNOM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tnom);
  p.addPar("TOXE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::toxe);
  p.addPar("TOXM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::toxm);
  p.addPar("TOXP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::toxp);
  p.addPar("TOXREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::toxref);
  p.addPar("TPB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tpb);
  p.addPar("TPBSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tpbsw);
  p.addPar("TPBSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tpbswg);
  p.addPar("TRNQSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::trnqsmod);
  p.addPar("TVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tvfbsdoff);
  p.addPar("TVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tvoff);
  p.addPar("TVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::tvoffcv);
  p.addPar("TYPE",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::type);
  p.addPar("U0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::u0);
  p.addPar("UA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ua);
  p.addPar("UA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ua1);
  p.addPar("UB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ub);
  p.addPar("UB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ub1);
  p.addPar("UC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::uc);
  p.addPar("UC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::uc1);
  p.addPar("UCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ucs);
  p.addPar("UCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ucste);
  p.addPar("UD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ud);
  p.addPar("UD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ud1);
  p.addPar("UP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::up);
  p.addPar("UTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ute);
  p.addPar("VBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vbm);
  p.addPar("VBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vbx);
  p.addPar("VDDEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vddeot);
  p.addPar("VERBOSE",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::verbose);
  p.addPar("VERSION",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::version);
  p.addPar("VFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vfb);
  p.addPar("VFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vfbcv);
  p.addPar("VFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vfbsdoff);
  p.addPar("VOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::voff);
  p.addPar("VOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::voffcv);
  p.addPar("VOFFCVL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::voffcvl);
  p.addPar("VOFFL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::voffl);
  p.addPar("VSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vsat);
  p.addPar("VTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vth0);
  p.addPar("VTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtl);
  p.addPar("VTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtsd);
  p.addPar("VTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtss);
  p.addPar("VTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtsswd);
  p.addPar("VTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtsswgd);
  p.addPar("VTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtsswgs);
  p.addPar("VTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::vtssws);
  p.addPar("W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::w);
  p.addPar("W0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::w0);
  p.addPar("WA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wa0);
  p.addPar("WA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wa1);
  p.addPar("WA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wa2);
  p.addPar("WACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wacde);
  p.addPar("WAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wagidl);
  p.addPar("WAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wagisl);
  p.addPar("WAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wags);
  p.addPar("WAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigbacc);
  p.addPar("WAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigbinv);
  p.addPar("WAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigc);
  p.addPar("WAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigd);
  p.addPar("WAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigs);
  p.addPar("WAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::waigsd);
  p.addPar("WALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::walpha0);
  p.addPar("WALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::walpha1);
  p.addPar("WAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wat);
  p.addPar("WB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wb0);
  p.addPar("WB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wb1);
  p.addPar("WBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbeta0);
  p.addPar("WBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbgidl);
  p.addPar("WBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbgisl);
  p.addPar("WBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigbacc);
  p.addPar("WBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigbinv);
  p.addPar("WBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigc);
  p.addPar("WBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigd);
  p.addPar("WBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigs);
  p.addPar("WBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wbigsd);
  p.addPar("WCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcdsc);
  p.addPar("WCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcdscb);
  p.addPar("WCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcdscd);
  p.addPar("WCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcf);
  p.addPar("WCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcgdl);
  p.addPar("WCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcgidl);
  p.addPar("WCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcgisl);
  p.addPar("WCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcgsl);
  p.addPar("WCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigbacc);
  p.addPar("WCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigbinv);
  p.addPar("WCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigc);
  p.addPar("WCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigd);
  p.addPar("WCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigs);
  p.addPar("WCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcigsd);
  p.addPar("WCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcit);
  p.addPar("WCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wckappad);
  p.addPar("WCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wckappas);
  p.addPar("WCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wclc);
  p.addPar("WCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wcle);
  p.addPar("WDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdelta);
  p.addPar("WDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdrout);
  p.addPar("WDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdsub);
  p.addPar("WDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt0);
  p.addPar("WDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt0w);
  p.addPar("WDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt1);
  p.addPar("WDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt1w);
  p.addPar("WDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt2);
  p.addPar("WDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvt2w);
  p.addPar("WDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp0);
  p.addPar("WDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp1);
  p.addPar("WDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp2);
  p.addPar("WDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp3);
  p.addPar("WDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp4);
  p.addPar("WDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdvtp5);
  p.addPar("WDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdwb);
  p.addPar("WDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wdwg);
  p.addPar("WEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::web);
  p.addPar("WEC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wec);
  p.addPar("WEFFEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::weffeot);
  p.addPar("WEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wegidl);
  p.addPar("WEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wegisl);
  p.addPar("WEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::weigbinv);
  p.addPar("WETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::weta0);
  p.addPar("WETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wetab);
  p.addPar("WEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::weu);
  p.addPar("WFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wfgidl);
  p.addPar("WFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wfgisl);
  p.addPar("WFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wfprout);
  p.addPar("WGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wgamma1);
  p.addPar("WGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wgamma2);
  p.addPar("WINT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wint);
  p.addPar("WK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wk1);
  p.addPar("WK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wk2);
  p.addPar("WK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wk2we);
  p.addPar("WK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wk3);
  p.addPar("WK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wk3b);
  p.addPar("WKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wketa);
  p.addPar("WKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkgidl);
  p.addPar("WKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkgisl);
  p.addPar("WKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkt1);
  p.addPar("WKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkt1l);
  p.addPar("WKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkt2);
  p.addPar("WKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wku0);
  p.addPar("WKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wku0we);
  p.addPar("WKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkvth0);
  p.addPar("WKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wkvth0we);
  p.addPar("WL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wl);
  p.addPar("WLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlambda);
  p.addPar("WLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlc);
  p.addPar("WLN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wln);
  p.addPar("WLOD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlod);
  p.addPar("WLODKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlodku0);
  p.addPar("WLODVTH",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlodvth);
  p.addPar("WLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlp);
  p.addPar("WLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlpe0);
  p.addPar("WLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wlpeb);
  p.addPar("WMAX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wmax);
  p.addPar("WMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wmin);
  p.addPar("WMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wminv);
  p.addPar("WMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wminvcv);
  p.addPar("WMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wmoin);
  p.addPar("WNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wndep);
  p.addPar("WNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnfactor);
  p.addPar("WNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wngate);
  p.addPar("WNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnigbacc);
  p.addPar("WNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnigbinv);
  p.addPar("WNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnigc);
  p.addPar("WNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnoff);
  p.addPar("WNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnsd);
  p.addPar("WNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wnsub);
  p.addPar("WNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wntox);
  p.addPar("WPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpclm);
  p.addPar("WPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpdiblc1);
  p.addPar("WPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpdiblc2);
  p.addPar("WPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpdiblcb);
  p.addPar("WPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpdits);
  p.addPar("WPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpditsd);
  p.addPar("WPEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Instance::wpemod);
  p.addPar("WPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wphin);
  p.addPar("WPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpigcd);
  p.addPar("WPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpoxedge);
  p.addPar("WPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wprt);
  p.addPar("WPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wprwb);
  p.addPar("WPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wprwg);
  p.addPar("WPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpscbe1);
  p.addPar("WPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpscbe2);
  p.addPar("WPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wpvag);
  p.addPar("WR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wr);
  p.addPar("WRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wrdsw);
  p.addPar("WRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wrdw);
  p.addPar("WRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wrgidl);
  p.addPar("WRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wrgisl);
  p.addPar("WRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wrsw);
  p.addPar("WTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wteta0);
  p.addPar("WTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wtnfactor);
  p.addPar("WTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wtvfbsdoff);
  p.addPar("WTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wtvoff);
  p.addPar("WTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wtvoffcv);
  p.addPar("WU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wu0);
  p.addPar("WUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wua);
  p.addPar("WUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wua1);
  p.addPar("WUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wub);
  p.addPar("WUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wub1);
  p.addPar("WUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wuc);
  p.addPar("WUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wuc1);
  p.addPar("WUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wucs);
  p.addPar("WUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wucste);
  p.addPar("WUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wud);
  p.addPar("WUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wud1);
  p.addPar("WUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wup);
  p.addPar("WUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wute);
  p.addPar("WVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvbm);
  p.addPar("WVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvbx);
  p.addPar("WVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvfb);
  p.addPar("WVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvfbcv);
  p.addPar("WVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvfbsdoff);
  p.addPar("WVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvoff);
  p.addPar("WVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvoffcv);
  p.addPar("WVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvsat);
  p.addPar("WVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvth0);
  p.addPar("WVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wvtl);
  p.addPar("WW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ww);
  p.addPar("WW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::ww0);
  p.addPar("WWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wwc);
  p.addPar("WWL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wwl);
  p.addPar("WWLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wwlc);
  p.addPar("WWN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wwn);
  p.addPar("WWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wwr);
  p.addPar("WXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wxj);
  p.addPar("WXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wxn);
  p.addPar("WXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wxrcrg1);
  p.addPar("WXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wxrcrg2);
  p.addPar("WXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::wxt);
  p.addPar("XGL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xgl);
  p.addPar("XGW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xgw);
  p.addPar("XJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xj);
  p.addPar("XJBVD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xjbvd);
  p.addPar("XJBVS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xjbvs);
  p.addPar("XL",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xl);
  p.addPar("XN",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xn);
  p.addPar("XPART",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xpart);
  p.addPar("XRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xrcrg1);
  p.addPar("XRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xrcrg2);
  p.addPar("XT",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xt);
  p.addPar("XTID",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtid);
  p.addPar("XTIS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtis);
  p.addPar("XTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtsd);
  p.addPar("XTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtss);
  p.addPar("XTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtsswd);
  p.addPar("XTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtsswgd);
  p.addPar("XTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtsswgs);
  p.addPar("XTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xtssws);
  p.addPar("XW",static_cast<double>(-12345789), &COGENDAbsim4_va::Instance::xw);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAbsim4_va::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAbsim4_va::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("_CKT_GMIN",static_cast<double>(1e-12), &COGENDAbsim4_va::Model::_ckt_gmin);
  p.addPar("_MIN",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::_min);
  p.addPar("A0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::a0);
  p.addPar("A1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::a1);
  p.addPar("A2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::a2);
  p.addPar("ACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::acde);
  p.addPar("ACNQSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::acnqsmod);
  p.addPar("AD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ad);
  p.addPar("ADOS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ados);
  p.addPar("AF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::af);
  p.addPar("AGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::agidl);
  p.addPar("AGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::agisl);
  p.addPar("AGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ags);
  p.addPar("AIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigbacc);
  p.addPar("AIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigbinv);
  p.addPar("AIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigc);
  p.addPar("AIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigd);
  p.addPar("AIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigs);
  p.addPar("AIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::aigsd);
  p.addPar("ALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::alpha0);
  p.addPar("ALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::alpha1);
  p.addPar("AS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::as);
  p.addPar("AT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::at);
  p.addPar("B0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::b0);
  p.addPar("B1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::b1);
  p.addPar("BDOS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bdos);
  p.addPar("BETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::beta0);
  p.addPar("BG0SUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bg0sub);
  p.addPar("BGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bgidl);
  p.addPar("BGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bgisl);
  p.addPar("BIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigbacc);
  p.addPar("BIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigbinv);
  p.addPar("BIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigc);
  p.addPar("BIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigd);
  p.addPar("BIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigs);
  p.addPar("BIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bigsd);
  p.addPar("BINUNIT",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::binunit);
  p.addPar("BVD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bvd);
  p.addPar("BVS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::bvs);
  p.addPar("CAPMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::capmod);
  p.addPar("CDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cdsc);
  p.addPar("CDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cdscb);
  p.addPar("CDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cdscd);
  p.addPar("CF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cf);
  p.addPar("CGBO",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgbo);
  p.addPar("CGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgdl);
  p.addPar("CGDO",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgdo);
  p.addPar("CGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgidl);
  p.addPar("CGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgisl);
  p.addPar("CGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgsl);
  p.addPar("CGSO",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cgso);
  p.addPar("CIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigbacc);
  p.addPar("CIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigbinv);
  p.addPar("CIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigc);
  p.addPar("CIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigd);
  p.addPar("CIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigs);
  p.addPar("CIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cigsd);
  p.addPar("CIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cit);
  p.addPar("CJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjd);
  p.addPar("CJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjs);
  p.addPar("CJSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjswd);
  p.addPar("CJSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjswgd);
  p.addPar("CJSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjswgs);
  p.addPar("CJSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cjsws);
  p.addPar("CKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ckappad);
  p.addPar("CKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ckappas);
  p.addPar("CLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::clc);
  p.addPar("CLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::cle);
  p.addPar("CVCHARGEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::cvchargemod);
  p.addPar("DELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::delta);
  p.addPar("DELVTO",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::delvto);
  p.addPar("DIOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::diomod);
  p.addPar("DLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dlc);
  p.addPar("DLCIG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dlcig);
  p.addPar("DLCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dlcigd);
  p.addPar("DMCG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dmcg);
  p.addPar("DMCGT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dmcgt);
  p.addPar("DMCI",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dmci);
  p.addPar("DMDG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dmdg);
  p.addPar("DROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::drout);
  p.addPar("DSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dsub);
  p.addPar("DTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dtox);
  p.addPar("DVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt0);
  p.addPar("DVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt0w);
  p.addPar("DVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt1);
  p.addPar("DVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt1w);
  p.addPar("DVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt2);
  p.addPar("DVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvt2w);
  p.addPar("DVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp0);
  p.addPar("DVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp1);
  p.addPar("DVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp2);
  p.addPar("DVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp3);
  p.addPar("DVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp4);
  p.addPar("DVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dvtp5);
  p.addPar("DWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dwb);
  p.addPar("DWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dwc);
  p.addPar("DWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dwg);
  p.addPar("DWJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::dwj);
  p.addPar("EASUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::easub);
  p.addPar("EF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ef);
  p.addPar("EGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::egidl);
  p.addPar("EGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::egisl);
  p.addPar("EIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::eigbinv);
  p.addPar("EM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::em);
  p.addPar("EOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::eot);
  p.addPar("EPSRGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::epsrgate);
  p.addPar("EPSROX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::epsrox);
  p.addPar("EPSRSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::epsrsub);
  p.addPar("ETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::eta0);
  p.addPar("ETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::etab);
  p.addPar("EU",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::eu);
  p.addPar("FGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::fgidl);
  p.addPar("FGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::fgisl);
  p.addPar("FNOIMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::fnoimod);
  p.addPar("FPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::fprout);
  p.addPar("GAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::gamma1);
  p.addPar("GAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::gamma2);
  p.addPar("GBMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::gbmin);
  p.addPar("GEOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::geomod);
  p.addPar("GIDLMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::gidlmod);
  p.addPar("IGBMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::igbmod);
  p.addPar("IGCMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::igcmod);
  p.addPar("IJTHDFWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ijthdfwd);
  p.addPar("IJTHDREV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ijthdrev);
  p.addPar("IJTHSFWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ijthsfwd);
  p.addPar("IJTHSREV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ijthsrev);
  p.addPar("JSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jsd);
  p.addPar("JSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jss);
  p.addPar("JSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jswd);
  p.addPar("JSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jswgd);
  p.addPar("JSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jswgs);
  p.addPar("JSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jsws);
  p.addPar("JTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtsd);
  p.addPar("JTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtss);
  p.addPar("JTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtsswd);
  p.addPar("JTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtsswgd);
  p.addPar("JTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtsswgs);
  p.addPar("JTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtssws);
  p.addPar("JTWEFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::jtweff);
  p.addPar("K1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::k1);
  p.addPar("K2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::k2);
  p.addPar("K2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::k2we);
  p.addPar("K3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::k3);
  p.addPar("K3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::k3b);
  p.addPar("KETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::keta);
  p.addPar("KF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kf);
  p.addPar("KGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kgidl);
  p.addPar("KGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kgisl);
  p.addPar("KT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kt1);
  p.addPar("KT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kt1l);
  p.addPar("KT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kt2);
  p.addPar("KU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ku0);
  p.addPar("KU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ku0we);
  p.addPar("KVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kvsat);
  p.addPar("KVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kvth0);
  p.addPar("KVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::kvth0we);
  p.addPar("L",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::l);
  p.addPar("LA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::la0);
  p.addPar("LA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::la1);
  p.addPar("LA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::la2);
  p.addPar("LACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lacde);
  p.addPar("LAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lagidl);
  p.addPar("LAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lagisl);
  p.addPar("LAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lags);
  p.addPar("LAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigbacc);
  p.addPar("LAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigbinv);
  p.addPar("LAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigc);
  p.addPar("LAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigd);
  p.addPar("LAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigs);
  p.addPar("LAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::laigsd);
  p.addPar("LALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lalpha0);
  p.addPar("LALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lalpha1);
  p.addPar("LAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lambda);
  p.addPar("LAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lat);
  p.addPar("LB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lb0);
  p.addPar("LB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lb1);
  p.addPar("LBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbeta0);
  p.addPar("LBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbgidl);
  p.addPar("LBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbgisl);
  p.addPar("LBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigbacc);
  p.addPar("LBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigbinv);
  p.addPar("LBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigc);
  p.addPar("LBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigd);
  p.addPar("LBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigs);
  p.addPar("LBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lbigsd);
  p.addPar("LC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lc);
  p.addPar("LCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcdsc);
  p.addPar("LCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcdscb);
  p.addPar("LCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcdscd);
  p.addPar("LCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcf);
  p.addPar("LCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcgdl);
  p.addPar("LCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcgidl);
  p.addPar("LCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcgisl);
  p.addPar("LCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcgsl);
  p.addPar("LCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigbacc);
  p.addPar("LCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigbinv);
  p.addPar("LCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigc);
  p.addPar("LCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigd);
  p.addPar("LCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigs);
  p.addPar("LCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcigsd);
  p.addPar("LCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcit);
  p.addPar("LCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lckappad);
  p.addPar("LCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lckappas);
  p.addPar("LCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lclc);
  p.addPar("LCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lcle);
  p.addPar("LDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldelta);
  p.addPar("LDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldrout);
  p.addPar("LDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldsub);
  p.addPar("LDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt0);
  p.addPar("LDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt0w);
  p.addPar("LDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt1);
  p.addPar("LDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt1w);
  p.addPar("LDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt2);
  p.addPar("LDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvt2w);
  p.addPar("LDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp0);
  p.addPar("LDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp1);
  p.addPar("LDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp2);
  p.addPar("LDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp3);
  p.addPar("LDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp4);
  p.addPar("LDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldvtp5);
  p.addPar("LDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldwb);
  p.addPar("LDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ldwg);
  p.addPar("LEFFEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::leffeot);
  p.addPar("LEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::legidl);
  p.addPar("LEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::legisl);
  p.addPar("LEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::leigbinv);
  p.addPar("LETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::leta0);
  p.addPar("LETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::letab);
  p.addPar("LEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::leu);
  p.addPar("LEVEL",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::level);
  p.addPar("LFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lfgidl);
  p.addPar("LFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lfgisl);
  p.addPar("LFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lfprout);
  p.addPar("LGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lgamma1);
  p.addPar("LGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lgamma2);
  p.addPar("LINT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lint);
  p.addPar("LK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lk1);
  p.addPar("LK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lk2);
  p.addPar("LK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lk2we);
  p.addPar("LK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lk3);
  p.addPar("LK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lk3b);
  p.addPar("LKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lketa);
  p.addPar("LKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkgidl);
  p.addPar("LKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkgisl);
  p.addPar("LKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkt1);
  p.addPar("LKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkt1l);
  p.addPar("LKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkt2);
  p.addPar("LKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lku0);
  p.addPar("LKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lku0we);
  p.addPar("LKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkvth0);
  p.addPar("LKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lkvth0we);
  p.addPar("LL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ll);
  p.addPar("LLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llambda);
  p.addPar("LLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llc);
  p.addPar("LLN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lln);
  p.addPar("LLODKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llodku0);
  p.addPar("LLODVTH",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llodvth);
  p.addPar("LLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llp);
  p.addPar("LLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llpe0);
  p.addPar("LLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::llpeb);
  p.addPar("LMAX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lmax);
  p.addPar("LMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lmin);
  p.addPar("LMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lminv);
  p.addPar("LMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lminvcv);
  p.addPar("LMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lmoin);
  p.addPar("LNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lndep);
  p.addPar("LNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnfactor);
  p.addPar("LNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lngate);
  p.addPar("LNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnigbacc);
  p.addPar("LNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnigbinv);
  p.addPar("LNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnigc);
  p.addPar("LNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnoff);
  p.addPar("LNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnsd);
  p.addPar("LNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lnsub);
  p.addPar("LNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lntox);
  p.addPar("LODETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lodeta0);
  p.addPar("LODK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lodk2);
  p.addPar("LP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lp);
  p.addPar("LPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpclm);
  p.addPar("LPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpdiblc1);
  p.addPar("LPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpdiblc2);
  p.addPar("LPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpdiblcb);
  p.addPar("LPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpdits);
  p.addPar("LPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpditsd);
  p.addPar("LPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpe0);
  p.addPar("LPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpeb);
  p.addPar("LPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lphin);
  p.addPar("LPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpigcd);
  p.addPar("LPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpoxedge);
  p.addPar("LPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lprt);
  p.addPar("LPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lprwb);
  p.addPar("LPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lprwg);
  p.addPar("LPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpscbe1);
  p.addPar("LPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpscbe2);
  p.addPar("LPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lpvag);
  p.addPar("LRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lrdsw);
  p.addPar("LRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lrdw);
  p.addPar("LRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lrgidl);
  p.addPar("LRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lrgisl);
  p.addPar("LRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lrsw);
  p.addPar("LTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lteta0);
  p.addPar("LTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ltnfactor);
  p.addPar("LTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ltvfbsdoff);
  p.addPar("LTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ltvoff);
  p.addPar("LTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ltvoffcv);
  p.addPar("LU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lu0);
  p.addPar("LUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lua);
  p.addPar("LUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lua1);
  p.addPar("LUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lub);
  p.addPar("LUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lub1);
  p.addPar("LUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::luc);
  p.addPar("LUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::luc1);
  p.addPar("LUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lucs);
  p.addPar("LUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lucste);
  p.addPar("LUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lud);
  p.addPar("LUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lud1);
  p.addPar("LUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lup);
  p.addPar("LUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lute);
  p.addPar("LVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvbm);
  p.addPar("LVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvbx);
  p.addPar("LVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvfb);
  p.addPar("LVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvfbcv);
  p.addPar("LVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvfbsdoff);
  p.addPar("LVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvoff);
  p.addPar("LVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvoffcv);
  p.addPar("LVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvsat);
  p.addPar("LVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvth0);
  p.addPar("LVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lvtl);
  p.addPar("LW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lw);
  p.addPar("LW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lw0);
  p.addPar("LWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lwc);
  p.addPar("LWL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lwl);
  p.addPar("LWLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lwlc);
  p.addPar("LWN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lwn);
  p.addPar("LWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lwr);
  p.addPar("LXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lxj);
  p.addPar("LXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lxn);
  p.addPar("LXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lxrcrg1);
  p.addPar("LXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lxrcrg2);
  p.addPar("LXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::lxt);
  p.addPar("MINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::minv);
  p.addPar("MINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::minvcv);
  p.addPar("MJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjd);
  p.addPar("MJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjs);
  p.addPar("MJSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjswd);
  p.addPar("MJSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjswgd);
  p.addPar("MJSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjswgs);
  p.addPar("MJSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::mjsws);
  p.addPar("MOBMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::mobmod);
  p.addPar("MOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::moin);
  p.addPar("MTRLCOMPATMOD",static_cast<double>(-9999999), &COGENDAbsim4_va::Model::mtrlcompatmod);
  p.addPar("MTRLMOD",static_cast<double>(-9999999), &COGENDAbsim4_va::Model::mtrlmod);
  p.addPar("NDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ndep);
  p.addPar("NF",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::nf);
  p.addPar("NFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nfactor);
  p.addPar("NGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ngate);
  p.addPar("NGCON",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ngcon);
  p.addPar("NI0SUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ni0sub);
  p.addPar("NIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nigbacc);
  p.addPar("NIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nigbinv);
  p.addPar("NIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nigc);
  p.addPar("NJD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njd);
  p.addPar("NJS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njs);
  p.addPar("NJTS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njts);
  p.addPar("NJTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njtsd);
  p.addPar("NJTSSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njtssw);
  p.addPar("NJTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njtsswd);
  p.addPar("NJTSSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njtsswg);
  p.addPar("NJTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::njtsswgd);
  p.addPar("NOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::noff);
  p.addPar("NOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::noia);
  p.addPar("NOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::noib);
  p.addPar("NOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::noic);
  p.addPar("NRD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nrd);
  p.addPar("NRS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nrs);
  p.addPar("NSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nsd);
  p.addPar("NSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::nsub);
  p.addPar("NTNOI",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ntnoi);
  p.addPar("NTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ntox);
  p.addPar("OFF",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::off);
  p.addPar("PA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pa0);
  p.addPar("PA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pa1);
  p.addPar("PA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pa2);
  p.addPar("PACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pacde);
  p.addPar("PAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pagidl);
  p.addPar("PAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pagisl);
  p.addPar("PAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pags);
  p.addPar("PAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigbacc);
  p.addPar("PAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigbinv);
  p.addPar("PAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigc);
  p.addPar("PAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigd);
  p.addPar("PAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigs);
  p.addPar("PAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::paigsd);
  p.addPar("PALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::palpha0);
  p.addPar("PALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::palpha1);
  p.addPar("PARAMCHK",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::paramchk);
  p.addPar("PAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pat);
  p.addPar("PB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pb0);
  p.addPar("PB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pb1);
  p.addPar("PBD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbd);
  p.addPar("PBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbeta0);
  p.addPar("PBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbgidl);
  p.addPar("PBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbgisl);
  p.addPar("PBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigbacc);
  p.addPar("PBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigbinv);
  p.addPar("PBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigc);
  p.addPar("PBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigd);
  p.addPar("PBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigs);
  p.addPar("PBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbigsd);
  p.addPar("PBS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbs);
  p.addPar("PBSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbswd);
  p.addPar("PBSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbswgd);
  p.addPar("PBSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbswgs);
  p.addPar("PBSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pbsws);
  p.addPar("PCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcdsc);
  p.addPar("PCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcdscb);
  p.addPar("PCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcdscd);
  p.addPar("PCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcf);
  p.addPar("PCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcgdl);
  p.addPar("PCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcgidl);
  p.addPar("PCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcgisl);
  p.addPar("PCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcgsl);
  p.addPar("PCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigbacc);
  p.addPar("PCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigbinv);
  p.addPar("PCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigc);
  p.addPar("PCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigd);
  p.addPar("PCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigs);
  p.addPar("PCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcigsd);
  p.addPar("PCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcit);
  p.addPar("PCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pckappad);
  p.addPar("PCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pckappas);
  p.addPar("PCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pclc);
  p.addPar("PCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pcle);
  p.addPar("PCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pclm);
  p.addPar("PD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pd);
  p.addPar("PDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdelta);
  p.addPar("PDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdiblc1);
  p.addPar("PDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdiblc2);
  p.addPar("PDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdiblcb);
  p.addPar("PDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdits);
  p.addPar("PDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pditsd);
  p.addPar("PDITSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pditsl);
  p.addPar("PDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdrout);
  p.addPar("PDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdsub);
  p.addPar("PDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt0);
  p.addPar("PDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt0w);
  p.addPar("PDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt1);
  p.addPar("PDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt1w);
  p.addPar("PDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt2);
  p.addPar("PDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvt2w);
  p.addPar("PDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp0);
  p.addPar("PDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp1);
  p.addPar("PDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp2);
  p.addPar("PDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp3);
  p.addPar("PDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp4);
  p.addPar("PDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdvtp5);
  p.addPar("PDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdwb);
  p.addPar("PDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pdwg);
  p.addPar("PEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pegidl);
  p.addPar("PEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pegisl);
  p.addPar("PEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::peigbinv);
  p.addPar("PERMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::permod);
  p.addPar("PETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::peta0);
  p.addPar("PETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::petab);
  p.addPar("PEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::peu);
  p.addPar("PFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pfgidl);
  p.addPar("PFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pfgisl);
  p.addPar("PFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pfprout);
  p.addPar("PGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pgamma1);
  p.addPar("PGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pgamma2);
  p.addPar("PHIG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::phig);
  p.addPar("PHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::phin);
  p.addPar("PIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pigcd);
  p.addPar("PK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pk1);
  p.addPar("PK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pk2);
  p.addPar("PK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pk2we);
  p.addPar("PK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pk3);
  p.addPar("PK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pk3b);
  p.addPar("PKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pketa);
  p.addPar("PKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkgidl);
  p.addPar("PKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkgisl);
  p.addPar("PKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkt1);
  p.addPar("PKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkt1l);
  p.addPar("PKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkt2);
  p.addPar("PKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pku0);
  p.addPar("PKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pku0we);
  p.addPar("PKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkvth0);
  p.addPar("PKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pkvth0we);
  p.addPar("PLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::plambda);
  p.addPar("PLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::plp);
  p.addPar("PLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::plpe0);
  p.addPar("PLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::plpeb);
  p.addPar("PMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pminv);
  p.addPar("PMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pminvcv);
  p.addPar("PMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pmoin);
  p.addPar("PNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pndep);
  p.addPar("PNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnfactor);
  p.addPar("PNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pngate);
  p.addPar("PNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnigbacc);
  p.addPar("PNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnigbinv);
  p.addPar("PNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnigc);
  p.addPar("PNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnoff);
  p.addPar("PNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnsd);
  p.addPar("PNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pnsub);
  p.addPar("PNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pntox);
  p.addPar("POXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::poxedge);
  p.addPar("PPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppclm);
  p.addPar("PPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppdiblc1);
  p.addPar("PPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppdiblc2);
  p.addPar("PPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppdiblcb);
  p.addPar("PPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppdits);
  p.addPar("PPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppditsd);
  p.addPar("PPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pphin);
  p.addPar("PPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppigcd);
  p.addPar("PPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppoxedge);
  p.addPar("PPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pprt);
  p.addPar("PPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pprwb);
  p.addPar("PPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pprwg);
  p.addPar("PPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppscbe1);
  p.addPar("PPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppscbe2);
  p.addPar("PPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ppvag);
  p.addPar("PRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prdsw);
  p.addPar("PRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prdw);
  p.addPar("PRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prgidl);
  p.addPar("PRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prgisl);
  p.addPar("PRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prsw);
  p.addPar("PRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prt);
  p.addPar("PRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prwb);
  p.addPar("PRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::prwg);
  p.addPar("PS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ps);
  p.addPar("PSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pscbe1);
  p.addPar("PSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pscbe2);
  p.addPar("PTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pteta0);
  p.addPar("PTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ptnfactor);
  p.addPar("PTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ptvfbsdoff);
  p.addPar("PTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ptvoff);
  p.addPar("PTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ptvoffcv);
  p.addPar("PU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pu0);
  p.addPar("PUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pua);
  p.addPar("PUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pua1);
  p.addPar("PUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pub);
  p.addPar("PUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pub1);
  p.addPar("PUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::puc);
  p.addPar("PUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::puc1);
  p.addPar("PUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pucs);
  p.addPar("PUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pucste);
  p.addPar("PUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pud);
  p.addPar("PUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pud1);
  p.addPar("PUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pup);
  p.addPar("PUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pute);
  p.addPar("PVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvag);
  p.addPar("PVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvbm);
  p.addPar("PVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvbx);
  p.addPar("PVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvfb);
  p.addPar("PVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvfbcv);
  p.addPar("PVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvfbsdoff);
  p.addPar("PVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvoff);
  p.addPar("PVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvoffcv);
  p.addPar("PVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvsat);
  p.addPar("PVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvth0);
  p.addPar("PVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pvtl);
  p.addPar("PW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pw0);
  p.addPar("PWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pwr);
  p.addPar("PXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pxj);
  p.addPar("PXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pxn);
  p.addPar("PXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pxrcrg1);
  p.addPar("PXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pxrcrg2);
  p.addPar("PXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::pxt);
  p.addPar("RBDB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbdb);
  p.addPar("RBDBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbdbx0);
  p.addPar("RBDBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbdby0);
  p.addPar("RBODYMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::rbodymod);
  p.addPar("RBPB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpb);
  p.addPar("RBPBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbx0);
  p.addPar("RBPBXL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbxl);
  p.addPar("RBPBXNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbxnf);
  p.addPar("RBPBXW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbxw);
  p.addPar("RBPBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpby0);
  p.addPar("RBPBYL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbyl);
  p.addPar("RBPBYNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbynf);
  p.addPar("RBPBYW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpbyw);
  p.addPar("RBPD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpd);
  p.addPar("RBPD0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpd0);
  p.addPar("RBPDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpdl);
  p.addPar("RBPDNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpdnf);
  p.addPar("RBPDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpdw);
  p.addPar("RBPS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbps);
  p.addPar("RBPS0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbps0);
  p.addPar("RBPSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpsl);
  p.addPar("RBPSNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpsnf);
  p.addPar("RBPSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbpsw);
  p.addPar("RBSB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsb);
  p.addPar("RBSBX0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsbx0);
  p.addPar("RBSBY0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsby0);
  p.addPar("RBSDBXL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbxl);
  p.addPar("RBSDBXNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbxnf);
  p.addPar("RBSDBXW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbxw);
  p.addPar("RBSDBYL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbyl);
  p.addPar("RBSDBYNF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbynf);
  p.addPar("RBSDBYW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rbsdbyw);
  p.addPar("RDSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::rdsmod);
  p.addPar("RDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rdsw);
  p.addPar("RDSWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rdswmin);
  p.addPar("RDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rdw);
  p.addPar("RDWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rdwmin);
  p.addPar("RGATEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::rgatemod);
  p.addPar("RGEOMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::rgeomod);
  p.addPar("RGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rgidl);
  p.addPar("RGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rgisl);
  p.addPar("RNOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rnoia);
  p.addPar("RNOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rnoib);
  p.addPar("RNOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rnoic);
  p.addPar("RSH",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rsh);
  p.addPar("RSHG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rshg);
  p.addPar("RSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rsw);
  p.addPar("RSWMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::rswmin);
  p.addPar("SA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sa);
  p.addPar("SAREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::saref);
  p.addPar("SB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sb);
  p.addPar("SBREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sbref);
  p.addPar("SC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sc);
  p.addPar("SCA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sca);
  p.addPar("SCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::scb);
  p.addPar("SCC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::scc);
  p.addPar("SCREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::scref);
  p.addPar("SD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::sd);
  p.addPar("STETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::steta0);
  p.addPar("STK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::stk2);
  p.addPar("TBGASUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tbgasub);
  p.addPar("TBGBSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tbgbsub);
  p.addPar("TCJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tcj);
  p.addPar("TCJSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tcjsw);
  p.addPar("TCJSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tcjswg);
  p.addPar("TEMPEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tempeot);
  p.addPar("TEMPMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::tempmod);
  p.addPar("TETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::teta0);
  p.addPar("TKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tku0);
  p.addPar("TNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnfactor);
  p.addPar("TNJTS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjts);
  p.addPar("TNJTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjtsd);
  p.addPar("TNJTSSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjtssw);
  p.addPar("TNJTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjtsswd);
  p.addPar("TNJTSSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjtsswg);
  p.addPar("TNJTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnjtsswgd);
  p.addPar("TNOIA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnoia);
  p.addPar("TNOIB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnoib);
  p.addPar("TNOIC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnoic);
  p.addPar("TNOIMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::tnoimod);
  p.addPar("TNOM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tnom);
  p.addPar("TOXE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::toxe);
  p.addPar("TOXM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::toxm);
  p.addPar("TOXP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::toxp);
  p.addPar("TOXREF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::toxref);
  p.addPar("TPB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tpb);
  p.addPar("TPBSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tpbsw);
  p.addPar("TPBSWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tpbswg);
  p.addPar("TRNQSMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::trnqsmod);
  p.addPar("TVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tvfbsdoff);
  p.addPar("TVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tvoff);
  p.addPar("TVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::tvoffcv);
  p.addPar("TYPE",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::type);
  p.addPar("U0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::u0);
  p.addPar("UA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ua);
  p.addPar("UA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ua1);
  p.addPar("UB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ub);
  p.addPar("UB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ub1);
  p.addPar("UC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::uc);
  p.addPar("UC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::uc1);
  p.addPar("UCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ucs);
  p.addPar("UCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ucste);
  p.addPar("UD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ud);
  p.addPar("UD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ud1);
  p.addPar("UP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::up);
  p.addPar("UTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ute);
  p.addPar("VBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vbm);
  p.addPar("VBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vbx);
  p.addPar("VDDEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vddeot);
  p.addPar("VERBOSE",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::verbose);
  p.addPar("VERSION",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::version);
  p.addPar("VFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vfb);
  p.addPar("VFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vfbcv);
  p.addPar("VFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vfbsdoff);
  p.addPar("VOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::voff);
  p.addPar("VOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::voffcv);
  p.addPar("VOFFCVL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::voffcvl);
  p.addPar("VOFFL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::voffl);
  p.addPar("VSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vsat);
  p.addPar("VTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vth0);
  p.addPar("VTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtl);
  p.addPar("VTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtsd);
  p.addPar("VTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtss);
  p.addPar("VTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtsswd);
  p.addPar("VTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtsswgd);
  p.addPar("VTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtsswgs);
  p.addPar("VTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::vtssws);
  p.addPar("W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::w);
  p.addPar("W0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::w0);
  p.addPar("WA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wa0);
  p.addPar("WA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wa1);
  p.addPar("WA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wa2);
  p.addPar("WACDE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wacde);
  p.addPar("WAGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wagidl);
  p.addPar("WAGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wagisl);
  p.addPar("WAGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wags);
  p.addPar("WAIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigbacc);
  p.addPar("WAIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigbinv);
  p.addPar("WAIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigc);
  p.addPar("WAIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigd);
  p.addPar("WAIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigs);
  p.addPar("WAIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::waigsd);
  p.addPar("WALPHA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::walpha0);
  p.addPar("WALPHA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::walpha1);
  p.addPar("WAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wat);
  p.addPar("WB0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wb0);
  p.addPar("WB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wb1);
  p.addPar("WBETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbeta0);
  p.addPar("WBGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbgidl);
  p.addPar("WBGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbgisl);
  p.addPar("WBIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigbacc);
  p.addPar("WBIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigbinv);
  p.addPar("WBIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigc);
  p.addPar("WBIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigd);
  p.addPar("WBIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigs);
  p.addPar("WBIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wbigsd);
  p.addPar("WCDSC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcdsc);
  p.addPar("WCDSCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcdscb);
  p.addPar("WCDSCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcdscd);
  p.addPar("WCF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcf);
  p.addPar("WCGDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcgdl);
  p.addPar("WCGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcgidl);
  p.addPar("WCGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcgisl);
  p.addPar("WCGSL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcgsl);
  p.addPar("WCIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigbacc);
  p.addPar("WCIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigbinv);
  p.addPar("WCIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigc);
  p.addPar("WCIGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigd);
  p.addPar("WCIGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigs);
  p.addPar("WCIGSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcigsd);
  p.addPar("WCIT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcit);
  p.addPar("WCKAPPAD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wckappad);
  p.addPar("WCKAPPAS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wckappas);
  p.addPar("WCLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wclc);
  p.addPar("WCLE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wcle);
  p.addPar("WDELTA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdelta);
  p.addPar("WDROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdrout);
  p.addPar("WDSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdsub);
  p.addPar("WDVT0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt0);
  p.addPar("WDVT0W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt0w);
  p.addPar("WDVT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt1);
  p.addPar("WDVT1W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt1w);
  p.addPar("WDVT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt2);
  p.addPar("WDVT2W",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvt2w);
  p.addPar("WDVTP0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp0);
  p.addPar("WDVTP1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp1);
  p.addPar("WDVTP2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp2);
  p.addPar("WDVTP3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp3);
  p.addPar("WDVTP4",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp4);
  p.addPar("WDVTP5",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdvtp5);
  p.addPar("WDWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdwb);
  p.addPar("WDWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wdwg);
  p.addPar("WEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::web);
  p.addPar("WEC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wec);
  p.addPar("WEFFEOT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::weffeot);
  p.addPar("WEGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wegidl);
  p.addPar("WEGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wegisl);
  p.addPar("WEIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::weigbinv);
  p.addPar("WETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::weta0);
  p.addPar("WETAB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wetab);
  p.addPar("WEU",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::weu);
  p.addPar("WFGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wfgidl);
  p.addPar("WFGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wfgisl);
  p.addPar("WFPROUT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wfprout);
  p.addPar("WGAMMA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wgamma1);
  p.addPar("WGAMMA2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wgamma2);
  p.addPar("WINT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wint);
  p.addPar("WK1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wk1);
  p.addPar("WK2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wk2);
  p.addPar("WK2WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wk2we);
  p.addPar("WK3",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wk3);
  p.addPar("WK3B",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wk3b);
  p.addPar("WKETA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wketa);
  p.addPar("WKGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkgidl);
  p.addPar("WKGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkgisl);
  p.addPar("WKT1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkt1);
  p.addPar("WKT1L",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkt1l);
  p.addPar("WKT2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkt2);
  p.addPar("WKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wku0);
  p.addPar("WKU0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wku0we);
  p.addPar("WKVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkvth0);
  p.addPar("WKVTH0WE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wkvth0we);
  p.addPar("WL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wl);
  p.addPar("WLAMBDA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlambda);
  p.addPar("WLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlc);
  p.addPar("WLN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wln);
  p.addPar("WLOD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlod);
  p.addPar("WLODKU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlodku0);
  p.addPar("WLODVTH",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlodvth);
  p.addPar("WLP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlp);
  p.addPar("WLPE0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlpe0);
  p.addPar("WLPEB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wlpeb);
  p.addPar("WMAX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wmax);
  p.addPar("WMIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wmin);
  p.addPar("WMINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wminv);
  p.addPar("WMINVCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wminvcv);
  p.addPar("WMOIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wmoin);
  p.addPar("WNDEP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wndep);
  p.addPar("WNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnfactor);
  p.addPar("WNGATE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wngate);
  p.addPar("WNIGBACC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnigbacc);
  p.addPar("WNIGBINV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnigbinv);
  p.addPar("WNIGC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnigc);
  p.addPar("WNOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnoff);
  p.addPar("WNSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnsd);
  p.addPar("WNSUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wnsub);
  p.addPar("WNTOX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wntox);
  p.addPar("WPCLM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpclm);
  p.addPar("WPDIBLC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpdiblc1);
  p.addPar("WPDIBLC2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpdiblc2);
  p.addPar("WPDIBLCB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpdiblcb);
  p.addPar("WPDITS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpdits);
  p.addPar("WPDITSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpditsd);
  p.addPar("WPEMOD",static_cast<int>(-9999999), &COGENDAbsim4_va::Model::wpemod);
  p.addPar("WPHIN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wphin);
  p.addPar("WPIGCD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpigcd);
  p.addPar("WPOXEDGE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpoxedge);
  p.addPar("WPRT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wprt);
  p.addPar("WPRWB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wprwb);
  p.addPar("WPRWG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wprwg);
  p.addPar("WPSCBE1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpscbe1);
  p.addPar("WPSCBE2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpscbe2);
  p.addPar("WPVAG",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wpvag);
  p.addPar("WR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wr);
  p.addPar("WRDSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wrdsw);
  p.addPar("WRDW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wrdw);
  p.addPar("WRGIDL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wrgidl);
  p.addPar("WRGISL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wrgisl);
  p.addPar("WRSW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wrsw);
  p.addPar("WTETA0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wteta0);
  p.addPar("WTNFACTOR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wtnfactor);
  p.addPar("WTVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wtvfbsdoff);
  p.addPar("WTVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wtvoff);
  p.addPar("WTVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wtvoffcv);
  p.addPar("WU0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wu0);
  p.addPar("WUA",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wua);
  p.addPar("WUA1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wua1);
  p.addPar("WUB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wub);
  p.addPar("WUB1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wub1);
  p.addPar("WUC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wuc);
  p.addPar("WUC1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wuc1);
  p.addPar("WUCS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wucs);
  p.addPar("WUCSTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wucste);
  p.addPar("WUD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wud);
  p.addPar("WUD1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wud1);
  p.addPar("WUP",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wup);
  p.addPar("WUTE",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wute);
  p.addPar("WVBM",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvbm);
  p.addPar("WVBX",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvbx);
  p.addPar("WVFB",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvfb);
  p.addPar("WVFBCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvfbcv);
  p.addPar("WVFBSDOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvfbsdoff);
  p.addPar("WVOFF",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvoff);
  p.addPar("WVOFFCV",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvoffcv);
  p.addPar("WVSAT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvsat);
  p.addPar("WVTH0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvth0);
  p.addPar("WVTL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wvtl);
  p.addPar("WW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ww);
  p.addPar("WW0",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::ww0);
  p.addPar("WWC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wwc);
  p.addPar("WWL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wwl);
  p.addPar("WWLC",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wwlc);
  p.addPar("WWN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wwn);
  p.addPar("WWR",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wwr);
  p.addPar("WXJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wxj);
  p.addPar("WXN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wxn);
  p.addPar("WXRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wxrcrg1);
  p.addPar("WXRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wxrcrg2);
  p.addPar("WXT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::wxt);
  p.addPar("XGL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xgl);
  p.addPar("XGW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xgw);
  p.addPar("XJ",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xj);
  p.addPar("XJBVD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xjbvd);
  p.addPar("XJBVS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xjbvs);
  p.addPar("XL",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xl);
  p.addPar("XN",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xn);
  p.addPar("XPART",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xpart);
  p.addPar("XRCRG1",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xrcrg1);
  p.addPar("XRCRG2",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xrcrg2);
  p.addPar("XT",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xt);
  p.addPar("XTID",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtid);
  p.addPar("XTIS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtis);
  p.addPar("XTSD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtsd);
  p.addPar("XTSS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtss);
  p.addPar("XTSSWD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtsswd);
  p.addPar("XTSSWGD",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtsswgd);
  p.addPar("XTSSWGS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtsswgs);
  p.addPar("XTSSWS",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xtssws);
  p.addPar("XW",static_cast<double>(-12345789), &COGENDAbsim4_va::Model::xw);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("_CKT_GMIN"))
    _ckt_gmin = model_._ckt_gmin;
  if(!given("_MIN"))
    _min = model_._min;
  if(!given("A0"))
    a0 = model_.a0;
  if(!given("A1"))
    a1 = model_.a1;
  if(!given("A2"))
    a2 = model_.a2;
  if(!given("ACDE"))
    acde = model_.acde;
  if(!given("ACNQSMOD"))
    acnqsmod = model_.acnqsmod;
  if(!given("AD"))
    ad = model_.ad;
  if(!given("ADOS"))
    ados = model_.ados;
  if(!given("AF"))
    af = model_.af;
  if(!given("AGIDL"))
    agidl = model_.agidl;
  if(!given("AGISL"))
    agisl = model_.agisl;
  if(!given("AGS"))
    ags = model_.ags;
  if(!given("AIGBACC"))
    aigbacc = model_.aigbacc;
  if(!given("AIGBINV"))
    aigbinv = model_.aigbinv;
  if(!given("AIGC"))
    aigc = model_.aigc;
  if(!given("AIGD"))
    aigd = model_.aigd;
  if(!given("AIGS"))
    aigs = model_.aigs;
  if(!given("AIGSD"))
    aigsd = model_.aigsd;
  if(!given("ALPHA0"))
    alpha0 = model_.alpha0;
  if(!given("ALPHA1"))
    alpha1 = model_.alpha1;
  if(!given("AS"))
    as = model_.as;
  if(!given("AT"))
    at = model_.at;
  if(!given("B0"))
    b0 = model_.b0;
  if(!given("B1"))
    b1 = model_.b1;
  if(!given("BDOS"))
    bdos = model_.bdos;
  if(!given("BETA0"))
    beta0 = model_.beta0;
  if(!given("BG0SUB"))
    bg0sub = model_.bg0sub;
  if(!given("BGIDL"))
    bgidl = model_.bgidl;
  if(!given("BGISL"))
    bgisl = model_.bgisl;
  if(!given("BIGBACC"))
    bigbacc = model_.bigbacc;
  if(!given("BIGBINV"))
    bigbinv = model_.bigbinv;
  if(!given("BIGC"))
    bigc = model_.bigc;
  if(!given("BIGD"))
    bigd = model_.bigd;
  if(!given("BIGS"))
    bigs = model_.bigs;
  if(!given("BIGSD"))
    bigsd = model_.bigsd;
  if(!given("BINUNIT"))
    binunit = model_.binunit;
  if(!given("BVD"))
    bvd = model_.bvd;
  if(!given("BVS"))
    bvs = model_.bvs;
  if(!given("CAPMOD"))
    capmod = model_.capmod;
  if(!given("CDSC"))
    cdsc = model_.cdsc;
  if(!given("CDSCB"))
    cdscb = model_.cdscb;
  if(!given("CDSCD"))
    cdscd = model_.cdscd;
  if(!given("CF"))
    cf = model_.cf;
  if(!given("CGBO"))
    cgbo = model_.cgbo;
  if(!given("CGDL"))
    cgdl = model_.cgdl;
  if(!given("CGDO"))
    cgdo = model_.cgdo;
  if(!given("CGIDL"))
    cgidl = model_.cgidl;
  if(!given("CGISL"))
    cgisl = model_.cgisl;
  if(!given("CGSL"))
    cgsl = model_.cgsl;
  if(!given("CGSO"))
    cgso = model_.cgso;
  if(!given("CIGBACC"))
    cigbacc = model_.cigbacc;
  if(!given("CIGBINV"))
    cigbinv = model_.cigbinv;
  if(!given("CIGC"))
    cigc = model_.cigc;
  if(!given("CIGD"))
    cigd = model_.cigd;
  if(!given("CIGS"))
    cigs = model_.cigs;
  if(!given("CIGSD"))
    cigsd = model_.cigsd;
  if(!given("CIT"))
    cit = model_.cit;
  if(!given("CJD"))
    cjd = model_.cjd;
  if(!given("CJS"))
    cjs = model_.cjs;
  if(!given("CJSWD"))
    cjswd = model_.cjswd;
  if(!given("CJSWGD"))
    cjswgd = model_.cjswgd;
  if(!given("CJSWGS"))
    cjswgs = model_.cjswgs;
  if(!given("CJSWS"))
    cjsws = model_.cjsws;
  if(!given("CKAPPAD"))
    ckappad = model_.ckappad;
  if(!given("CKAPPAS"))
    ckappas = model_.ckappas;
  if(!given("CLC"))
    clc = model_.clc;
  if(!given("CLE"))
    cle = model_.cle;
  if(!given("CVCHARGEMOD"))
    cvchargemod = model_.cvchargemod;
  if(!given("DELTA"))
    delta = model_.delta;
  if(!given("DELVTO"))
    delvto = model_.delvto;
  if(!given("DIOMOD"))
    diomod = model_.diomod;
  if(!given("DLC"))
    dlc = model_.dlc;
  if(!given("DLCIG"))
    dlcig = model_.dlcig;
  if(!given("DLCIGD"))
    dlcigd = model_.dlcigd;
  if(!given("DMCG"))
    dmcg = model_.dmcg;
  if(!given("DMCGT"))
    dmcgt = model_.dmcgt;
  if(!given("DMCI"))
    dmci = model_.dmci;
  if(!given("DMDG"))
    dmdg = model_.dmdg;
  if(!given("DROUT"))
    drout = model_.drout;
  if(!given("DSUB"))
    dsub = model_.dsub;
  if(!given("DTOX"))
    dtox = model_.dtox;
  if(!given("DVT0"))
    dvt0 = model_.dvt0;
  if(!given("DVT0W"))
    dvt0w = model_.dvt0w;
  if(!given("DVT1"))
    dvt1 = model_.dvt1;
  if(!given("DVT1W"))
    dvt1w = model_.dvt1w;
  if(!given("DVT2"))
    dvt2 = model_.dvt2;
  if(!given("DVT2W"))
    dvt2w = model_.dvt2w;
  if(!given("DVTP0"))
    dvtp0 = model_.dvtp0;
  if(!given("DVTP1"))
    dvtp1 = model_.dvtp1;
  if(!given("DVTP2"))
    dvtp2 = model_.dvtp2;
  if(!given("DVTP3"))
    dvtp3 = model_.dvtp3;
  if(!given("DVTP4"))
    dvtp4 = model_.dvtp4;
  if(!given("DVTP5"))
    dvtp5 = model_.dvtp5;
  if(!given("DWB"))
    dwb = model_.dwb;
  if(!given("DWC"))
    dwc = model_.dwc;
  if(!given("DWG"))
    dwg = model_.dwg;
  if(!given("DWJ"))
    dwj = model_.dwj;
  if(!given("EASUB"))
    easub = model_.easub;
  if(!given("EF"))
    ef = model_.ef;
  if(!given("EGIDL"))
    egidl = model_.egidl;
  if(!given("EGISL"))
    egisl = model_.egisl;
  if(!given("EIGBINV"))
    eigbinv = model_.eigbinv;
  if(!given("EM"))
    em = model_.em;
  if(!given("EOT"))
    eot = model_.eot;
  if(!given("EPSRGATE"))
    epsrgate = model_.epsrgate;
  if(!given("EPSROX"))
    epsrox = model_.epsrox;
  if(!given("EPSRSUB"))
    epsrsub = model_.epsrsub;
  if(!given("ETA0"))
    eta0 = model_.eta0;
  if(!given("ETAB"))
    etab = model_.etab;
  if(!given("EU"))
    eu = model_.eu;
  if(!given("FGIDL"))
    fgidl = model_.fgidl;
  if(!given("FGISL"))
    fgisl = model_.fgisl;
  if(!given("FNOIMOD"))
    fnoimod = model_.fnoimod;
  if(!given("FPROUT"))
    fprout = model_.fprout;
  if(!given("GAMMA1"))
    gamma1 = model_.gamma1;
  if(!given("GAMMA2"))
    gamma2 = model_.gamma2;
  if(!given("GBMIN"))
    gbmin = model_.gbmin;
  if(!given("GEOMOD"))
    geomod = model_.geomod;
  if(!given("GIDLMOD"))
    gidlmod = model_.gidlmod;
  if(!given("IGBMOD"))
    igbmod = model_.igbmod;
  if(!given("IGCMOD"))
    igcmod = model_.igcmod;
  if(!given("IJTHDFWD"))
    ijthdfwd = model_.ijthdfwd;
  if(!given("IJTHDREV"))
    ijthdrev = model_.ijthdrev;
  if(!given("IJTHSFWD"))
    ijthsfwd = model_.ijthsfwd;
  if(!given("IJTHSREV"))
    ijthsrev = model_.ijthsrev;
  if(!given("JSD"))
    jsd = model_.jsd;
  if(!given("JSS"))
    jss = model_.jss;
  if(!given("JSWD"))
    jswd = model_.jswd;
  if(!given("JSWGD"))
    jswgd = model_.jswgd;
  if(!given("JSWGS"))
    jswgs = model_.jswgs;
  if(!given("JSWS"))
    jsws = model_.jsws;
  if(!given("JTSD"))
    jtsd = model_.jtsd;
  if(!given("JTSS"))
    jtss = model_.jtss;
  if(!given("JTSSWD"))
    jtsswd = model_.jtsswd;
  if(!given("JTSSWGD"))
    jtsswgd = model_.jtsswgd;
  if(!given("JTSSWGS"))
    jtsswgs = model_.jtsswgs;
  if(!given("JTSSWS"))
    jtssws = model_.jtssws;
  if(!given("JTWEFF"))
    jtweff = model_.jtweff;
  if(!given("K1"))
    k1 = model_.k1;
  if(!given("K2"))
    k2 = model_.k2;
  if(!given("K2WE"))
    k2we = model_.k2we;
  if(!given("K3"))
    k3 = model_.k3;
  if(!given("K3B"))
    k3b = model_.k3b;
  if(!given("KETA"))
    keta = model_.keta;
  if(!given("KF"))
    kf = model_.kf;
  if(!given("KGIDL"))
    kgidl = model_.kgidl;
  if(!given("KGISL"))
    kgisl = model_.kgisl;
  if(!given("KT1"))
    kt1 = model_.kt1;
  if(!given("KT1L"))
    kt1l = model_.kt1l;
  if(!given("KT2"))
    kt2 = model_.kt2;
  if(!given("KU0"))
    ku0 = model_.ku0;
  if(!given("KU0WE"))
    ku0we = model_.ku0we;
  if(!given("KVSAT"))
    kvsat = model_.kvsat;
  if(!given("KVTH0"))
    kvth0 = model_.kvth0;
  if(!given("KVTH0WE"))
    kvth0we = model_.kvth0we;
  if(!given("L"))
    l = model_.l;
  if(!given("LA0"))
    la0 = model_.la0;
  if(!given("LA1"))
    la1 = model_.la1;
  if(!given("LA2"))
    la2 = model_.la2;
  if(!given("LACDE"))
    lacde = model_.lacde;
  if(!given("LAGIDL"))
    lagidl = model_.lagidl;
  if(!given("LAGISL"))
    lagisl = model_.lagisl;
  if(!given("LAGS"))
    lags = model_.lags;
  if(!given("LAIGBACC"))
    laigbacc = model_.laigbacc;
  if(!given("LAIGBINV"))
    laigbinv = model_.laigbinv;
  if(!given("LAIGC"))
    laigc = model_.laigc;
  if(!given("LAIGD"))
    laigd = model_.laigd;
  if(!given("LAIGS"))
    laigs = model_.laigs;
  if(!given("LAIGSD"))
    laigsd = model_.laigsd;
  if(!given("LALPHA0"))
    lalpha0 = model_.lalpha0;
  if(!given("LALPHA1"))
    lalpha1 = model_.lalpha1;
  if(!given("LAMBDA"))
    lambda = model_.lambda;
  if(!given("LAT"))
    lat = model_.lat;
  if(!given("LB0"))
    lb0 = model_.lb0;
  if(!given("LB1"))
    lb1 = model_.lb1;
  if(!given("LBETA0"))
    lbeta0 = model_.lbeta0;
  if(!given("LBGIDL"))
    lbgidl = model_.lbgidl;
  if(!given("LBGISL"))
    lbgisl = model_.lbgisl;
  if(!given("LBIGBACC"))
    lbigbacc = model_.lbigbacc;
  if(!given("LBIGBINV"))
    lbigbinv = model_.lbigbinv;
  if(!given("LBIGC"))
    lbigc = model_.lbigc;
  if(!given("LBIGD"))
    lbigd = model_.lbigd;
  if(!given("LBIGS"))
    lbigs = model_.lbigs;
  if(!given("LBIGSD"))
    lbigsd = model_.lbigsd;
  if(!given("LC"))
    lc = model_.lc;
  if(!given("LCDSC"))
    lcdsc = model_.lcdsc;
  if(!given("LCDSCB"))
    lcdscb = model_.lcdscb;
  if(!given("LCDSCD"))
    lcdscd = model_.lcdscd;
  if(!given("LCF"))
    lcf = model_.lcf;
  if(!given("LCGDL"))
    lcgdl = model_.lcgdl;
  if(!given("LCGIDL"))
    lcgidl = model_.lcgidl;
  if(!given("LCGISL"))
    lcgisl = model_.lcgisl;
  if(!given("LCGSL"))
    lcgsl = model_.lcgsl;
  if(!given("LCIGBACC"))
    lcigbacc = model_.lcigbacc;
  if(!given("LCIGBINV"))
    lcigbinv = model_.lcigbinv;
  if(!given("LCIGC"))
    lcigc = model_.lcigc;
  if(!given("LCIGD"))
    lcigd = model_.lcigd;
  if(!given("LCIGS"))
    lcigs = model_.lcigs;
  if(!given("LCIGSD"))
    lcigsd = model_.lcigsd;
  if(!given("LCIT"))
    lcit = model_.lcit;
  if(!given("LCKAPPAD"))
    lckappad = model_.lckappad;
  if(!given("LCKAPPAS"))
    lckappas = model_.lckappas;
  if(!given("LCLC"))
    lclc = model_.lclc;
  if(!given("LCLE"))
    lcle = model_.lcle;
  if(!given("LDELTA"))
    ldelta = model_.ldelta;
  if(!given("LDROUT"))
    ldrout = model_.ldrout;
  if(!given("LDSUB"))
    ldsub = model_.ldsub;
  if(!given("LDVT0"))
    ldvt0 = model_.ldvt0;
  if(!given("LDVT0W"))
    ldvt0w = model_.ldvt0w;
  if(!given("LDVT1"))
    ldvt1 = model_.ldvt1;
  if(!given("LDVT1W"))
    ldvt1w = model_.ldvt1w;
  if(!given("LDVT2"))
    ldvt2 = model_.ldvt2;
  if(!given("LDVT2W"))
    ldvt2w = model_.ldvt2w;
  if(!given("LDVTP0"))
    ldvtp0 = model_.ldvtp0;
  if(!given("LDVTP1"))
    ldvtp1 = model_.ldvtp1;
  if(!given("LDVTP2"))
    ldvtp2 = model_.ldvtp2;
  if(!given("LDVTP3"))
    ldvtp3 = model_.ldvtp3;
  if(!given("LDVTP4"))
    ldvtp4 = model_.ldvtp4;
  if(!given("LDVTP5"))
    ldvtp5 = model_.ldvtp5;
  if(!given("LDWB"))
    ldwb = model_.ldwb;
  if(!given("LDWG"))
    ldwg = model_.ldwg;
  if(!given("LEFFEOT"))
    leffeot = model_.leffeot;
  if(!given("LEGIDL"))
    legidl = model_.legidl;
  if(!given("LEGISL"))
    legisl = model_.legisl;
  if(!given("LEIGBINV"))
    leigbinv = model_.leigbinv;
  if(!given("LETA0"))
    leta0 = model_.leta0;
  if(!given("LETAB"))
    letab = model_.letab;
  if(!given("LEU"))
    leu = model_.leu;
  if(!given("LEVEL"))
    level = model_.level;
  if(!given("LFGIDL"))
    lfgidl = model_.lfgidl;
  if(!given("LFGISL"))
    lfgisl = model_.lfgisl;
  if(!given("LFPROUT"))
    lfprout = model_.lfprout;
  if(!given("LGAMMA1"))
    lgamma1 = model_.lgamma1;
  if(!given("LGAMMA2"))
    lgamma2 = model_.lgamma2;
  if(!given("LINT"))
    lint = model_.lint;
  if(!given("LK1"))
    lk1 = model_.lk1;
  if(!given("LK2"))
    lk2 = model_.lk2;
  if(!given("LK2WE"))
    lk2we = model_.lk2we;
  if(!given("LK3"))
    lk3 = model_.lk3;
  if(!given("LK3B"))
    lk3b = model_.lk3b;
  if(!given("LKETA"))
    lketa = model_.lketa;
  if(!given("LKGIDL"))
    lkgidl = model_.lkgidl;
  if(!given("LKGISL"))
    lkgisl = model_.lkgisl;
  if(!given("LKT1"))
    lkt1 = model_.lkt1;
  if(!given("LKT1L"))
    lkt1l = model_.lkt1l;
  if(!given("LKT2"))
    lkt2 = model_.lkt2;
  if(!given("LKU0"))
    lku0 = model_.lku0;
  if(!given("LKU0WE"))
    lku0we = model_.lku0we;
  if(!given("LKVTH0"))
    lkvth0 = model_.lkvth0;
  if(!given("LKVTH0WE"))
    lkvth0we = model_.lkvth0we;
  if(!given("LL"))
    ll = model_.ll;
  if(!given("LLAMBDA"))
    llambda = model_.llambda;
  if(!given("LLC"))
    llc = model_.llc;
  if(!given("LLN"))
    lln = model_.lln;
  if(!given("LLODKU0"))
    llodku0 = model_.llodku0;
  if(!given("LLODVTH"))
    llodvth = model_.llodvth;
  if(!given("LLP"))
    llp = model_.llp;
  if(!given("LLPE0"))
    llpe0 = model_.llpe0;
  if(!given("LLPEB"))
    llpeb = model_.llpeb;
  if(!given("LMAX"))
    lmax = model_.lmax;
  if(!given("LMIN"))
    lmin = model_.lmin;
  if(!given("LMINV"))
    lminv = model_.lminv;
  if(!given("LMINVCV"))
    lminvcv = model_.lminvcv;
  if(!given("LMOIN"))
    lmoin = model_.lmoin;
  if(!given("LNDEP"))
    lndep = model_.lndep;
  if(!given("LNFACTOR"))
    lnfactor = model_.lnfactor;
  if(!given("LNGATE"))
    lngate = model_.lngate;
  if(!given("LNIGBACC"))
    lnigbacc = model_.lnigbacc;
  if(!given("LNIGBINV"))
    lnigbinv = model_.lnigbinv;
  if(!given("LNIGC"))
    lnigc = model_.lnigc;
  if(!given("LNOFF"))
    lnoff = model_.lnoff;
  if(!given("LNSD"))
    lnsd = model_.lnsd;
  if(!given("LNSUB"))
    lnsub = model_.lnsub;
  if(!given("LNTOX"))
    lntox = model_.lntox;
  if(!given("LODETA0"))
    lodeta0 = model_.lodeta0;
  if(!given("LODK2"))
    lodk2 = model_.lodk2;
  if(!given("LP"))
    lp = model_.lp;
  if(!given("LPCLM"))
    lpclm = model_.lpclm;
  if(!given("LPDIBLC1"))
    lpdiblc1 = model_.lpdiblc1;
  if(!given("LPDIBLC2"))
    lpdiblc2 = model_.lpdiblc2;
  if(!given("LPDIBLCB"))
    lpdiblcb = model_.lpdiblcb;
  if(!given("LPDITS"))
    lpdits = model_.lpdits;
  if(!given("LPDITSD"))
    lpditsd = model_.lpditsd;
  if(!given("LPE0"))
    lpe0 = model_.lpe0;
  if(!given("LPEB"))
    lpeb = model_.lpeb;
  if(!given("LPHIN"))
    lphin = model_.lphin;
  if(!given("LPIGCD"))
    lpigcd = model_.lpigcd;
  if(!given("LPOXEDGE"))
    lpoxedge = model_.lpoxedge;
  if(!given("LPRT"))
    lprt = model_.lprt;
  if(!given("LPRWB"))
    lprwb = model_.lprwb;
  if(!given("LPRWG"))
    lprwg = model_.lprwg;
  if(!given("LPSCBE1"))
    lpscbe1 = model_.lpscbe1;
  if(!given("LPSCBE2"))
    lpscbe2 = model_.lpscbe2;
  if(!given("LPVAG"))
    lpvag = model_.lpvag;
  if(!given("LRDSW"))
    lrdsw = model_.lrdsw;
  if(!given("LRDW"))
    lrdw = model_.lrdw;
  if(!given("LRGIDL"))
    lrgidl = model_.lrgidl;
  if(!given("LRGISL"))
    lrgisl = model_.lrgisl;
  if(!given("LRSW"))
    lrsw = model_.lrsw;
  if(!given("LTETA0"))
    lteta0 = model_.lteta0;
  if(!given("LTNFACTOR"))
    ltnfactor = model_.ltnfactor;
  if(!given("LTVFBSDOFF"))
    ltvfbsdoff = model_.ltvfbsdoff;
  if(!given("LTVOFF"))
    ltvoff = model_.ltvoff;
  if(!given("LTVOFFCV"))
    ltvoffcv = model_.ltvoffcv;
  if(!given("LU0"))
    lu0 = model_.lu0;
  if(!given("LUA"))
    lua = model_.lua;
  if(!given("LUA1"))
    lua1 = model_.lua1;
  if(!given("LUB"))
    lub = model_.lub;
  if(!given("LUB1"))
    lub1 = model_.lub1;
  if(!given("LUC"))
    luc = model_.luc;
  if(!given("LUC1"))
    luc1 = model_.luc1;
  if(!given("LUCS"))
    lucs = model_.lucs;
  if(!given("LUCSTE"))
    lucste = model_.lucste;
  if(!given("LUD"))
    lud = model_.lud;
  if(!given("LUD1"))
    lud1 = model_.lud1;
  if(!given("LUP"))
    lup = model_.lup;
  if(!given("LUTE"))
    lute = model_.lute;
  if(!given("LVBM"))
    lvbm = model_.lvbm;
  if(!given("LVBX"))
    lvbx = model_.lvbx;
  if(!given("LVFB"))
    lvfb = model_.lvfb;
  if(!given("LVFBCV"))
    lvfbcv = model_.lvfbcv;
  if(!given("LVFBSDOFF"))
    lvfbsdoff = model_.lvfbsdoff;
  if(!given("LVOFF"))
    lvoff = model_.lvoff;
  if(!given("LVOFFCV"))
    lvoffcv = model_.lvoffcv;
  if(!given("LVSAT"))
    lvsat = model_.lvsat;
  if(!given("LVTH0"))
    lvth0 = model_.lvth0;
  if(!given("LVTL"))
    lvtl = model_.lvtl;
  if(!given("LW"))
    lw = model_.lw;
  if(!given("LW0"))
    lw0 = model_.lw0;
  if(!given("LWC"))
    lwc = model_.lwc;
  if(!given("LWL"))
    lwl = model_.lwl;
  if(!given("LWLC"))
    lwlc = model_.lwlc;
  if(!given("LWN"))
    lwn = model_.lwn;
  if(!given("LWR"))
    lwr = model_.lwr;
  if(!given("LXJ"))
    lxj = model_.lxj;
  if(!given("LXN"))
    lxn = model_.lxn;
  if(!given("LXRCRG1"))
    lxrcrg1 = model_.lxrcrg1;
  if(!given("LXRCRG2"))
    lxrcrg2 = model_.lxrcrg2;
  if(!given("LXT"))
    lxt = model_.lxt;
  if(!given("MINV"))
    minv = model_.minv;
  if(!given("MINVCV"))
    minvcv = model_.minvcv;
  if(!given("MJD"))
    mjd = model_.mjd;
  if(!given("MJS"))
    mjs = model_.mjs;
  if(!given("MJSWD"))
    mjswd = model_.mjswd;
  if(!given("MJSWGD"))
    mjswgd = model_.mjswgd;
  if(!given("MJSWGS"))
    mjswgs = model_.mjswgs;
  if(!given("MJSWS"))
    mjsws = model_.mjsws;
  if(!given("MOBMOD"))
    mobmod = model_.mobmod;
  if(!given("MOIN"))
    moin = model_.moin;
  if(!given("MTRLCOMPATMOD"))
    mtrlcompatmod = model_.mtrlcompatmod;
  if(!given("MTRLMOD"))
    mtrlmod = model_.mtrlmod;
  if(!given("NDEP"))
    ndep = model_.ndep;
  if(!given("NF"))
    nf = model_.nf;
  if(!given("NFACTOR"))
    nfactor = model_.nfactor;
  if(!given("NGATE"))
    ngate = model_.ngate;
  if(!given("NGCON"))
    ngcon = model_.ngcon;
  if(!given("NI0SUB"))
    ni0sub = model_.ni0sub;
  if(!given("NIGBACC"))
    nigbacc = model_.nigbacc;
  if(!given("NIGBINV"))
    nigbinv = model_.nigbinv;
  if(!given("NIGC"))
    nigc = model_.nigc;
  if(!given("NJD"))
    njd = model_.njd;
  if(!given("NJS"))
    njs = model_.njs;
  if(!given("NJTS"))
    njts = model_.njts;
  if(!given("NJTSD"))
    njtsd = model_.njtsd;
  if(!given("NJTSSW"))
    njtssw = model_.njtssw;
  if(!given("NJTSSWD"))
    njtsswd = model_.njtsswd;
  if(!given("NJTSSWG"))
    njtsswg = model_.njtsswg;
  if(!given("NJTSSWGD"))
    njtsswgd = model_.njtsswgd;
  if(!given("NOFF"))
    noff = model_.noff;
  if(!given("NOIA"))
    noia = model_.noia;
  if(!given("NOIB"))
    noib = model_.noib;
  if(!given("NOIC"))
    noic = model_.noic;
  if(!given("NRD"))
    nrd = model_.nrd;
  if(!given("NRS"))
    nrs = model_.nrs;
  if(!given("NSD"))
    nsd = model_.nsd;
  if(!given("NSUB"))
    nsub = model_.nsub;
  if(!given("NTNOI"))
    ntnoi = model_.ntnoi;
  if(!given("NTOX"))
    ntox = model_.ntox;
  if(!given("OFF"))
    off = model_.off;
  if(!given("PA0"))
    pa0 = model_.pa0;
  if(!given("PA1"))
    pa1 = model_.pa1;
  if(!given("PA2"))
    pa2 = model_.pa2;
  if(!given("PACDE"))
    pacde = model_.pacde;
  if(!given("PAGIDL"))
    pagidl = model_.pagidl;
  if(!given("PAGISL"))
    pagisl = model_.pagisl;
  if(!given("PAGS"))
    pags = model_.pags;
  if(!given("PAIGBACC"))
    paigbacc = model_.paigbacc;
  if(!given("PAIGBINV"))
    paigbinv = model_.paigbinv;
  if(!given("PAIGC"))
    paigc = model_.paigc;
  if(!given("PAIGD"))
    paigd = model_.paigd;
  if(!given("PAIGS"))
    paigs = model_.paigs;
  if(!given("PAIGSD"))
    paigsd = model_.paigsd;
  if(!given("PALPHA0"))
    palpha0 = model_.palpha0;
  if(!given("PALPHA1"))
    palpha1 = model_.palpha1;
  if(!given("PARAMCHK"))
    paramchk = model_.paramchk;
  if(!given("PAT"))
    pat = model_.pat;
  if(!given("PB0"))
    pb0 = model_.pb0;
  if(!given("PB1"))
    pb1 = model_.pb1;
  if(!given("PBD"))
    pbd = model_.pbd;
  if(!given("PBETA0"))
    pbeta0 = model_.pbeta0;
  if(!given("PBGIDL"))
    pbgidl = model_.pbgidl;
  if(!given("PBGISL"))
    pbgisl = model_.pbgisl;
  if(!given("PBIGBACC"))
    pbigbacc = model_.pbigbacc;
  if(!given("PBIGBINV"))
    pbigbinv = model_.pbigbinv;
  if(!given("PBIGC"))
    pbigc = model_.pbigc;
  if(!given("PBIGD"))
    pbigd = model_.pbigd;
  if(!given("PBIGS"))
    pbigs = model_.pbigs;
  if(!given("PBIGSD"))
    pbigsd = model_.pbigsd;
  if(!given("PBS"))
    pbs = model_.pbs;
  if(!given("PBSWD"))
    pbswd = model_.pbswd;
  if(!given("PBSWGD"))
    pbswgd = model_.pbswgd;
  if(!given("PBSWGS"))
    pbswgs = model_.pbswgs;
  if(!given("PBSWS"))
    pbsws = model_.pbsws;
  if(!given("PCDSC"))
    pcdsc = model_.pcdsc;
  if(!given("PCDSCB"))
    pcdscb = model_.pcdscb;
  if(!given("PCDSCD"))
    pcdscd = model_.pcdscd;
  if(!given("PCF"))
    pcf = model_.pcf;
  if(!given("PCGDL"))
    pcgdl = model_.pcgdl;
  if(!given("PCGIDL"))
    pcgidl = model_.pcgidl;
  if(!given("PCGISL"))
    pcgisl = model_.pcgisl;
  if(!given("PCGSL"))
    pcgsl = model_.pcgsl;
  if(!given("PCIGBACC"))
    pcigbacc = model_.pcigbacc;
  if(!given("PCIGBINV"))
    pcigbinv = model_.pcigbinv;
  if(!given("PCIGC"))
    pcigc = model_.pcigc;
  if(!given("PCIGD"))
    pcigd = model_.pcigd;
  if(!given("PCIGS"))
    pcigs = model_.pcigs;
  if(!given("PCIGSD"))
    pcigsd = model_.pcigsd;
  if(!given("PCIT"))
    pcit = model_.pcit;
  if(!given("PCKAPPAD"))
    pckappad = model_.pckappad;
  if(!given("PCKAPPAS"))
    pckappas = model_.pckappas;
  if(!given("PCLC"))
    pclc = model_.pclc;
  if(!given("PCLE"))
    pcle = model_.pcle;
  if(!given("PCLM"))
    pclm = model_.pclm;
  if(!given("PD"))
    pd = model_.pd;
  if(!given("PDELTA"))
    pdelta = model_.pdelta;
  if(!given("PDIBLC1"))
    pdiblc1 = model_.pdiblc1;
  if(!given("PDIBLC2"))
    pdiblc2 = model_.pdiblc2;
  if(!given("PDIBLCB"))
    pdiblcb = model_.pdiblcb;
  if(!given("PDITS"))
    pdits = model_.pdits;
  if(!given("PDITSD"))
    pditsd = model_.pditsd;
  if(!given("PDITSL"))
    pditsl = model_.pditsl;
  if(!given("PDROUT"))
    pdrout = model_.pdrout;
  if(!given("PDSUB"))
    pdsub = model_.pdsub;
  if(!given("PDVT0"))
    pdvt0 = model_.pdvt0;
  if(!given("PDVT0W"))
    pdvt0w = model_.pdvt0w;
  if(!given("PDVT1"))
    pdvt1 = model_.pdvt1;
  if(!given("PDVT1W"))
    pdvt1w = model_.pdvt1w;
  if(!given("PDVT2"))
    pdvt2 = model_.pdvt2;
  if(!given("PDVT2W"))
    pdvt2w = model_.pdvt2w;
  if(!given("PDVTP0"))
    pdvtp0 = model_.pdvtp0;
  if(!given("PDVTP1"))
    pdvtp1 = model_.pdvtp1;
  if(!given("PDVTP2"))
    pdvtp2 = model_.pdvtp2;
  if(!given("PDVTP3"))
    pdvtp3 = model_.pdvtp3;
  if(!given("PDVTP4"))
    pdvtp4 = model_.pdvtp4;
  if(!given("PDVTP5"))
    pdvtp5 = model_.pdvtp5;
  if(!given("PDWB"))
    pdwb = model_.pdwb;
  if(!given("PDWG"))
    pdwg = model_.pdwg;
  if(!given("PEGIDL"))
    pegidl = model_.pegidl;
  if(!given("PEGISL"))
    pegisl = model_.pegisl;
  if(!given("PEIGBINV"))
    peigbinv = model_.peigbinv;
  if(!given("PERMOD"))
    permod = model_.permod;
  if(!given("PETA0"))
    peta0 = model_.peta0;
  if(!given("PETAB"))
    petab = model_.petab;
  if(!given("PEU"))
    peu = model_.peu;
  if(!given("PFGIDL"))
    pfgidl = model_.pfgidl;
  if(!given("PFGISL"))
    pfgisl = model_.pfgisl;
  if(!given("PFPROUT"))
    pfprout = model_.pfprout;
  if(!given("PGAMMA1"))
    pgamma1 = model_.pgamma1;
  if(!given("PGAMMA2"))
    pgamma2 = model_.pgamma2;
  if(!given("PHIG"))
    phig = model_.phig;
  if(!given("PHIN"))
    phin = model_.phin;
  if(!given("PIGCD"))
    pigcd = model_.pigcd;
  if(!given("PK1"))
    pk1 = model_.pk1;
  if(!given("PK2"))
    pk2 = model_.pk2;
  if(!given("PK2WE"))
    pk2we = model_.pk2we;
  if(!given("PK3"))
    pk3 = model_.pk3;
  if(!given("PK3B"))
    pk3b = model_.pk3b;
  if(!given("PKETA"))
    pketa = model_.pketa;
  if(!given("PKGIDL"))
    pkgidl = model_.pkgidl;
  if(!given("PKGISL"))
    pkgisl = model_.pkgisl;
  if(!given("PKT1"))
    pkt1 = model_.pkt1;
  if(!given("PKT1L"))
    pkt1l = model_.pkt1l;
  if(!given("PKT2"))
    pkt2 = model_.pkt2;
  if(!given("PKU0"))
    pku0 = model_.pku0;
  if(!given("PKU0WE"))
    pku0we = model_.pku0we;
  if(!given("PKVTH0"))
    pkvth0 = model_.pkvth0;
  if(!given("PKVTH0WE"))
    pkvth0we = model_.pkvth0we;
  if(!given("PLAMBDA"))
    plambda = model_.plambda;
  if(!given("PLP"))
    plp = model_.plp;
  if(!given("PLPE0"))
    plpe0 = model_.plpe0;
  if(!given("PLPEB"))
    plpeb = model_.plpeb;
  if(!given("PMINV"))
    pminv = model_.pminv;
  if(!given("PMINVCV"))
    pminvcv = model_.pminvcv;
  if(!given("PMOIN"))
    pmoin = model_.pmoin;
  if(!given("PNDEP"))
    pndep = model_.pndep;
  if(!given("PNFACTOR"))
    pnfactor = model_.pnfactor;
  if(!given("PNGATE"))
    pngate = model_.pngate;
  if(!given("PNIGBACC"))
    pnigbacc = model_.pnigbacc;
  if(!given("PNIGBINV"))
    pnigbinv = model_.pnigbinv;
  if(!given("PNIGC"))
    pnigc = model_.pnigc;
  if(!given("PNOFF"))
    pnoff = model_.pnoff;
  if(!given("PNSD"))
    pnsd = model_.pnsd;
  if(!given("PNSUB"))
    pnsub = model_.pnsub;
  if(!given("PNTOX"))
    pntox = model_.pntox;
  if(!given("POXEDGE"))
    poxedge = model_.poxedge;
  if(!given("PPCLM"))
    ppclm = model_.ppclm;
  if(!given("PPDIBLC1"))
    ppdiblc1 = model_.ppdiblc1;
  if(!given("PPDIBLC2"))
    ppdiblc2 = model_.ppdiblc2;
  if(!given("PPDIBLCB"))
    ppdiblcb = model_.ppdiblcb;
  if(!given("PPDITS"))
    ppdits = model_.ppdits;
  if(!given("PPDITSD"))
    ppditsd = model_.ppditsd;
  if(!given("PPHIN"))
    pphin = model_.pphin;
  if(!given("PPIGCD"))
    ppigcd = model_.ppigcd;
  if(!given("PPOXEDGE"))
    ppoxedge = model_.ppoxedge;
  if(!given("PPRT"))
    pprt = model_.pprt;
  if(!given("PPRWB"))
    pprwb = model_.pprwb;
  if(!given("PPRWG"))
    pprwg = model_.pprwg;
  if(!given("PPSCBE1"))
    ppscbe1 = model_.ppscbe1;
  if(!given("PPSCBE2"))
    ppscbe2 = model_.ppscbe2;
  if(!given("PPVAG"))
    ppvag = model_.ppvag;
  if(!given("PRDSW"))
    prdsw = model_.prdsw;
  if(!given("PRDW"))
    prdw = model_.prdw;
  if(!given("PRGIDL"))
    prgidl = model_.prgidl;
  if(!given("PRGISL"))
    prgisl = model_.prgisl;
  if(!given("PRSW"))
    prsw = model_.prsw;
  if(!given("PRT"))
    prt = model_.prt;
  if(!given("PRWB"))
    prwb = model_.prwb;
  if(!given("PRWG"))
    prwg = model_.prwg;
  if(!given("PS"))
    ps = model_.ps;
  if(!given("PSCBE1"))
    pscbe1 = model_.pscbe1;
  if(!given("PSCBE2"))
    pscbe2 = model_.pscbe2;
  if(!given("PTETA0"))
    pteta0 = model_.pteta0;
  if(!given("PTNFACTOR"))
    ptnfactor = model_.ptnfactor;
  if(!given("PTVFBSDOFF"))
    ptvfbsdoff = model_.ptvfbsdoff;
  if(!given("PTVOFF"))
    ptvoff = model_.ptvoff;
  if(!given("PTVOFFCV"))
    ptvoffcv = model_.ptvoffcv;
  if(!given("PU0"))
    pu0 = model_.pu0;
  if(!given("PUA"))
    pua = model_.pua;
  if(!given("PUA1"))
    pua1 = model_.pua1;
  if(!given("PUB"))
    pub = model_.pub;
  if(!given("PUB1"))
    pub1 = model_.pub1;
  if(!given("PUC"))
    puc = model_.puc;
  if(!given("PUC1"))
    puc1 = model_.puc1;
  if(!given("PUCS"))
    pucs = model_.pucs;
  if(!given("PUCSTE"))
    pucste = model_.pucste;
  if(!given("PUD"))
    pud = model_.pud;
  if(!given("PUD1"))
    pud1 = model_.pud1;
  if(!given("PUP"))
    pup = model_.pup;
  if(!given("PUTE"))
    pute = model_.pute;
  if(!given("PVAG"))
    pvag = model_.pvag;
  if(!given("PVBM"))
    pvbm = model_.pvbm;
  if(!given("PVBX"))
    pvbx = model_.pvbx;
  if(!given("PVFB"))
    pvfb = model_.pvfb;
  if(!given("PVFBCV"))
    pvfbcv = model_.pvfbcv;
  if(!given("PVFBSDOFF"))
    pvfbsdoff = model_.pvfbsdoff;
  if(!given("PVOFF"))
    pvoff = model_.pvoff;
  if(!given("PVOFFCV"))
    pvoffcv = model_.pvoffcv;
  if(!given("PVSAT"))
    pvsat = model_.pvsat;
  if(!given("PVTH0"))
    pvth0 = model_.pvth0;
  if(!given("PVTL"))
    pvtl = model_.pvtl;
  if(!given("PW0"))
    pw0 = model_.pw0;
  if(!given("PWR"))
    pwr = model_.pwr;
  if(!given("PXJ"))
    pxj = model_.pxj;
  if(!given("PXN"))
    pxn = model_.pxn;
  if(!given("PXRCRG1"))
    pxrcrg1 = model_.pxrcrg1;
  if(!given("PXRCRG2"))
    pxrcrg2 = model_.pxrcrg2;
  if(!given("PXT"))
    pxt = model_.pxt;
  if(!given("RBDB"))
    rbdb = model_.rbdb;
  if(!given("RBDBX0"))
    rbdbx0 = model_.rbdbx0;
  if(!given("RBDBY0"))
    rbdby0 = model_.rbdby0;
  if(!given("RBODYMOD"))
    rbodymod = model_.rbodymod;
  if(!given("RBPB"))
    rbpb = model_.rbpb;
  if(!given("RBPBX0"))
    rbpbx0 = model_.rbpbx0;
  if(!given("RBPBXL"))
    rbpbxl = model_.rbpbxl;
  if(!given("RBPBXNF"))
    rbpbxnf = model_.rbpbxnf;
  if(!given("RBPBXW"))
    rbpbxw = model_.rbpbxw;
  if(!given("RBPBY0"))
    rbpby0 = model_.rbpby0;
  if(!given("RBPBYL"))
    rbpbyl = model_.rbpbyl;
  if(!given("RBPBYNF"))
    rbpbynf = model_.rbpbynf;
  if(!given("RBPBYW"))
    rbpbyw = model_.rbpbyw;
  if(!given("RBPD"))
    rbpd = model_.rbpd;
  if(!given("RBPD0"))
    rbpd0 = model_.rbpd0;
  if(!given("RBPDL"))
    rbpdl = model_.rbpdl;
  if(!given("RBPDNF"))
    rbpdnf = model_.rbpdnf;
  if(!given("RBPDW"))
    rbpdw = model_.rbpdw;
  if(!given("RBPS"))
    rbps = model_.rbps;
  if(!given("RBPS0"))
    rbps0 = model_.rbps0;
  if(!given("RBPSL"))
    rbpsl = model_.rbpsl;
  if(!given("RBPSNF"))
    rbpsnf = model_.rbpsnf;
  if(!given("RBPSW"))
    rbpsw = model_.rbpsw;
  if(!given("RBSB"))
    rbsb = model_.rbsb;
  if(!given("RBSBX0"))
    rbsbx0 = model_.rbsbx0;
  if(!given("RBSBY0"))
    rbsby0 = model_.rbsby0;
  if(!given("RBSDBXL"))
    rbsdbxl = model_.rbsdbxl;
  if(!given("RBSDBXNF"))
    rbsdbxnf = model_.rbsdbxnf;
  if(!given("RBSDBXW"))
    rbsdbxw = model_.rbsdbxw;
  if(!given("RBSDBYL"))
    rbsdbyl = model_.rbsdbyl;
  if(!given("RBSDBYNF"))
    rbsdbynf = model_.rbsdbynf;
  if(!given("RBSDBYW"))
    rbsdbyw = model_.rbsdbyw;
  if(!given("RDSMOD"))
    rdsmod = model_.rdsmod;
  if(!given("RDSW"))
    rdsw = model_.rdsw;
  if(!given("RDSWMIN"))
    rdswmin = model_.rdswmin;
  if(!given("RDW"))
    rdw = model_.rdw;
  if(!given("RDWMIN"))
    rdwmin = model_.rdwmin;
  if(!given("RGATEMOD"))
    rgatemod = model_.rgatemod;
  if(!given("RGEOMOD"))
    rgeomod = model_.rgeomod;
  if(!given("RGIDL"))
    rgidl = model_.rgidl;
  if(!given("RGISL"))
    rgisl = model_.rgisl;
  if(!given("RNOIA"))
    rnoia = model_.rnoia;
  if(!given("RNOIB"))
    rnoib = model_.rnoib;
  if(!given("RNOIC"))
    rnoic = model_.rnoic;
  if(!given("RSH"))
    rsh = model_.rsh;
  if(!given("RSHG"))
    rshg = model_.rshg;
  if(!given("RSW"))
    rsw = model_.rsw;
  if(!given("RSWMIN"))
    rswmin = model_.rswmin;
  if(!given("SA"))
    sa = model_.sa;
  if(!given("SAREF"))
    saref = model_.saref;
  if(!given("SB"))
    sb = model_.sb;
  if(!given("SBREF"))
    sbref = model_.sbref;
  if(!given("SC"))
    sc = model_.sc;
  if(!given("SCA"))
    sca = model_.sca;
  if(!given("SCB"))
    scb = model_.scb;
  if(!given("SCC"))
    scc = model_.scc;
  if(!given("SCREF"))
    scref = model_.scref;
  if(!given("SD"))
    sd = model_.sd;
  if(!given("STETA0"))
    steta0 = model_.steta0;
  if(!given("STK2"))
    stk2 = model_.stk2;
  if(!given("TBGASUB"))
    tbgasub = model_.tbgasub;
  if(!given("TBGBSUB"))
    tbgbsub = model_.tbgbsub;
  if(!given("TCJ"))
    tcj = model_.tcj;
  if(!given("TCJSW"))
    tcjsw = model_.tcjsw;
  if(!given("TCJSWG"))
    tcjswg = model_.tcjswg;
  if(!given("TEMPEOT"))
    tempeot = model_.tempeot;
  if(!given("TEMPMOD"))
    tempmod = model_.tempmod;
  if(!given("TETA0"))
    teta0 = model_.teta0;
  if(!given("TKU0"))
    tku0 = model_.tku0;
  if(!given("TNFACTOR"))
    tnfactor = model_.tnfactor;
  if(!given("TNJTS"))
    tnjts = model_.tnjts;
  if(!given("TNJTSD"))
    tnjtsd = model_.tnjtsd;
  if(!given("TNJTSSW"))
    tnjtssw = model_.tnjtssw;
  if(!given("TNJTSSWD"))
    tnjtsswd = model_.tnjtsswd;
  if(!given("TNJTSSWG"))
    tnjtsswg = model_.tnjtsswg;
  if(!given("TNJTSSWGD"))
    tnjtsswgd = model_.tnjtsswgd;
  if(!given("TNOIA"))
    tnoia = model_.tnoia;
  if(!given("TNOIB"))
    tnoib = model_.tnoib;
  if(!given("TNOIC"))
    tnoic = model_.tnoic;
  if(!given("TNOIMOD"))
    tnoimod = model_.tnoimod;
  if(!given("TNOM"))
    tnom = model_.tnom;
  else
    if(tnom < -100 || tnom>150)
      tnom -= P_CELSIUS0;
  if(!given("TOXE"))
    toxe = model_.toxe;
  if(!given("TOXM"))
    toxm = model_.toxm;
  if(!given("TOXP"))
    toxp = model_.toxp;
  if(!given("TOXREF"))
    toxref = model_.toxref;
  if(!given("TPB"))
    tpb = model_.tpb;
  if(!given("TPBSW"))
    tpbsw = model_.tpbsw;
  if(!given("TPBSWG"))
    tpbswg = model_.tpbswg;
  if(!given("TRNQSMOD"))
    trnqsmod = model_.trnqsmod;
  if(!given("TVFBSDOFF"))
    tvfbsdoff = model_.tvfbsdoff;
  if(!given("TVOFF"))
    tvoff = model_.tvoff;
  if(!given("TVOFFCV"))
    tvoffcv = model_.tvoffcv;
  if(!given("TYPE"))
    type = model_.type;
  if(!given("U0"))
    u0 = model_.u0;
  if(!given("UA"))
    ua = model_.ua;
  if(!given("UA1"))
    ua1 = model_.ua1;
  if(!given("UB"))
    ub = model_.ub;
  if(!given("UB1"))
    ub1 = model_.ub1;
  if(!given("UC"))
    uc = model_.uc;
  if(!given("UC1"))
    uc1 = model_.uc1;
  if(!given("UCS"))
    ucs = model_.ucs;
  if(!given("UCSTE"))
    ucste = model_.ucste;
  if(!given("UD"))
    ud = model_.ud;
  if(!given("UD1"))
    ud1 = model_.ud1;
  if(!given("UP"))
    up = model_.up;
  if(!given("UTE"))
    ute = model_.ute;
  if(!given("VBM"))
    vbm = model_.vbm;
  if(!given("VBX"))
    vbx = model_.vbx;
  if(!given("VDDEOT"))
    vddeot = model_.vddeot;
  if(!given("VERBOSE"))
    verbose = model_.verbose;
  if(!given("VERSION"))
    version = model_.version;
  if(!given("VFB"))
    vfb = model_.vfb;
  if(!given("VFBCV"))
    vfbcv = model_.vfbcv;
  if(!given("VFBSDOFF"))
    vfbsdoff = model_.vfbsdoff;
  if(!given("VOFF"))
    voff = model_.voff;
  if(!given("VOFFCV"))
    voffcv = model_.voffcv;
  if(!given("VOFFCVL"))
    voffcvl = model_.voffcvl;
  if(!given("VOFFL"))
    voffl = model_.voffl;
  if(!given("VSAT"))
    vsat = model_.vsat;
  if(!given("VTH0"))
    vth0 = model_.vth0;
  if(!given("VTL"))
    vtl = model_.vtl;
  if(!given("VTSD"))
    vtsd = model_.vtsd;
  if(!given("VTSS"))
    vtss = model_.vtss;
  if(!given("VTSSWD"))
    vtsswd = model_.vtsswd;
  if(!given("VTSSWGD"))
    vtsswgd = model_.vtsswgd;
  if(!given("VTSSWGS"))
    vtsswgs = model_.vtsswgs;
  if(!given("VTSSWS"))
    vtssws = model_.vtssws;
  if(!given("W"))
    w = model_.w;
  if(!given("W0"))
    w0 = model_.w0;
  if(!given("WA0"))
    wa0 = model_.wa0;
  if(!given("WA1"))
    wa1 = model_.wa1;
  if(!given("WA2"))
    wa2 = model_.wa2;
  if(!given("WACDE"))
    wacde = model_.wacde;
  if(!given("WAGIDL"))
    wagidl = model_.wagidl;
  if(!given("WAGISL"))
    wagisl = model_.wagisl;
  if(!given("WAGS"))
    wags = model_.wags;
  if(!given("WAIGBACC"))
    waigbacc = model_.waigbacc;
  if(!given("WAIGBINV"))
    waigbinv = model_.waigbinv;
  if(!given("WAIGC"))
    waigc = model_.waigc;
  if(!given("WAIGD"))
    waigd = model_.waigd;
  if(!given("WAIGS"))
    waigs = model_.waigs;
  if(!given("WAIGSD"))
    waigsd = model_.waigsd;
  if(!given("WALPHA0"))
    walpha0 = model_.walpha0;
  if(!given("WALPHA1"))
    walpha1 = model_.walpha1;
  if(!given("WAT"))
    wat = model_.wat;
  if(!given("WB0"))
    wb0 = model_.wb0;
  if(!given("WB1"))
    wb1 = model_.wb1;
  if(!given("WBETA0"))
    wbeta0 = model_.wbeta0;
  if(!given("WBGIDL"))
    wbgidl = model_.wbgidl;
  if(!given("WBGISL"))
    wbgisl = model_.wbgisl;
  if(!given("WBIGBACC"))
    wbigbacc = model_.wbigbacc;
  if(!given("WBIGBINV"))
    wbigbinv = model_.wbigbinv;
  if(!given("WBIGC"))
    wbigc = model_.wbigc;
  if(!given("WBIGD"))
    wbigd = model_.wbigd;
  if(!given("WBIGS"))
    wbigs = model_.wbigs;
  if(!given("WBIGSD"))
    wbigsd = model_.wbigsd;
  if(!given("WCDSC"))
    wcdsc = model_.wcdsc;
  if(!given("WCDSCB"))
    wcdscb = model_.wcdscb;
  if(!given("WCDSCD"))
    wcdscd = model_.wcdscd;
  if(!given("WCF"))
    wcf = model_.wcf;
  if(!given("WCGDL"))
    wcgdl = model_.wcgdl;
  if(!given("WCGIDL"))
    wcgidl = model_.wcgidl;
  if(!given("WCGISL"))
    wcgisl = model_.wcgisl;
  if(!given("WCGSL"))
    wcgsl = model_.wcgsl;
  if(!given("WCIGBACC"))
    wcigbacc = model_.wcigbacc;
  if(!given("WCIGBINV"))
    wcigbinv = model_.wcigbinv;
  if(!given("WCIGC"))
    wcigc = model_.wcigc;
  if(!given("WCIGD"))
    wcigd = model_.wcigd;
  if(!given("WCIGS"))
    wcigs = model_.wcigs;
  if(!given("WCIGSD"))
    wcigsd = model_.wcigsd;
  if(!given("WCIT"))
    wcit = model_.wcit;
  if(!given("WCKAPPAD"))
    wckappad = model_.wckappad;
  if(!given("WCKAPPAS"))
    wckappas = model_.wckappas;
  if(!given("WCLC"))
    wclc = model_.wclc;
  if(!given("WCLE"))
    wcle = model_.wcle;
  if(!given("WDELTA"))
    wdelta = model_.wdelta;
  if(!given("WDROUT"))
    wdrout = model_.wdrout;
  if(!given("WDSUB"))
    wdsub = model_.wdsub;
  if(!given("WDVT0"))
    wdvt0 = model_.wdvt0;
  if(!given("WDVT0W"))
    wdvt0w = model_.wdvt0w;
  if(!given("WDVT1"))
    wdvt1 = model_.wdvt1;
  if(!given("WDVT1W"))
    wdvt1w = model_.wdvt1w;
  if(!given("WDVT2"))
    wdvt2 = model_.wdvt2;
  if(!given("WDVT2W"))
    wdvt2w = model_.wdvt2w;
  if(!given("WDVTP0"))
    wdvtp0 = model_.wdvtp0;
  if(!given("WDVTP1"))
    wdvtp1 = model_.wdvtp1;
  if(!given("WDVTP2"))
    wdvtp2 = model_.wdvtp2;
  if(!given("WDVTP3"))
    wdvtp3 = model_.wdvtp3;
  if(!given("WDVTP4"))
    wdvtp4 = model_.wdvtp4;
  if(!given("WDVTP5"))
    wdvtp5 = model_.wdvtp5;
  if(!given("WDWB"))
    wdwb = model_.wdwb;
  if(!given("WDWG"))
    wdwg = model_.wdwg;
  if(!given("WEB"))
    web = model_.web;
  if(!given("WEC"))
    wec = model_.wec;
  if(!given("WEFFEOT"))
    weffeot = model_.weffeot;
  if(!given("WEGIDL"))
    wegidl = model_.wegidl;
  if(!given("WEGISL"))
    wegisl = model_.wegisl;
  if(!given("WEIGBINV"))
    weigbinv = model_.weigbinv;
  if(!given("WETA0"))
    weta0 = model_.weta0;
  if(!given("WETAB"))
    wetab = model_.wetab;
  if(!given("WEU"))
    weu = model_.weu;
  if(!given("WFGIDL"))
    wfgidl = model_.wfgidl;
  if(!given("WFGISL"))
    wfgisl = model_.wfgisl;
  if(!given("WFPROUT"))
    wfprout = model_.wfprout;
  if(!given("WGAMMA1"))
    wgamma1 = model_.wgamma1;
  if(!given("WGAMMA2"))
    wgamma2 = model_.wgamma2;
  if(!given("WINT"))
    wint = model_.wint;
  if(!given("WK1"))
    wk1 = model_.wk1;
  if(!given("WK2"))
    wk2 = model_.wk2;
  if(!given("WK2WE"))
    wk2we = model_.wk2we;
  if(!given("WK3"))
    wk3 = model_.wk3;
  if(!given("WK3B"))
    wk3b = model_.wk3b;
  if(!given("WKETA"))
    wketa = model_.wketa;
  if(!given("WKGIDL"))
    wkgidl = model_.wkgidl;
  if(!given("WKGISL"))
    wkgisl = model_.wkgisl;
  if(!given("WKT1"))
    wkt1 = model_.wkt1;
  if(!given("WKT1L"))
    wkt1l = model_.wkt1l;
  if(!given("WKT2"))
    wkt2 = model_.wkt2;
  if(!given("WKU0"))
    wku0 = model_.wku0;
  if(!given("WKU0WE"))
    wku0we = model_.wku0we;
  if(!given("WKVTH0"))
    wkvth0 = model_.wkvth0;
  if(!given("WKVTH0WE"))
    wkvth0we = model_.wkvth0we;
  if(!given("WL"))
    wl = model_.wl;
  if(!given("WLAMBDA"))
    wlambda = model_.wlambda;
  if(!given("WLC"))
    wlc = model_.wlc;
  if(!given("WLN"))
    wln = model_.wln;
  if(!given("WLOD"))
    wlod = model_.wlod;
  if(!given("WLODKU0"))
    wlodku0 = model_.wlodku0;
  if(!given("WLODVTH"))
    wlodvth = model_.wlodvth;
  if(!given("WLP"))
    wlp = model_.wlp;
  if(!given("WLPE0"))
    wlpe0 = model_.wlpe0;
  if(!given("WLPEB"))
    wlpeb = model_.wlpeb;
  if(!given("WMAX"))
    wmax = model_.wmax;
  if(!given("WMIN"))
    wmin = model_.wmin;
  if(!given("WMINV"))
    wminv = model_.wminv;
  if(!given("WMINVCV"))
    wminvcv = model_.wminvcv;
  if(!given("WMOIN"))
    wmoin = model_.wmoin;
  if(!given("WNDEP"))
    wndep = model_.wndep;
  if(!given("WNFACTOR"))
    wnfactor = model_.wnfactor;
  if(!given("WNGATE"))
    wngate = model_.wngate;
  if(!given("WNIGBACC"))
    wnigbacc = model_.wnigbacc;
  if(!given("WNIGBINV"))
    wnigbinv = model_.wnigbinv;
  if(!given("WNIGC"))
    wnigc = model_.wnigc;
  if(!given("WNOFF"))
    wnoff = model_.wnoff;
  if(!given("WNSD"))
    wnsd = model_.wnsd;
  if(!given("WNSUB"))
    wnsub = model_.wnsub;
  if(!given("WNTOX"))
    wntox = model_.wntox;
  if(!given("WPCLM"))
    wpclm = model_.wpclm;
  if(!given("WPDIBLC1"))
    wpdiblc1 = model_.wpdiblc1;
  if(!given("WPDIBLC2"))
    wpdiblc2 = model_.wpdiblc2;
  if(!given("WPDIBLCB"))
    wpdiblcb = model_.wpdiblcb;
  if(!given("WPDITS"))
    wpdits = model_.wpdits;
  if(!given("WPDITSD"))
    wpditsd = model_.wpditsd;
  if(!given("WPEMOD"))
    wpemod = model_.wpemod;
  if(!given("WPHIN"))
    wphin = model_.wphin;
  if(!given("WPIGCD"))
    wpigcd = model_.wpigcd;
  if(!given("WPOXEDGE"))
    wpoxedge = model_.wpoxedge;
  if(!given("WPRT"))
    wprt = model_.wprt;
  if(!given("WPRWB"))
    wprwb = model_.wprwb;
  if(!given("WPRWG"))
    wprwg = model_.wprwg;
  if(!given("WPSCBE1"))
    wpscbe1 = model_.wpscbe1;
  if(!given("WPSCBE2"))
    wpscbe2 = model_.wpscbe2;
  if(!given("WPVAG"))
    wpvag = model_.wpvag;
  if(!given("WR"))
    wr = model_.wr;
  if(!given("WRDSW"))
    wrdsw = model_.wrdsw;
  if(!given("WRDW"))
    wrdw = model_.wrdw;
  if(!given("WRGIDL"))
    wrgidl = model_.wrgidl;
  if(!given("WRGISL"))
    wrgisl = model_.wrgisl;
  if(!given("WRSW"))
    wrsw = model_.wrsw;
  if(!given("WTETA0"))
    wteta0 = model_.wteta0;
  if(!given("WTNFACTOR"))
    wtnfactor = model_.wtnfactor;
  if(!given("WTVFBSDOFF"))
    wtvfbsdoff = model_.wtvfbsdoff;
  if(!given("WTVOFF"))
    wtvoff = model_.wtvoff;
  if(!given("WTVOFFCV"))
    wtvoffcv = model_.wtvoffcv;
  if(!given("WU0"))
    wu0 = model_.wu0;
  if(!given("WUA"))
    wua = model_.wua;
  if(!given("WUA1"))
    wua1 = model_.wua1;
  if(!given("WUB"))
    wub = model_.wub;
  if(!given("WUB1"))
    wub1 = model_.wub1;
  if(!given("WUC"))
    wuc = model_.wuc;
  if(!given("WUC1"))
    wuc1 = model_.wuc1;
  if(!given("WUCS"))
    wucs = model_.wucs;
  if(!given("WUCSTE"))
    wucste = model_.wucste;
  if(!given("WUD"))
    wud = model_.wud;
  if(!given("WUD1"))
    wud1 = model_.wud1;
  if(!given("WUP"))
    wup = model_.wup;
  if(!given("WUTE"))
    wute = model_.wute;
  if(!given("WVBM"))
    wvbm = model_.wvbm;
  if(!given("WVBX"))
    wvbx = model_.wvbx;
  if(!given("WVFB"))
    wvfb = model_.wvfb;
  if(!given("WVFBCV"))
    wvfbcv = model_.wvfbcv;
  if(!given("WVFBSDOFF"))
    wvfbsdoff = model_.wvfbsdoff;
  if(!given("WVOFF"))
    wvoff = model_.wvoff;
  if(!given("WVOFFCV"))
    wvoffcv = model_.wvoffcv;
  if(!given("WVSAT"))
    wvsat = model_.wvsat;
  if(!given("WVTH0"))
    wvth0 = model_.wvth0;
  if(!given("WVTL"))
    wvtl = model_.wvtl;
  if(!given("WW"))
    ww = model_.ww;
  if(!given("WW0"))
    ww0 = model_.ww0;
  if(!given("WWC"))
    wwc = model_.wwc;
  if(!given("WWL"))
    wwl = model_.wwl;
  if(!given("WWLC"))
    wwlc = model_.wwlc;
  if(!given("WWN"))
    wwn = model_.wwn;
  if(!given("WWR"))
    wwr = model_.wwr;
  if(!given("WXJ"))
    wxj = model_.wxj;
  if(!given("WXN"))
    wxn = model_.wxn;
  if(!given("WXRCRG1"))
    wxrcrg1 = model_.wxrcrg1;
  if(!given("WXRCRG2"))
    wxrcrg2 = model_.wxrcrg2;
  if(!given("WXT"))
    wxt = model_.wxt;
  if(!given("XGL"))
    xgl = model_.xgl;
  if(!given("XGW"))
    xgw = model_.xgw;
  if(!given("XJ"))
    xj = model_.xj;
  if(!given("XJBVD"))
    xjbvd = model_.xjbvd;
  if(!given("XJBVS"))
    xjbvs = model_.xjbvs;
  if(!given("XL"))
    xl = model_.xl;
  if(!given("XN"))
    xn = model_.xn;
  if(!given("XPART"))
    xpart = model_.xpart;
  if(!given("XRCRG1"))
    xrcrg1 = model_.xrcrg1;
  if(!given("XRCRG2"))
    xrcrg2 = model_.xrcrg2;
  if(!given("XT"))
    xt = model_.xt;
  if(!given("XTID"))
    xtid = model_.xtid;
  if(!given("XTIS"))
    xtis = model_.xtis;
  if(!given("XTSD"))
    xtsd = model_.xtsd;
  if(!given("XTSS"))
    xtss = model_.xtss;
  if(!given("XTSSWD"))
    xtsswd = model_.xtsswd;
  if(!given("XTSSWGD"))
    xtsswgd = model_.xtsswgd;
  if(!given("XTSSWGS"))
    xtsswgs = model_.xtsswgs;
  if(!given("XTSSWS"))
    xtssws = model_.xtssws;
  if(!given("XW"))
    xw = model_.xw;
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
    li_d(-1),
    li_g(-1),
    li_s(-1),
    li_b(-1),
    li_di(-1),
    li_si(-1),
    li_gi(-1),
    li_gm(-1),
    li_bi(-1),
    li_sbulk(-1),
    li_dbulk(-1),
    li_branch_id(-1),
    li_branch_ig(-1),
    li_branch_is(-1),
    li_branch_ib(-1),
    f_di_Equ_bi_Node_Ptr(0),
    f_di_Equ_si_Node_Ptr(0),
    f_di_Equ_sbulk_Node_Ptr(0),
    f_di_Equ_di_Node_Ptr(0),
    f_di_Equ_dbulk_Node_Ptr(0),
    f_di_Equ_gi_Node_Ptr(0),
    f_si_Equ_bi_Node_Ptr(0),
    f_si_Equ_si_Node_Ptr(0),
    f_si_Equ_sbulk_Node_Ptr(0),
    f_si_Equ_di_Node_Ptr(0),
    f_si_Equ_dbulk_Node_Ptr(0),
    f_si_Equ_gi_Node_Ptr(0),
    f_gi_Equ_bi_Node_Ptr(0),
    f_gi_Equ_si_Node_Ptr(0),
    f_gi_Equ_sbulk_Node_Ptr(0),
    f_gi_Equ_di_Node_Ptr(0),
    f_gi_Equ_dbulk_Node_Ptr(0),
    f_gi_Equ_gi_Node_Ptr(0),
    f_bi_Equ_bi_Node_Ptr(0),
    f_bi_Equ_si_Node_Ptr(0),
    f_bi_Equ_sbulk_Node_Ptr(0),
    f_bi_Equ_di_Node_Ptr(0),
    f_bi_Equ_dbulk_Node_Ptr(0),
    f_bi_Equ_gi_Node_Ptr(0),
    f_s_Equ_s_Node_Ptr(0),
    f_s_Equ_si_Node_Ptr(0),
    f_s_Equ_bi_Node_Ptr(0),
    f_s_Equ_sbulk_Node_Ptr(0),
    f_s_Equ_di_Node_Ptr(0),
    f_s_Equ_dbulk_Node_Ptr(0),
    f_s_Equ_gi_Node_Ptr(0),
    f_si_Equ_s_Node_Ptr(0),
    f_d_Equ_d_Node_Ptr(0),
    f_d_Equ_di_Node_Ptr(0),
    f_d_Equ_bi_Node_Ptr(0),
    f_d_Equ_si_Node_Ptr(0),
    f_d_Equ_sbulk_Node_Ptr(0),
    f_d_Equ_dbulk_Node_Ptr(0),
    f_d_Equ_gi_Node_Ptr(0),
    f_di_Equ_d_Node_Ptr(0),
    f_g_Equ_g_Node_Ptr(0),
    f_g_Equ_gm_Node_Ptr(0),
    f_gm_Equ_g_Node_Ptr(0),
    f_gm_Equ_gm_Node_Ptr(0),
    f_gm_Equ_gi_Node_Ptr(0),
    f_gm_Equ_bi_Node_Ptr(0),
    f_gm_Equ_si_Node_Ptr(0),
    f_gm_Equ_sbulk_Node_Ptr(0),
    f_gm_Equ_di_Node_Ptr(0),
    f_gm_Equ_dbulk_Node_Ptr(0),
    f_gi_Equ_gm_Node_Ptr(0),
    f_sbulk_Equ_sbulk_Node_Ptr(0),
    f_sbulk_Equ_bi_Node_Ptr(0),
    f_sbulk_Equ_b_Node_Ptr(0),
    f_b_Equ_sbulk_Node_Ptr(0),
    f_b_Equ_b_Node_Ptr(0),
    f_b_Equ_bi_Node_Ptr(0),
    f_bi_Equ_b_Node_Ptr(0),
    f_b_Equ_dbulk_Node_Ptr(0),
    f_dbulk_Equ_b_Node_Ptr(0),
    f_dbulk_Equ_dbulk_Node_Ptr(0),
    f_dbulk_Equ_bi_Node_Ptr(0),
    f_sbulk_Equ_si_Node_Ptr(0),
    f_sbulk_Equ_di_Node_Ptr(0),
    f_sbulk_Equ_dbulk_Node_Ptr(0),
    f_dbulk_Equ_si_Node_Ptr(0),
    f_dbulk_Equ_di_Node_Ptr(0),
    f_dbulk_Equ_sbulk_Node_Ptr(0),
    f_si_Equ_gm_Node_Ptr(0),
    f_di_Equ_gm_Node_Ptr(0),
    f_bi_Equ_gm_Node_Ptr(0),
    f_sbulk_Equ_gi_Node_Ptr(0),
    f_dbulk_Equ_gi_Node_Ptr(0),
    q_di_Equ_bi_Node_Ptr(0),
    q_di_Equ_si_Node_Ptr(0),
    q_di_Equ_sbulk_Node_Ptr(0),
    q_di_Equ_di_Node_Ptr(0),
    q_di_Equ_dbulk_Node_Ptr(0),
    q_di_Equ_gi_Node_Ptr(0),
    q_si_Equ_bi_Node_Ptr(0),
    q_si_Equ_si_Node_Ptr(0),
    q_si_Equ_sbulk_Node_Ptr(0),
    q_si_Equ_di_Node_Ptr(0),
    q_si_Equ_dbulk_Node_Ptr(0),
    q_si_Equ_gi_Node_Ptr(0),
    q_gi_Equ_bi_Node_Ptr(0),
    q_gi_Equ_si_Node_Ptr(0),
    q_gi_Equ_sbulk_Node_Ptr(0),
    q_gi_Equ_di_Node_Ptr(0),
    q_gi_Equ_dbulk_Node_Ptr(0),
    q_gi_Equ_gi_Node_Ptr(0),
    q_bi_Equ_bi_Node_Ptr(0),
    q_bi_Equ_si_Node_Ptr(0),
    q_bi_Equ_sbulk_Node_Ptr(0),
    q_bi_Equ_di_Node_Ptr(0),
    q_bi_Equ_dbulk_Node_Ptr(0),
    q_bi_Equ_gi_Node_Ptr(0),
    q_s_Equ_s_Node_Ptr(0),
    q_s_Equ_si_Node_Ptr(0),
    q_s_Equ_bi_Node_Ptr(0),
    q_s_Equ_sbulk_Node_Ptr(0),
    q_s_Equ_di_Node_Ptr(0),
    q_s_Equ_dbulk_Node_Ptr(0),
    q_s_Equ_gi_Node_Ptr(0),
    q_si_Equ_s_Node_Ptr(0),
    q_d_Equ_d_Node_Ptr(0),
    q_d_Equ_di_Node_Ptr(0),
    q_d_Equ_bi_Node_Ptr(0),
    q_d_Equ_si_Node_Ptr(0),
    q_d_Equ_sbulk_Node_Ptr(0),
    q_d_Equ_dbulk_Node_Ptr(0),
    q_d_Equ_gi_Node_Ptr(0),
    q_di_Equ_d_Node_Ptr(0),
    q_g_Equ_g_Node_Ptr(0),
    q_g_Equ_gm_Node_Ptr(0),
    q_gm_Equ_g_Node_Ptr(0),
    q_gm_Equ_gm_Node_Ptr(0),
    q_gm_Equ_gi_Node_Ptr(0),
    q_gm_Equ_bi_Node_Ptr(0),
    q_gm_Equ_si_Node_Ptr(0),
    q_gm_Equ_sbulk_Node_Ptr(0),
    q_gm_Equ_di_Node_Ptr(0),
    q_gm_Equ_dbulk_Node_Ptr(0),
    q_gi_Equ_gm_Node_Ptr(0),
    q_sbulk_Equ_sbulk_Node_Ptr(0),
    q_sbulk_Equ_bi_Node_Ptr(0),
    q_sbulk_Equ_b_Node_Ptr(0),
    q_b_Equ_sbulk_Node_Ptr(0),
    q_b_Equ_b_Node_Ptr(0),
    q_b_Equ_bi_Node_Ptr(0),
    q_bi_Equ_b_Node_Ptr(0),
    q_b_Equ_dbulk_Node_Ptr(0),
    q_dbulk_Equ_b_Node_Ptr(0),
    q_dbulk_Equ_dbulk_Node_Ptr(0),
    q_dbulk_Equ_bi_Node_Ptr(0),
    q_sbulk_Equ_si_Node_Ptr(0),
    q_sbulk_Equ_di_Node_Ptr(0),
    q_sbulk_Equ_dbulk_Node_Ptr(0),
    q_dbulk_Equ_si_Node_Ptr(0),
    q_dbulk_Equ_di_Node_Ptr(0),
    q_dbulk_Equ_sbulk_Node_Ptr(0),
    q_si_Equ_gm_Node_Ptr(0),
    q_di_Equ_gm_Node_Ptr(0),
    q_bi_Equ_gm_Node_Ptr(0),
    q_sbulk_Equ_gi_Node_Ptr(0),
    q_dbulk_Equ_gi_Node_Ptr(0),
    m_di_Equ_bi_NodeOffset(-1),
    m_di_Equ_si_NodeOffset(-1),
    m_di_Equ_sbulk_NodeOffset(-1),
    m_di_Equ_di_NodeOffset(-1),
    m_di_Equ_dbulk_NodeOffset(-1),
    m_di_Equ_gi_NodeOffset(-1),
    m_si_Equ_bi_NodeOffset(-1),
    m_si_Equ_si_NodeOffset(-1),
    m_si_Equ_sbulk_NodeOffset(-1),
    m_si_Equ_di_NodeOffset(-1),
    m_si_Equ_dbulk_NodeOffset(-1),
    m_si_Equ_gi_NodeOffset(-1),
    m_gi_Equ_bi_NodeOffset(-1),
    m_gi_Equ_si_NodeOffset(-1),
    m_gi_Equ_sbulk_NodeOffset(-1),
    m_gi_Equ_di_NodeOffset(-1),
    m_gi_Equ_dbulk_NodeOffset(-1),
    m_gi_Equ_gi_NodeOffset(-1),
    m_bi_Equ_bi_NodeOffset(-1),
    m_bi_Equ_si_NodeOffset(-1),
    m_bi_Equ_sbulk_NodeOffset(-1),
    m_bi_Equ_di_NodeOffset(-1),
    m_bi_Equ_dbulk_NodeOffset(-1),
    m_bi_Equ_gi_NodeOffset(-1),
    m_s_Equ_s_NodeOffset(-1),
    m_s_Equ_si_NodeOffset(-1),
    m_s_Equ_bi_NodeOffset(-1),
    m_s_Equ_sbulk_NodeOffset(-1),
    m_s_Equ_di_NodeOffset(-1),
    m_s_Equ_dbulk_NodeOffset(-1),
    m_s_Equ_gi_NodeOffset(-1),
    m_si_Equ_s_NodeOffset(-1),
    m_d_Equ_d_NodeOffset(-1),
    m_d_Equ_di_NodeOffset(-1),
    m_d_Equ_bi_NodeOffset(-1),
    m_d_Equ_si_NodeOffset(-1),
    m_d_Equ_sbulk_NodeOffset(-1),
    m_d_Equ_dbulk_NodeOffset(-1),
    m_d_Equ_gi_NodeOffset(-1),
    m_di_Equ_d_NodeOffset(-1),
    m_g_Equ_g_NodeOffset(-1),
    m_g_Equ_gm_NodeOffset(-1),
    m_gm_Equ_g_NodeOffset(-1),
    m_gm_Equ_gm_NodeOffset(-1),
    m_gm_Equ_gi_NodeOffset(-1),
    m_gm_Equ_bi_NodeOffset(-1),
    m_gm_Equ_si_NodeOffset(-1),
    m_gm_Equ_sbulk_NodeOffset(-1),
    m_gm_Equ_di_NodeOffset(-1),
    m_gm_Equ_dbulk_NodeOffset(-1),
    m_gi_Equ_gm_NodeOffset(-1),
    m_sbulk_Equ_sbulk_NodeOffset(-1),
    m_sbulk_Equ_bi_NodeOffset(-1),
    m_sbulk_Equ_b_NodeOffset(-1),
    m_b_Equ_sbulk_NodeOffset(-1),
    m_b_Equ_b_NodeOffset(-1),
    m_b_Equ_bi_NodeOffset(-1),
    m_bi_Equ_b_NodeOffset(-1),
    m_b_Equ_dbulk_NodeOffset(-1),
    m_dbulk_Equ_b_NodeOffset(-1),
    m_dbulk_Equ_dbulk_NodeOffset(-1),
    m_dbulk_Equ_bi_NodeOffset(-1),
    m_sbulk_Equ_si_NodeOffset(-1),
    m_sbulk_Equ_di_NodeOffset(-1),
    m_sbulk_Equ_dbulk_NodeOffset(-1),
    m_dbulk_Equ_si_NodeOffset(-1),
    m_dbulk_Equ_di_NodeOffset(-1),
    m_dbulk_Equ_sbulk_NodeOffset(-1),
    m_si_Equ_gm_NodeOffset(-1),
    m_di_Equ_gm_NodeOffset(-1),
    m_bi_Equ_gm_NodeOffset(-1),
    m_sbulk_Equ_gi_NodeOffset(-1),
    m_dbulk_Equ_gi_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 4;
  numIntVars = 7;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 4;
  leadCurrentF.resize(4);
  leadCurrentQ.resize(4);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_s));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_s));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_d));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_d));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_g));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_g));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_bi));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_dbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_sbulk));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_gm));
  jacobianElements.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_gi));
  jacobianElements.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_gi));

  setDefaultParams();
  setParams(instance_block.params);

  if (!given("XYCEADMSINSTTEMP"))
    cogendaInstTemp=getDeviceOptions().temp.getImmutableValue<double>();

  updateDependentParameters();

  processParams();
  initInternalVars();

  PairVector collapsedNodes;
  collapseNodes();
  if (collapseNode_si) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_s));
  if (collapseNode_di) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_d));
  if (collapseNode_gm) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_gm,cogendaNodeID_g));
  if (collapseNode_gi) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_gi,cogendaNodeID_gm));
  if (collapseNode_sbulk) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_sbulk,cogendaNodeID_b));
  if (collapseNode_bi) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_bi,cogendaNodeID_b));
  if (collapseNode_dbulk) 
    collapsedNodes.push_back(IntPair(cogendaNodeID_dbulk,cogendaNodeID_b));

  if (jacStamp.empty())
  {
    int originalSize = 11;
    computeJacStampAndMaps(jacobianElements,collapsedNodes,jacStamp,nodeMap,pairToJacStampMap,originalSize);
  }
}

  void Instance::collapseNodes() {
  collapseNode_si = false;
  collapseNode_di = false;
  collapseNode_gm = false;
  collapseNode_gi = false;
  collapseNode_sbulk = false;
  collapseNode_bi = false;
  collapseNode_dbulk = false;
if(BSIM4rdsMod) {
{
if((this->verbose == 1)) {
{
}
}
}
}
else {
{
  collapseNode_si = true;
  numIntVars--;
  collapseNode_di = true;
  numIntVars--;
}
}
switch(BSIM4rgateMod){
case 0:
{
  collapseNode_gm = true;
  numIntVars--;
  collapseNode_gi = true;
  numIntVars--;
}
break;
case 1:
{
  collapseNode_gi = true;
  numIntVars--;
}
break;
case 2:
{
  collapseNode_gm = true;
  numIntVars--;
}
break;
case 3:
{
if((this->verbose == 1)) {
{
}
}
}
break;
}
if(BSIM4rbodyMod) {
{
}
}
else {
{
  collapseNode_sbulk = true;
  numIntVars--;
  collapseNode_bi = true;
  numIntVars--;
  collapseNode_dbulk = true;
  numIntVars--;
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

  li_d = localLIDVec[nodeMap[cogendaNodeID_d]];
  li_g = localLIDVec[nodeMap[cogendaNodeID_g]];
  li_s = localLIDVec[nodeMap[cogendaNodeID_s]];
  li_b = localLIDVec[nodeMap[cogendaNodeID_b]];
  li_di = localLIDVec[nodeMap[cogendaNodeID_di]];
  li_si = localLIDVec[nodeMap[cogendaNodeID_si]];
  li_gi = localLIDVec[nodeMap[cogendaNodeID_gi]];
  li_gm = localLIDVec[nodeMap[cogendaNodeID_gm]];
  li_bi = localLIDVec[nodeMap[cogendaNodeID_bi]];
  li_sbulk = localLIDVec[nodeMap[cogendaNodeID_sbulk]];
  li_dbulk = localLIDVec[nodeMap[cogendaNodeID_dbulk]];
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
  /*The initial_step block stars...*/
  {
    T = ((_TEMPER_)*UNITFAD).val();
    BSIM4Adeff = ((0.0)*UNITFAD).val();
    BSIM4Aseff = ((0.0)*UNITFAD).val();
    BSIM4DslpFwd = ((0.0)*UNITFAD).val();
    BSIM4DslpRev = ((0.0)*UNITFAD).val();
    BSIM4DunitAreaTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4DunitLengthGateSidewallTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4DunitLengthSidewallTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4IVjdmFwd = ((0.0)*UNITFAD).val();
    BSIM4IVjdmRev = ((0.0)*UNITFAD).val();
    BSIM4IVjsmFwd = ((0.0)*UNITFAD).val();
    BSIM4IVjsmRev = ((0.0)*UNITFAD).val();
    BSIM4Pdeff = ((0.0)*UNITFAD).val();
    BSIM4Pseff = ((0.0)*UNITFAD).val();
    BSIM4SslpFwd = ((0.0)*UNITFAD).val();
    BSIM4SslpRev = ((0.0)*UNITFAD).val();
    BSIM4SunitAreaTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4SunitLengthGateSidewallTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4SunitLengthSidewallTempJctCap = ((0.0)*UNITFAD).val();
    BSIM4af = ((this->af)*UNITFAD).val();
    BSIM4kf = ((this->kf)*UNITFAD).val();
    BSIM4ef = ((this->ef)*UNITFAD).val();
    BSIM4em = ((this->em)*UNITFAD).val();
    BSIM4cbd = ((0.0)*UNITFAD).val();
    BSIM4cbs = ((0.0)*UNITFAD).val();
    BSIM4gbd = ((0.0)*UNITFAD).val();
    BSIM4gbs = ((0.0)*UNITFAD).val();
    BSIM4gcrg = ((0.0)*UNITFAD).val();
    BSIM4gcrgb = ((0.0)*UNITFAD).val();
    BSIM4gcrgd = ((0.0)*UNITFAD).val();
    BSIM4gcrgg = ((0.0)*UNITFAD).val();
    BSIM4grbdb = ((0.0)*UNITFAD).val();
    BSIM4grbpb = ((0.0)*UNITFAD).val();
    BSIM4grbpd = ((0.0)*UNITFAD).val();
    BSIM4grbps = ((0.0)*UNITFAD).val();
    BSIM4grbsb = ((0.0)*UNITFAD).val();
    BSIM4icVBS = ((0.0)*UNITFAD).val();
    BSIM4icVDS = ((0.0)*UNITFAD).val();
    BSIM4icVGS = ((0.0)*UNITFAD).val();
    BSIM4qbd = ((0.0)*UNITFAD).val();
    BSIM4qbs = ((0.0)*UNITFAD).val();
    BSIM4qchqs = ((0.0)*UNITFAD).val();
    BSIM4vjdmFwd = ((0.0)*UNITFAD).val();
    BSIM4vjdmRev = ((0.0)*UNITFAD).val();
    BSIM4vjsmFwd = ((0.0)*UNITFAD).val();
    BSIM4vjsmRev = ((0.0)*UNITFAD).val();
    DrainSatCurrent = ((0.0)*UNITFAD).val();
    Igd = ((0.0)*UNITFAD).val();
    Igs = ((0.0)*UNITFAD).val();
    Nvtmd = ((0.0)*UNITFAD).val();
    Rd = ((0.0)*UNITFAD).val();
    Rend = ((0.0)*UNITFAD).val();
    Rint = ((0.0)*UNITFAD).val();
    Rs = ((0.0)*UNITFAD).val();
    Vfb = ((0.0)*UNITFAD).val();
    Voxacc = ((0.0)*UNITFAD).val();
    Voxdepinv = ((0.0)*UNITFAD).val();
    nuEndD = ((0.0)*UNITFAD).val();
    nuEndS = ((0.0)*UNITFAD).val();
    nuIntD = ((0.0)*UNITFAD).val();
    nuIntS = ((0.0)*UNITFAD).val();
    qbulk = ((0.0)*UNITFAD).val();
    qdef = ((0.0)*UNITFAD).val();
    qdrn = ((0.0)*UNITFAD).val();
    qgate = ((0.0)*UNITFAD).val();
    tmp = ((0.0)*UNITFAD).val();
    dVdsat_dVb = ((0.0)*UNITFAD).val();
    dVdsat_dVd = ((0.0)*UNITFAD).val();
    dVdsat_dVg = ((0.0)*UNITFAD).val();
    dVdseff_dVb = ((0.0)*UNITFAD).val();
    dVdseff_dVd = ((0.0)*UNITFAD).val();
    dVdseff_dVg = ((0.0)*UNITFAD).val();
    dVgs_eff_dVg = ((0.0)*UNITFAD).val();
    dVgsteff_dVb = ((0.0)*UNITFAD).val();
    dVgsteff_dVd = ((0.0)*UNITFAD).val();
    dVgsteff_dVg = ((0.0)*UNITFAD).val();
    dVoxacc_dVb = ((0.0)*UNITFAD).val();
    dVoxacc_dVg = ((0.0)*UNITFAD).val();
    dVoxdepinv_dVb = ((0.0)*UNITFAD).val();
    dVoxdepinv_dVd = ((0.0)*UNITFAD).val();
    dVoxdepinv_dVg = ((0.0)*UNITFAD).val();
    dVth_dVb = ((0.0)*UNITFAD).val();
    dVth_dVd = ((0.0)*UNITFAD).val();
    dWeff_dVb = ((0.0)*UNITFAD).val();
    dWeff_dVg = ((0.0)*UNITFAD).val();
    dXdep_dVb = ((0.0)*UNITFAD).val();
    dltw_dVb = ((0.0)*UNITFAD).val();
    dn_dVb = ((0.0)*UNITFAD).val();
    dn_dVd = ((0.0)*UNITFAD).val();
    dsqrtPhis_dVb = ((0.0)*UNITFAD).val();
    dueff_dVb = ((0.0)*UNITFAD).val();
    dueff_dVd = ((0.0)*UNITFAD).val();
    dueff_dVg = ((0.0)*UNITFAD).val();
    grdsw = ((0.0)*UNITFAD).val();
    inv_grdsw = ((0.0)*UNITFAD).val();
    BSIM4gdtot = ((0.0)*UNITFAD).val();
    BSIM4gstot = ((0.0)*UNITFAD).val();
    BSIM4lxn = ((0.0)*UNITFAD).val();
    BSIM4pxn = ((0.0)*UNITFAD).val();
    BSIM4qinv = ((0.0)*UNITFAD).val();
    BSIM4tfactor = ((0.0)*UNITFAD).val();
    BSIM4wxn = ((0.0)*UNITFAD).val();
    ExpVxNVt = ((0.0)*UNITFAD).val();
    Gds = ((0.0)*UNITFAD).val();
    Gm = ((0.0)*UNITFAD).val();
    Gmb = ((0.0)*UNITFAD).val();
    Igc = ((0.0)*UNITFAD).val();
    Pigcd = ((0.0)*UNITFAD).val();
    T6 = ((0.0)*UNITFAD).val();
    Vaux = ((0.0)*UNITFAD).val();
    Vfbeff = ((0.0)*UNITFAD).val();
    dAbulk_dVb = ((0.0)*UNITFAD).val();
    dCclm_dVb = ((0.0)*UNITFAD).val();
    dCclm_dVd = ((0.0)*UNITFAD).val();
    dCclm_dVg = ((0.0)*UNITFAD).val();
    dDelt_vth_dVb = ((0.0)*UNITFAD).val();
    dEsatL_dVb = ((0.0)*UNITFAD).val();
    dEsatL_dVd = ((0.0)*UNITFAD).val();
    dEsatL_dVg = ((0.0)*UNITFAD).val();
    dFP_dVg = ((0.0)*UNITFAD).val();
    dIdl_dVb = ((0.0)*UNITFAD).val();
    dIdl_dVd = ((0.0)*UNITFAD).val();
    dIdl_dVg = ((0.0)*UNITFAD).val();
    dIdsa_dVb = ((0.0)*UNITFAD).val();
    dIdsa_dVd = ((0.0)*UNITFAD).val();
    dIdsa_dVg = ((0.0)*UNITFAD).val();
    dLambda_dVg = ((0.0)*UNITFAD).val();
    dPvagTerm_dVb = ((0.0)*UNITFAD).val();
    dPvagTerm_dVd = ((0.0)*UNITFAD).val();
    dPvagTerm_dVg = ((0.0)*UNITFAD).val();
    dRds_dVb = ((0.0)*UNITFAD).val();
    dRds_dVg = ((0.0)*UNITFAD).val();
    dTheta0_dVb = ((0.0)*UNITFAD).val();
    dVACLM_dVb = ((0.0)*UNITFAD).val();
    dVACLM_dVd = ((0.0)*UNITFAD).val();
    dVACLM_dVg = ((0.0)*UNITFAD).val();
    dVADIBL_dVb = ((0.0)*UNITFAD).val();
    dVADIBL_dVd = ((0.0)*UNITFAD).val();
    dVADIBL_dVg = ((0.0)*UNITFAD).val();
    dVASCBE_dVb = ((0.0)*UNITFAD).val();
    dVASCBE_dVd = ((0.0)*UNITFAD).val();
    dVASCBE_dVg = ((0.0)*UNITFAD).val();
    dVa_dVb = ((0.0)*UNITFAD).val();
    dVa_dVd = ((0.0)*UNITFAD).val();
    dVa_dVg = ((0.0)*UNITFAD).val();
    dVasat_dVb = ((0.0)*UNITFAD).val();
    dVasat_dVd = ((0.0)*UNITFAD).val();
    dVasat_dVg = ((0.0)*UNITFAD).val();
    dVbseff_dVb = ((0.0)*UNITFAD).val();
    BSIM4l = ((this->l)*UNITFAD).val();
    BSIM4w = ((this->w)*UNITFAD).val();
    BSIM4nf = ((this->nf)*UNITFAD).val();
    Ldrn = ((BSIM4l)*UNITFAD).val();
    BSIM4lc = ((this->lc)*UNITFAD).val();
    BSIM4llodku0 = ((this->llodku0)*UNITFAD).val();
    BSIM4wlodku0 = ((this->wlodku0)*UNITFAD).val();
    BSIM4rnoia = ((this->rnoia)*UNITFAD).val();
    BSIM4rnoib = ((this->rnoib)*UNITFAD).val();
    BSIM4rnoic = ((this->rnoic)*UNITFAD).val();
    BSIM4min = ((this->_min)*UNITFAD).val();
    BSIM4drainArea = ((this->ad)*UNITFAD).val();
    BSIM4sourceArea = ((this->as)*UNITFAD).val();
    BSIM4drainSquares = ((this->nrd)*UNITFAD).val();
    BSIM4sourceSquares = ((this->nrs)*UNITFAD).val();
    BSIM4sourcePerimeter = ((this->ps)*UNITFAD).val();
    BSIM4drainPerimeter = ((this->pd)*UNITFAD).val();
    BSIM4off = ((this->off)*UNITFAD).val();
    BSIM4trnqsMod = ((this->trnqsmod)*UNITFAD).val();
    BSIM4acnqsMod = ((this->acnqsmod)*UNITFAD).val();
    BSIM4rbodyMod = ((this->rbodymod)*UNITFAD).val();
    BSIM4rgateMod = ((this->rgatemod)*UNITFAD).val();
    BSIM4geoMod = ((this->geomod)*UNITFAD).val();
    BSIM4rgeoMod = ((this->rgeomod)*UNITFAD).val();
    BSIM4capMod = ((this->capmod)*UNITFAD).val();
    BSIM4dioMod = ((this->diomod)*UNITFAD).val();
    BSIM4rdsMod = ((this->rdsmod)*UNITFAD).val();
    BSIM4fnoiMod = ((this->fnoimod)*UNITFAD).val();
    BSIM4tnoiMod = ((this->tnoimod)*UNITFAD).val();
    BSIM4igcMod = ((this->igcmod)*UNITFAD).val();
    BSIM4igbMod = ((this->igbmod)*UNITFAD).val();
    BSIM4type = ((this->type)*UNITFAD).val();
    BSIM4mobMod = ((this->mobmod)*UNITFAD).val();
    BSIM4tempMod = ((this->tempmod)*UNITFAD).val();
    BSIM4binUnit = ((this->binunit)*UNITFAD).val();
    BSIM4paramChk = ((this->paramchk)*UNITFAD).val();
    BSIM4perMod = ((this->permod)*UNITFAD).val();
    BSIM4version = ((this->version)*UNITFAD).val();
    BSIM4toxe = ((this->toxe)*UNITFAD).val();
    BSIM4toxp = ((this->toxp)*UNITFAD).val();
    BSIM4toxm = ((this->toxm)*UNITFAD).val();
    BSIM4dtox = ((this->dtox)*UNITFAD).val();
    BSIM4epsrox = ((this->epsrox)*UNITFAD).val();
    BSIM4cdsc = ((this->cdsc)*UNITFAD).val();
    BSIM4cdscb = ((this->cdscb)*UNITFAD).val();
    BSIM4cdscd = ((this->cdscd)*UNITFAD).val();
    BSIM4cit = ((this->cit)*UNITFAD).val();
    BSIM4nfactor = ((this->nfactor)*UNITFAD).val();
    BSIM4xj = ((this->xj)*UNITFAD).val();
    BSIM4vsat = ((this->vsat)*UNITFAD).val();
    BSIM4at = ((this->at)*UNITFAD).val();
    BSIM4a0 = ((this->a0)*UNITFAD).val();
    BSIM4ags = ((this->ags)*UNITFAD).val();
    BSIM4a1 = ((this->a1)*UNITFAD).val();
    BSIM4a2 = ((this->a2)*UNITFAD).val();
    BSIM4keta = ((this->keta)*UNITFAD).val();
    BSIM4nsub = ((this->nsub)*UNITFAD).val();
    BSIM4ndep = ((this->ndep)*UNITFAD).val();
    BSIM4nsd = ((this->nsd)*UNITFAD).val();
    BSIM4phin = ((this->phin)*UNITFAD).val();
    BSIM4ngate = ((this->ngate)*UNITFAD).val();
    BSIM4gamma1 = ((this->gamma1)*UNITFAD).val();
    BSIM4gamma2 = ((this->gamma2)*UNITFAD).val();
    BSIM4vbx = ((this->vbx)*UNITFAD).val();
    BSIM4vbm = ((this->vbm)*UNITFAD).val();
    BSIM4xt = ((this->xt)*UNITFAD).val();
    BSIM4k1 = ((this->k1)*UNITFAD).val();
    BSIM4kt1 = ((this->kt1)*UNITFAD).val();
    BSIM4kt1l = ((this->kt1l)*UNITFAD).val();
    BSIM4kt2 = ((this->kt2)*UNITFAD).val();
    BSIM4k2 = ((this->k2)*UNITFAD).val();
    BSIM4k3 = ((this->k3)*UNITFAD).val();
    BSIM4k3b = ((this->k3b)*UNITFAD).val();
    BSIM4w0 = ((this->w0)*UNITFAD).val();
    BSIM4dvtp0 = ((this->dvtp0)*UNITFAD).val();
    BSIM4dvtp1 = ((this->dvtp1)*UNITFAD).val();
    BSIM4lpe0 = ((this->lpe0)*UNITFAD).val();
    BSIM4lpeb = ((this->lpeb)*UNITFAD).val();
    BSIM4dvt0 = ((this->dvt0)*UNITFAD).val();
    BSIM4dvt1 = ((this->dvt1)*UNITFAD).val();
    BSIM4dvt2 = ((this->dvt2)*UNITFAD).val();
    BSIM4dvt0w = ((this->dvt0w)*UNITFAD).val();
    BSIM4dvt1w = ((this->dvt1w)*UNITFAD).val();
    BSIM4dvt2w = ((this->dvt2w)*UNITFAD).val();
    BSIM4drout = ((this->drout)*UNITFAD).val();
    BSIM4dsub = ((this->dsub)*UNITFAD).val();
    BSIM4vth0 = ((this->vth0)*UNITFAD).val();
    BSIM4eu = ((this->eu)*UNITFAD).val();
    BSIM4ua = ((this->ua)*UNITFAD).val();
    BSIM4ua1 = ((this->ua1)*UNITFAD).val();
    BSIM4ub = ((this->ub)*UNITFAD).val();
    BSIM4ub1 = ((this->ub1)*UNITFAD).val();
    BSIM4uc = ((this->uc)*UNITFAD).val();
    BSIM4uc1 = ((this->uc1)*UNITFAD).val();
    BSIM4u0 = ((this->u0)*UNITFAD).val();
    BSIM4ute = ((this->ute)*UNITFAD).val();
    BSIM4voff = ((this->voff)*UNITFAD).val();
    BSIM4minv = ((this->minv)*UNITFAD).val();
    BSIM4voffl = ((this->voffl)*UNITFAD).val();
    BSIM4delta = ((this->delta)*UNITFAD).val();
    BSIM4rdsw = ((this->rdsw)*UNITFAD).val();
    BSIM4rdswmin = ((this->rdswmin)*UNITFAD).val();
    BSIM4rdwmin = ((this->rdwmin)*UNITFAD).val();
    BSIM4rswmin = ((this->rswmin)*UNITFAD).val();
    BSIM4rsw = ((this->rsw)*UNITFAD).val();
    BSIM4rdw = ((this->rdw)*UNITFAD).val();
    BSIM4prwg = ((this->prwg)*UNITFAD).val();
    BSIM4prwb = ((this->prwb)*UNITFAD).val();
    BSIM4prt = ((this->prt)*UNITFAD).val();
    BSIM4eta0 = ((this->eta0)*UNITFAD).val();
    BSIM4etab = ((this->etab)*UNITFAD).val();
    BSIM4pclm = ((this->pclm)*UNITFAD).val();
    BSIM4pdibl1 = ((this->pdiblc1)*UNITFAD).val();
    BSIM4pdibl2 = ((this->pdiblc2)*UNITFAD).val();
    BSIM4pdiblb = ((this->pdiblcb)*UNITFAD).val();
    BSIM4lpdibl1 = ((this->lpdiblc1)*UNITFAD).val();
    BSIM4lpdibl2 = ((this->lpdiblc2)*UNITFAD).val();
    BSIM4lpdiblb = ((this->lpdiblcb)*UNITFAD).val();
    BSIM4wpdibl1 = ((this->wpdiblc1)*UNITFAD).val();
    BSIM4wpdibl2 = ((this->wpdiblc2)*UNITFAD).val();
    BSIM4wpdiblb = ((this->wpdiblcb)*UNITFAD).val();
    BSIM4ppdibl1 = ((this->ppdiblc1)*UNITFAD).val();
    BSIM4ppdibl2 = ((this->ppdiblc2)*UNITFAD).val();
    BSIM4ppdiblb = ((this->ppdiblcb)*UNITFAD).val();
    BSIM4fprout = ((this->fprout)*UNITFAD).val();
    BSIM4pdits = ((this->pdits)*UNITFAD).val();
    BSIM4pditsd = ((this->pditsd)*UNITFAD).val();
    BSIM4pditsl = ((this->pditsl)*UNITFAD).val();
    BSIM4pscbe1 = ((this->pscbe1)*UNITFAD).val();
    BSIM4pscbe2 = ((this->pscbe2)*UNITFAD).val();
    BSIM4pvag = ((this->pvag)*UNITFAD).val();
    BSIM4wr = ((this->wr)*UNITFAD).val();
    BSIM4dwg = ((this->dwg)*UNITFAD).val();
    BSIM4dwb = ((this->dwb)*UNITFAD).val();
    BSIM4b0 = ((this->b0)*UNITFAD).val();
    BSIM4b1 = ((this->b1)*UNITFAD).val();
    BSIM4alpha0 = ((this->alpha0)*UNITFAD).val();
    BSIM4alpha1 = ((this->alpha1)*UNITFAD).val();
    BSIM4beta0 = ((this->beta0)*UNITFAD).val();
    BSIM4agidl = ((this->agidl)*UNITFAD).val();
    BSIM4bgidl = ((this->bgidl)*UNITFAD).val();
    BSIM4cgidl = ((this->cgidl)*UNITFAD).val();
    BSIM4egidl = ((this->egidl)*UNITFAD).val();
    BSIM4aigc = ((this->aigc)*UNITFAD).val();
    BSIM4bigc = ((this->bigc)*UNITFAD).val();
    BSIM4cigc = ((this->cigc)*UNITFAD).val();
    BSIM4aigsd = ((this->aigsd)*UNITFAD).val();
    BSIM4bigsd = ((this->bigsd)*UNITFAD).val();
    BSIM4cigsd = ((this->cigsd)*UNITFAD).val();
    BSIM4aigbacc = ((this->aigbacc)*UNITFAD).val();
    BSIM4bigbacc = ((this->bigbacc)*UNITFAD).val();
    BSIM4cigbacc = ((this->cigbacc)*UNITFAD).val();
    BSIM4aigbinv = ((this->aigbinv)*UNITFAD).val();
    BSIM4bigbinv = ((this->bigbinv)*UNITFAD).val();
    BSIM4cigbinv = ((this->cigbinv)*UNITFAD).val();
    BSIM4nigc = ((this->nigc)*UNITFAD).val();
    BSIM4nigbacc = ((this->nigbacc)*UNITFAD).val();
    BSIM4nigbinv = ((this->nigbinv)*UNITFAD).val();
    BSIM4ntox = ((this->ntox)*UNITFAD).val();
    BSIM4eigbinv = ((this->eigbinv)*UNITFAD).val();
    BSIM4pigcd = ((this->pigcd)*UNITFAD).val();
    BSIM4poxedge = ((this->poxedge)*UNITFAD).val();
    BSIM4toxref = ((this->toxref)*UNITFAD).val();
    BSIM4ijthdfwd = ((this->ijthdfwd)*UNITFAD).val();
    BSIM4ijthsfwd = ((this->ijthsfwd)*UNITFAD).val();
    BSIM4ijthdrev = ((this->ijthdrev)*UNITFAD).val();
    BSIM4ijthsrev = ((this->ijthsrev)*UNITFAD).val();
    BSIM4xjbvd = ((this->xjbvd)*UNITFAD).val();
    BSIM4xjbvs = ((this->xjbvs)*UNITFAD).val();
    BSIM4bvd = ((this->bvd)*UNITFAD).val();
    BSIM4bvs = ((this->bvs)*UNITFAD).val();
    BSIM4xrcrg1 = ((this->xrcrg1)*UNITFAD).val();
    BSIM4xrcrg2 = ((this->xrcrg2)*UNITFAD).val();
    BSIM4vfb = ((this->vfb)*UNITFAD).val();
    BSIM4gbmin = ((this->gbmin)*UNITFAD).val();
    BSIM4rbdb = ((this->rbdb)*UNITFAD).val();
    BSIM4rbsb = ((this->rbsb)*UNITFAD).val();
    BSIM4rbpb = ((this->rbpb)*UNITFAD).val();
    BSIM4rbps = ((this->rbps)*UNITFAD).val();
    BSIM4rbpd = ((this->rbpd)*UNITFAD).val();
    BSIM4oxideTrapDensityA = ((this->noia)*UNITFAD).val();
    BSIM4oxideTrapDensityB = ((this->noib)*UNITFAD).val();
    BSIM4oxideTrapDensityC = ((this->noic)*UNITFAD).val();
    BSIM4tnoia = ((this->tnoia)*UNITFAD).val();
    BSIM4tnoib = ((this->tnoib)*UNITFAD).val();
    BSIM4tnoic = ((this->tnoic)*UNITFAD).val();
    BSIM4ntnoi = ((this->ntnoi)*UNITFAD).val();
    BSIM4cgsl = ((this->cgsl)*UNITFAD).val();
    BSIM4cgdl = ((this->cgdl)*UNITFAD).val();
    BSIM4ckappas = ((this->ckappas)*UNITFAD).val();
    BSIM4ckappad = ((this->ckappad)*UNITFAD).val();
    BSIM4cf = ((this->cf)*UNITFAD).val();
    BSIM4vfbcv = ((this->vfbcv)*UNITFAD).val();
    BSIM4clc = ((this->clc)*UNITFAD).val();
    BSIM4cle = ((this->cle)*UNITFAD).val();
    BSIM4dwc = ((this->dwc)*UNITFAD).val();
    BSIM4dlc = ((this->dlc)*UNITFAD).val();
    BSIM4xw = ((this->xw)*UNITFAD).val();
    BSIM4xl = ((this->xl)*UNITFAD).val();
    BSIM4dlcig = ((this->dlcig)*UNITFAD).val();
    BSIM4dlcigd = ((this->dlcigd)*UNITFAD).val();
    BSIM4dwj = ((this->dwj)*UNITFAD).val();
    BSIM4noff = ((this->noff)*UNITFAD).val();
    BSIM4voffcv = ((this->voffcv)*UNITFAD).val();
    BSIM4acde = ((this->acde)*UNITFAD).val();
    BSIM4moin = ((this->moin)*UNITFAD).val();
    BSIM4tcj = ((this->tcj)*UNITFAD).val();
    BSIM4tcjsw = ((this->tcjsw)*UNITFAD).val();
    BSIM4tcjswg = ((this->tcjswg)*UNITFAD).val();
    BSIM4tpb = ((this->tpb)*UNITFAD).val();
    BSIM4tpbsw = ((this->tpbsw)*UNITFAD).val();
    BSIM4tpbswg = ((this->tpbswg)*UNITFAD).val();
    BSIM4dmcg = ((this->dmcg)*UNITFAD).val();
    BSIM4dmci = ((this->dmci)*UNITFAD).val();
    BSIM4dmdg = ((this->dmdg)*UNITFAD).val();
    BSIM4dmcgt = ((this->dmcgt)*UNITFAD).val();
    BSIM4xgw = ((this->xgw)*UNITFAD).val();
    BSIM4xgl = ((this->xgl)*UNITFAD).val();
    BSIM4rsh = ((this->rsh)*UNITFAD).val();
    BSIM4rshg = ((this->rshg)*UNITFAD).val();
    BSIM4ngcon = ((this->ngcon)*UNITFAD).val();
    BSIM4lcdsc = ((this->lcdsc)*UNITFAD).val();
    BSIM4lcdscb = ((this->lcdscb)*UNITFAD).val();
    BSIM4lcdscd = ((this->lcdscd)*UNITFAD).val();
    BSIM4lcit = ((this->lcit)*UNITFAD).val();
    BSIM4lnfactor = ((this->lnfactor)*UNITFAD).val();
    BSIM4lxj = ((this->lxj)*UNITFAD).val();
    BSIM4lvsat = ((this->lvsat)*UNITFAD).val();
    BSIM4lat = ((this->lat)*UNITFAD).val();
    BSIM4la0 = ((this->la0)*UNITFAD).val();
    BSIM4lags = ((this->lags)*UNITFAD).val();
    BSIM4la1 = ((this->la1)*UNITFAD).val();
    BSIM4la2 = ((this->la2)*UNITFAD).val();
    BSIM4lketa = ((this->lketa)*UNITFAD).val();
    BSIM4lnsub = ((this->lnsub)*UNITFAD).val();
    BSIM4lndep = ((this->lndep)*UNITFAD).val();
    BSIM4lnsd = ((this->lnsd)*UNITFAD).val();
    BSIM4lphin = ((this->lphin)*UNITFAD).val();
    BSIM4lngate = ((this->lngate)*UNITFAD).val();
    BSIM4lgamma1 = ((this->lgamma1)*UNITFAD).val();
    BSIM4lgamma2 = ((this->lgamma2)*UNITFAD).val();
    BSIM4lvbx = ((this->lvbx)*UNITFAD).val();
    BSIM4lvbm = ((this->lvbm)*UNITFAD).val();
    BSIM4lxt = ((this->lxt)*UNITFAD).val();
    BSIM4lk1 = ((this->lk1)*UNITFAD).val();
    BSIM4lkt1 = ((this->lkt1)*UNITFAD).val();
    BSIM4lkt1l = ((this->lkt1l)*UNITFAD).val();
    BSIM4lkt2 = ((this->lkt2)*UNITFAD).val();
    BSIM4lk2 = ((this->lk2)*UNITFAD).val();
    BSIM4lk3 = ((this->lk3)*UNITFAD).val();
    BSIM4lk3b = ((this->lk3b)*UNITFAD).val();
    BSIM4lw0 = ((this->lw0)*UNITFAD).val();
    BSIM4ldvtp0 = ((this->ldvtp0)*UNITFAD).val();
    BSIM4ldvtp1 = ((this->ldvtp1)*UNITFAD).val();
    BSIM4llpe0 = ((this->llpe0)*UNITFAD).val();
    BSIM4llpeb = ((this->llpeb)*UNITFAD).val();
    BSIM4ldvt0 = ((this->ldvt0)*UNITFAD).val();
    BSIM4ldvt1 = ((this->ldvt1)*UNITFAD).val();
    BSIM4ldvt2 = ((this->ldvt2)*UNITFAD).val();
    BSIM4ldvt0w = ((this->ldvt0w)*UNITFAD).val();
    BSIM4ldvt1w = ((this->ldvt1w)*UNITFAD).val();
    BSIM4ldvt2w = ((this->ldvt2w)*UNITFAD).val();
    BSIM4ldrout = ((this->ldrout)*UNITFAD).val();
    BSIM4ldsub = ((this->ldsub)*UNITFAD).val();
    BSIM4lvth0 = ((this->lvth0)*UNITFAD).val();
    BSIM4lua = ((this->lua)*UNITFAD).val();
    BSIM4lua1 = ((this->lua1)*UNITFAD).val();
    BSIM4lub = ((this->lub)*UNITFAD).val();
    BSIM4lub1 = ((this->lub1)*UNITFAD).val();
    BSIM4luc = ((this->luc)*UNITFAD).val();
    BSIM4luc1 = ((this->luc1)*UNITFAD).val();
    BSIM4lu0 = ((this->lu0)*UNITFAD).val();
    BSIM4leu = ((this->leu)*UNITFAD).val();
    BSIM4lute = ((this->lute)*UNITFAD).val();
    BSIM4lvoff = ((this->lvoff)*UNITFAD).val();
    BSIM4lminv = ((this->lminv)*UNITFAD).val();
    BSIM4ldelta = ((this->ldelta)*UNITFAD).val();
    BSIM4lrdsw = ((this->lrdsw)*UNITFAD).val();
    BSIM4lrsw = ((this->lrsw)*UNITFAD).val();
    BSIM4lrdw = ((this->lrdw)*UNITFAD).val();
    BSIM4lprwg = ((this->lprwg)*UNITFAD).val();
    BSIM4lprwb = ((this->lprwb)*UNITFAD).val();
    BSIM4lprt = ((this->lprt)*UNITFAD).val();
    BSIM4leta0 = ((this->leta0)*UNITFAD).val();
    BSIM4letab = ((this->letab)*UNITFAD).val();
    BSIM4lpclm = ((this->lpclm)*UNITFAD).val();
    BSIM4lfprout = ((this->lfprout)*UNITFAD).val();
    BSIM4lpdits = ((this->lpdits)*UNITFAD).val();
    BSIM4lpditsd = ((this->lpditsd)*UNITFAD).val();
    BSIM4lpscbe1 = ((this->lpscbe1)*UNITFAD).val();
    BSIM4lpscbe2 = ((this->lpscbe2)*UNITFAD).val();
    BSIM4lpvag = ((this->lpvag)*UNITFAD).val();
    BSIM4lwr = ((this->lwr)*UNITFAD).val();
    BSIM4ldwg = ((this->ldwg)*UNITFAD).val();
    BSIM4ldwb = ((this->ldwb)*UNITFAD).val();
    BSIM4lb0 = ((this->lb0)*UNITFAD).val();
    BSIM4lb1 = ((this->lb1)*UNITFAD).val();
    BSIM4lalpha0 = ((this->lalpha0)*UNITFAD).val();
    BSIM4lalpha1 = ((this->lalpha1)*UNITFAD).val();
    BSIM4lbeta0 = ((this->lbeta0)*UNITFAD).val();
    BSIM4lvfb = ((this->lvfb)*UNITFAD).val();
    BSIM4lagidl = ((this->lagidl)*UNITFAD).val();
    BSIM4lbgidl = ((this->lbgidl)*UNITFAD).val();
    BSIM4lcgidl = ((this->lcgidl)*UNITFAD).val();
    BSIM4legidl = ((this->legidl)*UNITFAD).val();
    BSIM4laigc = ((this->laigc)*UNITFAD).val();
    BSIM4lbigc = ((this->lbigc)*UNITFAD).val();
    BSIM4lcigc = ((this->lcigc)*UNITFAD).val();
    BSIM4laigsd = ((this->laigsd)*UNITFAD).val();
    BSIM4lbigsd = ((this->lbigsd)*UNITFAD).val();
    BSIM4lcigsd = ((this->lcigsd)*UNITFAD).val();
    BSIM4laigbacc = ((this->laigbacc)*UNITFAD).val();
    BSIM4lbigbacc = ((this->lbigbacc)*UNITFAD).val();
    BSIM4lcigbacc = ((this->lcigbacc)*UNITFAD).val();
    BSIM4laigbinv = ((this->laigbinv)*UNITFAD).val();
    BSIM4lbigbinv = ((this->lbigbinv)*UNITFAD).val();
    BSIM4lcigbinv = ((this->lcigbinv)*UNITFAD).val();
    BSIM4lnigc = ((this->lnigc)*UNITFAD).val();
    BSIM4lnigbacc = ((this->lnigbacc)*UNITFAD).val();
    BSIM4lnigbinv = ((this->lnigbinv)*UNITFAD).val();
    BSIM4lntox = ((this->lntox)*UNITFAD).val();
    BSIM4leigbinv = ((this->leigbinv)*UNITFAD).val();
    BSIM4lpigcd = ((this->lpigcd)*UNITFAD).val();
    BSIM4lpoxedge = ((this->lpoxedge)*UNITFAD).val();
    BSIM4lxrcrg1 = ((this->lxrcrg1)*UNITFAD).val();
    BSIM4lxrcrg2 = ((this->lxrcrg2)*UNITFAD).val();
    BSIM4lcgsl = ((this->lcgsl)*UNITFAD).val();
    BSIM4lcgdl = ((this->lcgdl)*UNITFAD).val();
    BSIM4lckappas = ((this->lckappas)*UNITFAD).val();
    BSIM4lckappad = ((this->lckappad)*UNITFAD).val();
    BSIM4lcf = ((this->lcf)*UNITFAD).val();
    BSIM4lclc = ((this->lclc)*UNITFAD).val();
    BSIM4lcle = ((this->lcle)*UNITFAD).val();
    BSIM4lvfbcv = ((this->lvfbcv)*UNITFAD).val();
    BSIM4lnoff = ((this->lnoff)*UNITFAD).val();
    BSIM4lvoffcv = ((this->lvoffcv)*UNITFAD).val();
    BSIM4lacde = ((this->lacde)*UNITFAD).val();
    BSIM4lmoin = ((this->lmoin)*UNITFAD).val();
    BSIM4wcdsc = ((this->wcdsc)*UNITFAD).val();
    BSIM4wcdscb = ((this->wcdscb)*UNITFAD).val();
    BSIM4wcdscd = ((this->wcdscd)*UNITFAD).val();
    BSIM4wcit = ((this->wcit)*UNITFAD).val();
    BSIM4wnfactor = ((this->wnfactor)*UNITFAD).val();
    BSIM4wxj = ((this->wxj)*UNITFAD).val();
    BSIM4wvsat = ((this->wvsat)*UNITFAD).val();
    BSIM4wat = ((this->wat)*UNITFAD).val();
    BSIM4wa0 = ((this->wa0)*UNITFAD).val();
    BSIM4wags = ((this->wags)*UNITFAD).val();
    BSIM4wa1 = ((this->wa1)*UNITFAD).val();
    BSIM4wa2 = ((this->wa2)*UNITFAD).val();
    BSIM4wketa = ((this->wketa)*UNITFAD).val();
    BSIM4wnsub = ((this->wnsub)*UNITFAD).val();
    BSIM4wndep = ((this->wndep)*UNITFAD).val();
    BSIM4wnsd = ((this->wnsd)*UNITFAD).val();
    BSIM4wphin = ((this->wphin)*UNITFAD).val();
    BSIM4wngate = ((this->wngate)*UNITFAD).val();
    BSIM4wgamma1 = ((this->wgamma1)*UNITFAD).val();
    BSIM4wgamma2 = ((this->wgamma2)*UNITFAD).val();
    BSIM4wvbx = ((this->wvbx)*UNITFAD).val();
    BSIM4wvbm = ((this->wvbm)*UNITFAD).val();
    BSIM4wxt = ((this->wxt)*UNITFAD).val();
    BSIM4wk1 = ((this->wk1)*UNITFAD).val();
    BSIM4wkt1 = ((this->wkt1)*UNITFAD).val();
    BSIM4wkt1l = ((this->wkt1l)*UNITFAD).val();
    BSIM4wkt2 = ((this->wkt2)*UNITFAD).val();
    BSIM4wk2 = ((this->wk2)*UNITFAD).val();
    BSIM4wk3 = ((this->wk3)*UNITFAD).val();
    BSIM4wk3b = ((this->wk3b)*UNITFAD).val();
    BSIM4ww0 = ((this->ww0)*UNITFAD).val();
    BSIM4wdvtp0 = ((this->wdvtp0)*UNITFAD).val();
    BSIM4wdvtp1 = ((this->wdvtp1)*UNITFAD).val();
    BSIM4wlpe0 = ((this->wlpe0)*UNITFAD).val();
    BSIM4wlpeb = ((this->wlpeb)*UNITFAD).val();
    BSIM4wdvt0 = ((this->wdvt0)*UNITFAD).val();
    BSIM4wdvt1 = ((this->wdvt1)*UNITFAD).val();
    BSIM4wdvt2 = ((this->wdvt2)*UNITFAD).val();
    BSIM4wdvt0w = ((this->wdvt0w)*UNITFAD).val();
    BSIM4wdvt1w = ((this->wdvt1w)*UNITFAD).val();
    BSIM4wdvt2w = ((this->wdvt2w)*UNITFAD).val();
    BSIM4wdrout = ((this->wdrout)*UNITFAD).val();
    BSIM4wdsub = ((this->wdsub)*UNITFAD).val();
    BSIM4wvth0 = ((this->wvth0)*UNITFAD).val();
    BSIM4wua = ((this->wua)*UNITFAD).val();
    BSIM4wua1 = ((this->wua1)*UNITFAD).val();
    BSIM4wub = ((this->wub)*UNITFAD).val();
    BSIM4wub1 = ((this->wub1)*UNITFAD).val();
    BSIM4wuc = ((this->wuc)*UNITFAD).val();
    BSIM4wuc1 = ((this->wuc1)*UNITFAD).val();
    BSIM4wu0 = ((this->wu0)*UNITFAD).val();
    BSIM4weu = ((this->weu)*UNITFAD).val();
    BSIM4wute = ((this->wute)*UNITFAD).val();
    BSIM4wvoff = ((this->wvoff)*UNITFAD).val();
    BSIM4wminv = ((this->wminv)*UNITFAD).val();
    BSIM4wdelta = ((this->wdelta)*UNITFAD).val();
    BSIM4wrdsw = ((this->wrdsw)*UNITFAD).val();
    BSIM4wrsw = ((this->wrsw)*UNITFAD).val();
    BSIM4wrdw = ((this->wrdw)*UNITFAD).val();
    BSIM4wprwg = ((this->wprwg)*UNITFAD).val();
    BSIM4wprwb = ((this->wprwb)*UNITFAD).val();
    BSIM4wprt = ((this->wprt)*UNITFAD).val();
    BSIM4weta0 = ((this->weta0)*UNITFAD).val();
    BSIM4wetab = ((this->wetab)*UNITFAD).val();
    BSIM4wpclm = ((this->wpclm)*UNITFAD).val();
    BSIM4wfprout = ((this->wfprout)*UNITFAD).val();
    BSIM4wpdits = ((this->wpdits)*UNITFAD).val();
    BSIM4wpditsd = ((this->wpditsd)*UNITFAD).val();
    BSIM4wpscbe1 = ((this->wpscbe1)*UNITFAD).val();
    BSIM4wpscbe2 = ((this->wpscbe2)*UNITFAD).val();
    BSIM4wpvag = ((this->wpvag)*UNITFAD).val();
    BSIM4wwr = ((this->wwr)*UNITFAD).val();
    BSIM4wdwg = ((this->wdwg)*UNITFAD).val();
    BSIM4wdwb = ((this->wdwb)*UNITFAD).val();
    BSIM4wb0 = ((this->wb0)*UNITFAD).val();
    BSIM4wb1 = ((this->wb1)*UNITFAD).val();
    BSIM4walpha0 = ((this->walpha0)*UNITFAD).val();
    BSIM4walpha1 = ((this->walpha1)*UNITFAD).val();
    BSIM4wbeta0 = ((this->wbeta0)*UNITFAD).val();
    BSIM4wvfb = ((this->wvfb)*UNITFAD).val();
    BSIM4wagidl = ((this->wagidl)*UNITFAD).val();
    BSIM4wbgidl = ((this->wbgidl)*UNITFAD).val();
    BSIM4wcgidl = ((this->wcgidl)*UNITFAD).val();
    BSIM4wegidl = ((this->wegidl)*UNITFAD).val();
    BSIM4waigc = ((this->waigc)*UNITFAD).val();
    BSIM4wbigc = ((this->wbigc)*UNITFAD).val();
    BSIM4wcigc = ((this->wcigc)*UNITFAD).val();
    BSIM4waigsd = ((this->waigsd)*UNITFAD).val();
    BSIM4wbigsd = ((this->wbigsd)*UNITFAD).val();
    BSIM4wcigsd = ((this->wcigsd)*UNITFAD).val();
    BSIM4waigbacc = ((this->waigbacc)*UNITFAD).val();
    BSIM4wbigbacc = ((this->wbigbacc)*UNITFAD).val();
    BSIM4wcigbacc = ((this->wcigbacc)*UNITFAD).val();
    BSIM4waigbinv = ((this->waigbinv)*UNITFAD).val();
    BSIM4wbigbinv = ((this->wbigbinv)*UNITFAD).val();
    BSIM4wcigbinv = ((this->wcigbinv)*UNITFAD).val();
    BSIM4wnigc = ((this->wnigc)*UNITFAD).val();
    BSIM4wnigbacc = ((this->wnigbacc)*UNITFAD).val();
    BSIM4wnigbinv = ((this->wnigbinv)*UNITFAD).val();
    BSIM4wntox = ((this->wntox)*UNITFAD).val();
    BSIM4weigbinv = ((this->weigbinv)*UNITFAD).val();
    BSIM4wpigcd = ((this->wpigcd)*UNITFAD).val();
    BSIM4wpoxedge = ((this->wpoxedge)*UNITFAD).val();
    BSIM4wxrcrg1 = ((this->wxrcrg1)*UNITFAD).val();
    BSIM4wxrcrg2 = ((this->wxrcrg2)*UNITFAD).val();
    BSIM4wcgsl = ((this->wcgsl)*UNITFAD).val();
    BSIM4wcgdl = ((this->wcgdl)*UNITFAD).val();
    BSIM4wckappas = ((this->wckappas)*UNITFAD).val();
    BSIM4wckappad = ((this->wckappad)*UNITFAD).val();
    BSIM4wcf = ((this->wcf)*UNITFAD).val();
    BSIM4wclc = ((this->wclc)*UNITFAD).val();
    BSIM4wcle = ((this->wcle)*UNITFAD).val();
    BSIM4wvfbcv = ((this->wvfbcv)*UNITFAD).val();
    BSIM4wnoff = ((this->wnoff)*UNITFAD).val();
    BSIM4wvoffcv = ((this->wvoffcv)*UNITFAD).val();
    BSIM4wacde = ((this->wacde)*UNITFAD).val();
    BSIM4wmoin = ((this->wmoin)*UNITFAD).val();
    BSIM4pcdsc = ((this->pcdsc)*UNITFAD).val();
    BSIM4pcdscb = ((this->pcdscb)*UNITFAD).val();
    BSIM4pcdscd = ((this->pcdscd)*UNITFAD).val();
    BSIM4pcit = ((this->pcit)*UNITFAD).val();
    BSIM4pnfactor = ((this->pnfactor)*UNITFAD).val();
    BSIM4pxj = ((this->pxj)*UNITFAD).val();
    BSIM4pvsat = ((this->pvsat)*UNITFAD).val();
    BSIM4pat = ((this->pat)*UNITFAD).val();
    BSIM4pa0 = ((this->pa0)*UNITFAD).val();
    BSIM4pags = ((this->pags)*UNITFAD).val();
    BSIM4pa1 = ((this->pa1)*UNITFAD).val();
    BSIM4pa2 = ((this->pa2)*UNITFAD).val();
    BSIM4pketa = ((this->pketa)*UNITFAD).val();
    BSIM4pnsub = ((this->pnsub)*UNITFAD).val();
    BSIM4pndep = ((this->pndep)*UNITFAD).val();
    BSIM4pnsd = ((this->pnsd)*UNITFAD).val();
    BSIM4pphin = ((this->pphin)*UNITFAD).val();
    BSIM4pngate = ((this->pngate)*UNITFAD).val();
    BSIM4pgamma1 = ((this->pgamma1)*UNITFAD).val();
    BSIM4pgamma2 = ((this->pgamma2)*UNITFAD).val();
    BSIM4pvbx = ((this->pvbx)*UNITFAD).val();
    BSIM4pvbm = ((this->pvbm)*UNITFAD).val();
    BSIM4pxt = ((this->pxt)*UNITFAD).val();
    BSIM4pk1 = ((this->pk1)*UNITFAD).val();
    BSIM4pkt1 = ((this->pkt1)*UNITFAD).val();
    BSIM4pkt1l = ((this->pkt1l)*UNITFAD).val();
    BSIM4pkt2 = ((this->pkt2)*UNITFAD).val();
    BSIM4pk2 = ((this->pk2)*UNITFAD).val();
    BSIM4pk3 = ((this->pk3)*UNITFAD).val();
    BSIM4pk3b = ((this->pk3b)*UNITFAD).val();
    BSIM4pw0 = ((this->pw0)*UNITFAD).val();
    BSIM4pdvtp0 = ((this->pdvtp0)*UNITFAD).val();
    BSIM4pdvtp1 = ((this->pdvtp1)*UNITFAD).val();
    BSIM4plpe0 = ((this->plpe0)*UNITFAD).val();
    BSIM4plpeb = ((this->plpeb)*UNITFAD).val();
    BSIM4pdvt0 = ((this->pdvt0)*UNITFAD).val();
    BSIM4pdvt1 = ((this->pdvt1)*UNITFAD).val();
    BSIM4pdvt2 = ((this->pdvt2)*UNITFAD).val();
    BSIM4pdvt0w = ((this->pdvt0w)*UNITFAD).val();
    BSIM4pdvt1w = ((this->pdvt1w)*UNITFAD).val();
    BSIM4pdvt2w = ((this->pdvt2w)*UNITFAD).val();
    BSIM4pdrout = ((this->pdrout)*UNITFAD).val();
    BSIM4pdsub = ((this->pdsub)*UNITFAD).val();
    BSIM4pvth0 = ((this->pvth0)*UNITFAD).val();
    BSIM4pua = ((this->pua)*UNITFAD).val();
    BSIM4pua1 = ((this->pua1)*UNITFAD).val();
    BSIM4pub = ((this->pub)*UNITFAD).val();
    BSIM4pub1 = ((this->pub1)*UNITFAD).val();
    BSIM4puc = ((this->puc)*UNITFAD).val();
    BSIM4puc1 = ((this->puc1)*UNITFAD).val();
    BSIM4pu0 = ((this->pu0)*UNITFAD).val();
    BSIM4peu = ((this->peu)*UNITFAD).val();
    BSIM4pute = ((this->pute)*UNITFAD).val();
    BSIM4pvoff = ((this->pvoff)*UNITFAD).val();
    BSIM4pminv = ((this->pminv)*UNITFAD).val();
    BSIM4pdelta = ((this->pdelta)*UNITFAD).val();
    BSIM4prdsw = ((this->prdsw)*UNITFAD).val();
    BSIM4prsw = ((this->prsw)*UNITFAD).val();
    BSIM4prdw = ((this->prdw)*UNITFAD).val();
    BSIM4pprwg = ((this->pprwg)*UNITFAD).val();
    BSIM4pprwb = ((this->pprwb)*UNITFAD).val();
    BSIM4pprt = ((this->pprt)*UNITFAD).val();
    BSIM4peta0 = ((this->peta0)*UNITFAD).val();
    BSIM4petab = ((this->petab)*UNITFAD).val();
    BSIM4ppclm = ((this->ppclm)*UNITFAD).val();
    BSIM4pfprout = ((this->pfprout)*UNITFAD).val();
    BSIM4ppdits = ((this->ppdits)*UNITFAD).val();
    BSIM4ppditsd = ((this->ppditsd)*UNITFAD).val();
    BSIM4ppscbe1 = ((this->ppscbe1)*UNITFAD).val();
    BSIM4ppscbe2 = ((this->ppscbe2)*UNITFAD).val();
    BSIM4ppvag = ((this->ppvag)*UNITFAD).val();
    BSIM4pwr = ((this->pwr)*UNITFAD).val();
    BSIM4pdwg = ((this->pdwg)*UNITFAD).val();
    BSIM4pdwb = ((this->pdwb)*UNITFAD).val();
    BSIM4pb0 = ((this->pb0)*UNITFAD).val();
    BSIM4pb1 = ((this->pb1)*UNITFAD).val();
    BSIM4palpha0 = ((this->palpha0)*UNITFAD).val();
    BSIM4palpha1 = ((this->palpha1)*UNITFAD).val();
    BSIM4pbeta0 = ((this->pbeta0)*UNITFAD).val();
    BSIM4pvfb = ((this->pvfb)*UNITFAD).val();
    BSIM4pagidl = ((this->pagidl)*UNITFAD).val();
    BSIM4pbgidl = ((this->pbgidl)*UNITFAD).val();
    BSIM4pcgidl = ((this->pcgidl)*UNITFAD).val();
    BSIM4pegidl = ((this->pegidl)*UNITFAD).val();
    BSIM4paigc = ((this->paigc)*UNITFAD).val();
    BSIM4pbigc = ((this->pbigc)*UNITFAD).val();
    BSIM4pcigc = ((this->pcigc)*UNITFAD).val();
    BSIM4paigsd = ((this->paigsd)*UNITFAD).val();
    BSIM4pbigsd = ((this->pbigsd)*UNITFAD).val();
    BSIM4pcigsd = ((this->pcigsd)*UNITFAD).val();
    BSIM4paigbacc = ((this->paigbacc)*UNITFAD).val();
    BSIM4pbigbacc = ((this->pbigbacc)*UNITFAD).val();
    BSIM4pcigbacc = ((this->pcigbacc)*UNITFAD).val();
    BSIM4paigbinv = ((this->paigbinv)*UNITFAD).val();
    BSIM4pbigbinv = ((this->pbigbinv)*UNITFAD).val();
    BSIM4pcigbinv = ((this->pcigbinv)*UNITFAD).val();
    BSIM4pnigc = ((this->pnigc)*UNITFAD).val();
    BSIM4pnigbacc = ((this->pnigbacc)*UNITFAD).val();
    BSIM4pnigbinv = ((this->pnigbinv)*UNITFAD).val();
    BSIM4pntox = ((this->pntox)*UNITFAD).val();
    BSIM4peigbinv = ((this->peigbinv)*UNITFAD).val();
    BSIM4ppigcd = ((this->ppigcd)*UNITFAD).val();
    BSIM4ppoxedge = ((this->ppoxedge)*UNITFAD).val();
    BSIM4pxrcrg1 = ((this->pxrcrg1)*UNITFAD).val();
    BSIM4pxrcrg2 = ((this->pxrcrg2)*UNITFAD).val();
    BSIM4pcgsl = ((this->pcgsl)*UNITFAD).val();
    BSIM4pcgdl = ((this->pcgdl)*UNITFAD).val();
    BSIM4pckappas = ((this->pckappas)*UNITFAD).val();
    BSIM4pckappad = ((this->pckappad)*UNITFAD).val();
    BSIM4pcf = ((this->pcf)*UNITFAD).val();
    BSIM4pclc = ((this->pclc)*UNITFAD).val();
    BSIM4pcle = ((this->pcle)*UNITFAD).val();
    BSIM4pvfbcv = ((this->pvfbcv)*UNITFAD).val();
    BSIM4pnoff = ((this->pnoff)*UNITFAD).val();
    BSIM4pvoffcv = ((this->pvoffcv)*UNITFAD).val();
    BSIM4pacde = ((this->pacde)*UNITFAD).val();
    BSIM4pmoin = ((this->pmoin)*UNITFAD).val();
    BSIM4tnom = ((this->tnom)*UNITFAD).val();
    BSIM4cgso = ((this->cgso)*UNITFAD).val();
    BSIM4cgdo = ((this->cgdo)*UNITFAD).val();
    BSIM4cgbo = ((this->cgbo)*UNITFAD).val();
    BSIM4xpart = ((this->xpart)*UNITFAD).val();
    BSIM4sheetResistance = ((this->rsh)*UNITFAD).val();
    BSIM4SjctSatCurDensity = ((this->jss)*UNITFAD).val();
    BSIM4DjctSatCurDensity = ((this->jsd)*UNITFAD).val();
    BSIM4SjctSidewallSatCurDensity = ((this->jsws)*UNITFAD).val();
    BSIM4DjctSidewallSatCurDensity = ((this->jswd)*UNITFAD).val();
    BSIM4SjctGateSidewallSatCurDensity = ((this->jswgs)*UNITFAD).val();
    BSIM4DjctGateSidewallSatCurDensity = ((this->jswgd)*UNITFAD).val();
    BSIM4SbulkJctPotential = ((this->pbs)*UNITFAD).val();
    BSIM4DbulkJctPotential = ((this->pbd)*UNITFAD).val();
    BSIM4SbulkJctBotGradingCoeff = ((this->mjs)*UNITFAD).val();
    BSIM4DbulkJctBotGradingCoeff = ((this->mjd)*UNITFAD).val();
    BSIM4SbulkJctSideGradingCoeff = ((this->mjsws)*UNITFAD).val();
    BSIM4DbulkJctSideGradingCoeff = ((this->mjswd)*UNITFAD).val();
    BSIM4SbulkJctGateSideGradingCoeff = ((this->mjswgs)*UNITFAD).val();
    BSIM4DbulkJctGateSideGradingCoeff = ((this->mjswgd)*UNITFAD).val();
    BSIM4SsidewallJctPotential = ((this->pbsws)*UNITFAD).val();
    BSIM4DsidewallJctPotential = ((this->pbswd)*UNITFAD).val();
    BSIM4SGatesidewallJctPotential = ((this->pbswgs)*UNITFAD).val();
    BSIM4DGatesidewallJctPotential = ((this->pbswgd)*UNITFAD).val();
    BSIM4SunitAreaJctCap = ((this->cjs)*UNITFAD).val();
    BSIM4DunitAreaJctCap = ((this->cjd)*UNITFAD).val();
    BSIM4SunitLengthSidewallJctCap = ((this->cjsws)*UNITFAD).val();
    BSIM4DunitLengthSidewallJctCap = ((this->cjswd)*UNITFAD).val();
    BSIM4SunitLengthGateSidewallJctCap = ((this->cjswgs)*UNITFAD).val();
    BSIM4DunitLengthGateSidewallJctCap = ((this->cjswgd)*UNITFAD).val();
    BSIM4SjctEmissionCoeff = ((this->njs)*UNITFAD).val();
    BSIM4DjctEmissionCoeff = ((this->njd)*UNITFAD).val();
    BSIM4SjctTempExponent = ((this->xtis)*UNITFAD).val();
    BSIM4DjctTempExponent = ((this->xtid)*UNITFAD).val();
    BSIM4Lint = ((this->lint)*UNITFAD).val();
    BSIM4Ll = ((this->ll)*UNITFAD).val();
    BSIM4Llc = ((this->llc)*UNITFAD).val();
    BSIM4Lln = ((this->lln)*UNITFAD).val();
    BSIM4Lw = ((this->lw)*UNITFAD).val();
    BSIM4Lwc = ((this->lwc)*UNITFAD).val();
    BSIM4Lwn = ((this->lwn)*UNITFAD).val();
    BSIM4Lwl = ((this->lwl)*UNITFAD).val();
    BSIM4Lwlc = ((this->lwlc)*UNITFAD).val();
    BSIM4Lmin = ((this->lmin)*UNITFAD).val();
    BSIM4Lmax = ((this->lmax)*UNITFAD).val();
    BSIM4Wint = ((this->wint)*UNITFAD).val();
    BSIM4Wl = ((this->wl)*UNITFAD).val();
    BSIM4Wlc = ((this->wlc)*UNITFAD).val();
    BSIM4Wln = ((this->wln)*UNITFAD).val();
    BSIM4Ww = ((this->ww)*UNITFAD).val();
    BSIM4Wwc = ((this->wwc)*UNITFAD).val();
    BSIM4Wwn = ((this->wwn)*UNITFAD).val();
    BSIM4Wwl = ((this->wwl)*UNITFAD).val();
    BSIM4Wwlc = ((this->wwlc)*UNITFAD).val();
    BSIM4Wmin = ((this->wmin)*UNITFAD).val();
    BSIM4Wmax = ((this->wmax)*UNITFAD).val();
    BSIM4jss = ((this->jss)*UNITFAD).val();
    BSIM4jsws = ((this->jsws)*UNITFAD).val();
    BSIM4jswgs = ((this->jswgs)*UNITFAD).val();
    BSIM4pbs = ((this->pbs)*UNITFAD).val();
    BSIM4njs = ((this->njs)*UNITFAD).val();
    BSIM4xtis = ((this->xtis)*UNITFAD).val();
    BSIM4mjs = ((this->mjs)*UNITFAD).val();
    BSIM4pbsws = ((this->pbsws)*UNITFAD).val();
    BSIM4mjsws = ((this->mjsws)*UNITFAD).val();
    BSIM4pbswgs = ((this->pbswgs)*UNITFAD).val();
    BSIM4mjswgs = ((this->mjswgs)*UNITFAD).val();
    BSIM4cjs = ((this->cjs)*UNITFAD).val();
    BSIM4cjsws = ((this->cjsws)*UNITFAD).val();
    BSIM4cjswgs = ((this->cjswgs)*UNITFAD).val();
    BSIM4jsd = ((this->jsd)*UNITFAD).val();
    BSIM4jswd = ((this->jswd)*UNITFAD).val();
    BSIM4jswgd = ((this->jswgd)*UNITFAD).val();
    BSIM4pbd = ((this->pbd)*UNITFAD).val();
    BSIM4njd = ((this->njd)*UNITFAD).val();
    BSIM4xtid = ((this->xtid)*UNITFAD).val();
    BSIM4mjd = ((this->mjd)*UNITFAD).val();
    BSIM4pbswd = ((this->pbswd)*UNITFAD).val();
    BSIM4mjswd = ((this->mjswd)*UNITFAD).val();
    BSIM4pbswgd = ((this->pbswgd)*UNITFAD).val();
    BSIM4mjswgd = ((this->mjswgd)*UNITFAD).val();
    BSIM4cjd = ((this->cjd)*UNITFAD).val();
    BSIM4cjswd = ((this->cjswd)*UNITFAD).val();
    BSIM4cjswgd = ((this->cjswgd)*UNITFAD).val();
    BSIM4lambda = ((this->lambda)*UNITFAD).val();
    BSIM4llambda = ((this->llambda)*UNITFAD).val();
    BSIM4wlambda = ((this->wlambda)*UNITFAD).val();
    BSIM4plambda = ((this->plambda)*UNITFAD).val();
    BSIM4vtl = ((this->vtl)*UNITFAD).val();
    BSIM4lvtl = ((this->lvtl)*UNITFAD).val();
    BSIM4wvtl = ((this->wvtl)*UNITFAD).val();
    BSIM4pvtl = ((this->pvtl)*UNITFAD).val();
    BSIM4xn = ((this->xn)*UNITFAD).val();
    BSIM4sa = ((this->sa)*UNITFAD).val();
    BSIM4sb = ((this->sb)*UNITFAD).val();
    BSIM4sd = ((this->sd)*UNITFAD).val();
    BSIM4saref = ((this->saref)*UNITFAD).val();
    BSIM4sbref = ((this->sbref)*UNITFAD).val();
    BSIM4wlod = ((this->wlod)*UNITFAD).val();
    BSIM4ku0 = ((this->ku0)*UNITFAD).val();
    BSIM4kvsat = ((this->kvsat)*UNITFAD).val();
    BSIM4tku0 = ((this->tku0)*UNITFAD).val();
    BSIM4lku0 = ((this->lku0)*UNITFAD).val();
    BSIM4wku0 = ((this->wku0)*UNITFAD).val();
    BSIM4pku0 = ((this->pku0)*UNITFAD).val();
    BSIM4kvth0 = ((this->kvth0)*UNITFAD).val();
    BSIM4wlodku0 = ((this->wlodku0)*UNITFAD).val();
    BSIM4lkvth0 = ((this->lkvth0)*UNITFAD).val();
    BSIM4wkvth0 = ((this->wkvth0)*UNITFAD).val();
    BSIM4pkvth0 = ((this->pkvth0)*UNITFAD).val();
    BSIM4llodvth = ((this->llodvth)*UNITFAD).val();
    BSIM4wlodvth = ((this->wlodvth)*UNITFAD).val();
    BSIM4stk2 = ((this->stk2)*UNITFAD).val();
    BSIM4lodk2 = ((this->lodk2)*UNITFAD).val();
    BSIM4steta0 = ((this->steta0)*UNITFAD).val();
    BSIM4lodeta0 = ((this->lodeta0)*UNITFAD).val();
    BSIM4sc = ((this->sc)*UNITFAD).val();
    BSIM4tvoffcv = ((this->tvoffcv)*UNITFAD).val();
    BSIM4wtvoffcv = ((this->wtvoffcv)*UNITFAD).val();
    BSIM4ltvoffcv = ((this->ltvoffcv)*UNITFAD).val();
    BSIM4ptvoffcv = ((this->ptvoffcv)*UNITFAD).val();
    BSIM4teta0 = ((this->teta0)*UNITFAD).val();
    BSIM4wteta0 = ((this->wteta0)*UNITFAD).val();
    BSIM4lteta0 = ((this->lteta0)*UNITFAD).val();
    BSIM4pteta0 = ((this->pteta0)*UNITFAD).val();
    BSIM4rbpdl = ((this->rbpdl)*UNITFAD).val();
    BSIM4rbpdnf = ((this->rbpdnf)*UNITFAD).val();
    BSIM4voffcvl = ((this->voffcvl)*UNITFAD).val();
    BSIM4tnfactor = ((this->tnfactor)*UNITFAD).val();
    BSIM4wtnfactor = ((this->wtnfactor)*UNITFAD).val();
    BSIM4ltnfactor = ((this->ltnfactor)*UNITFAD).val();
    BSIM4ptnfactor = ((this->ptnfactor)*UNITFAD).val();
    BSIM4minvcv = ((this->minvcv)*UNITFAD).val();
    BSIM4wminvcv = ((this->wminvcv)*UNITFAD).val();
    BSIM4lminvcv = ((this->lminvcv)*UNITFAD).val();
    BSIM4pminvcv = ((this->pminvcv)*UNITFAD).val();
    BSIM4tvfbsdoff = ((this->tvfbsdoff)*UNITFAD).val();
    BSIM4wtvfbsdoff = ((this->wtvfbsdoff)*UNITFAD).val();
    BSIM4ltvfbsdoff = ((this->ltvfbsdoff)*UNITFAD).val();
    BSIM4ptvfbsdoff = ((this->ptvfbsdoff)*UNITFAD).val();
    BSIM4gidlMod = ((this->gidlmod)*UNITFAD).val();
    BSIM4aigs = ((this->aigs)*UNITFAD).val();
    BSIM4waigs = ((this->waigs)*UNITFAD).val();
    BSIM4laigs = ((this->laigs)*UNITFAD).val();
    BSIM4paigs = ((this->paigs)*UNITFAD).val();
    BSIM4bigs = ((this->bigs)*UNITFAD).val();
    BSIM4wbigs = ((this->wbigs)*UNITFAD).val();
    BSIM4lbigs = ((this->lbigs)*UNITFAD).val();
    BSIM4pbigs = ((this->pbigs)*UNITFAD).val();
    BSIM4cigs = ((this->cigs)*UNITFAD).val();
    BSIM4wcigs = ((this->wcigs)*UNITFAD).val();
    BSIM4lcigs = ((this->lcigs)*UNITFAD).val();
    BSIM4pcigs = ((this->pcigs)*UNITFAD).val();
    BSIM4aigd = ((this->aigd)*UNITFAD).val();
    BSIM4waigd = ((this->waigd)*UNITFAD).val();
    BSIM4laigd = ((this->laigd)*UNITFAD).val();
    BSIM4paigd = ((this->paigd)*UNITFAD).val();
    BSIM4bigd = ((this->bigd)*UNITFAD).val();
    BSIM4wbigd = ((this->wbigd)*UNITFAD).val();
    BSIM4lbigd = ((this->lbigd)*UNITFAD).val();
    BSIM4pbigd = ((this->pbigd)*UNITFAD).val();
    BSIM4cigd = ((this->cigd)*UNITFAD).val();
    BSIM4wcigd = ((this->wcigd)*UNITFAD).val();
    BSIM4lcigd = ((this->lcigd)*UNITFAD).val();
    BSIM4pcigd = ((this->pcigd)*UNITFAD).val();
    BSIM4delvto = ((this->delvto)*UNITFAD).val();
    BSIM4ados = ((this->ados)*UNITFAD).val();
    BSIM4bdos = ((this->bdos)*UNITFAD).val();
    BSIM4agisl = ((this->agisl)*UNITFAD).val();
    BSIM4wagisl = ((this->wagisl)*UNITFAD).val();
    BSIM4lagisl = ((this->lagisl)*UNITFAD).val();
    BSIM4pagisl = ((this->pagisl)*UNITFAD).val();
    BSIM4bgisl = ((this->bgisl)*UNITFAD).val();
    BSIM4wbgisl = ((this->wbgisl)*UNITFAD).val();
    BSIM4lbgisl = ((this->lbgisl)*UNITFAD).val();
    BSIM4pbgisl = ((this->pbgisl)*UNITFAD).val();
    BSIM4cgisl = ((this->cgisl)*UNITFAD).val();
    BSIM4wcgisl = ((this->wcgisl)*UNITFAD).val();
    BSIM4lcgisl = ((this->lcgisl)*UNITFAD).val();
    BSIM4pcgisl = ((this->pcgisl)*UNITFAD).val();
    BSIM4egisl = ((this->egisl)*UNITFAD).val();
    BSIM4wegisl = ((this->wegisl)*UNITFAD).val();
    BSIM4legisl = ((this->legisl)*UNITFAD).val();
    BSIM4pegisl = ((this->pegisl)*UNITFAD).val();
    BSIM4rgisl = ((this->rgisl)*UNITFAD).val();
    BSIM4wrgisl = ((this->wrgisl)*UNITFAD).val();
    BSIM4lrgisl = ((this->lrgisl)*UNITFAD).val();
    BSIM4prgisl = ((this->prgisl)*UNITFAD).val();
    BSIM4kgisl = ((this->kgisl)*UNITFAD).val();
    BSIM4wkgisl = ((this->wkgisl)*UNITFAD).val();
    BSIM4lkgisl = ((this->lkgisl)*UNITFAD).val();
    BSIM4pkgisl = ((this->pkgisl)*UNITFAD).val();
    BSIM4fgisl = ((this->fgisl)*UNITFAD).val();
    BSIM4wfgisl = ((this->wfgisl)*UNITFAD).val();
    BSIM4lfgisl = ((this->lfgisl)*UNITFAD).val();
    BSIM4pfgisl = ((this->pfgisl)*UNITFAD).val();
    BSIM4rgidl = ((this->rgidl)*UNITFAD).val();
    BSIM4wrgidl = ((this->wrgidl)*UNITFAD).val();
    BSIM4lrgidl = ((this->lrgidl)*UNITFAD).val();
    BSIM4prgidl = ((this->prgidl)*UNITFAD).val();
    BSIM4kgidl = ((this->kgidl)*UNITFAD).val();
    BSIM4wkgidl = ((this->wkgidl)*UNITFAD).val();
    BSIM4lkgidl = ((this->lkgidl)*UNITFAD).val();
    BSIM4pkgidl = ((this->pkgidl)*UNITFAD).val();
    BSIM4fgidl = ((this->fgidl)*UNITFAD).val();
    BSIM4wfgidl = ((this->wfgidl)*UNITFAD).val();
    BSIM4lfgidl = ((this->lfgidl)*UNITFAD).val();
    BSIM4pfgidl = ((this->pfgidl)*UNITFAD).val();
    BSIM4dvtp2 = ((this->dvtp2)*UNITFAD).val();
    BSIM4wdvtp2 = ((this->wdvtp2)*UNITFAD).val();
    BSIM4ldvtp2 = ((this->ldvtp2)*UNITFAD).val();
    BSIM4pdvtp2 = ((this->pdvtp2)*UNITFAD).val();
    BSIM4dvtp3 = ((this->dvtp3)*UNITFAD).val();
    BSIM4wdvtp3 = ((this->wdvtp3)*UNITFAD).val();
    BSIM4ldvtp3 = ((this->ldvtp3)*UNITFAD).val();
    BSIM4pdvtp3 = ((this->pdvtp3)*UNITFAD).val();
    BSIM4dvtp4 = ((this->dvtp4)*UNITFAD).val();
    BSIM4wdvtp4 = ((this->wdvtp4)*UNITFAD).val();
    BSIM4ldvtp4 = ((this->ldvtp4)*UNITFAD).val();
    BSIM4pdvtp4 = ((this->pdvtp4)*UNITFAD).val();
    BSIM4dvtp5 = ((this->dvtp5)*UNITFAD).val();
    BSIM4wdvtp5 = ((this->wdvtp5)*UNITFAD).val();
    BSIM4ldvtp5 = ((this->ldvtp5)*UNITFAD).val();
    BSIM4pdvtp5 = ((this->pdvtp5)*UNITFAD).val();
    BSIM4ni0sub = ((this->ni0sub)*UNITFAD).val();
    BSIM4bg0sub = ((this->bg0sub)*UNITFAD).val();
    BSIM4tbgasub = ((this->tbgasub)*UNITFAD).val();
    BSIM4tbgbsub = ((this->tbgbsub)*UNITFAD).val();
    BSIM4tempeot = ((this->tempeot)*UNITFAD).val();
    BSIM4leffeot = ((this->leffeot)*UNITFAD).val();
    BSIM4weffeot = ((this->weffeot)*UNITFAD).val();
    BSIM4vddeot = ((this->vddeot)*UNITFAD).val();
    BSIM4eot = ((this->eot)*UNITFAD).val();
    BSIM4epsrgate = ((this->epsrgate)*UNITFAD).val();
    BSIM4epsrsub = ((this->epsrsub)*UNITFAD).val();
    BSIM4phig = ((this->phig)*UNITFAD).val();
    BSIM4easub = ((this->easub)*UNITFAD).val();
    BSIM4mtrlcompatmod = ((this->mtrlcompatmod)*UNITFAD).val();
    BSIM4mtrlmod = ((this->mtrlmod)*UNITFAD).val();
    BSIM4cvchargeMod = ((this->cvchargemod)*UNITFAD).val();
    BSIM4ud = ((this->ud)*UNITFAD).val();
    BSIM4wud = ((this->wud)*UNITFAD).val();
    BSIM4lud = ((this->lud)*UNITFAD).val();
    BSIM4pud = ((this->pud)*UNITFAD).val();
    BSIM4ud1 = ((this->ud1)*UNITFAD).val();
    BSIM4wud1 = ((this->wud1)*UNITFAD).val();
    BSIM4lud1 = ((this->lud1)*UNITFAD).val();
    BSIM4pud1 = ((this->pud1)*UNITFAD).val();
    BSIM4up = ((this->up)*UNITFAD).val();
    BSIM4wup = ((this->wup)*UNITFAD).val();
    BSIM4lup = ((this->lup)*UNITFAD).val();
    BSIM4pup = ((this->pup)*UNITFAD).val();
    BSIM4lp = ((this->lp)*UNITFAD).val();
    BSIM4wlp = ((this->wlp)*UNITFAD).val();
    BSIM4llp = ((this->llp)*UNITFAD).val();
    BSIM4plp = ((this->plp)*UNITFAD).val();
    BSIM4ucs = ((this->ucs)*UNITFAD).val();
    BSIM4wucs = ((this->wucs)*UNITFAD).val();
    BSIM4lucs = ((this->lucs)*UNITFAD).val();
    BSIM4pucs = ((this->pucs)*UNITFAD).val();
    BSIM4ucste = ((this->ucste)*UNITFAD).val();
    BSIM4wucste = ((this->wucste)*UNITFAD).val();
    BSIM4lucste = ((this->lucste)*UNITFAD).val();
    BSIM4pucste = ((this->pucste)*UNITFAD).val();
    BSIM4kvth0we = ((this->kvth0we)*UNITFAD).val();
    BSIM4wkvth0we = ((this->wkvth0we)*UNITFAD).val();
    BSIM4lkvth0we = ((this->lkvth0we)*UNITFAD).val();
    BSIM4pkvth0we = ((this->pkvth0we)*UNITFAD).val();
    BSIM4k2we = ((this->k2we)*UNITFAD).val();
    BSIM4wk2we = ((this->wk2we)*UNITFAD).val();
    BSIM4lk2we = ((this->lk2we)*UNITFAD).val();
    BSIM4pk2we = ((this->pk2we)*UNITFAD).val();
    BSIM4ku0we = ((this->ku0we)*UNITFAD).val();
    BSIM4wku0we = ((this->wku0we)*UNITFAD).val();
    BSIM4lku0we = ((this->lku0we)*UNITFAD).val();
    BSIM4pku0we = ((this->pku0we)*UNITFAD).val();
    BSIM4vfbsdoff = ((this->vfbsdoff)*UNITFAD).val();
    BSIM4wvfbsdoff = ((this->wvfbsdoff)*UNITFAD).val();
    BSIM4lvfbsdoff = ((this->lvfbsdoff)*UNITFAD).val();
    BSIM4pvfbsdoff = ((this->pvfbsdoff)*UNITFAD).val();
    BSIM4tvoff = ((this->tvoff)*UNITFAD).val();
    BSIM4wtvoff = ((this->wtvoff)*UNITFAD).val();
    BSIM4ltvoff = ((this->ltvoff)*UNITFAD).val();
    BSIM4ptvoff = ((this->ptvoff)*UNITFAD).val();
    BSIM4wpemod = ((this->wpemod)*UNITFAD).val();
    BSIM4sca = ((this->sca)*UNITFAD).val();
    BSIM4scb = ((this->scb)*UNITFAD).val();
    BSIM4scc = ((this->scc)*UNITFAD).val();
    BSIM4web = ((this->web)*UNITFAD).val();
    BSIM4wec = ((this->wec)*UNITFAD).val();
    BSIM4scref = ((this->scref)*UNITFAD).val();
    BSIM4rbps0 = ((this->rbps0)*UNITFAD).val();
    BSIM4rbpsl = ((this->rbpsl)*UNITFAD).val();
    BSIM4rbpsw = ((this->rbpsw)*UNITFAD).val();
    BSIM4rbpsnf = ((this->rbpsnf)*UNITFAD).val();
    BSIM4rbpd0 = ((this->rbpd0)*UNITFAD).val();
    BSIM4rbpdw = ((this->rbpdw)*UNITFAD).val();
    BSIM4rbpbx0 = ((this->rbpbx0)*UNITFAD).val();
    BSIM4rbpbxl = ((this->rbpbxl)*UNITFAD).val();
    BSIM4rbpbxw = ((this->rbpbxw)*UNITFAD).val();
    BSIM4rbpbxnf = ((this->rbpbxnf)*UNITFAD).val();
    BSIM4rbpby0 = ((this->rbpby0)*UNITFAD).val();
    BSIM4rbpbyl = ((this->rbpbyl)*UNITFAD).val();
    BSIM4rbpbyw = ((this->rbpbyw)*UNITFAD).val();
    BSIM4rbpbynf = ((this->rbpbynf)*UNITFAD).val();
    BSIM4rbsbx0 = ((this->rbsbx0)*UNITFAD).val();
    BSIM4rbsby0 = ((this->rbsby0)*UNITFAD).val();
    BSIM4rbdbx0 = ((this->rbdbx0)*UNITFAD).val();
    BSIM4rbdby0 = ((this->rbdby0)*UNITFAD).val();
    BSIM4rbsdbxl = ((this->rbsdbxl)*UNITFAD).val();
    BSIM4rbsdbxw = ((this->rbsdbxw)*UNITFAD).val();
    BSIM4rbsdbxnf = ((this->rbsdbxnf)*UNITFAD).val();
    BSIM4rbsdbyl = ((this->rbsdbyl)*UNITFAD).val();
    BSIM4rbsdbyw = ((this->rbsdbyw)*UNITFAD).val();
    BSIM4rbsdbynf = ((this->rbsdbynf)*UNITFAD).val();
    BSIM4jtss = ((this->jtss)*UNITFAD).val();
    BSIM4jtssws = ((this->jtssws)*UNITFAD).val();
    BSIM4jtsswgs = ((this->jtsswgs)*UNITFAD).val();
    BSIM4jtweff = ((this->jtweff)*UNITFAD).val();
    BSIM4njts = ((this->njts)*UNITFAD).val();
    BSIM4njtssw = ((this->njtssw)*UNITFAD).val();
    BSIM4njtsswg = ((this->njtsswg)*UNITFAD).val();
    BSIM4xtss = ((this->xtss)*UNITFAD).val();
    BSIM4xtssws = ((this->xtssws)*UNITFAD).val();
    BSIM4xtsswgs = ((this->xtsswgs)*UNITFAD).val();
    BSIM4vtss = ((this->vtss)*UNITFAD).val();
    BSIM4vtssws = ((this->vtssws)*UNITFAD).val();
    BSIM4vtsswgs = ((this->vtsswgs)*UNITFAD).val();
    BSIM4tnjts = ((this->tnjts)*UNITFAD).val();
    BSIM4tnjtssw = ((this->tnjtssw)*UNITFAD).val();
    BSIM4tnjtsswg = ((this->tnjtsswg)*UNITFAD).val();
    BSIM4jtsd = ((this->jtsd)*UNITFAD).val();
    BSIM4jtsswd = ((this->jtsswd)*UNITFAD).val();
    BSIM4jtsswgd = ((this->jtsswgd)*UNITFAD).val();
    BSIM4njtsd = ((this->njtsd)*UNITFAD).val();
    BSIM4njtsswd = ((this->njtsswd)*UNITFAD).val();
    BSIM4njtsswgd = ((this->njtsswgd)*UNITFAD).val();
    BSIM4xtsd = ((this->xtsd)*UNITFAD).val();
    BSIM4xtsswd = ((this->xtsswd)*UNITFAD).val();
    BSIM4xtsswgd = ((this->xtsswgd)*UNITFAD).val();
    BSIM4vtsd = ((this->vtsd)*UNITFAD).val();
    BSIM4vtsswd = ((this->vtsswd)*UNITFAD).val();
    BSIM4vtsswgd = ((this->vtsswgd)*UNITFAD).val();
    BSIM4tnjtsd = ((this->tnjtsd)*UNITFAD).val();
    BSIM4tnjtsswd = ((this->tnjtsswd)*UNITFAD).val();
    BSIM4tnjtsswgd = ((this->tnjtsswgd)*UNITFAD).val();
    BSIM4VgsteffVth = ((0.0)*UNITFAD).val();
    BSIM4scGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sc == -12345789),0,1))*UNITFAD).val();
    BSIM4tvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4wtvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wtvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4ltvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ltvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4ptvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ptvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4teta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4teta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wteta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wteta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lteta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lteta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pteta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pteta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpdlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpdl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpdnfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpdnf == -12345789),0,1))*UNITFAD).val();
    BSIM4voffcvlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4voffcvl == -12345789),0,1))*UNITFAD).val();
    BSIM4tnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4wtnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wtnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4ltnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ltnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4ptnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ptnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4minvcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4minvcv == -12345789),0,1))*UNITFAD).val();
    BSIM4wminvcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wminvcv == -12345789),0,1))*UNITFAD).val();
    BSIM4lminvcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lminvcv == -12345789),0,1))*UNITFAD).val();
    BSIM4pminvcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pminvcv == -12345789),0,1))*UNITFAD).val();
    BSIM4tvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wtvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wtvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4ltvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ltvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4ptvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ptvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4gidlModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4gidlMod == -9999999),0,1))*UNITFAD).val();
    BSIM4aigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigs == -12345789),0,1))*UNITFAD).val();
    BSIM4waigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigs == -12345789),0,1))*UNITFAD).val();
    BSIM4laigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigs == -12345789),0,1))*UNITFAD).val();
    BSIM4paigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigs == -12345789),0,1))*UNITFAD).val();
    BSIM4bigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigs == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigs == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigs == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigs == -12345789),0,1))*UNITFAD).val();
    BSIM4cigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigs == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigs == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigs == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigs == -12345789),0,1))*UNITFAD).val();
    BSIM4aigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigd == -12345789),0,1))*UNITFAD).val();
    BSIM4waigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigd == -12345789),0,1))*UNITFAD).val();
    BSIM4laigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigd == -12345789),0,1))*UNITFAD).val();
    BSIM4paigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigd == -12345789),0,1))*UNITFAD).val();
    BSIM4bigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigd == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigd == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigd == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigd == -12345789),0,1))*UNITFAD).val();
    BSIM4cigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigd == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigd == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigd == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigd == -12345789),0,1))*UNITFAD).val();
    BSIM4delvtoGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4delvto == -12345789),0,1))*UNITFAD).val();
    BSIM4adosGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ados == -12345789),0,1))*UNITFAD).val();
    BSIM4bdosGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bdos == -12345789),0,1))*UNITFAD).val();
    BSIM4agislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4agisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wagisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lagisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pagisl == -12345789),0,1))*UNITFAD).val();
    BSIM4bgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4cgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4egislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4egisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wegislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wegisl == -12345789),0,1))*UNITFAD).val();
    BSIM4legislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4legisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pegislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pegisl == -12345789),0,1))*UNITFAD).val();
    BSIM4rgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wrgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wrgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lrgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lrgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4prgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4kgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4fgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4fgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4wfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wfgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4lfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lfgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4pfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pfgisl == -12345789),0,1))*UNITFAD).val();
    BSIM4rgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wrgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wrgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4lrgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lrgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4prgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4kgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4lkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4pkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4fgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4fgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wfgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4lfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lfgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4pfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pfgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4cvchargeModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cvchargeMod == -9999999),0,1))*UNITFAD).val();
    BSIM4dvtp2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp2 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp2 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvtp3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp3 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp3 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp3 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp3 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvtp4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp4 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp4 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp4 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp4 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvtp5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp5 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp5 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp5 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp5 == -12345789),0,1))*UNITFAD).val();
    BSIM4tempeotGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tempeot == -12345789),0,1))*UNITFAD).val();
    BSIM4leffeotGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4leffeot == -12345789),0,1))*UNITFAD).val();
    BSIM4weffeotGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4weffeot == -12345789),0,1))*UNITFAD).val();
    BSIM4vddeotGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vddeot == -12345789),0,1))*UNITFAD).val();
    BSIM4eotGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4eot == -12345789),0,1))*UNITFAD).val();
    BSIM4epsrgateGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4epsrgate == -12345789),0,1))*UNITFAD).val();
    BSIM4epsrsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4epsrsub == -12345789),0,1))*UNITFAD).val();
    BSIM4phigGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4phig == -12345789),0,1))*UNITFAD).val();
    BSIM4easubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4easub == -12345789),0,1))*UNITFAD).val();
    BSIM4mtrlcompatmodGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4mtrlcompatmod == -9999999),0,1))*UNITFAD).val();
    BSIM4mtrlmodGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4mtrlmod == -9999999),0,1))*UNITFAD).val();
    BSIM4ni0subGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ni0sub == -12345789),0,1))*UNITFAD).val();
    BSIM4bg0subGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bg0sub == -12345789),0,1))*UNITFAD).val();
    BSIM4tbgasubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tbgasub == -12345789),0,1))*UNITFAD).val();
    BSIM4tbgbsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tbgbsub == -12345789),0,1))*UNITFAD).val();
    BSIM4udGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ud == -12345789),0,1))*UNITFAD).val();
    BSIM4wudGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wud == -12345789),0,1))*UNITFAD).val();
    BSIM4ludGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lud == -12345789),0,1))*UNITFAD).val();
    BSIM4pudGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pud == -12345789),0,1))*UNITFAD).val();
    BSIM4ud1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ud1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wud1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wud1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lud1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lud1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pud1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pud1 == -12345789),0,1))*UNITFAD).val();
    BSIM4upGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4up == -12345789),0,1))*UNITFAD).val();
    BSIM4wupGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wup == -12345789),0,1))*UNITFAD).val();
    BSIM4lupGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lup == -12345789),0,1))*UNITFAD).val();
    BSIM4pupGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pup == -12345789),0,1))*UNITFAD).val();
    BSIM4lpGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lp == -12345789),0,1))*UNITFAD).val();
    BSIM4wlpGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlp == -12345789),0,1))*UNITFAD).val();
    BSIM4llpGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4llp == -12345789),0,1))*UNITFAD).val();
    BSIM4plpGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4plp == -12345789),0,1))*UNITFAD).val();
    BSIM4ucsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ucs == -12345789),0,1))*UNITFAD).val();
    BSIM4wucsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wucs == -12345789),0,1))*UNITFAD).val();
    BSIM4lucsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lucs == -12345789),0,1))*UNITFAD).val();
    BSIM4pucsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pucs == -12345789),0,1))*UNITFAD).val();
    BSIM4ucsteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ucste == -12345789),0,1))*UNITFAD).val();
    BSIM4wucsteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wucste == -12345789),0,1))*UNITFAD).val();
    BSIM4lucsteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lucste == -12345789),0,1))*UNITFAD).val();
    BSIM4pucsteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pucste == -12345789),0,1))*UNITFAD).val();
    BSIM4kvth0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kvth0we == -12345789),0,1))*UNITFAD).val();
    BSIM4wkvth0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkvth0we == -12345789),0,1))*UNITFAD).val();
    BSIM4lkvth0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkvth0we == -12345789),0,1))*UNITFAD).val();
    BSIM4pkvth0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkvth0we == -12345789),0,1))*UNITFAD).val();
    BSIM4k2weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4k2we == -12345789),0,1))*UNITFAD).val();
    BSIM4wk2weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wk2we == -12345789),0,1))*UNITFAD).val();
    BSIM4lk2weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lk2we == -12345789),0,1))*UNITFAD).val();
    BSIM4pk2weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pk2we == -12345789),0,1))*UNITFAD).val();
    BSIM4ku0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ku0we == -12345789),0,1))*UNITFAD).val();
    BSIM4wku0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wku0we == -12345789),0,1))*UNITFAD).val();
    BSIM4lku0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lku0we == -12345789),0,1))*UNITFAD).val();
    BSIM4pku0weGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pku0we == -12345789),0,1))*UNITFAD).val();
    BSIM4vfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4lvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4pvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvfbsdoff == -12345789),0,1))*UNITFAD).val();
    BSIM4tvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wtvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wtvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4ltvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ltvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4ptvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ptvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wpemodGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpemod == -9999999),0,1))*UNITFAD).val();
    BSIM4scaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sca == -12345789),0,1))*UNITFAD).val();
    BSIM4scbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4scb == -12345789),0,1))*UNITFAD).val();
    BSIM4sccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4scc == -12345789),0,1))*UNITFAD).val();
    BSIM4webGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4web == -12345789),0,1))*UNITFAD).val();
    BSIM4wecGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wec == -12345789),0,1))*UNITFAD).val();
    BSIM4screfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4scref == -12345789),0,1))*UNITFAD).val();
    BSIM4rbps0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbps0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpsl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpsw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpsnfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpsnf == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpd0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpd0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpdwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpdw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbx0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbx0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbxlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbxl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbxwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbxw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbxnfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbxnf == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpby0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpby0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbylGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbyl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbywGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbyw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbynfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpbynf == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsbx0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsbx0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsby0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsby0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbdbx0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbdbx0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbdby0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbdby0 == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbxlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbxl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbxwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbxw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbxnfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbxnf == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbylGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbyl == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbywGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbyw == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsdbynfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsdbynf == -12345789),0,1))*UNITFAD).val();
    BSIM4jtssGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtss == -12345789),0,1))*UNITFAD).val();
    BSIM4jtsswsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtssws == -12345789),0,1))*UNITFAD).val();
    BSIM4jtsswgsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtsswgs == -12345789),0,1))*UNITFAD).val();
    BSIM4jtweffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtweff == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njts == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njtssw == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsswgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njtsswg == -12345789),0,1))*UNITFAD).val();
    BSIM4xtssGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtss == -12345789),0,1))*UNITFAD).val();
    BSIM4xtsswsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtssws == -12345789),0,1))*UNITFAD).val();
    BSIM4xtsswgsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtsswgs == -12345789),0,1))*UNITFAD).val();
    BSIM4vtssGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtss == -12345789),0,1))*UNITFAD).val();
    BSIM4vtsswsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtssws == -12345789),0,1))*UNITFAD).val();
    BSIM4vtsswgsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtsswgs == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjts == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjtssw == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsswgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjtsswg == -12345789),0,1))*UNITFAD).val();
    BSIM4jtsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtsd == -12345789),0,1))*UNITFAD).val();
    BSIM4jtsswdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtsswd == -12345789),0,1))*UNITFAD).val();
    BSIM4jtsswgdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4jtsswgd == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njtsd == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsswdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njtsswd == -12345789),0,1))*UNITFAD).val();
    BSIM4njtsswgdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4njtsswgd == -12345789),0,1))*UNITFAD).val();
    BSIM4xtsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtsd == -12345789),0,1))*UNITFAD).val();
    BSIM4xtsswdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtsswd == -12345789),0,1))*UNITFAD).val();
    BSIM4xtsswgdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xtsswgd == -12345789),0,1))*UNITFAD).val();
    BSIM4vtsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtsd == -12345789),0,1))*UNITFAD).val();
    BSIM4vtsswdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtsswd == -12345789),0,1))*UNITFAD).val();
    BSIM4vtsswgdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtsswgd == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjtsd == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsswdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjtsswd == -12345789),0,1))*UNITFAD).val();
    BSIM4tnjtsswgdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnjtsswgd == -12345789),0,1))*UNITFAD).val();
    BSIM4lGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4l == -12345789),0,1))*UNITFAD).val();
    BSIM4wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4w == -12345789),0,1))*UNITFAD).val();
    BSIM4nfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nf == -9999999),0,1))*UNITFAD).val();
    BSIM4minGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4min == -9999999),0,1))*UNITFAD).val();
    BSIM4drainAreaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4drainArea == -12345789),0,1))*UNITFAD).val();
    BSIM4sourceAreaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sourceArea == -12345789),0,1))*UNITFAD).val();
    BSIM4drainSquaresGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4drainSquares == -12345789),0,1))*UNITFAD).val();
    BSIM4sourceSquaresGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sourceSquares == -12345789),0,1))*UNITFAD).val();
    BSIM4drainPerimeterGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4drainPerimeter == -12345789),0,1))*UNITFAD).val();
    BSIM4sourcePerimeterGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sourcePerimeter == -12345789),0,1))*UNITFAD).val();
    BSIM4rbdbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbdb == -12345789),0,1))*UNITFAD).val();
    BSIM4rbsbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbsb == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpb == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbpd == -12345789),0,1))*UNITFAD).val();
    BSIM4rbpsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbps == -12345789),0,1))*UNITFAD).val();
    BSIM4icVDSGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4icVDS == -12345789),0,1))*UNITFAD).val();
    BSIM4icVGSGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4icVGS == -12345789),0,1))*UNITFAD).val();
    BSIM4icVBSGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4icVBS == -12345789),0,1))*UNITFAD).val();
    BSIM4acnqsModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4acnqsMod == -9999999),0,1))*UNITFAD).val();
    BSIM4rbodyModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rbodyMod == -9999999),0,1))*UNITFAD).val();
    BSIM4geoModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4geoMod == -9999999),0,1))*UNITFAD).val();
    BSIM4rgeoModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rgeoMod == -9999999),0,1))*UNITFAD).val();
    BSIM4mobModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4mobMod == -9999999),0,1))*UNITFAD).val();
    BSIM4binUnitGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4binUnit == -9999999),0,1))*UNITFAD).val();
    BSIM4capModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4capMod == -9999999),0,1))*UNITFAD).val();
    BSIM4dioModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dioMod == -9999999),0,1))*UNITFAD).val();
    BSIM4rdsModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rdsMod == -9999999),0,1))*UNITFAD).val();
    BSIM4rgateModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rgateMod == -9999999),0,1))*UNITFAD).val();
    BSIM4perModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4perMod == -9999999),0,1))*UNITFAD).val();
    BSIM4paramChkGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paramChk == -9999999),0,1))*UNITFAD).val();
    BSIM4trnqsModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4trnqsMod == -9999999),0,1))*UNITFAD).val();
    BSIM4fnoiModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4fnoiMod == -9999999),0,1))*UNITFAD).val();
    BSIM4tnoiModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnoiMod == -9999999),0,1))*UNITFAD).val();
    BSIM4igcModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4igcMod == -9999999),0,1))*UNITFAD).val();
    BSIM4igbModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4igbMod == -9999999),0,1))*UNITFAD).val();
    BSIM4typeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == -9999999),0,1))*UNITFAD).val();
    BSIM4toxrefGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4toxref == -12345789),0,1))*UNITFAD).val();
    BSIM4toxeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4toxe == -12345789),0,1))*UNITFAD).val();
    BSIM4toxpGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4toxp == -12345789),0,1))*UNITFAD).val();
    BSIM4toxmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4toxm == -12345789),0,1))*UNITFAD).val();
    BSIM4dtoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dtox == -12345789),0,1))*UNITFAD).val();
    BSIM4epsroxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4epsrox == -12345789),0,1))*UNITFAD).val();
    BSIM4versionGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4version == -12345789),0,1))*UNITFAD).val();
    BSIM4cdscGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cdsc == -12345789),0,1))*UNITFAD).val();
    BSIM4cdscbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cdscb == -12345789),0,1))*UNITFAD).val();
    BSIM4cdscdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cdscd == -12345789),0,1))*UNITFAD).val();
    BSIM4citGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cit == -12345789),0,1))*UNITFAD).val();
    BSIM4nfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4xjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xj == -12345789),0,1))*UNITFAD).val();
    BSIM4vsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vsat == -12345789),0,1))*UNITFAD).val();
    BSIM4atGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4at == -12345789),0,1))*UNITFAD).val();
    BSIM4a0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4a0 == -12345789),0,1))*UNITFAD).val();
    BSIM4agsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ags == -12345789),0,1))*UNITFAD).val();
    BSIM4a1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4a1 == -12345789),0,1))*UNITFAD).val();
    BSIM4a2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4a2 == -12345789),0,1))*UNITFAD).val();
    BSIM4ketaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4keta == -12345789),0,1))*UNITFAD).val();
    BSIM4nsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nsub == -12345789),0,1))*UNITFAD).val();
    BSIM4ndepGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ndep == -12345789),0,1))*UNITFAD).val();
    BSIM4nsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nsd == -12345789),0,1))*UNITFAD).val();
    BSIM4phinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4phin == -12345789),0,1))*UNITFAD).val();
    BSIM4ngateGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ngate == -12345789),0,1))*UNITFAD).val();
    BSIM4gamma1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4gamma1 == -12345789),0,1))*UNITFAD).val();
    BSIM4gamma2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4gamma2 == -12345789),0,1))*UNITFAD).val();
    BSIM4vbxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vbx == -12345789),0,1))*UNITFAD).val();
    BSIM4vbmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vbm == -12345789),0,1))*UNITFAD).val();
    BSIM4xtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xt == -12345789),0,1))*UNITFAD).val();
    BSIM4k1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4k1 == -12345789),0,1))*UNITFAD).val();
    BSIM4kt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4kt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4kt1lGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kt1l == -12345789),0,1))*UNITFAD).val();
    BSIM4kt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4kt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4k2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4k2 == -12345789),0,1))*UNITFAD).val();
    BSIM4k3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4k3 == -12345789),0,1))*UNITFAD).val();
    BSIM4k3bGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4k3b == -12345789),0,1))*UNITFAD).val();
    BSIM4w0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4w0 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvtp0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp0 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvtp1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvtp1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpe0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpe0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpebGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpeb == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt0 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt0wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt0w == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt1wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt1w == -12345789),0,1))*UNITFAD).val();
    BSIM4dvt2wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dvt2w == -12345789),0,1))*UNITFAD).val();
    BSIM4droutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4drout == -12345789),0,1))*UNITFAD).val();
    BSIM4dsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dsub == -12345789),0,1))*UNITFAD).val();
    BSIM4vth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4vth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4euGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4eu == -12345789),0,1))*UNITFAD).val();
    BSIM4uaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ua == -12345789),0,1))*UNITFAD).val();
    BSIM4ua1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ua1 == -12345789),0,1))*UNITFAD).val();
    BSIM4ubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ub == -12345789),0,1))*UNITFAD).val();
    BSIM4ub1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ub1 == -12345789),0,1))*UNITFAD).val();
    BSIM4ucGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4uc == -12345789),0,1))*UNITFAD).val();
    BSIM4uc1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4uc1 == -12345789),0,1))*UNITFAD).val();
    BSIM4u0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4u0 == -12345789),0,1))*UNITFAD).val();
    BSIM4uteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ute == -12345789),0,1))*UNITFAD).val();
    BSIM4voffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4voff == -12345789),0,1))*UNITFAD).val();
    BSIM4vofflGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4voffl == -12345789),0,1))*UNITFAD).val();
    BSIM4minvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4minv == -12345789),0,1))*UNITFAD).val();
    BSIM4rdswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rdsw == -12345789),0,1))*UNITFAD).val();
    BSIM4rdswminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rdswmin == -12345789),0,1))*UNITFAD).val();
    BSIM4rdwminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rdwmin == -12345789),0,1))*UNITFAD).val();
    BSIM4rswminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rswmin == -12345789),0,1))*UNITFAD).val();
    BSIM4rswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rsw == -12345789),0,1))*UNITFAD).val();
    BSIM4rdwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rdw == -12345789),0,1))*UNITFAD).val();
    BSIM4prwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prwg == -12345789),0,1))*UNITFAD).val();
    BSIM4prwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prwb == -12345789),0,1))*UNITFAD).val();
    BSIM4prtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prt == -12345789),0,1))*UNITFAD).val();
    BSIM4eta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4eta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4etabGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4etab == -12345789),0,1))*UNITFAD).val();
    BSIM4pclmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pclm == -12345789),0,1))*UNITFAD).val();
    BSIM4pdibl1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdibl1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdibl2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdibl2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdiblbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdiblb == -12345789),0,1))*UNITFAD).val();
    BSIM4fproutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4fprout == -12345789),0,1))*UNITFAD).val();
    BSIM4pditsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdits == -12345789),0,1))*UNITFAD).val();
    BSIM4pditsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pditsd == -12345789),0,1))*UNITFAD).val();
    BSIM4pditslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pditsl == -12345789),0,1))*UNITFAD).val();
    BSIM4pscbe1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pscbe1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pscbe2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pscbe2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pvagGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvag == -12345789),0,1))*UNITFAD).val();
    BSIM4deltaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4delta == -12345789),0,1))*UNITFAD).val();
    BSIM4wrGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wr == -12345789),0,1))*UNITFAD).val();
    BSIM4dwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dwg == -12345789),0,1))*UNITFAD).val();
    BSIM4dwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dwb == -12345789),0,1))*UNITFAD).val();
    BSIM4b0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4b0 == -12345789),0,1))*UNITFAD).val();
    BSIM4b1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4b1 == -12345789),0,1))*UNITFAD).val();
    BSIM4alpha0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4alpha0 == -12345789),0,1))*UNITFAD).val();
    BSIM4alpha1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4alpha1 == -12345789),0,1))*UNITFAD).val();
    BSIM4beta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4beta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4agidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4agidl == -12345789),0,1))*UNITFAD).val();
    BSIM4bgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4cgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4egidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4egidl == -12345789),0,1))*UNITFAD).val();
    BSIM4aigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigc == -12345789),0,1))*UNITFAD).val();
    BSIM4bigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigc == -12345789),0,1))*UNITFAD).val();
    BSIM4cigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigc == -12345789),0,1))*UNITFAD).val();
    BSIM4aigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4bigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4cigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4aigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4bigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4cigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4aigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4aigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4bigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4cigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4nigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nigc == -12345789),0,1))*UNITFAD).val();
    BSIM4nigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4nigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4nigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4ntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ntox == -12345789),0,1))*UNITFAD).val();
    BSIM4eigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4eigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4pigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pigcd == -12345789),0,1))*UNITFAD).val();
    BSIM4poxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4poxedge == -12345789),0,1))*UNITFAD).val();
    BSIM4ijthdfwdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ijthdfwd == -12345789),0,1))*UNITFAD).val();
    BSIM4ijthsfwdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ijthsfwd == -12345789),0,1))*UNITFAD).val();
    BSIM4ijthdrevGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ijthdrev == -12345789),0,1))*UNITFAD).val();
    BSIM4ijthsrevGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ijthsrev == -12345789),0,1))*UNITFAD).val();
    BSIM4xjbvdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xjbvd == -12345789),0,1))*UNITFAD).val();
    BSIM4xjbvsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xjbvs == -12345789),0,1))*UNITFAD).val();
    BSIM4bvdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bvd == -12345789),0,1))*UNITFAD).val();
    BSIM4bvsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4bvs == -12345789),0,1))*UNITFAD).val();
    BSIM4vfbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vfb == -12345789),0,1))*UNITFAD).val();
    BSIM4gbminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4gbmin == -12345789),0,1))*UNITFAD).val();
    BSIM4xrcrg1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4xrcrg1 == -12345789),0,1))*UNITFAD).val();
    BSIM4xrcrg2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4xrcrg2 == -12345789),0,1))*UNITFAD).val();
    BSIM4tnoiaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnoia == -12345789),0,1))*UNITFAD).val();
    BSIM4tnoibGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnoib == -12345789),0,1))*UNITFAD).val();
    BSIM4tnoicGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnoic == -12345789),0,1))*UNITFAD).val();
    BSIM4ntnoiGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ntnoi == -12345789),0,1))*UNITFAD).val();
    BSIM4cgslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgsl == -12345789),0,1))*UNITFAD).val();
    BSIM4cgdlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgdl == -12345789),0,1))*UNITFAD).val();
    BSIM4ckappasGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ckappas == -12345789),0,1))*UNITFAD).val();
    BSIM4ckappadGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ckappad == -12345789),0,1))*UNITFAD).val();
    BSIM4cfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cf == -12345789),0,1))*UNITFAD).val();
    BSIM4vfbcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vfbcv == -12345789),0,1))*UNITFAD).val();
    BSIM4clcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4clc == -12345789),0,1))*UNITFAD).val();
    BSIM4cleGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cle == -12345789),0,1))*UNITFAD).val();
    BSIM4dwcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dwc == -12345789),0,1))*UNITFAD).val();
    BSIM4dlcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dlc == -12345789),0,1))*UNITFAD).val();
    BSIM4xwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xw == -12345789),0,1))*UNITFAD).val();
    BSIM4xlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xl == -12345789),0,1))*UNITFAD).val();
    BSIM4dlcigGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dlcig == -12345789),0,1))*UNITFAD).val();
    BSIM4dlcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dlcigd == -12345789),0,1))*UNITFAD).val();
    BSIM4dwjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dwj == -12345789),0,1))*UNITFAD).val();
    BSIM4noffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4noff == -12345789),0,1))*UNITFAD).val();
    BSIM4voffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4voffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4acdeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4acde == -12345789),0,1))*UNITFAD).val();
    BSIM4moinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4moin == -12345789),0,1))*UNITFAD).val();
    BSIM4tcjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tcj == -12345789),0,1))*UNITFAD).val();
    BSIM4tcjswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tcjsw == -12345789),0,1))*UNITFAD).val();
    BSIM4tcjswgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tcjswg == -12345789),0,1))*UNITFAD).val();
    BSIM4tpbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tpb == -12345789),0,1))*UNITFAD).val();
    BSIM4tpbswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tpbsw == -12345789),0,1))*UNITFAD).val();
    BSIM4tpbswgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tpbswg == -12345789),0,1))*UNITFAD).val();
    BSIM4dmcgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dmcg == -12345789),0,1))*UNITFAD).val();
    BSIM4dmciGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dmci == -12345789),0,1))*UNITFAD).val();
    BSIM4dmdgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dmdg == -12345789),0,1))*UNITFAD).val();
    BSIM4dmcgtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4dmcgt == -12345789),0,1))*UNITFAD).val();
    BSIM4xgwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xgw == -12345789),0,1))*UNITFAD).val();
    BSIM4xglGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xgl == -12345789),0,1))*UNITFAD).val();
    BSIM4rshgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rshg == -12345789),0,1))*UNITFAD).val();
    BSIM4ngconGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ngcon == -12345789),0,1))*UNITFAD).val();
    BSIM4lcdscGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcdsc == -12345789),0,1))*UNITFAD).val();
    BSIM4lcdscbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcdscb == -12345789),0,1))*UNITFAD).val();
    BSIM4lcdscdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcdscd == -12345789),0,1))*UNITFAD).val();
    BSIM4lcitGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcit == -12345789),0,1))*UNITFAD).val();
    BSIM4lnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4lxjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lxj == -12345789),0,1))*UNITFAD).val();
    BSIM4lvsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvsat == -12345789),0,1))*UNITFAD).val();
    BSIM4latGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lat == -12345789),0,1))*UNITFAD).val();
    BSIM4la0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4la0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lagsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lags == -12345789),0,1))*UNITFAD).val();
    BSIM4la1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4la1 == -12345789),0,1))*UNITFAD).val();
    BSIM4la2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4la2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lketaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lketa == -12345789),0,1))*UNITFAD).val();
    BSIM4lnsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnsub == -12345789),0,1))*UNITFAD).val();
    BSIM4lndepGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lndep == -12345789),0,1))*UNITFAD).val();
    BSIM4lnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnsd == -12345789),0,1))*UNITFAD).val();
    BSIM4lphinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lphin == -12345789),0,1))*UNITFAD).val();
    BSIM4lngateGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lngate == -12345789),0,1))*UNITFAD).val();
    BSIM4lgamma1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lgamma1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lgamma2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lgamma2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lvbxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvbx == -12345789),0,1))*UNITFAD).val();
    BSIM4lvbmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvbm == -12345789),0,1))*UNITFAD).val();
    BSIM4lxtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lxt == -12345789),0,1))*UNITFAD).val();
    BSIM4lk1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lk1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lkt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lkt1lGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkt1l == -12345789),0,1))*UNITFAD).val();
    BSIM4lkt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lk2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lk3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lk3 == -12345789),0,1))*UNITFAD).val();
    BSIM4lk3bGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lk3b == -12345789),0,1))*UNITFAD).val();
    BSIM4lw0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lw0 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp0 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvtp1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvtp1 == -12345789),0,1))*UNITFAD).val();
    BSIM4llpe0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4llpe0 == -12345789),0,1))*UNITFAD).val();
    BSIM4llpebGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4llpeb == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt0 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt0wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt0w == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt1wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt1w == -12345789),0,1))*UNITFAD).val();
    BSIM4ldvt2wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldvt2w == -12345789),0,1))*UNITFAD).val();
    BSIM4ldroutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldrout == -12345789),0,1))*UNITFAD).val();
    BSIM4ldsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldsub == -12345789),0,1))*UNITFAD).val();
    BSIM4lvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4luaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lua == -12345789),0,1))*UNITFAD).val();
    BSIM4lua1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lua1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lub == -12345789),0,1))*UNITFAD).val();
    BSIM4lub1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lub1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lucGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4luc == -12345789),0,1))*UNITFAD).val();
    BSIM4luc1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4luc1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lu0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lu0 == -12345789),0,1))*UNITFAD).val();
    BSIM4leuGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4leu == -12345789),0,1))*UNITFAD).val();
    BSIM4luteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lute == -12345789),0,1))*UNITFAD).val();
    BSIM4lvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4lminvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lminv == -12345789),0,1))*UNITFAD).val();
    BSIM4lrdswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lrdsw == -12345789),0,1))*UNITFAD).val();
    BSIM4lrswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lrsw == -12345789),0,1))*UNITFAD).val();
    BSIM4lrdwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lrdw == -12345789),0,1))*UNITFAD).val();
    BSIM4lprwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lprwg == -12345789),0,1))*UNITFAD).val();
    BSIM4lprwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lprwb == -12345789),0,1))*UNITFAD).val();
    BSIM4lprtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lprt == -12345789),0,1))*UNITFAD).val();
    BSIM4leta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4leta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4letabGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4letab == -12345789),0,1))*UNITFAD).val();
    BSIM4lpclmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpclm == -12345789),0,1))*UNITFAD).val();
    BSIM4lpdibl1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpdibl1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpdibl2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpdibl2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpdiblbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpdiblb == -12345789),0,1))*UNITFAD).val();
    BSIM4lfproutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lfprout == -12345789),0,1))*UNITFAD).val();
    BSIM4lpditsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpdits == -12345789),0,1))*UNITFAD).val();
    BSIM4lpditsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpditsd == -12345789),0,1))*UNITFAD).val();
    BSIM4lpscbe1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpscbe1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpscbe2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpscbe2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lpvagGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpvag == -12345789),0,1))*UNITFAD).val();
    BSIM4ldeltaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldelta == -12345789),0,1))*UNITFAD).val();
    BSIM4lwrGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lwr == -12345789),0,1))*UNITFAD).val();
    BSIM4ldwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldwg == -12345789),0,1))*UNITFAD).val();
    BSIM4ldwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ldwb == -12345789),0,1))*UNITFAD).val();
    BSIM4lb0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lb0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lb1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lb1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lalpha0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lalpha0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lalpha1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lalpha1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lbeta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbeta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lvfbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvfb == -12345789),0,1))*UNITFAD).val();
    BSIM4lagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lagidl == -12345789),0,1))*UNITFAD).val();
    BSIM4lbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4lcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4legidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4legidl == -12345789),0,1))*UNITFAD).val();
    BSIM4laigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigc == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigc == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigc == -12345789),0,1))*UNITFAD).val();
    BSIM4laigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4laigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4laigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4laigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4lbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lbigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4lcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4lnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnigc == -12345789),0,1))*UNITFAD).val();
    BSIM4lnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4lnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4lntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lntox == -12345789),0,1))*UNITFAD).val();
    BSIM4leigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4leigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4lpigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpigcd == -12345789),0,1))*UNITFAD).val();
    BSIM4lpoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lpoxedge == -12345789),0,1))*UNITFAD).val();
    BSIM4lxrcrg1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lxrcrg1 == -12345789),0,1))*UNITFAD).val();
    BSIM4lxrcrg2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lxrcrg2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lcgslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcgsl == -12345789),0,1))*UNITFAD).val();
    BSIM4lcgdlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcgdl == -12345789),0,1))*UNITFAD).val();
    BSIM4lckappasGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lckappas == -12345789),0,1))*UNITFAD).val();
    BSIM4lckappadGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lckappad == -12345789),0,1))*UNITFAD).val();
    BSIM4lcfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcf == -12345789),0,1))*UNITFAD).val();
    BSIM4lclcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lclc == -12345789),0,1))*UNITFAD).val();
    BSIM4lcleGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lcle == -12345789),0,1))*UNITFAD).val();
    BSIM4lvfbcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvfbcv == -12345789),0,1))*UNITFAD).val();
    BSIM4lnoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lnoff == -12345789),0,1))*UNITFAD).val();
    BSIM4lvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4lacdeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lacde == -12345789),0,1))*UNITFAD).val();
    BSIM4lmoinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lmoin == -12345789),0,1))*UNITFAD).val();
    BSIM4wcdscGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcdsc == -12345789),0,1))*UNITFAD).val();
    BSIM4wcdscbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcdscb == -12345789),0,1))*UNITFAD).val();
    BSIM4wcdscdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcdscd == -12345789),0,1))*UNITFAD).val();
    BSIM4wcitGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcit == -12345789),0,1))*UNITFAD).val();
    BSIM4wnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4wxjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wxj == -12345789),0,1))*UNITFAD).val();
    BSIM4wvsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvsat == -12345789),0,1))*UNITFAD).val();
    BSIM4watGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wat == -12345789),0,1))*UNITFAD).val();
    BSIM4wa0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wa0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wagsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wags == -12345789),0,1))*UNITFAD).val();
    BSIM4wa1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wa1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wa2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wa2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wketaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wketa == -12345789),0,1))*UNITFAD).val();
    BSIM4wnsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnsub == -12345789),0,1))*UNITFAD).val();
    BSIM4wndepGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wndep == -12345789),0,1))*UNITFAD).val();
    BSIM4wnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnsd == -12345789),0,1))*UNITFAD).val();
    BSIM4wphinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wphin == -12345789),0,1))*UNITFAD).val();
    BSIM4wngateGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wngate == -12345789),0,1))*UNITFAD).val();
    BSIM4wgamma1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wgamma1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wgamma2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wgamma2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wvbxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvbx == -12345789),0,1))*UNITFAD).val();
    BSIM4wvbmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvbm == -12345789),0,1))*UNITFAD).val();
    BSIM4wxtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wxt == -12345789),0,1))*UNITFAD).val();
    BSIM4wk1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wk1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wkt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wkt1lGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkt1l == -12345789),0,1))*UNITFAD).val();
    BSIM4wkt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wk2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wk3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wk3 == -12345789),0,1))*UNITFAD).val();
    BSIM4wk3bGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wk3b == -12345789),0,1))*UNITFAD).val();
    BSIM4ww0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ww0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvtp1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvtp1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wlpe0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlpe0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wlpebGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlpeb == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt0wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt0w == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt1wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt1w == -12345789),0,1))*UNITFAD).val();
    BSIM4wdvt2wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdvt2w == -12345789),0,1))*UNITFAD).val();
    BSIM4wdroutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdrout == -12345789),0,1))*UNITFAD).val();
    BSIM4wdsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdsub == -12345789),0,1))*UNITFAD).val();
    BSIM4wvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wuaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wua == -12345789),0,1))*UNITFAD).val();
    BSIM4wua1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wua1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wub == -12345789),0,1))*UNITFAD).val();
    BSIM4wub1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wub1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wucGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wuc == -12345789),0,1))*UNITFAD).val();
    BSIM4wuc1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wuc1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wu0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wu0 == -12345789),0,1))*UNITFAD).val();
    BSIM4weuGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4weu == -12345789),0,1))*UNITFAD).val();
    BSIM4wuteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wute == -12345789),0,1))*UNITFAD).val();
    BSIM4wvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wminvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wminv == -12345789),0,1))*UNITFAD).val();
    BSIM4wrdswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wrdsw == -12345789),0,1))*UNITFAD).val();
    BSIM4wrswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wrsw == -12345789),0,1))*UNITFAD).val();
    BSIM4wrdwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wrdw == -12345789),0,1))*UNITFAD).val();
    BSIM4wprwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wprwg == -12345789),0,1))*UNITFAD).val();
    BSIM4wprwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wprwb == -12345789),0,1))*UNITFAD).val();
    BSIM4wprtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wprt == -12345789),0,1))*UNITFAD).val();
    BSIM4weta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4weta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wetabGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wetab == -12345789),0,1))*UNITFAD).val();
    BSIM4wpclmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpclm == -12345789),0,1))*UNITFAD).val();
    BSIM4wpdibl1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpdibl1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wpdibl2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpdibl2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wpdiblbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpdiblb == -12345789),0,1))*UNITFAD).val();
    BSIM4wfproutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wfprout == -12345789),0,1))*UNITFAD).val();
    BSIM4wpditsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpdits == -12345789),0,1))*UNITFAD).val();
    BSIM4wpditsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpditsd == -12345789),0,1))*UNITFAD).val();
    BSIM4wpscbe1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpscbe1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wpscbe2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpscbe2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wpvagGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpvag == -12345789),0,1))*UNITFAD).val();
    BSIM4wdeltaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdelta == -12345789),0,1))*UNITFAD).val();
    BSIM4wwrGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wwr == -12345789),0,1))*UNITFAD).val();
    BSIM4wdwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdwg == -12345789),0,1))*UNITFAD).val();
    BSIM4wdwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wdwb == -12345789),0,1))*UNITFAD).val();
    BSIM4wb0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wb0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wb1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wb1 == -12345789),0,1))*UNITFAD).val();
    BSIM4walpha0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4walpha0 == -12345789),0,1))*UNITFAD).val();
    BSIM4walpha1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4walpha1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wbeta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbeta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wvfbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvfb == -12345789),0,1))*UNITFAD).val();
    BSIM4wagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wagidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4wegidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wegidl == -12345789),0,1))*UNITFAD).val();
    BSIM4waigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigc == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigc == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigc == -12345789),0,1))*UNITFAD).val();
    BSIM4waigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4waigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4waigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4waigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4wbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wbigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4wcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4wnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnigc == -12345789),0,1))*UNITFAD).val();
    BSIM4wnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4wnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4wntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wntox == -12345789),0,1))*UNITFAD).val();
    BSIM4weigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4weigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4wpigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpigcd == -12345789),0,1))*UNITFAD).val();
    BSIM4wpoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wpoxedge == -12345789),0,1))*UNITFAD).val();
    BSIM4wxrcrg1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wxrcrg1 == -12345789),0,1))*UNITFAD).val();
    BSIM4wxrcrg2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wxrcrg2 == -12345789),0,1))*UNITFAD).val();
    BSIM4wcgslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcgsl == -12345789),0,1))*UNITFAD).val();
    BSIM4wcgdlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcgdl == -12345789),0,1))*UNITFAD).val();
    BSIM4wckappasGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wckappas == -12345789),0,1))*UNITFAD).val();
    BSIM4wckappadGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wckappad == -12345789),0,1))*UNITFAD).val();
    BSIM4wcfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcf == -12345789),0,1))*UNITFAD).val();
    BSIM4wclcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wclc == -12345789),0,1))*UNITFAD).val();
    BSIM4wcleGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wcle == -12345789),0,1))*UNITFAD).val();
    BSIM4wvfbcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvfbcv == -12345789),0,1))*UNITFAD).val();
    BSIM4wnoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wnoff == -12345789),0,1))*UNITFAD).val();
    BSIM4wvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4wacdeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wacde == -12345789),0,1))*UNITFAD).val();
    BSIM4wmoinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wmoin == -12345789),0,1))*UNITFAD).val();
    BSIM4pcdscGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcdsc == -12345789),0,1))*UNITFAD).val();
    BSIM4pcdscbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcdscb == -12345789),0,1))*UNITFAD).val();
    BSIM4pcdscdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcdscd == -12345789),0,1))*UNITFAD).val();
    BSIM4pcitGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcit == -12345789),0,1))*UNITFAD).val();
    BSIM4pnfactorGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnfactor == -12345789),0,1))*UNITFAD).val();
    BSIM4pxjGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pxj == -12345789),0,1))*UNITFAD).val();
    BSIM4pvsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvsat == -12345789),0,1))*UNITFAD).val();
    BSIM4patGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pat == -12345789),0,1))*UNITFAD).val();
    BSIM4pa0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pa0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pagsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pags == -12345789),0,1))*UNITFAD).val();
    BSIM4pa1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pa1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pa2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pa2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pketaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pketa == -12345789),0,1))*UNITFAD).val();
    BSIM4pnsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnsub == -12345789),0,1))*UNITFAD).val();
    BSIM4pndepGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pndep == -12345789),0,1))*UNITFAD).val();
    BSIM4pnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnsd == -12345789),0,1))*UNITFAD).val();
    BSIM4pphinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pphin == -12345789),0,1))*UNITFAD).val();
    BSIM4pngateGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pngate == -12345789),0,1))*UNITFAD).val();
    BSIM4pgamma1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pgamma1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pgamma2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pgamma2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pvbxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvbx == -12345789),0,1))*UNITFAD).val();
    BSIM4pvbmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvbm == -12345789),0,1))*UNITFAD).val();
    BSIM4pxtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pxt == -12345789),0,1))*UNITFAD).val();
    BSIM4pk1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pk1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pkt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pkt1lGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkt1l == -12345789),0,1))*UNITFAD).val();
    BSIM4pkt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pk2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pk3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pk3 == -12345789),0,1))*UNITFAD).val();
    BSIM4pk3bGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pk3b == -12345789),0,1))*UNITFAD).val();
    BSIM4pw0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pw0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvtp1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvtp1 == -12345789),0,1))*UNITFAD).val();
    BSIM4plpe0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4plpe0 == -12345789),0,1))*UNITFAD).val();
    BSIM4plpebGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4plpeb == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt0wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt0w == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt1wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt1w == -12345789),0,1))*UNITFAD).val();
    BSIM4pdvt2wGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdvt2w == -12345789),0,1))*UNITFAD).val();
    BSIM4pdroutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdrout == -12345789),0,1))*UNITFAD).val();
    BSIM4pdsubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdsub == -12345789),0,1))*UNITFAD).val();
    BSIM4pvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4puaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pua == -12345789),0,1))*UNITFAD).val();
    BSIM4pua1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pua1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pubGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pub == -12345789),0,1))*UNITFAD).val();
    BSIM4pub1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pub1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pucGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4puc == -12345789),0,1))*UNITFAD).val();
    BSIM4puc1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4puc1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pu0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pu0 == -12345789),0,1))*UNITFAD).val();
    BSIM4peuGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4peu == -12345789),0,1))*UNITFAD).val();
    BSIM4puteGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pute == -12345789),0,1))*UNITFAD).val();
    BSIM4pvoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvoff == -12345789),0,1))*UNITFAD).val();
    BSIM4pminvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pminv == -12345789),0,1))*UNITFAD).val();
    BSIM4prdswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prdsw == -12345789),0,1))*UNITFAD).val();
    BSIM4prswGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prsw == -12345789),0,1))*UNITFAD).val();
    BSIM4prdwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4prdw == -12345789),0,1))*UNITFAD).val();
    BSIM4pprwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pprwg == -12345789),0,1))*UNITFAD).val();
    BSIM4pprwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pprwb == -12345789),0,1))*UNITFAD).val();
    BSIM4pprtGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pprt == -12345789),0,1))*UNITFAD).val();
    BSIM4peta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4peta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4petabGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4petab == -12345789),0,1))*UNITFAD).val();
    BSIM4ppclmGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppclm == -12345789),0,1))*UNITFAD).val();
    BSIM4ppdibl1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppdibl1 == -12345789),0,1))*UNITFAD).val();
    BSIM4ppdibl2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppdibl2 == -12345789),0,1))*UNITFAD).val();
    BSIM4ppdiblbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppdiblb == -12345789),0,1))*UNITFAD).val();
    BSIM4pfproutGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pfprout == -12345789),0,1))*UNITFAD).val();
    BSIM4ppditsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppdits == -12345789),0,1))*UNITFAD).val();
    BSIM4ppditsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppditsd == -12345789),0,1))*UNITFAD).val();
    BSIM4ppscbe1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppscbe1 == -12345789),0,1))*UNITFAD).val();
    BSIM4ppscbe2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppscbe2 == -12345789),0,1))*UNITFAD).val();
    BSIM4ppvagGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppvag == -12345789),0,1))*UNITFAD).val();
    BSIM4pdeltaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdelta == -12345789),0,1))*UNITFAD).val();
    BSIM4pwrGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pwr == -12345789),0,1))*UNITFAD).val();
    BSIM4pdwgGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdwg == -12345789),0,1))*UNITFAD).val();
    BSIM4pdwbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pdwb == -12345789),0,1))*UNITFAD).val();
    BSIM4pb0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pb0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pb1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pb1 == -12345789),0,1))*UNITFAD).val();
    BSIM4palpha0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4palpha0 == -12345789),0,1))*UNITFAD).val();
    BSIM4palpha1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4palpha1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pbeta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbeta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pvfbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvfb == -12345789),0,1))*UNITFAD).val();
    BSIM4pagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pagidl == -12345789),0,1))*UNITFAD).val();
    BSIM4pbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4pcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcgidl == -12345789),0,1))*UNITFAD).val();
    BSIM4pegidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pegidl == -12345789),0,1))*UNITFAD).val();
    BSIM4paigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigc == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigc == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigc == -12345789),0,1))*UNITFAD).val();
    BSIM4paigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigsd == -12345789),0,1))*UNITFAD).val();
    BSIM4paigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4paigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4paigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4pbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pbigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4pcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4pnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnigc == -12345789),0,1))*UNITFAD).val();
    BSIM4pnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4pnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnigbacc == -12345789),0,1))*UNITFAD).val();
    BSIM4pntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pntox == -12345789),0,1))*UNITFAD).val();
    BSIM4peigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4peigbinv == -12345789),0,1))*UNITFAD).val();
    BSIM4ppigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppigcd == -12345789),0,1))*UNITFAD).val();
    BSIM4ppoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ppoxedge == -12345789),0,1))*UNITFAD).val();
    BSIM4pxrcrg1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pxrcrg1 == -12345789),0,1))*UNITFAD).val();
    BSIM4pxrcrg2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pxrcrg2 == -12345789),0,1))*UNITFAD).val();
    BSIM4pcgslGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcgsl == -12345789),0,1))*UNITFAD).val();
    BSIM4pcgdlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcgdl == -12345789),0,1))*UNITFAD).val();
    BSIM4pckappasGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pckappas == -12345789),0,1))*UNITFAD).val();
    BSIM4pckappadGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pckappad == -12345789),0,1))*UNITFAD).val();
    BSIM4pcfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcf == -12345789),0,1))*UNITFAD).val();
    BSIM4pclcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pclc == -12345789),0,1))*UNITFAD).val();
    BSIM4pcleGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pcle == -12345789),0,1))*UNITFAD).val();
    BSIM4pvfbcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvfbcv == -12345789),0,1))*UNITFAD).val();
    BSIM4pnoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pnoff == -12345789),0,1))*UNITFAD).val();
    BSIM4pvoffcvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvoffcv == -12345789),0,1))*UNITFAD).val();
    BSIM4pacdeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pacde == -12345789),0,1))*UNITFAD).val();
    BSIM4pmoinGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pmoin == -12345789),0,1))*UNITFAD).val();
    BSIM4tnomGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tnom == -12345789),0,1))*UNITFAD).val();
    BSIM4cgsoGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgso == -12345789),0,1))*UNITFAD).val();
    BSIM4cgdoGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgdo == -12345789),0,1))*UNITFAD).val();
    BSIM4cgboGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4cgbo == -12345789),0,1))*UNITFAD).val();
    BSIM4xpartGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xpart == -12345789),0,1))*UNITFAD).val();
    BSIM4sheetResistanceGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sheetResistance == -12345789),0,1))*UNITFAD).val();
    BSIM4SjctSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SjctSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4SjctSidewallSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SjctSidewallSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4SjctGateSidewallSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SjctGateSidewallSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4SbulkJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SbulkJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4SbulkJctBotGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SbulkJctBotGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4SsidewallJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SsidewallJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4SGatesidewallJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SGatesidewallJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4SbulkJctSideGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SbulkJctSideGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4SunitAreaJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SunitAreaJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4SunitLengthSidewallJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SunitLengthSidewallJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4SbulkJctGateSideGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SbulkJctGateSideGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4SunitLengthGateSidewallJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SunitLengthGateSidewallJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4SjctEmissionCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SjctEmissionCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4SjctTempExponentGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4SjctTempExponent == -12345789),0,1))*UNITFAD).val();
    BSIM4DjctSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DjctSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4DjctSidewallSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DjctSidewallSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4DjctGateSidewallSatCurDensityGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DjctGateSidewallSatCurDensity == -12345789),0,1))*UNITFAD).val();
    BSIM4DbulkJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DbulkJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4DbulkJctBotGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DbulkJctBotGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4DsidewallJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DsidewallJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4DGatesidewallJctPotentialGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DGatesidewallJctPotential == -12345789),0,1))*UNITFAD).val();
    BSIM4DbulkJctSideGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DbulkJctSideGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4DunitAreaJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DunitAreaJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4DunitLengthSidewallJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DunitLengthSidewallJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4DbulkJctGateSideGradingCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DbulkJctGateSideGradingCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4DunitLengthGateSidewallJctCapGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DunitLengthGateSidewallJctCap == -12345789),0,1))*UNITFAD).val();
    BSIM4DjctEmissionCoeffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DjctEmissionCoeff == -12345789),0,1))*UNITFAD).val();
    BSIM4DjctTempExponentGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4DjctTempExponent == -12345789),0,1))*UNITFAD).val();
    BSIM4oxideTrapDensityAGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4oxideTrapDensityA == -12345789),0,1))*UNITFAD).val();
    BSIM4oxideTrapDensityBGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4oxideTrapDensityB == -12345789),0,1))*UNITFAD).val();
    BSIM4oxideTrapDensityCGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4oxideTrapDensityC == -12345789),0,1))*UNITFAD).val();
    BSIM4emGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4em == -12345789),0,1))*UNITFAD).val();
    BSIM4efGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4ef == -12345789),0,1))*UNITFAD).val();
    BSIM4afGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4af == -12345789),0,1))*UNITFAD).val();
    BSIM4kfGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kf == -12345789),0,1))*UNITFAD).val();
    BSIM4LintGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lint == -12345789),0,1))*UNITFAD).val();
    BSIM4LlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Ll == -12345789),0,1))*UNITFAD).val();
    BSIM4LlcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Llc == -12345789),0,1))*UNITFAD).val();
    BSIM4LlnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lln == -12345789),0,1))*UNITFAD).val();
    BSIM4LwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lw == -12345789),0,1))*UNITFAD).val();
    BSIM4LwcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lwc == -12345789),0,1))*UNITFAD).val();
    BSIM4LwnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lwn == -12345789),0,1))*UNITFAD).val();
    BSIM4LwlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lwl == -12345789),0,1))*UNITFAD).val();
    BSIM4LwlcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lwlc == -12345789),0,1))*UNITFAD).val();
    BSIM4LminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lmin == -12345789),0,1))*UNITFAD).val();
    BSIM4LmaxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Lmax == -12345789),0,1))*UNITFAD).val();
    BSIM4WintGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wint == -12345789),0,1))*UNITFAD).val();
    BSIM4WlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wl == -12345789),0,1))*UNITFAD).val();
    BSIM4WlcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wlc == -12345789),0,1))*UNITFAD).val();
    BSIM4WlnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wln == -12345789),0,1))*UNITFAD).val();
    BSIM4WwGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Ww == -12345789),0,1))*UNITFAD).val();
    BSIM4WwcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wwc == -12345789),0,1))*UNITFAD).val();
    BSIM4WwnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wwn == -12345789),0,1))*UNITFAD).val();
    BSIM4WwlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wwl == -12345789),0,1))*UNITFAD).val();
    BSIM4WwlcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wwlc == -12345789),0,1))*UNITFAD).val();
    BSIM4WminGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wmin == -12345789),0,1))*UNITFAD).val();
    BSIM4WmaxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4Wmax == -12345789),0,1))*UNITFAD).val();
    BSIM4tempModGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4tempMod == -9999999),0,1))*UNITFAD).val();
    BSIM4xnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4xn == -12345789),0,1))*UNITFAD).val();
    BSIM4lxnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lxn == -12345789),0,1))*UNITFAD).val();
    BSIM4wxnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wxn == -12345789),0,1))*UNITFAD).val();
    BSIM4pxnGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pxn == -12345789),0,1))*UNITFAD).val();
    BSIM4rnoiaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rnoia == -12345789),0,1))*UNITFAD).val();
    BSIM4rnoibGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rnoib == -12345789),0,1))*UNITFAD).val();
    BSIM4rnoicGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4rnoic == -12345789),0,1))*UNITFAD).val();
    BSIM4lambdaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lambda == -12345789),0,1))*UNITFAD).val();
    BSIM4llambdaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4llambda == -12345789),0,1))*UNITFAD).val();
    BSIM4wlambdaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlambda == -12345789),0,1))*UNITFAD).val();
    BSIM4plambdaGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4plambda == -12345789),0,1))*UNITFAD).val();
    BSIM4vtlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4vtl == -12345789),0,1))*UNITFAD).val();
    BSIM4lvtlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lvtl == -12345789),0,1))*UNITFAD).val();
    BSIM4wvtlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wvtl == -12345789),0,1))*UNITFAD).val();
    BSIM4pvtlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4pvtl == -12345789),0,1))*UNITFAD).val();
    BSIM4lcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4lc == -12345789),0,1))*UNITFAD).val();
    BSIM4saGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sa == -12345789),0,1))*UNITFAD).val();
    BSIM4sbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sb == -12345789),0,1))*UNITFAD).val();
    BSIM4sdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sd == -12345789),0,1))*UNITFAD).val();
    BSIM4sarefGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4saref == -12345789),0,1))*UNITFAD).val();
    BSIM4sbrefGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4sbref == -12345789),0,1))*UNITFAD).val();
    BSIM4wlodGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlod == -12345789),0,1))*UNITFAD).val();
    BSIM4ku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4ku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4kvsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4kvsat == -12345789),0,1))*UNITFAD).val();
    BSIM4tku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4tku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4llodku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4llodku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wlodku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlodku0 == -12345789),0,1))*UNITFAD).val();
    BSIM4kvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4kvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lkvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4wkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4wkvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4pkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4pkvth0 == -12345789),0,1))*UNITFAD).val();
    BSIM4llodvthGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4llodvth == -12345789),0,1))*UNITFAD).val();
    BSIM4wlodvthGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM4wlodvth == -12345789),0,1))*UNITFAD).val();
    BSIM4stk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4stk2 == -12345789),0,1))*UNITFAD).val();
    BSIM4lodk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lodk2 == -12345789),0,1))*UNITFAD).val();
    BSIM4steta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4steta0 == -12345789),0,1))*UNITFAD).val();
    BSIM4lodeta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM4lodeta0 == -12345789),0,1))*UNITFAD).val();
    if(!BSIM4typeGiven) {
      BSIM4type = ((1)*UNITFAD).val();
    }

    if(!BSIM4mobModGiven) {
      BSIM4mobMod = ((0)*UNITFAD).val();
    }
      else if((((((((BSIM4mobMod != 0) && (BSIM4mobMod != 1)) && (BSIM4mobMod != 2)) && (BSIM4mobMod != 3)) && (BSIM4mobMod != 4)) && (BSIM4mobMod != 5)) && (BSIM4mobMod != 6))) {
      {
        BSIM4mobMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) mobMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4binUnitGiven) {
      BSIM4binUnit = ((1)*UNITFAD).val();
    }

    if(!BSIM4paramChkGiven) {
      BSIM4paramChk = ((1)*UNITFAD).val();
    }

    if(!BSIM4dioModGiven) {
      BSIM4dioMod = ((1)*UNITFAD).val();
    }
      else if((((BSIM4dioMod != 0) && (BSIM4dioMod != 1)) && (BSIM4dioMod != 2))) {
      {
        BSIM4dioMod = ((1)*UNITFAD).val();
        printf("Warning: (instance %M) dioMod has been set to its default value: 1.\n");
      }

    }

    if(!BSIM4capModGiven) {
      BSIM4capMod = ((2)*UNITFAD).val();
    }
      else if((((BSIM4capMod != 0) && (BSIM4capMod != 1)) && (BSIM4capMod != 2))) {
      {
        BSIM4capMod = ((2)*UNITFAD).val();
        printf("Warning: (instance %M) capMod has been set to its default value: 2.\n");
      }

    }

    if(!BSIM4rdsModGiven) {
      BSIM4rdsMod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4rdsMod != 0) && (BSIM4rdsMod != 1))) {
      {
        BSIM4rdsMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) rdsMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4rbodyModGiven) {
      BSIM4rbodyMod = ((0)*UNITFAD).val();
    }
      else if((((BSIM4rbodyMod != 0) && (BSIM4rbodyMod != 1)) && (BSIM4rbodyMod != 2))) {
      {
        BSIM4rbodyMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) rbodyMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4rgateModGiven) {
      BSIM4rgateMod = ((0)*UNITFAD).val();
    }
      else if(((((BSIM4rgateMod != 0) && (BSIM4rgateMod != 1)) && (BSIM4rgateMod != 2)) && (BSIM4rgateMod != 3))) {
      {
        BSIM4rgateMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) rgateMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4perModGiven) {
      BSIM4perMod = ((1)*UNITFAD).val();
    }
      else if(((BSIM4perMod != 0) && (BSIM4perMod != 1))) {
      {
        BSIM4perMod = ((1)*UNITFAD).val();
        printf("Warning: (instance %M) perMod has been set to its default value: 1.\n");
      }

    }

    if(!BSIM4geoModGiven) {
      BSIM4geoMod = ((0)*UNITFAD).val();
    }

    if(!BSIM4rgeoModGiven) {
      BSIM4rgeoMod = ((0)*UNITFAD).val();
    }

    if(!BSIM4fnoiModGiven) {
      BSIM4fnoiMod = ((1)*UNITFAD).val();
    }
      else if(((BSIM4fnoiMod != 0) && (BSIM4fnoiMod != 1))) {
      {
        BSIM4fnoiMod = ((1)*UNITFAD).val();
        printf("Warning: (instance %M) fnoiMod has been set to its default value: 1.\n");
      }

    }

    if(!BSIM4tnoiModGiven) {
      BSIM4tnoiMod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4tnoiMod != 0) && (BSIM4tnoiMod != 1))) {
      {
        BSIM4tnoiMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) tnoiMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4trnqsModGiven) {
      BSIM4trnqsMod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4trnqsMod != 0) && (BSIM4trnqsMod != 1))) {
      {
        BSIM4trnqsMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) trnqsMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4acnqsModGiven) {
      BSIM4acnqsMod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4acnqsMod != 0) && (BSIM4acnqsMod != 1))) {
      {
        BSIM4acnqsMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) acnqsMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4igcModGiven) {
      {
        BSIM4igcMod = ((0)*UNITFAD).val();
        printf("\nRESET igcMod to 0");
      }

    }
      else if((((BSIM4igcMod != 0) && (BSIM4igcMod != 1)) && (BSIM4igcMod != 2))) {
      {
        BSIM4igcMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) igcMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4igbModGiven) {
      BSIM4igbMod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4igbMod != 0) && (BSIM4igbMod != 1))) {
      {
        BSIM4igbMod = ((0)*UNITFAD).val();
        printf("Warning: (instance %M) igbMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4versionGiven) {
      BSIM4version = ((4.8)*UNITFAD).val();
    }

    if(!BSIM4toxrefGiven) {
      BSIM4toxref = ((30.0e-10)*UNITFAD).val();
    }

    if(!BSIM4toxeGiven) {
      BSIM4toxe = ((30.0e-10)*UNITFAD).val();
    }

    if(!BSIM4toxpGiven) {
      BSIM4toxp = ((BSIM4toxe)*UNITFAD).val();
    }

    if(!BSIM4toxmGiven) {
      BSIM4toxm = ((BSIM4toxe)*UNITFAD).val();
    }

    if(!BSIM4dtoxGiven) {
      BSIM4dtox = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4epsroxGiven) {
      BSIM4epsrox = ((3.9)*UNITFAD).val();
    }

    if(!BSIM4cdscGiven) {
      BSIM4cdsc = ((2.4e-4)*UNITFAD).val();
    }

    if(!BSIM4cdscbGiven) {
      BSIM4cdscb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4cdscdGiven) {
      BSIM4cdscd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4citGiven) {
      BSIM4cit = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4nfactorGiven) {
      BSIM4nfactor = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4xjGiven) {
      BSIM4xj = ((0.15e-6)*UNITFAD).val();
    }

    if(!BSIM4vsatGiven) {
      BSIM4vsat = ((8.0e4)*UNITFAD).val();
    }

    if(!BSIM4atGiven) {
      BSIM4at = ((3.3e4)*UNITFAD).val();
    }

    if(!BSIM4a0Given) {
      BSIM4a0 = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4agsGiven) {
      BSIM4ags = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4a1Given) {
      BSIM4a1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4a2Given) {
      BSIM4a2 = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4ketaGiven) {
      BSIM4keta = ((-0.047)*UNITFAD).val();
    }

    if(!BSIM4nsubGiven) {
      BSIM4nsub = ((6.0e16)*UNITFAD).val();
    }

    if(!BSIM4ndepGiven) {
      BSIM4ndep = ((1.7e17)*UNITFAD).val();
    }

    if(!BSIM4nsdGiven) {
      BSIM4nsd = ((1.0e20)*UNITFAD).val();
    }

    if(!BSIM4phinGiven) {
      BSIM4phin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ngateGiven) {
      BSIM4ngate = ((0)*UNITFAD).val();
    }

    if(!BSIM4vbmGiven) {
      BSIM4vbm = ((-3.0)*UNITFAD).val();
    }

    if(!BSIM4xtGiven) {
      BSIM4xt = ((1.55e-7)*UNITFAD).val();
    }

    if(!BSIM4kt1Given) {
      BSIM4kt1 = ((-0.11)*UNITFAD).val();
    }

    if(!BSIM4kt1lGiven) {
      BSIM4kt1l = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4kt2Given) {
      BSIM4kt2 = ((0.022)*UNITFAD).val();
    }

    if(!BSIM4k3Given) {
      BSIM4k3 = ((80.0)*UNITFAD).val();
    }

    if(!BSIM4k3bGiven) {
      BSIM4k3b = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4w0Given) {
      BSIM4w0 = ((2.5e-6)*UNITFAD).val();
    }

    if(!BSIM4lpe0Given) {
      BSIM4lpe0 = ((1.74e-7)*UNITFAD).val();
    }

    if(!BSIM4lpebGiven) {
      BSIM4lpeb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp0Given) {
      BSIM4dvtp0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp1Given) {
      BSIM4dvtp1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvt0Given) {
      BSIM4dvt0 = ((2.2)*UNITFAD).val();
    }

    if(!BSIM4dvt1Given) {
      BSIM4dvt1 = ((0.53)*UNITFAD).val();
    }

    if(!BSIM4dvt2Given) {
      BSIM4dvt2 = ((-0.032)*UNITFAD).val();
    }

    if(!BSIM4dvt0wGiven) {
      BSIM4dvt0w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvt1wGiven) {
      BSIM4dvt1w = ((5.3e6)*UNITFAD).val();
    }

    if(!BSIM4dvt2wGiven) {
      BSIM4dvt2w = ((-0.032)*UNITFAD).val();
    }

    if(!BSIM4droutGiven) {
      BSIM4drout = ((0.56)*UNITFAD).val();
    }

    if(!BSIM4dsubGiven) {
      BSIM4dsub = ((BSIM4drout)*UNITFAD).val();
    }

    if(!BSIM4vth0Given) {
      BSIM4vth0 = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.7,-0.7))*UNITFAD).val();
    }

    if(!BSIM4vfbGiven) {
      BSIM4vfb = ((-1.0)*UNITFAD).val();
    }

    if(!BSIM4euGiven) {
      BSIM4eu = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.67,1.0))*UNITFAD).val();
    }

    if(!BSIM4uaGiven) {
      BSIM4ua = ((cogenda_ternary_op<CogendaFadType>((BSIM4mobMod == 2),1.0e-15,1.0e-9))*UNITFAD).val();
    }

    if(!BSIM4ua1Given) {
      BSIM4ua1 = ((1.0e-9)*UNITFAD).val();
    }

    if(!BSIM4ubGiven) {
      BSIM4ub = ((1.0e-19)*UNITFAD).val();
    }

    if(!BSIM4ub1Given) {
      BSIM4ub1 = ((-1.0e-18)*UNITFAD).val();
    }

    if(!BSIM4ucGiven) {
      BSIM4uc = ((cogenda_ternary_op<CogendaFadType>((BSIM4mobMod == 1),-0.0465,-0.0465e-9))*UNITFAD).val();
    }

    if(!BSIM4uc1Given) {
      BSIM4uc1 = ((cogenda_ternary_op<CogendaFadType>((BSIM4mobMod == 1),-0.056,-0.056e-9))*UNITFAD).val();
    }

    if(!BSIM4u0Given) {
      BSIM4u0 = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.067,0.025))*UNITFAD).val();
    }

    if(!BSIM4uteGiven) {
      BSIM4ute = ((-1.5)*UNITFAD).val();
    }

    if(!BSIM4voffGiven) {
      BSIM4voff = ((-0.08)*UNITFAD).val();
    }

    if(!BSIM4vofflGiven) {
      BSIM4voffl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4minvGiven) {
      BSIM4minv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4fproutGiven) {
      BSIM4fprout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pditsGiven) {
      BSIM4pdits = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pditsdGiven) {
      BSIM4pditsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pditslGiven) {
      BSIM4pditsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4deltaGiven) {
      BSIM4delta = ((0.01)*UNITFAD).val();
    }

    if(!BSIM4rdswminGiven) {
      BSIM4rdswmin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rdwminGiven) {
      BSIM4rdwmin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rswminGiven) {
      BSIM4rswmin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rdswGiven) {
      BSIM4rdsw = ((200.0)*UNITFAD).val();
    }

    if(!BSIM4rdwGiven) {
      BSIM4rdw = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rswGiven) {
      BSIM4rsw = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4prwgGiven) {
      BSIM4prwg = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4prwbGiven) {
      BSIM4prwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4prtGiven) {
      BSIM4prt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4eta0Given) {
      BSIM4eta0 = ((0.08)*UNITFAD).val();
    }

    if(!BSIM4etabGiven) {
      BSIM4etab = ((-0.07)*UNITFAD).val();
    }

    if(!BSIM4pclmGiven) {
      BSIM4pclm = ((1.3)*UNITFAD).val();
    }

    if(!BSIM4pdibl1Given) {
      BSIM4pdibl1 = ((0.39)*UNITFAD).val();
    }

    if(!BSIM4pdibl2Given) {
      BSIM4pdibl2 = ((0.0086)*UNITFAD).val();
    }

    if(!BSIM4pdiblbGiven) {
      BSIM4pdiblb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pscbe1Given) {
      BSIM4pscbe1 = ((4.24e8)*UNITFAD).val();
    }

    if(!BSIM4pscbe2Given) {
      BSIM4pscbe2 = ((1.0e-5)*UNITFAD).val();
    }

    if(!BSIM4pvagGiven) {
      BSIM4pvag = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wrGiven) {
      BSIM4wr = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4dwgGiven) {
      BSIM4dwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dwbGiven) {
      BSIM4dwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4b0Given) {
      BSIM4b0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4b1Given) {
      BSIM4b1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4alpha0Given) {
      BSIM4alpha0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4alpha1Given) {
      BSIM4alpha1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4beta0Given) {
      BSIM4beta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4agidlGiven) {
      BSIM4agidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4bgidlGiven) {
      BSIM4bgidl = ((2.3e9)*UNITFAD).val();
    }

    if(!BSIM4cgidlGiven) {
      BSIM4cgidl = ((0.5)*UNITFAD).val();
    }

    if(!BSIM4egidlGiven) {
      BSIM4egidl = ((0.8)*UNITFAD).val();
    }

    if(!BSIM4aigcGiven) {
      BSIM4aigc = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
    }

    if(!BSIM4bigcGiven) {
      BSIM4bigc = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
    }

    if(!BSIM4cigcGiven) {
      BSIM4cigc = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.075,0.03))*UNITFAD).val();
    }

    if(BSIM4aigsdGiven) {
      {
        BSIM4aigs = ((BSIM4aigsd)*UNITFAD).val();
        BSIM4aigd = ((BSIM4aigsd)*UNITFAD).val();
      }

    }
    else {
      {
        BSIM4aigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
        if(!BSIM4aigsGiven) {
          BSIM4aigs = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
        }

        if(!BSIM4aigdGiven) {
          BSIM4aigd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
        }

      }

    }

    if(BSIM4bigsdGiven) {
      {
        BSIM4bigs = ((BSIM4bigsd)*UNITFAD).val();
        BSIM4bigd = ((BSIM4bigsd)*UNITFAD).val();
      }

    }
    else {
      {
        BSIM4bigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
        if(!BSIM4bigsGiven) {
          BSIM4bigs = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
        }

        if(!BSIM4bigdGiven) {
          BSIM4bigd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
        }

      }

    }

    if(BSIM4cigsdGiven) {
      {
        BSIM4cigs = ((BSIM4cigsd)*UNITFAD).val();
        BSIM4cigd = ((BSIM4cigsd)*UNITFAD).val();
      }

    }
    else {
      {
        BSIM4cigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.075,0.03))*UNITFAD).val();
        if(!BSIM4cigsGiven) {
          BSIM4cigs = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.075,0.03))*UNITFAD).val();
        }

        if(!BSIM4cigdGiven) {
          BSIM4cigd = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),0.075,0.03))*UNITFAD).val();
        }

      }

    }

    if(!BSIM4aigbaccGiven) {
      BSIM4aigbacc = ((1.36e-2)*UNITFAD).val();
    }

    if(!BSIM4bigbaccGiven) {
      BSIM4bigbacc = ((1.71e-3)*UNITFAD).val();
    }

    if(!BSIM4cigbaccGiven) {
      BSIM4cigbacc = ((0.075)*UNITFAD).val();
    }

    if(!BSIM4aigbinvGiven) {
      BSIM4aigbinv = ((1.11e-2)*UNITFAD).val();
    }

    if(!BSIM4bigbinvGiven) {
      BSIM4bigbinv = ((9.49e-4)*UNITFAD).val();
    }

    if(!BSIM4cigbinvGiven) {
      BSIM4cigbinv = ((0.006)*UNITFAD).val();
    }

    if(!BSIM4nigcGiven) {
      BSIM4nigc = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4nigbinvGiven) {
      BSIM4nigbinv = ((3.0)*UNITFAD).val();
    }

    if(!BSIM4nigbaccGiven) {
      BSIM4nigbacc = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4ntoxGiven) {
      BSIM4ntox = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4eigbinvGiven) {
      BSIM4eigbinv = ((1.1)*UNITFAD).val();
    }

    if(!BSIM4pigcdGiven) {
      BSIM4pigcd = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4poxedgeGiven) {
      BSIM4poxedge = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4xrcrg1Given) {
      BSIM4xrcrg1 = ((12.0)*UNITFAD).val();
    }

    if(!BSIM4xrcrg2Given) {
      BSIM4xrcrg2 = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4ijthsfwdGiven) {
      BSIM4ijthsfwd = ((0.1)*UNITFAD).val();
    }

    if(!BSIM4ijthdfwdGiven) {
      BSIM4ijthdfwd = ((BSIM4ijthsfwd)*UNITFAD).val();
    }

    if(!BSIM4ijthsrevGiven) {
      BSIM4ijthsrev = ((0.1)*UNITFAD).val();
    }

    if(!BSIM4ijthdrevGiven) {
      BSIM4ijthdrev = ((BSIM4ijthsrev)*UNITFAD).val();
    }

    if(!BSIM4tnoiaGiven) {
      BSIM4tnoia = ((1.5)*UNITFAD).val();
    }

    if(!BSIM4tnoibGiven) {
      BSIM4tnoib = ((3.5)*UNITFAD).val();
    }

    if(!BSIM4tnoicGiven) {
      BSIM4tnoic = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ntnoiGiven) {
      BSIM4ntnoi = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4xjbvsGiven) {
      BSIM4xjbvs = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4xjbvdGiven) {
      BSIM4xjbvd = ((BSIM4xjbvs)*UNITFAD).val();
    }

    if(!BSIM4bvsGiven) {
      BSIM4bvs = ((10.0)*UNITFAD).val();
    }

    if(!BSIM4bvdGiven) {
      BSIM4bvd = ((BSIM4bvs)*UNITFAD).val();
    }

    if(!BSIM4gbminGiven) {
      BSIM4gbmin = ((1.0e-12)*UNITFAD).val();
    }

    if(!BSIM4rbdbGiven) {
      BSIM4rbdb = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbGiven) {
      BSIM4rbpb = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbsbGiven) {
      BSIM4rbsb = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbpsGiven) {
      BSIM4rbps = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbpdGiven) {
      BSIM4rbpd = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4cgslGiven) {
      BSIM4cgsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4cgdlGiven) {
      BSIM4cgdl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ckappasGiven) {
      BSIM4ckappas = ((0.6)*UNITFAD).val();
    }

    if(!BSIM4ckappadGiven) {
      BSIM4ckappad = ((BSIM4ckappas)*UNITFAD).val();
    }

    if(!BSIM4clcGiven) {
      BSIM4clc = ((0.1e-6)*UNITFAD).val();
    }

    if(!BSIM4cleGiven) {
      BSIM4cle = ((0.6)*UNITFAD).val();
    }

    if(!BSIM4vfbcvGiven) {
      BSIM4vfbcv = ((-1.0)*UNITFAD).val();
    }

    if(!BSIM4acdeGiven) {
      BSIM4acde = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4moinGiven) {
      BSIM4moin = ((15.0)*UNITFAD).val();
    }

    if(!BSIM4noffGiven) {
      BSIM4noff = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4voffcvGiven) {
      BSIM4voffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dmcgGiven) {
      BSIM4dmcg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dmciGiven) {
      BSIM4dmci = ((BSIM4dmcg)*UNITFAD).val();
    }

    if(!BSIM4dmdgGiven) {
      BSIM4dmdg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dmcgtGiven) {
      BSIM4dmcgt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4xgwGiven) {
      BSIM4xgw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4xglGiven) {
      BSIM4xgl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rshgGiven) {
      BSIM4rshg = ((0.1)*UNITFAD).val();
    }

    if(!BSIM4ngconGiven) {
      BSIM4ngcon = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4tcjGiven) {
      BSIM4tcj = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tpbGiven) {
      BSIM4tpb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tcjswGiven) {
      BSIM4tcjsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tpbswGiven) {
      BSIM4tpbsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tcjswgGiven) {
      BSIM4tcjswg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tpbswgGiven) {
      BSIM4tpbswg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcdscGiven) {
      BSIM4lcdsc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcdscbGiven) {
      BSIM4lcdscb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcdscdGiven) {
      BSIM4lcdscd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcitGiven) {
      BSIM4lcit = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnfactorGiven) {
      BSIM4lnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lxjGiven) {
      BSIM4lxj = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvsatGiven) {
      BSIM4lvsat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4latGiven) {
      BSIM4lat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4la0Given) {
      BSIM4la0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lagsGiven) {
      BSIM4lags = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4la1Given) {
      BSIM4la1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4la2Given) {
      BSIM4la2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lketaGiven) {
      BSIM4lketa = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnsubGiven) {
      BSIM4lnsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lndepGiven) {
      BSIM4lndep = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnsdGiven) {
      BSIM4lnsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lphinGiven) {
      BSIM4lphin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lngateGiven) {
      BSIM4lngate = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvbmGiven) {
      BSIM4lvbm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lxtGiven) {
      BSIM4lxt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkt1Given) {
      BSIM4lkt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkt1lGiven) {
      BSIM4lkt1l = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkt2Given) {
      BSIM4lkt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lk3Given) {
      BSIM4lk3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lk3bGiven) {
      BSIM4lk3b = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lw0Given) {
      BSIM4lw0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llpe0Given) {
      BSIM4llpe0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llpebGiven) {
      BSIM4llpeb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp0Given) {
      BSIM4ldvtp0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp1Given) {
      BSIM4ldvtp1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt0Given) {
      BSIM4ldvt0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt1Given) {
      BSIM4ldvt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt2Given) {
      BSIM4ldvt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt0wGiven) {
      BSIM4ldvt0w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt1wGiven) {
      BSIM4ldvt1w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvt2wGiven) {
      BSIM4ldvt2w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldroutGiven) {
      BSIM4ldrout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldsubGiven) {
      BSIM4ldsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvth0Given) {
      BSIM4lvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4luaGiven) {
      BSIM4lua = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lua1Given) {
      BSIM4lua1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lubGiven) {
      BSIM4lub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lub1Given) {
      BSIM4lub1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lucGiven) {
      BSIM4luc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4luc1Given) {
      BSIM4luc1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lu0Given) {
      BSIM4lu0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4luteGiven) {
      BSIM4lute = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvoffGiven) {
      BSIM4lvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lminvGiven) {
      BSIM4lminv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lfproutGiven) {
      BSIM4lfprout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpditsGiven) {
      BSIM4lpdits = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpditsdGiven) {
      BSIM4lpditsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldeltaGiven) {
      BSIM4ldelta = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lrdswGiven) {
      BSIM4lrdsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lrdwGiven) {
      BSIM4lrdw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lrswGiven) {
      BSIM4lrsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lprwbGiven) {
      BSIM4lprwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lprwgGiven) {
      BSIM4lprwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lprtGiven) {
      BSIM4lprt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4leta0Given) {
      BSIM4leta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4letabGiven) {
      BSIM4letab = ((-0.0)*UNITFAD).val();
    }

    if(!BSIM4lpclmGiven) {
      BSIM4lpclm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpdibl1Given) {
      BSIM4lpdibl1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpdibl2Given) {
      BSIM4lpdibl2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpdiblbGiven) {
      BSIM4lpdiblb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpscbe1Given) {
      BSIM4lpscbe1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpscbe2Given) {
      BSIM4lpscbe2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpvagGiven) {
      BSIM4lpvag = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lwrGiven) {
      BSIM4lwr = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldwgGiven) {
      BSIM4ldwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldwbGiven) {
      BSIM4ldwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lb0Given) {
      BSIM4lb0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lb1Given) {
      BSIM4lb1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lalpha0Given) {
      BSIM4lalpha0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lalpha1Given) {
      BSIM4lalpha1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbeta0Given) {
      BSIM4lbeta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lagidlGiven) {
      BSIM4lagidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbgidlGiven) {
      BSIM4lbgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcgidlGiven) {
      BSIM4lcgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4legidlGiven) {
      BSIM4legidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4laigcGiven) {
      BSIM4laigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbigcGiven) {
      BSIM4lbigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcigcGiven) {
      BSIM4lcigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4laigsdGiven) {
      BSIM4laigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbigsdGiven) {
      BSIM4lbigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcigsdGiven) {
      BSIM4lcigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4laigbaccGiven) {
      BSIM4laigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbigbaccGiven) {
      BSIM4lbigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcigbaccGiven) {
      BSIM4lcigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4laigbinvGiven) {
      BSIM4laigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lbigbinvGiven) {
      BSIM4lbigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcigbinvGiven) {
      BSIM4lcigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnigcGiven) {
      BSIM4lnigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnigbinvGiven) {
      BSIM4lnigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnigbaccGiven) {
      BSIM4lnigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lntoxGiven) {
      BSIM4lntox = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4leigbinvGiven) {
      BSIM4leigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpigcdGiven) {
      BSIM4lpigcd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpoxedgeGiven) {
      BSIM4lpoxedge = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lxrcrg1Given) {
      BSIM4lxrcrg1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lxrcrg2Given) {
      BSIM4lxrcrg2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4leuGiven) {
      BSIM4leu = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvfbGiven) {
      BSIM4lvfb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcgslGiven) {
      BSIM4lcgsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcgdlGiven) {
      BSIM4lcgdl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lckappasGiven) {
      BSIM4lckappas = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lckappadGiven) {
      BSIM4lckappad = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lclcGiven) {
      BSIM4lclc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcleGiven) {
      BSIM4lcle = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcfGiven) {
      BSIM4lcf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvfbcvGiven) {
      BSIM4lvfbcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lacdeGiven) {
      BSIM4lacde = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lmoinGiven) {
      BSIM4lmoin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lnoffGiven) {
      BSIM4lnoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvoffcvGiven) {
      BSIM4lvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lgamma1Given) {
      BSIM4lgamma1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lgamma2Given) {
      BSIM4lgamma2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvbxGiven) {
      BSIM4lvbx = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lk1Given) {
      BSIM4lk1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lk2Given) {
      BSIM4lk2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcdscGiven) {
      BSIM4wcdsc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcdscbGiven) {
      BSIM4wcdscb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcdscdGiven) {
      BSIM4wcdscd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcitGiven) {
      BSIM4wcit = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnfactorGiven) {
      BSIM4wnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wxjGiven) {
      BSIM4wxj = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvsatGiven) {
      BSIM4wvsat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4watGiven) {
      BSIM4wat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wa0Given) {
      BSIM4wa0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wagsGiven) {
      BSIM4wags = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wa1Given) {
      BSIM4wa1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wa2Given) {
      BSIM4wa2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wketaGiven) {
      BSIM4wketa = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnsubGiven) {
      BSIM4wnsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wndepGiven) {
      BSIM4wndep = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnsdGiven) {
      BSIM4wnsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wphinGiven) {
      BSIM4wphin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wngateGiven) {
      BSIM4wngate = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvbmGiven) {
      BSIM4wvbm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wxtGiven) {
      BSIM4wxt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkt1Given) {
      BSIM4wkt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkt1lGiven) {
      BSIM4wkt1l = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkt2Given) {
      BSIM4wkt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wk3Given) {
      BSIM4wk3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wk3bGiven) {
      BSIM4wk3b = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ww0Given) {
      BSIM4ww0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wlpe0Given) {
      BSIM4wlpe0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wlpebGiven) {
      BSIM4wlpeb = ((BSIM4wlpe0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp0Given) {
      BSIM4wdvtp0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp1Given) {
      BSIM4wdvtp1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt0Given) {
      BSIM4wdvt0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt1Given) {
      BSIM4wdvt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt2Given) {
      BSIM4wdvt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt0wGiven) {
      BSIM4wdvt0w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt1wGiven) {
      BSIM4wdvt1w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvt2wGiven) {
      BSIM4wdvt2w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdroutGiven) {
      BSIM4wdrout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdsubGiven) {
      BSIM4wdsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvth0Given) {
      BSIM4wvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wuaGiven) {
      BSIM4wua = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wua1Given) {
      BSIM4wua1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wubGiven) {
      BSIM4wub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wub1Given) {
      BSIM4wub1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wucGiven) {
      BSIM4wuc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wuc1Given) {
      BSIM4wuc1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wu0Given) {
      BSIM4wu0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wuteGiven) {
      BSIM4wute = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvoffGiven) {
      BSIM4wvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wminvGiven) {
      BSIM4wminv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wfproutGiven) {
      BSIM4wfprout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpditsGiven) {
      BSIM4wpdits = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpditsdGiven) {
      BSIM4wpditsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdeltaGiven) {
      BSIM4wdelta = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wrdswGiven) {
      BSIM4wrdsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wrdwGiven) {
      BSIM4wrdw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wrswGiven) {
      BSIM4wrsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wprwbGiven) {
      BSIM4wprwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wprwgGiven) {
      BSIM4wprwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wprtGiven) {
      BSIM4wprt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4weta0Given) {
      BSIM4weta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wetabGiven) {
      BSIM4wetab = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpclmGiven) {
      BSIM4wpclm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpdibl1Given) {
      BSIM4wpdibl1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpdibl2Given) {
      BSIM4wpdibl2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpdiblbGiven) {
      BSIM4wpdiblb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpscbe1Given) {
      BSIM4wpscbe1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpscbe2Given) {
      BSIM4wpscbe2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpvagGiven) {
      BSIM4wpvag = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wwrGiven) {
      BSIM4wwr = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdwgGiven) {
      BSIM4wdwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdwbGiven) {
      BSIM4wdwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wb0Given) {
      BSIM4wb0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wb1Given) {
      BSIM4wb1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4walpha0Given) {
      BSIM4walpha0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4walpha1Given) {
      BSIM4walpha1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbeta0Given) {
      BSIM4wbeta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wagidlGiven) {
      BSIM4wagidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbgidlGiven) {
      BSIM4wbgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcgidlGiven) {
      BSIM4wcgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wegidlGiven) {
      BSIM4wegidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4waigcGiven) {
      BSIM4waigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbigcGiven) {
      BSIM4wbigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigcGiven) {
      BSIM4wcigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4waigsdGiven) {
      BSIM4waigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbigsdGiven) {
      BSIM4wbigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigsdGiven) {
      BSIM4wcigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4waigbaccGiven) {
      BSIM4waigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbigbaccGiven) {
      BSIM4wbigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigbaccGiven) {
      BSIM4wcigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4waigbinvGiven) {
      BSIM4waigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wbigbinvGiven) {
      BSIM4wbigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigbinvGiven) {
      BSIM4wcigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnigcGiven) {
      BSIM4wnigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnigbinvGiven) {
      BSIM4wnigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnigbaccGiven) {
      BSIM4wnigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wntoxGiven) {
      BSIM4wntox = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4weigbinvGiven) {
      BSIM4weigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpigcdGiven) {
      BSIM4wpigcd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpoxedgeGiven) {
      BSIM4wpoxedge = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wxrcrg1Given) {
      BSIM4wxrcrg1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wxrcrg2Given) {
      BSIM4wxrcrg2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4weuGiven) {
      BSIM4weu = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvfbGiven) {
      BSIM4wvfb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcgslGiven) {
      BSIM4wcgsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcgdlGiven) {
      BSIM4wcgdl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wckappasGiven) {
      BSIM4wckappas = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wckappadGiven) {
      BSIM4wckappad = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcfGiven) {
      BSIM4wcf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wclcGiven) {
      BSIM4wclc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcleGiven) {
      BSIM4wcle = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvfbcvGiven) {
      BSIM4wvfbcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wacdeGiven) {
      BSIM4wacde = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wmoinGiven) {
      BSIM4wmoin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wnoffGiven) {
      BSIM4wnoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvoffcvGiven) {
      BSIM4wvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wgamma2Given) {
      BSIM4wgamma2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvbxGiven) {
      BSIM4wvbx = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wk1Given) {
      BSIM4wk1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wk1Given) {
      BSIM4wk1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcdscGiven) {
      BSIM4pcdsc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcdscbGiven) {
      BSIM4pcdscb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcdscdGiven) {
      BSIM4pcdscd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcitGiven) {
      BSIM4pcit = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnfactorGiven) {
      BSIM4pnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pxjGiven) {
      BSIM4pxj = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvsatGiven) {
      BSIM4pvsat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4patGiven) {
      BSIM4pat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pa0Given) {
      BSIM4pa0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pagsGiven) {
      BSIM4pags = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pa1Given) {
      BSIM4pa1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pa2Given) {
      BSIM4pa2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pketaGiven) {
      BSIM4pketa = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnsubGiven) {
      BSIM4pnsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pndepGiven) {
      BSIM4pndep = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnsdGiven) {
      BSIM4pnsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pphinGiven) {
      BSIM4pphin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pngateGiven) {
      BSIM4pngate = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvbmGiven) {
      BSIM4pvbm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pxtGiven) {
      BSIM4pxt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkt1Given) {
      BSIM4pkt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkt1lGiven) {
      BSIM4pkt1l = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkt2Given) {
      BSIM4pkt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pk3Given) {
      BSIM4pk3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pk3bGiven) {
      BSIM4pk3b = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pw0Given) {
      BSIM4pw0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4plpe0Given) {
      BSIM4plpe0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4plpebGiven) {
      BSIM4plpeb = ((BSIM4plpe0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp0Given) {
      BSIM4pdvtp0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp1Given) {
      BSIM4pdvtp1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt0Given) {
      BSIM4pdvt0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt1Given) {
      BSIM4pdvt1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt2Given) {
      BSIM4pdvt2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt0wGiven) {
      BSIM4pdvt0w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt1wGiven) {
      BSIM4pdvt1w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvt2wGiven) {
      BSIM4pdvt2w = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdroutGiven) {
      BSIM4pdrout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdsubGiven) {
      BSIM4pdsub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvth0Given) {
      BSIM4pvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4puaGiven) {
      BSIM4pua = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pua1Given) {
      BSIM4pua1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pubGiven) {
      BSIM4pub = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pub1Given) {
      BSIM4pub1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pucGiven) {
      BSIM4puc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4puc1Given) {
      BSIM4puc1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pu0Given) {
      BSIM4pu0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4puteGiven) {
      BSIM4pute = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvoffGiven) {
      BSIM4pvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pminvGiven) {
      BSIM4pminv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pfproutGiven) {
      BSIM4pfprout = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppditsGiven) {
      BSIM4ppdits = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppditsdGiven) {
      BSIM4ppditsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdeltaGiven) {
      BSIM4pdelta = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4prdswGiven) {
      BSIM4prdsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4prdwGiven) {
      BSIM4prdw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4prswGiven) {
      BSIM4prsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pprwbGiven) {
      BSIM4pprwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pprwgGiven) {
      BSIM4pprwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pprtGiven) {
      BSIM4pprt = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4peta0Given) {
      BSIM4peta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4petabGiven) {
      BSIM4petab = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppclmGiven) {
      BSIM4ppclm = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppdibl1Given) {
      BSIM4ppdibl1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppdibl2Given) {
      BSIM4ppdibl2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppdiblbGiven) {
      BSIM4ppdiblb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppscbe1Given) {
      BSIM4ppscbe1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppscbe2Given) {
      BSIM4ppscbe2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppvagGiven) {
      BSIM4ppvag = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pwrGiven) {
      BSIM4pwr = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdwgGiven) {
      BSIM4pdwg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdwbGiven) {
      BSIM4pdwb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pb0Given) {
      BSIM4pb0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pb1Given) {
      BSIM4pb1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4palpha0Given) {
      BSIM4palpha0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4palpha1Given) {
      BSIM4palpha1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbeta0Given) {
      BSIM4pbeta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pagidlGiven) {
      BSIM4pagidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbgidlGiven) {
      BSIM4pbgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcgidlGiven) {
      BSIM4pcgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pegidlGiven) {
      BSIM4pegidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4paigcGiven) {
      BSIM4paigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbigcGiven) {
      BSIM4pbigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigcGiven) {
      BSIM4pcigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4paigsdGiven) {
      BSIM4paigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbigsdGiven) {
      BSIM4pbigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigsdGiven) {
      BSIM4pcigsd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4paigbaccGiven) {
      BSIM4paigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbigbaccGiven) {
      BSIM4pbigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigbaccGiven) {
      BSIM4pcigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4paigbinvGiven) {
      BSIM4paigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbigbinvGiven) {
      BSIM4pbigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigbinvGiven) {
      BSIM4pcigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnigcGiven) {
      BSIM4pnigc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnigbinvGiven) {
      BSIM4pnigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnigbaccGiven) {
      BSIM4pnigbacc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pntoxGiven) {
      BSIM4pntox = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4peigbinvGiven) {
      BSIM4peigbinv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppigcdGiven) {
      BSIM4ppigcd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ppoxedgeGiven) {
      BSIM4ppoxedge = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pxrcrg1Given) {
      BSIM4pxrcrg1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pxrcrg2Given) {
      BSIM4pxrcrg2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4peuGiven) {
      BSIM4peu = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvfbGiven) {
      BSIM4pvfb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcgslGiven) {
      BSIM4pcgsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcgdlGiven) {
      BSIM4pcgdl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pckappasGiven) {
      BSIM4pckappas = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pckappadGiven) {
      BSIM4pckappad = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcfGiven) {
      BSIM4pcf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pclcGiven) {
      BSIM4pclc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcleGiven) {
      BSIM4pcle = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvfbcvGiven) {
      BSIM4pvfbcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pacdeGiven) {
      BSIM4pacde = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pmoinGiven) {
      BSIM4pmoin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pnoffGiven) {
      BSIM4pnoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvoffcvGiven) {
      BSIM4pvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pphinGiven) {
      BSIM4pphin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pgamma1Given) {
      BSIM4pgamma1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pgamma2Given) {
      BSIM4pgamma2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvbxGiven) {
      BSIM4pvbx = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvbxGiven) {
      BSIM4pvbx = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pk1Given) {
      BSIM4pk1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pk2Given) {
      BSIM4pk2 = ((0.0)*UNITFAD).val();
    }

    if((BSIM4tnom == -12345789)) {
      BSIM4tnom = (((25+273.15))*UNITFAD).val();
    }
    else {
      BSIM4tnom = (((BSIM4tnom+273.15))*UNITFAD).val();
    }

    if(!BSIM4LintGiven) {
      BSIM4Lint = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4LlGiven) {
      BSIM4Ll = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4LlcGiven) {
      BSIM4Llc = ((BSIM4Ll)*UNITFAD).val();
    }

    if(!BSIM4LlnGiven) {
      BSIM4Lln = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4LwGiven) {
      BSIM4Lw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4LwcGiven) {
      BSIM4Lwc = ((BSIM4Lw)*UNITFAD).val();
    }

    if(!BSIM4LwnGiven) {
      BSIM4Lwn = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4LwlGiven) {
      BSIM4Lwl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4LwlcGiven) {
      BSIM4Lwlc = ((BSIM4Lwl)*UNITFAD).val();
    }

    if(!BSIM4LminGiven) {
      BSIM4Lmin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4LmaxGiven) {
      BSIM4Lmax = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4WintGiven) {
      BSIM4Wint = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4WlGiven) {
      BSIM4Wl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4WlcGiven) {
      BSIM4Wlc = ((BSIM4Wl)*UNITFAD).val();
    }

    if(!BSIM4WlnGiven) {
      BSIM4Wln = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4WwGiven) {
      BSIM4Ww = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4WwcGiven) {
      BSIM4Wwc = ((BSIM4Ww)*UNITFAD).val();
    }

    if(!BSIM4WwnGiven) {
      BSIM4Wwn = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4WwlGiven) {
      BSIM4Wwl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4WwlcGiven) {
      BSIM4Wwlc = ((BSIM4Wwl)*UNITFAD).val();
    }

    if(!BSIM4WminGiven) {
      BSIM4Wmin = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4WmaxGiven) {
      BSIM4Wmax = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4dwcGiven) {
      BSIM4dwc = ((BSIM4Wint)*UNITFAD).val();
    }

    if(!BSIM4dlcGiven) {
      BSIM4dlc = ((BSIM4Lint)*UNITFAD).val();
    }

    if(!BSIM4xlGiven) {
      BSIM4xl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4xwGiven) {
      BSIM4xw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dlcigGiven) {
      BSIM4dlcig = ((BSIM4Lint)*UNITFAD).val();
    }

    if(!BSIM4dlcigdGiven) {
      BSIM4dlcigd = ((BSIM4Lint)*UNITFAD).val();
    }

    if(!BSIM4dwjGiven) {
      BSIM4dwj = ((BSIM4dwc)*UNITFAD).val();
    }

    if(!BSIM4cfGiven) {
      BSIM4cf = ((((((2.0*BSIM4epsrox)*8.854187817e-12)/3.14159265358979323846)*log((1.0+(0.4e-6/BSIM4toxe)))))*UNITFAD).val();
    }

    if(!BSIM4xpartGiven) {
      BSIM4xpart = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sheetResistanceGiven) {
      BSIM4sheetResistance = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4SunitAreaJctCapGiven) {
      BSIM4SunitAreaJctCap = ((5.0E-4)*UNITFAD).val();
    }

    if(!BSIM4DunitAreaJctCapGiven) {
      BSIM4DunitAreaJctCap = ((BSIM4SunitAreaJctCap)*UNITFAD).val();
    }

    if(!BSIM4SunitLengthSidewallJctCapGiven) {
      BSIM4SunitLengthSidewallJctCap = ((5.0E-10)*UNITFAD).val();
    }

    if(!BSIM4DunitLengthSidewallJctCapGiven) {
      BSIM4DunitLengthSidewallJctCap = ((BSIM4SunitLengthSidewallJctCap)*UNITFAD).val();
    }

    if(!BSIM4SunitLengthGateSidewallJctCapGiven) {
      BSIM4SunitLengthGateSidewallJctCap = ((BSIM4SunitLengthSidewallJctCap)*UNITFAD).val();
    }

    if(!BSIM4DunitLengthGateSidewallJctCapGiven) {
      BSIM4DunitLengthGateSidewallJctCap = ((BSIM4SunitLengthGateSidewallJctCap)*UNITFAD).val();
    }

    if(!BSIM4SjctSatCurDensityGiven) {
      BSIM4SjctSatCurDensity = ((1.0E-4)*UNITFAD).val();
    }

    if(!BSIM4DjctSatCurDensityGiven) {
      BSIM4DjctSatCurDensity = ((BSIM4SjctSatCurDensity)*UNITFAD).val();
    }

    if(!BSIM4SjctSidewallSatCurDensityGiven) {
      BSIM4SjctSidewallSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4DjctSidewallSatCurDensityGiven) {
      BSIM4DjctSidewallSatCurDensity = ((BSIM4SjctSidewallSatCurDensity)*UNITFAD).val();
    }

    if(!BSIM4SjctGateSidewallSatCurDensityGiven) {
      BSIM4SjctGateSidewallSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4DjctGateSidewallSatCurDensityGiven) {
      BSIM4DjctGateSidewallSatCurDensity = ((BSIM4SjctGateSidewallSatCurDensity)*UNITFAD).val();
    }

    if(!BSIM4SbulkJctPotentialGiven) {
      BSIM4SbulkJctPotential = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4DbulkJctPotentialGiven) {
      BSIM4DbulkJctPotential = ((BSIM4SbulkJctPotential)*UNITFAD).val();
    }

    if(!BSIM4SsidewallJctPotentialGiven) {
      BSIM4SsidewallJctPotential = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4DsidewallJctPotentialGiven) {
      BSIM4DsidewallJctPotential = ((BSIM4SsidewallJctPotential)*UNITFAD).val();
    }

    if(!BSIM4SGatesidewallJctPotentialGiven) {
      BSIM4SGatesidewallJctPotential = ((BSIM4SsidewallJctPotential)*UNITFAD).val();
    }

    if(!BSIM4DGatesidewallJctPotentialGiven) {
      BSIM4DGatesidewallJctPotential = ((BSIM4SGatesidewallJctPotential)*UNITFAD).val();
    }

    if(!BSIM4SbulkJctBotGradingCoeffGiven) {
      BSIM4SbulkJctBotGradingCoeff = ((0.5)*UNITFAD).val();
    }

    if(!BSIM4DbulkJctBotGradingCoeffGiven) {
      BSIM4DbulkJctBotGradingCoeff = ((BSIM4SbulkJctBotGradingCoeff)*UNITFAD).val();
    }

    if(!BSIM4SbulkJctSideGradingCoeffGiven) {
      BSIM4SbulkJctSideGradingCoeff = ((0.33)*UNITFAD).val();
    }

    if(!BSIM4DbulkJctSideGradingCoeffGiven) {
      BSIM4DbulkJctSideGradingCoeff = ((BSIM4SbulkJctSideGradingCoeff)*UNITFAD).val();
    }

    if(!BSIM4SbulkJctGateSideGradingCoeffGiven) {
      BSIM4SbulkJctGateSideGradingCoeff = ((BSIM4SbulkJctSideGradingCoeff)*UNITFAD).val();
    }

    if(!BSIM4DbulkJctGateSideGradingCoeffGiven) {
      BSIM4DbulkJctGateSideGradingCoeff = ((BSIM4SbulkJctGateSideGradingCoeff)*UNITFAD).val();
    }

    if(!BSIM4SjctEmissionCoeffGiven) {
      BSIM4SjctEmissionCoeff = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4DjctEmissionCoeffGiven) {
      BSIM4DjctEmissionCoeff = ((BSIM4SjctEmissionCoeff)*UNITFAD).val();
    }

    if(!BSIM4SjctTempExponentGiven) {
      BSIM4SjctTempExponent = ((3.0)*UNITFAD).val();
    }

    if(!BSIM4DjctTempExponentGiven) {
      BSIM4DjctTempExponent = ((BSIM4SjctTempExponent)*UNITFAD).val();
    }

    if(!BSIM4oxideTrapDensityAGiven) {
      {
        if((BSIM4type == 1)) {
          BSIM4oxideTrapDensityA = ((6.25e41)*UNITFAD).val();
        }
        else {
          BSIM4oxideTrapDensityA = ((6.188e40)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4oxideTrapDensityBGiven) {
      {
        if((BSIM4type == 1)) {
          BSIM4oxideTrapDensityB = ((3.125e26)*UNITFAD).val();
        }
        else {
          BSIM4oxideTrapDensityB = ((1.5e25)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4oxideTrapDensityCGiven) {
      BSIM4oxideTrapDensityC = ((8.75e9)*UNITFAD).val();
    }

    if(!BSIM4emGiven) {
      BSIM4em = ((4.1e7)*UNITFAD).val();
    }

    if(!BSIM4efGiven) {
      BSIM4ef = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4afGiven) {
      BSIM4af = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4kfGiven) {
      BSIM4kf = ((0.0)*UNITFAD).val();
    }

    DMCGeff = (((BSIM4dmcg-BSIM4dmcgt))*UNITFAD).val();
    DMCIeff = ((BSIM4dmci)*UNITFAD).val();
    DMDGeff = (((BSIM4dmdg-BSIM4dmcgt))*UNITFAD).val();
    if(!BSIM4lGiven) {
      BSIM4l = ((5.0e-6)*UNITFAD).val();
    }

    if(!BSIM4wGiven) {
      BSIM4w = ((5.0e-6)*UNITFAD).val();
    }

    if(!BSIM4nfGiven) {
      BSIM4nf = ((1)*UNITFAD).val();
    }

    if(!BSIM4minGiven) {
      BSIM4min = ((0)*UNITFAD).val();
    }

    if(!BSIM4icVDSGiven) {
      BSIM4icVDS = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4icVGSGiven) {
      BSIM4icVGS = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4icVBSGiven) {
      BSIM4icVBS = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4drainAreaGiven) {
      BSIM4drainArea = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4drainPerimeterGiven) {
      BSIM4drainPerimeter = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4drainSquaresGiven) {
      BSIM4drainSquares = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4sourceAreaGiven) {
      BSIM4sourceArea = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sourcePerimeterGiven) {
      BSIM4sourcePerimeter = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sourceSquaresGiven) {
      BSIM4sourceSquares = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4lambdaGiven) {
      BSIM4lambda = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llambdaGiven) {
      BSIM4llambda = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wlambdaGiven) {
      BSIM4wlambda = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4plambdaGiven) {
      BSIM4plambda = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4vtlGiven) {
      BSIM4vtl = ((2.0e5)*UNITFAD).val();
    }

    if(!BSIM4lvtlGiven) {
      BSIM4lvtl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvtlGiven) {
      BSIM4wvtl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvtlGiven) {
      BSIM4pvtl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lcGiven) {
      BSIM4lc = ((5.0e-9)*UNITFAD).val();
    }

    if(!BSIM4xnGiven) {
      BSIM4xn = ((3.0)*UNITFAD).val();
    }

    if(!BSIM4lxnGiven) {
      BSIM4lxn = ((0.4)*UNITFAD).val();
    }

    if(!BSIM4wxnGiven) {
      BSIM4wxn = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pxnGiven) {
      BSIM4pxn = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tempModGiven) {
      BSIM4tempMod = ((0)*UNITFAD).val();
    }

    if(!BSIM4saGiven) {
      BSIM4sa = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sbGiven) {
      BSIM4sb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sdGiven) {
      BSIM4sd = (((2*BSIM4dmcg))*UNITFAD).val();
    }

    if(!BSIM4sarefGiven) {
      BSIM4saref = ((1.0e-6)*UNITFAD).val();
    }

    if(!BSIM4sbrefGiven) {
      BSIM4sbref = ((1.0e-6)*UNITFAD).val();
    }

    if(!BSIM4wlodGiven) {
      BSIM4wlod = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ku0Given) {
      BSIM4ku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lku0Given) {
      BSIM4lku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4kvsatGiven) {
      BSIM4kvsat = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tku0Given) {
      BSIM4tku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wku0Given) {
      BSIM4wku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pku0Given) {
      BSIM4pku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llodku0Given) {
      BSIM4llodku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wlodku0Given) {
      BSIM4wlodku0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4kvth0Given) {
      BSIM4kvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkvth0Given) {
      BSIM4lkvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkvth0Given) {
      BSIM4wkvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkvth0Given) {
      BSIM4pkvth0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llodvthGiven) {
      BSIM4llodvth = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wlodvthGiven) {
      BSIM4wlodvth = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4stk2Given) {
      BSIM4stk2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lodk2Given) {
      BSIM4lodk2 = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4steta0Given) {
      BSIM4steta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lodeta0Given) {
      BSIM4lodeta0 = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4rnoiaGiven) {
      BSIM4rnoia = ((0.577)*UNITFAD).val();
    }

    if(!BSIM4rnoibGiven) {
      BSIM4rnoib = ((0.5164)*UNITFAD).val();
    }

    if(!BSIM4rnoicGiven) {
      BSIM4rnoic = ((0.395)*UNITFAD).val();
    }

    if(!BSIM4scGiven) {
      BSIM4sc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tvoffcvGiven) {
      BSIM4tvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wtvoffcvGiven) {
      BSIM4wtvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ltvoffcvGiven) {
      BSIM4ltvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ptvoffcvGiven) {
      BSIM4ptvoffcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4teta0Given) {
      BSIM4teta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wteta0Given) {
      BSIM4wteta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lteta0Given) {
      BSIM4lteta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pteta0Given) {
      BSIM4pteta0 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpdlGiven) {
      BSIM4rbpdl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpdnfGiven) {
      BSIM4rbpdnf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4voffcvlGiven) {
      BSIM4voffcvl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tnfactorGiven) {
      BSIM4tnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wtnfactorGiven) {
      BSIM4wtnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ltnfactorGiven) {
      BSIM4ltnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ptnfactorGiven) {
      BSIM4ptnfactor = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4minvcvGiven) {
      BSIM4minvcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wminvcvGiven) {
      BSIM4wminvcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lminvcvGiven) {
      BSIM4lminvcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pminvcvGiven) {
      BSIM4pminvcv = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tvfbsdoffGiven) {
      BSIM4tvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wtvfbsdoffGiven) {
      BSIM4wtvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ltvfbsdoffGiven) {
      BSIM4ltvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ptvfbsdoffGiven) {
      BSIM4ptvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4gidlModGiven) {
      BSIM4gidlMod = ((0.0)*UNITFAD).val();
    }

    if((!BSIM4aigsdGiven && (BSIM4aigsGiven || BSIM4aigdGiven))) {
      {
        if(!BSIM4laigsGiven) {
          BSIM4laigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4laigdGiven) {
          BSIM4laigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4laigsdGiven) {
          BSIM4laigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4laigs = ((BSIM4laigsd)*UNITFAD).val();
        BSIM4laigd = ((BSIM4laigsd)*UNITFAD).val();
      }

    }

    if(!BSIM4wbigsGiven) {
      BSIM4wbigs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pbigsGiven) {
      BSIM4pbigs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigsGiven) {
      BSIM4wcigs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigsGiven) {
      BSIM4pcigs = ((0.0)*UNITFAD).val();
    }

    if((!BSIM4aigsdGiven && (BSIM4aigsGiven || BSIM4aigdGiven))) {
      {
        if(!BSIM4waigsGiven) {
          BSIM4waigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4waigdGiven) {
          BSIM4waigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4waigsdGiven) {
          BSIM4waigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4waigs = ((BSIM4waigsd)*UNITFAD).val();
        BSIM4waigd = ((BSIM4waigsd)*UNITFAD).val();
      }

    }

    if((!BSIM4bigsdGiven && (BSIM4bigsGiven || BSIM4bigdGiven))) {
      {
        if(!BSIM4wbigsGiven) {
          BSIM4wbigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4wbigdGiven) {
          BSIM4wbigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4wbigsdGiven) {
          BSIM4wbigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4wbigs = ((BSIM4wbigsd)*UNITFAD).val();
        BSIM4wbigd = ((BSIM4wbigsd)*UNITFAD).val();
      }

    }

    if((!BSIM4cigsdGiven && (BSIM4cigsGiven || BSIM4cigdGiven))) {
      {
        if(!BSIM4wcigsGiven) {
          BSIM4wcigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4wcigdGiven) {
          BSIM4wcigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4wcigsdGiven) {
          BSIM4wcigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4wcigs = ((BSIM4wcigsd)*UNITFAD).val();
        BSIM4wcigd = ((BSIM4wcigsd)*UNITFAD).val();
      }

    }

    if(!BSIM4wbigdGiven) {
      BSIM4wbigd = ((0.0)*UNITFAD).val();
    }

    if((!BSIM4bigsdGiven && (BSIM4bigsGiven || BSIM4bigdGiven))) {
      {
        if(!BSIM4lbigsGiven) {
          BSIM4lbigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4lbigdGiven) {
          BSIM4lbigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4lbigsdGiven) {
          BSIM4lbigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4lbigs = ((BSIM4lbigsd)*UNITFAD).val();
        BSIM4lbigd = ((BSIM4lbigsd)*UNITFAD).val();
      }

    }

    if(!BSIM4pbigdGiven) {
      BSIM4pbigd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wcigdGiven) {
      BSIM4wcigd = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pcigdGiven) {
      BSIM4pcigd = ((0.0)*UNITFAD).val();
    }

    if((!BSIM4cigsdGiven && (BSIM4cigsGiven || BSIM4cigdGiven))) {
      {
        if(!BSIM4lcigsGiven) {
          BSIM4lcigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4lcigdGiven) {
          BSIM4lcigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4lcigsdGiven) {
          BSIM4lcigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4lcigs = ((BSIM4lcigsd)*UNITFAD).val();
        BSIM4lcigd = ((BSIM4lcigsd)*UNITFAD).val();
      }

    }

    if((!BSIM4aigsdGiven && (BSIM4aigsGiven || BSIM4aigdGiven))) {
      {
        if(!BSIM4paigsGiven) {
          BSIM4paigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4paigdGiven) {
          BSIM4paigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4paigsdGiven) {
          BSIM4paigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4paigs = ((BSIM4paigsd)*UNITFAD).val();
        BSIM4paigd = ((BSIM4paigsd)*UNITFAD).val();
      }

    }

    if((!BSIM4bigsdGiven && (BSIM4bigsGiven || BSIM4bigdGiven))) {
      {
        if(!BSIM4pbigsGiven) {
          BSIM4pbigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4pbigdGiven) {
          BSIM4pbigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4pbigsdGiven) {
          BSIM4pbigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4pbigs = ((BSIM4pbigsd)*UNITFAD).val();
        BSIM4pbigd = ((BSIM4pbigsd)*UNITFAD).val();
      }

    }

    if((!BSIM4cigsdGiven && (BSIM4cigsGiven || BSIM4cigdGiven))) {
      {
        if(!BSIM4pcigsGiven) {
          BSIM4pcigs = ((0.0)*UNITFAD).val();
        }

        if(!BSIM4pcigdGiven) {
          BSIM4pcigd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        if(!BSIM4pcigsdGiven) {
          BSIM4pcigsd = ((0.0)*UNITFAD).val();
        }

        BSIM4pcigs = ((BSIM4pcigsd)*UNITFAD).val();
        BSIM4pcigd = ((BSIM4pcigsd)*UNITFAD).val();
      }

    }

    if(!BSIM4delvtoGiven) {
      BSIM4delvto = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4adosGiven) {
      BSIM4ados = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4bdosGiven) {
      BSIM4bdos = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4rgidlGiven) {
      BSIM4rgidl = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4wrgidlGiven) {
      BSIM4wrgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lrgidlGiven) {
      BSIM4lrgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4prgidlGiven) {
      BSIM4prgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4kgidlGiven) {
      BSIM4kgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkgidlGiven) {
      BSIM4wkgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkgidlGiven) {
      BSIM4lkgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkgidlGiven) {
      BSIM4pkgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4fgidlGiven) {
      BSIM4fgidl = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4wfgidlGiven) {
      BSIM4wfgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lfgidlGiven) {
      BSIM4lfgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pfgidlGiven) {
      BSIM4pfgidl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4agislGiven) {
      BSIM4agisl = ((BSIM4agidl)*UNITFAD).val();
    }

    if(!BSIM4wagislGiven) {
      BSIM4wagisl = ((BSIM4wagidl)*UNITFAD).val();
    }

    if(!BSIM4lagislGiven) {
      BSIM4lagisl = ((BSIM4lagidl)*UNITFAD).val();
    }

    if(!BSIM4pagislGiven) {
      BSIM4pagisl = ((BSIM4pagidl)*UNITFAD).val();
    }

    if(!BSIM4bgislGiven) {
      BSIM4bgisl = ((BSIM4bgidl)*UNITFAD).val();
    }

    if(!BSIM4wbgislGiven) {
      BSIM4wbgisl = ((BSIM4wbgidl)*UNITFAD).val();
    }

    if(!BSIM4lbgislGiven) {
      BSIM4lbgisl = ((BSIM4lbgidl)*UNITFAD).val();
    }

    if(!BSIM4pbgislGiven) {
      BSIM4pbgisl = ((BSIM4pbgidl)*UNITFAD).val();
    }

    if(!BSIM4cgislGiven) {
      BSIM4cgisl = ((BSIM4cgidl)*UNITFAD).val();
    }

    if(!BSIM4wcgislGiven) {
      BSIM4wcgisl = ((BSIM4wcgidl)*UNITFAD).val();
    }

    if(!BSIM4lcgislGiven) {
      BSIM4lcgisl = ((BSIM4lcgidl)*UNITFAD).val();
    }

    if(!BSIM4pcgislGiven) {
      BSIM4pcgisl = ((BSIM4pcgidl)*UNITFAD).val();
    }

    if(!BSIM4egislGiven) {
      BSIM4egisl = ((BSIM4egidl)*UNITFAD).val();
    }

    if(!BSIM4wegislGiven) {
      BSIM4wegisl = ((BSIM4wegidl)*UNITFAD).val();
    }

    if(!BSIM4legislGiven) {
      BSIM4legisl = ((BSIM4legidl)*UNITFAD).val();
    }

    if(!BSIM4pegislGiven) {
      BSIM4pegisl = ((BSIM4pegidl)*UNITFAD).val();
    }

    if(!BSIM4rgislGiven) {
      BSIM4rgisl = ((BSIM4rgidl)*UNITFAD).val();
    }

    if(!BSIM4wrgislGiven) {
      BSIM4wrgisl = ((BSIM4wrgidl)*UNITFAD).val();
    }

    if(!BSIM4lrgislGiven) {
      BSIM4lrgisl = ((BSIM4lrgidl)*UNITFAD).val();
    }

    if(!BSIM4prgislGiven) {
      BSIM4prgisl = ((BSIM4prgidl)*UNITFAD).val();
    }

    if(!BSIM4kgislGiven) {
      BSIM4kgisl = ((BSIM4kgidl)*UNITFAD).val();
    }

    if(!BSIM4wkgislGiven) {
      BSIM4wkgisl = ((BSIM4wkgidl)*UNITFAD).val();
    }

    if(!BSIM4lkgislGiven) {
      BSIM4lkgisl = ((BSIM4lkgidl)*UNITFAD).val();
    }

    if(!BSIM4pkgislGiven) {
      BSIM4pkgisl = ((BSIM4pkgidl)*UNITFAD).val();
    }

    if(!BSIM4fgislGiven) {
      BSIM4fgisl = ((BSIM4fgidl)*UNITFAD).val();
    }

    if(!BSIM4wfgislGiven) {
      BSIM4wfgisl = ((BSIM4wfgidl)*UNITFAD).val();
    }

    if(!BSIM4lfgislGiven) {
      BSIM4lfgisl = ((BSIM4lfgidl)*UNITFAD).val();
    }

    if(!BSIM4pfgislGiven) {
      BSIM4pfgisl = ((BSIM4pfgidl)*UNITFAD).val();
    }

    if(!BSIM4cvchargeModGiven) {
      BSIM4cvchargeMod = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp2Given) {
      BSIM4dvtp2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp2Given) {
      BSIM4wdvtp2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp2Given) {
      BSIM4ldvtp2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp2Given) {
      BSIM4pdvtp2 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp3Given) {
      BSIM4dvtp3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp3Given) {
      BSIM4wdvtp3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp3Given) {
      BSIM4ldvtp3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp3Given) {
      BSIM4pdvtp3 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp4Given) {
      BSIM4dvtp4 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp4Given) {
      BSIM4wdvtp4 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp4Given) {
      BSIM4ldvtp4 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp4Given) {
      BSIM4pdvtp4 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4dvtp5Given) {
      BSIM4dvtp5 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wdvtp5Given) {
      BSIM4wdvtp5 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ldvtp5Given) {
      BSIM4ldvtp5 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pdvtp5Given) {
      BSIM4pdvtp5 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ni0subGiven) {
      BSIM4ni0sub = ((1.45e10)*UNITFAD).val();
    }

    if(!BSIM4bg0subGiven) {
      BSIM4bg0sub = ((1.16)*UNITFAD).val();
    }

    if(!BSIM4tbgasubGiven) {
      BSIM4tbgasub = ((7.02e-4)*UNITFAD).val();
    }

    if(!BSIM4tbgbsubGiven) {
      BSIM4tbgbsub = ((1108.0)*UNITFAD).val();
    }

    if(!BSIM4tempeotGiven) {
      BSIM4tempeot = ((300.15)*UNITFAD).val();
    }

    if(!BSIM4leffeotGiven) {
      BSIM4leffeot = ((1.0)*UNITFAD).val();
    }

    if(!BSIM4weffeotGiven) {
      BSIM4weffeot = ((10.0)*UNITFAD).val();
    }

    if(!BSIM4vddeotGiven) {
      BSIM4vddeot = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.5,-1.5))*UNITFAD).val();
    }

    if(!BSIM4eotGiven) {
      BSIM4eot = ((15.0e-10)*UNITFAD).val();
    }

    if(!BSIM4epsrgateGiven) {
      BSIM4epsrgate = ((11.7)*UNITFAD).val();
    }

    if(!BSIM4epsrsubGiven) {
      BSIM4epsrsub = ((11.7)*UNITFAD).val();
    }

    if(!BSIM4phigGiven) {
      BSIM4phig = ((4.05)*UNITFAD).val();
    }

    if(!BSIM4easubGiven) {
      BSIM4easub = ((4.05)*UNITFAD).val();
    }

    if(!BSIM4mtrlmodGiven) {
      BSIM4mtrlmod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4mtrlmod != 0) && (BSIM4mtrlmod != 1))) {
      {
        BSIM4mtrlmod = ((0)*UNITFAD).val();
        printf("Warning: mtrlMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4mtrlcompatmodGiven) {
      BSIM4mtrlcompatmod = ((0)*UNITFAD).val();
    }
      else if(((BSIM4mtrlcompatmod != 0) && (BSIM4mtrlcompatmod != 1))) {
      {
        BSIM4mtrlcompatmod = ((0)*UNITFAD).val();
        printf("Warning: mtrlCompatMod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4udGiven) {
      BSIM4ud = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wudGiven) {
      BSIM4wud = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ludGiven) {
      BSIM4lud = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pudGiven) {
      BSIM4pud = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ud1Given) {
      BSIM4ud1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wud1Given) {
      BSIM4wud1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lud1Given) {
      BSIM4lud1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pud1Given) {
      BSIM4pud1 = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4upGiven) {
      BSIM4up = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wupGiven) {
      BSIM4wup = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lupGiven) {
      BSIM4lup = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pupGiven) {
      BSIM4pup = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lpGiven) {
      BSIM4lp = ((1.0e-8)*UNITFAD).val();
    }

    if(!BSIM4wlpGiven) {
      BSIM4wlp = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4llpGiven) {
      BSIM4llp = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4plpGiven) {
      BSIM4plp = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ucsGiven) {
      BSIM4ucs = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),1.67,1.0))*UNITFAD).val();
    }

    if(!BSIM4wucsGiven) {
      BSIM4wucs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lucsGiven) {
      BSIM4lucs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pucsGiven) {
      BSIM4pucs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ucsteGiven) {
      BSIM4ucste = ((-4.775e-3)*UNITFAD).val();
    }

    if(!BSIM4wucsteGiven) {
      BSIM4wucste = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lucsteGiven) {
      BSIM4lucste = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pucsteGiven) {
      BSIM4pucste = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4kvth0weGiven) {
      BSIM4kvth0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wkvth0weGiven) {
      BSIM4wkvth0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lkvth0weGiven) {
      BSIM4lkvth0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pkvth0weGiven) {
      BSIM4pkvth0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4k2weGiven) {
      BSIM4k2we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wk2weGiven) {
      BSIM4wk2we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lk2weGiven) {
      BSIM4lk2we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pk2weGiven) {
      BSIM4pk2we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ku0weGiven) {
      BSIM4ku0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wku0weGiven) {
      BSIM4wku0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lku0weGiven) {
      BSIM4lku0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pku0weGiven) {
      BSIM4pku0we = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4vfbsdoffGiven) {
      BSIM4vfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wvfbsdoffGiven) {
      BSIM4wvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4lvfbsdoffGiven) {
      BSIM4lvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4pvfbsdoffGiven) {
      BSIM4pvfbsdoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tvoffGiven) {
      BSIM4tvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wtvoffGiven) {
      BSIM4wtvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ltvoffGiven) {
      BSIM4ltvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4ptvoffGiven) {
      BSIM4ptvoff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wpemodGiven) {
      BSIM4wpemod = ((0.0)*UNITFAD).val();
    }
      else if(((BSIM4wpemod != 0) && (BSIM4wpemod != 1))) {
      {
        BSIM4wpemod = ((0)*UNITFAD).val();
        printf("Warning: wpemod has been set to its default value: 0.\n");
      }

    }

    if(!BSIM4scaGiven) {
      BSIM4sca = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4scbGiven) {
      BSIM4scb = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4sccGiven) {
      BSIM4scc = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4webGiven) {
      BSIM4web = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4wecGiven) {
      BSIM4wec = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4screfGiven) {
      BSIM4scref = ((1.0E-6)*UNITFAD).val();
    }

    if(!BSIM4rbps0Given) {
      BSIM4rbps0 = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbpslGiven) {
      BSIM4rbpsl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpswGiven) {
      BSIM4rbpsw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpsnfGiven) {
      BSIM4rbpsnf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpd0Given) {
      BSIM4rbpd0 = ((50.0)*UNITFAD).val();
    }

    if(!BSIM4rbpdwGiven) {
      BSIM4rbpdw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbx0Given) {
      BSIM4rbpbx0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbxlGiven) {
      BSIM4rbpbxl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbxwGiven) {
      BSIM4rbpbxw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbxnfGiven) {
      BSIM4rbpbxnf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpby0Given) {
      BSIM4rbpby0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbylGiven) {
      BSIM4rbpbyl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbywGiven) {
      BSIM4rbpbyw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbpbynfGiven) {
      BSIM4rbpbynf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsbx0Given) {
      BSIM4rbsbx0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbsby0Given) {
      BSIM4rbsby0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbdbx0Given) {
      BSIM4rbdbx0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbdby0Given) {
      BSIM4rbdby0 = ((100.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbxlGiven) {
      BSIM4rbsdbxl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbxwGiven) {
      BSIM4rbsdbxw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbxnfGiven) {
      BSIM4rbsdbxnf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbylGiven) {
      BSIM4rbsdbyl = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbywGiven) {
      BSIM4rbsdbyw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4rbsdbynfGiven) {
      BSIM4rbsdbynf = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4jtssGiven) {
      BSIM4jtss = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4jtsswsGiven) {
      BSIM4jtssws = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4jtsswgsGiven) {
      BSIM4jtsswgs = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4jtweffGiven) {
      BSIM4jtweff = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4njtsGiven) {
      BSIM4njts = ((20.0)*UNITFAD).val();
    }

    if(!BSIM4njtsswGiven) {
      BSIM4njtssw = ((20.0)*UNITFAD).val();
    }

    if(!BSIM4njtsswgGiven) {
      BSIM4njtsswg = ((20.0)*UNITFAD).val();
    }

    if(!BSIM4xtssGiven) {
      BSIM4xtss = ((0.02)*UNITFAD).val();
    }

    if(!BSIM4xtsswsGiven) {
      BSIM4xtssws = ((0.02)*UNITFAD).val();
    }

    if(!BSIM4xtsswgsGiven) {
      BSIM4xtsswgs = ((0.02)*UNITFAD).val();
    }

    if(!BSIM4vtssGiven) {
      BSIM4vtss = ((10.0)*UNITFAD).val();
    }

    if(!BSIM4vtsswsGiven) {
      BSIM4vtssws = ((10.0)*UNITFAD).val();
    }

    if(!BSIM4vtsswgsGiven) {
      BSIM4vtsswgs = ((10.0)*UNITFAD).val();
    }

    if(!BSIM4tnjtsGiven) {
      BSIM4tnjts = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tnjtsswGiven) {
      BSIM4tnjtssw = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4tnjtsswgGiven) {
      BSIM4tnjtsswg = ((0.0)*UNITFAD).val();
    }

    if(!BSIM4jtsdGiven) {
      BSIM4jtsd = ((BSIM4jtss)*UNITFAD).val();
    }

    if(!BSIM4jtsswdGiven) {
      BSIM4jtsswd = ((BSIM4jtssws)*UNITFAD).val();
    }

    if(!BSIM4jtsswgdGiven) {
      BSIM4jtsswgd = ((BSIM4jtsswgs)*UNITFAD).val();
    }

    if(!BSIM4njtsdGiven) {
      {
        if(BSIM4njtsGiven) {
          BSIM4njtsd = ((BSIM4njts)*UNITFAD).val();
        }
        else {
          BSIM4njtsd = ((20.0)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4njtsswdGiven) {
      {
        if(BSIM4njtsswGiven) {
          BSIM4njtsswd = ((BSIM4njtssw)*UNITFAD).val();
        }
        else {
          BSIM4njtsswd = ((20.0)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4njtsswgdGiven) {
      {
        if(BSIM4njtsswgGiven) {
          BSIM4njtsswgd = ((BSIM4njtsswg)*UNITFAD).val();
        }
        else {
          BSIM4njtsswgd = ((20.0)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4xtsdGiven) {
      BSIM4xtsd = ((BSIM4xtss)*UNITFAD).val();
    }

    if(!BSIM4xtsswdGiven) {
      BSIM4xtsswd = ((BSIM4xtssws)*UNITFAD).val();
    }

    if(!BSIM4xtsswgdGiven) {
      BSIM4xtsswgd = ((BSIM4xtsswgs)*UNITFAD).val();
    }

    if(!BSIM4vtsdGiven) {
      BSIM4vtsd = ((BSIM4vtss)*UNITFAD).val();
    }

    if(!BSIM4vtsswdGiven) {
      BSIM4vtsswd = ((BSIM4vtssws)*UNITFAD).val();
    }

    if(!BSIM4vtsswgdGiven) {
      BSIM4vtsswgd = ((BSIM4vtsswgs)*UNITFAD).val();
    }

    if(!BSIM4tnjtsdGiven) {
      {
        if(BSIM4tnjtsGiven) {
          BSIM4tnjtsd = ((BSIM4tnjts)*UNITFAD).val();
        }
        else {
          BSIM4tnjtsd = ((0.0)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4tnjtsswdGiven) {
      {
        if(BSIM4tnjtsswGiven) {
          BSIM4tnjtsswd = ((BSIM4tnjtssw)*UNITFAD).val();
        }
        else {
          BSIM4tnjtsswd = ((0.0)*UNITFAD).val();
        }

      }

    }

    if(!BSIM4tnjtsswgdGiven) {
      {
        if(BSIM4tnjtsswgGiven) {
          BSIM4tnjtsswgd = ((BSIM4tnjtsswg)*UNITFAD).val();
        }
        else {
          BSIM4tnjtsswgd = ((0.0)*UNITFAD).val();
        }

      }

    }

    if((BSIM4SbulkJctPotential < 0.1)) {
      {
        BSIM4SbulkJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbs is less than 0.1. Pbs is set to 0.1.\n");
      }

    }

    if((BSIM4SsidewallJctPotential < 0.1)) {
      {
        BSIM4SsidewallJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbsws is less than 0.1. Pbsws is set to 0.1.\n");
      }

    }

    if((BSIM4SGatesidewallJctPotential < 0.1)) {
      {
        BSIM4SGatesidewallJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbswgs is less than 0.1. Pbswgs is set to 0.1.\n");
      }

    }

    if((BSIM4DbulkJctPotential < 0.1)) {
      {
        BSIM4DbulkJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbd is less than 0.1. Pbd is set to 0.1.\n");
      }

    }

    if((BSIM4DsidewallJctPotential < 0.1)) {
      {
        BSIM4DsidewallJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbswd is less than 0.1. Pbswd is set to 0.1.\n");
      }

    }

    if((BSIM4DGatesidewallJctPotential < 0.1)) {
      {
        BSIM4DGatesidewallJctPotential = ((0.1)*UNITFAD).val();
        printf("Given pbswgd is less than 0.1. Pbswgd is set to 0.1.\n");
      }

    }

    if((BSIM4mtrlmod == 0)) {
      {
        if((((BSIM4toxeGiven && BSIM4toxpGiven) && BSIM4dtoxGiven) && (BSIM4toxe != (BSIM4toxp+BSIM4dtox)))) {
          {
            if((BSIM4toxe == 0.0)) {
              printf("Warning: toxe, toxp and dtox all given and toxe != toxp + dtox; dtox ignored.");
            }
                      else if((fabs(((BSIM4toxe-(BSIM4toxp+BSIM4dtox))/BSIM4toxe)) >= 1e-15)) {
              printf("Warning: toxe, toxp and dtox all given and toxe != toxp + dtox; dtox ignored.");
            }

          }

        }
              else if((BSIM4toxeGiven && !BSIM4toxpGiven)) {
          BSIM4toxp = (((BSIM4toxe-BSIM4dtox))*UNITFAD).val();
        }
              else if((!BSIM4toxeGiven && BSIM4toxpGiven)) {
          {
            BSIM4toxe = (((BSIM4toxp+BSIM4dtox))*UNITFAD).val();
            if(!BSIM4toxmGiven) {
              BSIM4toxm = ((BSIM4toxe)*UNITFAD).val();
            }

          }

        }

      }

    }
      else if((BSIM4mtrlcompatmod != 0)) {
      {
        T0 = (((BSIM4epsrox/3.9))*UNITFAD).val();
        if((((BSIM4eotGiven && BSIM4toxpGiven) && BSIM4dtoxGiven) && (fabs(((BSIM4eot*T0)-(BSIM4toxp+BSIM4dtox))) > 1.0e-20))) {
          printf("Warning: eot, toxp and dtox all given and eot * EPSROX / 3.9 != toxp + dtox; dtox ignored.\n");
        }
              else if((BSIM4eotGiven && !BSIM4toxpGiven)) {
          BSIM4toxp = ((((T0*BSIM4eot)-BSIM4dtox))*UNITFAD).val();
        }
              else if((!BSIM4eotGiven && BSIM4toxpGiven)) {
          {
            BSIM4eot = ((((BSIM4toxp+BSIM4dtox)/T0))*UNITFAD).val();
            if(!BSIM4toxmGiven) {
              BSIM4toxm = ((BSIM4eot)*UNITFAD).val();
            }

          }

        }

      }

    }

    if(BSIM4mtrlmod) {
      {
        epsroxl = ((3.9)*UNITFAD).val();
        toxel = ((BSIM4eot)*UNITFAD).val();
        epssubl = (((8.85418e-12*BSIM4epsrsub))*UNITFAD).val();
      }

    }
    else {
      {
        epsroxl = ((BSIM4epsrox)*UNITFAD).val();
        toxel = ((BSIM4toxe)*UNITFAD).val();
        epssubl = ((1.03594e-10)*UNITFAD).val();
      }

    }

    BSIM4coxe = ((((epsroxl*8.85418e-12)/toxel))*UNITFAD).val();
    if(((BSIM4mtrlmod == 0) || (BSIM4mtrlcompatmod != 0))) {
      BSIM4coxp = ((((BSIM4epsrox*8.85418e-12)/BSIM4toxp))*UNITFAD).val();
    }

    if(!BSIM4cgdoGiven) {
      {
        if((BSIM4dlcGiven && (BSIM4dlc > 0.0))) {
          BSIM4cgdo = ((((BSIM4dlc*BSIM4coxe)-BSIM4cgdl))*UNITFAD).val();
        }
        else {
          BSIM4cgdo = ((((0.6*BSIM4xj)*BSIM4coxe))*UNITFAD).val();
        }

      }

    }

    if(!BSIM4cgsoGiven) {
      {
        if((BSIM4dlcGiven && (BSIM4dlc > 0.0))) {
          BSIM4cgso = ((((BSIM4dlc*BSIM4coxe)-BSIM4cgsl))*UNITFAD).val();
        }
        else {
          BSIM4cgso = ((((0.6*BSIM4xj)*BSIM4coxe))*UNITFAD).val();
        }

      }

    }

    if(!BSIM4cgboGiven) {
      BSIM4cgbo = ((((2.0*BSIM4dwc)*BSIM4coxe))*UNITFAD).val();
    }

    Tnom = ((BSIM4tnom)*UNITFAD).val();
    TRatio = (((T/Tnom))*UNITFAD).val();
    BSIM4factor1 = ((sqrt(((epssubl/(epsroxl*8.85418e-12))*toxel)))*UNITFAD).val();
    Vtm0 = ((((1.3806503e-23/1.602176462e-19)*Tnom))*UNITFAD).val();
    if((BSIM4mtrlmod == 0)) {
      {
        Eg0 = (((1.16-(((7.02e-4*Tnom)*Tnom)/(Tnom+1108.0))))*UNITFAD).val();
        ni = (((((1.45e10*(Tnom/300.15))*sqrt((Tnom/300.15)))*exp((21.5565981-(Eg0/(2.0*Vtm0))))))*UNITFAD).val();
      }

    }
    else {
      {
        Eg0 = (((BSIM4bg0sub-(((BSIM4tbgasub*Tnom)*Tnom)/(Tnom+BSIM4tbgbsub))))*UNITFAD).val();
        T0 = (((BSIM4bg0sub-((BSIM4tbgasub*90090.0225)/(300.15+BSIM4tbgbsub))))*UNITFAD).val();
        ni = (((((BSIM4ni0sub*(Tnom/300.15))*sqrt((Tnom/300.15)))*exp(((T0-Eg0)/(2.0*Vtm0)))))*UNITFAD).val();
      }

    }

    BSIM4Eg0 = ((Eg0)*UNITFAD).val();
    BSIM4vtm = ((((1.3806503e-23/1.602176462e-19)*T))*UNITFAD).val();
    if((BSIM4mtrlmod == 0)) {
      Eg = (((1.16-(((7.02e-4*T)*T)/(T+1108.0))))*UNITFAD).val();
    }
    else {
      Eg = (((BSIM4bg0sub-(((BSIM4tbgasub*T)*T)/(T+BSIM4tbgbsub))))*UNITFAD).val();
    }

    if((T != Tnom)) {
      {
        T0 = ((((Eg0/Vtm0)-(Eg/BSIM4vtm)))*UNITFAD).val();
        T1 = ((log((T/Tnom)))*UNITFAD).val();
        T2 = (((T0+(BSIM4SjctTempExponent*T1)))*UNITFAD).val();
        T3 = ((exp((T2/BSIM4SjctEmissionCoeff)))*UNITFAD).val();
        BSIM4SjctTempSatCurDensity = (((BSIM4SjctSatCurDensity*T3))*UNITFAD).val();
        BSIM4SjctSidewallTempSatCurDensity = (((BSIM4SjctSidewallSatCurDensity*T3))*UNITFAD).val();
        BSIM4SjctGateSidewallTempSatCurDensity = (((BSIM4SjctGateSidewallSatCurDensity*T3))*UNITFAD).val();
        T2 = (((T0+(BSIM4DjctTempExponent*T1)))*UNITFAD).val();
        T3 = ((exp((T2/BSIM4DjctEmissionCoeff)))*UNITFAD).val();
        BSIM4DjctTempSatCurDensity = (((BSIM4DjctSatCurDensity*T3))*UNITFAD).val();
        BSIM4DjctSidewallTempSatCurDensity = (((BSIM4DjctSidewallSatCurDensity*T3))*UNITFAD).val();
        BSIM4DjctGateSidewallTempSatCurDensity = (((BSIM4DjctGateSidewallSatCurDensity*T3))*UNITFAD).val();
      }

    }
    else {
      {
        BSIM4SjctTempSatCurDensity = ((BSIM4SjctSatCurDensity)*UNITFAD).val();
        BSIM4SjctSidewallTempSatCurDensity = ((BSIM4SjctSidewallSatCurDensity)*UNITFAD).val();
        BSIM4SjctGateSidewallTempSatCurDensity = ((BSIM4SjctGateSidewallSatCurDensity)*UNITFAD).val();
        BSIM4DjctTempSatCurDensity = ((BSIM4DjctSatCurDensity)*UNITFAD).val();
        BSIM4DjctSidewallTempSatCurDensity = ((BSIM4DjctSidewallSatCurDensity)*UNITFAD).val();
        BSIM4DjctGateSidewallTempSatCurDensity = ((BSIM4DjctGateSidewallSatCurDensity)*UNITFAD).val();
      }

    }

    if((BSIM4SjctTempSatCurDensity < 0.0)) {
      BSIM4SjctTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if((BSIM4SjctSidewallTempSatCurDensity < 0.0)) {
      BSIM4SjctSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if((BSIM4SjctGateSidewallTempSatCurDensity < 0.0)) {
      BSIM4SjctGateSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if((BSIM4DjctTempSatCurDensity < 0.0)) {
      BSIM4DjctTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if((BSIM4DjctSidewallTempSatCurDensity < 0.0)) {
      BSIM4DjctSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    if((BSIM4DjctGateSidewallTempSatCurDensity < 0.0)) {
      BSIM4DjctGateSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
    }

    delTemp = (((T-Tnom))*UNITFAD).val();
    T0 = (((BSIM4tcj*delTemp))*UNITFAD).val();
    if((T0 >= -1.0)) {
      {
        BSIM4SunitAreaTempJctCap = (((BSIM4SunitAreaJctCap*(1.0+T0)))*UNITFAD).val();
        BSIM4DunitAreaTempJctCap = (((BSIM4DunitAreaJctCap*(1.0+T0)))*UNITFAD).val();
      }

    }
    else {
      {
        if((BSIM4SunitAreaJctCap > 0.0)) {
          {
            BSIM4SunitAreaTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjs to be negative. \
                             Cjs is clamped to zero.\n");
          }

        }

        if((BSIM4DunitAreaJctCap > 0.0)) {
          {
            BSIM4DunitAreaTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjd to be negative. \
                             Cjd is clamped to zero.\n");
          }

        }

      }

    }

    T0 = (((BSIM4tcjsw*delTemp))*UNITFAD).val();
    if((T0 >= -1.0)) {
      {
        BSIM4SunitLengthSidewallTempJctCap = (((BSIM4SunitLengthSidewallJctCap*(1.0+T0)))*UNITFAD).val();
        BSIM4DunitLengthSidewallTempJctCap = (((BSIM4DunitLengthSidewallJctCap*(1.0+T0)))*UNITFAD).val();
      }

    }
    else {
      {
        if((BSIM4SunitLengthSidewallJctCap > 0.0)) {
          {
            BSIM4SunitLengthSidewallTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjsws to be negative. \
                             Cjsws is clamped to zero.\n");
          }

        }

        if((BSIM4DunitLengthSidewallJctCap > 0.0)) {
          {
            BSIM4DunitLengthSidewallTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjswd to be negative. \
                             Cjswd is clamped to zero.\n");
          }

        }

      }

    }

    T0 = (((BSIM4tcjswg*delTemp))*UNITFAD).val();
    if((T0 >= -1.0)) {
      {
        BSIM4SunitLengthGateSidewallTempJctCap = (((BSIM4SunitLengthGateSidewallJctCap*(1.0+T0)))*UNITFAD).val();
        BSIM4DunitLengthGateSidewallTempJctCap = (((BSIM4DunitLengthGateSidewallJctCap*(1.0+T0)))*UNITFAD).val();
      }

    }
    else {
      {
        if((BSIM4SunitLengthGateSidewallJctCap > 0.0)) {
          {
            BSIM4SunitLengthGateSidewallTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjswgs to be negative. \
                             Cjswgs is clamped to zero.\n");
          }

        }

        if((BSIM4DunitLengthGateSidewallJctCap > 0.0)) {
          {
            BSIM4DunitLengthGateSidewallTempJctCap = ((0.0)*UNITFAD).val();
            printf("Temperature effect has caused cjswgd to be negative. \
                             Cjswgd is clamped to zero.\n");
          }

        }

      }

    }

    BSIM4PhiBS = (((BSIM4SbulkJctPotential-(BSIM4tpb*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBS < 0.01)) {
      {
        BSIM4PhiBS = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbs to be less than 0.01. \
                         Pbs is clamped to 0.01.\n");
      }

    }

    BSIM4PhiBD = (((BSIM4DbulkJctPotential-(BSIM4tpb*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBD < 0.01)) {
      {
        BSIM4PhiBD = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbd to be less than 0.01. \
                         Pbd is clamped to 0.01.\n");
      }

    }

    BSIM4PhiBSWS = (((BSIM4SsidewallJctPotential-(BSIM4tpbsw*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBSWS <= 0.01)) {
      {
        BSIM4PhiBSWS = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbsws to be less than 0.01. \
                         Pbsws is clamped to 0.01.\n");
      }

    }

    BSIM4PhiBSWD = (((BSIM4DsidewallJctPotential-(BSIM4tpbsw*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBSWD <= 0.01)) {
      {
        BSIM4PhiBSWD = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbswd to be less than 0.01. \
                         Pbswd is clamped to 0.01.\n");
      }

    }

    BSIM4PhiBSWGS = (((BSIM4SGatesidewallJctPotential-(BSIM4tpbswg*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBSWGS <= 0.01)) {
      {
        BSIM4PhiBSWGS = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbswgs to be less than 0.01. \
                         Pbswgs is clamped to 0.01.\n");
      }

    }

    BSIM4PhiBSWGD = (((BSIM4DGatesidewallJctPotential-(BSIM4tpbswg*delTemp)))*UNITFAD).val();
    if((BSIM4PhiBSWGD <= 0.01)) {
      {
        BSIM4PhiBSWGD = ((0.01)*UNITFAD).val();
        printf("Temperature effect has caused pbswgd to be less than 0.01. \
                         Pbswgd is clamped to 0.01.\n");
      }

    }

    if((BSIM4ijthdfwd <= 0.0)) {
      {
        BSIM4ijthdfwd = ((0.0)*UNITFAD).val();
        printf("Ijthdfwd reset to %g.\n",((BSIM4ijthdfwd)*UNITFAD).val());
      }

    }

    if((BSIM4ijthsfwd <= 0.0)) {
      {
        BSIM4ijthsfwd = ((0.0)*UNITFAD).val();
        printf("Ijthsfwd reset to %g.\n",((BSIM4ijthsfwd)*UNITFAD).val());
      }

    }

    if((BSIM4ijthdrev <= 0.0)) {
      {
        BSIM4ijthdrev = ((0.0)*UNITFAD).val();
        printf("Ijthdrev reset to %g.\n",((BSIM4ijthdrev)*UNITFAD).val());
      }

    }

    if((BSIM4ijthsrev <= 0.0)) {
      {
        BSIM4ijthsrev = ((0.0)*UNITFAD).val();
        printf("Ijthsrev reset to %g.\n",((BSIM4ijthsrev)*UNITFAD).val());
      }

    }

    if(((BSIM4xjbvd <= 0.0) && (BSIM4dioMod == 2))) {
      {
        BSIM4xjbvd = ((0.0)*UNITFAD).val();
        printf("Xjbvd reset to %g.\n",((BSIM4xjbvd)*UNITFAD).val());
      }

    }
      else if(((BSIM4xjbvd < 0.0) && (BSIM4dioMod == 0))) {
      {
        BSIM4xjbvd = ((0.0)*UNITFAD).val();
        printf("Xjbvd reset to %g.\n",((BSIM4xjbvd)*UNITFAD).val());
      }

    }

    if((BSIM4bvd <= 0.0)) {
      {
        BSIM4bvd = ((0.0)*UNITFAD).val();
        printf("BVD reset to %g.\n",((BSIM4bvd)*UNITFAD).val());
      }

    }

    if(((BSIM4xjbvs <= 0.0) && (BSIM4dioMod == 2))) {
      {
        BSIM4xjbvs = ((0.0)*UNITFAD).val();
        printf("Xjbvs reset to %g.\n",((BSIM4xjbvs)*UNITFAD).val());
      }

    }
      else if(((BSIM4xjbvs < 0.0) && (BSIM4dioMod == 0))) {
      {
        BSIM4xjbvs = ((0.0)*UNITFAD).val();
        printf("Xjbvs reset to %g.\n",((BSIM4xjbvs)*UNITFAD).val());
      }

    }

    if((BSIM4bvs <= 0.0)) {
      {
        BSIM4bvs = ((0.0)*UNITFAD).val();
        printf("BVS reset to %g.\n",((BSIM4bvs)*UNITFAD).val());
      }

    }

    Ldrn = ((BSIM4l)*UNITFAD).val();
    Wdrn = (((BSIM4w/BSIM4nf))*UNITFAD).val();
    Lnew = (((BSIM4l+BSIM4xl))*UNITFAD).val();
    Wnew = ((((BSIM4w/BSIM4nf)+BSIM4xw))*UNITFAD).val();
    T0 = ((pow(Lnew,BSIM4Lln))*UNITFAD).val();
    T1 = ((pow(Wnew,BSIM4Lwn))*UNITFAD).val();
    tmp1 = (((((BSIM4Ll/T0)+(BSIM4Lw/T1))+(BSIM4Lwl/(T0*T1))))*UNITFAD).val();
    BSIM4dl = (((BSIM4Lint+tmp1))*UNITFAD).val();
    tmp2 = (((((BSIM4Llc/T0)+(BSIM4Lwc/T1))+(BSIM4Lwlc/(T0*T1))))*UNITFAD).val();
    BSIM4dlc = (((BSIM4dlc+tmp2))*UNITFAD).val();
    T2 = ((pow(Lnew,BSIM4Wln))*UNITFAD).val();
    T3 = ((pow(Wnew,BSIM4Wwn))*UNITFAD).val();
    tmp1 = (((((BSIM4Wl/T2)+(BSIM4Ww/T3))+(BSIM4Wwl/(T2*T3))))*UNITFAD).val();
    BSIM4dw = (((BSIM4Wint+tmp1))*UNITFAD).val();
    tmp2 = (((((BSIM4Wlc/T2)+(BSIM4Wwc/T3))+(BSIM4Wwlc/(T2*T3))))*UNITFAD).val();
    BSIM4dwc = (((BSIM4dwc+tmp2))*UNITFAD).val();
    BSIM4dwj = (((BSIM4dwj+tmp2))*UNITFAD).val();
    BSIM4leff = (((Lnew-(2.0*BSIM4dl)))*UNITFAD).val();
    if((BSIM4leff <= 0.0)) {
      printf("\nFATAL Error: leff <= 0.");
    }

    BSIM4weff = (((Wnew-(2.0*BSIM4dw)))*UNITFAD).val();
    if((BSIM4weff <= 0.0)) {
      printf("\nFATAL Error: weff <= 0.");
    }

    BSIM4leffCV = (((Lnew-(2.0*BSIM4dlc)))*UNITFAD).val();
    if((BSIM4leffCV <= 0.0)) {
      printf("\nFATAL Error: leffCV <= 0.");
    }

    BSIM4weffCV = (((Wnew-(2.0*BSIM4dwc)))*UNITFAD).val();
    if((BSIM4weffCV <= 0.0)) {
      printf("\nFATAL Error: weffCV <= 0.");
    }

    BSIM4weffCJ = (((Wnew-(2.0*BSIM4dwj)))*UNITFAD).val();
    if((BSIM4binUnit == 1)) {
      {
        Inv_L = (((1.0e-6/BSIM4leff))*UNITFAD).val();
        Inv_W = (((1.0e-6/BSIM4weff))*UNITFAD).val();
        Inv_LW = (((1.0e-12/(BSIM4leff*BSIM4weff)))*UNITFAD).val();
      }

    }
    else {
      {
        Inv_L = (((1.0/BSIM4leff))*UNITFAD).val();
        Inv_W = (((1.0/BSIM4weff))*UNITFAD).val();
        Inv_LW = (((1.0/(BSIM4leff*BSIM4weff)))*UNITFAD).val();
      }

    }

    BSIM4cdsc = (((((BSIM4cdsc+(BSIM4lcdsc*Inv_L))+(BSIM4wcdsc*Inv_W))+(BSIM4pcdsc*Inv_LW)))*UNITFAD).val();
    BSIM4cdscb = (((((BSIM4cdscb+(BSIM4lcdscb*Inv_L))+(BSIM4wcdscb*Inv_W))+(BSIM4pcdscb*Inv_LW)))*UNITFAD).val();
    BSIM4cdscd = (((((BSIM4cdscd+(BSIM4lcdscd*Inv_L))+(BSIM4wcdscd*Inv_W))+(BSIM4pcdscd*Inv_LW)))*UNITFAD).val();
    BSIM4cit = (((((BSIM4cit+(BSIM4lcit*Inv_L))+(BSIM4wcit*Inv_W))+(BSIM4pcit*Inv_LW)))*UNITFAD).val();
    BSIM4nfactor = (((((BSIM4nfactor+(BSIM4lnfactor*Inv_L))+(BSIM4wnfactor*Inv_W))+(BSIM4pnfactor*Inv_LW)))*UNITFAD).val();
    BSIM4xj = (((((BSIM4xj+(BSIM4lxj*Inv_L))+(BSIM4wxj*Inv_W))+(BSIM4pxj*Inv_LW)))*UNITFAD).val();
    BSIM4vsat = (((((BSIM4vsat+(BSIM4lvsat*Inv_L))+(BSIM4wvsat*Inv_W))+(BSIM4pvsat*Inv_LW)))*UNITFAD).val();
    BSIM4at = (((((BSIM4at+(BSIM4lat*Inv_L))+(BSIM4wat*Inv_W))+(BSIM4pat*Inv_LW)))*UNITFAD).val();
    BSIM4a0 = (((((BSIM4a0+(BSIM4la0*Inv_L))+(BSIM4wa0*Inv_W))+(BSIM4pa0*Inv_LW)))*UNITFAD).val();
    BSIM4ags = (((((BSIM4ags+(BSIM4lags*Inv_L))+(BSIM4wags*Inv_W))+(BSIM4pags*Inv_LW)))*UNITFAD).val();
    BSIM4a1 = (((((BSIM4a1+(BSIM4la1*Inv_L))+(BSIM4wa1*Inv_W))+(BSIM4pa1*Inv_LW)))*UNITFAD).val();
    BSIM4a2 = (((((BSIM4a2+(BSIM4la2*Inv_L))+(BSIM4wa2*Inv_W))+(BSIM4pa2*Inv_LW)))*UNITFAD).val();
    BSIM4keta = (((((BSIM4keta+(BSIM4lketa*Inv_L))+(BSIM4wketa*Inv_W))+(BSIM4pketa*Inv_LW)))*UNITFAD).val();
    BSIM4nsub = (((((BSIM4nsub+(BSIM4lnsub*Inv_L))+(BSIM4wnsub*Inv_W))+(BSIM4pnsub*Inv_LW)))*UNITFAD).val();
    BSIM4ndep = (((((BSIM4ndep+(BSIM4lndep*Inv_L))+(BSIM4wndep*Inv_W))+(BSIM4pndep*Inv_LW)))*UNITFAD).val();
    BSIM4nsd = (((((BSIM4nsd+(BSIM4lnsd*Inv_L))+(BSIM4wnsd*Inv_W))+(BSIM4pnsd*Inv_LW)))*UNITFAD).val();
    BSIM4phin = (((((BSIM4phin+(BSIM4lphin*Inv_L))+(BSIM4wphin*Inv_W))+(BSIM4pphin*Inv_LW)))*UNITFAD).val();
    BSIM4ngate = (((((BSIM4ngate+(BSIM4lngate*Inv_L))+(BSIM4wngate*Inv_W))+(BSIM4pngate*Inv_LW)))*UNITFAD).val();
    BSIM4gamma1 = (((((BSIM4gamma1+(BSIM4lgamma1*Inv_L))+(BSIM4wgamma1*Inv_W))+(BSIM4pgamma1*Inv_LW)))*UNITFAD).val();
    BSIM4gamma2 = (((((BSIM4gamma2+(BSIM4lgamma2*Inv_L))+(BSIM4wgamma2*Inv_W))+(BSIM4pgamma2*Inv_LW)))*UNITFAD).val();
    BSIM4vbx = (((((BSIM4vbx+(BSIM4lvbx*Inv_L))+(BSIM4wvbx*Inv_W))+(BSIM4pvbx*Inv_LW)))*UNITFAD).val();
    BSIM4vbm = (((((BSIM4vbm+(BSIM4lvbm*Inv_L))+(BSIM4wvbm*Inv_W))+(BSIM4pvbm*Inv_LW)))*UNITFAD).val();
    BSIM4xt = (((((BSIM4xt+(BSIM4lxt*Inv_L))+(BSIM4wxt*Inv_W))+(BSIM4pxt*Inv_LW)))*UNITFAD).val();
    BSIM4vfb = (((((BSIM4vfb+(BSIM4lvfb*Inv_L))+(BSIM4wvfb*Inv_W))+(BSIM4pvfb*Inv_LW)))*UNITFAD).val();
    BSIM4k1 = (((((BSIM4k1+(BSIM4lk1*Inv_L))+(BSIM4wk1*Inv_W))+(BSIM4pk1*Inv_LW)))*UNITFAD).val();
    BSIM4kt1 = (((((BSIM4kt1+(BSIM4lkt1*Inv_L))+(BSIM4wkt1*Inv_W))+(BSIM4pkt1*Inv_LW)))*UNITFAD).val();
    BSIM4kt1l = (((((BSIM4kt1l+(BSIM4lkt1l*Inv_L))+(BSIM4wkt1l*Inv_W))+(BSIM4pkt1l*Inv_LW)))*UNITFAD).val();
    BSIM4k2 = (((((BSIM4k2+(BSIM4lk2*Inv_L))+(BSIM4wk2*Inv_W))+(BSIM4pk2*Inv_LW)))*UNITFAD).val();
    BSIM4kt2 = (((((BSIM4kt2+(BSIM4lkt2*Inv_L))+(BSIM4wkt2*Inv_W))+(BSIM4pkt2*Inv_LW)))*UNITFAD).val();
    BSIM4k3 = (((((BSIM4k3+(BSIM4lk3*Inv_L))+(BSIM4wk3*Inv_W))+(BSIM4pk3*Inv_LW)))*UNITFAD).val();
    BSIM4k3b = (((((BSIM4k3b+(BSIM4lk3b*Inv_L))+(BSIM4wk3b*Inv_W))+(BSIM4pk3b*Inv_LW)))*UNITFAD).val();
    BSIM4w0 = (((((BSIM4w0+(BSIM4lw0*Inv_L))+(BSIM4ww0*Inv_W))+(BSIM4pw0*Inv_LW)))*UNITFAD).val();
    BSIM4lpe0 = (((((BSIM4lpe0+(BSIM4llpe0*Inv_L))+(BSIM4wlpe0*Inv_W))+(BSIM4plpe0*Inv_LW)))*UNITFAD).val();
    BSIM4lpeb = (((((BSIM4lpeb+(BSIM4llpeb*Inv_L))+(BSIM4wlpeb*Inv_W))+(BSIM4plpeb*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp0 = (((((BSIM4dvtp0+(BSIM4ldvtp0*Inv_L))+(BSIM4wdvtp0*Inv_W))+(BSIM4pdvtp0*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp1 = (((((BSIM4dvtp1+(BSIM4ldvtp1*Inv_L))+(BSIM4wdvtp1*Inv_W))+(BSIM4pdvtp1*Inv_LW)))*UNITFAD).val();
    BSIM4dvt0 = (((((BSIM4dvt0+(BSIM4ldvt0*Inv_L))+(BSIM4wdvt0*Inv_W))+(BSIM4pdvt0*Inv_LW)))*UNITFAD).val();
    BSIM4dvt1 = (((((BSIM4dvt1+(BSIM4ldvt1*Inv_L))+(BSIM4wdvt1*Inv_W))+(BSIM4pdvt1*Inv_LW)))*UNITFAD).val();
    BSIM4dvt2 = (((((BSIM4dvt2+(BSIM4ldvt2*Inv_L))+(BSIM4wdvt2*Inv_W))+(BSIM4pdvt2*Inv_LW)))*UNITFAD).val();
    BSIM4dvt0w = (((((BSIM4dvt0w+(BSIM4ldvt0w*Inv_L))+(BSIM4wdvt0w*Inv_W))+(BSIM4pdvt0w*Inv_LW)))*UNITFAD).val();
    BSIM4dvt1w = (((((BSIM4dvt1w+(BSIM4ldvt1w*Inv_L))+(BSIM4wdvt1w*Inv_W))+(BSIM4pdvt1w*Inv_LW)))*UNITFAD).val();
    BSIM4dvt2w = (((((BSIM4dvt2w+(BSIM4ldvt2w*Inv_L))+(BSIM4wdvt2w*Inv_W))+(BSIM4pdvt2w*Inv_LW)))*UNITFAD).val();
    BSIM4drout = (((((BSIM4drout+(BSIM4ldrout*Inv_L))+(BSIM4wdrout*Inv_W))+(BSIM4pdrout*Inv_LW)))*UNITFAD).val();
    BSIM4dsub = (((((BSIM4dsub+(BSIM4ldsub*Inv_L))+(BSIM4wdsub*Inv_W))+(BSIM4pdsub*Inv_LW)))*UNITFAD).val();
    BSIM4vth0 = (((((BSIM4vth0+(BSIM4lvth0*Inv_L))+(BSIM4wvth0*Inv_W))+(BSIM4pvth0*Inv_LW)))*UNITFAD).val();
    BSIM4ua = (((((BSIM4ua+(BSIM4lua*Inv_L))+(BSIM4wua*Inv_W))+(BSIM4pua*Inv_LW)))*UNITFAD).val();
    BSIM4ua1 = (((((BSIM4ua1+(BSIM4lua1*Inv_L))+(BSIM4wua1*Inv_W))+(BSIM4pua1*Inv_LW)))*UNITFAD).val();
    BSIM4ub = (((((BSIM4ub+(BSIM4lub*Inv_L))+(BSIM4wub*Inv_W))+(BSIM4pub*Inv_LW)))*UNITFAD).val();
    BSIM4ub1 = (((((BSIM4ub1+(BSIM4lub1*Inv_L))+(BSIM4wub1*Inv_W))+(BSIM4pub1*Inv_LW)))*UNITFAD).val();
    BSIM4uc = (((((BSIM4uc+(BSIM4luc*Inv_L))+(BSIM4wuc*Inv_W))+(BSIM4puc*Inv_LW)))*UNITFAD).val();
    BSIM4uc1 = (((((BSIM4uc1+(BSIM4luc1*Inv_L))+(BSIM4wuc1*Inv_W))+(BSIM4puc1*Inv_LW)))*UNITFAD).val();
    BSIM4eu = (((((BSIM4eu+(BSIM4leu*Inv_L))+(BSIM4weu*Inv_W))+(BSIM4peu*Inv_LW)))*UNITFAD).val();
    BSIM4u0 = (((((BSIM4u0+(BSIM4lu0*Inv_L))+(BSIM4wu0*Inv_W))+(BSIM4pu0*Inv_LW)))*UNITFAD).val();
    BSIM4ute = (((((BSIM4ute+(BSIM4lute*Inv_L))+(BSIM4wute*Inv_W))+(BSIM4pute*Inv_LW)))*UNITFAD).val();
    BSIM4tvoffcv = (((((BSIM4tvoffcv+(BSIM4ltvoffcv*Inv_L))+(BSIM4wtvoffcv*Inv_W))+(BSIM4ptvoffcv*Inv_LW)))*UNITFAD).val();
    BSIM4teta0 = (((((BSIM4teta0+(BSIM4lteta0*Inv_L))+(BSIM4wteta0*Inv_W))+(BSIM4pteta0*Inv_LW)))*UNITFAD).val();
    BSIM4tnfactor = (((((BSIM4tnfactor+(BSIM4ltnfactor*Inv_L))+(BSIM4wtnfactor*Inv_W))+(BSIM4ptnfactor*Inv_LW)))*UNITFAD).val();
    BSIM4minvcv = (((((BSIM4minvcv+(BSIM4lminvcv*Inv_L))+(BSIM4wminvcv*Inv_W))+(BSIM4pminvcv*Inv_LW)))*UNITFAD).val();
    BSIM4tvfbsdoff = (((((BSIM4tvfbsdoff+(BSIM4ltvfbsdoff*Inv_L))+(BSIM4wtvfbsdoff*Inv_W))+(BSIM4ptvfbsdoff*Inv_LW)))*UNITFAD).val();
    BSIM4aigs = (((((BSIM4aigs+(BSIM4laigs*Inv_L))+(BSIM4waigs*Inv_W))+(BSIM4paigs*Inv_LW)))*UNITFAD).val();
    BSIM4bigs = (((((BSIM4bigs+(BSIM4lbigs*Inv_L))+(BSIM4wbigs*Inv_W))+(BSIM4pbigs*Inv_LW)))*UNITFAD).val();
    BSIM4cigs = (((((BSIM4cigs+(BSIM4lcigs*Inv_L))+(BSIM4wcigs*Inv_W))+(BSIM4pcigs*Inv_LW)))*UNITFAD).val();
    BSIM4aigd = (((((BSIM4aigd+(BSIM4laigd*Inv_L))+(BSIM4waigd*Inv_W))+(BSIM4paigd*Inv_LW)))*UNITFAD).val();
    BSIM4bigd = (((((BSIM4bigd+(BSIM4lbigd*Inv_L))+(BSIM4wbigd*Inv_W))+(BSIM4pbigd*Inv_LW)))*UNITFAD).val();
    BSIM4cigd = (((((BSIM4cigd+(BSIM4lcigd*Inv_L))+(BSIM4wcigd*Inv_W))+(BSIM4pcigd*Inv_LW)))*UNITFAD).val();
    BSIM4agisl = (((((BSIM4agisl+(BSIM4lagisl*Inv_L))+(BSIM4wagisl*Inv_W))+(BSIM4pagisl*Inv_LW)))*UNITFAD).val();
    BSIM4bgisl = (((((BSIM4bgisl+(BSIM4lbgisl*Inv_L))+(BSIM4wbgisl*Inv_W))+(BSIM4pbgisl*Inv_LW)))*UNITFAD).val();
    BSIM4cgisl = (((((BSIM4cgisl+(BSIM4lcgisl*Inv_L))+(BSIM4wcgisl*Inv_W))+(BSIM4pcgisl*Inv_LW)))*UNITFAD).val();
    BSIM4egisl = (((((BSIM4egisl+(BSIM4legisl*Inv_L))+(BSIM4wegisl*Inv_W))+(BSIM4pegisl*Inv_LW)))*UNITFAD).val();
    BSIM4rgisl = (((((BSIM4rgisl+(BSIM4lrgisl*Inv_L))+(BSIM4wrgisl*Inv_W))+(BSIM4prgisl*Inv_LW)))*UNITFAD).val();
    BSIM4kgisl = (((((BSIM4kgisl+(BSIM4lkgisl*Inv_L))+(BSIM4wkgisl*Inv_W))+(BSIM4pkgisl*Inv_LW)))*UNITFAD).val();
    BSIM4fgisl = (((((BSIM4fgisl+(BSIM4lfgisl*Inv_L))+(BSIM4wfgisl*Inv_W))+(BSIM4pfgisl*Inv_LW)))*UNITFAD).val();
    BSIM4rgidl = (((((BSIM4rgidl+(BSIM4lrgidl*Inv_L))+(BSIM4wrgidl*Inv_W))+(BSIM4prgidl*Inv_LW)))*UNITFAD).val();
    BSIM4kgidl = (((((BSIM4kgidl+(BSIM4lkgidl*Inv_L))+(BSIM4wkgidl*Inv_W))+(BSIM4pkgidl*Inv_LW)))*UNITFAD).val();
    BSIM4fgidl = (((((BSIM4fgidl+(BSIM4lfgidl*Inv_L))+(BSIM4wfgidl*Inv_W))+(BSIM4pfgidl*Inv_LW)))*UNITFAD).val();
    BSIM4ud = (((((BSIM4ud+(BSIM4lud*Inv_L))+(BSIM4wud*Inv_W))+(BSIM4pud*Inv_LW)))*UNITFAD).val();
    BSIM4ud1 = (((((BSIM4ud1+(BSIM4lud1*Inv_L))+(BSIM4wud1*Inv_W))+(BSIM4pud1*Inv_LW)))*UNITFAD).val();
    BSIM4up = (((((BSIM4up+(BSIM4lup*Inv_L))+(BSIM4wup*Inv_W))+(BSIM4pup*Inv_LW)))*UNITFAD).val();
    BSIM4lp = (((((BSIM4lp+(BSIM4llp*Inv_L))+(BSIM4wlp*Inv_W))+(BSIM4plp*Inv_LW)))*UNITFAD).val();
    BSIM4ucs = (((((BSIM4ucs+(BSIM4lucs*Inv_L))+(BSIM4wucs*Inv_W))+(BSIM4pucs*Inv_LW)))*UNITFAD).val();
    BSIM4ucste = (((((BSIM4ucste+(BSIM4lucste*Inv_L))+(BSIM4wucste*Inv_W))+(BSIM4pucste*Inv_LW)))*UNITFAD).val();
    BSIM4kvth0we = (((((BSIM4kvth0we+(BSIM4lkvth0we*Inv_L))+(BSIM4wkvth0we*Inv_W))+(BSIM4pkvth0we*Inv_LW)))*UNITFAD).val();
    BSIM4k2we = (((((BSIM4k2we+(BSIM4lk2we*Inv_L))+(BSIM4wk2we*Inv_W))+(BSIM4pk2we*Inv_LW)))*UNITFAD).val();
    BSIM4ku0we = (((((BSIM4ku0we+(BSIM4lku0we*Inv_L))+(BSIM4wku0we*Inv_W))+(BSIM4pku0we*Inv_LW)))*UNITFAD).val();
    BSIM4vfbsdoff = (((((BSIM4vfbsdoff+(BSIM4lvfbsdoff*Inv_L))+(BSIM4wvfbsdoff*Inv_W))+(BSIM4pvfbsdoff*Inv_LW)))*UNITFAD).val();
    BSIM4tvoff = (((((BSIM4tvoff+(BSIM4ltvoff*Inv_L))+(BSIM4wtvoff*Inv_W))+(BSIM4ptvoff*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp2 = (((((BSIM4dvtp2+(BSIM4ldvtp2*Inv_L))+(BSIM4wdvtp2*Inv_W))+(BSIM4pdvtp2*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp3 = (((((BSIM4dvtp3+(BSIM4ldvtp3*Inv_L))+(BSIM4wdvtp3*Inv_W))+(BSIM4pdvtp3*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp4 = (((((BSIM4dvtp4+(BSIM4ldvtp4*Inv_L))+(BSIM4wdvtp4*Inv_W))+(BSIM4pdvtp4*Inv_LW)))*UNITFAD).val();
    BSIM4dvtp5 = (((((BSIM4dvtp5+(BSIM4ldvtp5*Inv_L))+(BSIM4wdvtp5*Inv_W))+(BSIM4pdvtp5*Inv_LW)))*UNITFAD).val();
    BSIM4voff = (((((BSIM4voff+(BSIM4lvoff*Inv_L))+(BSIM4wvoff*Inv_W))+(BSIM4pvoff*Inv_LW)))*UNITFAD).val();
    BSIM4minv = (((((BSIM4minv+(BSIM4lminv*Inv_L))+(BSIM4wminv*Inv_W))+(BSIM4pminv*Inv_LW)))*UNITFAD).val();
    BSIM4fprout = (((((BSIM4fprout+(BSIM4lfprout*Inv_L))+(BSIM4wfprout*Inv_W))+(BSIM4pfprout*Inv_LW)))*UNITFAD).val();
    BSIM4pdits = (((((BSIM4pdits+(BSIM4lpdits*Inv_L))+(BSIM4wpdits*Inv_W))+(BSIM4ppdits*Inv_LW)))*UNITFAD).val();
    BSIM4pditsd = (((((BSIM4pditsd+(BSIM4lpditsd*Inv_L))+(BSIM4wpditsd*Inv_W))+(BSIM4ppditsd*Inv_LW)))*UNITFAD).val();
    BSIM4delta = (((((BSIM4delta+(BSIM4ldelta*Inv_L))+(BSIM4wdelta*Inv_W))+(BSIM4pdelta*Inv_LW)))*UNITFAD).val();
    BSIM4rdsw = (((((BSIM4rdsw+(BSIM4lrdsw*Inv_L))+(BSIM4wrdsw*Inv_W))+(BSIM4prdsw*Inv_LW)))*UNITFAD).val();
    BSIM4rdw = (((((BSIM4rdw+(BSIM4lrdw*Inv_L))+(BSIM4wrdw*Inv_W))+(BSIM4prdw*Inv_LW)))*UNITFAD).val();
    BSIM4rsw = (((((BSIM4rsw+(BSIM4lrsw*Inv_L))+(BSIM4wrsw*Inv_W))+(BSIM4prsw*Inv_LW)))*UNITFAD).val();
    BSIM4prwg = (((((BSIM4prwg+(BSIM4lprwg*Inv_L))+(BSIM4wprwg*Inv_W))+(BSIM4pprwg*Inv_LW)))*UNITFAD).val();
    BSIM4prwb = (((((BSIM4prwb+(BSIM4lprwb*Inv_L))+(BSIM4wprwb*Inv_W))+(BSIM4pprwb*Inv_LW)))*UNITFAD).val();
    BSIM4prt = (((((BSIM4prt+(BSIM4lprt*Inv_L))+(BSIM4wprt*Inv_W))+(BSIM4pprt*Inv_LW)))*UNITFAD).val();
    BSIM4eta0 = (((((BSIM4eta0+(BSIM4leta0*Inv_L))+(BSIM4weta0*Inv_W))+(BSIM4peta0*Inv_LW)))*UNITFAD).val();
    BSIM4etab = (((((BSIM4etab+(BSIM4letab*Inv_L))+(BSIM4wetab*Inv_W))+(BSIM4petab*Inv_LW)))*UNITFAD).val();
    BSIM4pclm = (((((BSIM4pclm+(BSIM4lpclm*Inv_L))+(BSIM4wpclm*Inv_W))+(BSIM4ppclm*Inv_LW)))*UNITFAD).val();
    BSIM4pdibl1 = (((((BSIM4pdibl1+(BSIM4lpdibl1*Inv_L))+(BSIM4wpdibl1*Inv_W))+(BSIM4ppdibl1*Inv_LW)))*UNITFAD).val();
    BSIM4pdibl2 = (((((BSIM4pdibl2+(BSIM4lpdibl2*Inv_L))+(BSIM4wpdibl2*Inv_W))+(BSIM4ppdibl2*Inv_LW)))*UNITFAD).val();
    BSIM4pdiblb = (((((BSIM4pdiblb+(BSIM4lpdiblb*Inv_L))+(BSIM4wpdiblb*Inv_W))+(BSIM4ppdiblb*Inv_LW)))*UNITFAD).val();
    BSIM4pscbe1 = (((((BSIM4pscbe1+(BSIM4lpscbe1*Inv_L))+(BSIM4wpscbe1*Inv_W))+(BSIM4ppscbe1*Inv_LW)))*UNITFAD).val();
    BSIM4pscbe2 = (((((BSIM4pscbe2+(BSIM4lpscbe2*Inv_L))+(BSIM4wpscbe2*Inv_W))+(BSIM4ppscbe2*Inv_LW)))*UNITFAD).val();
    BSIM4pvag = (((((BSIM4pvag+(BSIM4lpvag*Inv_L))+(BSIM4wpvag*Inv_W))+(BSIM4ppvag*Inv_LW)))*UNITFAD).val();
    BSIM4wr = (((((BSIM4wr+(BSIM4lwr*Inv_L))+(BSIM4wwr*Inv_W))+(BSIM4pwr*Inv_LW)))*UNITFAD).val();
    BSIM4dwg = (((((BSIM4dwg+(BSIM4ldwg*Inv_L))+(BSIM4wdwg*Inv_W))+(BSIM4pdwg*Inv_LW)))*UNITFAD).val();
    BSIM4dwb = (((((BSIM4dwb+(BSIM4ldwb*Inv_L))+(BSIM4wdwb*Inv_W))+(BSIM4pdwb*Inv_LW)))*UNITFAD).val();
    BSIM4b0 = (((((BSIM4b0+(BSIM4lb0*Inv_L))+(BSIM4wb0*Inv_W))+(BSIM4pb0*Inv_LW)))*UNITFAD).val();
    BSIM4b1 = (((((BSIM4b1+(BSIM4lb1*Inv_L))+(BSIM4wb1*Inv_W))+(BSIM4pb1*Inv_LW)))*UNITFAD).val();
    BSIM4alpha0 = (((((BSIM4alpha0+(BSIM4lalpha0*Inv_L))+(BSIM4walpha0*Inv_W))+(BSIM4palpha0*Inv_LW)))*UNITFAD).val();
    BSIM4alpha1 = (((((BSIM4alpha1+(BSIM4lalpha1*Inv_L))+(BSIM4walpha1*Inv_W))+(BSIM4palpha1*Inv_LW)))*UNITFAD).val();
    BSIM4beta0 = (((((BSIM4beta0+(BSIM4lbeta0*Inv_L))+(BSIM4wbeta0*Inv_W))+(BSIM4pbeta0*Inv_LW)))*UNITFAD).val();
    BSIM4agidl = (((((BSIM4agidl+(BSIM4lagidl*Inv_L))+(BSIM4wagidl*Inv_W))+(BSIM4pagidl*Inv_LW)))*UNITFAD).val();
    BSIM4bgidl = (((((BSIM4bgidl+(BSIM4lbgidl*Inv_L))+(BSIM4wbgidl*Inv_W))+(BSIM4pbgidl*Inv_LW)))*UNITFAD).val();
    BSIM4cgidl = (((((BSIM4cgidl+(BSIM4lcgidl*Inv_L))+(BSIM4wcgidl*Inv_W))+(BSIM4pcgidl*Inv_LW)))*UNITFAD).val();
    BSIM4egidl = (((((BSIM4egidl+(BSIM4legidl*Inv_L))+(BSIM4wegidl*Inv_W))+(BSIM4pegidl*Inv_LW)))*UNITFAD).val();
    BSIM4aigc = (((((BSIM4aigc+(BSIM4laigc*Inv_L))+(BSIM4waigc*Inv_W))+(BSIM4paigc*Inv_LW)))*UNITFAD).val();
    BSIM4bigc = (((((BSIM4bigc+(BSIM4lbigc*Inv_L))+(BSIM4wbigc*Inv_W))+(BSIM4pbigc*Inv_LW)))*UNITFAD).val();
    BSIM4cigc = (((((BSIM4cigc+(BSIM4lcigc*Inv_L))+(BSIM4wcigc*Inv_W))+(BSIM4pcigc*Inv_LW)))*UNITFAD).val();
    BSIM4aigsd = (((((BSIM4aigsd+(BSIM4laigsd*Inv_L))+(BSIM4waigsd*Inv_W))+(BSIM4paigsd*Inv_LW)))*UNITFAD).val();
    BSIM4bigsd = (((((BSIM4bigsd+(BSIM4lbigsd*Inv_L))+(BSIM4wbigsd*Inv_W))+(BSIM4pbigsd*Inv_LW)))*UNITFAD).val();
    BSIM4cigsd = (((((BSIM4cigsd+(BSIM4lcigsd*Inv_L))+(BSIM4wcigsd*Inv_W))+(BSIM4pcigsd*Inv_LW)))*UNITFAD).val();
    BSIM4aigbacc = (((((BSIM4aigbacc+(BSIM4laigbacc*Inv_L))+(BSIM4waigbacc*Inv_W))+(BSIM4paigbacc*Inv_LW)))*UNITFAD).val();
    BSIM4bigbacc = (((((BSIM4bigbacc+(BSIM4lbigbacc*Inv_L))+(BSIM4wbigbacc*Inv_W))+(BSIM4pbigbacc*Inv_LW)))*UNITFAD).val();
    BSIM4cigbacc = (((((BSIM4cigbacc+(BSIM4lcigbacc*Inv_L))+(BSIM4wcigbacc*Inv_W))+(BSIM4pcigbacc*Inv_LW)))*UNITFAD).val();
    BSIM4aigbinv = (((((BSIM4aigbinv+(BSIM4laigbinv*Inv_L))+(BSIM4waigbinv*Inv_W))+(BSIM4paigbinv*Inv_LW)))*UNITFAD).val();
    BSIM4bigbinv = (((((BSIM4bigbinv+(BSIM4lbigbinv*Inv_L))+(BSIM4wbigbinv*Inv_W))+(BSIM4pbigbinv*Inv_LW)))*UNITFAD).val();
    BSIM4cigbinv = (((((BSIM4cigbinv+(BSIM4lcigbinv*Inv_L))+(BSIM4wcigbinv*Inv_W))+(BSIM4pcigbinv*Inv_LW)))*UNITFAD).val();
    BSIM4nigc = (((((BSIM4nigc+(BSIM4lnigc*Inv_L))+(BSIM4wnigc*Inv_W))+(BSIM4pnigc*Inv_LW)))*UNITFAD).val();
    BSIM4nigbacc = (((((BSIM4nigbacc+(BSIM4lnigbacc*Inv_L))+(BSIM4wnigbacc*Inv_W))+(BSIM4pnigbacc*Inv_LW)))*UNITFAD).val();
    BSIM4nigbinv = (((((BSIM4nigbinv+(BSIM4lnigbinv*Inv_L))+(BSIM4wnigbinv*Inv_W))+(BSIM4pnigbinv*Inv_LW)))*UNITFAD).val();
    BSIM4ntox = (((((BSIM4ntox+(BSIM4lntox*Inv_L))+(BSIM4wntox*Inv_W))+(BSIM4pntox*Inv_LW)))*UNITFAD).val();
    BSIM4eigbinv = (((((BSIM4eigbinv+(BSIM4leigbinv*Inv_L))+(BSIM4weigbinv*Inv_W))+(BSIM4peigbinv*Inv_LW)))*UNITFAD).val();
    BSIM4pigcd = (((((BSIM4pigcd+(BSIM4lpigcd*Inv_L))+(BSIM4wpigcd*Inv_W))+(BSIM4ppigcd*Inv_LW)))*UNITFAD).val();
    BSIM4poxedge = (((((BSIM4poxedge+(BSIM4lpoxedge*Inv_L))+(BSIM4wpoxedge*Inv_W))+(BSIM4ppoxedge*Inv_LW)))*UNITFAD).val();
    BSIM4xrcrg1 = (((((BSIM4xrcrg1+(BSIM4lxrcrg1*Inv_L))+(BSIM4wxrcrg1*Inv_W))+(BSIM4pxrcrg1*Inv_LW)))*UNITFAD).val();
    BSIM4xrcrg2 = (((((BSIM4xrcrg2+(BSIM4lxrcrg2*Inv_L))+(BSIM4wxrcrg2*Inv_W))+(BSIM4pxrcrg2*Inv_LW)))*UNITFAD).val();
    BSIM4lambda = (((((BSIM4lambda+(BSIM4llambda*Inv_L))+(BSIM4wlambda*Inv_W))+(BSIM4plambda*Inv_LW)))*UNITFAD).val();
    BSIM4vtl = (((((BSIM4vtl+(BSIM4lvtl*Inv_L))+(BSIM4wvtl*Inv_W))+(BSIM4pvtl*Inv_LW)))*UNITFAD).val();
    BSIM4xn = (((((BSIM4xn+(BSIM4lxn*Inv_L))+(BSIM4wxn*Inv_W))+(BSIM4pxn*Inv_LW)))*UNITFAD).val();
    BSIM4cgsl = (((((BSIM4cgsl+(BSIM4lcgsl*Inv_L))+(BSIM4wcgsl*Inv_W))+(BSIM4pcgsl*Inv_LW)))*UNITFAD).val();
    BSIM4cgdl = (((((BSIM4cgdl+(BSIM4lcgdl*Inv_L))+(BSIM4wcgdl*Inv_W))+(BSIM4pcgdl*Inv_LW)))*UNITFAD).val();
    BSIM4ckappas = (((((BSIM4ckappas+(BSIM4lckappas*Inv_L))+(BSIM4wckappas*Inv_W))+(BSIM4pckappas*Inv_LW)))*UNITFAD).val();
    BSIM4ckappad = (((((BSIM4ckappad+(BSIM4lckappad*Inv_L))+(BSIM4wckappad*Inv_W))+(BSIM4pckappad*Inv_LW)))*UNITFAD).val();
    BSIM4cf = (((((BSIM4cf+(BSIM4lcf*Inv_L))+(BSIM4wcf*Inv_W))+(BSIM4pcf*Inv_LW)))*UNITFAD).val();
    BSIM4clc = (((((BSIM4clc+(BSIM4lclc*Inv_L))+(BSIM4wclc*Inv_W))+(BSIM4pclc*Inv_LW)))*UNITFAD).val();
    BSIM4cle = (((((BSIM4cle+(BSIM4lcle*Inv_L))+(BSIM4wcle*Inv_W))+(BSIM4pcle*Inv_LW)))*UNITFAD).val();
    BSIM4vfbcv = (((((BSIM4vfbcv+(BSIM4lvfbcv*Inv_L))+(BSIM4wvfbcv*Inv_W))+(BSIM4pvfbcv*Inv_LW)))*UNITFAD).val();
    BSIM4acde = (((((BSIM4acde+(BSIM4lacde*Inv_L))+(BSIM4wacde*Inv_W))+(BSIM4pacde*Inv_LW)))*UNITFAD).val();
    BSIM4moin = (((((BSIM4moin+(BSIM4lmoin*Inv_L))+(BSIM4wmoin*Inv_W))+(BSIM4pmoin*Inv_LW)))*UNITFAD).val();
    BSIM4noff = (((((BSIM4noff+(BSIM4lnoff*Inv_L))+(BSIM4wnoff*Inv_W))+(BSIM4pnoff*Inv_LW)))*UNITFAD).val();
    BSIM4voffcv = (((((BSIM4voffcv+(BSIM4lvoffcv*Inv_L))+(BSIM4wvoffcv*Inv_W))+(BSIM4pvoffcv*Inv_LW)))*UNITFAD).val();
    BSIM4abulkCVfactor = (((1.0+pow((BSIM4clc/BSIM4leffCV),BSIM4cle)))*UNITFAD).val();
    if((this->verbose == 1)) {
      printf("BSIM4abulkCVfactor=%g\n",((BSIM4abulkCVfactor)*UNITFAD).val());
    }

    T0 = (((TRatio-1.0))*UNITFAD).val();
    PowWeffWr = (((pow((BSIM4weffCJ*1.0e6),BSIM4wr)*BSIM4nf))*UNITFAD).val();
    T1 = ((0.0)*UNITFAD).val();
    T2 = ((0.0)*UNITFAD).val();
    T3 = ((0.0)*UNITFAD).val();
    T4 = ((0.0)*UNITFAD).val();
    if((BSIM4tempMod == 0)) {
      {
        BSIM4ua = (((BSIM4ua+(BSIM4ua1*T0)))*UNITFAD).val();
        BSIM4ub = (((BSIM4ub+(BSIM4ub1*T0)))*UNITFAD).val();
        BSIM4uc = (((BSIM4uc+(BSIM4uc1*T0)))*UNITFAD).val();
        BSIM4vsattemp = (((BSIM4vsat-(BSIM4at*T0)))*UNITFAD).val();
        T10 = (((BSIM4prt*T0))*UNITFAD).val();
        if(BSIM4rdsMod) {
          {
            T1 = (((BSIM4rdw+T10))*UNITFAD).val();
            T2 = (((BSIM4rdwmin+T10))*UNITFAD).val();
            T3 = (((BSIM4rsw+T10))*UNITFAD).val();
            T4 = (((BSIM4rswmin+T10))*UNITFAD).val();
          }

        }

        BSIM4rds0 = (((((BSIM4rdsw+T10)*BSIM4nf)/PowWeffWr))*UNITFAD).val();
        BSIM4rdswmin = (((((BSIM4rdswmin+T10)*BSIM4nf)/PowWeffWr))*UNITFAD).val();
      }

    }
    else {
      {
        if((BSIM4tempMod == 3)) {
          {
            BSIM4ua = (((BSIM4ua*pow(TRatio,BSIM4ua1)))*UNITFAD).val();
            BSIM4ub = (((BSIM4ub*pow(TRatio,BSIM4ub1)))*UNITFAD).val();
            BSIM4uc = (((BSIM4uc*pow(TRatio,BSIM4uc1)))*UNITFAD).val();
            BSIM4ud = (((BSIM4ud*pow(TRatio,BSIM4ud1)))*UNITFAD).val();
          }

        }
        else {
          {
            BSIM4ua = (((BSIM4ua*(1.0+(BSIM4ua1*delTemp))))*UNITFAD).val();
            BSIM4ub = (((BSIM4ub*(1.0+(BSIM4ub1*delTemp))))*UNITFAD).val();
            BSIM4uc = (((BSIM4uc*(1.0+(BSIM4uc1*delTemp))))*UNITFAD).val();
          }

        }

        BSIM4vsattemp = (((BSIM4vsat*(1.0-(BSIM4at*delTemp))))*UNITFAD).val();
        T10 = (((1.0+(BSIM4prt*delTemp)))*UNITFAD).val();
        if(BSIM4rdsMod) {
          {
            T1 = (((BSIM4rdw*T10))*UNITFAD).val();
            T2 = (((BSIM4rdwmin*T10))*UNITFAD).val();
            T3 = (((BSIM4rsw*T10))*UNITFAD).val();
            T4 = (((BSIM4rswmin*T10))*UNITFAD).val();
          }

        }

        BSIM4rds0 = (((((BSIM4rdsw*T10)*BSIM4nf)/PowWeffWr))*UNITFAD).val();
        BSIM4rdswmin = (((((BSIM4rdswmin*T10)*BSIM4nf)/PowWeffWr))*UNITFAD).val();
      }

    }

    if((T1 < 0.0)) {
      {
        T1 = ((0.0)*UNITFAD).val();
        printf("Warning: (instance %M) Rdw at current temperature is negative; set to 0.\n");
      }

    }

    if((T2 < 0.0)) {
      {
        T2 = ((0.0)*UNITFAD).val();
        printf("Warning: (instance %M) Rdwmin at current temperature is negative; \
                         set to 0.\n");
      }

    }

    BSIM4rd0 = (((T1/PowWeffWr))*UNITFAD).val();
    BSIM4rdwmin = (((T2/PowWeffWr))*UNITFAD).val();
    if((T3 < 0.0)) {
      {
        T3 = ((0.0)*UNITFAD).val();
        printf("Warning: (instance %M) Rsw at current temperature is negative; \
                         set to 0.\n");
      }

    }

    if((T4 < 0.0)) {
      {
        T4 = ((0.0)*UNITFAD).val();
        printf("Warning: (instance %M) Rswmin at current temperature is negative; \
                         set to 0.\n");
      }

    }

    BSIM4rs0 = (((T3/PowWeffWr))*UNITFAD).val();
    BSIM4rswmin = (((T4/PowWeffWr))*UNITFAD).val();
    if((BSIM4u0 > 1.0)) {
      BSIM4u0 = (((BSIM4u0/1.0e4))*UNITFAD).val();
    }

    T5 = (((1.0-(BSIM4up*exp((-BSIM4leff/BSIM4lp)))))*UNITFAD).val();
    BSIM4u0temp = ((((BSIM4u0*T5)*pow(TRatio,BSIM4ute)))*UNITFAD).val();
    if((BSIM4eu < 0.0)) {
      {
        BSIM4eu = ((0.0)*UNITFAD).val();
        printf("Warning: (instance %M) eu has been negative; reset to 0.0.\n");
      }

    }

    if((BSIM4ucs < 0.0)) {
      {
        BSIM4ucs = ((0.0)*UNITFAD).val();
        printf("Warning: ucs has been negative; reset to 0.0.\n");
      }

    }

    BSIM4vfbsdoff = (((BSIM4vfbsdoff*(1.0+(BSIM4tvfbsdoff*delTemp))))*UNITFAD).val();
    BSIM4voff = (((BSIM4voff*(1.0+(BSIM4tvoff*delTemp))))*UNITFAD).val();
    BSIM4nfactor = (((BSIM4nfactor+((BSIM4tnfactor*delTemp)/Tnom)))*UNITFAD).val();
    BSIM4voffcv = (((BSIM4voffcv*(1.0+(BSIM4tvoffcv*delTemp))))*UNITFAD).val();
    BSIM4eta0 = (((BSIM4eta0+((BSIM4teta0*delTemp)/Tnom)))*UNITFAD).val();
    if((BSIM4vtlGiven && (BSIM4vtl > 0.0))) {
      {
        if((BSIM4lc < 0.0)) {
          BSIM4lc = ((0.0)*UNITFAD).val();
        }

        T0 = (((BSIM4leff/((BSIM4xn*BSIM4leff)+BSIM4lc)))*UNITFAD).val();
        BSIM4tfactor = ((((1.0-T0)/(1.0+T0)))*UNITFAD).val();
      }

    }

    BSIM4cgdo = ((((BSIM4cgdo+BSIM4cf)*BSIM4weffCV))*UNITFAD).val();
    BSIM4cgso = ((((BSIM4cgso+BSIM4cf)*BSIM4weffCV))*UNITFAD).val();
    BSIM4cgbo = ((((BSIM4cgbo*BSIM4leffCV)*BSIM4nf))*UNITFAD).val();
    if((!BSIM4ndepGiven && BSIM4gamma1Given)) {
      {
        T0 = (((BSIM4gamma1*BSIM4coxe))*UNITFAD).val();
        BSIM4ndep = ((((3.01248e22*T0)*T0))*UNITFAD).val();
      }

    }

    BSIM4phi = (((((Vtm0*log((BSIM4ndep/ni)))+BSIM4phin)+0.4))*UNITFAD).val();
    BSIM4sqrtPhi = ((sqrt(BSIM4phi))*UNITFAD).val();
    BSIM4phis3 = (((BSIM4sqrtPhi*BSIM4phi))*UNITFAD).val();
    BSIM4Xdep0 = (((sqrt(((2.0*epssubl)/((1.602176462e-19*BSIM4ndep)*1.0e6)))*BSIM4sqrtPhi))*UNITFAD).val();
    BSIM4sqrtXdep0 = ((sqrt(BSIM4Xdep0))*UNITFAD).val();
    if((BSIM4mtrlmod == 0)) {
      BSIM4litl = ((sqrt(((((3.0*3.9)/epsroxl)*BSIM4xj)*toxel)))*UNITFAD).val();
    }
    else {
      BSIM4litl = ((sqrt((((BSIM4epsrsub/epsroxl)*BSIM4xj)*toxel)))*UNITFAD).val();
    }

    BSIM4vbi = (((Vtm0*log(((BSIM4nsd*BSIM4ndep)/(ni*ni)))))*UNITFAD).val();
    if((BSIM4mtrlmod == 0)) {
      {
        if((BSIM4ngate > 0.0)) {
          BSIM4vfbsd = (((Vtm0*log((BSIM4ngate/BSIM4nsd))))*UNITFAD).val();
        }
        else {
          BSIM4vfbsd = ((0.0)*UNITFAD).val();
        }

      }

    }
    else {
      {
        T0 = (((Vtm0*log((BSIM4nsd/ni))))*UNITFAD).val();
        T1 = (((0.5*Eg0))*UNITFAD).val();
        if((T0 > T1)) {
          T0 = ((T1)*UNITFAD).val();
        }

        T2 = ((((BSIM4easub+T1)-(BSIM4type*T0)))*UNITFAD).val();
        BSIM4vfbsd = (((BSIM4phig-T2))*UNITFAD).val();
      }

    }

    BSIM4cdep0 = ((sqrt((((((1.602176462e-19*epssubl)*BSIM4ndep)*1.0e6)/2.0)/BSIM4phi)))*UNITFAD).val();
    BSIM4ToxRatio = ((((exp((BSIM4ntox*log((BSIM4toxref/toxel))))/toxel)/toxel))*UNITFAD).val();
    BSIM4ToxRatioEdge = ((((((exp((BSIM4ntox*log((BSIM4toxref/(toxel*BSIM4poxedge)))))/toxel)/toxel)/BSIM4poxedge)/BSIM4poxedge))*UNITFAD).val();
    BSIM4Aechvb = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),4.97232e-7,3.42537e-7))*UNITFAD).val();
    BSIM4Bechvb = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),7.45669e11,1.16645e12))*UNITFAD).val();
    BSIM4AechvbEdgeS = (((((BSIM4Aechvb*BSIM4weff)*BSIM4dlcig)*BSIM4ToxRatioEdge))*UNITFAD).val();
    BSIM4AechvbEdgeD = (((((BSIM4Aechvb*BSIM4weff)*BSIM4dlcigd)*BSIM4ToxRatioEdge))*UNITFAD).val();
    BSIM4BechvbEdge = ((((-BSIM4Bechvb*toxel)*BSIM4poxedge))*UNITFAD).val();
    BSIM4Aechvb = (((BSIM4Aechvb*((BSIM4weff*BSIM4leff)*BSIM4ToxRatio)))*UNITFAD).val();
    BSIM4Bechvb = (((BSIM4Bechvb*-toxel))*UNITFAD).val();
    BSIM4mstar = (((0.5+(atan(BSIM4minv)/3.14159265358979323846)))*UNITFAD).val();
    BSIM4mstarcv = (((0.5+(atan(BSIM4minvcv)/3.14159265358979323846)))*UNITFAD).val();
    BSIM4voffcbn = (((BSIM4voff+(BSIM4voffl/BSIM4leff)))*UNITFAD).val();
    BSIM4voffcbncv = (((BSIM4voffcv+(BSIM4voffcvl/BSIM4leff)))*UNITFAD).val();
    BSIM4ldeb = (((sqrt(((epssubl*Vtm0)/((1.602176462e-19*BSIM4ndep)*1.0e6)))/3.0))*UNITFAD).val();
    BSIM4acde = (((BSIM4acde*pow((BSIM4ndep/2.0e16),-0.25)))*UNITFAD).val();
    if((this->verbose == 1)) {
      {
        printf("BSIM4AechvbEdgeS,D=%g,%g\n",((BSIM4AechvbEdgeS)*UNITFAD).val(),((BSIM4AechvbEdgeD)*UNITFAD).val());
        printf("BSIM4BechvbEdge,BSIM4Aechvb,BSIM4Bechvb=%g,%g,%g\n",((BSIM4BechvbEdge)*UNITFAD).val(),((BSIM4Aechvb)*UNITFAD).val(),((BSIM4Bechvb)*UNITFAD).val());
      }

    }

    if((BSIM4k1Given || BSIM4k2Given)) {
      {
        if(!BSIM4k1Given) {
          {
            printf("Warning: (instance %M) k1 should be specified with k2.\n");
            BSIM4k1 = ((0.53)*UNITFAD).val();
          }

        }

        if(!BSIM4k2Given) {
          {
            printf("Warning: (instance %M) k2 should be specified with k1.\n");
            BSIM4k2 = ((-0.0186)*UNITFAD).val();
          }

        }

        if(BSIM4nsubGiven) {
          printf("Warning: (instance %M) nsub is ignored because k1 or k2 is given.\n");
        }

        if(BSIM4xtGiven) {
          printf("Warning: (instance %M) xt is ignored because k1 or k2 is given.\n");
        }

        if(BSIM4vbxGiven) {
          printf("Warning: (instance %M) vbx is ignored because k1 or k2 is given.\n");
        }

        if(BSIM4gamma1Given) {
          printf("Warning: (instance %M) gamma1 is ignored because k1 or k2 \
                             is given.\n");
        }

        if(BSIM4gamma2Given) {
          printf("Warning: (instance %M) gamma2 is ignored because k1 or k2 \
                             is given.\n");
        }

      }

    }
    else {
      {
        if(!BSIM4vbxGiven) {
          printf("\n RECALCULATION for no K1 or K2");
        }

        BSIM4vbx = (((BSIM4phi-(((7.7348e-4*BSIM4ndep)*BSIM4xt)*BSIM4xt)))*UNITFAD).val();
        if((BSIM4vbx > 0.0)) {
          BSIM4vbx = ((-BSIM4vbx)*UNITFAD).val();
        }

        if((BSIM4vbm > 0.0)) {
          BSIM4vbm = ((-BSIM4vbm)*UNITFAD).val();
        }

        if(!BSIM4gamma1Given) {
          BSIM4gamma1 = ((((5.753e-12*sqrt(BSIM4ndep))/BSIM4coxe))*UNITFAD).val();
        }

        if(!BSIM4gamma2Given) {
          BSIM4gamma2 = ((((5.753e-12*sqrt(BSIM4nsub))/BSIM4coxe))*UNITFAD).val();
        }

        T0 = (((BSIM4gamma1-BSIM4gamma2))*UNITFAD).val();
        T1 = (((sqrt((BSIM4phi-BSIM4vbx))-BSIM4sqrtPhi))*UNITFAD).val();
        T2 = (((sqrt((BSIM4phi*(BSIM4phi-BSIM4vbm)))-BSIM4phi))*UNITFAD).val();
        BSIM4k2 = ((((T0*T1)/((2.0*T2)+BSIM4vbm)))*UNITFAD).val();
        BSIM4k1 = (((BSIM4gamma2-((2.0*BSIM4k2)*sqrt((BSIM4phi-BSIM4vbm)))))*UNITFAD).val();
      }

    }

    if(!BSIM4vfbGiven) {
      {
        if(BSIM4vth0Given) {
          BSIM4vfb = (((((BSIM4type*BSIM4vth0)-BSIM4phi)-(BSIM4k1*BSIM4sqrtPhi)))*UNITFAD).val();
        }
        else {
          {
            if(((BSIM4mtrlmod && BSIM4phigGiven) && BSIM4nsubGiven)) {
              {
                T0 = (((Vtm0*log((BSIM4nsub/ni))))*UNITFAD).val();
                T1 = (((0.5*Eg0))*UNITFAD).val();
                if((T0 > T1)) {
                  T0 = ((T1)*UNITFAD).val();
                }

                T2 = ((((BSIM4easub+T1)+(BSIM4type*T0)))*UNITFAD).val();
                BSIM4vfb = (((BSIM4phig-T2))*UNITFAD).val();
              }

            }
            else {
              BSIM4vfb = ((-1.0)*UNITFAD).val();
            }

          }

        }

      }

    }

    if(!BSIM4vth0Given) {
      {
        BSIM4vth0 = (((BSIM4type*((BSIM4vfb+BSIM4phi)+(BSIM4k1*BSIM4sqrtPhi))))*UNITFAD).val();
      }

    }

    BSIM4k1ox = ((((BSIM4k1*toxel)/BSIM4toxm))*UNITFAD).val();
    tmp = ((sqrt((((epssubl/(epsroxl*8.85418e-12))*toxel)*BSIM4Xdep0)))*UNITFAD).val();
    T0 = ((((BSIM4dsub*BSIM4leff)/tmp))*UNITFAD).val();
    if((T0 < 34.0)) {
      {
        T1 = ((exp(T0))*UNITFAD).val();
        T2 = (((T1-1.0))*UNITFAD).val();
        T3 = (((T2*T2))*UNITFAD).val();
        T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
        BSIM4theta0vb0 = (((T1/T4))*UNITFAD).val();
      }

    }
    else {
      BSIM4theta0vb0 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
    }

    T0 = ((((BSIM4drout*BSIM4leff)/tmp))*UNITFAD).val();
    if((T0 < 34.0)) {
      {
        T1 = ((exp(T0))*UNITFAD).val();
        T2 = (((T1-1.0))*UNITFAD).val();
        T3 = (((T2*T2))*UNITFAD).val();
        T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
        T5 = (((T1/T4))*UNITFAD).val();
      }

    }
    else {
      T5 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
    }

    BSIM4thetaRout = ((((BSIM4pdibl1*T5)+BSIM4pdibl2))*UNITFAD).val();
    tmp = ((sqrt(BSIM4Xdep0))*UNITFAD).val();
    tmp1 = (((BSIM4vbi-BSIM4phi))*UNITFAD).val();
    tmp2 = (((BSIM4factor1*tmp))*UNITFAD).val();
    T0 = (((((BSIM4dvt1w*BSIM4weff)*BSIM4leff)/tmp2))*UNITFAD).val();
    if((T0 < 34.0)) {
      {
        T1 = ((exp(T0))*UNITFAD).val();
        T2 = (((T1-1.0))*UNITFAD).val();
        T3 = (((T2*T2))*UNITFAD).val();
        T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
        T8 = (((T1/T4))*UNITFAD).val();
      }

    }
    else {
      T8 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
    }

    T0 = (((BSIM4dvt0w*T8))*UNITFAD).val();
    T8 = (((T0*tmp1))*UNITFAD).val();
    T0 = ((((BSIM4dvt1*BSIM4leff)/tmp2))*UNITFAD).val();
    if((T0 < 34.0)) {
      {
        T1 = ((exp(T0))*UNITFAD).val();
        T2 = (((T1-1.0))*UNITFAD).val();
        T3 = (((T2*T2))*UNITFAD).val();
        T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
        T9 = (((T1/T4))*UNITFAD).val();
      }

    }
    else {
      T9 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
    }

    T9 = ((((BSIM4dvt0*T9)*tmp1))*UNITFAD).val();
    T4 = ((((toxel*BSIM4phi)/(BSIM4weff+BSIM4w0)))*UNITFAD).val();
    T0 = ((sqrt((1.0+(BSIM4lpe0/BSIM4leff))))*UNITFAD).val();
    if(((BSIM4tempMod == 1) || (BSIM4tempMod == 0))) {
      T3 = ((((BSIM4kt1+(BSIM4kt1l/BSIM4leff))*(TRatio-1.0)))*UNITFAD).val();
    }

    if(((BSIM4tempMod == 2) || (BSIM4tempMod == 3))) {
      T3 = (((-BSIM4kt1*(TRatio-1.0)))*UNITFAD).val();
    }

    T5 = (((((BSIM4k1ox*(T0-1.0))*BSIM4sqrtPhi)+T3))*UNITFAD).val();
    BSIM4vfbzbfactor = (((((((-T8-T9)+(BSIM4k3*T4))+T5)-BSIM4phi)-(BSIM4k1*BSIM4sqrtPhi)))*UNITFAD).val();
    if((BSIM4wlod < 0.0)) {
      {
        printf("Warning: WLOD = %g is less than 0. 0.0 is used\n",((BSIM4wlod)*UNITFAD).val());
        BSIM4wlod = ((0.0)*UNITFAD).val();
      }

    }

    T0 = ((pow(Lnew,BSIM4llodku0))*UNITFAD).val();
    W_tmp = (((Wnew+BSIM4wlod))*UNITFAD).val();
    T1 = ((pow(W_tmp,BSIM4wlodku0))*UNITFAD).val();
    tmp1 = (((((BSIM4lku0/T0)+(BSIM4wku0/T1))+(BSIM4pku0/(T0*T1))))*UNITFAD).val();
    T9 = (((1.0+tmp1))*UNITFAD).val();
    T0 = ((pow(Lnew,BSIM4llodvth))*UNITFAD).val();
    T1 = ((pow(W_tmp,BSIM4wlodvth))*UNITFAD).val();
    tmp1 = (((((BSIM4lkvth0/T0)+(BSIM4wkvth0/T1))+(BSIM4pkvth0/(T0*T1))))*UNITFAD).val();
    param_BSIM4kvth0 = (((1.0+tmp1))*UNITFAD).val();
    param_BSIM4kvth0 = ((sqrt(((param_BSIM4kvth0*param_BSIM4kvth0)+1.0e-9)))*UNITFAD).val();
    T0 = (((TRatio-1.0))*UNITFAD).val();
    BSIM4ku0temp = ((((T9*(1.0+(BSIM4tku0*T0)))+1.0e-9))*UNITFAD).val();
    Inv_saref = (((1.0/(BSIM4saref+(0.5*Ldrn))))*UNITFAD).val();
    Inv_sbref = (((1.0/(BSIM4sbref+(0.5*Ldrn))))*UNITFAD).val();
    BSIM4inv_od_ref = (((Inv_saref+Inv_sbref))*UNITFAD).val();
    BSIM4rho_ref = ((((BSIM4ku0/BSIM4ku0temp)*BSIM4inv_od_ref))*UNITFAD).val();
    if((BSIM4mobMod == 3)) {
      {
        lt1 = (((BSIM4factor1*BSIM4sqrtXdep0))*UNITFAD).val();
        T0 = ((((BSIM4dvt1*BSIM4leff)/lt1))*UNITFAD).val();
        if((T0 < 34.0)) {
          {
            T1 = ((exp(T0))*UNITFAD).val();
            T2 = (((T1-1.0))*UNITFAD).val();
            T3 = (((T2*T2))*UNITFAD).val();
            T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
            Theta0 = (((T1/T4))*UNITFAD).val();
          }

        }
        else {
          Theta0 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
        }

        tmp1 = (((epssubl/BSIM4Xdep0))*UNITFAD).val();
        tmp2 = (((BSIM4nfactor*tmp1))*UNITFAD).val();
        tmp3 = (((((tmp2+(BSIM4cdsc*Theta0))+BSIM4cit)/BSIM4coxe))*UNITFAD).val();
        if((tmp3 >= -0.5)) {
          N0 = (((1.0+tmp3))*UNITFAD).val();
        }
        else {
          {
            T0 = (((1.0/(3.0+(8.0*tmp3))))*UNITFAD).val();
            N0 = ((((1.0+(3.0*tmp3))*T0))*UNITFAD).val();
          }

        }

        T0 = (((N0*BSIM4vtm))*UNITFAD).val();
        T1 = ((BSIM4voffcbn)*UNITFAD).val();
        T2 = (((T1/T0))*UNITFAD).val();
        if((T2 < -34.0)) {
          {
            T3 = ((((BSIM4coxe*1.713908431e-15)/BSIM4cdep0))*UNITFAD).val();
            T4 = (((BSIM4mstar+(T3*N0)))*UNITFAD).val();
          }

        }
              else if((T2 > 34.0)) {
          {
            T3 = ((((BSIM4coxe*5.834617425e14)/BSIM4cdep0))*UNITFAD).val();
            T4 = (((BSIM4mstar+(T3*N0)))*UNITFAD).val();
          }

        }
        else {
          {
            T3 = ((((exp(T2)*BSIM4coxe)/BSIM4cdep0))*UNITFAD).val();
            T4 = (((BSIM4mstar+(T3*N0)))*UNITFAD).val();
          }

        }

        BSIM4VgsteffVth = ((((T0*log(2.0))/T4))*UNITFAD).val();
      }

    }

    T0 = (((-BSIM4dvtp3*log(BSIM4leff)))*UNITFAD).val();
    if((T0 > 34.0)) {
      T1 = (((5.834617425e14*((1.0+T0)-34.0)))*UNITFAD).val();
    }
      else if((T0 < -34.0)) {
      T1 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T1 = ((exp(T0))*UNITFAD).val();
    }

    BSIM4dvtp2factor = (((BSIM4dvtp5+(BSIM4dvtp2*T1)))*UNITFAD).val();
    if((((BSIM4sa > 0.0) && (BSIM4sb > 0.0)) && ((BSIM4nf == 1.0) || ((BSIM4nf > 1.0) && (BSIM4sd > 0.0))))) {
      {
        Inv_sa = ((0.0)*UNITFAD).val();
        Inv_sb = ((0.0)*UNITFAD).val();
        if((BSIM4kvsat < -1.0)) {
          {
            printf("Warning: KVSAT = %g is too small; -1.0 is used.\n",((BSIM4kvsat)*UNITFAD).val());
            BSIM4kvsat = ((-1.0)*UNITFAD).val();
          }

        }

        if((BSIM4kvsat > 1.0)) {
          {
            printf("Warning: KVSAT = %g is too big; 1.0 is used.\n",((BSIM4kvsat)*UNITFAD).val());
            BSIM4kvsat = ((1.0)*UNITFAD).val();
          }

        }

        for(i = ((0)*UNITFAD).val();(i < BSIM4nf);i = (((i+1))*UNITFAD).val())
          {
            T0 = ((((1.0/BSIM4nf)/((BSIM4sa+(0.5*Ldrn))+(i*(BSIM4sd+Ldrn)))))*UNITFAD).val();
            T1 = ((((1.0/BSIM4nf)/((BSIM4sb+(0.5*Ldrn))+(i*(BSIM4sd+Ldrn)))))*UNITFAD).val();
            Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
            Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          }

        Inv_ODeff = (((Inv_sa+Inv_sb))*UNITFAD).val();
        rho = ((((BSIM4ku0/BSIM4ku0temp)*Inv_ODeff))*UNITFAD).val();
        T0 = ((((1.0+rho)/(1.0+BSIM4rho_ref)))*UNITFAD).val();
        here_BSIM4u0temp = (((BSIM4u0temp*T0))*UNITFAD).val();
        T1 = ((((1.0+(BSIM4kvsat*rho))/(1.0+(BSIM4kvsat*BSIM4rho_ref))))*UNITFAD).val();
        here_BSIM4vsattemp = (((BSIM4vsattemp*T1))*UNITFAD).val();
        OD_offset = (((Inv_ODeff-BSIM4inv_od_ref))*UNITFAD).val();
        dvth0_lod = ((((BSIM4kvth0/param_BSIM4kvth0)*OD_offset))*UNITFAD).val();
        dk2_lod = ((((BSIM4stk2/pow(param_BSIM4kvth0,BSIM4lodk2))*OD_offset))*UNITFAD).val();
        deta0_lod = ((((BSIM4steta0/pow(param_BSIM4kvth0,BSIM4lodeta0))*OD_offset))*UNITFAD).val();
        here_BSIM4vth0 = (((BSIM4vth0+dvth0_lod))*UNITFAD).val();
        here_BSIM4eta0 = (((BSIM4eta0+deta0_lod))*UNITFAD).val();
        here_BSIM4k2 = (((BSIM4k2+dk2_lod))*UNITFAD).val();
      }

    }
    else {
      {
        here_BSIM4u0temp = ((BSIM4u0temp)*UNITFAD).val();
        here_BSIM4vth0 = ((BSIM4vth0)*UNITFAD).val();
        here_BSIM4vsattemp = ((BSIM4vsattemp)*UNITFAD).val();
        here_BSIM4eta0 = ((BSIM4eta0)*UNITFAD).val();
        here_BSIM4k2 = ((BSIM4k2)*UNITFAD).val();
      }

    }

    if(BSIM4wpemod) {
      {
        if(((!BSIM4scaGiven && !BSIM4scbGiven) && !BSIM4sccGiven)) {
          {
            if((BSIM4scGiven && (BSIM4sc > 0.0))) {
              {
                T1 = (((BSIM4sc+Wdrn))*UNITFAD).val();
                T2 = (((1.0/BSIM4scref))*UNITFAD).val();
                BSIM4sca = ((((BSIM4scref*BSIM4scref)/(BSIM4sc*T1)))*UNITFAD).val();
                BSIM4scb = (((((((0.1*BSIM4sc)+(0.01*BSIM4scref))*exp(((-10.0*BSIM4sc)*T2)))-(((0.1*T1)+(0.01*BSIM4scref))*exp(((-10.0*T1)*T2))))/Wdrn))*UNITFAD).val();
                BSIM4scc = (((((((0.05*BSIM4sc)+(0.0025*BSIM4scref))*exp(((-20.0*BSIM4sc)*T2)))-(((0.05*T1)+(0.0025*BSIM4scref))*exp(((-20.0*T1)*T2))))/Wdrn))*UNITFAD).val();
              }

            }
            else {
              printf("Warning: No WPE as none of SCA, SCB, SCC, SC is given and/or SC not positive.\n");
            }

          }

        }

        if((BSIM4sca < 0.0)) {
          {
            printf("Warning: SCA = %g is negative. Set to 0.0.\n",((BSIM4sca)*UNITFAD).val());
            BSIM4sca = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4scb < 0.0)) {
          {
            printf("Warning: SCB = %g is negative. Set to 0.0.\n",((BSIM4scb)*UNITFAD).val());
            BSIM4scb = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4scc < 0.0)) {
          {
            printf("Warning: SCC = %g is negative. Set to 0.0.\n",((BSIM4scc)*UNITFAD).val());
            BSIM4scc = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4sc < 0.0)) {
          {
            printf("Warning: SC = %g is negative. Set to 0.0.\n",((BSIM4sc)*UNITFAD).val());
            BSIM4sc = ((0.0)*UNITFAD).val();
          }

        }

        sceff = ((((BSIM4sca+(BSIM4web*BSIM4scb))+(BSIM4wec*BSIM4scc)))*UNITFAD).val();
        here_BSIM4vth0 = (((here_BSIM4vth0+(BSIM4kvth0we*sceff)))*UNITFAD).val();
        here_BSIM4k2 = (((here_BSIM4k2+(BSIM4k2we*sceff)))*UNITFAD).val();
        T3 = (((1.0+(BSIM4ku0we*sceff)))*UNITFAD).val();
        if((T3 <= 0.0)) {
          {
            T3 = ((0.0)*UNITFAD).val();
            printf("Warning: ku0we = %g is negatively too high. Negative mobility! \n",((BSIM4ku0we)*UNITFAD).val());
          }

        }

        here_BSIM4u0temp = (((here_BSIM4u0temp*T3))*UNITFAD).val();
      }

    }

    here_BSIM4vth0 = (((here_BSIM4vth0+BSIM4delvto))*UNITFAD).val();
    here_BSIM4vfb = (((BSIM4vfb+(BSIM4type*BSIM4delvto)))*UNITFAD).val();
    T3 = (((((BSIM4type*here_BSIM4vth0)-here_BSIM4vfb)-BSIM4phi))*UNITFAD).val();
    T4 = (((T3+T3))*UNITFAD).val();
    T5 = (((2.5*T3))*UNITFAD).val();
    here_BSIM4vtfbphi1 = ((cogenda_ternary_op<CogendaFadType>((BSIM4type == 1),T4,T5))*UNITFAD).val();
    if((here_BSIM4vtfbphi1 < 0.0)) {
      here_BSIM4vtfbphi1 = ((0.0)*UNITFAD).val();
    }

    here_BSIM4vtfbphi2 = (((4.0*T3))*UNITFAD).val();
    if((here_BSIM4vtfbphi2 < 0.0)) {
      here_BSIM4vtfbphi2 = ((0.0)*UNITFAD).val();
    }

    if((here_BSIM4k2 < 0.0)) {
      {
        T0 = ((((0.5*BSIM4k1)/here_BSIM4k2))*UNITFAD).val();
        here_BSIM4vbsc = (((0.9*(BSIM4phi-(T0*T0))))*UNITFAD).val();
        if((here_BSIM4vbsc > -3.0)) {
          here_BSIM4vbsc = ((-3.0)*UNITFAD).val();
        }
              else if((here_BSIM4vbsc < -30.0)) {
          here_BSIM4vbsc = ((-30.0)*UNITFAD).val();
        }

      }

    }
    else {
      here_BSIM4vbsc = ((-30.0)*UNITFAD).val();
    }

    if((here_BSIM4vbsc > BSIM4vbm)) {
      here_BSIM4vbsc = ((BSIM4vbm)*UNITFAD).val();
    }

    here_BSIM4k2ox = ((((here_BSIM4k2*toxel)/BSIM4toxm))*UNITFAD).val();
    here_BSIM4vfbzb = (((BSIM4vfbzbfactor+(BSIM4type*here_BSIM4vth0)))*UNITFAD).val();
    lnl = ((log((BSIM4leff*1.0e6)))*UNITFAD).val();
    lnw = ((log((BSIM4weff*1.0e6)))*UNITFAD).val();
    lnnf = ((log(BSIM4nf))*UNITFAD).val();
    bodymode = ((5)*UNITFAD).val();
    if((!BSIM4rbps0Given || !BSIM4rbpd0Given)) {
      bodymode = ((1)*UNITFAD).val();
    }
      else if(((!BSIM4rbsbx0Given && !BSIM4rbsby0Given) || (!BSIM4rbdbx0Given && !BSIM4rbdby0Given))) {
      bodymode = ((3)*UNITFAD).val();
    }

    if((BSIM4rbodyMod == 2)) {
      {
        if((bodymode == 5)) {
          {
            T0 = (((BSIM4rbsbx0*exp((((BSIM4rbsdbxl*lnl)+(BSIM4rbsdbxw*lnw))+(BSIM4rbsdbxnf*lnnf)))))*UNITFAD).val();
            T1 = (((BSIM4rbsby0*exp((((BSIM4rbsdbyl*lnl)+(BSIM4rbsdbyw*lnw))+(BSIM4rbsdbynf*lnnf)))))*UNITFAD).val();
            BSIM4rbsb = ((((T0*T1)/(T0+T1)))*UNITFAD).val();
            T0 = (((BSIM4rbdbx0*exp((((BSIM4rbsdbxl*lnl)+(BSIM4rbsdbxw*lnw))+(BSIM4rbsdbxnf*lnnf)))))*UNITFAD).val();
            T1 = (((BSIM4rbdby0*exp((((BSIM4rbsdbyl*lnl)+(BSIM4rbsdbyw*lnw))+(BSIM4rbsdbynf*lnnf)))))*UNITFAD).val();
            BSIM4rbdb = ((((T0*T1)/(T0+T1)))*UNITFAD).val();
          }

        }

        if(((bodymode == 3) || (bodymode == 5))) {
          {
            BSIM4rbps = (((BSIM4rbps0*exp((((BSIM4rbpsl*lnl)+(BSIM4rbpsw*lnw))+(BSIM4rbpsnf*lnnf)))))*UNITFAD).val();
            BSIM4rbpd = (((BSIM4rbpd0*exp((((BSIM4rbpdl*lnl)+(BSIM4rbpdw*lnw))+(BSIM4rbpdnf*lnnf)))))*UNITFAD).val();
          }

        }

        T0 = (((BSIM4rbpbx0*exp((((BSIM4rbpbxl*lnl)+(BSIM4rbpbxw*lnw))+(BSIM4rbpbxnf*lnnf)))))*UNITFAD).val();
        T1 = (((BSIM4rbpby0*exp((((BSIM4rbpbyl*lnl)+(BSIM4rbpbyw*lnw))+(BSIM4rbpbynf*lnnf)))))*UNITFAD).val();
        BSIM4rbpb = ((((T0*T1)/(T0+T1)))*UNITFAD).val();
      }

    }

    if(((BSIM4rbodyMod == 1) || ((BSIM4rbodyMod == 2) && (bodymode == 5)))) {
      {
        if((BSIM4rbdb < 1.0e-3)) {
          BSIM4grbdb = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbdb = (((BSIM4gbmin+(1.0/BSIM4rbdb)))*UNITFAD).val();
        }

        if((BSIM4rbpb < 1.0e-3)) {
          BSIM4grbpb = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbpb = (((BSIM4gbmin+(1.0/BSIM4rbpb)))*UNITFAD).val();
        }

        if((BSIM4rbps < 1.0e-3)) {
          BSIM4grbps = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbps = (((BSIM4gbmin+(1.0/BSIM4rbps)))*UNITFAD).val();
        }

        if((BSIM4rbsb < 1.0e-3)) {
          BSIM4grbsb = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbsb = (((BSIM4gbmin+(1.0/BSIM4rbsb)))*UNITFAD).val();
        }

        if((BSIM4rbpd < 1.0e-3)) {
          BSIM4grbpd = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbpd = (((BSIM4gbmin+(1.0/BSIM4rbpd)))*UNITFAD).val();
        }

      }

    }

    if(((BSIM4rbodyMod == 2) && (bodymode == 3))) {
      {
        BSIM4grbdb = ((BSIM4gbmin)*UNITFAD).val();
        BSIM4grbsb = ((BSIM4gbmin)*UNITFAD).val();
        if((BSIM4rbpb < 1.0e-3)) {
          BSIM4grbpb = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbpb = (((BSIM4gbmin+(1.0/BSIM4rbpb)))*UNITFAD).val();
        }

        if((BSIM4rbps < 1.0e-3)) {
          BSIM4grbps = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbps = (((BSIM4gbmin+(1.0/BSIM4rbps)))*UNITFAD).val();
        }

        if((BSIM4rbpd < 1.0e-3)) {
          BSIM4grbpd = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbpd = (((BSIM4gbmin+(1.0/BSIM4rbpd)))*UNITFAD).val();
        }

      }

    }

    if(((BSIM4rbodyMod == 2) && (bodymode == 1))) {
      {
        BSIM4grbsb = ((BSIM4gbmin)*UNITFAD).val();
        BSIM4grbdb = ((BSIM4gbmin)*UNITFAD).val();
        BSIM4grbps = ((1.0e3)*UNITFAD).val();
        BSIM4grbpd = ((1.0e3)*UNITFAD).val();
        if((BSIM4rbpb < 1.0e-3)) {
          BSIM4grbpb = ((1.0e3)*UNITFAD).val();
        }
        else {
          BSIM4grbpb = (((BSIM4gbmin+(1.0/BSIM4rbpb)))*UNITFAD).val();
        }

      }

    }

    BSIM4grgeltd = ((((BSIM4rshg*(BSIM4xgw+((BSIM4weffCJ/3.0)/BSIM4ngcon)))/((BSIM4ngcon*BSIM4nf)*(Lnew-BSIM4xgl))))*UNITFAD).val();
    if((BSIM4grgeltd > 0.0)) {
      BSIM4grgeltd = (((1.0/BSIM4grgeltd))*UNITFAD).val();
    }
    else {
      {
        BSIM4grgeltd = ((1.0e3)*UNITFAD).val();
        if((BSIM4rgateMod != 0)) {
          printf("Warning: (instance %M) The gate conductance reset to 1.0e3 mho.\n");
        }

      }

    }

    DMCGeff = (((BSIM4dmcg-BSIM4dmcgt))*UNITFAD).val();
    DMCIeff = ((BSIM4dmci)*UNITFAD).val();
    DMDGeff = (((BSIM4dmdg-BSIM4dmcgt))*UNITFAD).val();
    if(BSIM4sourcePerimeterGiven) {
      {
        if((BSIM4sourcePerimeter == 0.0)) {
          BSIM4Pseff = ((0.0)*UNITFAD).val();
        }
              else if((BSIM4sourcePerimeter < 0.0)) {
          {
            printf("Warning: Source Perimeter is specified as negative, it is set to zero.\n");
            BSIM4Pseff = ((0.0)*UNITFAD).val();
          }

        }
        else {
          {
            if((BSIM4perMod == 0)) {
              BSIM4Pseff = ((BSIM4sourcePerimeter)*UNITFAD).val();
            }
            else {
              BSIM4Pseff = (((BSIM4sourcePerimeter-(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
            }

          }

        }

      }

    }
    else {
      {
        if((BSIM4geoMod < 9)) {
          if(((BSIM4nf%2) != 0)) {
            {
              nuEndD = ((1.0)*UNITFAD).val();
              nuEndS = ((1.0)*UNITFAD).val();
              nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
              nuIntS = ((nuIntD)*UNITFAD).val();
            }

          }
          else {
            {
              if((BSIM4min == 1)) {
                {
                  nuEndD = ((2.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                  nuEndS = ((0.0)*UNITFAD).val();
                  nuIntS = ((BSIM4nf)*UNITFAD).val();
                }

              }
              else {
                {
                  nuEndD = ((0.0)*UNITFAD).val();
                  nuIntD = ((BSIM4nf)*UNITFAD).val();
                  nuEndS = ((2.0)*UNITFAD).val();
                  nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                }

              }

            }

          }

        }

        T0 = (((DMCGeff+DMCIeff))*UNITFAD).val();
        T1 = (((DMCGeff+DMCGeff))*UNITFAD).val();
        T2 = (((DMDGeff+DMDGeff))*UNITFAD).val();
        PSiso = ((((T0+T0)+BSIM4weffCJ))*UNITFAD).val();
        PDiso = ((PSiso)*UNITFAD).val();
        PSsha = ((T1)*UNITFAD).val();
        PDsha = ((T1)*UNITFAD).val();
        PSmer = ((T2)*UNITFAD).val();
        PDmer = ((T2)*UNITFAD).val();
        ASiso = (((T0*BSIM4weffCJ))*UNITFAD).val();
        ADiso = ((ASiso)*UNITFAD).val();
        ASsha = (((DMCGeff*BSIM4weffCJ))*UNITFAD).val();
        ADsha = ((ASsha)*UNITFAD).val();
        ASmer = (((DMDGeff*BSIM4weffCJ))*UNITFAD).val();
        ADmer = ((ASmer)*UNITFAD).val();
        switch(BSIM4geoMod){
        case 0:
          {
            BSIM4Pseff = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 1:
          {
            BSIM4Pseff = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 2:
          {
            BSIM4Pseff = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 3:
          {
            BSIM4Pseff = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 4:
          {
            BSIM4Pseff = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 5:
          {
            BSIM4Pseff = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 6:
          {
            BSIM4Pseff = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 7:
          {
            BSIM4Pseff = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 8:
          {
            BSIM4Pseff = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 9:
          {
            BSIM4Pseff = (((PSiso+((BSIM4nf-1.0)*PSsha)))*UNITFAD).val();
            dumPd = (((BSIM4nf*PDsha))*UNITFAD).val();
            dumAs = (((ASiso+((BSIM4nf-1.0)*ASsha)))*UNITFAD).val();
            dumAd = (((BSIM4nf*ADsha))*UNITFAD).val();
          }
          break;
        case 10:
          {
            BSIM4Pseff = (((BSIM4nf*PSsha))*UNITFAD).val();
            dumPd = (((PDiso+((BSIM4nf-1.0)*PDsha)))*UNITFAD).val();
            dumAs = (((BSIM4nf*ASsha))*UNITFAD).val();
            dumAd = (((ADiso+((BSIM4nf-1.0)*ADsha)))*UNITFAD).val();
          }
          break;
        default:
          printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4PAeffGeo)",BSIM4geoMod);          break;
        }

      }

    }

    if((BSIM4Pseff < 0.0)) {
      {
        BSIM4Pseff = ((0.0)*UNITFAD).val();
        printf("Warning: Pseff is negative, it is set to zero.\n");
      }

    }

    if(BSIM4drainPerimeterGiven) {
      {
        if((BSIM4drainPerimeter == 0.0)) {
          BSIM4Pdeff = ((0.0)*UNITFAD).val();
        }
              else if((BSIM4drainPerimeter < 0.0)) {
          {
            printf("Warning: Drain Perimeter is specified as negative, it is set to zero.\n");
            BSIM4Pdeff = ((0.0)*UNITFAD).val();
          }

        }
        else {
          {
            if((BSIM4perMod == 0)) {
              BSIM4Pdeff = ((BSIM4drainPerimeter)*UNITFAD).val();
            }
            else {
              BSIM4Pdeff = (((BSIM4drainPerimeter-(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
            }

          }

        }

      }

    }
    else {
      {
        if((BSIM4geoMod < 9)) {
          if(((BSIM4nf%2) != 0)) {
            {
              nuEndD = ((1.0)*UNITFAD).val();
              nuEndS = ((1.0)*UNITFAD).val();
              nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
              nuIntS = ((nuIntD)*UNITFAD).val();
            }

          }
          else {
            {
              if((BSIM4min == 1)) {
                {
                  nuEndD = ((2.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                  nuEndS = ((0.0)*UNITFAD).val();
                  nuIntS = ((BSIM4nf)*UNITFAD).val();
                }

              }
              else {
                {
                  nuEndD = ((0.0)*UNITFAD).val();
                  nuIntD = ((BSIM4nf)*UNITFAD).val();
                  nuEndS = ((2.0)*UNITFAD).val();
                  nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                }

              }

            }

          }

        }

        T0 = (((DMCGeff+DMCIeff))*UNITFAD).val();
        T1 = (((DMCGeff+DMCGeff))*UNITFAD).val();
        T2 = (((DMDGeff+DMDGeff))*UNITFAD).val();
        PSiso = ((((T0+T0)+BSIM4weffCJ))*UNITFAD).val();
        PDiso = ((PSiso)*UNITFAD).val();
        PSsha = ((T1)*UNITFAD).val();
        PDsha = ((T1)*UNITFAD).val();
        PSmer = ((T2)*UNITFAD).val();
        PDmer = ((T2)*UNITFAD).val();
        ASiso = (((T0*BSIM4weffCJ))*UNITFAD).val();
        ADiso = ((ASiso)*UNITFAD).val();
        ASsha = (((DMCGeff*BSIM4weffCJ))*UNITFAD).val();
        ADsha = ((ASsha)*UNITFAD).val();
        ASmer = (((DMDGeff*BSIM4weffCJ))*UNITFAD).val();
        ADmer = ((ASmer)*UNITFAD).val();
        switch(BSIM4geoMod){
        case 0:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 1:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 2:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 3:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 4:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 5:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 6:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 7:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 8:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 9:
          {
            dumPs = (((PSiso+((BSIM4nf-1.0)*PSsha)))*UNITFAD).val();
            BSIM4Pdeff = (((BSIM4nf*PDsha))*UNITFAD).val();
            dumAs = (((ASiso+((BSIM4nf-1.0)*ASsha)))*UNITFAD).val();
            dumAd = (((BSIM4nf*ADsha))*UNITFAD).val();
          }
          break;
        case 10:
          {
            dumPs = (((BSIM4nf*PSsha))*UNITFAD).val();
            BSIM4Pdeff = (((PDiso+((BSIM4nf-1.0)*PDsha)))*UNITFAD).val();
            dumAs = (((BSIM4nf*ASsha))*UNITFAD).val();
            dumAd = (((ADiso+((BSIM4nf-1.0)*ADsha)))*UNITFAD).val();
          }
          break;
        default:
          printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4PAeffGeo)",BSIM4geoMod);          break;
        }

      }

    }

    if((BSIM4Pdeff < 0.0)) {
      {
        BSIM4Pdeff = ((0.0)*UNITFAD).val();
        printf("Warning: Pdeff is negative, it is set to zero.\n");
      }

    }

    if(BSIM4sourceAreaGiven) {
      BSIM4Aseff = ((BSIM4sourceArea)*UNITFAD).val();
    }
    else {
      {
        if((BSIM4geoMod < 9)) {
          if(((BSIM4nf%2) != 0)) {
            {
              nuEndD = ((1.0)*UNITFAD).val();
              nuEndS = ((1.0)*UNITFAD).val();
              nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
              nuIntS = ((nuIntD)*UNITFAD).val();
            }

          }
          else {
            {
              if((BSIM4min == 1)) {
                {
                  nuEndD = ((2.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                  nuEndS = ((0.0)*UNITFAD).val();
                  nuIntS = ((BSIM4nf)*UNITFAD).val();
                }

              }
              else {
                {
                  nuEndD = ((0.0)*UNITFAD).val();
                  nuIntD = ((BSIM4nf)*UNITFAD).val();
                  nuEndS = ((2.0)*UNITFAD).val();
                  nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                }

              }

            }

          }

        }

        T0 = (((DMCGeff+DMCIeff))*UNITFAD).val();
        T1 = (((DMCGeff+DMCGeff))*UNITFAD).val();
        T2 = (((DMDGeff+DMDGeff))*UNITFAD).val();
        PSiso = ((((T0+T0)+BSIM4weffCJ))*UNITFAD).val();
        PDiso = ((PSiso)*UNITFAD).val();
        PSsha = ((T1)*UNITFAD).val();
        PDsha = ((T1)*UNITFAD).val();
        PSmer = ((T2)*UNITFAD).val();
        PDmer = ((T2)*UNITFAD).val();
        ASiso = (((T0*BSIM4weffCJ))*UNITFAD).val();
        ADiso = ((ASiso)*UNITFAD).val();
        ASsha = (((DMCGeff*BSIM4weffCJ))*UNITFAD).val();
        ADsha = ((ASsha)*UNITFAD).val();
        ASmer = (((DMDGeff*BSIM4weffCJ))*UNITFAD).val();
        ADmer = ((ASmer)*UNITFAD).val();
        switch(BSIM4geoMod){
        case 0:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 1:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 2:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 3:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 4:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 5:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 6:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 7:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 8:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            BSIM4Aseff = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            dumAd = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 9:
          {
            dumPs = (((PSiso+((BSIM4nf-1.0)*PSsha)))*UNITFAD).val();
            dumPd = (((BSIM4nf*PDsha))*UNITFAD).val();
            BSIM4Aseff = (((ASiso+((BSIM4nf-1.0)*ASsha)))*UNITFAD).val();
            dumAd = (((BSIM4nf*ADsha))*UNITFAD).val();
          }
          break;
        case 10:
          {
            dumPs = (((BSIM4nf*PSsha))*UNITFAD).val();
            dumPd = (((PDiso+((BSIM4nf-1.0)*PDsha)))*UNITFAD).val();
            BSIM4Aseff = (((BSIM4nf*ASsha))*UNITFAD).val();
            dumAd = (((ADiso+((BSIM4nf-1.0)*ADsha)))*UNITFAD).val();
          }
          break;
        default:
          printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4PAeffGeo)",BSIM4geoMod);          break;
        }

      }

    }

    if((BSIM4Aseff < 0.0)) {
      {
        BSIM4Aseff = ((0.0)*UNITFAD).val();
        printf("Warning: Aseff is negative, it is set to zero.\n");
      }

    }

    if(BSIM4drainAreaGiven) {
      BSIM4Adeff = ((BSIM4drainArea)*UNITFAD).val();
    }
    else {
      {
        if((BSIM4geoMod < 9)) {
          if(((BSIM4nf%2) != 0)) {
            {
              nuEndD = ((1.0)*UNITFAD).val();
              nuEndS = ((1.0)*UNITFAD).val();
              nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
              nuIntS = ((nuIntD)*UNITFAD).val();
            }

          }
          else {
            {
              if((BSIM4min == 1)) {
                {
                  nuEndD = ((2.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                  nuEndS = ((0.0)*UNITFAD).val();
                  nuIntS = ((BSIM4nf)*UNITFAD).val();
                }

              }
              else {
                {
                  nuEndD = ((0.0)*UNITFAD).val();
                  nuIntD = ((BSIM4nf)*UNITFAD).val();
                  nuEndS = ((2.0)*UNITFAD).val();
                  nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                }

              }

            }

          }

        }

        T0 = (((DMCGeff+DMCIeff))*UNITFAD).val();
        T1 = (((DMCGeff+DMCGeff))*UNITFAD).val();
        T2 = (((DMDGeff+DMDGeff))*UNITFAD).val();
        PSiso = ((((T0+T0)+BSIM4weffCJ))*UNITFAD).val();
        PDiso = ((PSiso)*UNITFAD).val();
        PSsha = ((T1)*UNITFAD).val();
        PDsha = ((T1)*UNITFAD).val();
        PSmer = ((T2)*UNITFAD).val();
        PDmer = ((T2)*UNITFAD).val();
        ASiso = (((T0*BSIM4weffCJ))*UNITFAD).val();
        ADiso = ((ASiso)*UNITFAD).val();
        ASsha = (((DMCGeff*BSIM4weffCJ))*UNITFAD).val();
        ADsha = ((ASsha)*UNITFAD).val();
        ASmer = (((DMDGeff*BSIM4weffCJ))*UNITFAD).val();
        ADmer = ((ASmer)*UNITFAD).val();
        switch(BSIM4geoMod){
        case 0:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 1:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 2:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 3:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 4:
          {
            dumPs = ((((nuEndS*PSiso)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASiso)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 5:
          {
            dumPs = ((((nuEndS+nuIntS)*PSsha))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS+nuIntS)*ASsha))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 6:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDiso)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADiso)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 7:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD+nuIntD)*PDsha))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD+nuIntD)*ADsha))*UNITFAD).val();
          }
          break;
        case 8:
          {
            dumPs = ((((nuEndS*PSmer)+(nuIntS*PSsha)))*UNITFAD).val();
            dumPd = ((((nuEndD*PDmer)+(nuIntD*PDsha)))*UNITFAD).val();
            dumAs = ((((nuEndS*ASmer)+(nuIntS*ASsha)))*UNITFAD).val();
            BSIM4Adeff = ((((nuEndD*ADmer)+(nuIntD*ADsha)))*UNITFAD).val();
          }
          break;
        case 9:
          {
            dumPs = (((PSiso+((BSIM4nf-1.0)*PSsha)))*UNITFAD).val();
            dumPd = (((BSIM4nf*PDsha))*UNITFAD).val();
            dumAs = (((ASiso+((BSIM4nf-1.0)*ASsha)))*UNITFAD).val();
            BSIM4Adeff = (((BSIM4nf*ADsha))*UNITFAD).val();
          }
          break;
        case 10:
          {
            dumPs = (((BSIM4nf*PSsha))*UNITFAD).val();
            dumPd = (((PDiso+((BSIM4nf-1.0)*PDsha)))*UNITFAD).val();
            dumAs = (((BSIM4nf*ASsha))*UNITFAD).val();
            BSIM4Adeff = (((ADiso+((BSIM4nf-1.0)*ADsha)))*UNITFAD).val();
          }
          break;
        default:
          printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4PAeffGeo)",BSIM4geoMod);          break;
        }

      }

    }

    if((BSIM4Adeff < 0.0)) {
      {
        BSIM4Adeff = ((0.0)*UNITFAD).val();
        printf("Warning: Adeff is negative, it is set to zero.\n");
      }

    }

    BSIM4sourceConductance = ((0.0)*UNITFAD).val();
    if(BSIM4sourceSquaresGiven) {
      {
        BSIM4sourceConductance = (((BSIM4sheetResistance*BSIM4sourceSquares))*UNITFAD).val();
      }

    }
      else if((BSIM4rgeoMod > 0)) {
      {
        {
          if((BSIM4geoMod < 9)) {
            {
              if(((BSIM4nf%2) != 0)) {
                {
                  nuEndD = ((1.0)*UNITFAD).val();
                  nuEndS = ((1.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
                  nuIntS = ((nuIntD)*UNITFAD).val();
                }

              }
              else {
                {
                  if((BSIM4min == 1)) {
                    {
                      nuEndD = ((2.0)*UNITFAD).val();
                      nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                      nuEndS = ((0.0)*UNITFAD).val();
                      nuIntS = ((BSIM4nf)*UNITFAD).val();
                    }

                  }
                  else {
                    {
                      nuEndD = ((0.0)*UNITFAD).val();
                      nuIntD = ((BSIM4nf)*UNITFAD).val();
                      nuEndS = ((2.0)*UNITFAD).val();
                      nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                    }

                  }

                }

              }

              if((1 == 1)) {
                {
                  if((nuIntS == 0.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuIntS)))*UNITFAD).val();
                  }

                }

              }
              else {
                {
                  if((nuIntD == 0.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuIntD)))*UNITFAD).val();
                  }

                }

              }

            }

          }

          switch(BSIM4geoMod){
          case 0:
            {
              if((1 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 1:
            {
              if((1 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 2:
            {
              if((1 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 3:
            {
              if((1 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 4:
            {
              if((1 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
              else {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
              }

            }
            break;
          case 5:
            {
              if((1 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
              else {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
              }

            }
            break;
          case 6:
            {
              if((1 == 1)) {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 7:
            {
              if((1 == 1)) {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 8:
            {
              Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
            }
            break;
          case 9:
            {
              if((1 == 1)) {
                {
                  Rend = (((((0.5*BSIM4sheetResistance)*DMCGeff)/BSIM4weffCJ))*UNITFAD).val();
                  if((BSIM4nf == 2.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*(BSIM4nf-2.0))))*UNITFAD).val();
                  }

                }

              }
              else {
                {
                  Rend = ((0.0)*UNITFAD).val();
                  Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
                }

              }

            }
            break;
          case 10:
            {
              if((1 == 1)) {
                {
                  Rend = ((0.0)*UNITFAD).val();
                  Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
                }

              }
              else {
                {
                  Rend = (((((0.5*BSIM4sheetResistance)*DMCGeff)/BSIM4weffCJ))*UNITFAD).val();
                  if((BSIM4nf == 2.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*(BSIM4nf-2.0))))*UNITFAD).val();
                  }

                }

              }

            }
            break;
          default:
            printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4RdseffGeo)",BSIM4geoMod);            break;
          }

          if((Rint <= 0.0)) {
            BSIM4sourceConductance = ((Rend)*UNITFAD).val();
          }
                  else if((Rend <= 0.0)) {
            BSIM4sourceConductance = ((Rint)*UNITFAD).val();
          }
          else {
            BSIM4sourceConductance = ((((Rint*Rend)/(Rint+Rend)))*UNITFAD).val();
          }

          if((BSIM4sourceConductance == 0.0)) {
            printf("Warning: (instance %M) Zero resistance returned from RdseffGeo\n");
          }

        }

      }

    }
    else {
      BSIM4sourceConductance = ((0.0)*UNITFAD).val();
    }

    if((BSIM4sourceConductance > 0.0)) {
      BSIM4sourceConductance = (((1.0/BSIM4sourceConductance))*UNITFAD).val();
    }
    else {
      {
        if(((BSIM4sourceConductance == 0.0) && (((BSIM4rsh > 0 
) && (!BSIM4sourceSquaresGiven && (BSIM4rgeoMod != 0))) || (BSIM4rdsMod != 0)))) {
          {
            BSIM4sourceConductance = ((1.0e3)*UNITFAD).val();
            printf("Warning: (instance %M) Source diffusion conductance reset to 1.0e3 mho.\n");
          }

        }

      }

    }

    BSIM4drainConductance = ((0.0)*UNITFAD).val();
    if(BSIM4drainSquaresGiven) {
      BSIM4drainConductance = (((BSIM4sheetResistance*BSIM4drainSquares))*UNITFAD).val();
    }
      else if((BSIM4rgeoMod > 0)) {
      {
        {
          if((BSIM4geoMod < 9)) {
            {
              if(((BSIM4nf%2) != 0)) {
                {
                  nuEndD = ((1.0)*UNITFAD).val();
                  nuEndS = ((1.0)*UNITFAD).val();
                  nuIntD = (((2.0*max(((BSIM4nf-1.0)/2.0),0.0)))*UNITFAD).val();
                  nuIntS = ((nuIntD)*UNITFAD).val();
                }

              }
              else {
                {
                  if((BSIM4min == 1)) {
                    {
                      nuEndD = ((2.0)*UNITFAD).val();
                      nuIntD = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                      nuEndS = ((0.0)*UNITFAD).val();
                      nuIntS = ((BSIM4nf)*UNITFAD).val();
                    }

                  }
                  else {
                    {
                      nuEndD = ((0.0)*UNITFAD).val();
                      nuIntD = ((BSIM4nf)*UNITFAD).val();
                      nuEndS = ((2.0)*UNITFAD).val();
                      nuIntS = (((2.0*max(((BSIM4nf/2.0)-1.0),0.0)))*UNITFAD).val();
                    }

                  }

                }

              }

              if((0 == 1)) {
                {
                  if((nuIntS == 0.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuIntS)))*UNITFAD).val();
                  }

                }

              }
              else {
                {
                  if((nuIntD == 0.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuIntD)))*UNITFAD).val();
                  }

                }

              }

            }

          }

          switch(BSIM4geoMod){
          case 0:
            {
              if((0 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 1:
            {
              if((0 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 2:
            {
              if((0 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 3:
            {
              if((0 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 4:
            {
              if((0 == 1)) {
                if((1 == 1)) {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 2:
                    case 5:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 3:
                    case 4:
                    case 6:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }
                else {
                  {
                    switch(BSIM4rgeoMod){
                    case 1:
                    case 3:
                    case 7:
                      {
                        if((nuEndS == 0.0)) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                        }

                      }
                      break;
                    case 2:
                    case 4:
                    case 8:
                      {
                        if(((DMCGeff+DMCIeff) == 0.0)) {
                          {
                            printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                            if((DMCGeff == 0.0)) {
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                            if((DMCIeff == 0.0)) {
                              DMCIeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                        }

                        if(((nuEndS == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                          Rend = ((0.0)*UNITFAD).val();
                        }
                        else {
                          Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndS)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                        }

                      }
                      break;
                    default:
                      printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndS",BSIM4rgeoMod);                      break;
                    }

                  }

                }

              }
              else {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
              }

            }
            break;
          case 5:
            {
              if((0 == 1)) {
                {
                  if((1 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndS == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndS)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndS",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }
              else {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
              }

            }
            break;
          case 6:
            {
              if((0 == 1)) {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
              }
                          else if((0 == 1)) {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 2:
                  case 5:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 3:
                  case 4:
                  case 6:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO = %d not matched (BSIM4RdsEndIso)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }
              else {
                {
                  switch(BSIM4rgeoMod){
                  case 1:
                  case 3:
                  case 7:
                    {
                      if((nuEndD == 0.0)) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                      }

                    }
                    break;
                  case 2:
                  case 4:
                  case 8:
                    {
                      if(((DMCGeff+DMCIeff) == 0.0)) {
                        {
                          printf("(DMCGeff + DMCIeff) can not be equal to zero\n");
                          if((DMCGeff == 0.0)) {
                            DMCGeff = ((0.2e-6)*UNITFAD).val();
                          }

                          if((DMCIeff == 0.0)) {
                            DMCIeff = ((0.2e-6)*UNITFAD).val();
                          }

                        }

                      }

                      if(((nuEndD == 0.0) || ((DMCGeff+DMCIeff) == 0.0))) {
                        Rend = ((0.0)*UNITFAD).val();
                      }
                      else {
                        Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((3.0*nuEndD)*(DMCGeff+DMCIeff))))*UNITFAD).val();
                      }

                    }
                    break;
                  default:
                    printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndIso type !=1)\nuEndD",BSIM4rgeoMod);                    break;
                  }

                }

              }

            }
            break;
          case 7:
            {
              if((0 == 1)) {
                Rend = ((((BSIM4sheetResistance*DMDGeff)/(BSIM4weffCJ*nuEndS)))*UNITFAD).val();
              }
              else {
                {
                  if((0 == 1)) {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 2:
                      case 5:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 3:
                      case 4:
                      case 6:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }
                  else {
                    {
                      switch(BSIM4rgeoMod){
                      case 1:
                      case 3:
                      case 7:
                        {
                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*nuEndD)))*UNITFAD).val();
                          }

                        }
                        break;
                      case 2:
                      case 4:
                      case 8:
                        {
                          if((DMCGeff == 0.0)) {
                            {
                              printf("DMCGeff can not be equal to zero\n");
                              DMCGeff = ((0.2e-6)*UNITFAD).val();
                            }

                          }

                          if((nuEndD == 0.0)) {
                            Rend = ((0.0)*UNITFAD).val();
                          }
                          else {
                            Rend = ((((BSIM4sheetResistance*BSIM4weffCJ)/((6.0*nuEndD)*DMCGeff)))*UNITFAD).val();
                          }

                        }
                        break;
                      default:
                        printf("Warning: (instance %M) Specified RGEO=%d not matched (BSIM4RdsEndSha type !=1)\nuEndD",BSIM4rgeoMod);                        break;
                      }

                    }

                  }

                }

              }

            }
            break;
          case 8:
            {
              Rend = ((((BSIM4sheetResistance*DMDGeff)/BSIM4weffCJ))*UNITFAD).val();
            }
            break;
          case 9:
            {
              if((0 == 1)) {
                {
                  Rend = (((((0.5*BSIM4sheetResistance)*DMCGeff)/BSIM4weffCJ))*UNITFAD).val();
                  if((BSIM4nf == 2.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*(BSIM4nf-2.0))))*UNITFAD).val();
                  }

                }

              }
              else {
                {
                  Rend = ((0.0)*UNITFAD).val();
                  Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
                }

              }

            }
            break;
          case 10:
            {
              if((0 == 1)) {
                {
                  Rend = ((0.0)*UNITFAD).val();
                  Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*BSIM4nf)))*UNITFAD).val();
                }

              }
              else {
                {
                  Rend = (((((0.5*BSIM4sheetResistance)*DMCGeff)/BSIM4weffCJ))*UNITFAD).val();
                  if((BSIM4nf == 2.0)) {
                    Rint = ((0.0)*UNITFAD).val();
                  }
                  else {
                    Rint = ((((BSIM4sheetResistance*DMCGeff)/(BSIM4weffCJ*(BSIM4nf-2.0))))*UNITFAD).val();
                  }

                }

              }

            }
            break;
          default:
            printf("Warning: (instance %M) Specified GEO=%d not matched (BSIM4RdseffGeo)",BSIM4geoMod);            break;
          }

          if((Rint <= 0.0)) {
            BSIM4drainConductance = ((Rend)*UNITFAD).val();
          }
                  else if((Rend <= 0.0)) {
            BSIM4drainConductance = ((Rint)*UNITFAD).val();
          }
          else {
            BSIM4drainConductance = ((((Rint*Rend)/(Rint+Rend)))*UNITFAD).val();
          }

          if((BSIM4drainConductance == 0.0)) {
            printf("Warning: (instance %M) Zero resistance returned from RdseffGeo\n");
          }

        }

      }

    }
    else {
      BSIM4drainConductance = ((0.0)*UNITFAD).val();
    }

    if((BSIM4drainConductance > 0.0)) {
      BSIM4drainConductance = (((1.0/BSIM4drainConductance))*UNITFAD).val();
    }
    else {
      {
        if(((BSIM4drainConductance == 0.0) && (((BSIM4rsh > 0 
) && (!BSIM4drainSquaresGiven && (BSIM4rgeoMod != 0))) || (BSIM4rdsMod != 0)))) {
          {
            BSIM4drainConductance = ((1.0e3)*UNITFAD).val();
            printf("Warning: (instance %M) Drain diffusion conductance reset to 1.0e3 mho.\n");
          }

        }

      }

    }

    if((this->verbose == 1)) {
      {
        printf("BSIM4geoMod, BSIM4rgeoMod, d/s-conductance=%d,%d,%e,%e\n",BSIM4geoMod,BSIM4rgeoMod,((BSIM4drainConductance)*UNITFAD).val(),((BSIM4sourceConductance)*UNITFAD).val());
        printf("ps,pd,ad,as(given):%e,%e,%d,%e,%e,%d,%e,%e,%d,%e,%e,%d\n",((BSIM4Pseff)*UNITFAD).val(),((BSIM4sourcePerimeter)*UNITFAD).val(),BSIM4sourcePerimeterGiven,((BSIM4Pdeff)*UNITFAD).val(),((BSIM4drainPerimeter)*UNITFAD).val(),BSIM4drainPerimeterGiven,((BSIM4Adeff)*UNITFAD).val(),((BSIM4drainArea)*UNITFAD).val(),BSIM4drainAreaGiven,((BSIM4Aseff)*UNITFAD).val(),((BSIM4sourceArea)*UNITFAD).val(),BSIM4sourceAreaGiven);
      }

    }

    Nvtms = (((BSIM4vtm*BSIM4SjctEmissionCoeff))*UNITFAD).val();
    if(((BSIM4Aseff <= 0.0) && (BSIM4Pseff <= 0.0))) {
      SourceSatCurrent = ((0.0)*UNITFAD).val();
    }
    else {
      SourceSatCurrent = (((((BSIM4Aseff*BSIM4SjctTempSatCurDensity)+(BSIM4Pseff*BSIM4SjctSidewallTempSatCurDensity))+((BSIM4weffCJ*BSIM4nf)*BSIM4SjctGateSidewallTempSatCurDensity)))*UNITFAD).val();
    }

    if((SourceSatCurrent > 0.0)) {
      {
        switch(BSIM4dioMod){
        case 0:
          {
            if(((BSIM4bvs/Nvtms) > 34.0)) {
              BSIM4XExpBVS = (((BSIM4xjbvs*1.713908431e-15))*UNITFAD).val();
            }
            else {
              BSIM4XExpBVS = (((BSIM4xjbvs*exp((-BSIM4bvs/Nvtms))))*UNITFAD).val();
            }

          }
          break;
        case 1:
          {
            BSIM4vjsmFwd = ((AnalogFunctions::BSIM4DioIjthVjmEval<CogendaFadType>(Nvtms,BSIM4ijthsfwd,SourceSatCurrent,0.0))*UNITFAD).val();
            BSIM4IVjsmFwd = (((SourceSatCurrent*exp((BSIM4vjsmFwd/Nvtms))))*UNITFAD).val();
          }
          break;
        case 2:
          {
            if(((BSIM4bvs/Nvtms) > 34.0)) {
              {
                BSIM4XExpBVS = (((BSIM4xjbvs*1.713908431e-15))*UNITFAD).val();
                tmp = ((1.713908431e-15)*UNITFAD).val();
              }

            }
            else {
              {
                BSIM4XExpBVS = ((exp((-BSIM4bvs/Nvtms)))*UNITFAD).val();
                tmp = ((BSIM4XExpBVS)*UNITFAD).val();
                BSIM4XExpBVS = (((BSIM4XExpBVS*BSIM4xjbvs))*UNITFAD).val();
              }

            }

            BSIM4vjsmFwd = ((AnalogFunctions::BSIM4DioIjthVjmEval<CogendaFadType>(Nvtms,BSIM4ijthsfwd,SourceSatCurrent,BSIM4XExpBVS))*UNITFAD).val();
            T0 = ((exp((BSIM4vjsmFwd/Nvtms)))*UNITFAD).val();
            BSIM4IVjsmFwd = (((SourceSatCurrent*(((T0-(BSIM4XExpBVS/T0))+BSIM4XExpBVS)-1.0)))*UNITFAD).val();
            BSIM4SslpFwd = ((((SourceSatCurrent*(T0+(BSIM4XExpBVS/T0)))/Nvtms))*UNITFAD).val();
            T2 = (((BSIM4ijthsrev/SourceSatCurrent))*UNITFAD).val();
            if((T2 < 1.0)) {
              {
                T2 = ((10.0)*UNITFAD).val();
                printf("Warning: (instance %M) ijthsrev too small and set to 10 times IsbSat.\n");
              }

            }

            BSIM4vjsmRev = (((-BSIM4bvs-(Nvtms*log(((T2-1.0)/BSIM4xjbvs)))))*UNITFAD).val();
            T1 = (((BSIM4xjbvs*exp((-(BSIM4bvs+BSIM4vjsmRev)/Nvtms))))*UNITFAD).val();
            BSIM4IVjsmRev = (((SourceSatCurrent*(1.0+T1)))*UNITFAD).val();
            BSIM4SslpRev = ((((-SourceSatCurrent*T1)/Nvtms))*UNITFAD).val();
          }
          break;
        default:
          printf("Specified dioMod = %d not matched\n",BSIM4dioMod);          break;
        }

        Nvtmd = (((BSIM4vtm*BSIM4DjctEmissionCoeff))*UNITFAD).val();
        if(((BSIM4Adeff <= 0.0) && (BSIM4Pdeff <= 0.0))) {
          DrainSatCurrent = ((0.0)*UNITFAD).val();
        }
        else {
          DrainSatCurrent = (((((BSIM4Adeff*BSIM4DjctTempSatCurDensity)+(BSIM4Pdeff*BSIM4DjctSidewallTempSatCurDensity))+((BSIM4weffCJ*BSIM4nf)*BSIM4DjctGateSidewallTempSatCurDensity)))*UNITFAD).val();
        }

      }

    }

    if((DrainSatCurrent > 0.0)) {
      {
        switch(BSIM4dioMod){
        case 0:
          {
            if(((BSIM4bvd/Nvtmd) > 34.0)) {
              BSIM4XExpBVD = (((BSIM4xjbvd*1.713908431e-15))*UNITFAD).val();
            }
            else {
              BSIM4XExpBVD = (((BSIM4xjbvd*exp((-BSIM4bvd/Nvtmd))))*UNITFAD).val();
            }

          }
          break;
        case 1:
          {
            BSIM4vjdmFwd = ((AnalogFunctions::BSIM4DioIjthVjmEval<CogendaFadType>(Nvtmd,BSIM4ijthdfwd,DrainSatCurrent,0.0))*UNITFAD).val();
            BSIM4IVjdmFwd = (((DrainSatCurrent*exp((BSIM4vjdmFwd/Nvtmd))))*UNITFAD).val();
          }
          break;
        case 2:
          {
            if(((BSIM4bvd/Nvtmd) > 34.0)) {
              BSIM4XExpBVD = (((BSIM4xjbvd*1.713908431e-15))*UNITFAD).val();
            }
            else {
              {
                BSIM4XExpBVD = ((exp((-BSIM4bvd/Nvtmd)))*UNITFAD).val();
                BSIM4XExpBVD = (((BSIM4XExpBVD*BSIM4xjbvd))*UNITFAD).val();
              }

            }

            BSIM4vjdmFwd = ((AnalogFunctions::BSIM4DioIjthVjmEval<CogendaFadType>(Nvtmd,BSIM4ijthdfwd,DrainSatCurrent,BSIM4XExpBVD))*UNITFAD).val();
            T0 = ((exp((BSIM4vjdmFwd/Nvtmd)))*UNITFAD).val();
            BSIM4IVjdmFwd = (((DrainSatCurrent*(((T0-(BSIM4XExpBVD/T0))+BSIM4XExpBVD)-1.0)))*UNITFAD).val();
            BSIM4DslpFwd = ((((DrainSatCurrent*(T0+(BSIM4XExpBVD/T0)))/Nvtmd))*UNITFAD).val();
            T2 = (((BSIM4ijthdrev/DrainSatCurrent))*UNITFAD).val();
            if((T2 < 1.0)) {
              {
                T2 = ((10.0)*UNITFAD).val();
                printf("Warning: (instance %M) ijthdrev too small and set to 10 times IdbSat.\n");
              }

            }

            BSIM4vjdmRev = (((-BSIM4bvd-(Nvtmd*log(((T2-1.0)/BSIM4xjbvd)))))*UNITFAD).val();
            T1 = (((BSIM4xjbvd*exp((-(BSIM4bvd+BSIM4vjdmRev)/Nvtmd))))*UNITFAD).val();
            BSIM4IVjdmRev = (((DrainSatCurrent*(1.0+T1)))*UNITFAD).val();
            BSIM4DslpRev = ((((-DrainSatCurrent*T1)/Nvtmd))*UNITFAD).val();
          }
          break;
        default:
          printf("Specified dioMod = %d not matched\n",BSIM4dioMod);          break;
        }

      }

    }

    T0 = (((TRatio-1.0))*UNITFAD).val();
    BSIM4njtsstemp = (((BSIM4njts*(1.0+(BSIM4tnjts*T0))))*UNITFAD).val();
    BSIM4njtsswstemp = (((BSIM4njtssw*(1.0+(BSIM4tnjtssw*T0))))*UNITFAD).val();
    BSIM4njtsswgstemp = (((BSIM4njtsswg*(1.0+(BSIM4tnjtsswg*T0))))*UNITFAD).val();
    BSIM4njtsdtemp = (((BSIM4njtsd*(1.0+(BSIM4tnjtsd*T0))))*UNITFAD).val();
    BSIM4njtsswdtemp = (((BSIM4njtsswd*(1.0+(BSIM4tnjtsswd*T0))))*UNITFAD).val();
    BSIM4njtsswgdtemp = (((BSIM4njtsswgd*(1.0+(BSIM4tnjtsswgd*T0))))*UNITFAD).val();
    T7 = ((((Eg0/BSIM4vtm)*T0))*UNITFAD).val();
    T9 = (((BSIM4xtss*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T1 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T1 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T1 = ((exp(T9))*UNITFAD).val();
    }

    T9 = (((BSIM4xtsd*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T2 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T2 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T2 = ((exp(T9))*UNITFAD).val();
    }

    T9 = (((BSIM4xtssws*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T3 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T3 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T3 = ((exp(T9))*UNITFAD).val();
    }

    T9 = (((BSIM4xtsswd*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T4 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T4 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T4 = ((exp(T9))*UNITFAD).val();
    }

    T9 = (((BSIM4xtsswgs*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T5 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T5 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T5 = ((exp(T9))*UNITFAD).val();
    }

    T9 = (((BSIM4xtsswgd*T7))*UNITFAD).val();
    if((T9 > 34.0)) {
      T6 = (((5.834617425e14*((1.0+T9)-34.0)))*UNITFAD).val();
    }
      else if((T9 < -34.0)) {
      T6 = ((1.713908431e-15)*UNITFAD).val();
    }
    else {
      T6 = ((exp(T9))*UNITFAD).val();
    }

    if((BSIM4jtweff < 0.0)) {
      {
        BSIM4jtweff = ((0.0)*UNITFAD).val();
        printf("TAT width dependence effect is negative. Jtweff is clamped to zero.\n");
      }

    }

    T11 = (((sqrt((BSIM4jtweff/BSIM4weffCJ))+1.0))*UNITFAD).val();
    T10 = (((BSIM4weffCJ*BSIM4nf))*UNITFAD).val();
    BSIM4SjctTempRevSatCur = ((((T1*BSIM4Aseff)*BSIM4jtss))*UNITFAD).val();
    BSIM4DjctTempRevSatCur = ((((T2*BSIM4Adeff)*BSIM4jtsd))*UNITFAD).val();
    BSIM4SswTempRevSatCur = ((((T3*BSIM4Pseff)*BSIM4jtssws))*UNITFAD).val();
    BSIM4DswTempRevSatCur = ((((T4*BSIM4Pdeff)*BSIM4jtsswd))*UNITFAD).val();
    BSIM4SswgTempRevSatCur = (((((T5*T10)*T11)*BSIM4jtsswgs))*UNITFAD).val();
    BSIM4DswgTempRevSatCur = (((((T6*T10)*T11)*BSIM4jtsswgd))*UNITFAD).val();
    if(((BSIM4mtrlmod != 0) && (BSIM4mtrlcompatmod == 0))) {
      {
        Vtm0eot = ((((1.3806503e-23/1.602176462e-19)*BSIM4tempeot))*UNITFAD).val();
        Vtmeot = ((Vtm0eot)*UNITFAD).val();
        vbieot = (((Vtm0eot*log(((BSIM4nsd*BSIM4ndep)/(ni*ni)))))*UNITFAD).val();
        phieot = (((((Vtm0eot*log((BSIM4ndep/ni)))+BSIM4phin)+0.4))*UNITFAD).val();
        tmp2 = (((BSIM4vfb+phieot))*UNITFAD).val();
        vddeotl = (((BSIM4type*BSIM4vddeot))*UNITFAD).val();
        T0 = (((BSIM4epsrgate*8.85418e-12))*UNITFAD).val();
        if(((((BSIM4ngate > 1.0e18) && (BSIM4ngate < 1.0e25)) && (vddeotl > tmp2)) && (T0 != 0))) {
          {
            T1 = ((((((1.0e6*1.602176462e-19)*T0)*BSIM4ngate)/(BSIM4coxe*BSIM4coxe)))*UNITFAD).val();
            T8 = (((vddeotl-tmp2))*UNITFAD).val();
            T4 = ((sqrt((1.0+((2.0*T8)/T1))))*UNITFAD).val();
            T2 = ((((2.0*T8)/(T4+1.0)))*UNITFAD).val();
            T3 = (((((0.5*T2)*T2)/T1))*UNITFAD).val();
            T7 = ((((1.12-T3)-0.05))*UNITFAD).val();
            T6 = ((sqrt(((T7*T7)+0.224)))*UNITFAD).val();
            T5 = (((1.12-(0.5*(T7+T6))))*UNITFAD).val();
            Vgs_eff = (((vddeotl-T5))*UNITFAD).val();
          }

        }
        else {
          Vgs_eff = ((vddeotl)*UNITFAD).val();
        }

        V0 = (((vbieot-phieot))*UNITFAD).val();
        lt1 = (((BSIM4factor1*BSIM4sqrtXdep0))*UNITFAD).val();
        ltw = ((lt1)*UNITFAD).val();
        T0 = ((((BSIM4dvt1*BSIM4leffeot)/lt1))*UNITFAD).val();
        if((T0 < 34.0)) {
          {
            T1 = ((exp(T0))*UNITFAD).val();
            T2 = (((T1-1.0))*UNITFAD).val();
            T3 = (((T2*T2))*UNITFAD).val();
            T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
            Theta0 = (((T1/T4))*UNITFAD).val();
          }

        }
        else {
          Theta0 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
        }

        Delt_vth = ((((BSIM4dvt0*Theta0)*V0))*UNITFAD).val();
        T0 = (((((BSIM4dvt1w*BSIM4weffeot)*BSIM4leffeot)/ltw))*UNITFAD).val();
        if((T0 < 34.0)) {
          {
            T1 = ((exp(T0))*UNITFAD).val();
            T2 = (((T1-1.0))*UNITFAD).val();
            T3 = (((T2*T2))*UNITFAD).val();
            T4 = (((T3+((2.0*T1)*1.713908431e-15)))*UNITFAD).val();
            T5 = (((T1/T4))*UNITFAD).val();
          }

        }
        else {
          T5 = (((1.0/(5.834617425e14-2.0)))*UNITFAD).val();
        }

        T2 = ((((BSIM4dvt0w*T5)*V0))*UNITFAD).val();
        TempRatioeot = ((((BSIM4tempeot/BSIM4tnom)-1.0))*UNITFAD).val();
        T0 = ((sqrt((1.0+(BSIM4lpe0/BSIM4leffeot))))*UNITFAD).val();
        T1 = (((((BSIM4k1ox*(T0-1.0))*sqrt(phieot))+((BSIM4kt1+(BSIM4kt1l/BSIM4leffeot))*TempRatioeot)))*UNITFAD).val();
        Vth_NarrowW = ((((toxel*phieot)/(BSIM4weffeot+BSIM4w0)))*UNITFAD).val();
        Lpe_Vb = ((sqrt((1.0+(BSIM4lpeb/BSIM4leffeot))))*UNITFAD).val();
        Vth = ((((((((BSIM4type*BSIM4vth0)+(((BSIM4k1ox-BSIM4k1)*sqrt(phieot))*Lpe_Vb))-Delt_vth)-T2)+(BSIM4k3*Vth_NarrowW))+T1))*UNITFAD).val();
        tmp1 = (((epssubl/BSIM4Xdep0))*UNITFAD).val();
        tmp2 = (((BSIM4nfactor*tmp1))*UNITFAD).val();
        tmp3 = (((((tmp2+(BSIM4cdsc*Theta0))+BSIM4cit)/BSIM4coxe))*UNITFAD).val();
        if((tmp3 >= -0.5)) {
          n = (((1.0+tmp3))*UNITFAD).val();
        }
        else {
          {
            T0 = (((1.0/(3.0+(8.0*tmp3))))*UNITFAD).val();
            n = ((((1.0+(3.0*tmp3))*T0))*UNITFAD).val();
          }

        }

        if((BSIM4dvtp0 > 0.0)) {
          {
            T3 = (((BSIM4leffeot+(BSIM4dvtp0*2.0)))*UNITFAD).val();
            if((BSIM4tempMod < 2)) {
              T4 = (((Vtmeot*log((BSIM4leffeot/T3))))*UNITFAD).val();
            }
            else {
              T4 = (((Vtm0eot*log((BSIM4leffeot/T3))))*UNITFAD).val();
            }

            Vth = (((Vth-(n*T4)))*UNITFAD).val();
          }

        }

        Vgsteff = (((Vgs_eff-Vth))*UNITFAD).val();
        T3 = (((((BSIM4type*BSIM4vth0)-BSIM4vfb)-phieot))*UNITFAD).val();
        T4 = (((T3+T3))*UNITFAD).val();
        T5 = (((2.5*T3))*UNITFAD).val();
        vtfbphi2eot = (((4.0*T3))*UNITFAD).val();
        if((vtfbphi2eot < 0.0)) {
          vtfbphi2eot = ((0.0)*UNITFAD).val();
        }

        niter = ((0)*UNITFAD).val();
        toxpf = ((toxel)*UNITFAD).val();
        toxpi = ((0.0)*UNITFAD).val();
        while(((niter <= 4) && (fabs((toxpf-toxpi)) > 1e-12)))
          {
            toxpi = ((toxpf)*UNITFAD).val();
            tmp2 = (((2.0e8*toxpf))*UNITFAD).val();
            T0 = ((((Vgsteff+vtfbphi2eot)/tmp2))*UNITFAD).val();
            T1 = (((1.0+exp(((BSIM4bdos*0.7)*log(T0)))))*UNITFAD).val();
            Tcen = ((((BSIM4ados*1.9e-9)/T1))*UNITFAD).val();
            toxpf = (((toxel-((epsroxl/BSIM4epsrsub)*Tcen)))*UNITFAD).val();
            niter = (((niter+1))*UNITFAD).val();
            if((this->verbose == 1)) {
              printf("niter,toxpf,toxpi=%g,%g,%g\n",niter,((toxpf)*UNITFAD).val(),((toxpi)*UNITFAD).val());
            }

          }

        BSIM4toxp = ((toxpf)*UNITFAD).val();
        BSIM4coxp = ((((epsroxl*8.85418e-12)/BSIM4toxp))*UNITFAD).val();
      }

    }

    if((this->verbose == 1)) {
      {
        printf("New model param check starts...\n");
        printf("Topology Selection: rdsmod=%d rgatemod=%d rbodymod=%d\n",BSIM4rdsMod,BSIM4rgateMod,BSIM4rbodyMod);
      }

    }

    if(((BSIM4version-4.8) > 0.0001)) {
      printf("\nWarning: (instance %M) The latest version is BSIM4.8.0; you specified a wrong version number.\n");
    }

    if(((BSIM4rgateMod == 2) || (BSIM4rgateMod == 3))) {
      {
        if(((BSIM4trnqsMod == 1) || (BSIM4acnqsMod == 1))) {
          printf("Warning: (instance %M) You've selected both Rg and charge deficit NQS; select one only.\n");
        }

      }

    }

    if((BSIM4toxe <= 0.0)) {
      {
        printf("Fatal: Toxe = %g is not positive.\n",((BSIM4toxe)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4toxp <= 0.0)) {
      {
        printf("Fatal: Toxp = %g is not positive.\n",((BSIM4toxp)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4eot <= 0.0)) {
      {
        printf("Fatal: EOT = %g is not positive.\n",((BSIM4eot)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4epsrgate < 0.0)) {
      {
        printf("Fatal: Epsrgate = %g is not positive.\n",((BSIM4epsrgate)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4epsrsub < 0.0)) {
      {
        printf("Fatal: Epsrsub = %g is not positive.\n",((BSIM4epsrsub)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4easub < 0.0)) {
      {
        printf("Fatal: Easub = %g is not positive.\n",((BSIM4easub)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ni0sub <= 0.0)) {
      {
        printf("Fatal: Easub = %g is not positive.\n",((BSIM4ni0sub)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4toxm <= 0.0)) {
      {
        printf("Fatal: Toxm = %g is not positive.\n",((BSIM4toxm)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4toxref <= 0.0)) {
      {
        printf("Fatal: Toxref = %g is not positive.\n",((BSIM4toxref)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4lpe0 < -BSIM4leff)) {
      {
        printf("Fatal: Lpe0 = %g is less than -Leff.\n",((BSIM4lpe0)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4lpeb < -BSIM4leff)) {
      {
        printf("Fatal: Lpeb = %g is less than -Leff.\n",((BSIM4lpeb)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4phin < -0.4)) {
      {
        printf("Fatal: Phin = %g is less than -0.4.\n",((BSIM4phin)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ndep <= 0.0)) {
      {
        printf("Fatal: Ndep = %g is not positive.\n",((BSIM4ndep)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4phi <= 0.0)) {
      {
        printf("Fatal: Phi = %g is not positive. Please check Phin and Ndep\n",((BSIM4phi)*UNITFAD).val());
        printf("	   Phin = %g  Ndep = %g \n",((BSIM4phin)*UNITFAD).val(),((BSIM4ndep)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4nsub <= 0.0)) {
      {
        printf("Fatal: Nsub = %g is not positive.\n",((BSIM4nsub)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ngate < 0.0)) {
      {
        printf("Fatal: Ngate = %g Ngate is not positive.\n",((BSIM4ngate)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ngate > 1.0e25)) {
      {
        printf("Fatal: Ngate = %g Ngate is too high\n",((BSIM4ngate)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4xj <= 0.0)) {
      {
        printf("Fatal: Xj = %g is not positive.\n",((BSIM4xj)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4dvt1 < 0.0)) {
      {
        printf("Fatal: Dvt1 = %g is negative.\n",((BSIM4dvt1)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4dvt1w < 0.0)) {
      {
        printf("Fatal: Dvt1w = %g is negative.\n",((BSIM4dvt1w)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4w0 == -BSIM4weff)) {
      {
        printf("Fatal: (W0 + Weff) = 0 causing divided-by-zero.\n");
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4dsub < 0.0)) {
      {
        printf("Fatal: Dsub = %g is negative.\n",((BSIM4dsub)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4b1 == -BSIM4weff)) {
      {
        printf("Fatal: (B1 + Weff) = 0 causing divided-by-zero.\n");
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((here_BSIM4u0temp <= 0.0)) {
      {
        printf("Fatal: u0 at current temperature = %g is not positive.\n",((here_BSIM4u0temp)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4delta < 0.0)) {
      {
        printf("Fatal: Delta = %g is less than zero.\n",((BSIM4delta)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((here_BSIM4vsattemp <= 0.0)) {
      {
        printf("Fatal: Vsat at current temperature = %g is not positive.\n",((here_BSIM4vsattemp)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4pclm <= 0.0)) {
      {
        printf("Fatal: Pclm = %g is not positive.\n",((BSIM4pclm)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4drout < 0.0)) {
      {
        printf("Fatal: Drout = %g is negative.\n",((BSIM4drout)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4pscbe2 <= 0.0)) {
      {
        printf("Warning: (instance %M) Pscbe2 = %g is not positive.\n",((BSIM4pscbe2)*UNITFAD).val());
      }

    }

    if((BSIM4nf < 1.0)) {
      {
        printf("Fatal: Number of finger = %g is smaller than one.\n",BSIM4nf);
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((((BSIM4sa > 0.0) && (BSIM4sb > 0.0)) && ((BSIM4nf == 1.0) || ((BSIM4nf > 1.0) && (BSIM4sd > 0.0))))) {
      {
        if((BSIM4saref <= 0.0)) {
          {
            printf("Fatal: SAref = %g is not positive.\n",((BSIM4saref)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4sbref <= 0.0)) {
          {
            printf("Fatal: SBref = %g is not positive.\n",((BSIM4sbref)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

      }

    }

    if(((BSIM4l+BSIM4xl) <= BSIM4xgl)) {
      {
        printf("Fatal: The parameter xgl must be smaller than Ldrawn+XL.\n");
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ngcon < 1.0)) {
      {
        printf("Fatal: The parameter ngcon cannot be smaller than one.\n");
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if(((BSIM4ngcon != 1.0) && (BSIM4ngcon != 2.0))) {
      {
        BSIM4ngcon = ((1.0)*UNITFAD).val();
        printf("Warning: (instance %M) Ngcon must be equal to one or two; reset to 1.0.\n");
      }

    }

    if((BSIM4gbmin < 1.0e-20)) {
      printf("Warning: (instance %M) Gbmin = %g is too small.\n",((BSIM4gbmin)*UNITFAD).val());
    }

    if((BSIM4noff < 0.1)) {
      printf("Warning: (instance %M) Noff = %g is too small.\n",((BSIM4noff)*UNITFAD).val());
    }

    if((BSIM4noff > 4.0)) {
      printf("Warning: (instance %M) Noff = %g is too large.\n",((BSIM4noff)*UNITFAD).val());
    }

    if((BSIM4voffcv < -0.5)) {
      printf("Warning: (instance %M) Voffcv = %g is too small.\n",((BSIM4voffcv)*UNITFAD).val());
    }

    if((BSIM4voffcv > 0.5)) {
      printf("Warning: (instance %M) Voffcv = %g is too large.\n",((BSIM4voffcv)*UNITFAD).val());
    }

    if((BSIM4clc < 0.0)) {
      {
        printf("Fatal: Clc = %g is negative.\n",((BSIM4clc)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4ckappas < 0.02)) {
      {
        printf("Warning: ckappas = %g is too small.\n",((BSIM4ckappas)*UNITFAD).val());
        BSIM4ckappas = ((0.02)*UNITFAD).val();
      }

    }

    if((BSIM4ckappad < 0.02)) {
      {
        printf("Warning: ckappad = %g is too small.\n",((BSIM4ckappad)*UNITFAD).val());
        BSIM4ckappad = ((0.02)*UNITFAD).val();
      }

    }

    if((BSIM4vtss < 0.0)) {
      {
        printf("Fatal: Vtss = %g is negative.\n",((BSIM4vtss)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4vtsd < 0.0)) {
      {
        printf("Fatal: Vtsd = %g is negative.\n",((BSIM4vtsd)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4vtssws < 0.0)) {
      {
        printf("Fatal: Vtssws = %g is negative.\n",((BSIM4vtssws)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4vtsswd < 0.0)) {
      {
        printf("Fatal: Vtsswd = %g is negative.\n",((BSIM4vtsswd)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4vtsswgs < 0.0)) {
      {
        printf("Fatal: Vtsswgs = %g is negative.\n",((BSIM4vtsswgs)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4vtsswgd < 0.0)) {
      {
        printf("Fatal: Vtsswgd = %g is negative.\n",((BSIM4vtsswgd)*UNITFAD).val());
        Fatal_Flag = ((1)*UNITFAD).val();
      }

    }

    if((BSIM4moin < 5.0)) {
      printf("Warning: (instance %M) Moin = %g is too small.\n",((BSIM4moin)*UNITFAD).val());
    }

    if((BSIM4moin > 25.0)) {
      printf("Warning: (instance %M) Moin = %g is too large.\n",((BSIM4moin)*UNITFAD).val());
    }

    if((BSIM4capMod == 2)) {
      {
        if((BSIM4acde < 0.1)) {
          printf("Warning: (instance %M) Acde = %g is too small.\n",((BSIM4acde)*UNITFAD).val());
        }

        if((BSIM4acde > 1.6)) {
          printf("Warning: (instance %M) Acde = %g is too large.\n",((BSIM4acde)*UNITFAD).val());
        }

      }

    }

    if((BSIM4paramChk == 1)) {
      {
        if((BSIM4leff <= 1.0e-9)) {
          printf("Warning: (instance %M) Leff = %g <= 1.0e-9. Recommended Leff >= 1e-8 \n",((BSIM4leff)*UNITFAD).val());
        }

        if((BSIM4leffCV <= 1.0e-9)) {
          printf("Warning: (instance %M) Leff for CV = %g <= 1.0e-9. Recommended LeffCV >=1e-8 \n",((BSIM4leffCV)*UNITFAD).val());
        }

        if((BSIM4weff <= 1.0e-9)) {
          printf("Warning: (instance %M) Weff = %g <= 1.0e-9. Recommended Weff >=1e-7 \n",((BSIM4weff)*UNITFAD).val());
        }

        if((BSIM4weffCV <= 1.0e-9)) {
          printf("Warning: (instance %M) Weff for CV = %g <= 1.0e-9. Recommended WeffCV >= 1e-7 \n",((BSIM4weffCV)*UNITFAD).val());
        }

        if((BSIM4toxe < 1.0e-10)) {
          printf("Warning: (instance %M) Toxe = %g is less than 1A. Recommended Toxe >= 5A\n",((BSIM4toxe)*UNITFAD).val());
        }

        if((BSIM4toxp < 1.0e-10)) {
          printf("Warning: (instance %M) Toxp = %g is less than 1A. Recommended Toxp >= 5A\n",((BSIM4toxp)*UNITFAD).val());
        }

        if((BSIM4toxm < 1.0e-10)) {
          printf("Warning: (instance %M) Toxm = %g is less than 1A. Recommended Toxm >= 5A\n",((BSIM4toxm)*UNITFAD).val());
        }

        if((BSIM4ndep <= 1.0e12)) {
          printf("Warning: (instance %M) Ndep = %g may be too small.\n",((BSIM4ndep)*UNITFAD).val());
        }
              else if((BSIM4ndep >= 1.0e21)) {
          printf("Warning: (instance %M) Ndep = %g may be too large.\n",((BSIM4ndep)*UNITFAD).val());
        }

        if((BSIM4nsub <= 1.0e14)) {
          printf("Warning: (instance %M) Nsub = %g may be too small.\n",((BSIM4nsub)*UNITFAD).val());
        }
              else if((BSIM4nsub >= 1.0e21)) {
          printf("Warning: (instance %M) Nsub = %g may be too large.\n",((BSIM4nsub)*UNITFAD).val());
        }

        if(((BSIM4ngate > 0.0) && (BSIM4ngate <= 1.0e18))) {
          printf("Warning: (instance %M) Ngate = %g is less than 1.E18cm^-3.\n",((BSIM4ngate)*UNITFAD).val());
        }

        if((BSIM4dvt0 < 0.0)) {
          printf("Warning: (instance %M) Dvt0 = %g is negative.\n",((BSIM4dvt0)*UNITFAD).val());
        }

        if((fabs((1.0e-6/(BSIM4w0+BSIM4weff))) > 10.0)) {
          printf("Warning: (instance %M) (W0 + Weff) may be too small.\n");
        }

        if((BSIM4nfactor < 0.0)) {
          {
            printf("Warning: (instance %M) Nfactor = %g is negative.\n",((BSIM4nfactor)*UNITFAD).val());
          }

        }

        if((BSIM4cdsc < 0.0)) {
          {
            printf("Warning: (instance %M) Cdsc = %g is negative.\n",((BSIM4cdsc)*UNITFAD).val());
          }

        }

        if((BSIM4cdscd < 0.0)) {
          {
            printf("Warning: (instance %M) Cdscd = %g is negative.\n",((BSIM4cdscd)*UNITFAD).val());
          }

        }

        if((BSIM4eta0 < 0.0)) {
          {
            printf("Warning: (instance %M) Eta0 = %g is negative.\n",((BSIM4eta0)*UNITFAD).val());
          }

        }

        if((fabs((1.0e-6/(BSIM4b1+BSIM4weff))) > 10.0)) {
          {
            printf("Warning: (instance %M) (B1 + Weff) may be too small.\n");
          }

        }

        if((BSIM4a2 < 0.01)) {
          {
            printf("Warning: (instance %M) A2 = %g is too small. Set to 0.01.\n",((BSIM4a2)*UNITFAD).val());
            BSIM4a2 = ((0.01)*UNITFAD).val();
          }

        }
              else if((BSIM4a2 > 1.0)) {
          {
            printf("Warning: (instance %M) A2 = %g is larger than 1. A2 is set to 1 and A1 is set to 0.\n",((BSIM4a2)*UNITFAD).val());
            BSIM4a2 = ((1.0)*UNITFAD).val();
            BSIM4a1 = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4prwg < 0.0)) {
          {
            printf("Warning: (instance %M) Prwg = %g is negative. Set to zero.\n",((BSIM4prwg)*UNITFAD).val());
            BSIM4prwg = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4rdsw < 0.0)) {
          {
            printf("Warning: (instance %M) Rdsw = %g is negative. Set to zero.\n",((BSIM4rdsw)*UNITFAD).val());
            BSIM4rdsw = ((0.0)*UNITFAD).val();
            BSIM4rds0 = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4rds0 < 0.0)) {
          {
            printf("Warning: (instance %M) Rds at current temperature = %g is negative. Set to zero.\n",((BSIM4rds0)*UNITFAD).val());
            BSIM4rds0 = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4rdswmin < 0.0)) {
          {
            printf("Warning: (instance %M) Rdswmin at current temperature = %g is negative. Set to zero.\n",((BSIM4rdswmin)*UNITFAD).val());
            BSIM4rdswmin = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4vsattemp < 1.0e3)) {
          {
            printf("Warning: (instance %M) Vsat at current temperature = %g may be too small.\n",((BSIM4vsattemp)*UNITFAD).val());
          }

        }

        if((BSIM4fprout < 0.0)) {
          {
            printf("Fatal: fprout = %g is negative.\n",((BSIM4fprout)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4pdits < 0.0)) {
          {
            printf("Fatal: pdits = %g is negative.\n",((BSIM4pdits)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4pditsl < 0.0)) {
          {
            printf("Fatal: pditsl = %g is negative.\n",((BSIM4pditsl)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4pdibl1 < 0.0)) {
          {
            printf("Warning: (instance %M) Pdibl1 = %g is negative.\n",((BSIM4pdibl1)*UNITFAD).val());
          }

        }

        if((BSIM4pdibl2 < 0.0)) {
          {
            printf("Warning: (instance %M) Pdibl2 = %g is negative.\n",((BSIM4pdibl2)*UNITFAD).val());
          }

        }

        if((BSIM4nigbinv <= 0.0)) {
          {
            printf("Fatal: nigbinv = %g is non-positive.\n",((BSIM4nigbinv)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4nigbacc <= 0.0)) {
          {
            printf("Fatal: nigbacc = %g is non-positive.\n",((BSIM4nigbacc)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4nigc <= 0.0)) {
          {
            printf("Fatal: nigc = %g is non-positive.\n",((BSIM4nigc)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4poxedge <= 0.0)) {
          {
            printf("Fatal: poxedge = %g is non-positive.\n",((BSIM4poxedge)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4pigcd <= 0.0)) {
          {
            printf("Fatal: pigcd = %g is non-positive.\n",((BSIM4pigcd)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((((BSIM4sa > 0.0) && (BSIM4sb > 0.0)) && ((BSIM4nf == 1.0) || ((BSIM4nf > 1.0) && (BSIM4sd > 0.0))))) {
          {
            if((BSIM4lodk2 <= 0.0)) {
              {
                printf("Warning: LODK2 = %g is not positive.\n",((BSIM4lodk2)*UNITFAD).val());
              }

            }

            if((BSIM4lodeta0 <= 0.0)) {
              {
                printf("Warning: LODETA0 = %g is not positive.\n",((BSIM4lodeta0)*UNITFAD).val());
              }

            }

          }

        }

        if((BSIM4rgateMod == 1)) {
          {
            if((BSIM4rshg <= 0.0)) {
              printf("Warning: rshg should be positive for rgateMod = 1.\n");
            }

          }

        }
              else if((BSIM4rgateMod == 2)) {
          {
            if((BSIM4rshg <= 0.0)) {
              printf("Warning: rshg <= 0.0 for rgateMod = 2.\n");
            }
                      else if((BSIM4xrcrg1 <= 0.0)) {
              printf("Warning: xrcrg1 <= 0.0 for rgateMod = 2.\n");
            }

          }

        }

        if((BSIM4rgateMod == 3)) {
          {
            if((BSIM4rshg <= 0.0)) {
              printf("Warning: rshg should be positive for rgateMod = 3.\n");
            }
                      else if((BSIM4xrcrg1 <= 0.0)) {
              printf("Warning: xrcrg1 should be positive for rgateMod = 3.\n");
            }

          }

        }

        if((BSIM4rbps0 <= 0.0)) {
          {
            printf("Fatal: RBPS0 = %g is not positive.\n",((BSIM4rbps0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbpd0 <= 0.0)) {
          {
            printf("Fatal: RBPD0 = %g is not positive.\n",((BSIM4rbpd0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbpbx0 <= 0.0)) {
          {
            printf("Fatal: RBPBX0 = %g is not positive.\n",((BSIM4rbpbx0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbpby0 <= 0.0)) {
          {
            printf("Fatal: RBPBY0 = %g is not positive.\n",((BSIM4rbpby0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbdbx0 <= 0.0)) {
          {
            printf("Fatal: RBDBX0 = %g is not positive.\n",((BSIM4rbdbx0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbdby0 <= 0.0)) {
          {
            printf("Fatal: RBDBY0 = %g is not positive.\n",((BSIM4rbdby0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbsbx0 <= 0.0)) {
          {
            printf("Fatal: RBSBX0 = %g is not positive.\n",((BSIM4rbsbx0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4rbsby0 <= 0.0)) {
          {
            printf("Fatal: RBSBY0 = %g is not positive.\n",((BSIM4rbsby0)*UNITFAD).val());
            Fatal_Flag = ((1)*UNITFAD).val();
          }

        }

        if((BSIM4noff < 0.1)) {
          printf("Warning: Noff = %g is too small.\n",((BSIM4noff)*UNITFAD).val());
        }

        if((BSIM4voffcv < -0.5)) {
          printf("Warning: Voffcv = %g is too small.\n",((BSIM4voffcv)*UNITFAD).val());
        }

        if((BSIM4moin < 5.0)) {
          printf("Warning: Moin = %g is too small.\n",((BSIM4moin)*UNITFAD).val());
        }

        if((BSIM4moin > 25.0)) {
          printf("Warning: Moin = %g is too large.\n",((BSIM4moin)*UNITFAD).val());
        }

        if((BSIM4capMod == 2)) {
          {
            if((BSIM4acde < 0.1)) {
              printf("Warning: Acde = %g is too small.\n",((BSIM4acde)*UNITFAD).val());
            }

            if((BSIM4acde > 1.6)) {
              printf("Warning: Acde = %g is too large.\n",((BSIM4acde)*UNITFAD).val());
            }

          }

        }

        if((BSIM4cgdo < 0.0)) {
          {
            printf("Warning: (instance %M) cgdo = %g is negative. Set to zero.\n",((BSIM4cgdo)*UNITFAD).val());
            BSIM4cgdo = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4cgso < 0.0)) {
          {
            printf("Warning: (instance %M) cgso = %g is negative. Set to zero.\n",((BSIM4cgso)*UNITFAD).val());
            BSIM4cgso = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4tnoia < 0.0)) {
          {
            printf("Warning: (instance %M) tnoia = %g is negative. Set to zero.\n",((BSIM4tnoia)*UNITFAD).val());
            BSIM4tnoia = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4tnoib < 0.0)) {
          {
            printf("Warning: (instance %M) tnoib = %g is negative. Set to zero.\n",((BSIM4tnoib)*UNITFAD).val());
            BSIM4tnoib = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4ntnoi < 0.0)) {
          {
            printf("Warning: (instance %M) ntnoi = %g is negative. Set to zero.\n",((BSIM4ntnoi)*UNITFAD).val());
            BSIM4ntnoi = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4SjctEmissionCoeff < 0.1)) {
          {
            printf("Warning: Njs = %g is less than 0.1. Setting Njs to 0.1.\n",((BSIM4SjctEmissionCoeff)*UNITFAD).val());
            BSIM4SjctEmissionCoeff = ((0.1)*UNITFAD).val();
          }

        }
              else if((BSIM4SjctEmissionCoeff < 0.7)) {
          {
            printf("Warning: Njs = %g is less than 0.7.\n",((BSIM4SjctEmissionCoeff)*UNITFAD).val());
          }

        }

        if((BSIM4DjctEmissionCoeff < 0.1)) {
          {
            printf("Warning: Njd = %g is less than 0.1. Setting Njd to 0.1.\n",((BSIM4DjctEmissionCoeff)*UNITFAD).val());
            BSIM4DjctEmissionCoeff = ((0.1)*UNITFAD).val();
          }

        }
              else if((BSIM4DjctEmissionCoeff < 0.7)) {
          {
            printf("Warning: Njd = %g is less than 0.7.\n",((BSIM4DjctEmissionCoeff)*UNITFAD).val());
          }

        }

        if((BSIM4njtsstemp < 0.0)) {
          {
            printf("Warning: Njts = %g is negative at temperature = %g.\n",((BSIM4njtsstemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4njtsswstemp < 0.0)) {
          {
            printf("Warning: Njtssw = %g is negative at temperature = %g.\n",((BSIM4njtsswstemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4njtsswgstemp < 0.0)) {
          {
            printf("Warning: Njtsswg = %g is negative at temperature = %g.\n",((BSIM4njtsswgstemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4njtsdGiven && (BSIM4njtsdtemp < 0.0))) {
          {
            printf("Warning: Njtsd = %g is negative at temperature = %g.\n",((BSIM4njtsdtemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4njtsswdGiven && (BSIM4njtsswdtemp < 0.0))) {
          {
            printf("Warning: Njtsswd = %g is negative at temperature = %g.\n",((BSIM4njtsswdtemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4njtsswgdGiven && (BSIM4njtsswgdtemp < 0.0))) {
          {
            printf("Warning: Njtsswgd = %g is negative at temperature = %g.\n",((BSIM4njtsswgdtemp)*UNITFAD).val(),((T)*UNITFAD).val());
          }

        }

        if((BSIM4ntnoi < 0.0)) {
          {
            printf("Warning: ntnoi = %g is negative. Set to zero.\n",((BSIM4ntnoi)*UNITFAD).val());
            BSIM4ntnoi = ((0.0)*UNITFAD).val();
          }

        }

        if((BSIM4SbulkJctBotGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJS = %g is too big. Set to 0.99.\n",((BSIM4SbulkJctBotGradingCoeff)*UNITFAD).val());
            BSIM4SbulkJctBotGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4SbulkJctSideGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJSWS = %g is too big. Set to 0.99.\n",((BSIM4SbulkJctSideGradingCoeff)*UNITFAD).val());
            BSIM4SbulkJctSideGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4SbulkJctGateSideGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJSWGS = %g is too big. Set to 0.99.\n",((BSIM4SbulkJctGateSideGradingCoeff)*UNITFAD).val());
            BSIM4SbulkJctGateSideGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4DbulkJctBotGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJD = %g is too big. Set to 0.99.\n",((BSIM4DbulkJctBotGradingCoeff)*UNITFAD).val());
            BSIM4DbulkJctBotGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4DbulkJctSideGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJSWD = %g is too big. Set to 0.99.\n",((BSIM4DbulkJctSideGradingCoeff)*UNITFAD).val());
            BSIM4DbulkJctSideGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4DbulkJctGateSideGradingCoeff >= 0.99)) {
          {
            printf("Warning: MJSWGD = %g is too big. Set to 0.99.\n",((BSIM4DbulkJctGateSideGradingCoeff)*UNITFAD).val());
            BSIM4DbulkJctGateSideGradingCoeff = ((0.99)*UNITFAD).val();
          }

        }

        if((BSIM4wpemod == 1)) {
          {
            if((BSIM4scref <= 0.0)) {
              {
                printf("Warning: SCREF = %g is not positive. Set to 1e-6.\n",((BSIM4scref)*UNITFAD).val());
                BSIM4scref = ((1e-6)*UNITFAD).val();
              }

            }

          }

        }

      }

    }
    else {
      printf("Warning: (instance %M) Can't open log file; parameter checking skipped.\n");
    }

  }
  /*The initial_step block ends...*/
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  if (!collapseNode_di)
    addInternalNode(symbol_table, li_di, getName(), "di");
  if (!collapseNode_si)
    addInternalNode(symbol_table, li_si, getName(), "si");
  if (!collapseNode_gi)
    addInternalNode(symbol_table, li_gi, getName(), "gi");
  if (!collapseNode_gm)
    addInternalNode(symbol_table, li_gm, getName(), "gm");
  if (!collapseNode_bi)
    addInternalNode(symbol_table, li_bi, getName(), "bi");
  if (!collapseNode_sbulk)
    addInternalNode(symbol_table, li_sbulk, getName(), "sbulk");
  if (!collapseNode_dbulk)
    addInternalNode(symbol_table, li_dbulk, getName(), "dbulk");
  if (loadLeadCurrent)
  {
    addBranchDataNode( symbol_table, li_branch_id, getName(), "BRANCH_D1");
    addBranchDataNode( symbol_table, li_branch_ig, getName(), "BRANCH_D2");
    addBranchDataNode( symbol_table, li_branch_is, getName(), "BRANCH_D3");
    addBranchDataNode( symbol_table, li_branch_ib, getName(), "BRANCH_D4");
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
        li_branch_id = branchLIDVecRef[i++];
        li_branch_ig = branchLIDVecRef[i++];
        li_branch_is = branchLIDVecRef[i++];
        li_branch_ib = branchLIDVecRef[i++];
  }
}

const JacobianStamp & Instance::jacobianStamp() const
{  return jacStamp; }

  //void Instance::registerJacLIDs
void Instance::registerJacLIDs( const JacobianStamp & jacLIDVec)
{
  DeviceInstance::registerJacLIDs(jacLIDVec);
  IntPair jacLoc;
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_bi)];
  m_di_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_si)];
  m_di_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_sbulk)];
  m_di_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_di)];
  m_di_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_dbulk)];
  m_di_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_gi)];
  m_di_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_bi)];
  m_si_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_si)];
  m_si_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_sbulk)];
  m_si_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_di)];
  m_si_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_dbulk)];
  m_si_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_gi)];
  m_si_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_bi)];
  m_gi_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_si)];
  m_gi_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_sbulk)];
  m_gi_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_di)];
  m_gi_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_dbulk)];
  m_gi_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_gi)];
  m_gi_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_bi)];
  m_bi_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_si)];
  m_bi_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_sbulk)];
  m_bi_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_di)];
  m_bi_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_dbulk)];
  m_bi_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_gi)];
  m_bi_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_s)];
  m_s_Equ_s_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_si)];
  m_s_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_bi)];
  m_s_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_sbulk)];
  m_s_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_di)];
  m_s_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_dbulk)];
  m_s_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_gi)];
  m_s_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_s)];
  m_si_Equ_s_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_d)];
  m_d_Equ_d_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_di)];
  m_d_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_bi)];
  m_d_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_si)];
  m_d_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_sbulk)];
  m_d_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_dbulk)];
  m_d_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_gi)];
  m_d_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_d)];
  m_di_Equ_d_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_g)];
  m_g_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_gm)];
  m_g_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_g)];
  m_gm_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_gm)];
  m_gm_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_gi)];
  m_gm_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_bi)];
  m_gm_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_si)];
  m_gm_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_sbulk)];
  m_gm_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_di)];
  m_gm_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gm,cogendaNodeID_dbulk)];
  m_gm_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_gi,cogendaNodeID_gm)];
  m_gi_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_sbulk)];
  m_sbulk_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_bi)];
  m_sbulk_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_b)];
  m_sbulk_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_sbulk)];
  m_b_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_b)];
  m_b_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_bi)];
  m_b_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_b)];
  m_bi_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_dbulk)];
  m_b_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_b)];
  m_dbulk_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_dbulk)];
  m_dbulk_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_bi)];
  m_dbulk_Equ_bi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_si)];
  m_sbulk_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_di)];
  m_sbulk_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_dbulk)];
  m_sbulk_Equ_dbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_si)];
  m_dbulk_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_di)];
  m_dbulk_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_sbulk)];
  m_dbulk_Equ_sbulk_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_gm)];
  m_si_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_gm)];
  m_di_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_bi,cogendaNodeID_gm)];
  m_bi_Equ_gm_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_sbulk,cogendaNodeID_gi)];
  m_sbulk_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_dbulk,cogendaNodeID_gi)];
  m_dbulk_Equ_gi_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_di_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_bi);
  f_di_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_si);
  f_di_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_sbulk);
  f_di_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_di);
  f_di_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_dbulk);
  f_di_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_gi);
  f_si_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_bi);
  f_si_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_si);
  f_si_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_sbulk);
  f_si_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_di);
  f_si_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_dbulk);
  f_si_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_gi);
  f_gi_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_bi);
  f_gi_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_si);
  f_gi_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_sbulk);
  f_gi_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_di);
  f_gi_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_dbulk);
  f_gi_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_gi);
  f_bi_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_bi);
  f_bi_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_si);
  f_bi_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_sbulk);
  f_bi_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_di);
  f_bi_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_dbulk);
  f_bi_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_gi);
  f_s_Equ_s_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_s);
  f_s_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_si);
  f_s_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_bi);
  f_s_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_sbulk);
  f_s_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_di);
  f_s_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_dbulk);
  f_s_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_gi);
  f_si_Equ_s_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_s);
  f_d_Equ_d_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_d);
  f_d_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_di);
  f_d_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_bi);
  f_d_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_si);
  f_d_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_sbulk);
  f_d_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_dbulk);
  f_d_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_gi);
  f_di_Equ_d_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_d);
  f_g_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_g);
  f_g_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_gm);
  f_gm_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_g);
  f_gm_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_gm);
  f_gm_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_gi);
  f_gm_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_bi);
  f_gm_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_si);
  f_gm_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_sbulk);
  f_gm_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_di);
  f_gm_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gm,li_dbulk);
  f_gi_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_gi,li_gm);
  f_sbulk_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_sbulk);
  f_sbulk_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_bi);
  f_sbulk_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_b);
  f_b_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_sbulk);
  f_b_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_b);
  f_b_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_bi);
  f_bi_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_b);
  f_b_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_dbulk);
  f_dbulk_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_b);
  f_dbulk_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_dbulk);
  f_dbulk_Equ_bi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_bi);
  f_sbulk_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_si);
  f_sbulk_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_di);
  f_sbulk_Equ_dbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_dbulk);
  f_dbulk_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_si);
  f_dbulk_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_di);
  f_dbulk_Equ_sbulk_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_sbulk);
  f_si_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_gm);
  f_di_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_gm);
  f_bi_Equ_gm_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_bi,li_gm);
  f_sbulk_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_sbulk,li_gi);
  f_dbulk_Equ_gi_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_dbulk,li_gi);
  q_di_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_bi);
  q_di_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_si);
  q_di_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_sbulk);
  q_di_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_di);
  q_di_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_dbulk);
  q_di_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_gi);
  q_si_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_bi);
  q_si_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_si);
  q_si_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_sbulk);
  q_si_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_di);
  q_si_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_dbulk);
  q_si_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_gi);
  q_gi_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_bi);
  q_gi_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_si);
  q_gi_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_sbulk);
  q_gi_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_di);
  q_gi_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_dbulk);
  q_gi_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_gi);
  q_bi_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_bi);
  q_bi_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_si);
  q_bi_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_sbulk);
  q_bi_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_di);
  q_bi_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_dbulk);
  q_bi_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_gi);
  q_s_Equ_s_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_s);
  q_s_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_si);
  q_s_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_bi);
  q_s_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_sbulk);
  q_s_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_di);
  q_s_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_dbulk);
  q_s_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_gi);
  q_si_Equ_s_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_s);
  q_d_Equ_d_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_d);
  q_d_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_di);
  q_d_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_bi);
  q_d_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_si);
  q_d_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_sbulk);
  q_d_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_dbulk);
  q_d_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_gi);
  q_di_Equ_d_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_d);
  q_g_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_g);
  q_g_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_gm);
  q_gm_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_g);
  q_gm_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_gm);
  q_gm_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_gi);
  q_gm_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_bi);
  q_gm_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_si);
  q_gm_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_sbulk);
  q_gm_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_di);
  q_gm_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gm,li_dbulk);
  q_gi_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_gi,li_gm);
  q_sbulk_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_sbulk);
  q_sbulk_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_bi);
  q_sbulk_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_b);
  q_b_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_sbulk);
  q_b_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_b);
  q_b_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_bi);
  q_bi_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_b);
  q_b_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_dbulk);
  q_dbulk_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_b);
  q_dbulk_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_dbulk);
  q_dbulk_Equ_bi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_bi);
  q_sbulk_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_si);
  q_sbulk_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_di);
  q_sbulk_Equ_dbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_dbulk);
  q_dbulk_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_si);
  q_dbulk_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_di);
  q_dbulk_Equ_sbulk_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_sbulk);
  q_si_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_gm);
  q_di_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_gm);
  q_bi_Equ_gm_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_bi,li_gm);
  q_sbulk_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_sbulk,li_gi);
  q_dbulk_Equ_gi_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_dbulk,li_gi);
}

  //bool Instance::loadDAEFVector
bool Instance::loadDAEFVector()
{
  bool bsuccess=true;
  (*extData.daeFVectorPtr)[li_d] += staticContributions[cogendaNodeID_d].val();
  (*extData.daeFVectorPtr)[li_g] += staticContributions[cogendaNodeID_g].val();
  (*extData.daeFVectorPtr)[li_s] += staticContributions[cogendaNodeID_s].val();
  (*extData.daeFVectorPtr)[li_b] += staticContributions[cogendaNodeID_b].val();
  (*extData.daeFVectorPtr)[li_di] += staticContributions[cogendaNodeID_di].val();
  (*extData.daeFVectorPtr)[li_si] += staticContributions[cogendaNodeID_si].val();
  (*extData.daeFVectorPtr)[li_gi] += staticContributions[cogendaNodeID_gi].val();
  (*extData.daeFVectorPtr)[li_gm] += staticContributions[cogendaNodeID_gm].val();
  (*extData.daeFVectorPtr)[li_bi] += staticContributions[cogendaNodeID_bi].val();
  (*extData.daeFVectorPtr)[li_sbulk] += staticContributions[cogendaNodeID_sbulk].val();
  (*extData.daeFVectorPtr)[li_dbulk] += staticContributions[cogendaNodeID_dbulk].val();
  if (loadLeadCurrent)
  {
    double * leadF = extData.nextLeadCurrFCompRawPtr;
          leadF[li_branch_id] = leadCurrentF[cogendaNodeID_d];
          leadF[li_branch_ig] = leadCurrentF[cogendaNodeID_g];
          leadF[li_branch_is] = leadCurrentF[cogendaNodeID_s];
          leadF[li_branch_ib] = leadCurrentF[cogendaNodeID_b];
  }
  return bsuccess;
}

  //bool Instance::loadDAEQVector
bool Instance::loadDAEQVector()
{
  (*extData.daeQVectorPtr)[li_d] += dynamicContributions[cogendaNodeID_d].val();
  (*extData.daeQVectorPtr)[li_g] += dynamicContributions[cogendaNodeID_g].val();
  (*extData.daeQVectorPtr)[li_s] += dynamicContributions[cogendaNodeID_s].val();
  (*extData.daeQVectorPtr)[li_b] += dynamicContributions[cogendaNodeID_b].val();
  (*extData.daeQVectorPtr)[li_di] += dynamicContributions[cogendaNodeID_di].val();
  (*extData.daeQVectorPtr)[li_si] += dynamicContributions[cogendaNodeID_si].val();
  (*extData.daeQVectorPtr)[li_gi] += dynamicContributions[cogendaNodeID_gi].val();
  (*extData.daeQVectorPtr)[li_gm] += dynamicContributions[cogendaNodeID_gm].val();
  (*extData.daeQVectorPtr)[li_bi] += dynamicContributions[cogendaNodeID_bi].val();
  (*extData.daeQVectorPtr)[li_sbulk] += dynamicContributions[cogendaNodeID_sbulk].val();
  (*extData.daeQVectorPtr)[li_dbulk] += dynamicContributions[cogendaNodeID_dbulk].val();
  if (loadLeadCurrent)
  {
    double * leadQ = extData.nextLeadCurrQCompRawPtr;
          leadQ[li_branch_id] = leadCurrentQ[cogendaNodeID_d];
          leadQ[li_branch_ig] = leadCurrentQ[cogendaNodeID_g];
          leadQ[li_branch_is] = leadCurrentQ[cogendaNodeID_s];
          leadQ[li_branch_ib] = leadCurrentQ[cogendaNodeID_b];
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
  if (probeVars.size() != (20))
  {
    probeVars.resize(20);
    staticContributions.resize(11+0);
    dynamicContributions.resize(11+0);
  }

  for (int i=0; i < 11+0 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_V_di_si] = (*solVectorPtr)[li_di] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_di_si].diff(cogendaProbeID_V_di_si,20);
  probeVars[cogendaProbeID_V_gi_si] = (*solVectorPtr)[li_gi] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_gi_si].diff(cogendaProbeID_V_gi_si,20);
  probeVars[cogendaProbeID_V_bi_si] = (*solVectorPtr)[li_bi] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_bi_si].diff(cogendaProbeID_V_bi_si,20);
  probeVars[cogendaProbeID_V_g_si] = (*solVectorPtr)[li_g] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_g_si].diff(cogendaProbeID_V_g_si,20);
  probeVars[cogendaProbeID_V_gm_si] = (*solVectorPtr)[li_gm] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_gm_si].diff(cogendaProbeID_V_gm_si,20);
  probeVars[cogendaProbeID_V_dbulk_si] = (*solVectorPtr)[li_dbulk] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_dbulk_si].diff(cogendaProbeID_V_dbulk_si,20);
  probeVars[cogendaProbeID_V_sbulk_si] = (*solVectorPtr)[li_sbulk] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_sbulk_si].diff(cogendaProbeID_V_sbulk_si,20);
  probeVars[cogendaProbeID_V_s_si] = (*solVectorPtr)[li_s] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_s_si].diff(cogendaProbeID_V_s_si,20);
  probeVars[cogendaProbeID_V_d_si] = (*solVectorPtr)[li_d] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_d_si].diff(cogendaProbeID_V_d_si,20);
  probeVars[cogendaProbeID_V_d_di] = (*solVectorPtr)[li_d] - (*solVectorPtr)[li_di];
  probeVars[cogendaProbeID_V_d_di].diff(cogendaProbeID_V_d_di,20);
  probeVars[cogendaProbeID_V_g_gm] = (*solVectorPtr)[li_g] - (*solVectorPtr)[li_gm];
  probeVars[cogendaProbeID_V_g_gm].diff(cogendaProbeID_V_g_gm,20);
  probeVars[cogendaProbeID_V_gm_gi] = (*solVectorPtr)[li_gm] - (*solVectorPtr)[li_gi];
  probeVars[cogendaProbeID_V_gm_gi].diff(cogendaProbeID_V_gm_gi,20);
  probeVars[cogendaProbeID_V_sbulk_bi] = (*solVectorPtr)[li_sbulk] - (*solVectorPtr)[li_bi];
  probeVars[cogendaProbeID_V_sbulk_bi].diff(cogendaProbeID_V_sbulk_bi,20);
  probeVars[cogendaProbeID_V_sbulk_b] = (*solVectorPtr)[li_sbulk] - (*solVectorPtr)[li_b];
  probeVars[cogendaProbeID_V_sbulk_b].diff(cogendaProbeID_V_sbulk_b,20);
  probeVars[cogendaProbeID_V_b_bi] = (*solVectorPtr)[li_b] - (*solVectorPtr)[li_bi];
  probeVars[cogendaProbeID_V_b_bi].diff(cogendaProbeID_V_b_bi,20);
  probeVars[cogendaProbeID_V_b_dbulk] = (*solVectorPtr)[li_b] - (*solVectorPtr)[li_dbulk];
  probeVars[cogendaProbeID_V_b_dbulk].diff(cogendaProbeID_V_b_dbulk,20);
  probeVars[cogendaProbeID_V_bi_dbulk] = (*solVectorPtr)[li_bi] - (*solVectorPtr)[li_dbulk];
  probeVars[cogendaProbeID_V_bi_dbulk].diff(cogendaProbeID_V_bi_dbulk,20);
  probeVars[cogendaProbeID_V_bi_s] = (*solVectorPtr)[li_bi] - (*solVectorPtr)[li_s];
  probeVars[cogendaProbeID_V_bi_s].diff(cogendaProbeID_V_bi_s,20);
  probeVars[cogendaProbeID_V_dbulk_s] = (*solVectorPtr)[li_dbulk] - (*solVectorPtr)[li_s];
  probeVars[cogendaProbeID_V_dbulk_s].diff(cogendaProbeID_V_dbulk_s,20);
  probeVars[cogendaProbeID_V_sbulk_s] = (*solVectorPtr)[li_sbulk] - (*solVectorPtr)[li_s];
  probeVars[cogendaProbeID_V_sbulk_s].diff(cogendaProbeID_V_sbulk_s,20);

{
  if((this->verbose == 1)) {
    printf("New model main evaluation starts...\n");
  }
  vds = (BSIM4type*probeVars[cogendaProbeID_V_di_si]);
  vgs = (BSIM4type*probeVars[cogendaProbeID_V_gi_si]);
  vbs = (BSIM4type*probeVars[cogendaProbeID_V_bi_si]);
  vges = (BSIM4type*probeVars[cogendaProbeID_V_g_si]);
  vgms = (BSIM4type*probeVars[cogendaProbeID_V_gm_si]);
  vdbs = (BSIM4type*probeVars[cogendaProbeID_V_dbulk_si]);
  vsbs = (BSIM4type*probeVars[cogendaProbeID_V_sbulk_si]);
  vses = (BSIM4type*probeVars[cogendaProbeID_V_s_si]);
  vdes = (BSIM4type*probeVars[cogendaProbeID_V_d_si]);
  if((this->verbose == 1)) {
    printf("Volts:%g,%g,%g,%g,%g,%g,%g,%g,%g\n",vds.val(),vgs.val(),vbs.val(),vges.val(),vgms.val(),vdbs.val(),vsbs.val(),vses.val(),vdes.val());
  }
  vbd = (vbs-vds);
  vgd = (vgs-vds);
  vgb = (vgs-vbs);
  vged = (vges-vds);
  vgmd = (vgms-vds);
  vgmb = (vgms-vbs);
  vdbd = (vdbs-vds);
  vbs_jct = cogenda_ternary_op<CogendaFadType>(!BSIM4rbodyMod,vbs,vsbs);
  vbd_jct = cogenda_ternary_op<CogendaFadType>(!BSIM4rbodyMod,vbd,vdbd);
  Nvtms = (((BSIM4vtm*BSIM4SjctEmissionCoeff))*UNITFAD).val();
  if(((BSIM4Aseff <= 0.0) && (BSIM4Pseff <= 0.0))) {
    {
      SourceSatCurrent = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      SourceSatCurrent = (((((BSIM4Aseff*BSIM4SjctTempSatCurDensity)+(BSIM4Pseff*BSIM4SjctSidewallTempSatCurDensity))+((BSIM4weffCJ*BSIM4nf)*BSIM4SjctGateSidewallTempSatCurDensity)))*UNITFAD).val();
    }
  }
  if((SourceSatCurrent <= 0.0)) {
    {
      BSIM4gbs = ((this->_ckt_gmin)*UNITFAD).val();
      BSIM4cbs = (BSIM4gbs*vbs_jct);
    }
  }
  else {
    {
      switch(BSIM4dioMod){
      case 0:
        {
          evbs = exp((vbs_jct/Nvtms));
          T1 = (BSIM4xjbvs*exp((-(BSIM4bvs+vbs_jct)/Nvtms)));
          BSIM4gbs = (((SourceSatCurrent*(evbs+T1))/Nvtms)+this->_ckt_gmin);
          BSIM4cbs = ((SourceSatCurrent*(((evbs+BSIM4XExpBVS)-T1)-1.0))+(this->_ckt_gmin*vbs_jct));
        }
        break;
      case 1:
        {
          T2 = (vbs_jct/Nvtms);
          if((T2 < -34.0)) {
            {
              BSIM4gbs = this->_ckt_gmin;
              BSIM4cbs = ((SourceSatCurrent*(1.713908431e-15-1.0))+(this->_ckt_gmin*vbs_jct));
            }
          }
                  else if((vbs_jct <= BSIM4vjsmFwd)) {
            {
              evbs = exp(T2);
              BSIM4gbs = (((SourceSatCurrent*evbs)/Nvtms)+this->_ckt_gmin);
              BSIM4cbs = ((SourceSatCurrent*(evbs-1.0))+(this->_ckt_gmin*vbs_jct));
            }
          }
          else {
            {
              T0 = (((BSIM4IVjsmFwd/Nvtms))*UNITFAD).val();
              BSIM4gbs = (T0+this->_ckt_gmin);
              BSIM4cbs = (((BSIM4IVjsmFwd-SourceSatCurrent)+(T0*(vbs_jct-BSIM4vjsmFwd)))+(this->_ckt_gmin*vbs_jct));
            }
          }
        }
        break;
      case 2:
        {
          if((vbs_jct < BSIM4vjsmRev)) {
            {
              T0 = (vbs_jct/Nvtms);
              if((T0 < -34.0)) {
                {
                  evbs = 1.713908431e-15;
                  devbs_dvb = ((0.0)*UNITFAD).val();
                }
              }
              else {
                {
                  evbs = exp(T0);
                  devbs_dvb = (evbs/Nvtms);
                }
              }
              T1 = (evbs-1.0);
              T2 = (BSIM4IVjsmRev+(BSIM4SslpRev*(vbs_jct-BSIM4vjsmRev)));
              BSIM4gbs = (((devbs_dvb*T2)+(T1*BSIM4SslpRev))+this->_ckt_gmin);
              BSIM4cbs = ((T1*T2)+(this->_ckt_gmin*vbs_jct));
            }
          }
                  else if((vbs_jct <= BSIM4vjsmFwd)) {
            {
              T0 = (vbs_jct/Nvtms);
              if((T0 < -34.0)) {
                {
                  evbs = 1.713908431e-15;
                  devbs_dvb = 0.0;
                }
              }
              else {
                {
                  evbs = exp(T0);
                  devbs_dvb = (evbs/Nvtms);
                }
              }
              T1 = ((BSIM4bvs+vbs_jct)/Nvtms);
              if((T1 > 34.0)) {
                {
                  T2 = 1.713908431e-15;
                  T3 = ((0.0)*UNITFAD).val();
                }
              }
              else {
                {
                  T2 = exp(-T1);
                  T3 = (-T2/Nvtms);
                }
              }
              BSIM4gbs = ((SourceSatCurrent*(devbs_dvb-(BSIM4xjbvs*T3)))+this->_ckt_gmin);
              BSIM4cbs = ((SourceSatCurrent*(((evbs+BSIM4XExpBVS)-1.0
				)-(BSIM4xjbvs*T2)))+(this->_ckt_gmin*vbs_jct));
            }
          }
          else {
            {
              BSIM4gbs = (BSIM4SslpFwd+this->_ckt_gmin);
              BSIM4cbs = ((BSIM4IVjsmFwd+(BSIM4SslpFwd*(vbs_jct-BSIM4vjsmFwd)))+(this->_ckt_gmin*vbs_jct));
            }
          }
        }
        break;
      }
    }
  }
  Nvtmd = (((BSIM4vtm*BSIM4DjctEmissionCoeff))*UNITFAD).val();
  if(((BSIM4Adeff <= 0.0) && (BSIM4Pdeff <= 0.0))) {
    DrainSatCurrent = ((0.0)*UNITFAD).val();
  }
  else {
    DrainSatCurrent = (((((BSIM4Adeff*BSIM4DjctTempSatCurDensity)+(BSIM4Pdeff*BSIM4DjctSidewallTempSatCurDensity))+((BSIM4weffCJ*BSIM4nf)*BSIM4DjctGateSidewallTempSatCurDensity)))*UNITFAD).val();
  }
  if((DrainSatCurrent <= 0.0)) {
    {
      BSIM4gbd = ((this->_ckt_gmin)*UNITFAD).val();
      BSIM4cbd = (BSIM4gbd*vbd_jct);
    }
  }
  else {
    {
      switch(BSIM4dioMod){
      case 0:
        {
          evbd = exp((vbd_jct/Nvtmd));
          T1 = (BSIM4xjbvd*exp((-(BSIM4bvd+vbd_jct)/Nvtmd)));
          BSIM4gbd = (((DrainSatCurrent*(evbd+T1))/Nvtmd)+this->_ckt_gmin);
          BSIM4cbd = ((DrainSatCurrent*(((evbd+BSIM4XExpBVD)-T1)-1.0))+(this->_ckt_gmin*vbd_jct));
        }
        break;
      case 1:
        {
          T2 = (vbd_jct/Nvtmd);
          if((T2 < -34.0)) {
            {
              BSIM4gbd = this->_ckt_gmin;
              BSIM4cbd = ((DrainSatCurrent*(1.713908431e-15-1.0))+(this->_ckt_gmin*vbd_jct));
            }
          }
                  else if((vbd_jct <= BSIM4vjdmFwd)) {
            {
              evbd = exp(T2);
              BSIM4gbd = (((DrainSatCurrent*evbd)/Nvtmd)+this->_ckt_gmin);
              BSIM4cbd = ((DrainSatCurrent*(evbd-1.0))+(this->_ckt_gmin*vbd_jct));
            }
          }
          else {
            {
              T0 = (BSIM4IVjdmFwd/Nvtmd);
              BSIM4gbd = (T0+this->_ckt_gmin);
              BSIM4cbd = (((BSIM4IVjdmFwd-DrainSatCurrent)+(T0*(vbd_jct-BSIM4vjdmFwd)))+(this->_ckt_gmin*vbd_jct));
            }
          }
        }
        break;
      case 2:
        {
          if((vbd_jct < BSIM4vjdmRev)) {
            {
              T0 = (vbd_jct/Nvtmd);
              if((T0 < -34.0)) {
                {
                  evbd = 1.713908431e-15;
                  devbd_dvb = ((0.0)*UNITFAD).val();
                }
              }
              else {
                {
                  evbd = exp(T0);
                  devbd_dvb = (evbd/Nvtmd);
                }
              }
              T1 = (evbd-1.0);
              T2 = (BSIM4IVjdmRev+(BSIM4DslpRev*(vbd_jct-BSIM4vjdmRev)));
              BSIM4gbd = (((devbd_dvb*T2)+(T1*BSIM4DslpRev))+this->_ckt_gmin);
              BSIM4cbd = ((T1*T2)+(this->_ckt_gmin*vbd_jct));
            }
          }
                  else if((vbd_jct <= BSIM4vjdmFwd)) {
            {
              T0 = (vbd_jct/Nvtmd);
              if((T0 < -34.0)) {
                {
                  evbd = 1.713908431e-15;
                  devbd_dvb = 0.0;
                }
              }
              else {
                {
                  evbd = exp(T0);
                  devbd_dvb = (evbd/Nvtmd);
                }
              }
              T1 = ((BSIM4bvd+vbd_jct)/Nvtmd);
              if((T1 > 34.0)) {
                {
                  T2 = 1.713908431e-15;
                  T3 = 0.0;
                }
              }
              else {
                {
                  T2 = exp(-T1);
                  T3 = (-T2/Nvtmd);
                }
              }
              BSIM4gbd = ((DrainSatCurrent*(devbd_dvb-(BSIM4xjbvd*T3)))+this->_ckt_gmin);
              BSIM4cbd = ((DrainSatCurrent*(((evbd+BSIM4XExpBVD)-1.0
)-(BSIM4xjbvd*T2)))+(this->_ckt_gmin*vbd_jct));
            }
          }
          else {
            {
              BSIM4gbd = (BSIM4DslpFwd+this->_ckt_gmin);
              BSIM4cbd = ((BSIM4IVjdmFwd+(BSIM4DslpFwd*(vbd_jct-BSIM4vjdmFwd)))+(this->_ckt_gmin*vbd_jct));
            }
          }
        }
        break;
      }
    }
  }
  Nvtmrssws = (((Vtm0*BSIM4njtsswstemp))*UNITFAD).val();
  Nvtmrsswgs = (((Vtm0*BSIM4njtsswgstemp))*UNITFAD).val();
  Nvtmrss = (((Vtm0*BSIM4njtsstemp))*UNITFAD).val();
  Nvtmrsswd = (((Vtm0*BSIM4njtsswdtemp))*UNITFAD).val();
  Nvtmrsswgd = (((Vtm0*BSIM4njtsswgdtemp))*UNITFAD).val();
  Nvtmrsd = (((Vtm0*BSIM4njtsdtemp))*UNITFAD).val();
  if(((BSIM4vtss-vbs_jct) < (BSIM4vtss*1e-3))) {
    {
      T9 = ((1.0e3)*UNITFAD).val();
      T0 = ((-vbs_jct/Nvtmrss)*T9);
      if((T0 > 34.0)) {
        {
          T1 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = ((5.834617425e14)*UNITFAD).val();
        }
      }
          else if((T0 < -34.0)) {
        {
          T1 = 1.713908431e-15;
          T10 = ((0)*UNITFAD).val();
        }
      }
      else {
        {
          T1 = exp(T0);
          T10 = T1;
        }
      }
      dT1_dVb = ((T10/Nvtmrss)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtss-vbs_jct));
      T0 = (((-vbs_jct/Nvtmrss)*BSIM4vtss)*T9);
      dT0_dVb = ((BSIM4vtss/Nvtmrss)*(T9+((vbs_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T1 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T1 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T1 = exp(T0);
          T10 = T1;
        }
      }
      dT1_dVb = (T10*dT0_dVb);
    }
  }
  if(((BSIM4vtsd-vbd_jct) < (BSIM4vtsd*1e-3))) {
    {
      T9 = 1.0e3;
      T0 = ((-vbd_jct/Nvtmrsd)*T9);
      if((T0 > 34.0)) {
        {
          T2 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T2 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T2 = exp(T0);
          T10 = T2;
        }
      }
      dT2_dVb = ((T10/Nvtmrsd)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtsd-vbd_jct));
      T0 = (((-vbd_jct/Nvtmrsd)*BSIM4vtsd)*T9);
      dT0_dVb = ((BSIM4vtsd/Nvtmrsd)*(T9+((vbd_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T2 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T2 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T2 = exp(T0);
          T10 = T2;
        }
      }
      dT2_dVb = (T10*dT0_dVb);
    }
  }
  if(((BSIM4vtssws-vbs_jct) < (BSIM4vtssws*1e-3))) {
    {
      T9 = 1.0e3;
      T0 = ((-vbs_jct/Nvtmrssws)*T9);
      if((T0 > 34.0)) {
        {
          T3 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T3 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T3 = exp(T0);
          T10 = T3;
        }
      }
      dT3_dVb = ((T10/Nvtmrssws)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtssws-vbs_jct));
      T0 = (((-vbs_jct/Nvtmrssws)*BSIM4vtssws)*T9);
      dT0_dVb = ((BSIM4vtssws/Nvtmrssws)*(T9+((vbs_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T3 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T3 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T3 = exp(T0);
          T10 = T3;
        }
      }
      dT3_dVb = (T10*dT0_dVb);
    }
  }
  if(((BSIM4vtsswd-vbd_jct) < (BSIM4vtsswd*1e-3))) {
    {
      T9 = 1.0e3;
      T0 = ((-vbd_jct/Nvtmrsswd)*T9);
      if((T0 > 34.0)) {
        {
          T4 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T4 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T4 = exp(T0);
          T10 = T4;
        }
      }
      dT4_dVb = ((T10/Nvtmrsswd)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtsswd-vbd_jct));
      T0 = (((-vbd_jct/Nvtmrsswd)*BSIM4vtsswd)*T9);
      dT0_dVb = ((BSIM4vtsswd/Nvtmrsswd)*(T9+((vbd_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T4 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T4 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T4 = exp(T0);
          T10 = T4;
        }
      }
      dT4_dVb = (T10*dT0_dVb);
    }
  }
  if(((BSIM4vtsswgs-vbs_jct) < (BSIM4vtsswgs*1e-3))) {
    {
      T9 = 1.0e3;
      T0 = ((-vbs_jct/Nvtmrsswgs)*T9);
      if((T0 > 34.0)) {
        {
          T5 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T5 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T5 = exp(T0);
          T10 = T5;
        }
      }
      dT5_dVb = ((T10/Nvtmrsswgs)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtsswgs-vbs_jct));
      T0 = (((-vbs_jct/Nvtmrsswgs)*BSIM4vtsswgs)*T9);
      dT0_dVb = ((BSIM4vtsswgs/Nvtmrsswgs)*(T9+((vbs_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T5 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T5 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T5 = exp(T0);
          T10 = T5;
        }
      }
      dT5_dVb = (T10*dT0_dVb);
    }
  }
  if(((BSIM4vtsswgd-vbd_jct) < (BSIM4vtsswgd*1e-3))) {
    {
      T9 = 1.0e3;
      T0 = ((-vbd_jct/Nvtmrsswgd)*T9);
      if((T0 > 34.0)) {
        {
          T6 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T6 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T6 = exp(T0);
          T10 = T6;
        }
      }
      dT6_dVb = ((T10/Nvtmrsswgd)*T9);
    }
  }
  else {
    {
      T9 = (1.0/(BSIM4vtsswgd-vbd_jct));
      T0 = (((-vbd_jct/Nvtmrsswgd)*BSIM4vtsswgd)*T9);
      dT0_dVb = ((BSIM4vtsswgd/Nvtmrsswgd)*(T9+((vbd_jct*T9)*T9)));
      if((T0 > 34.0)) {
        {
          T6 = (5.834617425e14*((1.0+T0)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T0 < -34.0)) {
        {
          T6 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T6 = exp(T0);
          T10 = T6;
        }
      }
      dT6_dVb = (T10*dT0_dVb);
    }
  }
  BSIM4gbs = (((BSIM4gbs+(BSIM4SjctTempRevSatCur*dT1_dVb))+(BSIM4SswTempRevSatCur*dT3_dVb))+(BSIM4SswgTempRevSatCur*dT5_dVb));
  BSIM4cbs = (BSIM4cbs-(((BSIM4SjctTempRevSatCur*(T1-1.0))+(BSIM4SswTempRevSatCur*(T3-1.0)))+(BSIM4SswgTempRevSatCur*(T5-1.0))));
  BSIM4gbd = (((BSIM4gbd+(BSIM4DjctTempRevSatCur*dT2_dVb))+(BSIM4DswTempRevSatCur*dT4_dVb))+(BSIM4DswgTempRevSatCur*dT6_dVb));
  BSIM4cbd = (BSIM4cbd-(((BSIM4DjctTempRevSatCur*(T2-1.0))+(BSIM4DswTempRevSatCur*(T4-1.0)))+(BSIM4DswgTempRevSatCur*(T6-1.0))));
  if((this->verbose == 1)) {
    {
      printf("BSIM4cbd,s=%g,%g\n",BSIM4cbd.val(),BSIM4cbs.val());
      printf("BSIM4cbd's RHS=%g,%g,%g,%g,%g,%g\n",((BSIM4DjctTempRevSatCur)*UNITFAD).val(),((BSIM4DswTempRevSatCur)*UNITFAD).val(),((BSIM4DswgTempRevSatCur)*UNITFAD).val(),T2.val(),T4.val(),T6.val());
    }
  }
  if((vds >= 0.0)) {
    {
      BSIM4mode = ((1)*UNITFAD).val();
      Vds = vds;
      Vgs = vgs;
      Vbs = vbs;
      Vdb = (vds-vbs);
    }
  }
  else {
    {
      BSIM4mode = ((-1)*UNITFAD).val();
      Vds = -vds;
      Vgs = vgd;
      Vbs = vbd;
      Vdb = -vbs;
    }
  }
  T0 = ((Vbs-here_BSIM4vbsc)-0.001);
  T1 = sqrt(((T0*T0)-(0.004*here_BSIM4vbsc)));
  if((T0 >= 0.0)) {
    Vbseff = (here_BSIM4vbsc+(0.5*(T0+T1)));
  }
  else {
    {
      T2 = (-0.002/(T1-T0));
      Vbseff = (here_BSIM4vbsc*(1.0+T2));
    }
  }
  if((this->verbose == 1)) {
    {
      printf("Vbseff=%g,%g,%g,%g,%g\n",Vbseff.val(),Vds.val(),Vgs.val(),Vbs.val(),Vdb.val());
      printf("gbs,d=%g,%g\n",BSIM4gbs.val(),BSIM4gbd.val());
    }
  }
  T9 = (0.95*BSIM4phi);
  T0 = ((T9-Vbseff)-0.001);
  T1 = sqrt(((T0*T0)+(0.004*T9)));
  Vbseff = (T9-(0.5*(T0+T1)));
  Phis = (BSIM4phi-Vbseff);
  sqrtPhis = sqrt(Phis);
  if((this->verbose == 1)) {
    printf("Vbseff=%g\n",Vbseff.val());
  }
  Xdep = ((BSIM4Xdep0*sqrtPhis)/BSIM4sqrtPhi);
  Leff = ((BSIM4leff)*UNITFAD).val();
  Vtm = ((BSIM4vtm)*UNITFAD).val();
  if((this->verbose == 1)) {
    {
      printf("Leff=%g\n",((Leff)*UNITFAD).val());
      printf("Vtm=%g\n",((Vtm)*UNITFAD).val());
      printf("BSIM4phi=%g\n",((BSIM4phi)*UNITFAD).val());
      printf("Xdep=%g\n",Xdep.val());
      printf("here_BSIM4vbsc=%g\n",((here_BSIM4vbsc)*UNITFAD).val());
      printf("DrainSatCurrent,SourceSatCurrent=%g,%g\n",((DrainSatCurrent)*UNITFAD).val(),((SourceSatCurrent)*UNITFAD).val());
    }
  }
  T3 = sqrt(Xdep);
  V0 = (((BSIM4vbi-BSIM4phi))*UNITFAD).val();
  T0 = (BSIM4dvt2*Vbseff);
  if((T0 >= -0.5)) {
    {
      T1 = (1.0+T0);
      T2 = BSIM4dvt2;
    }
  }
  else {
    {
      T4 = (1.0/(3.0+(8.0*T0)));
      T1 = ((1.0+(3.0*T0))*T4);
      T2 = ((BSIM4dvt2*T4)*T4);
    }
  }
  lt1 = ((BSIM4factor1*T3)*T1);
  T0 = (BSIM4dvt2w*Vbseff);
  if((T0 >= -0.5)) {
    {
      T1 = (1.0+T0);
      T2 = BSIM4dvt2w;
    }
  }
  else {
    {
      T4 = (1.0/(3.0+(8.0*T0)));
      T1 = ((1.0+(3.0*T0))*T4);
      T2 = ((BSIM4dvt2w*T4)*T4);
    }
  }
  ltw = ((BSIM4factor1*T3)*T1);
  T0 = ((BSIM4dvt1*Leff)/lt1);
  if((T0 < 34.0)) {
    {
      T1 = exp(T0);
      T2 = (T1-1.0);
      T3 = (T2*T2);
      T4 = (T3+((2.0*T1)*1.713908431e-15));
      Theta0 = (T1/T4);
    }
  }
  else {
    Theta0 = (1.0/(5.834617425e14-2.0));
  }
  BSIM4thetavth = (BSIM4dvt0*Theta0);
  Delt_vth = (BSIM4thetavth*V0);
  if((this->verbose == 1)) {
    printf("Delt_vth=%g,%g,%g,%g,%g\n",Delt_vth.val(),((BSIM4dvt0)*UNITFAD).val(),lt1.val(),((BSIM4factor1)*UNITFAD).val(),((BSIM4vbi)*UNITFAD).val());
  }
  T0 = (((BSIM4dvt1w*BSIM4weff)*Leff)/ltw);
  if((T0 < 34.0)) {
    {
      T1 = exp(T0);
      T2 = (T1-1.0);
      T3 = (T2*T2);
      T4 = (T3+((2.0*T1)*1.713908431e-15));
      T5 = (T1/T4);
    }
  }
  else {
    T5 = (1.0/(5.834617425e14-2.0));
  }
  T0 = (BSIM4dvt0w*T5);
  T2 = (T0*V0);
  TempRatio = ((((T/BSIM4tnom)-1.0))*UNITFAD).val();
  T0 = sqrt((1.0+(BSIM4lpe0/Leff)));
  T1 = (((BSIM4k1ox*(T0-1.0))*BSIM4sqrtPhi)+(((BSIM4kt1+(BSIM4kt1l/Leff))+(BSIM4kt2*Vbseff))*TempRatio));
  Vth_NarrowW = ((((toxel*BSIM4phi)/(BSIM4weff+BSIM4w0)))*UNITFAD).val();
  T3 = (here_BSIM4eta0+(BSIM4etab*Vbseff));
  if((T3 < 1.0e-4)) {
    {
      T9 = (1.0/(3.0-(2.0e4*T3)));
      T3 = ((2.0e-4-T3)*T9);
    }
  }
  dDIBL_Sft_dVd = (T3*BSIM4theta0vb0);
  DIBL_Sft = (dDIBL_Sft_dVd*Vds);
  Lpe_Vb = ((sqrt((1.0+(BSIM4lpeb/Leff))))*UNITFAD).val();
  Vth = ((((((((BSIM4type*here_BSIM4vth0)+(((BSIM4k1ox*sqrtPhis)-(BSIM4k1*BSIM4sqrtPhi))*Lpe_Vb))-(here_BSIM4k2ox*Vbseff))-Delt_vth)-T2)+((BSIM4k3+(BSIM4k3b*Vbseff))*Vth_NarrowW))+T1)-DIBL_Sft);
  if((this->verbose == 1)) {
    printf("Vth=%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n",Vth.val(),((here_BSIM4vth0)*UNITFAD).val(),sqrtPhis.val(),((Lpe_Vb)*UNITFAD).val(),((here_BSIM4k2ox)*UNITFAD).val(),Vbseff.val(),Delt_vth.val(),T2.val(),((Vth_NarrowW)*UNITFAD).val(),T1.val(),DIBL_Sft.val());
  }
  tmp1 = (1.03594e-10/Xdep);
  BSIM4nstar = ((BSIM4vtm/1.602176462e-19)*((BSIM4coxe+tmp1)+BSIM4cit));
  tmp2 = (BSIM4nfactor*tmp1);
  tmp3 = ((BSIM4cdsc+(BSIM4cdscb*Vbseff))+(BSIM4cdscd*Vds));
  tmp4 = (((tmp2+(tmp3*Theta0))+BSIM4cit)/BSIM4coxe);
  if((tmp4 >= -0.5)) {
    n = (1.0+tmp4);
  }
  else {
    {
      T0 = (1.0/(3.0+(8.0*tmp4)));
      n = ((1.0+(3.0*tmp4))*T0);
    }
  }
  if((this->verbose == 1)) {
    printf("n=%g\n",n.val());
  }
  if((BSIM4dvtp0 > 0.0)) {
    {
      T0 = (-BSIM4dvtp1*Vds);
      if((T0 < -34.0)) {
        T2 = 1.713908431e-15;
      }
      else {
        T2 = exp(T0);
      }
      T3 = (Leff+(BSIM4dvtp0*(1.0+T2)));
      if((BSIM4tempMod < 2)) {
        T4 = (Vtm*log((Leff/T3)));
      }
      else {
        T4 = (Vtm0*log((Leff/T3)));
      }
      Vth = (Vth-(n*T4));
    }
  }
  if(((BSIM4dvtp4 == 0.0) || (BSIM4dvtp2factor == 0.0))) {
    {
      T0 = 0.0;
      DITS_Sft2 = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      T1 = ((2.0*BSIM4dvtp4)*Vds);
      if((T1 > 34.0)) {
        {
          T0 = (5.834617425e14*((1.0+T1)-34.0));
          T10 = 5.834617425e14;
        }
      }
          else if((T1 < -34.0)) {
        {
          T0 = 1.713908431e-15;
          T10 = 0;
        }
      }
      else {
        {
          T0 = exp(T1);
          T10 = T0;
        }
      }
      DITS_Sft2 = ((BSIM4dvtp2factor*(T0-1))/(T0+1));
      dDITS_Sft2_dVd = ((((BSIM4dvtp2factor*BSIM4dvtp4)*4.0)*T10)/((T0+1)*(T0+1)));
      Vth = (Vth-DITS_Sft2);
      dVth_dVd = (dVth_dVd-dDITS_Sft2_dVd);
    }
  }
  if((this->verbose == 1)) {
    printf("Vth=%g\n",Vth.val());
  }
  T0 = (here_BSIM4vfb+BSIM4phi);
  if((BSIM4mtrlmod == 0)) {
    T1 = 1.03594e-10;
  }
  else {
    T1 = (BSIM4epsrgate*8.85418e-12);
  }
  if((this->verbose == 1)) {
    {
      printf("T0,T1=%g,%g\n",T0.val(),T1.val());
    }
  }
  if(((((BSIM4ngate > 1.0e18) && (BSIM4ngate < 1.0e25)) && (vgs > T0)) && (T1 != 0))) {
    {
      T9 = ((((1.0e6*1.602176462e-19)*T1)*BSIM4ngate)/(BSIM4coxe*BSIM4coxe));
      T8 = (vgs-T0);
      T4 = sqrt((1.0+((2.0*T8)/T9)));
      T2 = ((2.0*T8)/(T4+1.0));
      T3 = (((0.5*T2)*T2)/T9);
      T7 = ((1.12-T3)-0.05);
      T6 = sqrt(((T7*T7)+0.224));
      T5 = (1.12-(0.5*(T7+T6)));
      vgs_eff = (vgs-T5);
      dvgs_eff_dvg = (1.0-((0.5-(0.5/T4))*(1.0+(T7/T6))));
    }
  }
  else {
    {
      vgs_eff = vgs;
      dvgs_eff_dvg = 1.0;
    }
  }
  if(((((BSIM4ngate > 1.0e18) && (BSIM4ngate < 1.0e25)) && (vgd > T0)) && (T1 != 0))) {
    {
      T9 = ((((1.0e6*1.602176462e-19)*T1)*BSIM4ngate)/(BSIM4coxe*BSIM4coxe));
      T8 = (vgd-T0);
      T4 = sqrt((1.0+((2.0*T8)/T9)));
      T2 = ((2.0*T8)/(T4+1.0));
      T3 = (((0.5*T2)*T2)/T9);
      T7 = ((1.12-T3)-0.05);
      T6 = sqrt(((T7*T7)+0.224));
      T5 = (1.12-(0.5*(T7+T6)));
      vgd_eff = (vgd-T5);
      dvgd_eff_dvg = (1.0-((0.5-(0.5/T4))*(1.0+(T7/T6))));
    }
  }
  else {
    {
      vgd_eff = vgd;
      dvgd_eff_dvg = 1.0;
    }
  }
  if((BSIM4mode > 0)) {
    Vgs_eff = vgs_eff;
  }
  else {
    Vgs_eff = vgd_eff;
  }
  Vgst = (Vgs_eff-Vth);
  if((this->verbose == 1)) {
    {
      printf("T0,T1=%g,%g\n",T0.val(),T1.val());
      printf("Vgst=%g,%g,%g\n",Vgst.val(),Vgs_eff.val(),vgs.val(),vgd.val());
      printf("Vgs,d_eff=%g,%g\n",vgs_eff.val(),vgd_eff.val());
    }
  }
  T0 = (n*Vtm);
  T1 = (BSIM4mstar*Vgst);
  T2 = (T1/T0);
  if((T2 > 34.0)) {
    T10 = T1;
  }
  else if((T2 < -34.0)) {
    {
      T10 = (Vtm*log((1.0+1.713908431e-15)));
      T10 = (T10*n);
    }
  }
  else {
    {
      ExpVgst = exp(T2);
      T3 = (Vtm*log((1.0+ExpVgst)));
      T10 = (n*T3);
    }
  }
  T1 = (BSIM4voffcbn-((1.0-BSIM4mstar)*Vgst));
  T2 = (T1/T0);
  if((T2 < -34.0)) {
    {
      T3 = ((BSIM4coxe*1.713908431e-15)/BSIM4cdep0);
      T9 = (BSIM4mstar+(T3*n));
    }
  }
  else if((T2 > 34.0)) {
    {
      T3 = ((BSIM4coxe*5.834617425e14)/BSIM4cdep0);
      T9 = (BSIM4mstar+(T3*n));
    }
  }
  else {
    {
      ExpVgst = exp(T2);
      T3 = (BSIM4coxe/BSIM4cdep0);
      T4 = (T3*ExpVgst);
      T5 = ((T1*T4)/T0);
      T9 = (BSIM4mstar+(n*T4));
    }
  }
  Vgsteff = (T10/T9);
  here_Vgsteff = Vgsteff;
  if((this->verbose == 1)) {
    printf("Vgsteff=%g\n",Vgsteff.val());
  }
  T9 = (sqrtPhis-BSIM4sqrtPhi);
  Weff = (BSIM4weff-(2.0*((BSIM4dwg*Vgsteff)+(BSIM4dwb*T9))));
  if((Weff < 2.0e-8)) {
    {
      T0 = (1.0/(6.0e-8-(2.0*Weff)));
      Weff = ((2.0e-8*(4.0e-8-Weff))*T0);
      T0 = (T0*(T0*4.0e-16));
    }
  }
  if((this->verbose == 1)) {
    printf("Weff=%g\n",Weff.val());
  }
  if((BSIM4rdsMod == 1)) {
    Rds = ((0.0)*UNITFAD).val();
  }
  else {
    {
      T0 = (1.0+(BSIM4prwg*Vgsteff));
      T1 = (BSIM4prwb*T9);
      T2 = ((1.0/T0)+T1);
      T3 = (T2+sqrt(((T2*T2)+0.01)));
      T4 = (BSIM4rds0*0.5);
      Rds = (BSIM4rdswmin+(T3*T4));
      if((Rds > 0.0)) {
        grdsw = ((1.0/Rds)*BSIM4nf);
      }
      else {
        grdsw = 0.0;
      }
    }
  }
  if((this->verbose == 1)) {
    printf("Rds=%g\n",Rds.val());
  }
  T9 = (((0.5*BSIM4k1ox)*Lpe_Vb)/sqrtPhis);
  T1 = ((T9+here_BSIM4k2ox)-(BSIM4k3b*Vth_NarrowW));
  T9 = sqrt((BSIM4xj*Xdep));
  tmp1 = (Leff+(2.0*T9));
  T5 = (Leff/tmp1);
  tmp2 = (BSIM4a0*T5);
  tmp3 = (BSIM4weff+BSIM4b1);
  tmp4 = (BSIM4b0/tmp3);
  T2 = (tmp2+tmp4);
  T7 = ((T5*T5)*T5);
  Abulk0 = (1.0+(T1*T2));
  T8 = ((BSIM4ags*BSIM4a0)*T7);
  dAbulk_dVg = (-T1*T8);
  Abulk = (Abulk0+(dAbulk_dVg*Vgsteff));
  if((Abulk0 < 0.1)) {
    {
      T9 = (1.0/(3.0-(20.0*Abulk0)));
      Abulk0 = ((0.2-Abulk0)*T9);
    }
  }
  if((Abulk < 0.1)) {
    {
      T9 = (1.0/(3.0-(20.0*Abulk)));
      Abulk = ((0.2-Abulk)*T9);
      T10 = (T9*T9);
      dAbulk_dVg = (dAbulk_dVg*T10);
    }
  }
  here_Abulk = Abulk;
  T2 = (BSIM4keta*Vbseff);
  if((T2 >= -0.9)) {
    T0 = (1.0/(1.0+T2));
  }
  else {
    {
      T1 = (1.0/(0.8+T2));
      T0 = ((17.0+(20.0*T2))*T1);
    }
  }
  Abulk = (Abulk*T0);
  Abulk0 = (Abulk0*T0);
  if((this->verbose == 1)) {
    {
      printf("Abulk,0=%g,%g\n",Abulk.val(),Abulk0.val());
    }
  }
  if((BSIM4mtrlmod && (BSIM4mtrlcompatmod == 0))) {
    T14 = ((((2.0*BSIM4type)*(((BSIM4phig-BSIM4easub)-(0.5*BSIM4Eg0))+0.45)))*UNITFAD).val();
  }
  else {
    T14 = ((0.0)*UNITFAD).val();
  }
  if((BSIM4mobMod == 0)) {
    {
      T0 = (((Vgsteff+Vth)+Vth)-T14);
      T2 = (BSIM4ua+(BSIM4uc*Vbseff));
      T3 = (T0/toxel);
      T12 = sqrt(((Vth*Vth)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*Vth);
      T6 = (T8*Vth);
      T5 = ((T3*(T2+(BSIM4ub*T3)))+T6);
    }
  }
  else if((BSIM4mobMod == 1)) {
    {
      T0 = (((Vgsteff+Vth)+Vth)-T14);
      T2 = (1.0+(BSIM4uc*Vbseff));
      T3 = (T0/toxel);
      T4 = (T3*(BSIM4ua+(BSIM4ub*T3)));
      T12 = sqrt(((Vth*Vth)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*Vth);
      T6 = (T8*Vth);
      T5 = ((T4*T2)+T6);
    }
  }
  else if((BSIM4mobMod == 2)) {
    {
      T0 = ((Vgsteff+here_BSIM4vtfbphi1)/toxel);
      T1 = exp((BSIM4eu*log(T0)));
      T2 = (BSIM4ua+(BSIM4uc*Vbseff));
      T12 = sqrt(((Vth*Vth)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*Vth);
      T6 = (T8*Vth);
      T5 = ((T1*T2)+T6);
    }
  }
  else if((BSIM4mobMod == 4)) {
    {
      T0 = ((Vgsteff+here_BSIM4vtfbphi1)-T14);
      T2 = (BSIM4ua+(BSIM4uc*Vbseff));
      T3 = (T0/toxel);
      T12 = sqrt(((here_BSIM4vtfbphi1*here_BSIM4vtfbphi1)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*here_BSIM4vtfbphi1);
      T6 = (T8*here_BSIM4vtfbphi1);
      T5 = ((T3*(T2+(BSIM4ub*T3)))+T6);
    }
  }
  else if((BSIM4mobMod == 5)) {
    {
      T0 = ((Vgsteff+here_BSIM4vtfbphi1)-T14);
      T2 = (1.0+(BSIM4uc*Vbseff));
      T3 = (T0/toxel);
      T4 = (T3*(BSIM4ua+(BSIM4ub*T3)));
      T12 = sqrt(((here_BSIM4vtfbphi1*here_BSIM4vtfbphi1)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*here_BSIM4vtfbphi1);
      T6 = (T8*here_BSIM4vtfbphi1);
      T5 = ((T4*T2)+T6);
    }
  }
  else if((BSIM4mobMod == 6)) {
    {
      T0 = ((Vgsteff+here_BSIM4vtfbphi1)/toxel);
      T1 = exp((BSIM4eu*log(T0)));
      T2 = (BSIM4ua+(BSIM4uc*Vbseff));
      T12 = sqrt(((here_BSIM4vtfbphi1*here_BSIM4vtfbphi1)+0.0001));
      T9 = (1.0/(Vgsteff+(2*T12)));
      T10 = (T9*toxel);
      T8 = (((BSIM4ud*T10)*T10)*here_BSIM4vtfbphi1);
      T6 = (T8*here_BSIM4vtfbphi1);
      T5 = ((T1*T2)+T6);
    }
  }
  else {
    {
      T0 = ((((Vgsteff+here_BSIM4vtfbphi1)*1.0e-8)/toxel)/6.0);
      T1 = exp((BSIM4eu*log(T0)));
      T2 = (BSIM4ua+(BSIM4uc*Vbseff));
      T10 = exp((BSIM4ucs*log((0.5+((0.5*Vgsteff)/BSIM4VgsteffVth)))));
      T11 = (BSIM4ud/T10);
      T5 = ((T1*T2)+T11);
    }
  }
  if((T5 >= -0.8)) {
    Denomi = (1.0+T5);
  }
  else {
    {
      T9 = (1.0/(7.0+(10.0*T5)));
      Denomi = ((0.6+T5)*T9);
    }
  }
  if((this->verbose == 1)) {
    printf("T0=%g,T2=%g,T3=%g,T12=%g,T9=%g,T10=%g,T5=%g,T6=%g,T8=%g\n",T0.val(),T2.val(),T3.val(),T12.val(),T9.val(),T10.val(),T5.val(),T6.val(),T8.val());
  }
  BSIM4ueff = (here_BSIM4u0temp/Denomi);
  ueff = BSIM4ueff;
  if((this->verbose == 1)) {
    printf("ueff=%g,here_BSIM4u0temp=%g,Denomi=%g\n",ueff.val(),((here_BSIM4u0temp)*UNITFAD).val(),Denomi.val());
  }
  WVCox = ((Weff*here_BSIM4vsattemp)*BSIM4coxe);
  WVCoxRds = (WVCox*Rds);
  Esat = ((2.0*here_BSIM4vsattemp)/ueff);
  EsatL = (Esat*Leff);
  if((this->verbose == 1)) {
    {
      printf("Esat,L=%g,%g\n",Esat.val(),EsatL.val());
    }
  }
  a1l = ((BSIM4a1)*UNITFAD).val();
  if((a1l == 0.0)) {
    Lambda = ((BSIM4a2)*UNITFAD).val();
  }
  else if((a1l > 0.0)) {
    {
      T0 = (1.0-BSIM4a2);
      T1 = ((T0-(BSIM4a1*Vgsteff))-0.0001);
      T2 = sqrt(((T1*T1)+(0.0004*T0)));
      Lambda = ((BSIM4a2+T0)-(0.5*(T1+T2)));
    }
  }
  else {
    {
      T1 = ((BSIM4a2+(BSIM4a1*Vgsteff))-0.0001);
      T2 = sqrt(((T1*T1)+(0.0004*BSIM4a2)));
      Lambda = (0.5*(T1+T2));
    }
  }
  if((this->verbose == 1)) {
    printf("Lambda=%g\n",Lambda.val());
  }
  Vgst2Vtm = (Vgsteff+(2.0*Vtm));
  if(((Rds == 0.0) && (Lambda == 1.0))) {
    {
      T0 = (1.0/((Abulk*EsatL)+Vgst2Vtm));
      T1 = (T0*T0);
      T2 = (Vgst2Vtm*T0);
      T3 = (EsatL*Vgst2Vtm);
      Vdsat = (T3*T0);
    }
  }
  else {
    {
      T9 = (Abulk*WVCoxRds);
      T8 = (Abulk*T9);
      T7 = (Vgst2Vtm*T9);
      T6 = (Vgst2Vtm*WVCoxRds);
      T0 = ((2.0*Abulk)*((T9-1.0)+(1.0/Lambda)));
      T1 = (((Vgst2Vtm*((2.0/Lambda)-1.0))+(Abulk*EsatL))+(3.0*T7));
      T2 = (Vgst2Vtm*(EsatL+(2.0*T6)));
      T3 = sqrt(((T1*T1)-((2.0*T0)*T2)));
      Vdsat = ((T1-T3)/T0);
    }
  }
  if((this->verbose == 1)) {
    printf("Vdsat=%g\n",Vdsat.val());
  }
  T1 = ((Vdsat-Vds)-BSIM4delta);
  T2 = sqrt(((T1*T1)+((4.0*BSIM4delta)*Vdsat)));
  T0 = (T1/T2);
  T9 = (2.0*BSIM4delta);
  T3 = (T9/T2);
  if((T1 >= 0.0)) {
    Vdseff = (Vdsat-(0.5*(T1+T2)));
  }
  else {
    {
      T4 = (T9/(T2-T1));
      T5 = (1.0-T4);
      T6 = ((Vdsat*T4)/(T2-T1));
      Vdseff = (Vdsat*T5);
    }
  }
  if((Vds == 0.0)) {
    Vdseff = 0.0;
  }
  if((Vdseff > Vds)) {
    Vdseff = Vds;
  }
  diffVds = (Vds-Vdseff);
  here_Vdseff = Vdseff;
  if((this->verbose == 1)) {
    printf("diffVds=%g\n",diffVds.val());
  }
  if((this->verbose == 1)) {
    printf("Vdseff=%g\n",Vdseff.val());
  }
  if((BSIM4lambdaGiven && (BSIM4lambda > 0.0))) {
    {
      T1 = (Leff*ueff);
      T2 = (BSIM4lambda/T1);
      T3 = ((-T2/T1)*Leff);
      T5 = (1.0/(Esat*BSIM4litl));
      T6 = (1.0+(diffVds*T5));
      T7 = (2.0/((T6*T6)+1.0));
      T8 = (1.0-T7);
      T10 = (1.0+(T2*T8));
      EsatL = (EsatL*T10);
      Esat = (EsatL/Leff);
    }
  }
  tmp4 = (1.0-(((0.5*Abulk)*Vdsat)/Vgst2Vtm));
  T9 = (WVCoxRds*Vgsteff);
  T8 = (T9/Vgst2Vtm);
  T0 = ((EsatL+Vdsat)+((2.0*T9)*tmp4));
  T9 = (WVCoxRds*Abulk);
  T1 = (((2.0/Lambda)-1.0)+T9);
  Vasat = (T0/T1);
  if((this->verbose == 1)) {
    printf("Vasat=%g\n",Vasat.val());
  }
  tmp1 = here_BSIM4vtfbphi2;
  tmp2 = (2.0e8*BSIM4toxp);
  T0 = ((Vgsteff+tmp1)/tmp2);
  tmp3 = exp(((BSIM4bdos*0.7)*log(T0)));
  T1 = (1.0+tmp3);
  T2 = (((BSIM4bdos*0.7)*tmp3)/T0);
  Tcen = ((BSIM4ados*1.9e-9)/T1);
  Coxeff = ((epssubl*BSIM4coxp)/(epssubl+(BSIM4coxp*Tcen)));
  CoxeffWovL = ((Coxeff*Weff)/Leff);
  beta = (ueff*CoxeffWovL);
  T3 = (ueff/Leff);
  AbovVgst2Vtm = (Abulk/Vgst2Vtm);
  T0 = (1.0-((0.5*Vdseff)*AbovVgst2Vtm));
  fgche1 = (Vgsteff*T0);
  T9 = (Vdseff/EsatL);
  fgche2 = (1.0+T9);
  gche = ((beta*fgche1)/fgche2);
  T0 = (1.0+(gche*Rds));
  Idl = (gche/T0);
  if((this->verbose == 1)) {
    printf("Idl=%g\n",Idl.val());
  }
  if((BSIM4fprout <= 0.0)) {
    {
      FP = ((1.0)*UNITFAD).val();
    }
  }
  else {
    {
      T9 = ((BSIM4fprout*sqrt(Leff))/Vgst2Vtm);
      FP = (1.0/(1.0+T9));
    }
  }
  if((this->verbose == 1)) {
    printf("FP=%g\n",FP.val());
  }
  T8 = (BSIM4pvag/EsatL);
  T9 = (T8*Vgsteff);
  if((T9 > -0.9)) {
    {
      PvagTerm = (1.0+T9);
    }
  }
  else {
    {
      T4 = (1.0/(17.0+(20.0*T9)));
      PvagTerm = ((0.8+T9)*T4);
    }
  }
  if(((BSIM4pclm > 1.713908431e-15) && (diffVds > 1.0e-10))) {
    {
      T0 = (1.0+(Rds*Idl));
      T2 = (Vdsat/Esat);
      T1 = (Leff+T2);
      Cclm = ((((FP*PvagTerm)*T0)*T1)/(BSIM4pclm*BSIM4litl));
      VACLM = (Cclm*diffVds);
    }
  }
  else {
    {
      VACLM = 5.834617425e14;
      Cclm = 5.834617425e14;
    }
  }
  if((this->verbose == 1)) {
    printf("VACLM=%g\n",VACLM.val());
  }
  if((BSIM4thetaRout > 1.713908431e-15)) {
    {
      T8 = (Abulk*Vdsat);
      T0 = (Vgst2Vtm*T8);
      T1 = (Vgst2Vtm+T8);
      T9 = (T1*T1);
      T2 = BSIM4thetaRout;
      VADIBL = ((Vgst2Vtm-(T0/T1))/T2);
      T7 = (BSIM4pdiblb*Vbseff);
      if((T7 >= -0.9)) {
        {
          T3 = (1.0/(1.0+T7));
          VADIBL = (VADIBL*T3);
        }
      }
      else {
        {
          T4 = (1.0/(0.8+T7));
          T3 = ((17.0+(20.0*T7))*T4);
          VADIBL = (VADIBL*T3);
        }
      }
      VADIBL = (VADIBL*PvagTerm);
    }
  }
  else {
    VADIBL = 5.834617425e14;
  }
  if((this->verbose == 1)) {
    printf("ELSE VADIBL=%g\n",VADIBL.val());
  }
  Va = (Vasat+VACLM);
  if((this->verbose == 1)) {
    printf("Va=%g\n",Va.val());
  }
  T0 = (BSIM4pditsd*Vds);
  if((T0 > 34.0)) {
    T1 = 5.834617425e14;
  }
  else {
    T1 = exp(T0);
  }
  if((BSIM4pdits > 1.713908431e-15)) {
    {
      T2 = (1.0+(BSIM4pditsl*Leff));
      VADITS = ((1.0+(T2*T1))/BSIM4pdits);
      VADITS = (VADITS*FP);
    }
  }
  else {
    VADITS = 5.834617425e14;
  }
  if((this->verbose == 1)) {
    printf("VADITS=%g\n",VADITS.val());
  }
  if(((BSIM4pscbe2 > 0.0) && (BSIM4pscbe1 >= 0.0))) {
    {
      if((diffVds > ((BSIM4pscbe1*BSIM4litl)/34.0))) {
        {
          T0 = ((BSIM4pscbe1*BSIM4litl)/diffVds);
          VASCBE = ((Leff*exp(T0))/BSIM4pscbe2);
        }
      }
      else {
        VASCBE = ((5.834617425e14*Leff)/BSIM4pscbe2);
      }
    }
  }
  else {
    VASCBE = 5.834617425e14;
  }
  if((this->verbose == 1)) {
    printf("VASCBE=%g\n",VASCBE.val());
  }
  T9 = (diffVds/VADIBL);
  T0 = (1.0+T9);
  if((this->verbose == 1)) {
    {
      printf("T9=%g\n",T9.val());
      printf("VADIBL=%g\n",VADIBL.val());
      printf("diffVds=%g\n",diffVds.val());
      printf("Idl=%g\n",Idl.val());
    }
  }
  Idsa = (Idl*T0);
  T9 = (diffVds/VADITS);
  if((this->verbose == 1)) {
    printf("T9=%g\n",T9.val());
  }
  T0 = (1.0+T9);
  Idsa = (Idsa*T0);
  if((this->verbose == 1)) {
    printf("Idsa=%g\n",Idsa.val());
  }
  T0 = log((Va/Vasat));
  T1 = (T0/Cclm);
  T9 = (1.0+T1);
  Idsa = (Idsa*T9);
  if((this->verbose == 1)) {
    printf("Idsa=%g\n",Idsa.val());
  }
  tmp = (((BSIM4alpha0+(BSIM4alpha1*Leff)))*UNITFAD).val();
  if(((tmp <= 0.0) || (BSIM4beta0 <= 0.0))) {
    {
      Isub = ((0.0)*UNITFAD).val();
      Gbd = ((0.0)*UNITFAD).val();
      Gbb = ((0.0)*UNITFAD).val();
      Gbg = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      T2 = (tmp/Leff);
      if((diffVds > (BSIM4beta0/34.0))) {
        {
          T0 = (-BSIM4beta0/diffVds);
          T1 = ((T2*diffVds)*exp(T0));
          T3 = ((T1/diffVds)*(T0-1.0));
        }
      }
      else {
        {
          T3 = (T2*1.713908431e-15);
          T1 = (T3*diffVds);
        }
      }
      T4 = (Idsa*Vdseff);
      Isub = (T1*T4);
    }
  }
  if((this->verbose == 1)) {
    printf("Isub=%g\n",Isub.val());
  }
  T9 = (diffVds/VASCBE);
  T0 = (1.0+T9);
  Ids = (Idsa*T0);
  cdrain = (Ids*Vdseff);
  if((this->verbose == 1)) {
    {
      printf("Ids=%g\n",Ids.val());
      printf("cdrain=%g\n",cdrain.val());
    }
  }
  if((BSIM4vtlGiven && (BSIM4vtl > 0.0))) {
    {
      T12 = ((1.0/Leff)/CoxeffWovL);
      T11 = (T12/Vgsteff);
      vs = (cdrain*T11);
      T0 = (2*3);
      T1 = (vs/(BSIM4vtl*BSIM4tfactor));
      if((T1 > 0.0)) {
        {
          T2 = (1.0+exp((T0*log(T1))));
          Fsevl = (1.0/exp((log(T2)/T0)));
        }
      }
      else {
        Fsevl = 1.0;
      }
      cdrain = (cdrain*Fsevl);
      if((this->verbose == 1)) {
        printf("cdrain (source end velocity limit)=%g\n",cdrain.val());
      }
    }
  }
  IdovVds = Ids;
  if((IdovVds <= 1.0e-9)) {
    IdovVds = 1.0e-9;
  }
  if((((BSIM4rgateMod > 1) || (BSIM4trnqsMod != 0)) || (BSIM4acnqsMod != 0))) {
    {
      T9 = (BSIM4xrcrg2*BSIM4vtm);
      T0 = (T9*beta);
      BSIM4gcrg = (BSIM4xrcrg1*(T0+Ids));
      if((BSIM4nf != 1.0)) {
        BSIM4gcrg = (BSIM4gcrg*BSIM4nf);
      }
      if((BSIM4rgateMod == 2)) {
        {
          T10 = (BSIM4grgeltd*BSIM4grgeltd);
          T11 = (BSIM4grgeltd+BSIM4gcrg);
          BSIM4gcrg = ((BSIM4grgeltd*BSIM4gcrg)/T11);
        }
      }
    }
  }
  if(BSIM4rdsMod) {
    {
      T0 = (vgs-BSIM4vfbsd);
      T1 = sqrt(((T0*T0)+1.0e-4));
      vgs_eff = (0.5*(T0+T1));
      T0 = (1.0+(BSIM4prwg*vgs_eff));
      T1 = (-BSIM4prwb*vbs);
      T2 = ((1.0/T0)+T1);
      T3 = (T2+sqrt(((T2*T2)+0.01)));
      T4 = (BSIM4rs0*0.5);
      Rs = (BSIM4rswmin+(T3*T4));
      if((this->verbose == 1)) {
        {
          printf("BSIM4vfbsd=%g\n",((BSIM4vfbsd)*UNITFAD).val());
          printf("BSIM4prwg=%g\n",((BSIM4prwg)*UNITFAD).val());
          printf("BSIM4prwb=%g\n",((BSIM4prwb)*UNITFAD).val());
          printf("BSIM4rs0=%g\n",((BSIM4rs0)*UNITFAD).val());
          printf("BSIM4rswmin=%g\n",((BSIM4rswmin)*UNITFAD).val());
          printf("vgs_eff=%g\n",vgs_eff.val());
          printf("T0=%g\n",T0.val());
          printf("T1=%g\n",T1.val());
          printf("T2=%g\n",T2.val());
          printf("T3=%g\n",T3.val());
          printf("T4=%g\n",T4.val());
          printf("Rs=%g\n",Rs.val());
        }
      }
      T0 = (vgd-BSIM4vfbsd);
      T1 = sqrt(((T0*T0)+1.0e-4));
      vgd_eff = (0.5*(T0+T1));
      T0 = (1.0+(BSIM4prwg*vgd_eff));
      T1 = (-BSIM4prwb*vbd);
      T2 = ((1.0/T0)+T1);
      T3 = (T2+sqrt(((T2*T2)+0.01)));
      T4 = (BSIM4rd0*0.5);
      Rd = (BSIM4rdwmin+(T3*T4));
    }
  }
  if((BSIM4mtrlmod == 0)) {
    T0 = (3.0*toxel);
  }
  else {
    T0 = ((BSIM4epsrsub*toxel)/epsroxl);
  }
  if((BSIM4gidlMod == 0)) {
    {
      if((BSIM4mtrlmod == 0)) {
        T1 = (((vds-vgs_eff)-BSIM4egidl)/T0);
      }
      else {
        T1 = ((((vds-vgs_eff)-BSIM4egidl)+BSIM4vfbsd)/T0);
      }
      if((((((BSIM4agidl <= 0.0) || (BSIM4bgidl <= 0.0)) || (T1 <= 0.0)) || (BSIM4cgidl <= 0.0)) || (vbd > 0.0))) {
        Igidl = ((0.0)*UNITFAD).val();
      }
      else {
        {
          T2 = (BSIM4bgidl/T1);
          if((T2 < 100.0)) {
            {
              Igidl = (((BSIM4agidl*BSIM4weffCJ)*T1)*exp(-T2));
              T3 = ((Igidl*(1.0+T2))/T1);
            }
          }
          else {
            {
              Igidl = ((BSIM4agidl*BSIM4weffCJ)*3.720075976e-44);
              Igidl = (Igidl*T1);
            }
          }
          T4 = (vbd*vbd);
          T5 = (-vbd*T4);
          T6 = (BSIM4cgidl+T5);
          T7 = (T5/T6);
          Igidl = (Igidl*T7);
        }
      }
      if((BSIM4mtrlmod == 0)) {
        T1 = (((-vds-vgd_eff)-BSIM4egisl)/T0);
      }
      else {
        T1 = ((((-vds-vgd_eff)-BSIM4egisl)+BSIM4vfbsd)/T0);
      }
      if((((((BSIM4agisl <= 0.0) || (BSIM4bgisl <= 0.0)) || (T1 <= 0.0)) || (BSIM4cgisl <= 0.0)) || (vbs > 0.0))) {
        Igisl = ((0.0)*UNITFAD).val();
      }
      else {
        {
          T2 = (BSIM4bgisl/T1);
          if((T2 < 100.0)) {
            {
              Igisl = (((BSIM4agisl*BSIM4weffCJ)*T1)*exp(-T2));
            }
          }
          else {
            {
              Igisl = ((BSIM4agisl*BSIM4weffCJ)*3.720075976e-44);
              Igisl = (Igisl*T1);
            }
          }
          T4 = (vbs*vbs);
          T5 = (-vbs*T4);
          T6 = (BSIM4cgisl+T5);
          T7 = (T5/T6);
          Igisl = (Igisl*T7);
        }
      }
    }
  }
  else {
    {
      if((BSIM4mtrlmod == 0)) {
        T1 = (((-vds-(BSIM4rgisl*vgd_eff))-BSIM4egisl)/T0);
      }
      else {
        T1 = ((((-vds-(BSIM4rgisl*vgd_eff))-BSIM4egisl)+BSIM4vfbsd)/T0);
      }
      if(((((BSIM4agisl <= 0.0) || (BSIM4bgisl <= 0.0)) || (T1 <= 0.0)) || (BSIM4cgisl < 0.0))) {
        Igisl = 0.0;
      }
      else {
        {
          T2 = (BSIM4bgisl/T1);
          if((T2 < 100.0)) {
            Igisl = (((BSIM4weffCJ*BSIM4agisl)*T1)*exp(-T2));
          }
          else {
            {
              T3 = ((BSIM4weffCJ*BSIM4agisl)*3.720075976e-44);
              Igisl = (T3*T1);
            }
          }
          T4 = (vbs-BSIM4fgisl);
          if((T4 == 0)) {
            T5 = 100.0;
          }
          else {
            T5 = (BSIM4kgisl/T4);
          }
          if((T5 < 100.0)) {
            T6 = exp(T5);
          }
          else {
            T6 = 2.688117142e+43;
          }
          Igisl = (Igisl*T6);
        }
      }
      if((BSIM4mtrlmod == 0)) {
        T1 = (((vds-(BSIM4rgidl*vgs_eff))-BSIM4egidl)/T0);
      }
      else {
        T1 = ((((vds-(BSIM4rgidl*vgs_eff))-BSIM4egidl)+BSIM4vfbsd)/T0);
      }
      if(((((BSIM4agidl <= 0.0) || (BSIM4bgidl <= 0.0)) || (T1 <= 0.0)) || (BSIM4cgidl < 0.0))) {
        Igidl = 0.0;
      }
      else {
        {
          dT1_dVd = (1/T0);
          dT1_dVg = ((-BSIM4rgidl*dT1_dVd)*dvgs_eff_dvg);
          T2 = (BSIM4bgidl/T1);
          if((T2 < 100.0)) {
            {
              Igidl = (((BSIM4weffCJ*BSIM4agidl)*T1)*exp(-T2));
            }
          }
          else {
            {
              T3 = ((BSIM4weffCJ*BSIM4agidl)*3.720075976e-44);
              Igidl = (T3*T1);
            }
          }
          T4 = (vbd-BSIM4fgidl);
          if((T4 == 0)) {
            T5 = 100.0;
          }
          else {
            T5 = (BSIM4kgidl/T4);
          }
          if((T5 < 100.0)) {
            T6 = exp(T5);
          }
          else {
            T6 = 2.688117142e+43;
          }
          Igidl = (Igidl*T6);
        }
      }
    }
  }
  if((this->verbose == 1)) {
    {
      printf("Igisl=%g\n",Igisl.val());
      printf("Igidl=%g\n",Igidl.val());
    }
  }
  if(((BSIM4igcMod != 0) || (BSIM4igbMod != 0))) {
    {
      Vfb = ((here_BSIM4vfbzb)*UNITFAD).val();
      V3 = (((Vfb-Vgs_eff)+Vbseff)-0.02);
      if((Vfb <= 0.0)) {
        T0 = sqrt(((V3*V3)-((4.0*0.02)*Vfb)));
      }
      else {
        T0 = sqrt(((V3*V3)+((4.0*0.02)*Vfb)));
      }
      T1 = (0.5*(1.0+(V3/T0)));
      Vfbeff = (Vfb-(0.5*(V3+T0)));
      Voxacc = (Vfb-Vfbeff);
      if((Voxacc < 0.0)) {
        Voxacc = 0.0;
      }
      T0 = (0.5*BSIM4k1ox);
      T3 = (((Vgs_eff-Vfbeff)-Vbseff)-Vgsteff);
      if((BSIM4k1ox == 0.0)) {
        Voxdepinv = ((0.0)*UNITFAD).val();
      }
          else if((T3 < 0.0)) {
        Voxdepinv = -T3;
      }
      else {
        {
          T1 = sqrt(((T0*T0)+T3));
          Voxdepinv = (BSIM4k1ox*(T1-T0));
        }
      }
      Voxdepinv = (Voxdepinv+Vgsteff);
    }
  }
  if((this->verbose == 1)) {
    printf("Voxdepinv=%g\n",Voxdepinv.val());
  }
  if((BSIM4tempMod < 2)) {
    tmp = ((Vtm)*UNITFAD).val();
  }
  else {
    tmp = ((Vtm0)*UNITFAD).val();
  }
  if(BSIM4igcMod) {
    {
      T0 = (tmp*BSIM4nigc);
      if((BSIM4igcMod == 1)) {
        {
          VxNVt = ((Vgs_eff-(BSIM4type*here_BSIM4vth0))/T0);
          if((VxNVt > 34.0)) {
            Vaux = (Vgs_eff-(BSIM4type*here_BSIM4vth0));
          }
        }
      }
          else if((BSIM4igcMod == 2)) {
        {
          VxNVt = ((Vgs_eff-Vth)/T0);
          if((VxNVt > 34.0)) {
            Vaux = (Vgs_eff-Vth);
          }
        }
      }
      if((VxNVt < -34.0)) {
        {
          Vaux = (T0*log((1.0+1.713908431e-15)));
        }
      }
          else if(((VxNVt >= -34.0) && (VxNVt <= 34.0))) {
        {
          ExpVxNVt = exp(VxNVt);
          Vaux = (T0*log((1.0+ExpVxNVt)));
        }
      }
      T2 = (Vgs_eff*Vaux);
      T11 = BSIM4Aechvb;
      T3 = ((BSIM4aigc*BSIM4cigc)-BSIM4bigc);
      T4 = (BSIM4bigc*BSIM4cigc);
      T5 = (BSIM4Bechvb*((BSIM4aigc+(T3*Voxdepinv))-((T4*Voxdepinv)*Voxdepinv)));
      if((T5 > 34.0)) {
        T6 = 5.834617425e14;
      }
          else if((T5 < -34.0)) {
        T6 = 1.713908431e-15;
      }
      else {
        T6 = exp(T5);
      }
      Igc = ((T11*T2)*T6);
      if(BSIM4pigcdGiven) {
        Pigcd = ((BSIM4pigcd)*UNITFAD).val();
      }
      else {
        {
          T11 = -BSIM4Bechvb;
          T12 = (Vgsteff+1.0e-20);
          T13 = ((T11/T12)/T12);
          T14 = (-T13/T12);
          Pigcd = (T13*(1.0-((0.5*Vdseff)/T12)));
        }
      }
      T7 = (-Pigcd*Vdseff);
      T8 = ((T7*T7)+2.0e-4);
      if((T7 > 34.0)) {
        T9 = 5.834617425e14;
      }
          else if((T7 < -34.0)) {
        T9 = 1.713908431e-15;
      }
      else {
        T9 = exp(T7);
      }
      T0 = (T8*T8);
      T1 = ((T9-1.0)+1.0e-4);
      T10 = ((T1-T7)/T8);
      Igcs = (Igc*T10);
      T1 = ((T9-1.0)-1.0e-4);
      T10 = (((T7*T9)-T1)/T8);
      Igcd = (Igc*T10);
      T0 = (vgs-(BSIM4vfbsd+BSIM4vfbsdoff));
      vgs_eff = sqrt(((T0*T0)+1.0e-4));
      T2 = (vgs*vgs_eff);
      T11 = BSIM4AechvbEdgeS;
      T12 = BSIM4BechvbEdge;
      T3 = ((BSIM4aigs*BSIM4cigs)-BSIM4bigs);
      T4 = (BSIM4bigs*BSIM4cigs);
      T5 = (T12*((BSIM4aigs+(T3*vgs_eff))-((T4*vgs_eff)*vgs_eff)));
      if((T5 > 34.0)) {
        T6 = 5.834617425e14;
      }
          else if((T5 < -34.0)) {
        T6 = 1.713908431e-15;
      }
      else {
        T6 = exp(T5);
      }
      Igs = ((T11*T2)*T6);
      if((this->verbose == 1)) {
        printf("Igs=%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n",Igs.val(),T11.val(),T2.val(),T6.val(),((BSIM4aigsd)*UNITFAD).val(),T5.val(),T3.val(),T4.val(),T12.val(),vgs_eff.val());
      }
      T0 = (vgd-(BSIM4vfbsd+BSIM4vfbsdoff));
      vgd_eff = sqrt(((T0*T0)+1.0e-4));
      T2 = (vgd*vgd_eff);
      T11 = BSIM4AechvbEdgeD;
      T3 = ((BSIM4aigd*BSIM4cigd)-BSIM4bigd);
      T4 = (BSIM4bigd*BSIM4cigd);
      T5 = (T12*((BSIM4aigd+(T3*vgd_eff))-((T4*vgd_eff)*vgd_eff)));
      if((T5 > 34.0)) {
        T6 = 5.834617425e14;
      }
          else if((T5 < -34.0)) {
        T6 = 1.713908431e-15;
      }
      else {
        T6 = exp(T5);
      }
      Igd = ((T11*T2)*T6);
      if((this->verbose == 1)) {
        printf("Igd=%g,%g,%g,%g\n",Igd.val(),T11.val(),T2.val(),T6.val());
      }
    }
  }
  else {
    {
      Igcs = 0.0;
      Igs = 0.0;
      Igcd = 0.0;
      Igd = 0.0;
    }
  }
  if(BSIM4igbMod) {
    {
      T0 = (tmp*BSIM4nigbacc);
      T1 = ((-Vgs_eff+Vbseff)+Vfb);
      VxNVt = (T1/T0);
      if((VxNVt > 34.0)) {
        Vaux = T1;
      }
          else if((VxNVt < -34.0)) {
        Vaux = (T0*log((1.0+1.713908431e-15)));
      }
      else {
        {
          ExpVxNVt = exp(VxNVt);
          Vaux = (T0*log((1.0+ExpVxNVt)));
        }
      }
      T2 = ((Vgs_eff-Vbseff)*Vaux);
      T11 = (((4.97232e-7*BSIM4weff)*BSIM4leff)*BSIM4ToxRatio);
      T12 = (-7.45669e11*toxel);
      T3 = ((BSIM4aigbacc*BSIM4cigbacc)-BSIM4bigbacc);
      T4 = (BSIM4bigbacc*BSIM4cigbacc);
      T5 = (T12*((BSIM4aigbacc+(T3*Voxacc))-((T4*Voxacc)*Voxacc)));
      if((T5 > 34.0)) {
        T6 = 5.834617425e14;
      }
          else if((T5 < -34.0)) {
        T6 = 1.713908431e-15;
      }
      else {
        T6 = exp(T5);
      }
      Igbacc = ((T11*T2)*T6);
      if((this->verbose == 1)) {
        printf("Igbacc=%g\n",Igbacc.val());
      }
      T0 = (tmp*BSIM4nigbinv);
      T1 = (Voxdepinv-BSIM4eigbinv);
      VxNVt = (T1/T0);
      if((VxNVt > 34.0)) {
        Vaux = T1;
      }
          else if((VxNVt < -34.0)) {
        Vaux = (T0*log((1.0+1.713908431e-15)));
      }
      else {
        {
          ExpVxNVt = exp(VxNVt);
          Vaux = (T0*log((1.0+ExpVxNVt)));
        }
      }
      T2 = ((Vgs_eff-Vbseff)*Vaux);
      T11 = (T11*0.75610);
      T12 = (T12*1.31724);
      T3 = ((BSIM4aigbinv*BSIM4cigbinv)-BSIM4bigbinv);
      T4 = (BSIM4bigbinv*BSIM4cigbinv);
      T5 = (T12*((BSIM4aigbinv+(T3*Voxdepinv))-((T4*Voxdepinv)*Voxdepinv)));
      if((T5 > 34.0)) {
        T6 = 5.834617425e14;
      }
          else if((T5 < -34.0)) {
        T6 = 1.713908431e-15;
      }
      else {
        T6 = exp(T5);
      }
      Igbinv = ((T11*T2)*T6);
      if((this->verbose == 1)) {
        printf("Igbinv=%g\n",Igbinv.val());
      }
      Igb = (Igbinv+Igbacc);
    }
  }
  else {
    Igb = 0.0;
  }
  if((BSIM4nf != 1.0)) {
    {
      cdrain = (cdrain*BSIM4nf);
      IdovVds = (IdovVds*BSIM4nf);
      Isub = (Isub*BSIM4nf);
      Igidl = (Igidl*BSIM4nf);
      Igisl = (Igisl*BSIM4nf);
      Igcs = (Igcs*BSIM4nf);
      Igs = (Igs*BSIM4nf);
      Igcd = (Igcd*BSIM4nf);
      Igd = (Igd*BSIM4nf);
      Igb = (Igb*BSIM4nf);
    }
  }
  if((BSIM4tnoiMod == 0)) {
    {
      Abulk = (Abulk0*BSIM4abulkCVfactor);
      Vdsat = (Vgsteff/Abulk);
      T0 = ((Vdsat-Vds)-0.02);
      T1 = sqrt(((T0*T0)+((4.0*0.02)*Vdsat)));
      if((T0 >= 0.0)) {
        Vdseff = (Vdsat-(0.5*(T0+T1)));
      }
      else {
        {
          T3 = ((0.02+0.02)/(T1-T0));
          T4 = (1.0-T3);
          T5 = ((Vdsat*T3)/(T1-T0));
          Vdseff = (Vdsat*T4);
        }
      }
      if((Vds == 0.0)) {
        Vdseff = 0.0;
      }
      T0 = (Abulk*Vdseff);
      T1 = (12.0*((Vgsteff-(0.5*T0))+1.0e-20));
      T2 = (Vdseff/T1);
      T3 = (T0*T2);
      BSIM4qinv = ((((Coxeff*BSIM4weffCV)*BSIM4nf)*BSIM4leffCV)*((Vgsteff-(0.5*T0))+(Abulk*T3)));
    }
  }
  if((BSIM4xpart < 0)) {
    {
      qgate = ((0.0)*UNITFAD).val();
      qdrn = ((0.0)*UNITFAD).val();
      qsrc = ((0.0)*UNITFAD).val();
      qbulk = ((0.0)*UNITFAD).val();
    }
  }
  else if((BSIM4capMod == 0)) {
    {
      if((Vbseff < 0.0)) {
        VbseffCV = Vbs;
      }
      else {
        VbseffCV = (BSIM4phi-Phis);
      }
      Vfb = ((BSIM4vfbcv)*UNITFAD).val();
      Vth = ((Vfb+BSIM4phi)+(BSIM4k1ox*sqrtPhis));
      Vgst = (Vgs_eff-Vth);
      CoxWL = (((((BSIM4coxe*BSIM4weffCV)*BSIM4leffCV)*BSIM4nf))*UNITFAD).val();
      Arg1 = ((Vgs_eff-VbseffCV)-Vfb);
      if((Arg1 <= 0.0)) {
        {
          qgate = (CoxWL*Arg1);
          qbulk = -qgate;
          qdrn = ((0.0)*UNITFAD).val();
        }
      }
          else if((Vgst <= 0.0)) {
        {
          T1 = (0.5*BSIM4k1ox);
          T2 = sqrt(((T1*T1)+Arg1));
          qgate = ((CoxWL*BSIM4k1ox)*(T2-T1));
          qbulk = -qgate;
          qdrn = ((0.0)*UNITFAD).val();
        }
      }
      else {
        {
          One_Third_CoxWL = (((CoxWL/3.0))*UNITFAD).val();
          Two_Third_CoxWL = (((2.0*One_Third_CoxWL))*UNITFAD).val();
          AbulkCV = (Abulk0*BSIM4abulkCVfactor);
          Vdsat = (Vgst/AbulkCV);
          if((BSIM4xpart > 0.5)) {
            {
              if((Vdsat <= Vds)) {
                {
                  T1 = (Vdsat/3.0);
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-T1));
                  T2 = (-Two_Third_CoxWL*Vgst);
                  qbulk = -(qgate+T2);
                  qdrn = ((0.0)*UNITFAD).val();
                }
              }
              else {
                {
                  Alphaz = (Vgst/Vdsat);
                  T1 = ((2.0*Vdsat)-Vds);
                  T2 = (Vds/(3.0*T1));
                  T3 = (T2*Vds);
                  T9 = (0.25*CoxWL);
                  T4 = (T9*Alphaz);
                  T7 = (((2.0*Vds)-T1)-(3.0*T3));
                  T8 = ((T3-T1)-(2.0*Vds));
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-(0.5*(Vds-T3))));
                  T10 = (T4*T8);
                  qdrn = (T4*T7);
                  qbulk = -((qgate+qdrn)+T10);
                }
              }
            }
          }
                  else if((BSIM4xpart < 0.5)) {
            {
              if((Vds >= Vdsat)) {
                {
                  T1 = (Vdsat/3.0);
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-T1));
                  T2 = (-Two_Third_CoxWL*Vgst);
                  qbulk = -(qgate+T2);
                  qdrn = (0.4*T2);
                }
              }
              else {
                {
                  Alphaz = (Vgst/Vdsat);
                  T1 = ((2.0*Vdsat)-Vds);
                  T2 = (Vds/(3.0*T1));
                  T3 = (T2*Vds);
                  T9 = (0.25*CoxWL);
                  T4 = (T9*Alphaz);
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-(0.5*(Vds-T3))));
                  T6 = ((((8.0*Vdsat)*Vdsat)-((6.0*Vdsat)*Vds))+((1.2*Vds)*Vds));
                  T8 = (T2/T1);
                  T7 = ((Vds-T1)-(T8*T6));
                  qdrn = (T4*T7);
                  T7 = (2.0*(T1+T3));
                  qbulk = -(qgate-(T4*T7));
                }
              }
            }
          }
          else {
            {
              if((Vds >= Vdsat)) {
                {
                  T1 = (Vdsat/3.0);
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-T1));
                  T2 = (-Two_Third_CoxWL*Vgst);
                  qbulk = -(qgate+T2);
                  qdrn = (0.5*T2);
                }
              }
              else {
                {
                  Alphaz = (Vgst/Vdsat);
                  T1 = ((2.0*Vdsat)-Vds);
                  T2 = (Vds/(3.0*T1));
                  T3 = (T2*Vds);
                  T9 = (0.25*CoxWL);
                  T4 = (T9*Alphaz);
                  qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM4phi)-(0.5*(Vds-T3))));
                  T7 = (T1+T3);
                  qdrn = (-T4*T7);
                  qbulk = -((qgate+qdrn)+qdrn);
                }
              }
            }
          }
        }
      }
    }
  }
  else {
    {
      if((Vbseff < 0.0)) {
        VbseffCV = Vbseff;
      }
      else {
        VbseffCV = (BSIM4phi-Phis);
      }
      CoxWL = (((((BSIM4coxe*BSIM4weffCV)*BSIM4leffCV)*BSIM4nf))*UNITFAD).val();
      if((BSIM4cvchargeMod == 0)) {
        {
          local_noff = (n*BSIM4noff);
          T0 = (Vtm*local_noff);
          local_voffcv = ((BSIM4voffcv)*UNITFAD).val();
          VgstNVt = ((Vgst-local_voffcv)/T0);
          if((VgstNVt > 34.0)) {
            {
              Vgsteff = (Vgst-local_voffcv);
            }
          }
                  else if((VgstNVt < -34.0)) {
            {
              Vgsteff = (T0*log((1.0+1.713908431e-15)));
            }
          }
          else {
            {
              ExpVgst = exp(VgstNVt);
              Vgsteff = (T0*log((1.0+ExpVgst)));
            }
          }
        }
      }
      else {
        {
          T0 = (n*Vtm);
          T1 = (BSIM4mstarcv*Vgst);
          T2 = (T1/T0);
          if((T2 > 34.0)) {
            T10 = T1;
          }
                  else if((T2 < -34.0)) {
            {
              T10 = (Vtm*log((1.0+1.713908431e-15)));
              T10 = (T10*n);
            }
          }
          else {
            {
              ExpVgst = exp(T2);
              T3 = (Vtm*log((1.0+ExpVgst)));
              T10 = (n*T3);
            }
          }
          T1 = (BSIM4voffcbncv-((1.0-BSIM4mstarcv)*Vgst));
          T2 = (T1/T0);
          if((T2 < -34.0)) {
            {
              T3 = ((BSIM4coxe*1.713908431e-15)/BSIM4cdep0);
              T9 = (BSIM4mstarcv+(T3*n));
            }
          }
                  else if((T2 > 34.0)) {
            {
              T3 = ((BSIM4coxe*5.834617425e14)/BSIM4cdep0);
              T9 = (BSIM4mstarcv+(T3*n));
            }
          }
          else {
            {
              ExpVgst = exp(T2);
              T3 = (BSIM4coxe/BSIM4cdep0);
              T4 = (T3*ExpVgst);
              T5 = ((T1*T4)/T0);
              T9 = (BSIM4mstarcv+(n*T4));
            }
          }
          Vgsteff = (T10/T9);
        }
      }
      if((BSIM4capMod == 1)) {
        {
          Vfb = ((here_BSIM4vfbzb)*UNITFAD).val();
          V3 = (((Vfb-Vgs_eff)+VbseffCV)-0.02);
          if((Vfb <= 0.0)) {
            T0 = sqrt(((V3*V3)-((4.0*0.02)*Vfb)));
          }
          else {
            T0 = sqrt(((V3*V3)+((4.0*0.02)*Vfb)));
          }
          Vfbeff = (Vfb-(0.5*(V3+T0)));
          Qac0 = (CoxWL*(Vfbeff-Vfb));
          T0 = (0.5*BSIM4k1ox);
          T3 = (((Vgs_eff-Vfbeff)-VbseffCV)-Vgsteff);
          if((BSIM4k1ox == 0.0)) {
            {
              T1 = 0.0;
            }
          }
                  else if((T3 < 0.0)) {
            {
              T1 = (T0+(T3/BSIM4k1ox));
            }
          }
          else {
            {
              T1 = sqrt(((T0*T0)+T3));
            }
          }
          Qsub0 = ((CoxWL*BSIM4k1ox)*(T1-T0));
          AbulkCV = (Abulk0*BSIM4abulkCVfactor);
          VdsatCV = (Vgsteff/AbulkCV);
          T0 = ((VdsatCV-Vds)-0.02);
          T1 = sqrt(((T0*T0)+((4.0*0.02)*VdsatCV)));
          if((T0 >= 0.0)) {
            {
              VdseffCV = (VdsatCV-(0.5*(T0+T1)));
            }
          }
          else {
            {
              T3 = ((0.02+0.02)/(T1-T0));
              T4 = (1.0-T3);
              VdseffCV = (VdsatCV*T4);
            }
          }
          if((Vds == 0.0)) {
            VdseffCV = 0.0;
          }
          T0 = (AbulkCV*VdseffCV);
          T1 = (12.0*((Vgsteff-(0.5*T0))+1.0e-20));
          T2 = (VdseffCV/T1);
          T3 = (T0*T2);
          qgate = (CoxWL*((Vgsteff-(0.5*VdseffCV))+T3));
          T7 = (1.0-AbulkCV);
          qbulk = ((CoxWL*T7)*((0.5*VdseffCV)-T3));
          if((BSIM4xpart > 0.5)) {
            {
              T1 = (T1+T1);
              qsrc = (-CoxWL*(((0.5*Vgsteff)+(0.25*T0))-((T0*T0)/T1)));
            }
          }
                  else if((BSIM4xpart < 0.5)) {
            {
              T1 = (T1/12.0);
              T2 = ((0.5*CoxWL)/(T1*T1));
              T3 = ((Vgsteff*((((2.0*T0)*T0)/3.0)+(Vgsteff*(Vgsteff-((4.0*T0)/3.0)))))-((((2.0*T0)*T0)*T0)/15.0));
              qsrc = (-T2*T3);
            }
          }
          else {
            {
              qsrc = (-0.5*(qgate+qbulk));
            }
          }
          qgate = (qgate+(Qac0+Qsub0));
          qbulk = (qbulk-(Qac0+Qsub0));
          qdrn = -((qgate+qbulk)+qsrc);
        }
      }
          else if((BSIM4capMod == 2)) {
        {
          V3 = (((here_BSIM4vfbzb-Vgs_eff)+VbseffCV)-0.02);
          if((here_BSIM4vfbzb <= 0.0)) {
            T0 = sqrt(((V3*V3)-((4.0*0.02)*here_BSIM4vfbzb)));
          }
          else {
            T0 = sqrt(((V3*V3)+((4.0*0.02)*here_BSIM4vfbzb)));
          }
          T1 = (0.5*(1.0+(V3/T0)));
          Vfbeff = (here_BSIM4vfbzb-(0.5*(V3+T0)));
          Cox = ((BSIM4coxp)*UNITFAD).val();
          Tox = (((1.0e8*BSIM4toxp))*UNITFAD).val();
          T0 = (((Vgs_eff-VbseffCV)-here_BSIM4vfbzb)/Tox);
          tmp = (T0*BSIM4acde);
          if(((-34.0 < tmp) && (tmp < 34.0))) {
            {
              Tcen = (BSIM4ldeb*exp(tmp));
            }
          }
                  else if((tmp <= -34.0)) {
            {
              Tcen = (BSIM4ldeb*1.713908431e-15);
            }
          }
          else {
            {
              Tcen = (BSIM4ldeb*5.834617425e14);
            }
          }
          LINK = (((1.0e-3*BSIM4toxp))*UNITFAD).val();
          V3 = ((BSIM4ldeb-Tcen)-LINK);
          V4 = sqrt(((V3*V3)+((4.0*LINK)*BSIM4ldeb)));
          Tcen = (BSIM4ldeb-(0.5*(V3+V4)));
          Ccen = (epssubl/Tcen);
          T2 = (Cox/(Cox+Ccen));
          Coxeff = (T2*Ccen);
          T3 = (-Ccen/Tcen);
          CoxWLcen = ((CoxWL*Coxeff)/BSIM4coxe);
          Qac0 = (CoxWLcen*(Vfbeff-here_BSIM4vfbzb));
          QovCox = (Qac0/Coxeff);
          T0 = (0.5*BSIM4k1ox);
          T3 = (((Vgs_eff-Vfbeff)-VbseffCV)-Vgsteff);
          if((BSIM4k1ox == 0.0)) {
            {
              T1 = 0.0;
              T2 = 0.0;
            }
          }
                  else if((T3 < 0.0)) {
            {
              T1 = (T0+(T3/BSIM4k1ox));
              T2 = CoxWLcen;
            }
          }
          else {
            {
              T1 = sqrt(((T0*T0)+T3));
              T2 = ((CoxWLcen*T0)/T1);
            }
          }
          Qsub0 = ((CoxWLcen*BSIM4k1ox)*(T1-T0));
          QovCox = (Qsub0/Coxeff);
          if((BSIM4k1ox <= 0.0)) {
            {
              Denomi = ((0.25*BSIM4moin)*Vtm);
              T0 = (0.5*BSIM4sqrtPhi);
            }
          }
          else {
            {
              Denomi = (((BSIM4moin*Vtm)*BSIM4k1ox)*BSIM4k1ox);
              T0 = (BSIM4k1ox*BSIM4sqrtPhi);
            }
          }
          T1 = ((2.0*T0)+Vgsteff);
          DeltaPhi = (Vtm*log((1.0+((T1*Vgsteff)/Denomi))));
          T0 = ((Vgsteff-DeltaPhi)-0.001);
          T1 = sqrt(((T0*T0)+(Vgsteff*0.004)));
          VgDP = (0.5*(T0+T1));
          Tox = (((Tox+Tox))*UNITFAD).val();
          T0 = ((Vgsteff+here_BSIM4vtfbphi2)/Tox);
          tmp = exp(((BSIM4bdos*0.7)*log(T0)));
          T1 = (1.0+tmp);
          Tcen = ((BSIM4ados*1.9e-9)/T1);
          Ccen = (epssubl/Tcen);
          T0 = (Cox/(Cox+Ccen));
          Coxeff = (T0*Ccen);
          CoxWLcen = ((CoxWL*Coxeff)/BSIM4coxe);
          AbulkCV = (Abulk0*BSIM4abulkCVfactor);
          VdsatCV = (VgDP/AbulkCV);
          T0 = ((VdsatCV-Vds)-0.02);
          T1 = sqrt(((T0*T0)+((4.0*0.02)*VdsatCV)));
          if((T0 >= 0.0)) {
            {
              VdseffCV = (VdsatCV-(0.5*(T0+T1)));
            }
          }
          else {
            {
              T3 = ((0.02+0.02)/(T1-T0));
              T4 = (1.0-T3);
              VdseffCV = (VdsatCV*T4);
            }
          }
          if((Vds == 0.0)) {
            VdseffCV = 0.0;
          }
          T0 = (AbulkCV*VdseffCV);
          T1 = VgDP;
          T2 = (12.0*((T1-(0.5*T0))+1.0e-20));
          T3 = (T0/T2);
          qgate = (CoxWLcen*(T1-(T0*(0.5-T3))));
          T7 = (1.0-AbulkCV);
          qbulk = ((CoxWLcen*T7)*((0.5*VdseffCV)-((T0*VdseffCV)/T2)));
          if((BSIM4xpart > 0.5)) {
            {
              qsrc = (-CoxWLcen*(((T1/2.0)+(T0/4.0))-(((0.5*T0)*T0)/T2)));
            }
          }
                  else if((BSIM4xpart < 0.5)) {
            {
              T2 = (T2/12.0);
              T3 = ((0.5*CoxWLcen)/(T2*T2));
              T4 = ((T1*((((2.0*T0)*T0)/3.0)+(T1*(T1-((4.0*T0)/3.0)))))-((((2.0*T0)*T0)*T0)/15.0));
              qsrc = (-T3*T4);
            }
          }
          else {
            {
              qsrc = (-0.5*qgate);
            }
          }
          qgate = (qgate+((Qac0+Qsub0)-qbulk));
          qbulk = (qbulk-(Qac0+Qsub0));
          qdrn = -((qgate+qbulk)+qsrc);
        }
      }
    }
  }
  if((BSIM4trnqsMod || BSIM4acnqsMod)) {
    {
      BSIM4qchqs = -(qbulk+qgate);
      qcheq = -(qbulk+qgate);
      CoxWL = (((((BSIM4coxe*BSIM4weffCV)*BSIM4nf)*BSIM4leffCV))*UNITFAD).val();
      T1 = (BSIM4gcrg/CoxWL);
      if(BSIM4acnqsMod) {
        BSIM4taunet = (1.0/T1);
      }
    }
  }
  czbd = (((BSIM4DunitAreaTempJctCap*BSIM4Adeff))*UNITFAD).val();
  czbs = (((BSIM4SunitAreaTempJctCap*BSIM4Aseff))*UNITFAD).val();
  czbdsw = (((BSIM4DunitLengthSidewallTempJctCap*BSIM4Pdeff))*UNITFAD).val();
  czbdswg = ((((BSIM4DunitLengthGateSidewallTempJctCap*BSIM4weffCJ)*BSIM4nf))*UNITFAD).val();
  czbssw = (((BSIM4SunitLengthSidewallTempJctCap*BSIM4Pseff))*UNITFAD).val();
  czbsswg = ((((BSIM4SunitLengthGateSidewallTempJctCap*BSIM4weffCJ)*BSIM4nf))*UNITFAD).val();
  MJS = ((BSIM4SbulkJctBotGradingCoeff)*UNITFAD).val();
  MJSWS = ((BSIM4SbulkJctSideGradingCoeff)*UNITFAD).val();
  MJSWGS = ((BSIM4SbulkJctGateSideGradingCoeff)*UNITFAD).val();
  MJD = ((BSIM4DbulkJctBotGradingCoeff)*UNITFAD).val();
  MJSWD = ((BSIM4DbulkJctSideGradingCoeff)*UNITFAD).val();
  MJSWGD = ((BSIM4DbulkJctGateSideGradingCoeff)*UNITFAD).val();
  if((vbs_jct == 0.0)) {
    BSIM4qbs = ((0.0)*UNITFAD).val();
  }
  else if((vbs_jct < 0.0)) {
    {
      if((czbs > 0.0)) {
        {
          arg = (1.0-(vbs_jct/BSIM4PhiBS));
          if((MJS == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJS*log(arg)));
          }
          BSIM4qbs = (((BSIM4PhiBS*czbs)*(1.0-(arg*sarg)))/(1.0-MJS));
        }
      }
      else {
        {
          BSIM4qbs = 0.0;
        }
      }
      if((czbssw > 0.0)) {
        {
          arg = (1.0-(vbs_jct/BSIM4PhiBSWS));
          if((MJSWS == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJSWS*log(arg)));
          }
          BSIM4qbs = (BSIM4qbs+(((BSIM4PhiBSWS*czbssw)*(1.0-(arg*sarg)))/(1.0-MJSWS)));
        }
      }
      if((czbsswg > 0.0)) {
        {
          arg = (1.0-(vbs_jct/BSIM4PhiBSWGS));
          if((MJSWGS == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJSWGS*log(arg)));
          }
          BSIM4qbs = (BSIM4qbs+(((BSIM4PhiBSWGS*czbsswg)*(1.0-(arg*sarg)))/(1.0-MJSWGS)));
        }
      }
    }
  }
  else {
    {
      T0 = ((czbs+czbssw)+czbsswg);
      T1 = (vbs_jct*((((czbs*MJS)/BSIM4PhiBS)+((czbssw*MJSWS)/BSIM4PhiBSWS))+((czbsswg*MJSWGS)/BSIM4PhiBSWGS)));
      BSIM4qbs = (vbs_jct*(T0+(0.5*T1)));
    }
  }
  if((vbd_jct == 0.0)) {
    {
      BSIM4qbd = ((0.0)*UNITFAD).val();
    }
  }
  else if((vbd_jct < 0.0)) {
    {
      if((czbd > 0.0)) {
        {
          arg = (1.0-(vbd_jct/BSIM4PhiBD));
          if((MJD == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJD*log(arg)));
          }
          BSIM4qbd = (((BSIM4PhiBD*czbd)*(1.0-(arg*sarg)))/(1.0-MJD));
        }
      }
      else {
        {
          BSIM4qbd = 0.0;
        }
      }
      if((czbdsw > 0.0)) {
        {
          arg = (1.0-(vbd_jct/BSIM4PhiBSWD));
          if((MJSWD == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJSWD*log(arg)));
          }
          BSIM4qbd = (BSIM4qbd+(((BSIM4PhiBSWD*czbdsw)*(1.0-(arg*sarg)))/(1.0-MJSWD)));
        }
      }
      if((czbdswg > 0.0)) {
        {
          arg = (1.0-(vbd_jct/BSIM4PhiBSWGD));
          if((MJSWGD == 0.5)) {
            sarg = (1.0/sqrt(arg));
          }
          else {
            sarg = exp((-MJSWGD*log(arg)));
          }
          BSIM4qbd = (BSIM4qbd+(((BSIM4PhiBSWGD*czbdswg)*(1.0-(arg*sarg)))/(1.0-MJSWGD)));
        }
      }
    }
  }
  else {
    {
      T0 = ((czbd+czbdsw)+czbdswg);
      T1 = (vbd_jct*((((czbd*MJD)/BSIM4PhiBD)+((czbdsw*MJSWD)/BSIM4PhiBSWD))+((czbdswg*MJSWGD)/BSIM4PhiBSWGD)));
      BSIM4qbd = (vbd_jct*(T0+(0.5*T1)));
    }
  }
  if((BSIM4rgateMod == 3)) {
    {
      vgdx = vgmd;
      vgsx = vgms;
    }
  }
  else {
    {
      vgdx = vgd;
      vgsx = vgs;
    }
  }
  if((BSIM4capMod == 0)) {
    {
      qgdo = (BSIM4cgdo*vgdx);
      qgso = (BSIM4cgso*vgsx);
    }
  }
  else {
    {
      T0 = (vgdx+0.02);
      T1 = sqrt(((T0*T0)+(4.0*0.02)));
      T2 = (0.5*(T0-T1));
      T3 = (BSIM4weffCV*BSIM4cgdl);
      T4 = sqrt((1.0-((4.0*T2)/BSIM4ckappad)));
      qgdo = (((BSIM4cgdo+T3)*vgdx)-(T3*(T2+((0.5*BSIM4ckappad)*(T4-1.0)))));
      T0 = (vgsx+0.02);
      T1 = sqrt(((T0*T0)+(4.0*0.02)));
      T2 = (0.5*(T0-T1));
      T3 = (BSIM4weffCV*BSIM4cgsl);
      T4 = sqrt((1.0-((4.0*T2)/BSIM4ckappas)));
      qgso = (((BSIM4cgso+T3)*vgsx)-(T3*(T2+((0.5*BSIM4ckappas)*(T4-1.0)))));
    }
  }
  if((BSIM4nf != 1.0)) {
    {
      qgdo = (qgdo*BSIM4nf);
      qgso = (qgso*BSIM4nf);
    }
  }
  if((BSIM4mode > 0)) {
    {
      if((BSIM4trnqsMod == 0)) {
        {
          qdrn = (qdrn-qgdo);
          if((BSIM4rgateMod == 3)) {
            {
              qgmb = (BSIM4cgbo*vgmb);
              qgmid = ((qgdo+qgso)+qgmb);
              qbulk = (qbulk-qgmb);
              qsrc = -(((qgate+qgmid)+qbulk)+qdrn);
            }
          }
          else {
            {
              qgb = (BSIM4cgbo*vgb);
              qgate = (((qgate+qgdo)+qgso)+qgb);
              qbulk = (qbulk-qgb);
              qsrc = -((qgate+qbulk)+qdrn);
            }
          }
        }
      }
      else {
        {
          qcheq = BSIM4qchqs;
          if((BSIM4rgateMod == 3)) {
            {
              qgmb = (BSIM4cgbo*vgmb);
              qgmid = ((qgdo+qgso)+qgmb);
              qgate = 0.0;
              qbulk = -qgmb;
              qdrn = -qgdo;
              qsrc = -((qgmid+qbulk)+qdrn);
            }
          }
          else {
            {
              qgb = (BSIM4cgbo*vgb);
              qgate = ((qgdo+qgso)+qgb);
              qbulk = -qgb;
              qdrn = -qgdo;
              qsrc = -((qgate+qbulk)+qdrn);
            }
          }
        }
      }
    }
  }
  else {
    {
      if((BSIM4trnqsMod == 0)) {
        {
          qsrc = (qdrn-qgso);
          if((BSIM4rgateMod == 3)) {
            {
              qgmb = (BSIM4cgbo*vgmb);
              qgmid = ((qgdo+qgso)+qgmb);
              qbulk = (qbulk-qgmb);
              qdrn = -(((qgate+qgmid)+qbulk)+qsrc);
            }
          }
          else {
            {
              qgb = (BSIM4cgbo*vgb);
              qgate = (((qgate+qgdo)+qgso)+qgb);
              qbulk = (qbulk-qgb);
              qdrn = -((qgate+qbulk)+qsrc);
            }
          }
        }
      }
      else {
        {
          if((BSIM4rgateMod == 3)) {
            {
              qgmb = (BSIM4cgbo*vgmb);
              qgmid = ((qgdo+qgso)+qgmb);
              qgate = 0.0;
              qbulk = -qgmb;
              qdrn = -qgdo;
              qsrc = -qgso;
            }
          }
          else {
            {
              qgb = (BSIM4cgbo*vgb);
              qgate = ((qgdo+qgso)+qgb);
              qbulk = -qgb;
              qdrn = -qgdo;
              qsrc = -qgso;
            }
          }
        }
      }
    }
  }
  if((BSIM4mode < 0)) {
    {
      cdrain = -cdrain;
    }
  }
//I-contrib...
  staticContributions[cogendaNodeID_di] += (BSIM4type*cdrain);
  staticContributions[cogendaNodeID_si] -= (BSIM4type*cdrain);
//I-contrib...
  staticContributions[cogendaNodeID_gi] += (BSIM4type*Igs);
  staticContributions[cogendaNodeID_si] -= (BSIM4type*Igs);
//I-contrib...
  staticContributions[cogendaNodeID_gi] += (BSIM4type*Igd);
  staticContributions[cogendaNodeID_di] -= (BSIM4type*Igd);
//I-contrib...
  staticContributions[cogendaNodeID_di] += (BSIM4type*(Isub+Igidl));
  staticContributions[cogendaNodeID_bi] -= (BSIM4type*(Isub+Igidl));
//I-contrib...
  staticContributions[cogendaNodeID_si] += (BSIM4type*Igisl);
  staticContributions[cogendaNodeID_bi] -= (BSIM4type*Igisl);
  if(BSIM4igcMod) {
    {
//I-contrib...
      staticContributions[cogendaNodeID_gi] += (BSIM4type*Igcs);
      staticContributions[cogendaNodeID_si] -= (BSIM4type*Igcs);
//I-contrib...
      staticContributions[cogendaNodeID_gi] += (BSIM4type*Igcd);
      staticContributions[cogendaNodeID_di] -= (BSIM4type*Igcd);
    }
  }
//I-contrib...
  staticContributions[cogendaNodeID_gi] += (BSIM4type*Igb);
  staticContributions[cogendaNodeID_bi] -= (BSIM4type*Igb);
  if(BSIM4rdsMod) {
    {
      Issi = (probeVars[cogendaProbeID_V_s_si]*(BSIM4sourceConductance/(1+(Rs*BSIM4sourceConductance))));
      Iddi = (probeVars[cogendaProbeID_V_d_di]*(BSIM4drainConductance/(1+(Rd*BSIM4drainConductance))));
      if((this->verbose == 1)) {
        {
          printf("In rdsmod=1 Issi=%g\n",Issi.val());
          printf("In rdsmod Iddi=%g\n",Iddi.val());
        }
      }
//I-contrib...
      staticContributions[cogendaNodeID_s] += Issi;
      staticContributions[cogendaNodeID_si] -= Issi;
//I-contrib...
      staticContributions[cogendaNodeID_d] += Iddi;
      staticContributions[cogendaNodeID_di] -= Iddi;
    }
  }
  else {
    {
//V-contrib...
//ZERO_For_Collapsed_Node
//V-contrib...
//ZERO_For_Collapsed_Node
    }
  }
  switch(BSIM4rgateMod){
  case 0:
    {
//V-contrib...
//ZERO_For_Collapsed_Node
//V-contrib...
//ZERO_For_Collapsed_Node
    }
    break;
  case 1:
    {
//I-contrib...
      staticContributions[cogendaNodeID_g] += (probeVars[cogendaProbeID_V_g_gm]*BSIM4grgeltd);
      staticContributions[cogendaNodeID_gm] -= (probeVars[cogendaProbeID_V_g_gm]*BSIM4grgeltd);
//V-contrib...
//ZERO_For_Collapsed_Node
    }
    break;
  case 2:
    {
//V-contrib...
//ZERO_For_Collapsed_Node
//I-contrib...
      staticContributions[cogendaNodeID_gm] += (probeVars[cogendaProbeID_V_gm_gi]*(BSIM4gcrg+BSIM4grgeltd));
      staticContributions[cogendaNodeID_gi] -= (probeVars[cogendaProbeID_V_gm_gi]*(BSIM4gcrg+BSIM4grgeltd));
    }
    break;
  case 3:
    {
      if((this->verbose == 1)) {
        {
          printf("BSIM4grgeltd=%g\n",((BSIM4grgeltd)*UNITFAD).val());
          printf("BSIM4gcrg=%g\n",BSIM4gcrg.val());
        }
      }
//I-contrib...
      staticContributions[cogendaNodeID_g] += (probeVars[cogendaProbeID_V_g_gm]*BSIM4grgeltd);
      staticContributions[cogendaNodeID_gm] -= (probeVars[cogendaProbeID_V_g_gm]*BSIM4grgeltd);
//I-contrib...
      staticContributions[cogendaNodeID_gm] += (probeVars[cogendaProbeID_V_gm_gi]*BSIM4gcrg);
      staticContributions[cogendaNodeID_gi] -= (probeVars[cogendaProbeID_V_gm_gi]*BSIM4gcrg);
    }
    break;
  }
  if(BSIM4rbodyMod) {
    {
//I-contrib...
      staticContributions[cogendaNodeID_sbulk] += (probeVars[cogendaProbeID_V_sbulk_bi]*BSIM4grbps);
      staticContributions[cogendaNodeID_bi] -= (probeVars[cogendaProbeID_V_sbulk_bi]*BSIM4grbps);
//I-contrib...
      staticContributions[cogendaNodeID_sbulk] += (probeVars[cogendaProbeID_V_sbulk_b]*BSIM4grbsb);
      staticContributions[cogendaNodeID_b] -= (probeVars[cogendaProbeID_V_sbulk_b]*BSIM4grbsb);
//I-contrib...
      staticContributions[cogendaNodeID_b] += (probeVars[cogendaProbeID_V_b_bi]*BSIM4grbpb);
      staticContributions[cogendaNodeID_bi] -= (probeVars[cogendaProbeID_V_b_bi]*BSIM4grbpb);
//I-contrib...
      staticContributions[cogendaNodeID_b] += (probeVars[cogendaProbeID_V_b_dbulk]*BSIM4grbdb);
      staticContributions[cogendaNodeID_dbulk] -= (probeVars[cogendaProbeID_V_b_dbulk]*BSIM4grbdb);
//I-contrib...
      staticContributions[cogendaNodeID_bi] += (probeVars[cogendaProbeID_V_bi_dbulk]*BSIM4grbpd);
      staticContributions[cogendaNodeID_dbulk] -= (probeVars[cogendaProbeID_V_bi_dbulk]*BSIM4grbpd);
    }
  }
  else {
    {
//V-contrib...
//ZERO_For_Collapsed_Node
//V-contrib...
//ZERO_For_Collapsed_Node
//V-contrib...
//ZERO_For_Collapsed_Node
    }
  }
//I-contrib...
  staticContributions[cogendaNodeID_sbulk] += (BSIM4type*BSIM4cbs);
  staticContributions[cogendaNodeID_si] -= (BSIM4type*BSIM4cbs);
//I-contrib...
  staticContributions[cogendaNodeID_dbulk] += (BSIM4type*BSIM4cbd);
  staticContributions[cogendaNodeID_di] -= (BSIM4type*BSIM4cbd);
  if((this->verbose == 1)) {
    {
      printf("BSIM4Ibs,gbs=%g,%g\n",BSIM4cbs.val(),BSIM4gbs.val());
      printf("BSIM4Ibd,gbd=%g,%g\n",BSIM4cbd.val(),BSIM4gbd.val());
      if(BSIM4rbodyMod) {
        {
          printf("Vbody(bi,db,sb)=%g,%g,%g\n",probeVars[cogendaProbeID_V_bi_s].val(),probeVars[cogendaProbeID_V_dbulk_s].val(),probeVars[cogendaProbeID_V_sbulk_s].val());
          printf("rbodymod,grbxx=%g,%g,%g,%g,%g,%g\n",BSIM4rbodyMod,((BSIM4grbps)*UNITFAD).val(),((BSIM4grbsb)*UNITFAD).val(),((BSIM4grbpb)*UNITFAD).val(),((BSIM4grbdb)*UNITFAD).val(),((BSIM4grbpd)*UNITFAD).val());
        }
      }
    }
  }
//Q-contrib...
  dynamicContributions[cogendaNodeID_gi] += (BSIM4type*(qgate));
  dynamicContributions[cogendaNodeID_si] -= (BSIM4type*(qgate));
//Q-contrib...
  dynamicContributions[cogendaNodeID_di] += (BSIM4type*(qdrn));
  dynamicContributions[cogendaNodeID_si] -= (BSIM4type*(qdrn));
//Q-contrib...
  dynamicContributions[cogendaNodeID_bi] += (BSIM4type*(qbulk));
  dynamicContributions[cogendaNodeID_si] -= (BSIM4type*(qbulk));
//Q-contrib...
  dynamicContributions[cogendaNodeID_sbulk] += (BSIM4type*(BSIM4qbs));
  dynamicContributions[cogendaNodeID_si] -= (BSIM4type*(BSIM4qbs));
//Q-contrib...
  dynamicContributions[cogendaNodeID_dbulk] += (BSIM4type*(BSIM4qbd));
  dynamicContributions[cogendaNodeID_di] -= (BSIM4type*(BSIM4qbd));
  if((this->verbose == 1)) {
    {
      printf("qgate=%g\n",qgate.val());
      printf("qdrn =%g\n",qdrn.val());
      printf("qsrc =%g\n",qsrc.val());
      printf("qgmid=%g\n",qgmid.val());
      printf("qbulk=%g\n",qbulk.val());
      printf("qgb  =%g\n",qgb.val());
      printf("qgmb =%g\n",qgmb.val());
      printf("qgdo =%g\n",qgdo.val());
      printf("qgso =%g\n",qgso.val());
      printf("here->BSIM4qbs=%g\n",BSIM4qbs.val());
      printf("here->BSIM4qbd=%g\n",BSIM4qbd.val());
      printf("BSIM4type=%d\n",BSIM4type);
      printf("BSIM4mode=%d\n",BSIM4mode);
    }
  }
  if((BSIM4tnoiMod == 0)) {
    {
      if((BSIM4rdsMod == 0)) {
        {
          if((BSIM4sourceConductance != 1000)) {
            gspr = ((BSIM4sourceConductance)*UNITFAD).val();
          }
          else {
            gspr = ((0)*UNITFAD).val();
          }
          if((BSIM4drainConductance != 1000)) {
            gdpr = ((BSIM4drainConductance)*UNITFAD).val();
          }
          else {
            gspr = ((0)*UNITFAD).val();
          }
          if((grdsw > 0.0)) {
            inv_grdsw = (1.0/grdsw);
          }
          else {
            inv_grdsw = 0.0;
          }
        }
      }
      else {
        {
          gspr = ((BSIM4gstot)*UNITFAD).val();
          gdpr = ((BSIM4gdtot)*UNITFAD).val();
          inv_grdsw = 0.0;
        }
      }
      npart_beta = ((0.0)*UNITFAD).val();
      npart_theta = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      T5 = (here_Vgsteff/EsatL);
      T5 = (T5*T5);
      npart_beta = (BSIM4rnoia*(1.0+((T5*BSIM4tnoia)*Leff)));
      npart_theta = (BSIM4rnoib*(1.0+((T5*BSIM4tnoib)*Leff)));
      if((BSIM4rdsMod == 0)) {
        {
          gspr = ((BSIM4sourceConductance)*UNITFAD).val();
          gdpr = ((BSIM4drainConductance)*UNITFAD).val();
        }
      }
      else {
        {
          gspr = ((BSIM4gstot)*UNITFAD).val();
          gdpr = ((BSIM4gdtot)*UNITFAD).val();
        }
      }
      if((vds >= 0.0)) {
        gspr = (gspr/(1.0+(((npart_theta*npart_theta)*gspr)/IdovVds)));
      }
      else {
        gdpr = (gdpr/(1.0+(((npart_theta*npart_theta)*gdpr)/IdovVds)));
      }
    }
  }
//I-contrib...
//ZERO_For_Small_Signal_Node;
//I-contrib...
//ZERO_For_Small_Signal_Node;
  if(((BSIM4rgateMod == 1) || (BSIM4rgateMod == 2))) {
//I-contrib...
//ZERO_For_Small_Signal_Node;
  }
  else if((BSIM4rgateMod == 3)) {
//I-contrib...
//ZERO_For_Small_Signal_Node;
  }
  if(BSIM4rbodyMod) {
    {
//V-contrib...
//ZERO_For_Small_Signal_Node;
//V-contrib...
//ZERO_For_Small_Signal_Node;
//V-contrib...
//ZERO_For_Small_Signal_Node;
//V-contrib...
//ZERO_For_Small_Signal_Node;
//V-contrib...
//ZERO_For_Small_Signal_Node;
    }
  }
  switch(BSIM4tnoiMod){
  case 0:
    {
      T0 = (ueff*fabs(BSIM4qinv));
      T1 = ((T0*inv_grdsw)+(Leff*Leff));
      thermalNoiseContrib = ((T0/T1)*BSIM4ntnoi);
    }
    break;
  case 1:
    {
      T0 = ((Gm+Gmb)+Gds);
      T0 = (T0*T0);
      igsquare = (((npart_theta*npart_theta)*T0)/IdovVds);
      T1 = ((npart_beta*(Gm+Gmb))+Gds);
      T2 = ((T1*T1)/IdovVds);
      thermalNoiseContrib = (T2-igsquare);
    }
    break;
  }
//I-contrib...
//ZERO_For_Small_Signal_Node;
  if((BSIM4mode >= 0)) {
    {
//I-contrib...
//ZERO_For_Small_Signal_Node;
//I-contrib...
//ZERO_For_Small_Signal_Node;
    }
  }
  else {
    {
//I-contrib...
//ZERO_For_Small_Signal_Node;
//I-contrib...
//ZERO_For_Small_Signal_Node;
    }
  }
//I-contrib...
//ZERO_For_Small_Signal_Node;
  switch(BSIM4fnoiMod){
  case 0:
    {
      flickerNoiseContrib = ((BSIM4kf*pow(fabs(cdrain),BSIM4af))/((BSIM4coxe*Leff)*Leff));
    }
    break;
  case 1:
    {
      if((vds < 0.0)) {
        vds = -vds;
      }
      esat = ((2.0*BSIM4vsattemp)/ueff);
      if((BSIM4em <= 0.0)) {
        DelClm = ((0.0)*UNITFAD).val();
      }
      else {
        {
          T0 = ((((Vds-here_Vdseff)/BSIM4litl)+BSIM4em)/esat);
          DelClm = (BSIM4litl*log(max(T0,1.0e-38)));
        }
      }
      T1 = (((((1.602176462e-19*1.602176462e-19)*1.3806503e-23)*fabs(cdrain))*T)*ueff);
      T2 = ((((1.0e10*here_Abulk)*BSIM4coxe)*Leff)*Leff);
      N0 = ((BSIM4coxe*here_Vgsteff)/1.602176462e-19);
      Nl = (((BSIM4coxe*here_Vgsteff)*(1.0-(AbovVgst2Vtm*here_Vdseff)))/1.602176462e-19);
      T3 = (BSIM4oxideTrapDensityA*log(max(((N0+BSIM4nstar)/(Nl+BSIM4nstar)),1.0e-38)));
      T4 = (BSIM4oxideTrapDensityB*(N0-Nl));
      T5 = ((BSIM4oxideTrapDensityC*0.5)*((N0*N0)-(Nl*Nl)));
      T6 = (((1.3806503e-23*T)*cdrain)*cdrain);
      T7 = (((1.0e10*Leff)*Leff)*Weff);
      T8 = ((BSIM4oxideTrapDensityA+(BSIM4oxideTrapDensityB*Nl))+((BSIM4oxideTrapDensityC*Nl)*Nl));
      T9 = ((Nl+BSIM4nstar)*(Nl+BSIM4nstar));
      Ssi = (((T1/T2)*((T3+T4)+T5))+((((T6/T7)*DelClm)*T8)/T9));
      T10 = ((BSIM4oxideTrapDensityA*1.3806503e-23)*T);
      T11 = ((((Weff*Leff)*1.0e10)*BSIM4nstar)*BSIM4nstar);
      Swi = (((T10/T11)*cdrain)*cdrain);
      T1 = (Swi+Ssi);
      if((T1 > 0.0)) {
        flickerNoiseContrib = ((Ssi*Swi)/T1);
      }
      else {
        flickerNoiseContrib = 0.0;
      }
    }
    break;
  }
//I-contrib...
//ZERO_For_Small_Signal_Node;
}
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 4 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 11; unCtNode++) {
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
  (*f_b_Equ_b_Node_Ptr) +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_sbulk_b)  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_bi)  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_dbulk);
  (*f_b_Equ_bi_Node_Ptr) +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_bi);
  (*f_b_Equ_dbulk_Node_Ptr) +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_dbulk);
  (*f_b_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_sbulk_b);
  (*f_bi_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_b_bi);
  (*f_bi_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_bi)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_b_bi)  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_dbulk);
  (*f_bi_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_dbulk);
  (*f_bi_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si);
  (*f_bi_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  (*f_bi_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_bi);
  (*f_bi_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  (*f_d_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_bi_si);
  (*f_d_Equ_d_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  (*f_d_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_dbulk_si);
  (*f_d_Equ_di_Node_Ptr) +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di)  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_di_si);
  (*f_d_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_gi_si);
  (*f_d_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_sbulk_si);
  (*f_d_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_gi_si);
  (*f_dbulk_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_b_dbulk);
  (*f_dbulk_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_dbulk)  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si);
  (*f_dbulk_Equ_dbulk_Node_Ptr) +=  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_b_dbulk)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_dbulk)  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si);
  (*f_dbulk_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si);
  (*f_dbulk_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  (*f_dbulk_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  (*f_di_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si);
  (*f_di_Equ_d_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  (*f_di_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si);
  (*f_di_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  (*f_di_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  (*f_di_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si);
  (*f_di_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  (*f_g_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_gm);
  (*f_g_Equ_gm_Node_Ptr) +=  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_gm);
  (*f_gi_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si);
  (*f_gi_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si);
  (*f_gi_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si);
  (*f_gi_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_gi);
  (*f_gi_Equ_gm_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_gi);
  (*f_gi_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si);
  (*f_gi_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si);
  (*f_gm_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_bi_si);
  (*f_gm_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_dbulk_si);
  (*f_gm_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_di_si);
  (*f_gm_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_g_gm);
  (*f_gm_Equ_gi_Node_Ptr) +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gm_gi)  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gi_si);
  (*f_gm_Equ_gm_Node_Ptr) +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_g_gm)  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gm_gi);
  (*f_gm_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_sbulk_si);
  (*f_gm_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gi_si);
  (*f_s_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_bi_si);
  (*f_s_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_dbulk_si);
  (*f_s_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_di_si);
  (*f_s_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_gi_si);
  (*f_s_Equ_s_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  (*f_s_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_sbulk_si);
  (*f_s_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_gi_si);
  (*f_sbulk_Equ_b_Node_Ptr) +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_b);
  (*f_sbulk_Equ_bi_Node_Ptr) +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_bi)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si);
  (*f_sbulk_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  (*f_sbulk_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si);
  (*f_sbulk_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_bi)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_b)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si);
  (*f_sbulk_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  (*f_si_Equ_bi_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si);
  (*f_si_Equ_dbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si);
  (*f_si_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  (*f_si_Equ_gi_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si);
  (*f_si_Equ_s_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
  (*f_si_Equ_sbulk_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si);
  (*f_si_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
#else
  //use the offsets instead of pointers
  dFdx[li_b][m_b_Equ_b_NodeOffset] +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_sbulk_b)  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_bi)  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_dbulk);
  dFdx[li_b][m_b_Equ_bi_NodeOffset] +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_bi);
  dFdx[li_b][m_b_Equ_dbulk_NodeOffset] +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_dbulk);
  dFdx[li_b][m_b_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_sbulk_b);
  dFdx[li_bi][m_bi_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_b_bi);
  dFdx[li_bi][m_bi_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_bi)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_b_bi)  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_dbulk);
  dFdx[li_bi][m_bi_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_dbulk);
  dFdx[li_bi][m_bi_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si);
  dFdx[li_bi][m_bi_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  dFdx[li_bi][m_bi_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  +staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_bi);
  dFdx[li_bi][m_bi_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  dFdx[li_d][m_d_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_bi_si);
  dFdx[li_d][m_d_Equ_d_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  dFdx[li_d][m_d_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_d][m_d_Equ_di_NodeOffset] +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di)  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_di_si);
  dFdx[li_d][m_d_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_gi_si);
  dFdx[li_d][m_d_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_d][m_d_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_gi_si);
  dFdx[li_dbulk][m_dbulk_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_b_dbulk);
  dFdx[li_dbulk][m_dbulk_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_dbulk)  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si);
  dFdx[li_dbulk][m_dbulk_Equ_dbulk_NodeOffset] +=  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_b_dbulk)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_dbulk)  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_dbulk][m_dbulk_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si);
  dFdx[li_dbulk][m_dbulk_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_dbulk][m_dbulk_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_di][m_di_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si);
  dFdx[li_di][m_di_Equ_d_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  dFdx[li_di][m_di_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_di][m_di_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  dFdx[li_di][m_di_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  dFdx[li_di][m_di_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_di][m_di_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  dFdx[li_g][m_g_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_gm);
  dFdx[li_g][m_g_Equ_gm_NodeOffset] +=  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_gm);
  dFdx[li_gi][m_gi_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si);
  dFdx[li_gi][m_gi_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_gi][m_gi_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si);
  dFdx[li_gi][m_gi_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_gi);
  dFdx[li_gi][m_gi_Equ_gm_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_gi);
  dFdx[li_gi][m_gi_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_gi][m_gi_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si);
  dFdx[li_gm][m_gm_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_bi_si);
  dFdx[li_gm][m_gm_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_gm][m_gm_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_di_si);
  dFdx[li_gm][m_gm_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_g_gm);
  dFdx[li_gm][m_gm_Equ_gi_NodeOffset] +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gm_gi)  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gi_si);
  dFdx[li_gm][m_gm_Equ_gm_NodeOffset] +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_g_gm)  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gm_gi);
  dFdx[li_gm][m_gm_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_gm][m_gm_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_gm].dx(cogendaProbeID_V_gi_si);
  dFdx[li_s][m_s_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_bi_si);
  dFdx[li_s][m_s_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_s][m_s_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_di_si);
  dFdx[li_s][m_s_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_gi_si);
  dFdx[li_s][m_s_Equ_s_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  dFdx[li_s][m_s_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_s][m_s_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_gi_si);
  dFdx[li_sbulk][m_sbulk_Equ_b_NodeOffset] +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_b);
  dFdx[li_sbulk][m_sbulk_Equ_bi_NodeOffset] +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_bi)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si);
  dFdx[li_sbulk][m_sbulk_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_sbulk][m_sbulk_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si);
  dFdx[li_sbulk][m_sbulk_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_bi)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_b)  +staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_sbulk][m_sbulk_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_si][m_si_Equ_bi_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si);
  dFdx[li_si][m_si_Equ_dbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si);
  dFdx[li_si][m_si_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  dFdx[li_si][m_si_Equ_gi_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si);
  dFdx[li_si][m_si_Equ_s_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
  dFdx[li_si][m_si_Equ_sbulk_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si);
  dFdx[li_si][m_si_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_bi_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si);
  (*q_bi_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si);
  (*q_bi_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si);
  (*q_bi_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  (*q_bi_Equ_gm_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gm_si);
  (*q_bi_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si);
  (*q_bi_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gm_si);
  (*q_dbulk_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si);
  (*q_dbulk_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si);
  (*q_dbulk_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si);
  (*q_dbulk_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_gi_si);
  (*q_dbulk_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  (*q_dbulk_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_gi_si);
  (*q_di_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si);
  (*q_di_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si);
  (*q_di_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  (*q_di_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  (*q_di_Equ_gm_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gm_si);
  (*q_di_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si);
  (*q_di_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gm_si);
  (*q_gi_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si);
  (*q_gi_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si);
  (*q_gi_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si);
  (*q_gi_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si);
  (*q_gi_Equ_gm_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_si);
  (*q_gi_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si);
  (*q_gi_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_si);
  (*q_sbulk_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si);
  (*q_sbulk_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  (*q_sbulk_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si);
  (*q_sbulk_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_gi_si);
  (*q_sbulk_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si);
  (*q_sbulk_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_gi_si);
  (*q_si_Equ_bi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si);
  (*q_si_Equ_dbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si);
  (*q_si_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  (*q_si_Equ_gi_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si);
  (*q_si_Equ_gm_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gm_si);
  (*q_si_Equ_sbulk_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si);
  (*q_si_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gm_si);
#else
  //use the offsets instead of pointers
  dQdx[li_bi][m_bi_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si);
  dQdx[li_bi][m_bi_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_bi][m_bi_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si);
  dQdx[li_bi][m_bi_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si);
  dQdx[li_bi][m_bi_Equ_gm_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gm_si);
  dQdx[li_bi][m_bi_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_bi][m_bi_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_bi].dx(cogendaProbeID_V_gm_si);
  dQdx[li_dbulk][m_dbulk_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si);
  dQdx[li_dbulk][m_dbulk_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_dbulk][m_dbulk_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si);
  dQdx[li_dbulk][m_dbulk_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_gi_si);
  dQdx[li_dbulk][m_dbulk_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_dbulk][m_dbulk_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_dbulk].dx(cogendaProbeID_V_gi_si);
  dQdx[li_di][m_di_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si);
  dQdx[li_di][m_di_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_di][m_di_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  dQdx[li_di][m_di_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si);
  dQdx[li_di][m_di_Equ_gm_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gm_si);
  dQdx[li_di][m_di_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_di][m_di_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_gm_si);
  dQdx[li_gi][m_gi_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si);
  dQdx[li_gi][m_gi_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_gi][m_gi_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si);
  dQdx[li_gi][m_gi_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si);
  dQdx[li_gi][m_gi_Equ_gm_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_si);
  dQdx[li_gi][m_gi_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_gi][m_gi_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_gi].dx(cogendaProbeID_V_gm_si);
  dQdx[li_sbulk][m_sbulk_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si);
  dQdx[li_sbulk][m_sbulk_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_sbulk][m_sbulk_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si);
  dQdx[li_sbulk][m_sbulk_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_gi_si);
  dQdx[li_sbulk][m_sbulk_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_sbulk][m_sbulk_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_sbulk].dx(cogendaProbeID_V_gi_si);
  dQdx[li_si][m_si_Equ_bi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si);
  dQdx[li_si][m_si_Equ_dbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si);
  dQdx[li_si][m_si_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  dQdx[li_si][m_si_Equ_gi_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si);
  dQdx[li_si][m_si_Equ_gm_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gm_si);
  dQdx[li_si][m_si_Equ_sbulk_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si);
  dQdx[li_si][m_si_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gi_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_bi_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_sbulk_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_dbulk_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_gm_si);
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
  if(!given("_CKT_GMIN"))
    _ckt_gmin = 1e-12;
  if(!given("_MIN"))
    _min = -9999999;
  if(!given("A0"))
    a0 = -12345789;
  if(!given("A1"))
    a1 = -12345789;
  if(!given("A2"))
    a2 = -12345789;
  if(!given("ACDE"))
    acde = -12345789;
  if(!given("ACNQSMOD"))
    acnqsmod = -9999999;
  if(!given("AD"))
    ad = -12345789;
  if(!given("ADOS"))
    ados = -12345789;
  if(!given("AF"))
    af = -12345789;
  if(!given("AGIDL"))
    agidl = -12345789;
  if(!given("AGISL"))
    agisl = -12345789;
  if(!given("AGS"))
    ags = -12345789;
  if(!given("AIGBACC"))
    aigbacc = -12345789;
  if(!given("AIGBINV"))
    aigbinv = -12345789;
  if(!given("AIGC"))
    aigc = -12345789;
  if(!given("AIGD"))
    aigd = -12345789;
  if(!given("AIGS"))
    aigs = -12345789;
  if(!given("AIGSD"))
    aigsd = -12345789;
  if(!given("ALPHA0"))
    alpha0 = -12345789;
  if(!given("ALPHA1"))
    alpha1 = -12345789;
  if(!given("AS"))
    as = -12345789;
  if(!given("AT"))
    at = -12345789;
  if(!given("B0"))
    b0 = -12345789;
  if(!given("B1"))
    b1 = -12345789;
  if(!given("BDOS"))
    bdos = -12345789;
  if(!given("BETA0"))
    beta0 = -12345789;
  if(!given("BG0SUB"))
    bg0sub = -12345789;
  if(!given("BGIDL"))
    bgidl = -12345789;
  if(!given("BGISL"))
    bgisl = -12345789;
  if(!given("BIGBACC"))
    bigbacc = -12345789;
  if(!given("BIGBINV"))
    bigbinv = -12345789;
  if(!given("BIGC"))
    bigc = -12345789;
  if(!given("BIGD"))
    bigd = -12345789;
  if(!given("BIGS"))
    bigs = -12345789;
  if(!given("BIGSD"))
    bigsd = -12345789;
  if(!given("BINUNIT"))
    binunit = -9999999;
  if(!given("BVD"))
    bvd = -12345789;
  if(!given("BVS"))
    bvs = -12345789;
  if(!given("CAPMOD"))
    capmod = -9999999;
  if(!given("CDSC"))
    cdsc = -12345789;
  if(!given("CDSCB"))
    cdscb = -12345789;
  if(!given("CDSCD"))
    cdscd = -12345789;
  if(!given("CF"))
    cf = -12345789;
  if(!given("CGBO"))
    cgbo = -12345789;
  if(!given("CGDL"))
    cgdl = -12345789;
  if(!given("CGDO"))
    cgdo = -12345789;
  if(!given("CGIDL"))
    cgidl = -12345789;
  if(!given("CGISL"))
    cgisl = -12345789;
  if(!given("CGSL"))
    cgsl = -12345789;
  if(!given("CGSO"))
    cgso = -12345789;
  if(!given("CIGBACC"))
    cigbacc = -12345789;
  if(!given("CIGBINV"))
    cigbinv = -12345789;
  if(!given("CIGC"))
    cigc = -12345789;
  if(!given("CIGD"))
    cigd = -12345789;
  if(!given("CIGS"))
    cigs = -12345789;
  if(!given("CIGSD"))
    cigsd = -12345789;
  if(!given("CIT"))
    cit = -12345789;
  if(!given("CJD"))
    cjd = -12345789;
  if(!given("CJS"))
    cjs = -12345789;
  if(!given("CJSWD"))
    cjswd = -12345789;
  if(!given("CJSWGD"))
    cjswgd = -12345789;
  if(!given("CJSWGS"))
    cjswgs = -12345789;
  if(!given("CJSWS"))
    cjsws = -12345789;
  if(!given("CKAPPAD"))
    ckappad = -12345789;
  if(!given("CKAPPAS"))
    ckappas = -12345789;
  if(!given("CLC"))
    clc = -12345789;
  if(!given("CLE"))
    cle = -12345789;
  if(!given("CVCHARGEMOD"))
    cvchargemod = -9999999;
  if(!given("DELTA"))
    delta = -12345789;
  if(!given("DELVTO"))
    delvto = -12345789;
  if(!given("DIOMOD"))
    diomod = -9999999;
  if(!given("DLC"))
    dlc = -12345789;
  if(!given("DLCIG"))
    dlcig = -12345789;
  if(!given("DLCIGD"))
    dlcigd = -12345789;
  if(!given("DMCG"))
    dmcg = -12345789;
  if(!given("DMCGT"))
    dmcgt = -12345789;
  if(!given("DMCI"))
    dmci = -12345789;
  if(!given("DMDG"))
    dmdg = -12345789;
  if(!given("DROUT"))
    drout = -12345789;
  if(!given("DSUB"))
    dsub = -12345789;
  if(!given("DTOX"))
    dtox = -12345789;
  if(!given("DVT0"))
    dvt0 = -12345789;
  if(!given("DVT0W"))
    dvt0w = -12345789;
  if(!given("DVT1"))
    dvt1 = -12345789;
  if(!given("DVT1W"))
    dvt1w = -12345789;
  if(!given("DVT2"))
    dvt2 = -12345789;
  if(!given("DVT2W"))
    dvt2w = -12345789;
  if(!given("DVTP0"))
    dvtp0 = -12345789;
  if(!given("DVTP1"))
    dvtp1 = -12345789;
  if(!given("DVTP2"))
    dvtp2 = -12345789;
  if(!given("DVTP3"))
    dvtp3 = -12345789;
  if(!given("DVTP4"))
    dvtp4 = -12345789;
  if(!given("DVTP5"))
    dvtp5 = -12345789;
  if(!given("DWB"))
    dwb = -12345789;
  if(!given("DWC"))
    dwc = -12345789;
  if(!given("DWG"))
    dwg = -12345789;
  if(!given("DWJ"))
    dwj = -12345789;
  if(!given("EASUB"))
    easub = -12345789;
  if(!given("EF"))
    ef = -12345789;
  if(!given("EGIDL"))
    egidl = -12345789;
  if(!given("EGISL"))
    egisl = -12345789;
  if(!given("EIGBINV"))
    eigbinv = -12345789;
  if(!given("EM"))
    em = -12345789;
  if(!given("EOT"))
    eot = -12345789;
  if(!given("EPSRGATE"))
    epsrgate = -12345789;
  if(!given("EPSROX"))
    epsrox = -12345789;
  if(!given("EPSRSUB"))
    epsrsub = -12345789;
  if(!given("ETA0"))
    eta0 = -12345789;
  if(!given("ETAB"))
    etab = -12345789;
  if(!given("EU"))
    eu = -12345789;
  if(!given("FGIDL"))
    fgidl = -12345789;
  if(!given("FGISL"))
    fgisl = -12345789;
  if(!given("FNOIMOD"))
    fnoimod = -9999999;
  if(!given("FPROUT"))
    fprout = -12345789;
  if(!given("GAMMA1"))
    gamma1 = -12345789;
  if(!given("GAMMA2"))
    gamma2 = -12345789;
  if(!given("GBMIN"))
    gbmin = -12345789;
  if(!given("GEOMOD"))
    geomod = -9999999;
  if(!given("GIDLMOD"))
    gidlmod = -9999999;
  if(!given("IGBMOD"))
    igbmod = -9999999;
  if(!given("IGCMOD"))
    igcmod = -9999999;
  if(!given("IJTHDFWD"))
    ijthdfwd = -12345789;
  if(!given("IJTHDREV"))
    ijthdrev = -12345789;
  if(!given("IJTHSFWD"))
    ijthsfwd = -12345789;
  if(!given("IJTHSREV"))
    ijthsrev = -12345789;
  if(!given("JSD"))
    jsd = -12345789;
  if(!given("JSS"))
    jss = -12345789;
  if(!given("JSWD"))
    jswd = -12345789;
  if(!given("JSWGD"))
    jswgd = -12345789;
  if(!given("JSWGS"))
    jswgs = -12345789;
  if(!given("JSWS"))
    jsws = -12345789;
  if(!given("JTSD"))
    jtsd = -12345789;
  if(!given("JTSS"))
    jtss = -12345789;
  if(!given("JTSSWD"))
    jtsswd = -12345789;
  if(!given("JTSSWGD"))
    jtsswgd = -12345789;
  if(!given("JTSSWGS"))
    jtsswgs = -12345789;
  if(!given("JTSSWS"))
    jtssws = -12345789;
  if(!given("JTWEFF"))
    jtweff = -12345789;
  if(!given("K1"))
    k1 = -12345789;
  if(!given("K2"))
    k2 = -12345789;
  if(!given("K2WE"))
    k2we = -12345789;
  if(!given("K3"))
    k3 = -12345789;
  if(!given("K3B"))
    k3b = -12345789;
  if(!given("KETA"))
    keta = -12345789;
  if(!given("KF"))
    kf = -12345789;
  if(!given("KGIDL"))
    kgidl = -12345789;
  if(!given("KGISL"))
    kgisl = -12345789;
  if(!given("KT1"))
    kt1 = -12345789;
  if(!given("KT1L"))
    kt1l = -12345789;
  if(!given("KT2"))
    kt2 = -12345789;
  if(!given("KU0"))
    ku0 = -12345789;
  if(!given("KU0WE"))
    ku0we = -12345789;
  if(!given("KVSAT"))
    kvsat = -12345789;
  if(!given("KVTH0"))
    kvth0 = -12345789;
  if(!given("KVTH0WE"))
    kvth0we = -12345789;
  if(!given("L"))
    l = -12345789;
  if(!given("LA0"))
    la0 = -12345789;
  if(!given("LA1"))
    la1 = -12345789;
  if(!given("LA2"))
    la2 = -12345789;
  if(!given("LACDE"))
    lacde = -12345789;
  if(!given("LAGIDL"))
    lagidl = -12345789;
  if(!given("LAGISL"))
    lagisl = -12345789;
  if(!given("LAGS"))
    lags = -12345789;
  if(!given("LAIGBACC"))
    laigbacc = -12345789;
  if(!given("LAIGBINV"))
    laigbinv = -12345789;
  if(!given("LAIGC"))
    laigc = -12345789;
  if(!given("LAIGD"))
    laigd = -12345789;
  if(!given("LAIGS"))
    laigs = -12345789;
  if(!given("LAIGSD"))
    laigsd = -12345789;
  if(!given("LALPHA0"))
    lalpha0 = -12345789;
  if(!given("LALPHA1"))
    lalpha1 = -12345789;
  if(!given("LAMBDA"))
    lambda = -12345789;
  if(!given("LAT"))
    lat = -12345789;
  if(!given("LB0"))
    lb0 = -12345789;
  if(!given("LB1"))
    lb1 = -12345789;
  if(!given("LBETA0"))
    lbeta0 = -12345789;
  if(!given("LBGIDL"))
    lbgidl = -12345789;
  if(!given("LBGISL"))
    lbgisl = -12345789;
  if(!given("LBIGBACC"))
    lbigbacc = -12345789;
  if(!given("LBIGBINV"))
    lbigbinv = -12345789;
  if(!given("LBIGC"))
    lbigc = -12345789;
  if(!given("LBIGD"))
    lbigd = -12345789;
  if(!given("LBIGS"))
    lbigs = -12345789;
  if(!given("LBIGSD"))
    lbigsd = -12345789;
  if(!given("LC"))
    lc = -12345789;
  if(!given("LCDSC"))
    lcdsc = -12345789;
  if(!given("LCDSCB"))
    lcdscb = -12345789;
  if(!given("LCDSCD"))
    lcdscd = -12345789;
  if(!given("LCF"))
    lcf = -12345789;
  if(!given("LCGDL"))
    lcgdl = -12345789;
  if(!given("LCGIDL"))
    lcgidl = -12345789;
  if(!given("LCGISL"))
    lcgisl = -12345789;
  if(!given("LCGSL"))
    lcgsl = -12345789;
  if(!given("LCIGBACC"))
    lcigbacc = -12345789;
  if(!given("LCIGBINV"))
    lcigbinv = -12345789;
  if(!given("LCIGC"))
    lcigc = -12345789;
  if(!given("LCIGD"))
    lcigd = -12345789;
  if(!given("LCIGS"))
    lcigs = -12345789;
  if(!given("LCIGSD"))
    lcigsd = -12345789;
  if(!given("LCIT"))
    lcit = -12345789;
  if(!given("LCKAPPAD"))
    lckappad = -12345789;
  if(!given("LCKAPPAS"))
    lckappas = -12345789;
  if(!given("LCLC"))
    lclc = -12345789;
  if(!given("LCLE"))
    lcle = -12345789;
  if(!given("LDELTA"))
    ldelta = -12345789;
  if(!given("LDROUT"))
    ldrout = -12345789;
  if(!given("LDSUB"))
    ldsub = -12345789;
  if(!given("LDVT0"))
    ldvt0 = -12345789;
  if(!given("LDVT0W"))
    ldvt0w = -12345789;
  if(!given("LDVT1"))
    ldvt1 = -12345789;
  if(!given("LDVT1W"))
    ldvt1w = -12345789;
  if(!given("LDVT2"))
    ldvt2 = -12345789;
  if(!given("LDVT2W"))
    ldvt2w = -12345789;
  if(!given("LDVTP0"))
    ldvtp0 = -12345789;
  if(!given("LDVTP1"))
    ldvtp1 = -12345789;
  if(!given("LDVTP2"))
    ldvtp2 = -12345789;
  if(!given("LDVTP3"))
    ldvtp3 = -12345789;
  if(!given("LDVTP4"))
    ldvtp4 = -12345789;
  if(!given("LDVTP5"))
    ldvtp5 = -12345789;
  if(!given("LDWB"))
    ldwb = -12345789;
  if(!given("LDWG"))
    ldwg = -12345789;
  if(!given("LEFFEOT"))
    leffeot = -12345789;
  if(!given("LEGIDL"))
    legidl = -12345789;
  if(!given("LEGISL"))
    legisl = -12345789;
  if(!given("LEIGBINV"))
    leigbinv = -12345789;
  if(!given("LETA0"))
    leta0 = -12345789;
  if(!given("LETAB"))
    letab = -12345789;
  if(!given("LEU"))
    leu = -12345789;
  if(!given("LEVEL"))
    level = -9999999;
  if(!given("LFGIDL"))
    lfgidl = -12345789;
  if(!given("LFGISL"))
    lfgisl = -12345789;
  if(!given("LFPROUT"))
    lfprout = -12345789;
  if(!given("LGAMMA1"))
    lgamma1 = -12345789;
  if(!given("LGAMMA2"))
    lgamma2 = -12345789;
  if(!given("LINT"))
    lint = -12345789;
  if(!given("LK1"))
    lk1 = -12345789;
  if(!given("LK2"))
    lk2 = -12345789;
  if(!given("LK2WE"))
    lk2we = -12345789;
  if(!given("LK3"))
    lk3 = -12345789;
  if(!given("LK3B"))
    lk3b = -12345789;
  if(!given("LKETA"))
    lketa = -12345789;
  if(!given("LKGIDL"))
    lkgidl = -12345789;
  if(!given("LKGISL"))
    lkgisl = -12345789;
  if(!given("LKT1"))
    lkt1 = -12345789;
  if(!given("LKT1L"))
    lkt1l = -12345789;
  if(!given("LKT2"))
    lkt2 = -12345789;
  if(!given("LKU0"))
    lku0 = -12345789;
  if(!given("LKU0WE"))
    lku0we = -12345789;
  if(!given("LKVTH0"))
    lkvth0 = -12345789;
  if(!given("LKVTH0WE"))
    lkvth0we = -12345789;
  if(!given("LL"))
    ll = -12345789;
  if(!given("LLAMBDA"))
    llambda = -12345789;
  if(!given("LLC"))
    llc = -12345789;
  if(!given("LLN"))
    lln = -12345789;
  if(!given("LLODKU0"))
    llodku0 = -12345789;
  if(!given("LLODVTH"))
    llodvth = -12345789;
  if(!given("LLP"))
    llp = -12345789;
  if(!given("LLPE0"))
    llpe0 = -12345789;
  if(!given("LLPEB"))
    llpeb = -12345789;
  if(!given("LMAX"))
    lmax = -12345789;
  if(!given("LMIN"))
    lmin = -12345789;
  if(!given("LMINV"))
    lminv = -12345789;
  if(!given("LMINVCV"))
    lminvcv = -12345789;
  if(!given("LMOIN"))
    lmoin = -12345789;
  if(!given("LNDEP"))
    lndep = -12345789;
  if(!given("LNFACTOR"))
    lnfactor = -12345789;
  if(!given("LNGATE"))
    lngate = -12345789;
  if(!given("LNIGBACC"))
    lnigbacc = -12345789;
  if(!given("LNIGBINV"))
    lnigbinv = -12345789;
  if(!given("LNIGC"))
    lnigc = -12345789;
  if(!given("LNOFF"))
    lnoff = -12345789;
  if(!given("LNSD"))
    lnsd = -12345789;
  if(!given("LNSUB"))
    lnsub = -12345789;
  if(!given("LNTOX"))
    lntox = -12345789;
  if(!given("LODETA0"))
    lodeta0 = -12345789;
  if(!given("LODK2"))
    lodk2 = -12345789;
  if(!given("LP"))
    lp = -12345789;
  if(!given("LPCLM"))
    lpclm = -12345789;
  if(!given("LPDIBLC1"))
    lpdiblc1 = -12345789;
  if(!given("LPDIBLC2"))
    lpdiblc2 = -12345789;
  if(!given("LPDIBLCB"))
    lpdiblcb = -12345789;
  if(!given("LPDITS"))
    lpdits = -12345789;
  if(!given("LPDITSD"))
    lpditsd = -12345789;
  if(!given("LPE0"))
    lpe0 = -12345789;
  if(!given("LPEB"))
    lpeb = -12345789;
  if(!given("LPHIN"))
    lphin = -12345789;
  if(!given("LPIGCD"))
    lpigcd = -12345789;
  if(!given("LPOXEDGE"))
    lpoxedge = -12345789;
  if(!given("LPRT"))
    lprt = -12345789;
  if(!given("LPRWB"))
    lprwb = -12345789;
  if(!given("LPRWG"))
    lprwg = -12345789;
  if(!given("LPSCBE1"))
    lpscbe1 = -12345789;
  if(!given("LPSCBE2"))
    lpscbe2 = -12345789;
  if(!given("LPVAG"))
    lpvag = -12345789;
  if(!given("LRDSW"))
    lrdsw = -12345789;
  if(!given("LRDW"))
    lrdw = -12345789;
  if(!given("LRGIDL"))
    lrgidl = -12345789;
  if(!given("LRGISL"))
    lrgisl = -12345789;
  if(!given("LRSW"))
    lrsw = -12345789;
  if(!given("LTETA0"))
    lteta0 = -12345789;
  if(!given("LTNFACTOR"))
    ltnfactor = -12345789;
  if(!given("LTVFBSDOFF"))
    ltvfbsdoff = -12345789;
  if(!given("LTVOFF"))
    ltvoff = -12345789;
  if(!given("LTVOFFCV"))
    ltvoffcv = -12345789;
  if(!given("LU0"))
    lu0 = -12345789;
  if(!given("LUA"))
    lua = -12345789;
  if(!given("LUA1"))
    lua1 = -12345789;
  if(!given("LUB"))
    lub = -12345789;
  if(!given("LUB1"))
    lub1 = -12345789;
  if(!given("LUC"))
    luc = -12345789;
  if(!given("LUC1"))
    luc1 = -12345789;
  if(!given("LUCS"))
    lucs = -12345789;
  if(!given("LUCSTE"))
    lucste = -12345789;
  if(!given("LUD"))
    lud = -12345789;
  if(!given("LUD1"))
    lud1 = -12345789;
  if(!given("LUP"))
    lup = -12345789;
  if(!given("LUTE"))
    lute = -12345789;
  if(!given("LVBM"))
    lvbm = -12345789;
  if(!given("LVBX"))
    lvbx = -12345789;
  if(!given("LVFB"))
    lvfb = -12345789;
  if(!given("LVFBCV"))
    lvfbcv = -12345789;
  if(!given("LVFBSDOFF"))
    lvfbsdoff = -12345789;
  if(!given("LVOFF"))
    lvoff = -12345789;
  if(!given("LVOFFCV"))
    lvoffcv = -12345789;
  if(!given("LVSAT"))
    lvsat = -12345789;
  if(!given("LVTH0"))
    lvth0 = -12345789;
  if(!given("LVTL"))
    lvtl = -12345789;
  if(!given("LW"))
    lw = -12345789;
  if(!given("LW0"))
    lw0 = -12345789;
  if(!given("LWC"))
    lwc = -12345789;
  if(!given("LWL"))
    lwl = -12345789;
  if(!given("LWLC"))
    lwlc = -12345789;
  if(!given("LWN"))
    lwn = -12345789;
  if(!given("LWR"))
    lwr = -12345789;
  if(!given("LXJ"))
    lxj = -12345789;
  if(!given("LXN"))
    lxn = -12345789;
  if(!given("LXRCRG1"))
    lxrcrg1 = -12345789;
  if(!given("LXRCRG2"))
    lxrcrg2 = -12345789;
  if(!given("LXT"))
    lxt = -12345789;
  if(!given("MINV"))
    minv = -12345789;
  if(!given("MINVCV"))
    minvcv = -12345789;
  if(!given("MJD"))
    mjd = -12345789;
  if(!given("MJS"))
    mjs = -12345789;
  if(!given("MJSWD"))
    mjswd = -12345789;
  if(!given("MJSWGD"))
    mjswgd = -12345789;
  if(!given("MJSWGS"))
    mjswgs = -12345789;
  if(!given("MJSWS"))
    mjsws = -12345789;
  if(!given("MOBMOD"))
    mobmod = -9999999;
  if(!given("MOIN"))
    moin = -12345789;
  if(!given("MTRLCOMPATMOD"))
    mtrlcompatmod = -9999999;
  if(!given("MTRLMOD"))
    mtrlmod = -9999999;
  if(!given("NDEP"))
    ndep = -12345789;
  if(!given("NF"))
    nf = -9999999;
  if(!given("NFACTOR"))
    nfactor = -12345789;
  if(!given("NGATE"))
    ngate = -12345789;
  if(!given("NGCON"))
    ngcon = -12345789;
  if(!given("NI0SUB"))
    ni0sub = -12345789;
  if(!given("NIGBACC"))
    nigbacc = -12345789;
  if(!given("NIGBINV"))
    nigbinv = -12345789;
  if(!given("NIGC"))
    nigc = -12345789;
  if(!given("NJD"))
    njd = -12345789;
  if(!given("NJS"))
    njs = -12345789;
  if(!given("NJTS"))
    njts = -12345789;
  if(!given("NJTSD"))
    njtsd = -12345789;
  if(!given("NJTSSW"))
    njtssw = -12345789;
  if(!given("NJTSSWD"))
    njtsswd = -12345789;
  if(!given("NJTSSWG"))
    njtsswg = -12345789;
  if(!given("NJTSSWGD"))
    njtsswgd = -12345789;
  if(!given("NOFF"))
    noff = -12345789;
  if(!given("NOIA"))
    noia = -12345789;
  if(!given("NOIB"))
    noib = -12345789;
  if(!given("NOIC"))
    noic = -12345789;
  if(!given("NRD"))
    nrd = -12345789;
  if(!given("NRS"))
    nrs = -12345789;
  if(!given("NSD"))
    nsd = -12345789;
  if(!given("NSUB"))
    nsub = -12345789;
  if(!given("NTNOI"))
    ntnoi = -12345789;
  if(!given("NTOX"))
    ntox = -12345789;
  if(!given("OFF"))
    off = -9999999;
  if(!given("PA0"))
    pa0 = -12345789;
  if(!given("PA1"))
    pa1 = -12345789;
  if(!given("PA2"))
    pa2 = -12345789;
  if(!given("PACDE"))
    pacde = -12345789;
  if(!given("PAGIDL"))
    pagidl = -12345789;
  if(!given("PAGISL"))
    pagisl = -12345789;
  if(!given("PAGS"))
    pags = -12345789;
  if(!given("PAIGBACC"))
    paigbacc = -12345789;
  if(!given("PAIGBINV"))
    paigbinv = -12345789;
  if(!given("PAIGC"))
    paigc = -12345789;
  if(!given("PAIGD"))
    paigd = -12345789;
  if(!given("PAIGS"))
    paigs = -12345789;
  if(!given("PAIGSD"))
    paigsd = -12345789;
  if(!given("PALPHA0"))
    palpha0 = -12345789;
  if(!given("PALPHA1"))
    palpha1 = -12345789;
  if(!given("PARAMCHK"))
    paramchk = -9999999;
  if(!given("PAT"))
    pat = -12345789;
  if(!given("PB0"))
    pb0 = -12345789;
  if(!given("PB1"))
    pb1 = -12345789;
  if(!given("PBD"))
    pbd = -12345789;
  if(!given("PBETA0"))
    pbeta0 = -12345789;
  if(!given("PBGIDL"))
    pbgidl = -12345789;
  if(!given("PBGISL"))
    pbgisl = -12345789;
  if(!given("PBIGBACC"))
    pbigbacc = -12345789;
  if(!given("PBIGBINV"))
    pbigbinv = -12345789;
  if(!given("PBIGC"))
    pbigc = -12345789;
  if(!given("PBIGD"))
    pbigd = -12345789;
  if(!given("PBIGS"))
    pbigs = -12345789;
  if(!given("PBIGSD"))
    pbigsd = -12345789;
  if(!given("PBS"))
    pbs = -12345789;
  if(!given("PBSWD"))
    pbswd = -12345789;
  if(!given("PBSWGD"))
    pbswgd = -12345789;
  if(!given("PBSWGS"))
    pbswgs = -12345789;
  if(!given("PBSWS"))
    pbsws = -12345789;
  if(!given("PCDSC"))
    pcdsc = -12345789;
  if(!given("PCDSCB"))
    pcdscb = -12345789;
  if(!given("PCDSCD"))
    pcdscd = -12345789;
  if(!given("PCF"))
    pcf = -12345789;
  if(!given("PCGDL"))
    pcgdl = -12345789;
  if(!given("PCGIDL"))
    pcgidl = -12345789;
  if(!given("PCGISL"))
    pcgisl = -12345789;
  if(!given("PCGSL"))
    pcgsl = -12345789;
  if(!given("PCIGBACC"))
    pcigbacc = -12345789;
  if(!given("PCIGBINV"))
    pcigbinv = -12345789;
  if(!given("PCIGC"))
    pcigc = -12345789;
  if(!given("PCIGD"))
    pcigd = -12345789;
  if(!given("PCIGS"))
    pcigs = -12345789;
  if(!given("PCIGSD"))
    pcigsd = -12345789;
  if(!given("PCIT"))
    pcit = -12345789;
  if(!given("PCKAPPAD"))
    pckappad = -12345789;
  if(!given("PCKAPPAS"))
    pckappas = -12345789;
  if(!given("PCLC"))
    pclc = -12345789;
  if(!given("PCLE"))
    pcle = -12345789;
  if(!given("PCLM"))
    pclm = -12345789;
  if(!given("PD"))
    pd = -12345789;
  if(!given("PDELTA"))
    pdelta = -12345789;
  if(!given("PDIBLC1"))
    pdiblc1 = -12345789;
  if(!given("PDIBLC2"))
    pdiblc2 = -12345789;
  if(!given("PDIBLCB"))
    pdiblcb = -12345789;
  if(!given("PDITS"))
    pdits = -12345789;
  if(!given("PDITSD"))
    pditsd = -12345789;
  if(!given("PDITSL"))
    pditsl = -12345789;
  if(!given("PDROUT"))
    pdrout = -12345789;
  if(!given("PDSUB"))
    pdsub = -12345789;
  if(!given("PDVT0"))
    pdvt0 = -12345789;
  if(!given("PDVT0W"))
    pdvt0w = -12345789;
  if(!given("PDVT1"))
    pdvt1 = -12345789;
  if(!given("PDVT1W"))
    pdvt1w = -12345789;
  if(!given("PDVT2"))
    pdvt2 = -12345789;
  if(!given("PDVT2W"))
    pdvt2w = -12345789;
  if(!given("PDVTP0"))
    pdvtp0 = -12345789;
  if(!given("PDVTP1"))
    pdvtp1 = -12345789;
  if(!given("PDVTP2"))
    pdvtp2 = -12345789;
  if(!given("PDVTP3"))
    pdvtp3 = -12345789;
  if(!given("PDVTP4"))
    pdvtp4 = -12345789;
  if(!given("PDVTP5"))
    pdvtp5 = -12345789;
  if(!given("PDWB"))
    pdwb = -12345789;
  if(!given("PDWG"))
    pdwg = -12345789;
  if(!given("PEGIDL"))
    pegidl = -12345789;
  if(!given("PEGISL"))
    pegisl = -12345789;
  if(!given("PEIGBINV"))
    peigbinv = -12345789;
  if(!given("PERMOD"))
    permod = -9999999;
  if(!given("PETA0"))
    peta0 = -12345789;
  if(!given("PETAB"))
    petab = -12345789;
  if(!given("PEU"))
    peu = -12345789;
  if(!given("PFGIDL"))
    pfgidl = -12345789;
  if(!given("PFGISL"))
    pfgisl = -12345789;
  if(!given("PFPROUT"))
    pfprout = -12345789;
  if(!given("PGAMMA1"))
    pgamma1 = -12345789;
  if(!given("PGAMMA2"))
    pgamma2 = -12345789;
  if(!given("PHIG"))
    phig = -12345789;
  if(!given("PHIN"))
    phin = -12345789;
  if(!given("PIGCD"))
    pigcd = -12345789;
  if(!given("PK1"))
    pk1 = -12345789;
  if(!given("PK2"))
    pk2 = -12345789;
  if(!given("PK2WE"))
    pk2we = -12345789;
  if(!given("PK3"))
    pk3 = -12345789;
  if(!given("PK3B"))
    pk3b = -12345789;
  if(!given("PKETA"))
    pketa = -12345789;
  if(!given("PKGIDL"))
    pkgidl = -12345789;
  if(!given("PKGISL"))
    pkgisl = -12345789;
  if(!given("PKT1"))
    pkt1 = -12345789;
  if(!given("PKT1L"))
    pkt1l = -12345789;
  if(!given("PKT2"))
    pkt2 = -12345789;
  if(!given("PKU0"))
    pku0 = -12345789;
  if(!given("PKU0WE"))
    pku0we = -12345789;
  if(!given("PKVTH0"))
    pkvth0 = -12345789;
  if(!given("PKVTH0WE"))
    pkvth0we = -12345789;
  if(!given("PLAMBDA"))
    plambda = -12345789;
  if(!given("PLP"))
    plp = -12345789;
  if(!given("PLPE0"))
    plpe0 = -12345789;
  if(!given("PLPEB"))
    plpeb = -12345789;
  if(!given("PMINV"))
    pminv = -12345789;
  if(!given("PMINVCV"))
    pminvcv = -12345789;
  if(!given("PMOIN"))
    pmoin = -12345789;
  if(!given("PNDEP"))
    pndep = -12345789;
  if(!given("PNFACTOR"))
    pnfactor = -12345789;
  if(!given("PNGATE"))
    pngate = -12345789;
  if(!given("PNIGBACC"))
    pnigbacc = -12345789;
  if(!given("PNIGBINV"))
    pnigbinv = -12345789;
  if(!given("PNIGC"))
    pnigc = -12345789;
  if(!given("PNOFF"))
    pnoff = -12345789;
  if(!given("PNSD"))
    pnsd = -12345789;
  if(!given("PNSUB"))
    pnsub = -12345789;
  if(!given("PNTOX"))
    pntox = -12345789;
  if(!given("POXEDGE"))
    poxedge = -12345789;
  if(!given("PPCLM"))
    ppclm = -12345789;
  if(!given("PPDIBLC1"))
    ppdiblc1 = -12345789;
  if(!given("PPDIBLC2"))
    ppdiblc2 = -12345789;
  if(!given("PPDIBLCB"))
    ppdiblcb = -12345789;
  if(!given("PPDITS"))
    ppdits = -12345789;
  if(!given("PPDITSD"))
    ppditsd = -12345789;
  if(!given("PPHIN"))
    pphin = -12345789;
  if(!given("PPIGCD"))
    ppigcd = -12345789;
  if(!given("PPOXEDGE"))
    ppoxedge = -12345789;
  if(!given("PPRT"))
    pprt = -12345789;
  if(!given("PPRWB"))
    pprwb = -12345789;
  if(!given("PPRWG"))
    pprwg = -12345789;
  if(!given("PPSCBE1"))
    ppscbe1 = -12345789;
  if(!given("PPSCBE2"))
    ppscbe2 = -12345789;
  if(!given("PPVAG"))
    ppvag = -12345789;
  if(!given("PRDSW"))
    prdsw = -12345789;
  if(!given("PRDW"))
    prdw = -12345789;
  if(!given("PRGIDL"))
    prgidl = -12345789;
  if(!given("PRGISL"))
    prgisl = -12345789;
  if(!given("PRSW"))
    prsw = -12345789;
  if(!given("PRT"))
    prt = -12345789;
  if(!given("PRWB"))
    prwb = -12345789;
  if(!given("PRWG"))
    prwg = -12345789;
  if(!given("PS"))
    ps = -12345789;
  if(!given("PSCBE1"))
    pscbe1 = -12345789;
  if(!given("PSCBE2"))
    pscbe2 = -12345789;
  if(!given("PTETA0"))
    pteta0 = -12345789;
  if(!given("PTNFACTOR"))
    ptnfactor = -12345789;
  if(!given("PTVFBSDOFF"))
    ptvfbsdoff = -12345789;
  if(!given("PTVOFF"))
    ptvoff = -12345789;
  if(!given("PTVOFFCV"))
    ptvoffcv = -12345789;
  if(!given("PU0"))
    pu0 = -12345789;
  if(!given("PUA"))
    pua = -12345789;
  if(!given("PUA1"))
    pua1 = -12345789;
  if(!given("PUB"))
    pub = -12345789;
  if(!given("PUB1"))
    pub1 = -12345789;
  if(!given("PUC"))
    puc = -12345789;
  if(!given("PUC1"))
    puc1 = -12345789;
  if(!given("PUCS"))
    pucs = -12345789;
  if(!given("PUCSTE"))
    pucste = -12345789;
  if(!given("PUD"))
    pud = -12345789;
  if(!given("PUD1"))
    pud1 = -12345789;
  if(!given("PUP"))
    pup = -12345789;
  if(!given("PUTE"))
    pute = -12345789;
  if(!given("PVAG"))
    pvag = -12345789;
  if(!given("PVBM"))
    pvbm = -12345789;
  if(!given("PVBX"))
    pvbx = -12345789;
  if(!given("PVFB"))
    pvfb = -12345789;
  if(!given("PVFBCV"))
    pvfbcv = -12345789;
  if(!given("PVFBSDOFF"))
    pvfbsdoff = -12345789;
  if(!given("PVOFF"))
    pvoff = -12345789;
  if(!given("PVOFFCV"))
    pvoffcv = -12345789;
  if(!given("PVSAT"))
    pvsat = -12345789;
  if(!given("PVTH0"))
    pvth0 = -12345789;
  if(!given("PVTL"))
    pvtl = -12345789;
  if(!given("PW0"))
    pw0 = -12345789;
  if(!given("PWR"))
    pwr = -12345789;
  if(!given("PXJ"))
    pxj = -12345789;
  if(!given("PXN"))
    pxn = -12345789;
  if(!given("PXRCRG1"))
    pxrcrg1 = -12345789;
  if(!given("PXRCRG2"))
    pxrcrg2 = -12345789;
  if(!given("PXT"))
    pxt = -12345789;
  if(!given("RBDB"))
    rbdb = -12345789;
  if(!given("RBDBX0"))
    rbdbx0 = -12345789;
  if(!given("RBDBY0"))
    rbdby0 = -12345789;
  if(!given("RBODYMOD"))
    rbodymod = -9999999;
  if(!given("RBPB"))
    rbpb = -12345789;
  if(!given("RBPBX0"))
    rbpbx0 = -12345789;
  if(!given("RBPBXL"))
    rbpbxl = -12345789;
  if(!given("RBPBXNF"))
    rbpbxnf = -12345789;
  if(!given("RBPBXW"))
    rbpbxw = -12345789;
  if(!given("RBPBY0"))
    rbpby0 = -12345789;
  if(!given("RBPBYL"))
    rbpbyl = -12345789;
  if(!given("RBPBYNF"))
    rbpbynf = -12345789;
  if(!given("RBPBYW"))
    rbpbyw = -12345789;
  if(!given("RBPD"))
    rbpd = -12345789;
  if(!given("RBPD0"))
    rbpd0 = -12345789;
  if(!given("RBPDL"))
    rbpdl = -12345789;
  if(!given("RBPDNF"))
    rbpdnf = -12345789;
  if(!given("RBPDW"))
    rbpdw = -12345789;
  if(!given("RBPS"))
    rbps = -12345789;
  if(!given("RBPS0"))
    rbps0 = -12345789;
  if(!given("RBPSL"))
    rbpsl = -12345789;
  if(!given("RBPSNF"))
    rbpsnf = -12345789;
  if(!given("RBPSW"))
    rbpsw = -12345789;
  if(!given("RBSB"))
    rbsb = -12345789;
  if(!given("RBSBX0"))
    rbsbx0 = -12345789;
  if(!given("RBSBY0"))
    rbsby0 = -12345789;
  if(!given("RBSDBXL"))
    rbsdbxl = -12345789;
  if(!given("RBSDBXNF"))
    rbsdbxnf = -12345789;
  if(!given("RBSDBXW"))
    rbsdbxw = -12345789;
  if(!given("RBSDBYL"))
    rbsdbyl = -12345789;
  if(!given("RBSDBYNF"))
    rbsdbynf = -12345789;
  if(!given("RBSDBYW"))
    rbsdbyw = -12345789;
  if(!given("RDSMOD"))
    rdsmod = -9999999;
  if(!given("RDSW"))
    rdsw = -12345789;
  if(!given("RDSWMIN"))
    rdswmin = -12345789;
  if(!given("RDW"))
    rdw = -12345789;
  if(!given("RDWMIN"))
    rdwmin = -12345789;
  if(!given("RGATEMOD"))
    rgatemod = -9999999;
  if(!given("RGEOMOD"))
    rgeomod = -9999999;
  if(!given("RGIDL"))
    rgidl = -12345789;
  if(!given("RGISL"))
    rgisl = -12345789;
  if(!given("RNOIA"))
    rnoia = -12345789;
  if(!given("RNOIB"))
    rnoib = -12345789;
  if(!given("RNOIC"))
    rnoic = -12345789;
  if(!given("RSH"))
    rsh = -12345789;
  if(!given("RSHG"))
    rshg = -12345789;
  if(!given("RSW"))
    rsw = -12345789;
  if(!given("RSWMIN"))
    rswmin = -12345789;
  if(!given("SA"))
    sa = -12345789;
  if(!given("SAREF"))
    saref = -12345789;
  if(!given("SB"))
    sb = -12345789;
  if(!given("SBREF"))
    sbref = -12345789;
  if(!given("SC"))
    sc = -12345789;
  if(!given("SCA"))
    sca = -12345789;
  if(!given("SCB"))
    scb = -12345789;
  if(!given("SCC"))
    scc = -12345789;
  if(!given("SCREF"))
    scref = -12345789;
  if(!given("SD"))
    sd = -12345789;
  if(!given("STETA0"))
    steta0 = -12345789;
  if(!given("STK2"))
    stk2 = -12345789;
  if(!given("TBGASUB"))
    tbgasub = -12345789;
  if(!given("TBGBSUB"))
    tbgbsub = -12345789;
  if(!given("TCJ"))
    tcj = -12345789;
  if(!given("TCJSW"))
    tcjsw = -12345789;
  if(!given("TCJSWG"))
    tcjswg = -12345789;
  if(!given("TEMPEOT"))
    tempeot = -12345789;
  if(!given("TEMPMOD"))
    tempmod = -9999999;
  if(!given("TETA0"))
    teta0 = -12345789;
  if(!given("TKU0"))
    tku0 = -12345789;
  if(!given("TNFACTOR"))
    tnfactor = -12345789;
  if(!given("TNJTS"))
    tnjts = -12345789;
  if(!given("TNJTSD"))
    tnjtsd = -12345789;
  if(!given("TNJTSSW"))
    tnjtssw = -12345789;
  if(!given("TNJTSSWD"))
    tnjtsswd = -12345789;
  if(!given("TNJTSSWG"))
    tnjtsswg = -12345789;
  if(!given("TNJTSSWGD"))
    tnjtsswgd = -12345789;
  if(!given("TNOIA"))
    tnoia = -12345789;
  if(!given("TNOIB"))
    tnoib = -12345789;
  if(!given("TNOIC"))
    tnoic = -12345789;
  if(!given("TNOIMOD"))
    tnoimod = -9999999;
  if(!given("TNOM"))
    tnom = -12345789;
  else
    if(tnom < -100 || tnom>150)
    tnom -= P_CELSIUS0;
  if(!given("TOXE"))
    toxe = -12345789;
  if(!given("TOXM"))
    toxm = -12345789;
  if(!given("TOXP"))
    toxp = -12345789;
  if(!given("TOXREF"))
    toxref = -12345789;
  if(!given("TPB"))
    tpb = -12345789;
  if(!given("TPBSW"))
    tpbsw = -12345789;
  if(!given("TPBSWG"))
    tpbswg = -12345789;
  if(!given("TRNQSMOD"))
    trnqsmod = -9999999;
  if(!given("TVFBSDOFF"))
    tvfbsdoff = -12345789;
  if(!given("TVOFF"))
    tvoff = -12345789;
  if(!given("TVOFFCV"))
    tvoffcv = -12345789;
  if(!given("TYPE"))
    type = -9999999;
  if(!given("U0"))
    u0 = -12345789;
  if(!given("UA"))
    ua = -12345789;
  if(!given("UA1"))
    ua1 = -12345789;
  if(!given("UB"))
    ub = -12345789;
  if(!given("UB1"))
    ub1 = -12345789;
  if(!given("UC"))
    uc = -12345789;
  if(!given("UC1"))
    uc1 = -12345789;
  if(!given("UCS"))
    ucs = -12345789;
  if(!given("UCSTE"))
    ucste = -12345789;
  if(!given("UD"))
    ud = -12345789;
  if(!given("UD1"))
    ud1 = -12345789;
  if(!given("UP"))
    up = -12345789;
  if(!given("UTE"))
    ute = -12345789;
  if(!given("VBM"))
    vbm = -12345789;
  if(!given("VBX"))
    vbx = -12345789;
  if(!given("VDDEOT"))
    vddeot = -12345789;
  if(!given("VERBOSE"))
    verbose = -9999999;
  if(!given("VERSION"))
    version = -12345789;
  if(!given("VFB"))
    vfb = -12345789;
  if(!given("VFBCV"))
    vfbcv = -12345789;
  if(!given("VFBSDOFF"))
    vfbsdoff = -12345789;
  if(!given("VOFF"))
    voff = -12345789;
  if(!given("VOFFCV"))
    voffcv = -12345789;
  if(!given("VOFFCVL"))
    voffcvl = -12345789;
  if(!given("VOFFL"))
    voffl = -12345789;
  if(!given("VSAT"))
    vsat = -12345789;
  if(!given("VTH0"))
    vth0 = -12345789;
  if(!given("VTL"))
    vtl = -12345789;
  if(!given("VTSD"))
    vtsd = -12345789;
  if(!given("VTSS"))
    vtss = -12345789;
  if(!given("VTSSWD"))
    vtsswd = -12345789;
  if(!given("VTSSWGD"))
    vtsswgd = -12345789;
  if(!given("VTSSWGS"))
    vtsswgs = -12345789;
  if(!given("VTSSWS"))
    vtssws = -12345789;
  if(!given("W"))
    w = -12345789;
  if(!given("W0"))
    w0 = -12345789;
  if(!given("WA0"))
    wa0 = -12345789;
  if(!given("WA1"))
    wa1 = -12345789;
  if(!given("WA2"))
    wa2 = -12345789;
  if(!given("WACDE"))
    wacde = -12345789;
  if(!given("WAGIDL"))
    wagidl = -12345789;
  if(!given("WAGISL"))
    wagisl = -12345789;
  if(!given("WAGS"))
    wags = -12345789;
  if(!given("WAIGBACC"))
    waigbacc = -12345789;
  if(!given("WAIGBINV"))
    waigbinv = -12345789;
  if(!given("WAIGC"))
    waigc = -12345789;
  if(!given("WAIGD"))
    waigd = -12345789;
  if(!given("WAIGS"))
    waigs = -12345789;
  if(!given("WAIGSD"))
    waigsd = -12345789;
  if(!given("WALPHA0"))
    walpha0 = -12345789;
  if(!given("WALPHA1"))
    walpha1 = -12345789;
  if(!given("WAT"))
    wat = -12345789;
  if(!given("WB0"))
    wb0 = -12345789;
  if(!given("WB1"))
    wb1 = -12345789;
  if(!given("WBETA0"))
    wbeta0 = -12345789;
  if(!given("WBGIDL"))
    wbgidl = -12345789;
  if(!given("WBGISL"))
    wbgisl = -12345789;
  if(!given("WBIGBACC"))
    wbigbacc = -12345789;
  if(!given("WBIGBINV"))
    wbigbinv = -12345789;
  if(!given("WBIGC"))
    wbigc = -12345789;
  if(!given("WBIGD"))
    wbigd = -12345789;
  if(!given("WBIGS"))
    wbigs = -12345789;
  if(!given("WBIGSD"))
    wbigsd = -12345789;
  if(!given("WCDSC"))
    wcdsc = -12345789;
  if(!given("WCDSCB"))
    wcdscb = -12345789;
  if(!given("WCDSCD"))
    wcdscd = -12345789;
  if(!given("WCF"))
    wcf = -12345789;
  if(!given("WCGDL"))
    wcgdl = -12345789;
  if(!given("WCGIDL"))
    wcgidl = -12345789;
  if(!given("WCGISL"))
    wcgisl = -12345789;
  if(!given("WCGSL"))
    wcgsl = -12345789;
  if(!given("WCIGBACC"))
    wcigbacc = -12345789;
  if(!given("WCIGBINV"))
    wcigbinv = -12345789;
  if(!given("WCIGC"))
    wcigc = -12345789;
  if(!given("WCIGD"))
    wcigd = -12345789;
  if(!given("WCIGS"))
    wcigs = -12345789;
  if(!given("WCIGSD"))
    wcigsd = -12345789;
  if(!given("WCIT"))
    wcit = -12345789;
  if(!given("WCKAPPAD"))
    wckappad = -12345789;
  if(!given("WCKAPPAS"))
    wckappas = -12345789;
  if(!given("WCLC"))
    wclc = -12345789;
  if(!given("WCLE"))
    wcle = -12345789;
  if(!given("WDELTA"))
    wdelta = -12345789;
  if(!given("WDROUT"))
    wdrout = -12345789;
  if(!given("WDSUB"))
    wdsub = -12345789;
  if(!given("WDVT0"))
    wdvt0 = -12345789;
  if(!given("WDVT0W"))
    wdvt0w = -12345789;
  if(!given("WDVT1"))
    wdvt1 = -12345789;
  if(!given("WDVT1W"))
    wdvt1w = -12345789;
  if(!given("WDVT2"))
    wdvt2 = -12345789;
  if(!given("WDVT2W"))
    wdvt2w = -12345789;
  if(!given("WDVTP0"))
    wdvtp0 = -12345789;
  if(!given("WDVTP1"))
    wdvtp1 = -12345789;
  if(!given("WDVTP2"))
    wdvtp2 = -12345789;
  if(!given("WDVTP3"))
    wdvtp3 = -12345789;
  if(!given("WDVTP4"))
    wdvtp4 = -12345789;
  if(!given("WDVTP5"))
    wdvtp5 = -12345789;
  if(!given("WDWB"))
    wdwb = -12345789;
  if(!given("WDWG"))
    wdwg = -12345789;
  if(!given("WEB"))
    web = -12345789;
  if(!given("WEC"))
    wec = -12345789;
  if(!given("WEFFEOT"))
    weffeot = -12345789;
  if(!given("WEGIDL"))
    wegidl = -12345789;
  if(!given("WEGISL"))
    wegisl = -12345789;
  if(!given("WEIGBINV"))
    weigbinv = -12345789;
  if(!given("WETA0"))
    weta0 = -12345789;
  if(!given("WETAB"))
    wetab = -12345789;
  if(!given("WEU"))
    weu = -12345789;
  if(!given("WFGIDL"))
    wfgidl = -12345789;
  if(!given("WFGISL"))
    wfgisl = -12345789;
  if(!given("WFPROUT"))
    wfprout = -12345789;
  if(!given("WGAMMA1"))
    wgamma1 = -12345789;
  if(!given("WGAMMA2"))
    wgamma2 = -12345789;
  if(!given("WINT"))
    wint = -12345789;
  if(!given("WK1"))
    wk1 = -12345789;
  if(!given("WK2"))
    wk2 = -12345789;
  if(!given("WK2WE"))
    wk2we = -12345789;
  if(!given("WK3"))
    wk3 = -12345789;
  if(!given("WK3B"))
    wk3b = -12345789;
  if(!given("WKETA"))
    wketa = -12345789;
  if(!given("WKGIDL"))
    wkgidl = -12345789;
  if(!given("WKGISL"))
    wkgisl = -12345789;
  if(!given("WKT1"))
    wkt1 = -12345789;
  if(!given("WKT1L"))
    wkt1l = -12345789;
  if(!given("WKT2"))
    wkt2 = -12345789;
  if(!given("WKU0"))
    wku0 = -12345789;
  if(!given("WKU0WE"))
    wku0we = -12345789;
  if(!given("WKVTH0"))
    wkvth0 = -12345789;
  if(!given("WKVTH0WE"))
    wkvth0we = -12345789;
  if(!given("WL"))
    wl = -12345789;
  if(!given("WLAMBDA"))
    wlambda = -12345789;
  if(!given("WLC"))
    wlc = -12345789;
  if(!given("WLN"))
    wln = -12345789;
  if(!given("WLOD"))
    wlod = -12345789;
  if(!given("WLODKU0"))
    wlodku0 = -12345789;
  if(!given("WLODVTH"))
    wlodvth = -12345789;
  if(!given("WLP"))
    wlp = -12345789;
  if(!given("WLPE0"))
    wlpe0 = -12345789;
  if(!given("WLPEB"))
    wlpeb = -12345789;
  if(!given("WMAX"))
    wmax = -12345789;
  if(!given("WMIN"))
    wmin = -12345789;
  if(!given("WMINV"))
    wminv = -12345789;
  if(!given("WMINVCV"))
    wminvcv = -12345789;
  if(!given("WMOIN"))
    wmoin = -12345789;
  if(!given("WNDEP"))
    wndep = -12345789;
  if(!given("WNFACTOR"))
    wnfactor = -12345789;
  if(!given("WNGATE"))
    wngate = -12345789;
  if(!given("WNIGBACC"))
    wnigbacc = -12345789;
  if(!given("WNIGBINV"))
    wnigbinv = -12345789;
  if(!given("WNIGC"))
    wnigc = -12345789;
  if(!given("WNOFF"))
    wnoff = -12345789;
  if(!given("WNSD"))
    wnsd = -12345789;
  if(!given("WNSUB"))
    wnsub = -12345789;
  if(!given("WNTOX"))
    wntox = -12345789;
  if(!given("WPCLM"))
    wpclm = -12345789;
  if(!given("WPDIBLC1"))
    wpdiblc1 = -12345789;
  if(!given("WPDIBLC2"))
    wpdiblc2 = -12345789;
  if(!given("WPDIBLCB"))
    wpdiblcb = -12345789;
  if(!given("WPDITS"))
    wpdits = -12345789;
  if(!given("WPDITSD"))
    wpditsd = -12345789;
  if(!given("WPEMOD"))
    wpemod = -9999999;
  if(!given("WPHIN"))
    wphin = -12345789;
  if(!given("WPIGCD"))
    wpigcd = -12345789;
  if(!given("WPOXEDGE"))
    wpoxedge = -12345789;
  if(!given("WPRT"))
    wprt = -12345789;
  if(!given("WPRWB"))
    wprwb = -12345789;
  if(!given("WPRWG"))
    wprwg = -12345789;
  if(!given("WPSCBE1"))
    wpscbe1 = -12345789;
  if(!given("WPSCBE2"))
    wpscbe2 = -12345789;
  if(!given("WPVAG"))
    wpvag = -12345789;
  if(!given("WR"))
    wr = -12345789;
  if(!given("WRDSW"))
    wrdsw = -12345789;
  if(!given("WRDW"))
    wrdw = -12345789;
  if(!given("WRGIDL"))
    wrgidl = -12345789;
  if(!given("WRGISL"))
    wrgisl = -12345789;
  if(!given("WRSW"))
    wrsw = -12345789;
  if(!given("WTETA0"))
    wteta0 = -12345789;
  if(!given("WTNFACTOR"))
    wtnfactor = -12345789;
  if(!given("WTVFBSDOFF"))
    wtvfbsdoff = -12345789;
  if(!given("WTVOFF"))
    wtvoff = -12345789;
  if(!given("WTVOFFCV"))
    wtvoffcv = -12345789;
  if(!given("WU0"))
    wu0 = -12345789;
  if(!given("WUA"))
    wua = -12345789;
  if(!given("WUA1"))
    wua1 = -12345789;
  if(!given("WUB"))
    wub = -12345789;
  if(!given("WUB1"))
    wub1 = -12345789;
  if(!given("WUC"))
    wuc = -12345789;
  if(!given("WUC1"))
    wuc1 = -12345789;
  if(!given("WUCS"))
    wucs = -12345789;
  if(!given("WUCSTE"))
    wucste = -12345789;
  if(!given("WUD"))
    wud = -12345789;
  if(!given("WUD1"))
    wud1 = -12345789;
  if(!given("WUP"))
    wup = -12345789;
  if(!given("WUTE"))
    wute = -12345789;
  if(!given("WVBM"))
    wvbm = -12345789;
  if(!given("WVBX"))
    wvbx = -12345789;
  if(!given("WVFB"))
    wvfb = -12345789;
  if(!given("WVFBCV"))
    wvfbcv = -12345789;
  if(!given("WVFBSDOFF"))
    wvfbsdoff = -12345789;
  if(!given("WVOFF"))
    wvoff = -12345789;
  if(!given("WVOFFCV"))
    wvoffcv = -12345789;
  if(!given("WVSAT"))
    wvsat = -12345789;
  if(!given("WVTH0"))
    wvth0 = -12345789;
  if(!given("WVTL"))
    wvtl = -12345789;
  if(!given("WW"))
    ww = -12345789;
  if(!given("WW0"))
    ww0 = -12345789;
  if(!given("WWC"))
    wwc = -12345789;
  if(!given("WWL"))
    wwl = -12345789;
  if(!given("WWLC"))
    wwlc = -12345789;
  if(!given("WWN"))
    wwn = -12345789;
  if(!given("WWR"))
    wwr = -12345789;
  if(!given("WXJ"))
    wxj = -12345789;
  if(!given("WXN"))
    wxn = -12345789;
  if(!given("WXRCRG1"))
    wxrcrg1 = -12345789;
  if(!given("WXRCRG2"))
    wxrcrg2 = -12345789;
  if(!given("WXT"))
    wxt = -12345789;
  if(!given("XGL"))
    xgl = -12345789;
  if(!given("XGW"))
    xgw = -12345789;
  if(!given("XJ"))
    xj = -12345789;
  if(!given("XJBVD"))
    xjbvd = -12345789;
  if(!given("XJBVS"))
    xjbvs = -12345789;
  if(!given("XL"))
    xl = -12345789;
  if(!given("XN"))
    xn = -12345789;
  if(!given("XPART"))
    xpart = -12345789;
  if(!given("XRCRG1"))
    xrcrg1 = -12345789;
  if(!given("XRCRG2"))
    xrcrg2 = -12345789;
  if(!given("XT"))
    xt = -12345789;
  if(!given("XTID"))
    xtid = -12345789;
  if(!given("XTIS"))
    xtis = -12345789;
  if(!given("XTSD"))
    xtsd = -12345789;
  if(!given("XTSS"))
    xtss = -12345789;
  if(!given("XTSSWD"))
    xtsswd = -12345789;
  if(!given("XTSSWGD"))
    xtsswgd = -12345789;
  if(!given("XTSSWGS"))
    xtsswgs = -12345789;
  if(!given("XTSSWS"))
    xtssws = -12345789;
  if(!given("XW"))
    xw = -12345789;
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
    _ckt_gmin(1e-12),
    _min(-9999999),
    a0(-12345789),
    a1(-12345789),
    a2(-12345789),
    acde(-12345789),
    acnqsmod(-9999999),
    ad(-12345789),
    ados(-12345789),
    af(-12345789),
    agidl(-12345789),
    agisl(-12345789),
    ags(-12345789),
    aigbacc(-12345789),
    aigbinv(-12345789),
    aigc(-12345789),
    aigd(-12345789),
    aigs(-12345789),
    aigsd(-12345789),
    alpha0(-12345789),
    alpha1(-12345789),
    as(-12345789),
    at(-12345789),
    b0(-12345789),
    b1(-12345789),
    bdos(-12345789),
    beta0(-12345789),
    bg0sub(-12345789),
    bgidl(-12345789),
    bgisl(-12345789),
    bigbacc(-12345789),
    bigbinv(-12345789),
    bigc(-12345789),
    bigd(-12345789),
    bigs(-12345789),
    bigsd(-12345789),
    binunit(-9999999),
    bvd(-12345789),
    bvs(-12345789),
    capmod(-9999999),
    cdsc(-12345789),
    cdscb(-12345789),
    cdscd(-12345789),
    cf(-12345789),
    cgbo(-12345789),
    cgdl(-12345789),
    cgdo(-12345789),
    cgidl(-12345789),
    cgisl(-12345789),
    cgsl(-12345789),
    cgso(-12345789),
    cigbacc(-12345789),
    cigbinv(-12345789),
    cigc(-12345789),
    cigd(-12345789),
    cigs(-12345789),
    cigsd(-12345789),
    cit(-12345789),
    cjd(-12345789),
    cjs(-12345789),
    cjswd(-12345789),
    cjswgd(-12345789),
    cjswgs(-12345789),
    cjsws(-12345789),
    ckappad(-12345789),
    ckappas(-12345789),
    clc(-12345789),
    cle(-12345789),
    cvchargemod(-9999999),
    delta(-12345789),
    delvto(-12345789),
    diomod(-9999999),
    dlc(-12345789),
    dlcig(-12345789),
    dlcigd(-12345789),
    dmcg(-12345789),
    dmcgt(-12345789),
    dmci(-12345789),
    dmdg(-12345789),
    drout(-12345789),
    dsub(-12345789),
    dtox(-12345789),
    dvt0(-12345789),
    dvt0w(-12345789),
    dvt1(-12345789),
    dvt1w(-12345789),
    dvt2(-12345789),
    dvt2w(-12345789),
    dvtp0(-12345789),
    dvtp1(-12345789),
    dvtp2(-12345789),
    dvtp3(-12345789),
    dvtp4(-12345789),
    dvtp5(-12345789),
    dwb(-12345789),
    dwc(-12345789),
    dwg(-12345789),
    dwj(-12345789),
    easub(-12345789),
    ef(-12345789),
    egidl(-12345789),
    egisl(-12345789),
    eigbinv(-12345789),
    em(-12345789),
    eot(-12345789),
    epsrgate(-12345789),
    epsrox(-12345789),
    epsrsub(-12345789),
    eta0(-12345789),
    etab(-12345789),
    eu(-12345789),
    fgidl(-12345789),
    fgisl(-12345789),
    fnoimod(-9999999),
    fprout(-12345789),
    gamma1(-12345789),
    gamma2(-12345789),
    gbmin(-12345789),
    geomod(-9999999),
    gidlmod(-9999999),
    igbmod(-9999999),
    igcmod(-9999999),
    ijthdfwd(-12345789),
    ijthdrev(-12345789),
    ijthsfwd(-12345789),
    ijthsrev(-12345789),
    jsd(-12345789),
    jss(-12345789),
    jswd(-12345789),
    jswgd(-12345789),
    jswgs(-12345789),
    jsws(-12345789),
    jtsd(-12345789),
    jtss(-12345789),
    jtsswd(-12345789),
    jtsswgd(-12345789),
    jtsswgs(-12345789),
    jtssws(-12345789),
    jtweff(-12345789),
    k1(-12345789),
    k2(-12345789),
    k2we(-12345789),
    k3(-12345789),
    k3b(-12345789),
    keta(-12345789),
    kf(-12345789),
    kgidl(-12345789),
    kgisl(-12345789),
    kt1(-12345789),
    kt1l(-12345789),
    kt2(-12345789),
    ku0(-12345789),
    ku0we(-12345789),
    kvsat(-12345789),
    kvth0(-12345789),
    kvth0we(-12345789),
    l(-12345789),
    la0(-12345789),
    la1(-12345789),
    la2(-12345789),
    lacde(-12345789),
    lagidl(-12345789),
    lagisl(-12345789),
    lags(-12345789),
    laigbacc(-12345789),
    laigbinv(-12345789),
    laigc(-12345789),
    laigd(-12345789),
    laigs(-12345789),
    laigsd(-12345789),
    lalpha0(-12345789),
    lalpha1(-12345789),
    lambda(-12345789),
    lat(-12345789),
    lb0(-12345789),
    lb1(-12345789),
    lbeta0(-12345789),
    lbgidl(-12345789),
    lbgisl(-12345789),
    lbigbacc(-12345789),
    lbigbinv(-12345789),
    lbigc(-12345789),
    lbigd(-12345789),
    lbigs(-12345789),
    lbigsd(-12345789),
    lc(-12345789),
    lcdsc(-12345789),
    lcdscb(-12345789),
    lcdscd(-12345789),
    lcf(-12345789),
    lcgdl(-12345789),
    lcgidl(-12345789),
    lcgisl(-12345789),
    lcgsl(-12345789),
    lcigbacc(-12345789),
    lcigbinv(-12345789),
    lcigc(-12345789),
    lcigd(-12345789),
    lcigs(-12345789),
    lcigsd(-12345789),
    lcit(-12345789),
    lckappad(-12345789),
    lckappas(-12345789),
    lclc(-12345789),
    lcle(-12345789),
    ldelta(-12345789),
    ldrout(-12345789),
    ldsub(-12345789),
    ldvt0(-12345789),
    ldvt0w(-12345789),
    ldvt1(-12345789),
    ldvt1w(-12345789),
    ldvt2(-12345789),
    ldvt2w(-12345789),
    ldvtp0(-12345789),
    ldvtp1(-12345789),
    ldvtp2(-12345789),
    ldvtp3(-12345789),
    ldvtp4(-12345789),
    ldvtp5(-12345789),
    ldwb(-12345789),
    ldwg(-12345789),
    leffeot(-12345789),
    legidl(-12345789),
    legisl(-12345789),
    leigbinv(-12345789),
    leta0(-12345789),
    letab(-12345789),
    leu(-12345789),
    level(-9999999),
    lfgidl(-12345789),
    lfgisl(-12345789),
    lfprout(-12345789),
    lgamma1(-12345789),
    lgamma2(-12345789),
    lint(-12345789),
    lk1(-12345789),
    lk2(-12345789),
    lk2we(-12345789),
    lk3(-12345789),
    lk3b(-12345789),
    lketa(-12345789),
    lkgidl(-12345789),
    lkgisl(-12345789),
    lkt1(-12345789),
    lkt1l(-12345789),
    lkt2(-12345789),
    lku0(-12345789),
    lku0we(-12345789),
    lkvth0(-12345789),
    lkvth0we(-12345789),
    ll(-12345789),
    llambda(-12345789),
    llc(-12345789),
    lln(-12345789),
    llodku0(-12345789),
    llodvth(-12345789),
    llp(-12345789),
    llpe0(-12345789),
    llpeb(-12345789),
    lmax(-12345789),
    lmin(-12345789),
    lminv(-12345789),
    lminvcv(-12345789),
    lmoin(-12345789),
    lndep(-12345789),
    lnfactor(-12345789),
    lngate(-12345789),
    lnigbacc(-12345789),
    lnigbinv(-12345789),
    lnigc(-12345789),
    lnoff(-12345789),
    lnsd(-12345789),
    lnsub(-12345789),
    lntox(-12345789),
    lodeta0(-12345789),
    lodk2(-12345789),
    lp(-12345789),
    lpclm(-12345789),
    lpdiblc1(-12345789),
    lpdiblc2(-12345789),
    lpdiblcb(-12345789),
    lpdits(-12345789),
    lpditsd(-12345789),
    lpe0(-12345789),
    lpeb(-12345789),
    lphin(-12345789),
    lpigcd(-12345789),
    lpoxedge(-12345789),
    lprt(-12345789),
    lprwb(-12345789),
    lprwg(-12345789),
    lpscbe1(-12345789),
    lpscbe2(-12345789),
    lpvag(-12345789),
    lrdsw(-12345789),
    lrdw(-12345789),
    lrgidl(-12345789),
    lrgisl(-12345789),
    lrsw(-12345789),
    lteta0(-12345789),
    ltnfactor(-12345789),
    ltvfbsdoff(-12345789),
    ltvoff(-12345789),
    ltvoffcv(-12345789),
    lu0(-12345789),
    lua(-12345789),
    lua1(-12345789),
    lub(-12345789),
    lub1(-12345789),
    luc(-12345789),
    luc1(-12345789),
    lucs(-12345789),
    lucste(-12345789),
    lud(-12345789),
    lud1(-12345789),
    lup(-12345789),
    lute(-12345789),
    lvbm(-12345789),
    lvbx(-12345789),
    lvfb(-12345789),
    lvfbcv(-12345789),
    lvfbsdoff(-12345789),
    lvoff(-12345789),
    lvoffcv(-12345789),
    lvsat(-12345789),
    lvth0(-12345789),
    lvtl(-12345789),
    lw(-12345789),
    lw0(-12345789),
    lwc(-12345789),
    lwl(-12345789),
    lwlc(-12345789),
    lwn(-12345789),
    lwr(-12345789),
    lxj(-12345789),
    lxn(-12345789),
    lxrcrg1(-12345789),
    lxrcrg2(-12345789),
    lxt(-12345789),
    minv(-12345789),
    minvcv(-12345789),
    mjd(-12345789),
    mjs(-12345789),
    mjswd(-12345789),
    mjswgd(-12345789),
    mjswgs(-12345789),
    mjsws(-12345789),
    mobmod(-9999999),
    moin(-12345789),
    mtrlcompatmod(-9999999),
    mtrlmod(-9999999),
    ndep(-12345789),
    nf(-9999999),
    nfactor(-12345789),
    ngate(-12345789),
    ngcon(-12345789),
    ni0sub(-12345789),
    nigbacc(-12345789),
    nigbinv(-12345789),
    nigc(-12345789),
    njd(-12345789),
    njs(-12345789),
    njts(-12345789),
    njtsd(-12345789),
    njtssw(-12345789),
    njtsswd(-12345789),
    njtsswg(-12345789),
    njtsswgd(-12345789),
    noff(-12345789),
    noia(-12345789),
    noib(-12345789),
    noic(-12345789),
    nrd(-12345789),
    nrs(-12345789),
    nsd(-12345789),
    nsub(-12345789),
    ntnoi(-12345789),
    ntox(-12345789),
    off(-9999999),
    pa0(-12345789),
    pa1(-12345789),
    pa2(-12345789),
    pacde(-12345789),
    pagidl(-12345789),
    pagisl(-12345789),
    pags(-12345789),
    paigbacc(-12345789),
    paigbinv(-12345789),
    paigc(-12345789),
    paigd(-12345789),
    paigs(-12345789),
    paigsd(-12345789),
    palpha0(-12345789),
    palpha1(-12345789),
    paramchk(-9999999),
    pat(-12345789),
    pb0(-12345789),
    pb1(-12345789),
    pbd(-12345789),
    pbeta0(-12345789),
    pbgidl(-12345789),
    pbgisl(-12345789),
    pbigbacc(-12345789),
    pbigbinv(-12345789),
    pbigc(-12345789),
    pbigd(-12345789),
    pbigs(-12345789),
    pbigsd(-12345789),
    pbs(-12345789),
    pbswd(-12345789),
    pbswgd(-12345789),
    pbswgs(-12345789),
    pbsws(-12345789),
    pcdsc(-12345789),
    pcdscb(-12345789),
    pcdscd(-12345789),
    pcf(-12345789),
    pcgdl(-12345789),
    pcgidl(-12345789),
    pcgisl(-12345789),
    pcgsl(-12345789),
    pcigbacc(-12345789),
    pcigbinv(-12345789),
    pcigc(-12345789),
    pcigd(-12345789),
    pcigs(-12345789),
    pcigsd(-12345789),
    pcit(-12345789),
    pckappad(-12345789),
    pckappas(-12345789),
    pclc(-12345789),
    pcle(-12345789),
    pclm(-12345789),
    pd(-12345789),
    pdelta(-12345789),
    pdiblc1(-12345789),
    pdiblc2(-12345789),
    pdiblcb(-12345789),
    pdits(-12345789),
    pditsd(-12345789),
    pditsl(-12345789),
    pdrout(-12345789),
    pdsub(-12345789),
    pdvt0(-12345789),
    pdvt0w(-12345789),
    pdvt1(-12345789),
    pdvt1w(-12345789),
    pdvt2(-12345789),
    pdvt2w(-12345789),
    pdvtp0(-12345789),
    pdvtp1(-12345789),
    pdvtp2(-12345789),
    pdvtp3(-12345789),
    pdvtp4(-12345789),
    pdvtp5(-12345789),
    pdwb(-12345789),
    pdwg(-12345789),
    pegidl(-12345789),
    pegisl(-12345789),
    peigbinv(-12345789),
    permod(-9999999),
    peta0(-12345789),
    petab(-12345789),
    peu(-12345789),
    pfgidl(-12345789),
    pfgisl(-12345789),
    pfprout(-12345789),
    pgamma1(-12345789),
    pgamma2(-12345789),
    phig(-12345789),
    phin(-12345789),
    pigcd(-12345789),
    pk1(-12345789),
    pk2(-12345789),
    pk2we(-12345789),
    pk3(-12345789),
    pk3b(-12345789),
    pketa(-12345789),
    pkgidl(-12345789),
    pkgisl(-12345789),
    pkt1(-12345789),
    pkt1l(-12345789),
    pkt2(-12345789),
    pku0(-12345789),
    pku0we(-12345789),
    pkvth0(-12345789),
    pkvth0we(-12345789),
    plambda(-12345789),
    plp(-12345789),
    plpe0(-12345789),
    plpeb(-12345789),
    pminv(-12345789),
    pminvcv(-12345789),
    pmoin(-12345789),
    pndep(-12345789),
    pnfactor(-12345789),
    pngate(-12345789),
    pnigbacc(-12345789),
    pnigbinv(-12345789),
    pnigc(-12345789),
    pnoff(-12345789),
    pnsd(-12345789),
    pnsub(-12345789),
    pntox(-12345789),
    poxedge(-12345789),
    ppclm(-12345789),
    ppdiblc1(-12345789),
    ppdiblc2(-12345789),
    ppdiblcb(-12345789),
    ppdits(-12345789),
    ppditsd(-12345789),
    pphin(-12345789),
    ppigcd(-12345789),
    ppoxedge(-12345789),
    pprt(-12345789),
    pprwb(-12345789),
    pprwg(-12345789),
    ppscbe1(-12345789),
    ppscbe2(-12345789),
    ppvag(-12345789),
    prdsw(-12345789),
    prdw(-12345789),
    prgidl(-12345789),
    prgisl(-12345789),
    prsw(-12345789),
    prt(-12345789),
    prwb(-12345789),
    prwg(-12345789),
    ps(-12345789),
    pscbe1(-12345789),
    pscbe2(-12345789),
    pteta0(-12345789),
    ptnfactor(-12345789),
    ptvfbsdoff(-12345789),
    ptvoff(-12345789),
    ptvoffcv(-12345789),
    pu0(-12345789),
    pua(-12345789),
    pua1(-12345789),
    pub(-12345789),
    pub1(-12345789),
    puc(-12345789),
    puc1(-12345789),
    pucs(-12345789),
    pucste(-12345789),
    pud(-12345789),
    pud1(-12345789),
    pup(-12345789),
    pute(-12345789),
    pvag(-12345789),
    pvbm(-12345789),
    pvbx(-12345789),
    pvfb(-12345789),
    pvfbcv(-12345789),
    pvfbsdoff(-12345789),
    pvoff(-12345789),
    pvoffcv(-12345789),
    pvsat(-12345789),
    pvth0(-12345789),
    pvtl(-12345789),
    pw0(-12345789),
    pwr(-12345789),
    pxj(-12345789),
    pxn(-12345789),
    pxrcrg1(-12345789),
    pxrcrg2(-12345789),
    pxt(-12345789),
    rbdb(-12345789),
    rbdbx0(-12345789),
    rbdby0(-12345789),
    rbodymod(-9999999),
    rbpb(-12345789),
    rbpbx0(-12345789),
    rbpbxl(-12345789),
    rbpbxnf(-12345789),
    rbpbxw(-12345789),
    rbpby0(-12345789),
    rbpbyl(-12345789),
    rbpbynf(-12345789),
    rbpbyw(-12345789),
    rbpd(-12345789),
    rbpd0(-12345789),
    rbpdl(-12345789),
    rbpdnf(-12345789),
    rbpdw(-12345789),
    rbps(-12345789),
    rbps0(-12345789),
    rbpsl(-12345789),
    rbpsnf(-12345789),
    rbpsw(-12345789),
    rbsb(-12345789),
    rbsbx0(-12345789),
    rbsby0(-12345789),
    rbsdbxl(-12345789),
    rbsdbxnf(-12345789),
    rbsdbxw(-12345789),
    rbsdbyl(-12345789),
    rbsdbynf(-12345789),
    rbsdbyw(-12345789),
    rdsmod(-9999999),
    rdsw(-12345789),
    rdswmin(-12345789),
    rdw(-12345789),
    rdwmin(-12345789),
    rgatemod(-9999999),
    rgeomod(-9999999),
    rgidl(-12345789),
    rgisl(-12345789),
    rnoia(-12345789),
    rnoib(-12345789),
    rnoic(-12345789),
    rsh(-12345789),
    rshg(-12345789),
    rsw(-12345789),
    rswmin(-12345789),
    sa(-12345789),
    saref(-12345789),
    sb(-12345789),
    sbref(-12345789),
    sc(-12345789),
    sca(-12345789),
    scb(-12345789),
    scc(-12345789),
    scref(-12345789),
    sd(-12345789),
    steta0(-12345789),
    stk2(-12345789),
    tbgasub(-12345789),
    tbgbsub(-12345789),
    tcj(-12345789),
    tcjsw(-12345789),
    tcjswg(-12345789),
    tempeot(-12345789),
    tempmod(-9999999),
    teta0(-12345789),
    tku0(-12345789),
    tnfactor(-12345789),
    tnjts(-12345789),
    tnjtsd(-12345789),
    tnjtssw(-12345789),
    tnjtsswd(-12345789),
    tnjtsswg(-12345789),
    tnjtsswgd(-12345789),
    tnoia(-12345789),
    tnoib(-12345789),
    tnoic(-12345789),
    tnoimod(-9999999),
    tnom(-12345789),
    toxe(-12345789),
    toxm(-12345789),
    toxp(-12345789),
    toxref(-12345789),
    tpb(-12345789),
    tpbsw(-12345789),
    tpbswg(-12345789),
    trnqsmod(-9999999),
    tvfbsdoff(-12345789),
    tvoff(-12345789),
    tvoffcv(-12345789),
    type(-9999999),
    u0(-12345789),
    ua(-12345789),
    ua1(-12345789),
    ub(-12345789),
    ub1(-12345789),
    uc(-12345789),
    uc1(-12345789),
    ucs(-12345789),
    ucste(-12345789),
    ud(-12345789),
    ud1(-12345789),
    up(-12345789),
    ute(-12345789),
    vbm(-12345789),
    vbx(-12345789),
    vddeot(-12345789),
    verbose(-9999999),
    version(-12345789),
    vfb(-12345789),
    vfbcv(-12345789),
    vfbsdoff(-12345789),
    voff(-12345789),
    voffcv(-12345789),
    voffcvl(-12345789),
    voffl(-12345789),
    vsat(-12345789),
    vth0(-12345789),
    vtl(-12345789),
    vtsd(-12345789),
    vtss(-12345789),
    vtsswd(-12345789),
    vtsswgd(-12345789),
    vtsswgs(-12345789),
    vtssws(-12345789),
    w(-12345789),
    w0(-12345789),
    wa0(-12345789),
    wa1(-12345789),
    wa2(-12345789),
    wacde(-12345789),
    wagidl(-12345789),
    wagisl(-12345789),
    wags(-12345789),
    waigbacc(-12345789),
    waigbinv(-12345789),
    waigc(-12345789),
    waigd(-12345789),
    waigs(-12345789),
    waigsd(-12345789),
    walpha0(-12345789),
    walpha1(-12345789),
    wat(-12345789),
    wb0(-12345789),
    wb1(-12345789),
    wbeta0(-12345789),
    wbgidl(-12345789),
    wbgisl(-12345789),
    wbigbacc(-12345789),
    wbigbinv(-12345789),
    wbigc(-12345789),
    wbigd(-12345789),
    wbigs(-12345789),
    wbigsd(-12345789),
    wcdsc(-12345789),
    wcdscb(-12345789),
    wcdscd(-12345789),
    wcf(-12345789),
    wcgdl(-12345789),
    wcgidl(-12345789),
    wcgisl(-12345789),
    wcgsl(-12345789),
    wcigbacc(-12345789),
    wcigbinv(-12345789),
    wcigc(-12345789),
    wcigd(-12345789),
    wcigs(-12345789),
    wcigsd(-12345789),
    wcit(-12345789),
    wckappad(-12345789),
    wckappas(-12345789),
    wclc(-12345789),
    wcle(-12345789),
    wdelta(-12345789),
    wdrout(-12345789),
    wdsub(-12345789),
    wdvt0(-12345789),
    wdvt0w(-12345789),
    wdvt1(-12345789),
    wdvt1w(-12345789),
    wdvt2(-12345789),
    wdvt2w(-12345789),
    wdvtp0(-12345789),
    wdvtp1(-12345789),
    wdvtp2(-12345789),
    wdvtp3(-12345789),
    wdvtp4(-12345789),
    wdvtp5(-12345789),
    wdwb(-12345789),
    wdwg(-12345789),
    web(-12345789),
    wec(-12345789),
    weffeot(-12345789),
    wegidl(-12345789),
    wegisl(-12345789),
    weigbinv(-12345789),
    weta0(-12345789),
    wetab(-12345789),
    weu(-12345789),
    wfgidl(-12345789),
    wfgisl(-12345789),
    wfprout(-12345789),
    wgamma1(-12345789),
    wgamma2(-12345789),
    wint(-12345789),
    wk1(-12345789),
    wk2(-12345789),
    wk2we(-12345789),
    wk3(-12345789),
    wk3b(-12345789),
    wketa(-12345789),
    wkgidl(-12345789),
    wkgisl(-12345789),
    wkt1(-12345789),
    wkt1l(-12345789),
    wkt2(-12345789),
    wku0(-12345789),
    wku0we(-12345789),
    wkvth0(-12345789),
    wkvth0we(-12345789),
    wl(-12345789),
    wlambda(-12345789),
    wlc(-12345789),
    wln(-12345789),
    wlod(-12345789),
    wlodku0(-12345789),
    wlodvth(-12345789),
    wlp(-12345789),
    wlpe0(-12345789),
    wlpeb(-12345789),
    wmax(-12345789),
    wmin(-12345789),
    wminv(-12345789),
    wminvcv(-12345789),
    wmoin(-12345789),
    wndep(-12345789),
    wnfactor(-12345789),
    wngate(-12345789),
    wnigbacc(-12345789),
    wnigbinv(-12345789),
    wnigc(-12345789),
    wnoff(-12345789),
    wnsd(-12345789),
    wnsub(-12345789),
    wntox(-12345789),
    wpclm(-12345789),
    wpdiblc1(-12345789),
    wpdiblc2(-12345789),
    wpdiblcb(-12345789),
    wpdits(-12345789),
    wpditsd(-12345789),
    wpemod(-9999999),
    wphin(-12345789),
    wpigcd(-12345789),
    wpoxedge(-12345789),
    wprt(-12345789),
    wprwb(-12345789),
    wprwg(-12345789),
    wpscbe1(-12345789),
    wpscbe2(-12345789),
    wpvag(-12345789),
    wr(-12345789),
    wrdsw(-12345789),
    wrdw(-12345789),
    wrgidl(-12345789),
    wrgisl(-12345789),
    wrsw(-12345789),
    wteta0(-12345789),
    wtnfactor(-12345789),
    wtvfbsdoff(-12345789),
    wtvoff(-12345789),
    wtvoffcv(-12345789),
    wu0(-12345789),
    wua(-12345789),
    wua1(-12345789),
    wub(-12345789),
    wub1(-12345789),
    wuc(-12345789),
    wuc1(-12345789),
    wucs(-12345789),
    wucste(-12345789),
    wud(-12345789),
    wud1(-12345789),
    wup(-12345789),
    wute(-12345789),
    wvbm(-12345789),
    wvbx(-12345789),
    wvfb(-12345789),
    wvfbcv(-12345789),
    wvfbsdoff(-12345789),
    wvoff(-12345789),
    wvoffcv(-12345789),
    wvsat(-12345789),
    wvth0(-12345789),
    wvtl(-12345789),
    ww(-12345789),
    ww0(-12345789),
    wwc(-12345789),
    wwl(-12345789),
    wwlc(-12345789),
    wwn(-12345789),
    wwr(-12345789),
    wxj(-12345789),
    wxn(-12345789),
    wxrcrg1(-12345789),
    wxrcrg2(-12345789),
    wxt(-12345789),
    xgl(-12345789),
    xgw(-12345789),
    xj(-12345789),
    xjbvd(-12345789),
    xjbvs(-12345789),
    xl(-12345789),
    xn(-12345789),
    xpart(-12345789),
    xrcrg1(-12345789),
    xrcrg2(-12345789),
    xt(-12345789),
    xtid(-12345789),
    xtis(-12345789),
    xtsd(-12345789),
    xtss(-12345789),
    xtsswd(-12345789),
    xtsswgd(-12345789),
    xtsswgs(-12345789),
    xtssws(-12345789),
    xw(-12345789)
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
    .registerDevice("bsim4_va", 1)
    .registerModelType("bsim4_va", 1);
}
} // namespace COGENDAbsim4_va
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAbsim4_va::registerDevice(); }}; 
Bootstrap COGENDAbsim4_va_bootstrap;

