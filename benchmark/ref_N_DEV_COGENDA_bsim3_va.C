//-------------------------------------------------------------------------
// ----------   Xyce Includes in Cxx ----------
#include <Xyce_config.h>
#include "bsim3_va.h"

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
namespace COGENDAbsim3_va {

    const int Instance::cogendaNodeID_d;
    const int Instance::cogendaNodeID_g;
    const int Instance::cogendaNodeID_s;
    const int Instance::cogendaNodeID_b;
    const int Instance::cogendaNodeID_di;
    const int Instance::cogendaNodeID_si;
    const int Instance::cogendaNodeID_GND;

/* class Traits member functions */
  //void Traits::loadInstanceParameters() 
void Traits::loadInstanceParameters(ParametricData<COGENDAbsim3_va::Instance> &p)
{
  // This kludge is to force us always to have an instance parameter
  // that the device manager can set to the temperature, even if we have
  // no "TEMP".
  p.addPar("XYCEADMSINSTTEMP", 0.0, &COGENDAbsim3_va::Instance::cogendaInstTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device instance temperature");
  p.addPar("_CKT_GMIN",static_cast<double>(1e-12), &COGENDAbsim3_va::Instance::_ckt_gmin);
  p.addPar("A0",static_cast<double>(1), &COGENDAbsim3_va::Instance::a0);
  p.addPar("A1",static_cast<double>(0), &COGENDAbsim3_va::Instance::a1);
  p.addPar("A2",static_cast<double>(1), &COGENDAbsim3_va::Instance::a2);
  p.addPar("ACDE",static_cast<double>(1), &COGENDAbsim3_va::Instance::acde);
  p.addPar("ACM",static_cast<int>(-9999999), &COGENDAbsim3_va::Instance::acm);
  p.addPar("AD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ad);
  p.addPar("AF",static_cast<double>(1), &COGENDAbsim3_va::Instance::af);
  p.addPar("AGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::agidl);
  p.addPar("AGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::agisl);
  p.addPar("AGS",static_cast<double>(0), &COGENDAbsim3_va::Instance::ags);
  p.addPar("AIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigbacc);
  p.addPar("AIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigbinv);
  p.addPar("AIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigc);
  p.addPar("AIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigd);
  p.addPar("AIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigs);
  p.addPar("AIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::aigsd);
  p.addPar("ALPHA0",static_cast<double>(0), &COGENDAbsim3_va::Instance::alpha0);
  p.addPar("ALPHA1",static_cast<double>(0), &COGENDAbsim3_va::Instance::alpha1);
  p.addPar("AS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::as);
  p.addPar("AT",static_cast<double>(3.3e4), &COGENDAbsim3_va::Instance::at);
  p.addPar("B0",static_cast<double>(0), &COGENDAbsim3_va::Instance::b0);
  p.addPar("B1",static_cast<double>(0), &COGENDAbsim3_va::Instance::b1);
  p.addPar("BETA0",static_cast<double>(30), &COGENDAbsim3_va::Instance::beta0);
  p.addPar("BGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bgidl);
  p.addPar("BGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bgisl);
  p.addPar("BIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigbacc);
  p.addPar("BIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigbinv);
  p.addPar("BIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigc);
  p.addPar("BIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigd);
  p.addPar("BIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigs);
  p.addPar("BIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bigsd);
  p.addPar("BINFLAG",static_cast<int>(0), &COGENDAbsim3_va::Instance::binflag);
  p.addPar("BINUNIT",static_cast<int>(1), &COGENDAbsim3_va::Instance::binunit);
  p.addPar("BREAKMOD",static_cast<int>(0), &COGENDAbsim3_va::Instance::breakmod);
  p.addPar("BVD",static_cast<double>(10.0), &COGENDAbsim3_va::Instance::bvd);
  p.addPar("BVJ",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::bvj);
  p.addPar("BVS",static_cast<double>(10.0), &COGENDAbsim3_va::Instance::bvs);
  p.addPar("CALCACM",static_cast<int>(-9999999), &COGENDAbsim3_va::Instance::calcacm);
  p.addPar("CAPMOD",static_cast<int>(3), &COGENDAbsim3_va::Instance::capmod);
  p.addPar("CBD",static_cast<double>(0), &COGENDAbsim3_va::Instance::cbd);
  p.addPar("CBS",static_cast<double>(0), &COGENDAbsim3_va::Instance::cbs);
  p.addPar("CDSC",static_cast<double>(2.4e-4), &COGENDAbsim3_va::Instance::cdsc);
  p.addPar("CDSCB",static_cast<double>(0), &COGENDAbsim3_va::Instance::cdscb);
  p.addPar("CDSCD",static_cast<double>(0), &COGENDAbsim3_va::Instance::cdscd);
  p.addPar("CF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cf);
  p.addPar("CGBO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cgbo);
  p.addPar("CGDL",static_cast<double>(0), &COGENDAbsim3_va::Instance::cgdl);
  p.addPar("CGDO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cgdo);
  p.addPar("CGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cgidl);
  p.addPar("CGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cgisl);
  p.addPar("CGSL",static_cast<double>(0), &COGENDAbsim3_va::Instance::cgsl);
  p.addPar("CGSO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cgso);
  p.addPar("CIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigbacc);
  p.addPar("CIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigbinv);
  p.addPar("CIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigc);
  p.addPar("CIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigd);
  p.addPar("CIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigs);
  p.addPar("CIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::cigsd);
  p.addPar("CIT",static_cast<double>(0), &COGENDAbsim3_va::Instance::cit);
  p.addPar("CJ",static_cast<double>(5e-4), &COGENDAbsim3_va::Instance::cj);
  p.addPar("CJGATE",static_cast<double>(5e-10), &COGENDAbsim3_va::Instance::cjgate);
  p.addPar("CJSW",static_cast<double>(5e-10), &COGENDAbsim3_va::Instance::cjsw);
  p.addPar("CJSWG",static_cast<double>(5e-10), &COGENDAbsim3_va::Instance::cjswg);
  p.addPar("CKAPPA",static_cast<double>(0.6), &COGENDAbsim3_va::Instance::ckappa);
  p.addPar("CLC",static_cast<double>(0.1e-6), &COGENDAbsim3_va::Instance::clc);
  p.addPar("CLE",static_cast<double>(0.6), &COGENDAbsim3_va::Instance::cle);
  p.addPar("CTA",static_cast<double>(0), &COGENDAbsim3_va::Instance::cta);
  p.addPar("CTP",static_cast<double>(0), &COGENDAbsim3_va::Instance::ctp);
  p.addPar("DEL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::del);
  p.addPar("DELK1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::delk1);
  p.addPar("DELNFCT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::delnfct);
  p.addPar("DELTA",static_cast<double>(0.01), &COGENDAbsim3_va::Instance::delta);
  p.addPar("DELTOX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::deltox);
  p.addPar("DELVTO",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::delvto);
  p.addPar("DLC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::dlc);
  p.addPar("DLCIG",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::dlcig);
  p.addPar("DROUT",static_cast<double>(0.56), &COGENDAbsim3_va::Instance::drout);
  p.addPar("DSUB",static_cast<double>(0.56), &COGENDAbsim3_va::Instance::dsub);
  p.addPar("DTOXCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::dtoxcv);
  p.addPar("DVT0",static_cast<double>(2.2), &COGENDAbsim3_va::Instance::dvt0);
  p.addPar("DVT0W",static_cast<double>(0), &COGENDAbsim3_va::Instance::dvt0w);
  p.addPar("DVT1",static_cast<double>(0.53), &COGENDAbsim3_va::Instance::dvt1);
  p.addPar("DVT1W",static_cast<double>(5.3e6), &COGENDAbsim3_va::Instance::dvt1w);
  p.addPar("DVT2",static_cast<double>(-0.032), &COGENDAbsim3_va::Instance::dvt2);
  p.addPar("DVT2W",static_cast<double>(-0.032), &COGENDAbsim3_va::Instance::dvt2w);
  p.addPar("DWB",static_cast<double>(0), &COGENDAbsim3_va::Instance::dwb);
  p.addPar("DWC",static_cast<double>(0), &COGENDAbsim3_va::Instance::dwc);
  p.addPar("DWG",static_cast<double>(0), &COGENDAbsim3_va::Instance::dwg);
  p.addPar("EF",static_cast<double>(1), &COGENDAbsim3_va::Instance::ef);
  p.addPar("EG",static_cast<double>(1.12452), &COGENDAbsim3_va::Instance::eg);
  p.addPar("EGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::egidl);
  p.addPar("EGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::egisl);
  p.addPar("EIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::eigbinv);
  p.addPar("ELM",static_cast<double>(5), &COGENDAbsim3_va::Instance::elm);
  p.addPar("EM",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::em);
  p.addPar("ETA0",static_cast<double>(0.08), &COGENDAbsim3_va::Instance::eta0);
  p.addPar("ETAB",static_cast<double>(-0.07), &COGENDAbsim3_va::Instance::etab);
  p.addPar("FC",static_cast<double>(0.5), &COGENDAbsim3_va::Instance::fc);
  p.addPar("FCSW",static_cast<double>(0.5), &COGENDAbsim3_va::Instance::fcsw);
  p.addPar("FGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::fgidl);
  p.addPar("FGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::fgisl);
  p.addPar("FLKMOD",static_cast<double>(0), &COGENDAbsim3_va::Instance::flkmod);
  p.addPar("GAMMA1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::gamma1);
  p.addPar("GAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::gamma2);
  p.addPar("GAP1",static_cast<double>(7.02e-4), &COGENDAbsim3_va::Instance::gap1);
  p.addPar("GAP2",static_cast<double>(1108), &COGENDAbsim3_va::Instance::gap2);
  p.addPar("GEO",static_cast<int>(0), &COGENDAbsim3_va::Instance::geo);
  p.addPar("GIDLMOD",static_cast<int>(0), &COGENDAbsim3_va::Instance::gidlmod);
  p.addPar("HDIF",static_cast<double>(0), &COGENDAbsim3_va::Instance::hdif);
  p.addPar("IGBMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Instance::igbmod);
  p.addPar("IGCMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Instance::igcmod);
  p.addPar("IJTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ijth);
  p.addPar("IJTHDREV",static_cast<double>(0.1), &COGENDAbsim3_va::Instance::ijthdrev);
  p.addPar("IJTHSREV",static_cast<double>(0.1), &COGENDAbsim3_va::Instance::ijthsrev);
  p.addPar("IMAX",static_cast<double>(1), &COGENDAbsim3_va::Instance::imax);
  p.addPar("IMELT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::imelt);
  p.addPar("IS",static_cast<double>(1e-14), &COGENDAbsim3_va::Instance::is);
  p.addPar("JMAX",static_cast<double>(1e8), &COGENDAbsim3_va::Instance::jmax);
  p.addPar("JMELT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::jmelt);
  p.addPar("JS",static_cast<double>(1e-4), &COGENDAbsim3_va::Instance::js);
  p.addPar("JSSW",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::jssw);
  p.addPar("JSW",static_cast<double>(0), &COGENDAbsim3_va::Instance::jsw);
  p.addPar("K1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::k1);
  p.addPar("K2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::k2);
  p.addPar("K3",static_cast<double>(80), &COGENDAbsim3_va::Instance::k3);
  p.addPar("K3B",static_cast<double>(0), &COGENDAbsim3_va::Instance::k3b);
  p.addPar("KETA",static_cast<double>(-0.047), &COGENDAbsim3_va::Instance::keta);
  p.addPar("KF",static_cast<double>(0), &COGENDAbsim3_va::Instance::kf);
  p.addPar("KGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::kgidl);
  p.addPar("KGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::kgisl);
  p.addPar("KT1",static_cast<double>(-0.11), &COGENDAbsim3_va::Instance::kt1);
  p.addPar("KT1L",static_cast<double>(0), &COGENDAbsim3_va::Instance::kt1l);
  p.addPar("KT2",static_cast<double>(0.022), &COGENDAbsim3_va::Instance::kt2);
  p.addPar("KU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ku0);
  p.addPar("KVSAT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::kvsat);
  p.addPar("KVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::kvth0);
  p.addPar("L",static_cast<double>(5e-6), &COGENDAbsim3_va::Instance::l);
  p.addPar("LA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::la0);
  p.addPar("LA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::la1);
  p.addPar("LA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::la2);
  p.addPar("LACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lacde);
  p.addPar("LAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lagidl);
  p.addPar("LAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lagisl);
  p.addPar("LAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lags);
  p.addPar("LAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigbacc);
  p.addPar("LAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigbinv);
  p.addPar("LAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigc);
  p.addPar("LAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigd);
  p.addPar("LAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigs);
  p.addPar("LAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::laigsd);
  p.addPar("LALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lalpha0);
  p.addPar("LALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lalpha1);
  p.addPar("LAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lat);
  p.addPar("LB0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lb0);
  p.addPar("LB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lb1);
  p.addPar("LBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lbeta0);
  p.addPar("LBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbgidl);
  p.addPar("LBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbgisl);
  p.addPar("LBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigbacc);
  p.addPar("LBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigbinv);
  p.addPar("LBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigc);
  p.addPar("LBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigd);
  p.addPar("LBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigs);
  p.addPar("LBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lbigsd);
  p.addPar("LCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcdsc);
  p.addPar("LCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcdscb);
  p.addPar("LCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcdscd);
  p.addPar("LCF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcf);
  p.addPar("LCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcgdl);
  p.addPar("LCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcgidl);
  p.addPar("LCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcgisl);
  p.addPar("LCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcgsl);
  p.addPar("LCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigbacc);
  p.addPar("LCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigbinv);
  p.addPar("LCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigc);
  p.addPar("LCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigd);
  p.addPar("LCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigs);
  p.addPar("LCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lcigsd);
  p.addPar("LCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcit);
  p.addPar("LCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lckappa);
  p.addPar("LCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lclc);
  p.addPar("LCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lcle);
  p.addPar("LD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ld);
  p.addPar("LDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldelta);
  p.addPar("LDIF",static_cast<double>(0), &COGENDAbsim3_va::Instance::ldif);
  p.addPar("LDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldrout);
  p.addPar("LDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldsub);
  p.addPar("LDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt0);
  p.addPar("LDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt0w);
  p.addPar("LDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt1);
  p.addPar("LDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt1w);
  p.addPar("LDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt2);
  p.addPar("LDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldvt2w);
  p.addPar("LDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldwb);
  p.addPar("LDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ldwg);
  p.addPar("LEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::legidl);
  p.addPar("LEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::legisl);
  p.addPar("LEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::leigbinv);
  p.addPar("LELM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lelm);
  p.addPar("LETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::leta0);
  p.addPar("LETAB",static_cast<double>(-0.0), &COGENDAbsim3_va::Instance::letab);
  p.addPar("LEVEL",static_cast<int>(49), &COGENDAbsim3_va::Instance::level);
  p.addPar("LFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lfgidl);
  p.addPar("LFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lfgisl);
  p.addPar("LGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lgamma1);
  p.addPar("LGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lgamma2);
  p.addPar("LGCD",static_cast<double>(0), &COGENDAbsim3_va::Instance::lgcd);
  p.addPar("LGCS",static_cast<double>(0), &COGENDAbsim3_va::Instance::lgcs);
  p.addPar("LINT",static_cast<double>(0), &COGENDAbsim3_va::Instance::lint);
  p.addPar("LK1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lk1);
  p.addPar("LK2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lk2);
  p.addPar("LK3",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lk3);
  p.addPar("LK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lk3b);
  p.addPar("LKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lketa);
  p.addPar("LKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lkgidl);
  p.addPar("LKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lkgisl);
  p.addPar("LKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lkt1);
  p.addPar("LKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lkt1l);
  p.addPar("LKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lkt2);
  p.addPar("LKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lku0);
  p.addPar("LKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lkvth0);
  p.addPar("LL",static_cast<double>(0), &COGENDAbsim3_va::Instance::ll);
  p.addPar("LLC",static_cast<double>(0), &COGENDAbsim3_va::Instance::llc);
  p.addPar("LLN",static_cast<double>(1), &COGENDAbsim3_va::Instance::lln);
  p.addPar("LLODKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::llodku0);
  p.addPar("LLODVTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::llodvth);
  p.addPar("LMAX",static_cast<double>(1), &COGENDAbsim3_va::Instance::lmax);
  p.addPar("LMIN",static_cast<double>(0), &COGENDAbsim3_va::Instance::lmin);
  p.addPar("LMLT",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::lmlt);
  p.addPar("LMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lmoin);
  p.addPar("LNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnch);
  p.addPar("LNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnfactor);
  p.addPar("LNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lngate);
  p.addPar("LNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lnigbacc);
  p.addPar("LNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lnigbinv);
  p.addPar("LNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lnigc);
  p.addPar("LNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnlx);
  p.addPar("LNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnoff);
  p.addPar("LNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnpeak);
  p.addPar("LNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lnsd);
  p.addPar("LNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lnsub);
  p.addPar("LNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lntox);
  p.addPar("LODETA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lodeta0);
  p.addPar("LODK2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lodk2);
  p.addPar("LPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpclm);
  p.addPar("LPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpdiblc1);
  p.addPar("LPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpdiblc2);
  p.addPar("LPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpdiblcb);
  p.addPar("LPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lpigcd);
  p.addPar("LPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lpoxedge);
  p.addPar("LPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lprt);
  p.addPar("LPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lprwb);
  p.addPar("LPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lprwg);
  p.addPar("LPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpscbe1);
  p.addPar("LPSCBE2",static_cast<double>(0), &COGENDAbsim3_va::Instance::lpscbe2);
  p.addPar("LPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lpvag);
  p.addPar("LRD",static_cast<double>(0), &COGENDAbsim3_va::Instance::lrd);
  p.addPar("LRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lrdsw);
  p.addPar("LREF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lref);
  p.addPar("LRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lrgidl);
  p.addPar("LRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lrgisl);
  p.addPar("LRS",static_cast<double>(0), &COGENDAbsim3_va::Instance::lrs);
  p.addPar("LTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ltvfbsdoff);
  p.addPar("LU0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lu0);
  p.addPar("LUA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lua);
  p.addPar("LUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lua1);
  p.addPar("LUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lub);
  p.addPar("LUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lub1);
  p.addPar("LUC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::luc);
  p.addPar("LUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::luc1);
  p.addPar("LUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lute);
  p.addPar("LVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvbm);
  p.addPar("LVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvbx);
  p.addPar("LVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvfb);
  p.addPar("LVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvfbcv);
  p.addPar("LVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::lvfbsdoff);
  p.addPar("LVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvoff);
  p.addPar("LVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvoffcv);
  p.addPar("LVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvsat);
  p.addPar("LVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lvth0);
  p.addPar("LW",static_cast<double>(0), &COGENDAbsim3_va::Instance::lw);
  p.addPar("LW0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lw0);
  p.addPar("LWC",static_cast<double>(0), &COGENDAbsim3_va::Instance::lwc);
  p.addPar("LWL",static_cast<double>(0), &COGENDAbsim3_va::Instance::lwl);
  p.addPar("LWLC",static_cast<double>(0), &COGENDAbsim3_va::Instance::lwlc);
  p.addPar("LWN",static_cast<double>(1), &COGENDAbsim3_va::Instance::lwn);
  p.addPar("LWR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lwr);
  p.addPar("LXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lxj);
  p.addPar("LXT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::lxt);
  p.addPar("METO",static_cast<double>(0), &COGENDAbsim3_va::Instance::meto);
  p.addPar("MINR",static_cast<double>(0.1), &COGENDAbsim3_va::Instance::minr);
  p.addPar("MJ",static_cast<double>(0.5), &COGENDAbsim3_va::Instance::mj);
  p.addPar("MJSW",static_cast<double>(0.33), &COGENDAbsim3_va::Instance::mjsw);
  p.addPar("MJSWG",static_cast<double>(0.33), &COGENDAbsim3_va::Instance::mjswg);
  p.addPar("MOBMOD",static_cast<int>(1), &COGENDAbsim3_va::Instance::mobmod);
  p.addPar("MOIN",static_cast<double>(15), &COGENDAbsim3_va::Instance::moin);
  p.addPar("MULU0",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::mulu0);
  p.addPar("MULUA",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::mulua);
  p.addPar("MULUB",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::mulub);
  p.addPar("NCH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nch);
  p.addPar("NF",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::nf);
  p.addPar("NFACTOR",static_cast<double>(1), &COGENDAbsim3_va::Instance::nfactor);
  p.addPar("NGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ngate);
  p.addPar("NIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nigbacc);
  p.addPar("NIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nigbinv);
  p.addPar("NIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nigc);
  p.addPar("NJ",static_cast<double>(1), &COGENDAbsim3_va::Instance::nj);
  p.addPar("NLX",static_cast<double>(1.74e-7), &COGENDAbsim3_va::Instance::nlx);
  p.addPar("NOFF",static_cast<double>(1), &COGENDAbsim3_va::Instance::noff);
  p.addPar("NOIA",static_cast<double>(1e20), &COGENDAbsim3_va::Instance::noia);
  p.addPar("NOIB",static_cast<double>(5e4), &COGENDAbsim3_va::Instance::noib);
  p.addPar("NOIC",static_cast<double>(-1.4e-12), &COGENDAbsim3_va::Instance::noic);
  p.addPar("NOIMOD",static_cast<int>(1), &COGENDAbsim3_va::Instance::noimod);
  p.addPar("NQSMOD",static_cast<int>(0), &COGENDAbsim3_va::Instance::nqsmod);
  p.addPar("NRD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nrd);
  p.addPar("NRS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nrs);
  p.addPar("NSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nsd);
  p.addPar("NSUB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::nsub);
  p.addPar("NTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ntox);
  p.addPar("PA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pa0);
  p.addPar("PA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pa1);
  p.addPar("PA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pa2);
  p.addPar("PACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pacde);
  p.addPar("PAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pagidl);
  p.addPar("PAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pagisl);
  p.addPar("PAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pags);
  p.addPar("PAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigbacc);
  p.addPar("PAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigbinv);
  p.addPar("PAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigc);
  p.addPar("PAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigd);
  p.addPar("PAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigs);
  p.addPar("PAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::paigsd);
  p.addPar("PALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::palpha0);
  p.addPar("PALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::palpha1);
  p.addPar("PARAMCHK",static_cast<int>(0), &COGENDAbsim3_va::Instance::paramchk);
  p.addPar("PAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pat);
  p.addPar("PB",static_cast<double>(1), &COGENDAbsim3_va::Instance::pb);
  p.addPar("PB0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pb0);
  p.addPar("PB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pb1);
  p.addPar("PBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pbeta0);
  p.addPar("PBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbgidl);
  p.addPar("PBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbgisl);
  p.addPar("PBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigbacc);
  p.addPar("PBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigbinv);
  p.addPar("PBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigc);
  p.addPar("PBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigd);
  p.addPar("PBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigs);
  p.addPar("PBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pbigsd);
  p.addPar("PBSW",static_cast<double>(1), &COGENDAbsim3_va::Instance::pbsw);
  p.addPar("PBSWG",static_cast<double>(1), &COGENDAbsim3_va::Instance::pbswg);
  p.addPar("PCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcdsc);
  p.addPar("PCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcdscb);
  p.addPar("PCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcdscd);
  p.addPar("PCF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcf);
  p.addPar("PCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcgdl);
  p.addPar("PCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcgidl);
  p.addPar("PCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcgisl);
  p.addPar("PCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcgsl);
  p.addPar("PCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigbacc);
  p.addPar("PCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigbinv);
  p.addPar("PCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigc);
  p.addPar("PCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigd);
  p.addPar("PCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigs);
  p.addPar("PCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pcigsd);
  p.addPar("PCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcit);
  p.addPar("PCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pckappa);
  p.addPar("PCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pclc);
  p.addPar("PCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pcle);
  p.addPar("PCLM",static_cast<double>(1.3), &COGENDAbsim3_va::Instance::pclm);
  p.addPar("PD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pd);
  p.addPar("PDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdelta);
  p.addPar("PDIBLC1",static_cast<double>(0.39), &COGENDAbsim3_va::Instance::pdiblc1);
  p.addPar("PDIBLC2",static_cast<double>(0.0086), &COGENDAbsim3_va::Instance::pdiblc2);
  p.addPar("PDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdiblcb);
  p.addPar("PDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdrout);
  p.addPar("PDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdsub);
  p.addPar("PDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt0);
  p.addPar("PDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt0w);
  p.addPar("PDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt1);
  p.addPar("PDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt1w);
  p.addPar("PDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt2);
  p.addPar("PDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdvt2w);
  p.addPar("PDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdwb);
  p.addPar("PDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pdwg);
  p.addPar("PEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pegidl);
  p.addPar("PEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pegisl);
  p.addPar("PEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::peigbinv);
  p.addPar("PELM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pelm);
  p.addPar("PETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::peta0);
  p.addPar("PETAB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::petab);
  p.addPar("PFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pfgidl);
  p.addPar("PFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pfgisl);
  p.addPar("PGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pgamma1);
  p.addPar("PGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pgamma2);
  p.addPar("PHP",static_cast<double>(1), &COGENDAbsim3_va::Instance::php);
  p.addPar("PIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pigcd);
  p.addPar("PK1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pk1);
  p.addPar("PK2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pk2);
  p.addPar("PK3",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pk3);
  p.addPar("PK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pk3b);
  p.addPar("PKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pketa);
  p.addPar("PKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pkgidl);
  p.addPar("PKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pkgisl);
  p.addPar("PKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pkt1);
  p.addPar("PKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pkt1l);
  p.addPar("PKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pkt2);
  p.addPar("PKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pku0);
  p.addPar("PKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pkvth0);
  p.addPar("PMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pmoin);
  p.addPar("PNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnch);
  p.addPar("PNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnfactor);
  p.addPar("PNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pngate);
  p.addPar("PNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pnigbacc);
  p.addPar("PNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pnigbinv);
  p.addPar("PNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pnigc);
  p.addPar("PNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnlx);
  p.addPar("PNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnoff);
  p.addPar("PNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnpeak);
  p.addPar("PNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pnsd);
  p.addPar("PNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pnsub);
  p.addPar("PNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pntox);
  p.addPar("POXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::poxedge);
  p.addPar("PPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppclm);
  p.addPar("PPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppdiblc1);
  p.addPar("PPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppdiblc2);
  p.addPar("PPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppdiblcb);
  p.addPar("PPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ppigcd);
  p.addPar("PPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ppoxedge);
  p.addPar("PPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pprt);
  p.addPar("PPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pprwb);
  p.addPar("PPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pprwg);
  p.addPar("PPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppscbe1);
  p.addPar("PPSCBE2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppscbe2);
  p.addPar("PPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ppvag);
  p.addPar("PRD",static_cast<double>(0), &COGENDAbsim3_va::Instance::prd);
  p.addPar("PRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::prdsw);
  p.addPar("PRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::prgidl);
  p.addPar("PRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::prgisl);
  p.addPar("PRS",static_cast<double>(0), &COGENDAbsim3_va::Instance::prs);
  p.addPar("PRT",static_cast<double>(0), &COGENDAbsim3_va::Instance::prt);
  p.addPar("PRWB",static_cast<double>(0), &COGENDAbsim3_va::Instance::prwb);
  p.addPar("PRWG",static_cast<double>(0), &COGENDAbsim3_va::Instance::prwg);
  p.addPar("PS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ps);
  p.addPar("PSCBE1",static_cast<double>(4.24e8), &COGENDAbsim3_va::Instance::pscbe1);
  p.addPar("PSCBE2",static_cast<double>(1e-5), &COGENDAbsim3_va::Instance::pscbe2);
  p.addPar("PTA",static_cast<double>(0), &COGENDAbsim3_va::Instance::pta);
  p.addPar("PTP",static_cast<double>(0), &COGENDAbsim3_va::Instance::ptp);
  p.addPar("PTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::ptvfbsdoff);
  p.addPar("PU0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pu0);
  p.addPar("PUA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pua);
  p.addPar("PUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pua1);
  p.addPar("PUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pub);
  p.addPar("PUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pub1);
  p.addPar("PUC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::puc);
  p.addPar("PUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::puc1);
  p.addPar("PUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pute);
  p.addPar("PVAG",static_cast<double>(0), &COGENDAbsim3_va::Instance::pvag);
  p.addPar("PVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvbm);
  p.addPar("PVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvbx);
  p.addPar("PVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvfb);
  p.addPar("PVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvfbcv);
  p.addPar("PVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::pvfbsdoff);
  p.addPar("PVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvoff);
  p.addPar("PVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvoffcv);
  p.addPar("PVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvsat);
  p.addPar("PVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pvth0);
  p.addPar("PW0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pw0);
  p.addPar("PWR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pwr);
  p.addPar("PXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pxj);
  p.addPar("PXT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::pxt);
  p.addPar("RD",static_cast<double>(0), &COGENDAbsim3_va::Instance::rd);
  p.addPar("RDC",static_cast<double>(0), &COGENDAbsim3_va::Instance::rdc);
  p.addPar("RDSW",static_cast<double>(0), &COGENDAbsim3_va::Instance::rdsw);
  p.addPar("RGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::rgidl);
  p.addPar("RGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::rgisl);
  p.addPar("RS",static_cast<double>(0), &COGENDAbsim3_va::Instance::rs);
  p.addPar("RSC",static_cast<double>(0), &COGENDAbsim3_va::Instance::rsc);
  p.addPar("RSH",static_cast<double>(0), &COGENDAbsim3_va::Instance::rsh);
  p.addPar("SA",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa);
  p.addPar("SA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa0);
  p.addPar("SA1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa1);
  p.addPar("SA10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa10);
  p.addPar("SA2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa2);
  p.addPar("SA3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa3);
  p.addPar("SA4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa4);
  p.addPar("SA5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa5);
  p.addPar("SA6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa6);
  p.addPar("SA7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa7);
  p.addPar("SA8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa8);
  p.addPar("SA9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sa9);
  p.addPar("SAREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::saref);
  p.addPar("SB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb);
  p.addPar("SB0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb0);
  p.addPar("SB1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb1);
  p.addPar("SB10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb10);
  p.addPar("SB2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb2);
  p.addPar("SB3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb3);
  p.addPar("SB4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb4);
  p.addPar("SB5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb5);
  p.addPar("SB6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb6);
  p.addPar("SB7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb7);
  p.addPar("SB8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb8);
  p.addPar("SB9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sb9);
  p.addPar("SBREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sbref);
  p.addPar("SC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sc);
  p.addPar("SD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::sd);
  p.addPar("STETA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::steta0);
  p.addPar("STIMOD",static_cast<int>(0), &COGENDAbsim3_va::Instance::stimod);
  p.addPar("STK2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::stk2);
  p.addPar("SW1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw1);
  p.addPar("SW10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw10);
  p.addPar("SW2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw2);
  p.addPar("SW3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw3);
  p.addPar("SW4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw4);
  p.addPar("SW5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw5);
  p.addPar("SW6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw6);
  p.addPar("SW7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw7);
  p.addPar("SW8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw8);
  p.addPar("SW9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::sw9);
  p.addPar("TCJ",static_cast<double>(0), &COGENDAbsim3_va::Instance::tcj);
  p.addPar("TCJSW",static_cast<double>(0), &COGENDAbsim3_va::Instance::tcjsw);
  p.addPar("TCJSWG",static_cast<double>(0), &COGENDAbsim3_va::Instance::tcjswg);
  p.addPar("TEMPMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Instance::tempmod);
  p.addPar("TKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::tku0);
  p.addPar("TLEV",static_cast<double>(0), &COGENDAbsim3_va::Instance::tlev);
  p.addPar("TLEVC",static_cast<double>(0), &COGENDAbsim3_va::Instance::tlevc);
  p.addPar("TNOM",static_cast<double>(25), &COGENDAbsim3_va::Instance::tnom);
  p.addPar("TOX",static_cast<double>(150.0e-10), &COGENDAbsim3_va::Instance::tox);
  p.addPar("TOXE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::toxe);
  p.addPar("TOXM",static_cast<double>(150.0e-10), &COGENDAbsim3_va::Instance::toxm);
  p.addPar("TOXREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::toxref);
  p.addPar("TPB",static_cast<double>(0), &COGENDAbsim3_va::Instance::tpb);
  p.addPar("TPBSW",static_cast<double>(0), &COGENDAbsim3_va::Instance::tpbsw);
  p.addPar("TPBSWG",static_cast<double>(0), &COGENDAbsim3_va::Instance::tpbswg);
  p.addPar("TRD",static_cast<double>(0), &COGENDAbsim3_va::Instance::trd);
  p.addPar("TRISE",static_cast<double>(0), &COGENDAbsim3_va::Instance::trise);
  p.addPar("TRS",static_cast<double>(0), &COGENDAbsim3_va::Instance::trs);
  p.addPar("TVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::tvfbsdoff);
  p.addPar("TYPE",static_cast<int>(1), &COGENDAbsim3_va::Instance::type);
  p.addPar("U0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::u0);
  p.addPar("UA",static_cast<double>(2.25e-9), &COGENDAbsim3_va::Instance::ua);
  p.addPar("UA1",static_cast<double>(4.31e-9), &COGENDAbsim3_va::Instance::ua1);
  p.addPar("UB",static_cast<double>(5.87e-19), &COGENDAbsim3_va::Instance::ub);
  p.addPar("UB1",static_cast<double>(-7.61e-18), &COGENDAbsim3_va::Instance::ub1);
  p.addPar("UC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::uc);
  p.addPar("UC1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::uc1);
  p.addPar("UTE",static_cast<double>(-1.5), &COGENDAbsim3_va::Instance::ute);
  p.addPar("VBM",static_cast<double>(-3), &COGENDAbsim3_va::Instance::vbm);
  p.addPar("VBOX",static_cast<double>(1e9), &COGENDAbsim3_va::Instance::vbox);
  p.addPar("VBX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::vbx);
  p.addPar("VERBOSE",static_cast<int>(-9999999), &COGENDAbsim3_va::Instance::verbose);
  p.addPar("VERSION",static_cast<double>(3.3), &COGENDAbsim3_va::Instance::version);
  p.addPar("VFB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::vfb);
  p.addPar("VFBCV",static_cast<double>(-1), &COGENDAbsim3_va::Instance::vfbcv);
  p.addPar("VFBFLAG",static_cast<int>(0), &COGENDAbsim3_va::Instance::vfbflag);
  p.addPar("VFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::vfbsdoff);
  p.addPar("VGSLIM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::vgslim);
  p.addPar("VOFF",static_cast<double>(-0.08), &COGENDAbsim3_va::Instance::voff);
  p.addPar("VOFFCV",static_cast<double>(0), &COGENDAbsim3_va::Instance::voffcv);
  p.addPar("VSAT",static_cast<double>(8e4), &COGENDAbsim3_va::Instance::vsat);
  p.addPar("VTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::vth0);
  p.addPar("VTM0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::vtm0);
  p.addPar("W",static_cast<double>(5e-6), &COGENDAbsim3_va::Instance::w);
  p.addPar("W0",static_cast<double>(2.5e-6), &COGENDAbsim3_va::Instance::w0);
  p.addPar("WA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wa0);
  p.addPar("WA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wa1);
  p.addPar("WA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wa2);
  p.addPar("WACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wacde);
  p.addPar("WAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wagidl);
  p.addPar("WAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wagisl);
  p.addPar("WAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wags);
  p.addPar("WAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigbacc);
  p.addPar("WAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigbinv);
  p.addPar("WAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigc);
  p.addPar("WAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigd);
  p.addPar("WAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigs);
  p.addPar("WAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::waigsd);
  p.addPar("WALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::walpha0);
  p.addPar("WALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::walpha1);
  p.addPar("WAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wat);
  p.addPar("WB0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wb0);
  p.addPar("WB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wb1);
  p.addPar("WBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wbeta0);
  p.addPar("WBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbgidl);
  p.addPar("WBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbgisl);
  p.addPar("WBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigbacc);
  p.addPar("WBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigbinv);
  p.addPar("WBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigc);
  p.addPar("WBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigd);
  p.addPar("WBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigs);
  p.addPar("WBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wbigsd);
  p.addPar("WCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcdsc);
  p.addPar("WCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcdscb);
  p.addPar("WCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcdscd);
  p.addPar("WCF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcf);
  p.addPar("WCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcgdl);
  p.addPar("WCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcgidl);
  p.addPar("WCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcgisl);
  p.addPar("WCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcgsl);
  p.addPar("WCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigbacc);
  p.addPar("WCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigbinv);
  p.addPar("WCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigc);
  p.addPar("WCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigd);
  p.addPar("WCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigs);
  p.addPar("WCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wcigsd);
  p.addPar("WCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcit);
  p.addPar("WCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wckappa);
  p.addPar("WCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wclc);
  p.addPar("WCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wcle);
  p.addPar("WD",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wd);
  p.addPar("WDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdelta);
  p.addPar("WDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdrout);
  p.addPar("WDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdsub);
  p.addPar("WDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt0);
  p.addPar("WDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt0w);
  p.addPar("WDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt1);
  p.addPar("WDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt1w);
  p.addPar("WDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt2);
  p.addPar("WDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdvt2w);
  p.addPar("WDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdwb);
  p.addPar("WDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wdwg);
  p.addPar("WEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wegidl);
  p.addPar("WEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wegisl);
  p.addPar("WEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::weigbinv);
  p.addPar("WELM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::welm);
  p.addPar("WETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::weta0);
  p.addPar("WETAB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wetab);
  p.addPar("WFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wfgidl);
  p.addPar("WFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wfgisl);
  p.addPar("WGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wgamma1);
  p.addPar("WGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wgamma2);
  p.addPar("WINT",static_cast<double>(0), &COGENDAbsim3_va::Instance::wint);
  p.addPar("WK1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wk1);
  p.addPar("WK2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wk2);
  p.addPar("WK3",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wk3);
  p.addPar("WK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wk3b);
  p.addPar("WKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wketa);
  p.addPar("WKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wkgidl);
  p.addPar("WKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wkgisl);
  p.addPar("WKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wkt1);
  p.addPar("WKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wkt1l);
  p.addPar("WKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wkt2);
  p.addPar("WKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wku0);
  p.addPar("WKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wkvth0);
  p.addPar("WL",static_cast<double>(0), &COGENDAbsim3_va::Instance::wl);
  p.addPar("WLC",static_cast<double>(0), &COGENDAbsim3_va::Instance::wlc);
  p.addPar("WLN",static_cast<double>(1), &COGENDAbsim3_va::Instance::wln);
  p.addPar("WLOD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wlod);
  p.addPar("WLODKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wlodku0);
  p.addPar("WLODVTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wlodvth);
  p.addPar("WMAX",static_cast<double>(1), &COGENDAbsim3_va::Instance::wmax);
  p.addPar("WMIN",static_cast<double>(0), &COGENDAbsim3_va::Instance::wmin);
  p.addPar("WMLT",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::wmlt);
  p.addPar("WMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wmoin);
  p.addPar("WNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnch);
  p.addPar("WNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnfactor);
  p.addPar("WNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wngate);
  p.addPar("WNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wnigbacc);
  p.addPar("WNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wnigbinv);
  p.addPar("WNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wnigc);
  p.addPar("WNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnlx);
  p.addPar("WNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnoff);
  p.addPar("WNOI",static_cast<double>(1e-5), &COGENDAbsim3_va::Instance::wnoi);
  p.addPar("WNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnpeak);
  p.addPar("WNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wnsd);
  p.addPar("WNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wnsub);
  p.addPar("WNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wntox);
  p.addPar("WPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpclm);
  p.addPar("WPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpdiblc1);
  p.addPar("WPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpdiblc2);
  p.addPar("WPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpdiblcb);
  p.addPar("WPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wpigcd);
  p.addPar("WPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wpoxedge);
  p.addPar("WPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wprt);
  p.addPar("WPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wprwb);
  p.addPar("WPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wprwg);
  p.addPar("WPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpscbe1);
  p.addPar("WPSCBE2",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpscbe2);
  p.addPar("WPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wpvag);
  p.addPar("WR",static_cast<double>(1), &COGENDAbsim3_va::Instance::wr);
  p.addPar("WRD",static_cast<double>(0), &COGENDAbsim3_va::Instance::wrd);
  p.addPar("WRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wrdsw);
  p.addPar("WREF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wref);
  p.addPar("WRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wrgidl);
  p.addPar("WRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wrgisl);
  p.addPar("WRS",static_cast<double>(0), &COGENDAbsim3_va::Instance::wrs);
  p.addPar("WTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wtvfbsdoff);
  p.addPar("WU0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wu0);
  p.addPar("WUA",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wua);
  p.addPar("WUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wua1);
  p.addPar("WUB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wub);
  p.addPar("WUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wub1);
  p.addPar("WUC",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wuc);
  p.addPar("WUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wuc1);
  p.addPar("WUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wute);
  p.addPar("WVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvbm);
  p.addPar("WVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvbx);
  p.addPar("WVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvfb);
  p.addPar("WVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvfbcv);
  p.addPar("WVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::wvfbsdoff);
  p.addPar("WVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvoff);
  p.addPar("WVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvoffcv);
  p.addPar("WVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvsat);
  p.addPar("WVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wvth0);
  p.addPar("WW",static_cast<double>(0), &COGENDAbsim3_va::Instance::ww);
  p.addPar("WW0",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::ww0);
  p.addPar("WWC",static_cast<double>(0), &COGENDAbsim3_va::Instance::wwc);
  p.addPar("WWL",static_cast<double>(0), &COGENDAbsim3_va::Instance::wwl);
  p.addPar("WWLC",static_cast<double>(0), &COGENDAbsim3_va::Instance::wwlc);
  p.addPar("WWN",static_cast<double>(1), &COGENDAbsim3_va::Instance::wwn);
  p.addPar("WWR",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wwr);
  p.addPar("WXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wxj);
  p.addPar("WXT",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::wxt);
  p.addPar("XJ",static_cast<double>(0.15e-6), &COGENDAbsim3_va::Instance::xj);
  p.addPar("XJBVD",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::xjbvd);
  p.addPar("XJBVS",static_cast<double>(1.0), &COGENDAbsim3_va::Instance::xjbvs);
  p.addPar("XL",static_cast<double>(0), &COGENDAbsim3_va::Instance::xl);
  p.addPar("XLREF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::xlref);
  p.addPar("XPART",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::xpart);
  p.addPar("XT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Instance::xt);
  p.addPar("XTI",static_cast<double>(3), &COGENDAbsim3_va::Instance::xti);
  p.addPar("XW",static_cast<double>(0), &COGENDAbsim3_va::Instance::xw);
  p.addPar("XWREF",static_cast<double>(0.0), &COGENDAbsim3_va::Instance::xwref);
}

  //void Traits::loadModelParameters()
void Traits::loadModelParameters(ParametricData<COGENDAbsim3_va::Model> &p)
{
  p.addPar("XYCEADMSMODTEMP", 0.0, &COGENDAbsim3_va::Model::cogendaModTemp)
    .setExpressionAccess(NO_DOC)
    .setUnit(U_DEGK)
    .setCategory(CAT_TEMP)
    .setDescription("Internally set parameter for device model temperature");
  p.addPar("_CKT_GMIN",static_cast<double>(1e-12), &COGENDAbsim3_va::Model::_ckt_gmin);
  p.addPar("A0",static_cast<double>(1), &COGENDAbsim3_va::Model::a0);
  p.addPar("A1",static_cast<double>(0), &COGENDAbsim3_va::Model::a1);
  p.addPar("A2",static_cast<double>(1), &COGENDAbsim3_va::Model::a2);
  p.addPar("ACDE",static_cast<double>(1), &COGENDAbsim3_va::Model::acde);
  p.addPar("ACM",static_cast<int>(-9999999), &COGENDAbsim3_va::Model::acm);
  p.addPar("AD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ad);
  p.addPar("AF",static_cast<double>(1), &COGENDAbsim3_va::Model::af);
  p.addPar("AGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::agidl);
  p.addPar("AGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::agisl);
  p.addPar("AGS",static_cast<double>(0), &COGENDAbsim3_va::Model::ags);
  p.addPar("AIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigbacc);
  p.addPar("AIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigbinv);
  p.addPar("AIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigc);
  p.addPar("AIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigd);
  p.addPar("AIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigs);
  p.addPar("AIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::aigsd);
  p.addPar("ALPHA0",static_cast<double>(0), &COGENDAbsim3_va::Model::alpha0);
  p.addPar("ALPHA1",static_cast<double>(0), &COGENDAbsim3_va::Model::alpha1);
  p.addPar("AS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::as);
  p.addPar("AT",static_cast<double>(3.3e4), &COGENDAbsim3_va::Model::at);
  p.addPar("B0",static_cast<double>(0), &COGENDAbsim3_va::Model::b0);
  p.addPar("B1",static_cast<double>(0), &COGENDAbsim3_va::Model::b1);
  p.addPar("BETA0",static_cast<double>(30), &COGENDAbsim3_va::Model::beta0);
  p.addPar("BGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bgidl);
  p.addPar("BGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bgisl);
  p.addPar("BIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigbacc);
  p.addPar("BIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigbinv);
  p.addPar("BIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigc);
  p.addPar("BIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigd);
  p.addPar("BIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigs);
  p.addPar("BIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bigsd);
  p.addPar("BINFLAG",static_cast<int>(0), &COGENDAbsim3_va::Model::binflag);
  p.addPar("BINUNIT",static_cast<int>(1), &COGENDAbsim3_va::Model::binunit);
  p.addPar("BREAKMOD",static_cast<int>(0), &COGENDAbsim3_va::Model::breakmod);
  p.addPar("BVD",static_cast<double>(10.0), &COGENDAbsim3_va::Model::bvd);
  p.addPar("BVJ",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::bvj);
  p.addPar("BVS",static_cast<double>(10.0), &COGENDAbsim3_va::Model::bvs);
  p.addPar("CALCACM",static_cast<int>(-9999999), &COGENDAbsim3_va::Model::calcacm);
  p.addPar("CAPMOD",static_cast<int>(3), &COGENDAbsim3_va::Model::capmod);
  p.addPar("CBD",static_cast<double>(0), &COGENDAbsim3_va::Model::cbd);
  p.addPar("CBS",static_cast<double>(0), &COGENDAbsim3_va::Model::cbs);
  p.addPar("CDSC",static_cast<double>(2.4e-4), &COGENDAbsim3_va::Model::cdsc);
  p.addPar("CDSCB",static_cast<double>(0), &COGENDAbsim3_va::Model::cdscb);
  p.addPar("CDSCD",static_cast<double>(0), &COGENDAbsim3_va::Model::cdscd);
  p.addPar("CF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cf);
  p.addPar("CGBO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cgbo);
  p.addPar("CGDL",static_cast<double>(0), &COGENDAbsim3_va::Model::cgdl);
  p.addPar("CGDO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cgdo);
  p.addPar("CGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cgidl);
  p.addPar("CGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cgisl);
  p.addPar("CGSL",static_cast<double>(0), &COGENDAbsim3_va::Model::cgsl);
  p.addPar("CGSO",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cgso);
  p.addPar("CIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigbacc);
  p.addPar("CIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigbinv);
  p.addPar("CIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigc);
  p.addPar("CIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigd);
  p.addPar("CIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigs);
  p.addPar("CIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::cigsd);
  p.addPar("CIT",static_cast<double>(0), &COGENDAbsim3_va::Model::cit);
  p.addPar("CJ",static_cast<double>(5e-4), &COGENDAbsim3_va::Model::cj);
  p.addPar("CJGATE",static_cast<double>(5e-10), &COGENDAbsim3_va::Model::cjgate);
  p.addPar("CJSW",static_cast<double>(5e-10), &COGENDAbsim3_va::Model::cjsw);
  p.addPar("CJSWG",static_cast<double>(5e-10), &COGENDAbsim3_va::Model::cjswg);
  p.addPar("CKAPPA",static_cast<double>(0.6), &COGENDAbsim3_va::Model::ckappa);
  p.addPar("CLC",static_cast<double>(0.1e-6), &COGENDAbsim3_va::Model::clc);
  p.addPar("CLE",static_cast<double>(0.6), &COGENDAbsim3_va::Model::cle);
  p.addPar("CTA",static_cast<double>(0), &COGENDAbsim3_va::Model::cta);
  p.addPar("CTP",static_cast<double>(0), &COGENDAbsim3_va::Model::ctp);
  p.addPar("DEL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::del);
  p.addPar("DELK1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::delk1);
  p.addPar("DELNFCT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::delnfct);
  p.addPar("DELTA",static_cast<double>(0.01), &COGENDAbsim3_va::Model::delta);
  p.addPar("DELTOX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::deltox);
  p.addPar("DELVTO",static_cast<double>(0.0), &COGENDAbsim3_va::Model::delvto);
  p.addPar("DLC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::dlc);
  p.addPar("DLCIG",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::dlcig);
  p.addPar("DROUT",static_cast<double>(0.56), &COGENDAbsim3_va::Model::drout);
  p.addPar("DSUB",static_cast<double>(0.56), &COGENDAbsim3_va::Model::dsub);
  p.addPar("DTOXCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::dtoxcv);
  p.addPar("DVT0",static_cast<double>(2.2), &COGENDAbsim3_va::Model::dvt0);
  p.addPar("DVT0W",static_cast<double>(0), &COGENDAbsim3_va::Model::dvt0w);
  p.addPar("DVT1",static_cast<double>(0.53), &COGENDAbsim3_va::Model::dvt1);
  p.addPar("DVT1W",static_cast<double>(5.3e6), &COGENDAbsim3_va::Model::dvt1w);
  p.addPar("DVT2",static_cast<double>(-0.032), &COGENDAbsim3_va::Model::dvt2);
  p.addPar("DVT2W",static_cast<double>(-0.032), &COGENDAbsim3_va::Model::dvt2w);
  p.addPar("DWB",static_cast<double>(0), &COGENDAbsim3_va::Model::dwb);
  p.addPar("DWC",static_cast<double>(0), &COGENDAbsim3_va::Model::dwc);
  p.addPar("DWG",static_cast<double>(0), &COGENDAbsim3_va::Model::dwg);
  p.addPar("EF",static_cast<double>(1), &COGENDAbsim3_va::Model::ef);
  p.addPar("EG",static_cast<double>(1.12452), &COGENDAbsim3_va::Model::eg);
  p.addPar("EGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::egidl);
  p.addPar("EGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::egisl);
  p.addPar("EIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::eigbinv);
  p.addPar("ELM",static_cast<double>(5), &COGENDAbsim3_va::Model::elm);
  p.addPar("EM",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::em);
  p.addPar("ETA0",static_cast<double>(0.08), &COGENDAbsim3_va::Model::eta0);
  p.addPar("ETAB",static_cast<double>(-0.07), &COGENDAbsim3_va::Model::etab);
  p.addPar("FC",static_cast<double>(0.5), &COGENDAbsim3_va::Model::fc);
  p.addPar("FCSW",static_cast<double>(0.5), &COGENDAbsim3_va::Model::fcsw);
  p.addPar("FGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::fgidl);
  p.addPar("FGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::fgisl);
  p.addPar("FLKMOD",static_cast<double>(0), &COGENDAbsim3_va::Model::flkmod);
  p.addPar("GAMMA1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::gamma1);
  p.addPar("GAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::gamma2);
  p.addPar("GAP1",static_cast<double>(7.02e-4), &COGENDAbsim3_va::Model::gap1);
  p.addPar("GAP2",static_cast<double>(1108), &COGENDAbsim3_va::Model::gap2);
  p.addPar("GEO",static_cast<int>(0), &COGENDAbsim3_va::Model::geo);
  p.addPar("GIDLMOD",static_cast<int>(0), &COGENDAbsim3_va::Model::gidlmod);
  p.addPar("HDIF",static_cast<double>(0), &COGENDAbsim3_va::Model::hdif);
  p.addPar("IGBMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Model::igbmod);
  p.addPar("IGCMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Model::igcmod);
  p.addPar("IJTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ijth);
  p.addPar("IJTHDREV",static_cast<double>(0.1), &COGENDAbsim3_va::Model::ijthdrev);
  p.addPar("IJTHSREV",static_cast<double>(0.1), &COGENDAbsim3_va::Model::ijthsrev);
  p.addPar("IMAX",static_cast<double>(1), &COGENDAbsim3_va::Model::imax);
  p.addPar("IMELT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::imelt);
  p.addPar("IS",static_cast<double>(1e-14), &COGENDAbsim3_va::Model::is);
  p.addPar("JMAX",static_cast<double>(1e8), &COGENDAbsim3_va::Model::jmax);
  p.addPar("JMELT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::jmelt);
  p.addPar("JS",static_cast<double>(1e-4), &COGENDAbsim3_va::Model::js);
  p.addPar("JSSW",static_cast<double>(0.0), &COGENDAbsim3_va::Model::jssw);
  p.addPar("JSW",static_cast<double>(0), &COGENDAbsim3_va::Model::jsw);
  p.addPar("K1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::k1);
  p.addPar("K2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::k2);
  p.addPar("K3",static_cast<double>(80), &COGENDAbsim3_va::Model::k3);
  p.addPar("K3B",static_cast<double>(0), &COGENDAbsim3_va::Model::k3b);
  p.addPar("KETA",static_cast<double>(-0.047), &COGENDAbsim3_va::Model::keta);
  p.addPar("KF",static_cast<double>(0), &COGENDAbsim3_va::Model::kf);
  p.addPar("KGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::kgidl);
  p.addPar("KGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::kgisl);
  p.addPar("KT1",static_cast<double>(-0.11), &COGENDAbsim3_va::Model::kt1);
  p.addPar("KT1L",static_cast<double>(0), &COGENDAbsim3_va::Model::kt1l);
  p.addPar("KT2",static_cast<double>(0.022), &COGENDAbsim3_va::Model::kt2);
  p.addPar("KU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ku0);
  p.addPar("KVSAT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::kvsat);
  p.addPar("KVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::kvth0);
  p.addPar("L",static_cast<double>(5e-6), &COGENDAbsim3_va::Model::l);
  p.addPar("LA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::la0);
  p.addPar("LA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::la1);
  p.addPar("LA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::la2);
  p.addPar("LACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lacde);
  p.addPar("LAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lagidl);
  p.addPar("LAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lagisl);
  p.addPar("LAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lags);
  p.addPar("LAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigbacc);
  p.addPar("LAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigbinv);
  p.addPar("LAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigc);
  p.addPar("LAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigd);
  p.addPar("LAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigs);
  p.addPar("LAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::laigsd);
  p.addPar("LALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lalpha0);
  p.addPar("LALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lalpha1);
  p.addPar("LAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lat);
  p.addPar("LB0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lb0);
  p.addPar("LB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lb1);
  p.addPar("LBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lbeta0);
  p.addPar("LBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbgidl);
  p.addPar("LBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbgisl);
  p.addPar("LBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigbacc);
  p.addPar("LBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigbinv);
  p.addPar("LBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigc);
  p.addPar("LBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigd);
  p.addPar("LBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigs);
  p.addPar("LBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lbigsd);
  p.addPar("LCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcdsc);
  p.addPar("LCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcdscb);
  p.addPar("LCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcdscd);
  p.addPar("LCF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcf);
  p.addPar("LCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcgdl);
  p.addPar("LCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcgidl);
  p.addPar("LCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcgisl);
  p.addPar("LCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcgsl);
  p.addPar("LCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigbacc);
  p.addPar("LCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigbinv);
  p.addPar("LCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigc);
  p.addPar("LCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigd);
  p.addPar("LCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigs);
  p.addPar("LCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lcigsd);
  p.addPar("LCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcit);
  p.addPar("LCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lckappa);
  p.addPar("LCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lclc);
  p.addPar("LCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lcle);
  p.addPar("LD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ld);
  p.addPar("LDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldelta);
  p.addPar("LDIF",static_cast<double>(0), &COGENDAbsim3_va::Model::ldif);
  p.addPar("LDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldrout);
  p.addPar("LDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldsub);
  p.addPar("LDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt0);
  p.addPar("LDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt0w);
  p.addPar("LDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt1);
  p.addPar("LDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt1w);
  p.addPar("LDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt2);
  p.addPar("LDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldvt2w);
  p.addPar("LDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldwb);
  p.addPar("LDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ldwg);
  p.addPar("LEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::legidl);
  p.addPar("LEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::legisl);
  p.addPar("LEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::leigbinv);
  p.addPar("LELM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lelm);
  p.addPar("LETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::leta0);
  p.addPar("LETAB",static_cast<double>(-0.0), &COGENDAbsim3_va::Model::letab);
  p.addPar("LEVEL",static_cast<int>(49), &COGENDAbsim3_va::Model::level);
  p.addPar("LFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lfgidl);
  p.addPar("LFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lfgisl);
  p.addPar("LGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lgamma1);
  p.addPar("LGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lgamma2);
  p.addPar("LGCD",static_cast<double>(0), &COGENDAbsim3_va::Model::lgcd);
  p.addPar("LGCS",static_cast<double>(0), &COGENDAbsim3_va::Model::lgcs);
  p.addPar("LINT",static_cast<double>(0), &COGENDAbsim3_va::Model::lint);
  p.addPar("LK1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lk1);
  p.addPar("LK2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lk2);
  p.addPar("LK3",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lk3);
  p.addPar("LK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lk3b);
  p.addPar("LKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lketa);
  p.addPar("LKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lkgidl);
  p.addPar("LKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lkgisl);
  p.addPar("LKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lkt1);
  p.addPar("LKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lkt1l);
  p.addPar("LKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lkt2);
  p.addPar("LKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lku0);
  p.addPar("LKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lkvth0);
  p.addPar("LL",static_cast<double>(0), &COGENDAbsim3_va::Model::ll);
  p.addPar("LLC",static_cast<double>(0), &COGENDAbsim3_va::Model::llc);
  p.addPar("LLN",static_cast<double>(1), &COGENDAbsim3_va::Model::lln);
  p.addPar("LLODKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::llodku0);
  p.addPar("LLODVTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::llodvth);
  p.addPar("LMAX",static_cast<double>(1), &COGENDAbsim3_va::Model::lmax);
  p.addPar("LMIN",static_cast<double>(0), &COGENDAbsim3_va::Model::lmin);
  p.addPar("LMLT",static_cast<double>(1.0), &COGENDAbsim3_va::Model::lmlt);
  p.addPar("LMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lmoin);
  p.addPar("LNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnch);
  p.addPar("LNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnfactor);
  p.addPar("LNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lngate);
  p.addPar("LNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lnigbacc);
  p.addPar("LNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lnigbinv);
  p.addPar("LNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lnigc);
  p.addPar("LNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnlx);
  p.addPar("LNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnoff);
  p.addPar("LNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnpeak);
  p.addPar("LNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lnsd);
  p.addPar("LNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lnsub);
  p.addPar("LNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lntox);
  p.addPar("LODETA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lodeta0);
  p.addPar("LODK2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lodk2);
  p.addPar("LPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpclm);
  p.addPar("LPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpdiblc1);
  p.addPar("LPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpdiblc2);
  p.addPar("LPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpdiblcb);
  p.addPar("LPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lpigcd);
  p.addPar("LPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lpoxedge);
  p.addPar("LPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lprt);
  p.addPar("LPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lprwb);
  p.addPar("LPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lprwg);
  p.addPar("LPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpscbe1);
  p.addPar("LPSCBE2",static_cast<double>(0), &COGENDAbsim3_va::Model::lpscbe2);
  p.addPar("LPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lpvag);
  p.addPar("LRD",static_cast<double>(0), &COGENDAbsim3_va::Model::lrd);
  p.addPar("LRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lrdsw);
  p.addPar("LREF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lref);
  p.addPar("LRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lrgidl);
  p.addPar("LRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lrgisl);
  p.addPar("LRS",static_cast<double>(0), &COGENDAbsim3_va::Model::lrs);
  p.addPar("LTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ltvfbsdoff);
  p.addPar("LU0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lu0);
  p.addPar("LUA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lua);
  p.addPar("LUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lua1);
  p.addPar("LUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lub);
  p.addPar("LUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lub1);
  p.addPar("LUC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::luc);
  p.addPar("LUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::luc1);
  p.addPar("LUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lute);
  p.addPar("LVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvbm);
  p.addPar("LVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvbx);
  p.addPar("LVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvfb);
  p.addPar("LVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvfbcv);
  p.addPar("LVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::lvfbsdoff);
  p.addPar("LVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvoff);
  p.addPar("LVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvoffcv);
  p.addPar("LVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvsat);
  p.addPar("LVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lvth0);
  p.addPar("LW",static_cast<double>(0), &COGENDAbsim3_va::Model::lw);
  p.addPar("LW0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lw0);
  p.addPar("LWC",static_cast<double>(0), &COGENDAbsim3_va::Model::lwc);
  p.addPar("LWL",static_cast<double>(0), &COGENDAbsim3_va::Model::lwl);
  p.addPar("LWLC",static_cast<double>(0), &COGENDAbsim3_va::Model::lwlc);
  p.addPar("LWN",static_cast<double>(1), &COGENDAbsim3_va::Model::lwn);
  p.addPar("LWR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lwr);
  p.addPar("LXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lxj);
  p.addPar("LXT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::lxt);
  p.addPar("METO",static_cast<double>(0), &COGENDAbsim3_va::Model::meto);
  p.addPar("MINR",static_cast<double>(0.1), &COGENDAbsim3_va::Model::minr);
  p.addPar("MJ",static_cast<double>(0.5), &COGENDAbsim3_va::Model::mj);
  p.addPar("MJSW",static_cast<double>(0.33), &COGENDAbsim3_va::Model::mjsw);
  p.addPar("MJSWG",static_cast<double>(0.33), &COGENDAbsim3_va::Model::mjswg);
  p.addPar("MOBMOD",static_cast<int>(1), &COGENDAbsim3_va::Model::mobmod);
  p.addPar("MOIN",static_cast<double>(15), &COGENDAbsim3_va::Model::moin);
  p.addPar("MULU0",static_cast<double>(1.0), &COGENDAbsim3_va::Model::mulu0);
  p.addPar("MULUA",static_cast<double>(1.0), &COGENDAbsim3_va::Model::mulua);
  p.addPar("MULUB",static_cast<double>(1.0), &COGENDAbsim3_va::Model::mulub);
  p.addPar("NCH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nch);
  p.addPar("NF",static_cast<double>(1.0), &COGENDAbsim3_va::Model::nf);
  p.addPar("NFACTOR",static_cast<double>(1), &COGENDAbsim3_va::Model::nfactor);
  p.addPar("NGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ngate);
  p.addPar("NIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nigbacc);
  p.addPar("NIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nigbinv);
  p.addPar("NIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nigc);
  p.addPar("NJ",static_cast<double>(1), &COGENDAbsim3_va::Model::nj);
  p.addPar("NLX",static_cast<double>(1.74e-7), &COGENDAbsim3_va::Model::nlx);
  p.addPar("NOFF",static_cast<double>(1), &COGENDAbsim3_va::Model::noff);
  p.addPar("NOIA",static_cast<double>(1e20), &COGENDAbsim3_va::Model::noia);
  p.addPar("NOIB",static_cast<double>(5e4), &COGENDAbsim3_va::Model::noib);
  p.addPar("NOIC",static_cast<double>(-1.4e-12), &COGENDAbsim3_va::Model::noic);
  p.addPar("NOIMOD",static_cast<int>(1), &COGENDAbsim3_va::Model::noimod);
  p.addPar("NQSMOD",static_cast<int>(0), &COGENDAbsim3_va::Model::nqsmod);
  p.addPar("NRD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nrd);
  p.addPar("NRS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nrs);
  p.addPar("NSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nsd);
  p.addPar("NSUB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::nsub);
  p.addPar("NTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ntox);
  p.addPar("PA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pa0);
  p.addPar("PA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pa1);
  p.addPar("PA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pa2);
  p.addPar("PACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pacde);
  p.addPar("PAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pagidl);
  p.addPar("PAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pagisl);
  p.addPar("PAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pags);
  p.addPar("PAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigbacc);
  p.addPar("PAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigbinv);
  p.addPar("PAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigc);
  p.addPar("PAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigd);
  p.addPar("PAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigs);
  p.addPar("PAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::paigsd);
  p.addPar("PALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::palpha0);
  p.addPar("PALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::palpha1);
  p.addPar("PARAMCHK",static_cast<int>(0), &COGENDAbsim3_va::Model::paramchk);
  p.addPar("PAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pat);
  p.addPar("PB",static_cast<double>(1), &COGENDAbsim3_va::Model::pb);
  p.addPar("PB0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pb0);
  p.addPar("PB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pb1);
  p.addPar("PBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pbeta0);
  p.addPar("PBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbgidl);
  p.addPar("PBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbgisl);
  p.addPar("PBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigbacc);
  p.addPar("PBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigbinv);
  p.addPar("PBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigc);
  p.addPar("PBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigd);
  p.addPar("PBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigs);
  p.addPar("PBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pbigsd);
  p.addPar("PBSW",static_cast<double>(1), &COGENDAbsim3_va::Model::pbsw);
  p.addPar("PBSWG",static_cast<double>(1), &COGENDAbsim3_va::Model::pbswg);
  p.addPar("PCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcdsc);
  p.addPar("PCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcdscb);
  p.addPar("PCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcdscd);
  p.addPar("PCF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcf);
  p.addPar("PCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcgdl);
  p.addPar("PCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcgidl);
  p.addPar("PCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcgisl);
  p.addPar("PCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcgsl);
  p.addPar("PCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigbacc);
  p.addPar("PCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigbinv);
  p.addPar("PCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigc);
  p.addPar("PCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigd);
  p.addPar("PCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigs);
  p.addPar("PCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pcigsd);
  p.addPar("PCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcit);
  p.addPar("PCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pckappa);
  p.addPar("PCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pclc);
  p.addPar("PCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pcle);
  p.addPar("PCLM",static_cast<double>(1.3), &COGENDAbsim3_va::Model::pclm);
  p.addPar("PD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pd);
  p.addPar("PDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdelta);
  p.addPar("PDIBLC1",static_cast<double>(0.39), &COGENDAbsim3_va::Model::pdiblc1);
  p.addPar("PDIBLC2",static_cast<double>(0.0086), &COGENDAbsim3_va::Model::pdiblc2);
  p.addPar("PDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdiblcb);
  p.addPar("PDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdrout);
  p.addPar("PDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdsub);
  p.addPar("PDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt0);
  p.addPar("PDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt0w);
  p.addPar("PDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt1);
  p.addPar("PDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt1w);
  p.addPar("PDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt2);
  p.addPar("PDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdvt2w);
  p.addPar("PDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdwb);
  p.addPar("PDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pdwg);
  p.addPar("PEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pegidl);
  p.addPar("PEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pegisl);
  p.addPar("PEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::peigbinv);
  p.addPar("PELM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pelm);
  p.addPar("PETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::peta0);
  p.addPar("PETAB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::petab);
  p.addPar("PFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pfgidl);
  p.addPar("PFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pfgisl);
  p.addPar("PGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pgamma1);
  p.addPar("PGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pgamma2);
  p.addPar("PHP",static_cast<double>(1), &COGENDAbsim3_va::Model::php);
  p.addPar("PIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pigcd);
  p.addPar("PK1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pk1);
  p.addPar("PK2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pk2);
  p.addPar("PK3",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pk3);
  p.addPar("PK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pk3b);
  p.addPar("PKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pketa);
  p.addPar("PKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pkgidl);
  p.addPar("PKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pkgisl);
  p.addPar("PKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pkt1);
  p.addPar("PKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pkt1l);
  p.addPar("PKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pkt2);
  p.addPar("PKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pku0);
  p.addPar("PKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pkvth0);
  p.addPar("PMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pmoin);
  p.addPar("PNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnch);
  p.addPar("PNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnfactor);
  p.addPar("PNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pngate);
  p.addPar("PNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pnigbacc);
  p.addPar("PNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pnigbinv);
  p.addPar("PNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pnigc);
  p.addPar("PNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnlx);
  p.addPar("PNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnoff);
  p.addPar("PNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnpeak);
  p.addPar("PNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pnsd);
  p.addPar("PNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pnsub);
  p.addPar("PNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pntox);
  p.addPar("POXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::poxedge);
  p.addPar("PPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppclm);
  p.addPar("PPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppdiblc1);
  p.addPar("PPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppdiblc2);
  p.addPar("PPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppdiblcb);
  p.addPar("PPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ppigcd);
  p.addPar("PPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ppoxedge);
  p.addPar("PPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pprt);
  p.addPar("PPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pprwb);
  p.addPar("PPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pprwg);
  p.addPar("PPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppscbe1);
  p.addPar("PPSCBE2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppscbe2);
  p.addPar("PPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ppvag);
  p.addPar("PRD",static_cast<double>(0), &COGENDAbsim3_va::Model::prd);
  p.addPar("PRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Model::prdsw);
  p.addPar("PRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::prgidl);
  p.addPar("PRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::prgisl);
  p.addPar("PRS",static_cast<double>(0), &COGENDAbsim3_va::Model::prs);
  p.addPar("PRT",static_cast<double>(0), &COGENDAbsim3_va::Model::prt);
  p.addPar("PRWB",static_cast<double>(0), &COGENDAbsim3_va::Model::prwb);
  p.addPar("PRWG",static_cast<double>(0), &COGENDAbsim3_va::Model::prwg);
  p.addPar("PS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ps);
  p.addPar("PSCBE1",static_cast<double>(4.24e8), &COGENDAbsim3_va::Model::pscbe1);
  p.addPar("PSCBE2",static_cast<double>(1e-5), &COGENDAbsim3_va::Model::pscbe2);
  p.addPar("PTA",static_cast<double>(0), &COGENDAbsim3_va::Model::pta);
  p.addPar("PTP",static_cast<double>(0), &COGENDAbsim3_va::Model::ptp);
  p.addPar("PTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::ptvfbsdoff);
  p.addPar("PU0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pu0);
  p.addPar("PUA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pua);
  p.addPar("PUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pua1);
  p.addPar("PUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pub);
  p.addPar("PUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pub1);
  p.addPar("PUC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::puc);
  p.addPar("PUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::puc1);
  p.addPar("PUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pute);
  p.addPar("PVAG",static_cast<double>(0), &COGENDAbsim3_va::Model::pvag);
  p.addPar("PVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvbm);
  p.addPar("PVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvbx);
  p.addPar("PVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvfb);
  p.addPar("PVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvfbcv);
  p.addPar("PVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::pvfbsdoff);
  p.addPar("PVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvoff);
  p.addPar("PVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvoffcv);
  p.addPar("PVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvsat);
  p.addPar("PVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pvth0);
  p.addPar("PW0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pw0);
  p.addPar("PWR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pwr);
  p.addPar("PXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pxj);
  p.addPar("PXT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::pxt);
  p.addPar("RD",static_cast<double>(0), &COGENDAbsim3_va::Model::rd);
  p.addPar("RDC",static_cast<double>(0), &COGENDAbsim3_va::Model::rdc);
  p.addPar("RDSW",static_cast<double>(0), &COGENDAbsim3_va::Model::rdsw);
  p.addPar("RGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::rgidl);
  p.addPar("RGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::rgisl);
  p.addPar("RS",static_cast<double>(0), &COGENDAbsim3_va::Model::rs);
  p.addPar("RSC",static_cast<double>(0), &COGENDAbsim3_va::Model::rsc);
  p.addPar("RSH",static_cast<double>(0), &COGENDAbsim3_va::Model::rsh);
  p.addPar("SA",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa);
  p.addPar("SA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa0);
  p.addPar("SA1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa1);
  p.addPar("SA10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa10);
  p.addPar("SA2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa2);
  p.addPar("SA3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa3);
  p.addPar("SA4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa4);
  p.addPar("SA5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa5);
  p.addPar("SA6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa6);
  p.addPar("SA7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa7);
  p.addPar("SA8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa8);
  p.addPar("SA9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sa9);
  p.addPar("SAREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::saref);
  p.addPar("SB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb);
  p.addPar("SB0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb0);
  p.addPar("SB1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb1);
  p.addPar("SB10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb10);
  p.addPar("SB2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb2);
  p.addPar("SB3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb3);
  p.addPar("SB4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb4);
  p.addPar("SB5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb5);
  p.addPar("SB6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb6);
  p.addPar("SB7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb7);
  p.addPar("SB8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb8);
  p.addPar("SB9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sb9);
  p.addPar("SBREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sbref);
  p.addPar("SC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sc);
  p.addPar("SD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::sd);
  p.addPar("STETA0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::steta0);
  p.addPar("STIMOD",static_cast<int>(0), &COGENDAbsim3_va::Model::stimod);
  p.addPar("STK2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::stk2);
  p.addPar("SW1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw1);
  p.addPar("SW10",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw10);
  p.addPar("SW2",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw2);
  p.addPar("SW3",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw3);
  p.addPar("SW4",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw4);
  p.addPar("SW5",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw5);
  p.addPar("SW6",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw6);
  p.addPar("SW7",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw7);
  p.addPar("SW8",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw8);
  p.addPar("SW9",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::sw9);
  p.addPar("TCJ",static_cast<double>(0), &COGENDAbsim3_va::Model::tcj);
  p.addPar("TCJSW",static_cast<double>(0), &COGENDAbsim3_va::Model::tcjsw);
  p.addPar("TCJSWG",static_cast<double>(0), &COGENDAbsim3_va::Model::tcjswg);
  p.addPar("TEMPMOD",static_cast<int>(0.0), &COGENDAbsim3_va::Model::tempmod);
  p.addPar("TKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::tku0);
  p.addPar("TLEV",static_cast<double>(0), &COGENDAbsim3_va::Model::tlev);
  p.addPar("TLEVC",static_cast<double>(0), &COGENDAbsim3_va::Model::tlevc);
  p.addPar("TNOM",static_cast<double>(25), &COGENDAbsim3_va::Model::tnom);
  p.addPar("TOX",static_cast<double>(150.0e-10), &COGENDAbsim3_va::Model::tox);
  p.addPar("TOXE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::toxe);
  p.addPar("TOXM",static_cast<double>(150.0e-10), &COGENDAbsim3_va::Model::toxm);
  p.addPar("TOXREF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::toxref);
  p.addPar("TPB",static_cast<double>(0), &COGENDAbsim3_va::Model::tpb);
  p.addPar("TPBSW",static_cast<double>(0), &COGENDAbsim3_va::Model::tpbsw);
  p.addPar("TPBSWG",static_cast<double>(0), &COGENDAbsim3_va::Model::tpbswg);
  p.addPar("TRD",static_cast<double>(0), &COGENDAbsim3_va::Model::trd);
  p.addPar("TRISE",static_cast<double>(0), &COGENDAbsim3_va::Model::trise);
  p.addPar("TRS",static_cast<double>(0), &COGENDAbsim3_va::Model::trs);
  p.addPar("TVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::tvfbsdoff);
  p.addPar("TYPE",static_cast<int>(1), &COGENDAbsim3_va::Model::type);
  p.addPar("U0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::u0);
  p.addPar("UA",static_cast<double>(2.25e-9), &COGENDAbsim3_va::Model::ua);
  p.addPar("UA1",static_cast<double>(4.31e-9), &COGENDAbsim3_va::Model::ua1);
  p.addPar("UB",static_cast<double>(5.87e-19), &COGENDAbsim3_va::Model::ub);
  p.addPar("UB1",static_cast<double>(-7.61e-18), &COGENDAbsim3_va::Model::ub1);
  p.addPar("UC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::uc);
  p.addPar("UC1",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::uc1);
  p.addPar("UTE",static_cast<double>(-1.5), &COGENDAbsim3_va::Model::ute);
  p.addPar("VBM",static_cast<double>(-3), &COGENDAbsim3_va::Model::vbm);
  p.addPar("VBOX",static_cast<double>(1e9), &COGENDAbsim3_va::Model::vbox);
  p.addPar("VBX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::vbx);
  p.addPar("VERBOSE",static_cast<int>(-9999999), &COGENDAbsim3_va::Model::verbose);
  p.addPar("VERSION",static_cast<double>(3.3), &COGENDAbsim3_va::Model::version);
  p.addPar("VFB",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::vfb);
  p.addPar("VFBCV",static_cast<double>(-1), &COGENDAbsim3_va::Model::vfbcv);
  p.addPar("VFBFLAG",static_cast<int>(0), &COGENDAbsim3_va::Model::vfbflag);
  p.addPar("VFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::vfbsdoff);
  p.addPar("VGSLIM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::vgslim);
  p.addPar("VOFF",static_cast<double>(-0.08), &COGENDAbsim3_va::Model::voff);
  p.addPar("VOFFCV",static_cast<double>(0), &COGENDAbsim3_va::Model::voffcv);
  p.addPar("VSAT",static_cast<double>(8e4), &COGENDAbsim3_va::Model::vsat);
  p.addPar("VTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::vth0);
  p.addPar("VTM0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::vtm0);
  p.addPar("W",static_cast<double>(5e-6), &COGENDAbsim3_va::Model::w);
  p.addPar("W0",static_cast<double>(2.5e-6), &COGENDAbsim3_va::Model::w0);
  p.addPar("WA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wa0);
  p.addPar("WA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wa1);
  p.addPar("WA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wa2);
  p.addPar("WACDE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wacde);
  p.addPar("WAGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wagidl);
  p.addPar("WAGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wagisl);
  p.addPar("WAGS",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wags);
  p.addPar("WAIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigbacc);
  p.addPar("WAIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigbinv);
  p.addPar("WAIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigc);
  p.addPar("WAIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigd);
  p.addPar("WAIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigs);
  p.addPar("WAIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::waigsd);
  p.addPar("WALPHA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::walpha0);
  p.addPar("WALPHA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::walpha1);
  p.addPar("WAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wat);
  p.addPar("WB0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wb0);
  p.addPar("WB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wb1);
  p.addPar("WBETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wbeta0);
  p.addPar("WBGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbgidl);
  p.addPar("WBGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbgisl);
  p.addPar("WBIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigbacc);
  p.addPar("WBIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigbinv);
  p.addPar("WBIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigc);
  p.addPar("WBIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigd);
  p.addPar("WBIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigs);
  p.addPar("WBIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wbigsd);
  p.addPar("WCDSC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcdsc);
  p.addPar("WCDSCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcdscb);
  p.addPar("WCDSCD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcdscd);
  p.addPar("WCF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcf);
  p.addPar("WCGDL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcgdl);
  p.addPar("WCGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcgidl);
  p.addPar("WCGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcgisl);
  p.addPar("WCGSL",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcgsl);
  p.addPar("WCIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigbacc);
  p.addPar("WCIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigbinv);
  p.addPar("WCIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigc);
  p.addPar("WCIGD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigd);
  p.addPar("WCIGS",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigs);
  p.addPar("WCIGSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wcigsd);
  p.addPar("WCIT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcit);
  p.addPar("WCKAPPA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wckappa);
  p.addPar("WCLC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wclc);
  p.addPar("WCLE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wcle);
  p.addPar("WD",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wd);
  p.addPar("WDELTA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdelta);
  p.addPar("WDROUT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdrout);
  p.addPar("WDSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdsub);
  p.addPar("WDVT0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt0);
  p.addPar("WDVT0W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt0w);
  p.addPar("WDVT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt1);
  p.addPar("WDVT1W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt1w);
  p.addPar("WDVT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt2);
  p.addPar("WDVT2W",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdvt2w);
  p.addPar("WDWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdwb);
  p.addPar("WDWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wdwg);
  p.addPar("WEGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wegidl);
  p.addPar("WEGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wegisl);
  p.addPar("WEIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::weigbinv);
  p.addPar("WELM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::welm);
  p.addPar("WETA0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::weta0);
  p.addPar("WETAB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wetab);
  p.addPar("WFGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wfgidl);
  p.addPar("WFGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wfgisl);
  p.addPar("WGAMMA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wgamma1);
  p.addPar("WGAMMA2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wgamma2);
  p.addPar("WINT",static_cast<double>(0), &COGENDAbsim3_va::Model::wint);
  p.addPar("WK1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wk1);
  p.addPar("WK2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wk2);
  p.addPar("WK3",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wk3);
  p.addPar("WK3B",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wk3b);
  p.addPar("WKETA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wketa);
  p.addPar("WKGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wkgidl);
  p.addPar("WKGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wkgisl);
  p.addPar("WKT1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wkt1);
  p.addPar("WKT1L",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wkt1l);
  p.addPar("WKT2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wkt2);
  p.addPar("WKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wku0);
  p.addPar("WKVTH0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wkvth0);
  p.addPar("WL",static_cast<double>(0), &COGENDAbsim3_va::Model::wl);
  p.addPar("WLC",static_cast<double>(0), &COGENDAbsim3_va::Model::wlc);
  p.addPar("WLN",static_cast<double>(1), &COGENDAbsim3_va::Model::wln);
  p.addPar("WLOD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wlod);
  p.addPar("WLODKU0",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wlodku0);
  p.addPar("WLODVTH",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wlodvth);
  p.addPar("WMAX",static_cast<double>(1), &COGENDAbsim3_va::Model::wmax);
  p.addPar("WMIN",static_cast<double>(0), &COGENDAbsim3_va::Model::wmin);
  p.addPar("WMLT",static_cast<double>(1.0), &COGENDAbsim3_va::Model::wmlt);
  p.addPar("WMOIN",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wmoin);
  p.addPar("WNCH",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnch);
  p.addPar("WNFACTOR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnfactor);
  p.addPar("WNGATE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wngate);
  p.addPar("WNIGBACC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wnigbacc);
  p.addPar("WNIGBINV",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wnigbinv);
  p.addPar("WNIGC",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wnigc);
  p.addPar("WNLX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnlx);
  p.addPar("WNOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnoff);
  p.addPar("WNOI",static_cast<double>(1e-5), &COGENDAbsim3_va::Model::wnoi);
  p.addPar("WNPEAK",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnpeak);
  p.addPar("WNSD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wnsd);
  p.addPar("WNSUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wnsub);
  p.addPar("WNTOX",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wntox);
  p.addPar("WPCLM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpclm);
  p.addPar("WPDIBLC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpdiblc1);
  p.addPar("WPDIBLC2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpdiblc2);
  p.addPar("WPDIBLCB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpdiblcb);
  p.addPar("WPIGCD",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wpigcd);
  p.addPar("WPOXEDGE",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wpoxedge);
  p.addPar("WPRT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wprt);
  p.addPar("WPRWB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wprwb);
  p.addPar("WPRWG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wprwg);
  p.addPar("WPSCBE1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpscbe1);
  p.addPar("WPSCBE2",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpscbe2);
  p.addPar("WPVAG",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wpvag);
  p.addPar("WR",static_cast<double>(1), &COGENDAbsim3_va::Model::wr);
  p.addPar("WRD",static_cast<double>(0), &COGENDAbsim3_va::Model::wrd);
  p.addPar("WRDSW",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wrdsw);
  p.addPar("WREF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wref);
  p.addPar("WRGIDL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wrgidl);
  p.addPar("WRGISL",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wrgisl);
  p.addPar("WRS",static_cast<double>(0), &COGENDAbsim3_va::Model::wrs);
  p.addPar("WTVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wtvfbsdoff);
  p.addPar("WU0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wu0);
  p.addPar("WUA",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wua);
  p.addPar("WUA1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wua1);
  p.addPar("WUB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wub);
  p.addPar("WUB1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wub1);
  p.addPar("WUC",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wuc);
  p.addPar("WUC1",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wuc1);
  p.addPar("WUTE",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wute);
  p.addPar("WVBM",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvbm);
  p.addPar("WVBX",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvbx);
  p.addPar("WVFB",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvfb);
  p.addPar("WVFBCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvfbcv);
  p.addPar("WVFBSDOFF",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::wvfbsdoff);
  p.addPar("WVOFF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvoff);
  p.addPar("WVOFFCV",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvoffcv);
  p.addPar("WVSAT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvsat);
  p.addPar("WVTH0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wvth0);
  p.addPar("WW",static_cast<double>(0), &COGENDAbsim3_va::Model::ww);
  p.addPar("WW0",static_cast<double>(0.0), &COGENDAbsim3_va::Model::ww0);
  p.addPar("WWC",static_cast<double>(0), &COGENDAbsim3_va::Model::wwc);
  p.addPar("WWL",static_cast<double>(0), &COGENDAbsim3_va::Model::wwl);
  p.addPar("WWLC",static_cast<double>(0), &COGENDAbsim3_va::Model::wwlc);
  p.addPar("WWN",static_cast<double>(1), &COGENDAbsim3_va::Model::wwn);
  p.addPar("WWR",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wwr);
  p.addPar("WXJ",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wxj);
  p.addPar("WXT",static_cast<double>(0.0), &COGENDAbsim3_va::Model::wxt);
  p.addPar("XJ",static_cast<double>(0.15e-6), &COGENDAbsim3_va::Model::xj);
  p.addPar("XJBVD",static_cast<double>(1.0), &COGENDAbsim3_va::Model::xjbvd);
  p.addPar("XJBVS",static_cast<double>(1.0), &COGENDAbsim3_va::Model::xjbvs);
  p.addPar("XL",static_cast<double>(0), &COGENDAbsim3_va::Model::xl);
  p.addPar("XLREF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::xlref);
  p.addPar("XPART",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::xpart);
  p.addPar("XT",static_cast<double>(-9.9999e-99), &COGENDAbsim3_va::Model::xt);
  p.addPar("XTI",static_cast<double>(3), &COGENDAbsim3_va::Model::xti);
  p.addPar("XW",static_cast<double>(0), &COGENDAbsim3_va::Model::xw);
  p.addPar("XWREF",static_cast<double>(0.0), &COGENDAbsim3_va::Model::xwref);
}

/* class Instance member functions */
bool Instance::processParams () {
  if(!given("_CKT_GMIN"))
    _ckt_gmin = model_._ckt_gmin;
  if(!given("A0"))
    a0 = model_.a0;
  if(!given("A1"))
    a1 = model_.a1;
  if(!given("A2"))
    a2 = model_.a2;
  if(!given("ACDE"))
    acde = model_.acde;
  if(!given("ACM"))
    acm = model_.acm;
  if(!given("AD"))
    ad = model_.ad;
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
  if(!given("BETA0"))
    beta0 = model_.beta0;
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
  if(!given("BINFLAG"))
    binflag = model_.binflag;
  if(!given("BINUNIT"))
    binunit = model_.binunit;
  if(!given("BREAKMOD"))
    breakmod = model_.breakmod;
  if(!given("BVD"))
    bvd = model_.bvd;
  if(!given("BVJ"))
    bvj = model_.bvj;
  if(!given("BVS"))
    bvs = model_.bvs;
  if(!given("CALCACM"))
    calcacm = model_.calcacm;
  if(!given("CAPMOD"))
    capmod = model_.capmod;
  if(!given("CBD"))
    cbd = model_.cbd;
  if(!given("CBS"))
    cbs = model_.cbs;
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
  if(!given("CJ"))
    cj = model_.cj;
  if(!given("CJGATE"))
    cjgate = model_.cjgate;
  if(!given("CJSW"))
    cjsw = model_.cjsw;
  if(!given("CJSWG"))
    cjswg = model_.cjswg;
  if(!given("CKAPPA"))
    ckappa = model_.ckappa;
  if(!given("CLC"))
    clc = model_.clc;
  if(!given("CLE"))
    cle = model_.cle;
  if(!given("CTA"))
    cta = model_.cta;
  if(!given("CTP"))
    ctp = model_.ctp;
  if(!given("DEL"))
    del = model_.del;
  if(!given("DELK1"))
    delk1 = model_.delk1;
  if(!given("DELNFCT"))
    delnfct = model_.delnfct;
  if(!given("DELTA"))
    delta = model_.delta;
  if(!given("DELTOX"))
    deltox = model_.deltox;
  if(!given("DELVTO"))
    delvto = model_.delvto;
  if(!given("DLC"))
    dlc = model_.dlc;
  if(!given("DLCIG"))
    dlcig = model_.dlcig;
  if(!given("DROUT"))
    drout = model_.drout;
  if(!given("DSUB"))
    dsub = model_.dsub;
  if(!given("DTOXCV"))
    dtoxcv = model_.dtoxcv;
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
  if(!given("DWB"))
    dwb = model_.dwb;
  if(!given("DWC"))
    dwc = model_.dwc;
  if(!given("DWG"))
    dwg = model_.dwg;
  if(!given("EF"))
    ef = model_.ef;
  if(!given("EG"))
    eg = model_.eg;
  if(!given("EGIDL"))
    egidl = model_.egidl;
  if(!given("EGISL"))
    egisl = model_.egisl;
  if(!given("EIGBINV"))
    eigbinv = model_.eigbinv;
  if(!given("ELM"))
    elm = model_.elm;
  if(!given("EM"))
    em = model_.em;
  if(!given("ETA0"))
    eta0 = model_.eta0;
  if(!given("ETAB"))
    etab = model_.etab;
  if(!given("FC"))
    fc = model_.fc;
  if(!given("FCSW"))
    fcsw = model_.fcsw;
  if(!given("FGIDL"))
    fgidl = model_.fgidl;
  if(!given("FGISL"))
    fgisl = model_.fgisl;
  if(!given("FLKMOD"))
    flkmod = model_.flkmod;
  if(!given("GAMMA1"))
    gamma1 = model_.gamma1;
  if(!given("GAMMA2"))
    gamma2 = model_.gamma2;
  if(!given("GAP1"))
    gap1 = model_.gap1;
  if(!given("GAP2"))
    gap2 = model_.gap2;
  if(!given("GEO"))
    geo = model_.geo;
  if(!given("GIDLMOD"))
    gidlmod = model_.gidlmod;
  if(!given("HDIF"))
    hdif = model_.hdif;
  if(!given("IGBMOD"))
    igbmod = model_.igbmod;
  if(!given("IGCMOD"))
    igcmod = model_.igcmod;
  if(!given("IJTH"))
    ijth = model_.ijth;
  if(!given("IJTHDREV"))
    ijthdrev = model_.ijthdrev;
  if(!given("IJTHSREV"))
    ijthsrev = model_.ijthsrev;
  if(!given("IMAX"))
    imax = model_.imax;
  if(!given("IMELT"))
    imelt = model_.imelt;
  if(!given("IS"))
    is = model_.is;
  if(!given("JMAX"))
    jmax = model_.jmax;
  if(!given("JMELT"))
    jmelt = model_.jmelt;
  if(!given("JS"))
    js = model_.js;
  if(!given("JSSW"))
    jssw = model_.jssw;
  if(!given("JSW"))
    jsw = model_.jsw;
  if(!given("K1"))
    k1 = model_.k1;
  if(!given("K2"))
    k2 = model_.k2;
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
  if(!given("KVSAT"))
    kvsat = model_.kvsat;
  if(!given("KVTH0"))
    kvth0 = model_.kvth0;
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
  if(!given("LCKAPPA"))
    lckappa = model_.lckappa;
  if(!given("LCLC"))
    lclc = model_.lclc;
  if(!given("LCLE"))
    lcle = model_.lcle;
  if(!given("LD"))
    ld = model_.ld;
  if(!given("LDELTA"))
    ldelta = model_.ldelta;
  if(!given("LDIF"))
    ldif = model_.ldif;
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
  if(!given("LDWB"))
    ldwb = model_.ldwb;
  if(!given("LDWG"))
    ldwg = model_.ldwg;
  if(!given("LEGIDL"))
    legidl = model_.legidl;
  if(!given("LEGISL"))
    legisl = model_.legisl;
  if(!given("LEIGBINV"))
    leigbinv = model_.leigbinv;
  if(!given("LELM"))
    lelm = model_.lelm;
  if(!given("LETA0"))
    leta0 = model_.leta0;
  if(!given("LETAB"))
    letab = model_.letab;
  if(!given("LEVEL"))
    level = model_.level;
  if(!given("LFGIDL"))
    lfgidl = model_.lfgidl;
  if(!given("LFGISL"))
    lfgisl = model_.lfgisl;
  if(!given("LGAMMA1"))
    lgamma1 = model_.lgamma1;
  if(!given("LGAMMA2"))
    lgamma2 = model_.lgamma2;
  if(!given("LGCD"))
    lgcd = model_.lgcd;
  if(!given("LGCS"))
    lgcs = model_.lgcs;
  if(!given("LINT"))
    lint = model_.lint;
  if(!given("LK1"))
    lk1 = model_.lk1;
  if(!given("LK2"))
    lk2 = model_.lk2;
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
  if(!given("LKVTH0"))
    lkvth0 = model_.lkvth0;
  if(!given("LL"))
    ll = model_.ll;
  if(!given("LLC"))
    llc = model_.llc;
  if(!given("LLN"))
    lln = model_.lln;
  if(!given("LLODKU0"))
    llodku0 = model_.llodku0;
  if(!given("LLODVTH"))
    llodvth = model_.llodvth;
  if(!given("LMAX"))
    lmax = model_.lmax;
  if(!given("LMIN"))
    lmin = model_.lmin;
  if(!given("LMLT"))
    lmlt = model_.lmlt;
  if(!given("LMOIN"))
    lmoin = model_.lmoin;
  if(!given("LNCH"))
    lnch = model_.lnch;
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
  if(!given("LNLX"))
    lnlx = model_.lnlx;
  if(!given("LNOFF"))
    lnoff = model_.lnoff;
  if(!given("LNPEAK"))
    lnpeak = model_.lnpeak;
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
  if(!given("LPCLM"))
    lpclm = model_.lpclm;
  if(!given("LPDIBLC1"))
    lpdiblc1 = model_.lpdiblc1;
  if(!given("LPDIBLC2"))
    lpdiblc2 = model_.lpdiblc2;
  if(!given("LPDIBLCB"))
    lpdiblcb = model_.lpdiblcb;
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
  if(!given("LRD"))
    lrd = model_.lrd;
  if(!given("LRDSW"))
    lrdsw = model_.lrdsw;
  if(!given("LREF"))
    lref = model_.lref;
  if(!given("LRGIDL"))
    lrgidl = model_.lrgidl;
  if(!given("LRGISL"))
    lrgisl = model_.lrgisl;
  if(!given("LRS"))
    lrs = model_.lrs;
  if(!given("LTVFBSDOFF"))
    ltvfbsdoff = model_.ltvfbsdoff;
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
  if(!given("LXT"))
    lxt = model_.lxt;
  if(!given("METO"))
    meto = model_.meto;
  if(!given("MINR"))
    minr = model_.minr;
  if(!given("MJ"))
    mj = model_.mj;
  if(!given("MJSW"))
    mjsw = model_.mjsw;
  if(!given("MJSWG"))
    mjswg = model_.mjswg;
  if(!given("MOBMOD"))
    mobmod = model_.mobmod;
  if(!given("MOIN"))
    moin = model_.moin;
  if(!given("MULU0"))
    mulu0 = model_.mulu0;
  if(!given("MULUA"))
    mulua = model_.mulua;
  if(!given("MULUB"))
    mulub = model_.mulub;
  if(!given("NCH"))
    nch = model_.nch;
  if(!given("NF"))
    nf = model_.nf;
  if(!given("NFACTOR"))
    nfactor = model_.nfactor;
  if(!given("NGATE"))
    ngate = model_.ngate;
  if(!given("NIGBACC"))
    nigbacc = model_.nigbacc;
  if(!given("NIGBINV"))
    nigbinv = model_.nigbinv;
  if(!given("NIGC"))
    nigc = model_.nigc;
  if(!given("NJ"))
    nj = model_.nj;
  if(!given("NLX"))
    nlx = model_.nlx;
  if(!given("NOFF"))
    noff = model_.noff;
  if(!given("NOIA"))
    noia = model_.noia;
  if(!given("NOIB"))
    noib = model_.noib;
  if(!given("NOIC"))
    noic = model_.noic;
  if(!given("NOIMOD"))
    noimod = model_.noimod;
  if(!given("NQSMOD"))
    nqsmod = model_.nqsmod;
  if(!given("NRD"))
    nrd = model_.nrd;
  if(!given("NRS"))
    nrs = model_.nrs;
  if(!given("NSD"))
    nsd = model_.nsd;
  if(!given("NSUB"))
    nsub = model_.nsub;
  if(!given("NTOX"))
    ntox = model_.ntox;
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
  if(!given("PB"))
    pb = model_.pb;
  if(!given("PB0"))
    pb0 = model_.pb0;
  if(!given("PB1"))
    pb1 = model_.pb1;
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
  if(!given("PBSW"))
    pbsw = model_.pbsw;
  if(!given("PBSWG"))
    pbswg = model_.pbswg;
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
  if(!given("PCKAPPA"))
    pckappa = model_.pckappa;
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
  if(!given("PELM"))
    pelm = model_.pelm;
  if(!given("PETA0"))
    peta0 = model_.peta0;
  if(!given("PETAB"))
    petab = model_.petab;
  if(!given("PFGIDL"))
    pfgidl = model_.pfgidl;
  if(!given("PFGISL"))
    pfgisl = model_.pfgisl;
  if(!given("PGAMMA1"))
    pgamma1 = model_.pgamma1;
  if(!given("PGAMMA2"))
    pgamma2 = model_.pgamma2;
  if(!given("PHP"))
    php = model_.php;
  if(!given("PIGCD"))
    pigcd = model_.pigcd;
  if(!given("PK1"))
    pk1 = model_.pk1;
  if(!given("PK2"))
    pk2 = model_.pk2;
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
  if(!given("PKVTH0"))
    pkvth0 = model_.pkvth0;
  if(!given("PMOIN"))
    pmoin = model_.pmoin;
  if(!given("PNCH"))
    pnch = model_.pnch;
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
  if(!given("PNLX"))
    pnlx = model_.pnlx;
  if(!given("PNOFF"))
    pnoff = model_.pnoff;
  if(!given("PNPEAK"))
    pnpeak = model_.pnpeak;
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
  if(!given("PRD"))
    prd = model_.prd;
  if(!given("PRDSW"))
    prdsw = model_.prdsw;
  if(!given("PRGIDL"))
    prgidl = model_.prgidl;
  if(!given("PRGISL"))
    prgisl = model_.prgisl;
  if(!given("PRS"))
    prs = model_.prs;
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
  if(!given("PTA"))
    pta = model_.pta;
  if(!given("PTP"))
    ptp = model_.ptp;
  if(!given("PTVFBSDOFF"))
    ptvfbsdoff = model_.ptvfbsdoff;
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
  if(!given("PW0"))
    pw0 = model_.pw0;
  if(!given("PWR"))
    pwr = model_.pwr;
  if(!given("PXJ"))
    pxj = model_.pxj;
  if(!given("PXT"))
    pxt = model_.pxt;
  if(!given("RD"))
    rd = model_.rd;
  if(!given("RDC"))
    rdc = model_.rdc;
  if(!given("RDSW"))
    rdsw = model_.rdsw;
  if(!given("RGIDL"))
    rgidl = model_.rgidl;
  if(!given("RGISL"))
    rgisl = model_.rgisl;
  if(!given("RS"))
    rs = model_.rs;
  if(!given("RSC"))
    rsc = model_.rsc;
  if(!given("RSH"))
    rsh = model_.rsh;
  if(!given("SA"))
    sa = model_.sa;
  if(!given("SA0"))
    sa0 = model_.sa0;
  if(!given("SA1"))
    sa1 = model_.sa1;
  if(!given("SA10"))
    sa10 = model_.sa10;
  if(!given("SA2"))
    sa2 = model_.sa2;
  if(!given("SA3"))
    sa3 = model_.sa3;
  if(!given("SA4"))
    sa4 = model_.sa4;
  if(!given("SA5"))
    sa5 = model_.sa5;
  if(!given("SA6"))
    sa6 = model_.sa6;
  if(!given("SA7"))
    sa7 = model_.sa7;
  if(!given("SA8"))
    sa8 = model_.sa8;
  if(!given("SA9"))
    sa9 = model_.sa9;
  if(!given("SAREF"))
    saref = model_.saref;
  if(!given("SB"))
    sb = model_.sb;
  if(!given("SB0"))
    sb0 = model_.sb0;
  if(!given("SB1"))
    sb1 = model_.sb1;
  if(!given("SB10"))
    sb10 = model_.sb10;
  if(!given("SB2"))
    sb2 = model_.sb2;
  if(!given("SB3"))
    sb3 = model_.sb3;
  if(!given("SB4"))
    sb4 = model_.sb4;
  if(!given("SB5"))
    sb5 = model_.sb5;
  if(!given("SB6"))
    sb6 = model_.sb6;
  if(!given("SB7"))
    sb7 = model_.sb7;
  if(!given("SB8"))
    sb8 = model_.sb8;
  if(!given("SB9"))
    sb9 = model_.sb9;
  if(!given("SBREF"))
    sbref = model_.sbref;
  if(!given("SC"))
    sc = model_.sc;
  if(!given("SD"))
    sd = model_.sd;
  if(!given("STETA0"))
    steta0 = model_.steta0;
  if(!given("STIMOD"))
    stimod = model_.stimod;
  if(!given("STK2"))
    stk2 = model_.stk2;
  if(!given("SW1"))
    sw1 = model_.sw1;
  if(!given("SW10"))
    sw10 = model_.sw10;
  if(!given("SW2"))
    sw2 = model_.sw2;
  if(!given("SW3"))
    sw3 = model_.sw3;
  if(!given("SW4"))
    sw4 = model_.sw4;
  if(!given("SW5"))
    sw5 = model_.sw5;
  if(!given("SW6"))
    sw6 = model_.sw6;
  if(!given("SW7"))
    sw7 = model_.sw7;
  if(!given("SW8"))
    sw8 = model_.sw8;
  if(!given("SW9"))
    sw9 = model_.sw9;
  if(!given("TCJ"))
    tcj = model_.tcj;
  if(!given("TCJSW"))
    tcjsw = model_.tcjsw;
  if(!given("TCJSWG"))
    tcjswg = model_.tcjswg;
  if(!given("TEMPMOD"))
    tempmod = model_.tempmod;
  if(!given("TKU0"))
    tku0 = model_.tku0;
  if(!given("TLEV"))
    tlev = model_.tlev;
  if(!given("TLEVC"))
    tlevc = model_.tlevc;
  if(!given("TNOM"))
    tnom = model_.tnom;
  else
    if(tnom < -100 || tnom>150)
      tnom -= P_CELSIUS0;
  if(!given("TOX"))
    tox = model_.tox;
  if(!given("TOXE"))
    toxe = model_.toxe;
  if(!given("TOXM"))
    toxm = model_.toxm;
  if(!given("TOXREF"))
    toxref = model_.toxref;
  if(!given("TPB"))
    tpb = model_.tpb;
  if(!given("TPBSW"))
    tpbsw = model_.tpbsw;
  if(!given("TPBSWG"))
    tpbswg = model_.tpbswg;
  if(!given("TRD"))
    trd = model_.trd;
  if(!given("TRISE"))
    trise = model_.trise;
  if(!given("TRS"))
    trs = model_.trs;
  if(!given("TVFBSDOFF"))
    tvfbsdoff = model_.tvfbsdoff;
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
  if(!given("UTE"))
    ute = model_.ute;
  if(!given("VBM"))
    vbm = model_.vbm;
  if(!given("VBOX"))
    vbox = model_.vbox;
  if(!given("VBX"))
    vbx = model_.vbx;
  if(!given("VERBOSE"))
    verbose = model_.verbose;
  if(!given("VERSION"))
    version = model_.version;
  if(!given("VFB"))
    vfb = model_.vfb;
  if(!given("VFBCV"))
    vfbcv = model_.vfbcv;
  if(!given("VFBFLAG"))
    vfbflag = model_.vfbflag;
  if(!given("VFBSDOFF"))
    vfbsdoff = model_.vfbsdoff;
  if(!given("VGSLIM"))
    vgslim = model_.vgslim;
  if(!given("VOFF"))
    voff = model_.voff;
  if(!given("VOFFCV"))
    voffcv = model_.voffcv;
  if(!given("VSAT"))
    vsat = model_.vsat;
  if(!given("VTH0"))
    vth0 = model_.vth0;
  if(!given("VTM0"))
    vtm0 = model_.vtm0;
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
  if(!given("WCKAPPA"))
    wckappa = model_.wckappa;
  if(!given("WCLC"))
    wclc = model_.wclc;
  if(!given("WCLE"))
    wcle = model_.wcle;
  if(!given("WD"))
    wd = model_.wd;
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
  if(!given("WDWB"))
    wdwb = model_.wdwb;
  if(!given("WDWG"))
    wdwg = model_.wdwg;
  if(!given("WEGIDL"))
    wegidl = model_.wegidl;
  if(!given("WEGISL"))
    wegisl = model_.wegisl;
  if(!given("WEIGBINV"))
    weigbinv = model_.weigbinv;
  if(!given("WELM"))
    welm = model_.welm;
  if(!given("WETA0"))
    weta0 = model_.weta0;
  if(!given("WETAB"))
    wetab = model_.wetab;
  if(!given("WFGIDL"))
    wfgidl = model_.wfgidl;
  if(!given("WFGISL"))
    wfgisl = model_.wfgisl;
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
  if(!given("WKVTH0"))
    wkvth0 = model_.wkvth0;
  if(!given("WL"))
    wl = model_.wl;
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
  if(!given("WMAX"))
    wmax = model_.wmax;
  if(!given("WMIN"))
    wmin = model_.wmin;
  if(!given("WMLT"))
    wmlt = model_.wmlt;
  if(!given("WMOIN"))
    wmoin = model_.wmoin;
  if(!given("WNCH"))
    wnch = model_.wnch;
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
  if(!given("WNLX"))
    wnlx = model_.wnlx;
  if(!given("WNOFF"))
    wnoff = model_.wnoff;
  if(!given("WNOI"))
    wnoi = model_.wnoi;
  if(!given("WNPEAK"))
    wnpeak = model_.wnpeak;
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
  if(!given("WRD"))
    wrd = model_.wrd;
  if(!given("WRDSW"))
    wrdsw = model_.wrdsw;
  if(!given("WREF"))
    wref = model_.wref;
  if(!given("WRGIDL"))
    wrgidl = model_.wrgidl;
  if(!given("WRGISL"))
    wrgisl = model_.wrgisl;
  if(!given("WRS"))
    wrs = model_.wrs;
  if(!given("WTVFBSDOFF"))
    wtvfbsdoff = model_.wtvfbsdoff;
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
  if(!given("WXT"))
    wxt = model_.wxt;
  if(!given("XJ"))
    xj = model_.xj;
  if(!given("XJBVD"))
    xjbvd = model_.xjbvd;
  if(!given("XJBVS"))
    xjbvs = model_.xjbvs;
  if(!given("XL"))
    xl = model_.xl;
  if(!given("XLREF"))
    xlref = model_.xlref;
  if(!given("XPART"))
    xpart = model_.xpart;
  if(!given("XT"))
    xt = model_.xt;
  if(!given("XTI"))
    xti = model_.xti;
  if(!given("XW"))
    xw = model_.xw;
  if(!given("XWREF"))
    xwref = model_.xwref;
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
    li_branch_id(-1),
    li_branch_ig(-1),
    li_branch_is(-1),
    li_branch_ib(-1),
    f_s_Equ_s_Node_Ptr(0),
    f_s_Equ_si_Node_Ptr(0),
    f_si_Equ_s_Node_Ptr(0),
    f_si_Equ_si_Node_Ptr(0),
    f_d_Equ_d_Node_Ptr(0),
    f_d_Equ_di_Node_Ptr(0),
    f_di_Equ_d_Node_Ptr(0),
    f_di_Equ_di_Node_Ptr(0),
    f_di_Equ_b_Node_Ptr(0),
    f_di_Equ_si_Node_Ptr(0),
    f_di_Equ_g_Node_Ptr(0),
    f_si_Equ_b_Node_Ptr(0),
    f_si_Equ_di_Node_Ptr(0),
    f_si_Equ_g_Node_Ptr(0),
    f_g_Equ_b_Node_Ptr(0),
    f_g_Equ_si_Node_Ptr(0),
    f_g_Equ_di_Node_Ptr(0),
    f_g_Equ_g_Node_Ptr(0),
    f_b_Equ_b_Node_Ptr(0),
    f_b_Equ_si_Node_Ptr(0),
    f_b_Equ_di_Node_Ptr(0),
    f_b_Equ_g_Node_Ptr(0),
    q_s_Equ_s_Node_Ptr(0),
    q_s_Equ_si_Node_Ptr(0),
    q_si_Equ_s_Node_Ptr(0),
    q_si_Equ_si_Node_Ptr(0),
    q_d_Equ_d_Node_Ptr(0),
    q_d_Equ_di_Node_Ptr(0),
    q_di_Equ_d_Node_Ptr(0),
    q_di_Equ_di_Node_Ptr(0),
    q_di_Equ_b_Node_Ptr(0),
    q_di_Equ_si_Node_Ptr(0),
    q_di_Equ_g_Node_Ptr(0),
    q_si_Equ_b_Node_Ptr(0),
    q_si_Equ_di_Node_Ptr(0),
    q_si_Equ_g_Node_Ptr(0),
    q_g_Equ_b_Node_Ptr(0),
    q_g_Equ_si_Node_Ptr(0),
    q_g_Equ_di_Node_Ptr(0),
    q_g_Equ_g_Node_Ptr(0),
    q_b_Equ_b_Node_Ptr(0),
    q_b_Equ_si_Node_Ptr(0),
    q_b_Equ_di_Node_Ptr(0),
    q_b_Equ_g_Node_Ptr(0),
    m_s_Equ_s_NodeOffset(-1),
    m_s_Equ_si_NodeOffset(-1),
    m_si_Equ_s_NodeOffset(-1),
    m_si_Equ_si_NodeOffset(-1),
    m_d_Equ_d_NodeOffset(-1),
    m_d_Equ_di_NodeOffset(-1),
    m_di_Equ_d_NodeOffset(-1),
    m_di_Equ_di_NodeOffset(-1),
    m_di_Equ_b_NodeOffset(-1),
    m_di_Equ_si_NodeOffset(-1),
    m_di_Equ_g_NodeOffset(-1),
    m_si_Equ_b_NodeOffset(-1),
    m_si_Equ_di_NodeOffset(-1),
    m_si_Equ_g_NodeOffset(-1),
    m_g_Equ_b_NodeOffset(-1),
    m_g_Equ_si_NodeOffset(-1),
    m_g_Equ_di_NodeOffset(-1),
    m_g_Equ_g_NodeOffset(-1),
    m_b_Equ_b_NodeOffset(-1),
    m_b_Equ_si_NodeOffset(-1),
    m_b_Equ_di_NodeOffset(-1),
    m_b_Equ_g_NodeOffset(-1),
      cogendaTemperature(getDeviceOptions().temp.getImmutableValue<double>())
{
  numExtVars = 4;
  numIntVars = 2;
  setNumStoreVars(0);
  setNumBranchDataVars(0);
  numBranchDataVarsIfAllocated = 4;
  leadCurrentF.resize(4);
  leadCurrentQ.resize(4);
  PairVector jacobianElements;
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_s));
  jacobianElements.push_back(IntPair(cogendaNodeID_s,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_s));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_d));
  jacobianElements.push_back(IntPair(cogendaNodeID_d,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_d));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_di,cogendaNodeID_g));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_si,cogendaNodeID_g));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_g,cogendaNodeID_g));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_b));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_si));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_di));
  jacobianElements.push_back(IntPair(cogendaNodeID_b,cogendaNodeID_g));

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

  if (jacStamp.empty())
  {
    int originalSize = 6;
    computeJacStampAndMaps(jacobianElements,collapsedNodes,jacStamp,nodeMap,pairToJacStampMap,originalSize);
  }
}

  void Instance::collapseNodes() {
  collapseNode_si = false;
  collapseNode_di = false;
if((BSIM3sourceResistance > 0.0)) {
{
}
}
else {
  collapseNode_si = true;
  numIntVars--;
}
if((BSIM3drainResistance > 0.0)) {
{
if((this->verbose == 1)) {
{
}
}
}
}
else {
  collapseNode_di = true;
  numIntVars--;
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
}

  //void Instance::initInternalVars
void Instance::initInternalVars() {
}

  //void Instance::loadNodeSymbols
void Instance::loadNodeSymbols(Util::SymbolTable &symbol_table) const
{
  if (!collapseNode_di)
    addInternalNode(symbol_table, li_di, getName(), "di");
  if (!collapseNode_si)
    addInternalNode(symbol_table, li_si, getName(), "si");
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
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_s)];
  m_s_Equ_s_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_s,cogendaNodeID_si)];
  m_s_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_s)];
  m_si_Equ_s_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_si)];
  m_si_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_d)];
  m_d_Equ_d_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_d,cogendaNodeID_di)];
  m_d_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_d)];
  m_di_Equ_d_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_di)];
  m_di_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_b)];
  m_di_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_si)];
  m_di_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_di,cogendaNodeID_g)];
  m_di_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_b)];
  m_si_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_di)];
  m_si_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_si,cogendaNodeID_g)];
  m_si_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_b)];
  m_g_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_si)];
  m_g_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_di)];
  m_g_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_g,cogendaNodeID_g)];
  m_g_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_b)];
  m_b_Equ_b_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_si)];
  m_b_Equ_si_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_di)];
  m_b_Equ_di_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
  jacLoc = pairToJacStampMap[IntPair(cogendaNodeID_b,cogendaNodeID_g)];
  m_b_Equ_g_NodeOffset = jacLIDVec[jacLoc.first][jacLoc.second];
}

  //void Instance::setupPointers
void Instance::setupPointers( )
{
  Linear::Matrix * dFdxMatPtr = extData.dFdxMatrixPtr;
  Linear::Matrix * dQdxMatPtr = extData.dQdxMatrixPtr;
  f_s_Equ_s_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_s);
  f_s_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_s,li_si);
  f_si_Equ_s_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_s);
  f_si_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_si);
  f_d_Equ_d_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_d);
  f_d_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_d,li_di);
  f_di_Equ_d_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_d);
  f_di_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_di);
  f_di_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_b);
  f_di_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_si);
  f_di_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_di,li_g);
  f_si_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_b);
  f_si_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_di);
  f_si_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_si,li_g);
  f_g_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_b);
  f_g_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_si);
  f_g_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_di);
  f_g_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_g,li_g);
  f_b_Equ_b_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_b);
  f_b_Equ_si_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_si);
  f_b_Equ_di_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_di);
  f_b_Equ_g_Node_Ptr = dFdxMatPtr->returnRawEntryPointer(li_b,li_g);
  q_s_Equ_s_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_s);
  q_s_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_s,li_si);
  q_si_Equ_s_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_s);
  q_si_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_si);
  q_d_Equ_d_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_d);
  q_d_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_d,li_di);
  q_di_Equ_d_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_d);
  q_di_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_di);
  q_di_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_b);
  q_di_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_si);
  q_di_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_di,li_g);
  q_si_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_b);
  q_si_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_di);
  q_si_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_si,li_g);
  q_g_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_b);
  q_g_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_si);
  q_g_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_di);
  q_g_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_g,li_g);
  q_b_Equ_b_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_b);
  q_b_Equ_si_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_si);
  q_b_Equ_di_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_di);
  q_b_Equ_g_Node_Ptr = dQdxMatPtr->returnRawEntryPointer(li_b,li_g);
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
  if (probeVars.size() != (5))
  {
    probeVars.resize(5);
    staticContributions.resize(6+0);
    dynamicContributions.resize(6+0);
  }

  for (int i=0; i < 6+0 ; ++i)
  {
     staticContributions[i]=0;
     dynamicContributions[i]=0;
  }

  probeVars[cogendaProbeID_V_di_si] = (*solVectorPtr)[li_di] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_di_si].diff(cogendaProbeID_V_di_si,5);
  probeVars[cogendaProbeID_V_b_si] = (*solVectorPtr)[li_b] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_b_si].diff(cogendaProbeID_V_b_si,5);
  probeVars[cogendaProbeID_V_g_si] = (*solVectorPtr)[li_g] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_g_si].diff(cogendaProbeID_V_g_si,5);
  probeVars[cogendaProbeID_V_s_si] = (*solVectorPtr)[li_s] - (*solVectorPtr)[li_si];
  probeVars[cogendaProbeID_V_s_si].diff(cogendaProbeID_V_s_si,5);
  probeVars[cogendaProbeID_V_d_di] = (*solVectorPtr)[li_d] - (*solVectorPtr)[li_di];
  probeVars[cogendaProbeID_V_d_di].diff(cogendaProbeID_V_d_di,5);

{
  BSIM3weff = ((0.0)*UNITFAD).val();
  qbulk = ((0.0)*UNITFAD).val();
  qdrn = ((0.0)*UNITFAD).val();
  qgate = ((0.0)*UNITFAD).val();
  BSIM3cgdo = ((this->cgdo)*UNITFAD).val();
  BSIM3cgso = ((this->cgso)*UNITFAD).val();
  BSIM3drainArea = ((this->ad)*UNITFAD).val();
  BSIM3sourceArea = ((this->as)*UNITFAD).val();
  BSIM3drainSquares = ((this->nrd)*UNITFAD).val();
  BSIM3sourceSquares = ((this->nrs)*UNITFAD).val();
  BSIM3drainPerimeter = ((this->pd)*UNITFAD).val();
  BSIM3sourcePerimeter = ((this->ps)*UNITFAD).val();
  BSIM3nqsMod = ((this->nqsmod)*UNITFAD).val();
  BSIM3cdsc = ((this->cdsc)*UNITFAD).val();
  BSIM3cdscb = ((this->cdscb)*UNITFAD).val();
  BSIM3cdscd = ((this->cdscd)*UNITFAD).val();
  BSIM3cit = ((this->cit)*UNITFAD).val();
  BSIM3nfactor = ((this->nfactor)*UNITFAD).val();
  BSIM3xj = ((this->xj)*UNITFAD).val();
  BSIM3vsat = ((this->vsat)*UNITFAD).val();
  BSIM3at = ((this->at)*UNITFAD).val();
  BSIM3a0 = ((this->a0)*UNITFAD).val();
  BSIM3ags = ((this->ags)*UNITFAD).val();
  BSIM3a1 = ((this->a1)*UNITFAD).val();
  BSIM3a2 = ((this->a2)*UNITFAD).val();
  BSIM3keta = ((this->keta)*UNITFAD).val();
  BSIM3nsub = ((this->nsub)*UNITFAD).val();
  BSIM3ngate = ((this->ngate)*UNITFAD).val();
  BSIM3gamma1 = ((this->gamma1)*UNITFAD).val();
  BSIM3gamma2 = ((this->gamma2)*UNITFAD).val();
  BSIM3vbx = ((this->vbx)*UNITFAD).val();
  BSIM3vbm = ((this->vbm)*UNITFAD).val();
  BSIM3xt = ((this->xt)*UNITFAD).val();
  BSIM3vfb = ((this->vfb)*UNITFAD).val();
  BSIM3kt1 = ((this->kt1)*UNITFAD).val();
  BSIM3kt1l = ((this->kt1l)*UNITFAD).val();
  BSIM3kt2 = ((this->kt2)*UNITFAD).val();
  BSIM3k3 = ((this->k3)*UNITFAD).val();
  BSIM3k3b = ((this->k3b)*UNITFAD).val();
  BSIM3w0 = ((this->w0)*UNITFAD).val();
  BSIM3nlx = ((this->nlx)*UNITFAD).val();
  BSIM3dvt0 = ((this->dvt0)*UNITFAD).val();
  BSIM3dvt1 = ((this->dvt1)*UNITFAD).val();
  BSIM3dvt2 = ((this->dvt2)*UNITFAD).val();
  BSIM3dvt0w = ((this->dvt0w)*UNITFAD).val();
  BSIM3dvt1w = ((this->dvt1w)*UNITFAD).val();
  BSIM3dvt2w = ((this->dvt2w)*UNITFAD).val();
  BSIM3drout = ((this->drout)*UNITFAD).val();
  BSIM3dsub = ((this->dsub)*UNITFAD).val();
  BSIM3vth0 = ((this->vth0)*UNITFAD).val();
  BSIM3ua = ((this->ua)*UNITFAD).val();
  BSIM3ua1 = ((this->ua1)*UNITFAD).val();
  BSIM3ub = ((this->ub)*UNITFAD).val();
  BSIM3ub1 = ((this->ub1)*UNITFAD).val();
  BSIM3uc = ((this->uc)*UNITFAD).val();
  BSIM3uc1 = ((this->uc1)*UNITFAD).val();
  BSIM3u0 = ((this->u0)*UNITFAD).val();
  BSIM3ute = ((this->ute)*UNITFAD).val();
  BSIM3voff = ((this->voff)*UNITFAD).val();
  BSIM3delta = ((this->delta)*UNITFAD).val();
  BSIM3rdsw = ((this->rdsw)*UNITFAD).val();
  BSIM3prwg = ((this->prwg)*UNITFAD).val();
  BSIM3prwb = ((this->prwb)*UNITFAD).val();
  BSIM3prt = ((this->prt)*UNITFAD).val();
  BSIM3eta0 = ((this->eta0)*UNITFAD).val();
  BSIM3etab = ((this->etab)*UNITFAD).val();
  BSIM3pscbe1 = ((this->pscbe1)*UNITFAD).val();
  BSIM3pscbe2 = ((this->pscbe2)*UNITFAD).val();
  BSIM3pvag = ((this->pvag)*UNITFAD).val();
  BSIM3wr = ((this->wr)*UNITFAD).val();
  BSIM3dwg = ((this->dwg)*UNITFAD).val();
  BSIM3dwb = ((this->dwb)*UNITFAD).val();
  BSIM3b0 = ((this->b0)*UNITFAD).val();
  BSIM3b1 = ((this->b1)*UNITFAD).val();
  BSIM3alpha0 = ((this->alpha0)*UNITFAD).val();
  BSIM3alpha1 = ((this->alpha1)*UNITFAD).val();
  BSIM3beta0 = ((this->beta0)*UNITFAD).val();
  BSIM3elm = ((this->elm)*UNITFAD).val();
  BSIM3cgsl = ((this->cgsl)*UNITFAD).val();
  BSIM3cgdl = ((this->cgdl)*UNITFAD).val();
  BSIM3ckappa = ((this->ckappa)*UNITFAD).val();
  BSIM3cf = ((this->cf)*UNITFAD).val();
  BSIM3clc = ((this->clc)*UNITFAD).val();
  BSIM3cle = ((this->cle)*UNITFAD).val();
  BSIM3vfbcv = ((this->vfbcv)*UNITFAD).val();
  BSIM3noff = ((this->noff)*UNITFAD).val();
  BSIM3voffcv = ((this->voffcv)*UNITFAD).val();
  BSIM3acde = ((this->acde)*UNITFAD).val();
  BSIM3moin = ((this->moin)*UNITFAD).val();
  BSIM3mobMod = ((this->mobmod)*UNITFAD).val();
  BSIM3capMod = ((this->capmod)*UNITFAD).val();
  BSIM3noiMod = ((this->noimod)*UNITFAD).val();
  BSIM3binUnit = ((this->binunit)*UNITFAD).val();
  BSIM3tox = ((this->tox)*UNITFAD).val();
  BSIM3toxm = ((this->toxm)*UNITFAD).val();
  BSIM3npeak = ((this->nch)*UNITFAD).val();
  BSIM3k1 = ((this->k1)*UNITFAD).val();
  BSIM3k2 = ((this->k2)*UNITFAD).val();
  BSIM3pdibl1 = ((this->pdiblc1)*UNITFAD).val();
  BSIM3pdibl2 = ((this->pdiblc2)*UNITFAD).val();
  BSIM3pdiblb = ((this->pdiblcb)*UNITFAD).val();
  BSIM3pclm = ((this->pclm)*UNITFAD).val();
  if((this->ijth == -9.9999e-99)) {
    BSIM3ijth = ((0.1)*UNITFAD).val();
  }
  else {
    BSIM3ijth = ((this->ijth)*UNITFAD).val();
  }
  BSIM3dwc = ((this->dwc)*UNITFAD).val();
  BSIM3dlc = ((this->dlc)*UNITFAD).val();
  BSIM3tcj = ((this->tcj)*UNITFAD).val();
  BSIM3tcjsw = ((this->tcjsw)*UNITFAD).val();
  BSIM3tcjswg = ((this->tcjswg)*UNITFAD).val();
  BSIM3tpb = ((this->tpb)*UNITFAD).val();
  BSIM3tpbsw = ((this->tpbsw)*UNITFAD).val();
  BSIM3tpbswg = ((this->tpbswg)*UNITFAD).val();
  BSIM3lcdsc = ((this->lcdsc)*UNITFAD).val();
  BSIM3lcdscb = ((this->lcdscb)*UNITFAD).val();
  BSIM3lcdscd = ((this->lcdscd)*UNITFAD).val();
  BSIM3lcit = ((this->lcit)*UNITFAD).val();
  BSIM3lnfactor = ((this->lnfactor)*UNITFAD).val();
  BSIM3lxj = ((this->lxj)*UNITFAD).val();
  BSIM3lvsat = ((this->lvsat)*UNITFAD).val();
  BSIM3lat = ((this->lat)*UNITFAD).val();
  BSIM3la0 = ((this->la0)*UNITFAD).val();
  BSIM3lags = ((this->lags)*UNITFAD).val();
  BSIM3la1 = ((this->la1)*UNITFAD).val();
  BSIM3la2 = ((this->la2)*UNITFAD).val();
  BSIM3lketa = ((this->lketa)*UNITFAD).val();
  BSIM3lnsub = ((this->lnsub)*UNITFAD).val();
  BSIM3lnpeak = ((this->lnpeak)*UNITFAD).val();
  BSIM3lngate = ((this->lngate)*UNITFAD).val();
  BSIM3lgamma1 = ((this->lgamma1)*UNITFAD).val();
  BSIM3lgamma2 = ((this->lgamma2)*UNITFAD).val();
  BSIM3lvbx = ((this->lvbx)*UNITFAD).val();
  BSIM3lvbm = ((this->lvbm)*UNITFAD).val();
  BSIM3lxt = ((this->lxt)*UNITFAD).val();
  BSIM3lk1 = ((this->lk1)*UNITFAD).val();
  BSIM3lkt1 = ((this->lkt1)*UNITFAD).val();
  BSIM3lkt1l = ((this->lkt1l)*UNITFAD).val();
  BSIM3lkt2 = ((this->lkt2)*UNITFAD).val();
  BSIM3lk2 = ((this->lk2)*UNITFAD).val();
  BSIM3lk3 = ((this->lk3)*UNITFAD).val();
  BSIM3lk3b = ((this->lk3b)*UNITFAD).val();
  BSIM3lw0 = ((this->lw0)*UNITFAD).val();
  BSIM3lnlx = ((this->lnlx)*UNITFAD).val();
  BSIM3ldvt0 = ((this->ldvt0)*UNITFAD).val();
  BSIM3ldvt1 = ((this->ldvt1)*UNITFAD).val();
  BSIM3ldvt2 = ((this->ldvt2)*UNITFAD).val();
  BSIM3ldvt0w = ((this->ldvt0w)*UNITFAD).val();
  BSIM3ldvt1w = ((this->ldvt1w)*UNITFAD).val();
  BSIM3ldvt2w = ((this->ldvt2w)*UNITFAD).val();
  BSIM3ldrout = ((this->ldrout)*UNITFAD).val();
  BSIM3ldsub = ((this->ldsub)*UNITFAD).val();
  BSIM3lvth0 = ((this->lvth0)*UNITFAD).val();
  BSIM3lua = ((this->lua)*UNITFAD).val();
  BSIM3lua1 = ((this->lua1)*UNITFAD).val();
  BSIM3lub = ((this->lub)*UNITFAD).val();
  BSIM3lub1 = ((this->lub1)*UNITFAD).val();
  BSIM3luc = ((this->luc)*UNITFAD).val();
  BSIM3luc1 = ((this->luc1)*UNITFAD).val();
  BSIM3lu0 = ((this->lu0)*UNITFAD).val();
  BSIM3lute = ((this->lute)*UNITFAD).val();
  BSIM3lvoff = ((this->lvoff)*UNITFAD).val();
  BSIM3ldelta = ((this->ldelta)*UNITFAD).val();
  BSIM3lrdsw = ((this->lrdsw)*UNITFAD).val();
  BSIM3lprwg = ((this->lprwg)*UNITFAD).val();
  BSIM3lprwb = ((this->lprwb)*UNITFAD).val();
  BSIM3lprt = ((this->lprt)*UNITFAD).val();
  BSIM3leta0 = ((this->leta0)*UNITFAD).val();
  BSIM3letab = ((this->letab)*UNITFAD).val();
  BSIM3lpclm = ((this->lpclm)*UNITFAD).val();
  BSIM3lpdibl1 = ((this->lpdiblc1)*UNITFAD).val();
  BSIM3lpdibl2 = ((this->lpdiblc2)*UNITFAD).val();
  BSIM3lpdiblb = ((this->lpdiblcb)*UNITFAD).val();
  BSIM3lpscbe1 = ((this->lpscbe1)*UNITFAD).val();
  BSIM3lpscbe2 = ((this->lpscbe2)*UNITFAD).val();
  BSIM3lpvag = ((this->lpvag)*UNITFAD).val();
  BSIM3lwr = ((this->lwr)*UNITFAD).val();
  BSIM3ldwg = ((this->ldwg)*UNITFAD).val();
  BSIM3ldwb = ((this->ldwb)*UNITFAD).val();
  BSIM3lb0 = ((this->lb0)*UNITFAD).val();
  BSIM3lb1 = ((this->lb1)*UNITFAD).val();
  BSIM3lalpha0 = ((this->lalpha0)*UNITFAD).val();
  BSIM3lalpha1 = ((this->lalpha1)*UNITFAD).val();
  BSIM3lbeta0 = ((this->lbeta0)*UNITFAD).val();
  BSIM3lvfb = ((this->lvfb)*UNITFAD).val();
  BSIM3lelm = ((this->lelm)*UNITFAD).val();
  BSIM3lcgsl = ((this->lcgsl)*UNITFAD).val();
  BSIM3lcgdl = ((this->lcgdl)*UNITFAD).val();
  BSIM3lckappa = ((this->lckappa)*UNITFAD).val();
  BSIM3lcf = ((this->lcf)*UNITFAD).val();
  BSIM3lclc = ((this->lclc)*UNITFAD).val();
  BSIM3lcle = ((this->lcle)*UNITFAD).val();
  BSIM3lvfbcv = ((this->lvfbcv)*UNITFAD).val();
  BSIM3lnoff = ((this->lnoff)*UNITFAD).val();
  BSIM3lvoffcv = ((this->lvoffcv)*UNITFAD).val();
  BSIM3lacde = ((this->lacde)*UNITFAD).val();
  BSIM3lmoin = ((this->lmoin)*UNITFAD).val();
  BSIM3wcdsc = ((this->wcdsc)*UNITFAD).val();
  BSIM3wcdscb = ((this->wcdscb)*UNITFAD).val();
  BSIM3wcdscd = ((this->wcdscd)*UNITFAD).val();
  BSIM3wcit = ((this->wcit)*UNITFAD).val();
  BSIM3wnfactor = ((this->wnfactor)*UNITFAD).val();
  BSIM3wxj = ((this->wxj)*UNITFAD).val();
  BSIM3wvsat = ((this->wvsat)*UNITFAD).val();
  BSIM3wat = ((this->wat)*UNITFAD).val();
  BSIM3wa0 = ((this->wa0)*UNITFAD).val();
  BSIM3wags = ((this->wags)*UNITFAD).val();
  BSIM3wa1 = ((this->wa1)*UNITFAD).val();
  BSIM3wa2 = ((this->wa2)*UNITFAD).val();
  BSIM3wketa = ((this->wketa)*UNITFAD).val();
  BSIM3wnsub = ((this->wnsub)*UNITFAD).val();
  BSIM3wnpeak = ((this->wnpeak)*UNITFAD).val();
  BSIM3wngate = ((this->wngate)*UNITFAD).val();
  BSIM3wgamma1 = ((this->wgamma1)*UNITFAD).val();
  BSIM3wgamma2 = ((this->wgamma2)*UNITFAD).val();
  BSIM3wvbx = ((this->wvbx)*UNITFAD).val();
  BSIM3wvbm = ((this->wvbm)*UNITFAD).val();
  BSIM3wxt = ((this->wxt)*UNITFAD).val();
  BSIM3wk1 = ((this->wk1)*UNITFAD).val();
  BSIM3wkt1 = ((this->wkt1)*UNITFAD).val();
  BSIM3wkt1l = ((this->wkt1l)*UNITFAD).val();
  BSIM3wkt2 = ((this->wkt2)*UNITFAD).val();
  BSIM3wk2 = ((this->wk2)*UNITFAD).val();
  BSIM3wk3 = ((this->wk3)*UNITFAD).val();
  BSIM3wk3b = ((this->wk3b)*UNITFAD).val();
  BSIM3ww0 = ((this->ww0)*UNITFAD).val();
  BSIM3wnlx = ((this->wnlx)*UNITFAD).val();
  BSIM3wdvt0 = ((this->wdvt0)*UNITFAD).val();
  BSIM3wdvt1 = ((this->wdvt1)*UNITFAD).val();
  BSIM3wdvt2 = ((this->wdvt2)*UNITFAD).val();
  BSIM3wdvt0w = ((this->wdvt0w)*UNITFAD).val();
  BSIM3wdvt1w = ((this->wdvt1w)*UNITFAD).val();
  BSIM3wdvt2w = ((this->wdvt2w)*UNITFAD).val();
  BSIM3wdrout = ((this->wdrout)*UNITFAD).val();
  BSIM3wdsub = ((this->wdsub)*UNITFAD).val();
  BSIM3wvth0 = ((this->wvth0)*UNITFAD).val();
  BSIM3wua = ((this->wua)*UNITFAD).val();
  BSIM3wua1 = ((this->wua1)*UNITFAD).val();
  BSIM3wub = ((this->wub)*UNITFAD).val();
  BSIM3wub1 = ((this->wub1)*UNITFAD).val();
  BSIM3wuc = ((this->wuc)*UNITFAD).val();
  BSIM3wuc1 = ((this->wuc1)*UNITFAD).val();
  BSIM3wu0 = ((this->wu0)*UNITFAD).val();
  BSIM3wute = ((this->wute)*UNITFAD).val();
  BSIM3wvoff = ((this->wvoff)*UNITFAD).val();
  BSIM3wdelta = ((this->wdelta)*UNITFAD).val();
  BSIM3wrdsw = ((this->wrdsw)*UNITFAD).val();
  BSIM3wprwg = ((this->wprwg)*UNITFAD).val();
  BSIM3wprwb = ((this->wprwb)*UNITFAD).val();
  BSIM3wprt = ((this->wprt)*UNITFAD).val();
  BSIM3weta0 = ((this->weta0)*UNITFAD).val();
  BSIM3wetab = ((this->wetab)*UNITFAD).val();
  BSIM3wpclm = ((this->wpclm)*UNITFAD).val();
  BSIM3wpdibl1 = ((this->wpdiblc1)*UNITFAD).val();
  BSIM3wpdibl2 = ((this->wpdiblc2)*UNITFAD).val();
  BSIM3wpdiblb = ((this->wpdiblcb)*UNITFAD).val();
  BSIM3wpscbe1 = ((this->wpscbe1)*UNITFAD).val();
  BSIM3wpscbe2 = ((this->wpscbe2)*UNITFAD).val();
  BSIM3wpvag = ((this->wpvag)*UNITFAD).val();
  BSIM3wwr = ((this->wwr)*UNITFAD).val();
  BSIM3wdwg = ((this->wdwg)*UNITFAD).val();
  BSIM3wdwb = ((this->wdwb)*UNITFAD).val();
  BSIM3wb0 = ((this->wb0)*UNITFAD).val();
  BSIM3wb1 = ((this->wb1)*UNITFAD).val();
  BSIM3walpha0 = ((this->walpha0)*UNITFAD).val();
  BSIM3walpha1 = ((this->walpha1)*UNITFAD).val();
  BSIM3wbeta0 = ((this->wbeta0)*UNITFAD).val();
  BSIM3wvfb = ((this->wvfb)*UNITFAD).val();
  BSIM3welm = ((this->welm)*UNITFAD).val();
  BSIM3wcgsl = ((this->wcgsl)*UNITFAD).val();
  BSIM3wcgdl = ((this->wcgdl)*UNITFAD).val();
  BSIM3wckappa = ((this->wckappa)*UNITFAD).val();
  BSIM3wcf = ((this->wcf)*UNITFAD).val();
  BSIM3wclc = ((this->wclc)*UNITFAD).val();
  BSIM3wcle = ((this->wcle)*UNITFAD).val();
  BSIM3wvfbcv = ((this->wvfbcv)*UNITFAD).val();
  BSIM3wnoff = ((this->wnoff)*UNITFAD).val();
  BSIM3wvoffcv = ((this->wvoffcv)*UNITFAD).val();
  BSIM3wacde = ((this->wacde)*UNITFAD).val();
  BSIM3wmoin = ((this->wmoin)*UNITFAD).val();
  BSIM3pcdsc = ((this->pcdsc)*UNITFAD).val();
  BSIM3pcdscb = ((this->pcdscb)*UNITFAD).val();
  BSIM3pcdscd = ((this->pcdscd)*UNITFAD).val();
  BSIM3pcit = ((this->pcit)*UNITFAD).val();
  BSIM3pnfactor = ((this->pnfactor)*UNITFAD).val();
  BSIM3pxj = ((this->pxj)*UNITFAD).val();
  BSIM3pvsat = ((this->pvsat)*UNITFAD).val();
  BSIM3pat = ((this->pat)*UNITFAD).val();
  BSIM3pa0 = ((this->pa0)*UNITFAD).val();
  BSIM3pags = ((this->pags)*UNITFAD).val();
  BSIM3pa1 = ((this->pa1)*UNITFAD).val();
  BSIM3pa2 = ((this->pa2)*UNITFAD).val();
  BSIM3pketa = ((this->pketa)*UNITFAD).val();
  BSIM3pnsub = ((this->pnsub)*UNITFAD).val();
  BSIM3pnpeak = ((this->pnpeak)*UNITFAD).val();
  BSIM3pngate = ((this->pngate)*UNITFAD).val();
  BSIM3pgamma1 = ((this->pgamma1)*UNITFAD).val();
  BSIM3pgamma2 = ((this->pgamma2)*UNITFAD).val();
  BSIM3pvbx = ((this->pvbx)*UNITFAD).val();
  BSIM3pvbm = ((this->pvbm)*UNITFAD).val();
  BSIM3pxt = ((this->pxt)*UNITFAD).val();
  BSIM3pk1 = ((this->pk1)*UNITFAD).val();
  BSIM3pkt1 = ((this->pkt1)*UNITFAD).val();
  BSIM3pkt1l = ((this->pkt1l)*UNITFAD).val();
  BSIM3pkt2 = ((this->pkt2)*UNITFAD).val();
  BSIM3pk2 = ((this->pk2)*UNITFAD).val();
  BSIM3pk3 = ((this->pk3)*UNITFAD).val();
  BSIM3pk3b = ((this->pk3b)*UNITFAD).val();
  BSIM3pw0 = ((this->pw0)*UNITFAD).val();
  BSIM3pnlx = ((this->pnlx)*UNITFAD).val();
  BSIM3pdvt0 = ((this->pdvt0)*UNITFAD).val();
  BSIM3pdvt1 = ((this->pdvt1)*UNITFAD).val();
  BSIM3pdvt2 = ((this->pdvt2)*UNITFAD).val();
  BSIM3pdvt0w = ((this->pdvt0w)*UNITFAD).val();
  BSIM3pdvt1w = ((this->pdvt1w)*UNITFAD).val();
  BSIM3pdvt2w = ((this->pdvt2w)*UNITFAD).val();
  BSIM3pdrout = ((this->pdrout)*UNITFAD).val();
  BSIM3pdsub = ((this->pdsub)*UNITFAD).val();
  BSIM3pvth0 = ((this->pvth0)*UNITFAD).val();
  BSIM3pua = ((this->pua)*UNITFAD).val();
  BSIM3pua1 = ((this->pua1)*UNITFAD).val();
  BSIM3pub = ((this->pub)*UNITFAD).val();
  BSIM3pub1 = ((this->pub1)*UNITFAD).val();
  BSIM3puc = ((this->puc)*UNITFAD).val();
  BSIM3puc1 = ((this->puc1)*UNITFAD).val();
  BSIM3pu0 = ((this->pu0)*UNITFAD).val();
  BSIM3pute = ((this->pute)*UNITFAD).val();
  BSIM3pvoff = ((this->pvoff)*UNITFAD).val();
  BSIM3pdelta = ((this->pdelta)*UNITFAD).val();
  BSIM3prdsw = ((this->prdsw)*UNITFAD).val();
  BSIM3pprwg = ((this->pprwg)*UNITFAD).val();
  BSIM3pprwb = ((this->pprwb)*UNITFAD).val();
  BSIM3pprt = ((this->pprt)*UNITFAD).val();
  BSIM3peta0 = ((this->peta0)*UNITFAD).val();
  BSIM3petab = ((this->petab)*UNITFAD).val();
  BSIM3ppclm = ((this->ppclm)*UNITFAD).val();
  BSIM3ppdibl1 = ((this->ppdiblc1)*UNITFAD).val();
  BSIM3ppdibl2 = ((this->ppdiblc2)*UNITFAD).val();
  BSIM3ppdiblb = ((this->ppdiblcb)*UNITFAD).val();
  BSIM3ppscbe1 = ((this->ppscbe1)*UNITFAD).val();
  BSIM3ppscbe2 = ((this->ppscbe2)*UNITFAD).val();
  BSIM3ppvag = ((this->ppvag)*UNITFAD).val();
  BSIM3pwr = ((this->pwr)*UNITFAD).val();
  BSIM3pdwg = ((this->pdwg)*UNITFAD).val();
  BSIM3pdwb = ((this->pdwb)*UNITFAD).val();
  BSIM3pb0 = ((this->pb0)*UNITFAD).val();
  BSIM3pb1 = ((this->pb1)*UNITFAD).val();
  BSIM3palpha0 = ((this->palpha0)*UNITFAD).val();
  BSIM3palpha1 = ((this->palpha1)*UNITFAD).val();
  BSIM3pbeta0 = ((this->pbeta0)*UNITFAD).val();
  BSIM3pvfb = ((this->pvfb)*UNITFAD).val();
  BSIM3pelm = ((this->pelm)*UNITFAD).val();
  BSIM3pcgsl = ((this->pcgsl)*UNITFAD).val();
  BSIM3pcgdl = ((this->pcgdl)*UNITFAD).val();
  BSIM3pckappa = ((this->pckappa)*UNITFAD).val();
  BSIM3pcf = ((this->pcf)*UNITFAD).val();
  BSIM3pclc = ((this->pclc)*UNITFAD).val();
  BSIM3pcle = ((this->pcle)*UNITFAD).val();
  BSIM3pvfbcv = ((this->pvfbcv)*UNITFAD).val();
  BSIM3pnoff = ((this->pnoff)*UNITFAD).val();
  BSIM3pvoffcv = ((this->pvoffcv)*UNITFAD).val();
  BSIM3pacde = ((this->pacde)*UNITFAD).val();
  BSIM3pmoin = ((this->pmoin)*UNITFAD).val();
  BSIM3tnom = (((this->tnom+273.15))*UNITFAD).val();
  BSIM3cgbo = ((this->cgbo)*UNITFAD).val();
  BSIM3xpart = ((this->xpart)*UNITFAD).val();
  BSIM3sheetResistance = ((this->rsh)*UNITFAD).val();
  BSIM3jctSatCurDensity = ((this->js)*UNITFAD).val();
  BSIM3jctSidewallSatCurDensity = ((this->jsw)*UNITFAD).val();
  BSIM3bulkJctPotential = ((this->pb)*UNITFAD).val();
  BSIM3bulkJctBotGradingCoeff = ((this->mj)*UNITFAD).val();
  BSIM3bulkJctSideGradingCoeff = ((this->mjsw)*UNITFAD).val();
  BSIM3bulkJctGateSideGradingCoeff = ((this->mjswg)*UNITFAD).val();
  BSIM3sidewallJctPotential = ((this->pbsw)*UNITFAD).val();
  BSIM3GatesidewallJctPotential = ((this->pbswg)*UNITFAD).val();
  BSIM3unitAreaJctCap = ((this->cj)*UNITFAD).val();
  BSIM3unitLengthSidewallJctCap = ((this->cjsw)*UNITFAD).val();
  BSIM3unitLengthGateSidewallJctCap = ((this->cjswg)*UNITFAD).val();
  BSIM3jctEmissionCoeff = ((this->nj)*UNITFAD).val();
  BSIM3jctTempExponent = ((this->xti)*UNITFAD).val();
  BSIM3Lint = ((this->lint)*UNITFAD).val();
  BSIM3Ll = ((this->ll)*UNITFAD).val();
  BSIM3Llc = ((this->llc)*UNITFAD).val();
  BSIM3Lln = ((this->lln)*UNITFAD).val();
  BSIM3Lw = ((this->lw)*UNITFAD).val();
  BSIM3Lwc = ((this->lwc)*UNITFAD).val();
  BSIM3Lwn = ((this->lwn)*UNITFAD).val();
  BSIM3Lwl = ((this->lwl)*UNITFAD).val();
  BSIM3Lwlc = ((this->lwlc)*UNITFAD).val();
  BSIM3Wint = ((this->wint)*UNITFAD).val();
  BSIM3Wl = ((this->wl)*UNITFAD).val();
  BSIM3Wlc = ((this->wlc)*UNITFAD).val();
  BSIM3Wln = ((this->wln)*UNITFAD).val();
  BSIM3Ww = ((this->ww)*UNITFAD).val();
  BSIM3Wwc = ((this->wwc)*UNITFAD).val();
  BSIM3Wwn = ((this->wwn)*UNITFAD).val();
  BSIM3Wwl = ((this->wwl)*UNITFAD).val();
  BSIM3Wwlc = ((this->wwlc)*UNITFAD).val();
  BSIM3unitAreaTempJctCap = ((this->tcj)*UNITFAD).val();
  BSIM3unitLengthSidewallTempJctCap = ((this->tcjsw)*UNITFAD).val();
  BSIM3unitLengthGateSidewallTempJctCap = ((this->tcjswg)*UNITFAD).val();
  BSIM3af = ((this->af)*UNITFAD).val();
  BSIM3kf = ((this->kf)*UNITFAD).val();
  BSIM3ef = ((this->ef)*UNITFAD).val();
  BSIM3type = ((this->type)*UNITFAD).val();
  BSIM3agidl = ((this->agidl)*UNITFAD).val();
  BSIM3lagidl = ((this->lagidl)*UNITFAD).val();
  BSIM3wagidl = ((this->wagidl)*UNITFAD).val();
  BSIM3pagidl = ((this->pagidl)*UNITFAD).val();
  BSIM3bgidl = ((this->bgidl)*UNITFAD).val();
  BSIM3lbgidl = ((this->lbgidl)*UNITFAD).val();
  BSIM3wbgidl = ((this->wbgidl)*UNITFAD).val();
  BSIM3pbgidl = ((this->pbgidl)*UNITFAD).val();
  BSIM3cgidl = ((this->cgidl)*UNITFAD).val();
  BSIM3lcgidl = ((this->lcgidl)*UNITFAD).val();
  BSIM3wcgidl = ((this->wcgidl)*UNITFAD).val();
  BSIM3pcgidl = ((this->pcgidl)*UNITFAD).val();
  BSIM3egidl = ((this->egidl)*UNITFAD).val();
  BSIM3legidl = ((this->legidl)*UNITFAD).val();
  BSIM3wegidl = ((this->wegidl)*UNITFAD).val();
  BSIM3pegidl = ((this->pegidl)*UNITFAD).val();
  BSIM3nigc = ((this->nigc)*UNITFAD).val();
  BSIM3lnigc = ((this->lnigc)*UNITFAD).val();
  BSIM3wnigc = ((this->wnigc)*UNITFAD).val();
  BSIM3pnigc = ((this->pnigc)*UNITFAD).val();
  BSIM3aigc = ((this->aigc)*UNITFAD).val();
  BSIM3laigc = ((this->laigc)*UNITFAD).val();
  BSIM3waigc = ((this->waigc)*UNITFAD).val();
  BSIM3paigc = ((this->paigc)*UNITFAD).val();
  BSIM3bigc = ((this->bigc)*UNITFAD).val();
  BSIM3lbigc = ((this->lbigc)*UNITFAD).val();
  BSIM3wbigc = ((this->wbigc)*UNITFAD).val();
  BSIM3pbigc = ((this->pbigc)*UNITFAD).val();
  BSIM3cigc = ((this->cigc)*UNITFAD).val();
  BSIM3lcigc = ((this->lcigc)*UNITFAD).val();
  BSIM3wcigc = ((this->wcigc)*UNITFAD).val();
  BSIM3pcigc = ((this->pcigc)*UNITFAD).val();
  BSIM3aigbacc = ((this->aigbacc)*UNITFAD).val();
  BSIM3laigbacc = ((this->laigbacc)*UNITFAD).val();
  BSIM3waigbacc = ((this->waigbacc)*UNITFAD).val();
  BSIM3paigbacc = ((this->paigbacc)*UNITFAD).val();
  BSIM3bigbacc = ((this->bigbacc)*UNITFAD).val();
  BSIM3lbigbacc = ((this->lbigbacc)*UNITFAD).val();
  BSIM3wbigbacc = ((this->wbigbacc)*UNITFAD).val();
  BSIM3pbigbacc = ((this->pbigbacc)*UNITFAD).val();
  BSIM3cigbacc = ((this->cigbacc)*UNITFAD).val();
  BSIM3lcigbacc = ((this->lcigbacc)*UNITFAD).val();
  BSIM3wcigbacc = ((this->wcigbacc)*UNITFAD).val();
  BSIM3pcigbacc = ((this->pcigbacc)*UNITFAD).val();
  BSIM3aigbinv = ((this->aigbinv)*UNITFAD).val();
  BSIM3laigbinv = ((this->laigbinv)*UNITFAD).val();
  BSIM3waigbinv = ((this->waigbinv)*UNITFAD).val();
  BSIM3paigbinv = ((this->paigbinv)*UNITFAD).val();
  BSIM3bigbinv = ((this->bigbinv)*UNITFAD).val();
  BSIM3lbigbinv = ((this->lbigbinv)*UNITFAD).val();
  BSIM3wbigbinv = ((this->wbigbinv)*UNITFAD).val();
  BSIM3pbigbinv = ((this->pbigbinv)*UNITFAD).val();
  BSIM3cigbinv = ((this->cigbinv)*UNITFAD).val();
  BSIM3lcigbinv = ((this->lcigbinv)*UNITFAD).val();
  BSIM3wcigbinv = ((this->wcigbinv)*UNITFAD).val();
  BSIM3pcigbinv = ((this->pcigbinv)*UNITFAD).val();
  BSIM3eigbinv = ((this->eigbinv)*UNITFAD).val();
  BSIM3leigbinv = ((this->leigbinv)*UNITFAD).val();
  BSIM3weigbinv = ((this->weigbinv)*UNITFAD).val();
  BSIM3peigbinv = ((this->peigbinv)*UNITFAD).val();
  BSIM3nigbinv = ((this->nigbinv)*UNITFAD).val();
  BSIM3lnigbinv = ((this->lnigbinv)*UNITFAD).val();
  BSIM3wnigbinv = ((this->wnigbinv)*UNITFAD).val();
  BSIM3pnigbinv = ((this->pnigbinv)*UNITFAD).val();
  BSIM3toxe = ((this->toxe)*UNITFAD).val();
  BSIM3vtm0 = ((this->vtm0)*UNITFAD).val();
  BSIM3toxref = ((this->toxref)*UNITFAD).val();
  BSIM3ntox = ((this->ntox)*UNITFAD).val();
  BSIM3lntox = ((this->lntox)*UNITFAD).val();
  BSIM3wntox = ((this->wntox)*UNITFAD).val();
  BSIM3pntox = ((this->pntox)*UNITFAD).val();
  BSIM3pigcd = ((this->pigcd)*UNITFAD).val();
  BSIM3lpigcd = ((this->lpigcd)*UNITFAD).val();
  BSIM3wpigcd = ((this->wpigcd)*UNITFAD).val();
  BSIM3ppigcd = ((this->ppigcd)*UNITFAD).val();
  BSIM3nsd = ((this->nsd)*UNITFAD).val();
  BSIM3lnsd = ((this->lnsd)*UNITFAD).val();
  BSIM3wnsd = ((this->wnsd)*UNITFAD).val();
  BSIM3pnsd = ((this->pnsd)*UNITFAD).val();
  BSIM3vfbsdoff = ((this->vfbsdoff)*UNITFAD).val();
  BSIM3lvfbsdoff = ((this->lvfbsdoff)*UNITFAD).val();
  BSIM3wvfbsdoff = ((this->wvfbsdoff)*UNITFAD).val();
  BSIM3pvfbsdoff = ((this->pvfbsdoff)*UNITFAD).val();
  BSIM3tvfbsdoff = ((this->tvfbsdoff)*UNITFAD).val();
  BSIM3ltvfbsdoff = ((this->ltvfbsdoff)*UNITFAD).val();
  BSIM3wtvfbsdoff = ((this->wtvfbsdoff)*UNITFAD).val();
  BSIM3ptvfbsdoff = ((this->ptvfbsdoff)*UNITFAD).val();
  BSIM3poxedge = ((this->poxedge)*UNITFAD).val();
  BSIM3lpoxedge = ((this->lpoxedge)*UNITFAD).val();
  BSIM3wpoxedge = ((this->wpoxedge)*UNITFAD).val();
  BSIM3ppoxedge = ((this->ppoxedge)*UNITFAD).val();
  BSIM3dlcig = ((this->dlcig)*UNITFAD).val();
  BSIM3aigs = ((this->aigs)*UNITFAD).val();
  BSIM3waigs = ((this->waigs)*UNITFAD).val();
  BSIM3laigs = ((this->laigs)*UNITFAD).val();
  BSIM3paigs = ((this->paigs)*UNITFAD).val();
  BSIM3bigs = ((this->bigs)*UNITFAD).val();
  BSIM3wbigs = ((this->wbigs)*UNITFAD).val();
  BSIM3lbigs = ((this->lbigs)*UNITFAD).val();
  BSIM3pbigs = ((this->pbigs)*UNITFAD).val();
  BSIM3cigs = ((this->cigs)*UNITFAD).val();
  BSIM3wcigs = ((this->wcigs)*UNITFAD).val();
  BSIM3lcigs = ((this->lcigs)*UNITFAD).val();
  BSIM3pcigs = ((this->pcigs)*UNITFAD).val();
  BSIM3aigd = ((this->aigd)*UNITFAD).val();
  BSIM3waigd = ((this->waigd)*UNITFAD).val();
  BSIM3laigd = ((this->laigd)*UNITFAD).val();
  BSIM3paigd = ((this->paigd)*UNITFAD).val();
  BSIM3bigd = ((this->bigd)*UNITFAD).val();
  BSIM3wbigd = ((this->wbigd)*UNITFAD).val();
  BSIM3lbigd = ((this->lbigd)*UNITFAD).val();
  BSIM3pbigd = ((this->pbigd)*UNITFAD).val();
  BSIM3cigd = ((this->cigd)*UNITFAD).val();
  BSIM3wcigd = ((this->wcigd)*UNITFAD).val();
  BSIM3lcigd = ((this->lcigd)*UNITFAD).val();
  BSIM3pcigd = ((this->pcigd)*UNITFAD).val();
  BSIM3aigsd = ((this->aigsd)*UNITFAD).val();
  BSIM3laigsd = ((this->laigsd)*UNITFAD).val();
  BSIM3waigsd = ((this->waigsd)*UNITFAD).val();
  BSIM3paigsd = ((this->paigsd)*UNITFAD).val();
  BSIM3bigsd = ((this->bigsd)*UNITFAD).val();
  BSIM3lbigsd = ((this->lbigsd)*UNITFAD).val();
  BSIM3wbigsd = ((this->wbigsd)*UNITFAD).val();
  BSIM3pbigsd = ((this->pbigsd)*UNITFAD).val();
  BSIM3cigsd = ((this->cigsd)*UNITFAD).val();
  BSIM3lcigsd = ((this->lcigsd)*UNITFAD).val();
  BSIM3wcigsd = ((this->wcigsd)*UNITFAD).val();
  BSIM3pcigsd = ((this->pcigsd)*UNITFAD).val();
  BSIM3nigbacc = ((this->nigbacc)*UNITFAD).val();
  BSIM3lnigbacc = ((this->lnigbacc)*UNITFAD).val();
  BSIM3wnigbacc = ((this->wnigbacc)*UNITFAD).val();
  BSIM3pnigbacc = ((this->pnigbacc)*UNITFAD).val();
  BSIM3agisl = ((this->agisl)*UNITFAD).val();
  BSIM3wagisl = ((this->wagisl)*UNITFAD).val();
  BSIM3lagisl = ((this->lagisl)*UNITFAD).val();
  BSIM3pagisl = ((this->pagisl)*UNITFAD).val();
  BSIM3bgisl = ((this->bgisl)*UNITFAD).val();
  BSIM3wbgisl = ((this->wbgisl)*UNITFAD).val();
  BSIM3lbgisl = ((this->lbgisl)*UNITFAD).val();
  BSIM3pbgisl = ((this->pbgisl)*UNITFAD).val();
  BSIM3cgisl = ((this->cgisl)*UNITFAD).val();
  BSIM3wcgisl = ((this->wcgisl)*UNITFAD).val();
  BSIM3lcgisl = ((this->lcgisl)*UNITFAD).val();
  BSIM3pcgisl = ((this->pcgisl)*UNITFAD).val();
  BSIM3egisl = ((this->egisl)*UNITFAD).val();
  BSIM3wegisl = ((this->wegisl)*UNITFAD).val();
  BSIM3legisl = ((this->legisl)*UNITFAD).val();
  BSIM3pegisl = ((this->pegisl)*UNITFAD).val();
  BSIM3rgisl = ((this->rgisl)*UNITFAD).val();
  BSIM3wrgisl = ((this->wrgisl)*UNITFAD).val();
  BSIM3lrgisl = ((this->lrgisl)*UNITFAD).val();
  BSIM3prgisl = ((this->prgisl)*UNITFAD).val();
  BSIM3kgisl = ((this->kgisl)*UNITFAD).val();
  BSIM3wkgisl = ((this->wkgisl)*UNITFAD).val();
  BSIM3lkgisl = ((this->lkgisl)*UNITFAD).val();
  BSIM3pkgisl = ((this->pkgisl)*UNITFAD).val();
  BSIM3fgisl = ((this->fgisl)*UNITFAD).val();
  BSIM3wfgisl = ((this->wfgisl)*UNITFAD).val();
  BSIM3lfgisl = ((this->lfgisl)*UNITFAD).val();
  BSIM3pfgisl = ((this->pfgisl)*UNITFAD).val();
  BSIM3rgidl = ((this->rgidl)*UNITFAD).val();
  BSIM3wrgidl = ((this->wrgidl)*UNITFAD).val();
  BSIM3lrgidl = ((this->lrgidl)*UNITFAD).val();
  BSIM3prgidl = ((this->prgidl)*UNITFAD).val();
  BSIM3kgidl = ((this->kgidl)*UNITFAD).val();
  BSIM3wkgidl = ((this->wkgidl)*UNITFAD).val();
  BSIM3lkgidl = ((this->lkgidl)*UNITFAD).val();
  BSIM3pkgidl = ((this->pkgidl)*UNITFAD).val();
  BSIM3fgidl = ((this->fgidl)*UNITFAD).val();
  BSIM3wfgidl = ((this->wfgidl)*UNITFAD).val();
  BSIM3lfgidl = ((this->lfgidl)*UNITFAD).val();
  BSIM3pfgidl = ((this->pfgidl)*UNITFAD).val();
  BSIM3calcacm = ((this->calcacm)*UNITFAD).val();
  BSIM3bvs = ((this->bvs)*UNITFAD).val();
  BSIM3bvd = ((this->bvd)*UNITFAD).val();
  BSIM3ijthsrev = ((this->ijthsrev)*UNITFAD).val();
  BSIM3ijthdrev = ((this->ijthdrev)*UNITFAD).val();
  BSIM3xjbvs = ((this->xjbvs)*UNITFAD).val();
  BSIM3xjbvd = ((this->xjbvd)*UNITFAD).val();
  BSIM3vgslim = ((this->vgslim)*UNITFAD).val();
  BSIM3acm = ((this->acm)*UNITFAD).val();
  BSIM3stiMod = ((this->stimod)*UNITFAD).val();
  BSIM3sa = ((this->sa)*UNITFAD).val();
  BSIM3sa0 = ((this->saref)*UNITFAD).val();
  BSIM3sb0 = ((this->sbref)*UNITFAD).val();
  BSIM3sb = ((this->sb)*UNITFAD).val();
  BSIM3mulu0 = ((this->mulu0)*UNITFAD).val();
  BSIM3mulua = ((this->mulua)*UNITFAD).val();
  BSIM3mulub = ((this->mulub)*UNITFAD).val();
  BSIM3binflag = ((this->binflag)*UNITFAD).val();
  BSIM3breakmod = ((this->breakmod)*UNITFAD).val();
  BSIM3wlod = ((this->wlod)*UNITFAD).val();
  BSIM3ku0 = ((this->ku0)*UNITFAD).val();
  BSIM3kvsat = ((this->kvsat)*UNITFAD).val();
  BSIM3kvth0 = ((this->kvth0)*UNITFAD).val();
  BSIM3tku0 = ((this->tku0)*UNITFAD).val();
  BSIM3llodku0 = ((this->llodku0)*UNITFAD).val();
  BSIM3wlodku0 = ((this->wlodku0)*UNITFAD).val();
  BSIM3llodvth = ((this->llodvth)*UNITFAD).val();
  BSIM3wlodvth = ((this->wlodvth)*UNITFAD).val();
  BSIM3lku0 = ((this->lku0)*UNITFAD).val();
  BSIM3wku0 = ((this->wku0)*UNITFAD).val();
  BSIM3pku0 = ((this->pku0)*UNITFAD).val();
  BSIM3lkvth0 = ((this->lkvth0)*UNITFAD).val();
  BSIM3wkvth0 = ((this->wkvth0)*UNITFAD).val();
  BSIM3pkvth0 = ((this->pkvth0)*UNITFAD).val();
  BSIM3stk2 = ((this->stk2)*UNITFAD).val();
  BSIM3lodk2 = ((this->lodk2)*UNITFAD).val();
  BSIM3steta0 = ((this->steta0)*UNITFAD).val();
  BSIM3lodeta0 = ((this->lodeta0)*UNITFAD).val();
  BSIM3sa1 = ((this->sa1)*UNITFAD).val();
  BSIM3sa2 = ((this->sa2)*UNITFAD).val();
  BSIM3sa3 = ((this->sa3)*UNITFAD).val();
  BSIM3sa4 = ((this->sa4)*UNITFAD).val();
  BSIM3sa5 = ((this->sa5)*UNITFAD).val();
  BSIM3sa6 = ((this->sa6)*UNITFAD).val();
  BSIM3sa7 = ((this->sa7)*UNITFAD).val();
  BSIM3sa8 = ((this->sa8)*UNITFAD).val();
  BSIM3sa9 = ((this->sa9)*UNITFAD).val();
  BSIM3sa10 = ((this->sa10)*UNITFAD).val();
  BSIM3sb1 = ((this->sb1)*UNITFAD).val();
  BSIM3sb2 = ((this->sb2)*UNITFAD).val();
  BSIM3sb3 = ((this->sb3)*UNITFAD).val();
  BSIM3sb4 = ((this->sb4)*UNITFAD).val();
  BSIM3sb5 = ((this->sb5)*UNITFAD).val();
  BSIM3sb6 = ((this->sb6)*UNITFAD).val();
  BSIM3sb7 = ((this->sb7)*UNITFAD).val();
  BSIM3sb8 = ((this->sb8)*UNITFAD).val();
  BSIM3sb9 = ((this->sb9)*UNITFAD).val();
  BSIM3sb10 = ((this->sb10)*UNITFAD).val();
  BSIM3sw1 = ((this->sw1)*UNITFAD).val();
  BSIM3sw2 = ((this->sw2)*UNITFAD).val();
  BSIM3sw3 = ((this->sw3)*UNITFAD).val();
  BSIM3sw4 = ((this->sw4)*UNITFAD).val();
  BSIM3sw5 = ((this->sw5)*UNITFAD).val();
  BSIM3sw6 = ((this->sw6)*UNITFAD).val();
  BSIM3sw7 = ((this->sw7)*UNITFAD).val();
  BSIM3sw8 = ((this->sw8)*UNITFAD).val();
  BSIM3sw9 = ((this->sw9)*UNITFAD).val();
  BSIM3sw10 = ((this->sw10)*UNITFAD).val();
  if(((BSIM3sa1 == -9.9999e-99) && (BSIM3sa == -9.9999e-99))) {
    {
      BSIM3sa = ((0.0)*UNITFAD).val();
      BSIM3sa1 = ((0.0)*UNITFAD).val();
    }
  }
  else if(((BSIM3sa1 == -9.9999e-99) && (BSIM3sa != -9.9999e-99))) {
    BSIM3sa1 = ((BSIM3sa)*UNITFAD).val();
  }
  else if((BSIM3sa1 != -9.9999e-99)) {
    BSIM3sa = ((BSIM3sa1)*UNITFAD).val();
  }
  if(((BSIM3sb1 == -9.9999e-99) && (BSIM3sb == -9.9999e-99))) {
    {
      BSIM3sb = ((0.0)*UNITFAD).val();
      BSIM3sb1 = ((0.0)*UNITFAD).val();
    }
  }
  else if(((BSIM3sb1 == -9.9999e-99) && (BSIM3sb != -9.9999e-99))) {
    BSIM3sb1 = ((BSIM3sb)*UNITFAD).val();
  }
  else if((BSIM3sb1 != -9.9999e-99)) {
    BSIM3sb = ((BSIM3sb1)*UNITFAD).val();
  }
  if((BSIM3mulu0 <= 0.0)) {
    BSIM3mulu0 = ((1.0)*UNITFAD).val();
  }
  if((BSIM3mulua <= 0.0)) {
    BSIM3mulua = ((1.0)*UNITFAD).val();
  }
  if((BSIM3mulub <= 0.0)) {
    BSIM3mulub = ((1.0)*UNITFAD).val();
  }
  if((BSIM3vgslim < 0.0)) {
    BSIM3vgslim = ((0.0)*UNITFAD).val();
  }
  else if(((BSIM3vgslim > 0.0) && (BSIM3vgslim < 5.0))) {
    BSIM3vgslim = ((5.0)*UNITFAD).val();
  }
  if((BSIM3acm == -9999999)) {
    {
      if((this->level == 53)) {
        BSIM3acm = ((10)*UNITFAD).val();
      }
      else {
        BSIM3acm = ((0)*UNITFAD).val();
      }
    }
  }
  if((BSIM3calcacm == -9999999)) {
    BSIM3calcacm = ((0)*UNITFAD).val();
  }
  if(((BSIM3acm < 3.1) || ((BSIM3calcacm > 0) && ((BSIM3acm < 12.1) && (BSIM3acm > 11.9))))) {
    BSIM3calcacm = ((1)*UNITFAD).val();
  }
  else {
    BSIM3calcacm = ((0)*UNITFAD).val();
  }
  if(((BSIM3breakmod != 0.0) && (BSIM3breakmod != 1.0))) {
    {
      BSIM3breakmod = ((0.0)*UNITFAD).val();
      printf("Warning:  breakmod is reset to 0.0 \n");
    }
  }
  if((BSIM3bvs < 0.0)) {
    {
      BSIM3bvs = ((0.0)*UNITFAD).val();
      printf("Warning:  bvs is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3bvd < 0.0)) {
    {
      BSIM3bvd = ((0.0)*UNITFAD).val();
      printf("Warning:  bvd is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3ijthsrev < 0.0)) {
    {
      BSIM3ijthsrev = ((0.0)*UNITFAD).val();
      printf("Warning:  ijthsrev is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3ijthdrev < 0.0)) {
    {
      BSIM3ijthdrev = ((0.0)*UNITFAD).val();
      printf("Warning:  ijthdrev is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3xjbvs < 0.0)) {
    {
      BSIM3xjbvs = ((0.0)*UNITFAD).val();
      printf("Warning:  xjbvs is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3xjbvd < 0.0)) {
    {
      BSIM3xjbvd = ((0.0)*UNITFAD).val();
      printf("Warning:  xjbvd is negative, reset to 0.0 \n");
    }
  }
  if((BSIM3tox == -9.9999e-99)) {
    BSIM3tox = ((150.0e-10)*UNITFAD).val();
  }
  else {
    {
      if((BSIM3tox < 5e-10)) {
        BSIM3tox = ((5e-10)*UNITFAD).val();
      }
    }
  }
  if((BSIM3ckappa == 0.0)) {
    BSIM3ckappa = ((0.6)*UNITFAD).val();
  }
  if((this->em == -9.9999e-99)) {
    BSIM3em = ((4.1e7)*UNITFAD).val();
  }
  else {
    BSIM3em = ((this->em)*UNITFAD).val();
  }
  if((this->vth0 == -9.9999e-99)) {
    {
      BSIM3vth0Given = ((0)*UNITFAD).val();
      BSIM3vth0 = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.7,-0.7))*UNITFAD).val();
    }
  }
  else {
    BSIM3vth0Given = ((1)*UNITFAD).val();
  }
  if((BSIM3xt == -9.9999e-99)) {
    {
      BSIM3xtGiven = ((0)*UNITFAD).val();
      BSIM3xt = ((1.55e-7)*UNITFAD).val();
    }
  }
  else {
    BSIM3xtGiven = ((1)*UNITFAD).val();
  }
  if((this->nch == -9.9999e-99)) {
    {
      BSIM3npeakGiven = ((0)*UNITFAD).val();
      BSIM3npeak = ((1.7e17)*UNITFAD).val();
    }
  }
  else {
    BSIM3npeakGiven = ((1)*UNITFAD).val();
  }
  if((BSIM3npeak > 1.0e20)) {
    BSIM3npeak = (((BSIM3npeak*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3lnpeak > 1.0e20)) {
    BSIM3lnpeak = (((BSIM3lnpeak*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3wnpeak > 1.0e20)) {
    BSIM3wnpeak = (((BSIM3wnpeak*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3pnpeak > 1.0e20)) {
    BSIM3pnpeak = (((BSIM3pnpeak*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3ngate > 1.0e23)) {
    BSIM3ngate = (((BSIM3ngate*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3lngate > 1.0e23)) {
    BSIM3lngate = (((BSIM3lngate*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3wngate > 1.0e23)) {
    BSIM3wngate = (((BSIM3wngate*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3pngate > 1.0e23)) {
    BSIM3pngate = (((BSIM3pngate*1.0e-6))*UNITFAD).val();
  }
  if((BSIM3gamma2 == -9.9999e-99)) {
    {
      BSIM3gamma2Given = ((0)*UNITFAD).val();
      BSIM3gamma2 = ((0.0)*UNITFAD).val();
    }
  }
  else {
    BSIM3gamma2Given = ((0)*UNITFAD).val();
  }
  if((BSIM3vbx == -9.9999e-99)) {
    {
      BSIM3vbxGiven = ((0)*UNITFAD).val();
      BSIM3vbx = ((0.0)*UNITFAD).val();
    }
  }
  else {
    BSIM3vbxGiven = ((0)*UNITFAD).val();
  }
  if((BSIM3nsub == -9.9999e-99)) {
    {
      BSIM3nsubGiven = ((0)*UNITFAD).val();
      BSIM3nsub = ((6.0e16)*UNITFAD).val();
    }
  }
  else {
    BSIM3nsubGiven = ((1)*UNITFAD).val();
  }
  if((this->uc == -9.9999e-99)) {
    {
      BSIM3uc = ((cogenda_ternary_op<CogendaFadType>((BSIM3mobMod == 3),-0.0465,-0.0465e-9))*UNITFAD).val();
    }
  }
  if((this->uc1 == -9.9999e-99)) {
    {
      BSIM3uc1 = ((cogenda_ternary_op<CogendaFadType>((BSIM3mobMod == 3),-0.056,-0.056e-9))*UNITFAD).val();
    }
  }
  if((this->u0 == -9.9999e-99)) {
    {
      BSIM3u0 = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.067,0.025))*UNITFAD).val();
    }
  }
  if((this->gamma1 == -9.9999e-99)) {
    {
      BSIM3gamma1Given = ((0)*UNITFAD).val();
      BSIM3gamma1 = ((0.0)*UNITFAD).val();
    }
  }
  else {
    BSIM3gamma1Given = ((1)*UNITFAD).val();
  }
  if((this->k1 == -9.9999e-99)) {
    {
      BSIM3k1Given = ((0)*UNITFAD).val();
      BSIM3k1 = ((0.0)*UNITFAD).val();
    }
  }
  else {
    BSIM3k1Given = ((1)*UNITFAD).val();
  }
  if((this->k2 == -9.9999e-99)) {
    {
      BSIM3k2Given = ((0)*UNITFAD).val();
      BSIM3k2 = ((0.0)*UNITFAD).val();
    }
  }
  else {
    BSIM3k2Given = ((1)*UNITFAD).val();
  }
  BSIM3cox = (((3.453133e-11/BSIM3tox))*UNITFAD).val();
  Cox = ((BSIM3cox)*UNITFAD).val();
  if((BSIM3cf == -9.9999e-99)) {
    BSIM3cf = (((((2.0*3.453133e-11)/3.14159265358979323846)*log((1.0+(0.4e-6/BSIM3tox)))))*UNITFAD).val();
  }
  if((BSIM3dlc == -9.9999e-99)) {
    {
      BSIM3dlcGiven = ((0)*UNITFAD).val();
      BSIM3dlc = ((this->lint)*UNITFAD).val();
    }
  }
  else {
    BSIM3dlcGiven = ((1)*UNITFAD).val();
  }
  if((BSIM3cgdo == -9.9999e-99)) {
    {
      if((BSIM3dlcGiven && (BSIM3dlc > 0.0))) {
        BSIM3cgdo = ((((BSIM3dlc*BSIM3cox)-BSIM3cgdl))*UNITFAD).val();
      }
      else {
        BSIM3cgdo = ((((0.6*BSIM3xj)*BSIM3cox))*UNITFAD).val();
      }
    }
  }
  if((BSIM3cgso == -9.9999e-99)) {
    {
      if((BSIM3dlcGiven && (BSIM3dlc > 0.0))) {
        BSIM3cgso = ((((BSIM3dlc*BSIM3cox)-BSIM3cgsl))*UNITFAD).val();
      }
      else {
        BSIM3cgso = ((((0.6*BSIM3xj)*BSIM3cox))*UNITFAD).val();
      }
    }
  }
  if((BSIM3cgbo == -9.9999e-99)) {
    BSIM3cgbo = ((((2.0*BSIM3dwc)*BSIM3cox))*UNITFAD).val();
  }
  if((BSIM3xpart == -9.9999e-99)) {
    {
      if((this->level == 53)) {
        BSIM3xpart = ((0.0)*UNITFAD).val();
      }
      else {
        {
          if((this->version > 3.031)) {
            BSIM3xpart = ((1.0)*UNITFAD).val();
          }
          else {
            BSIM3xpart = ((0.0)*UNITFAD).val();
          }
        }
      }
    }
  }
  BSIM3agidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3agidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lagidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wagidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pagidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pagidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3egidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3egidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3legidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3legidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wegidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wegidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pegidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pegidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3nigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3nigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lnigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wnigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pnigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pnigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigcGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3eigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3eigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3leigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3leigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3weigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3weigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3peigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3peigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3nigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3nigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lnigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wnigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pnigbinvGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pnigbinv == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3toxeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3toxe == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3vtm0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3vtm0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3toxrefGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3toxref == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3ntox == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lntox == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wntox == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pntoxGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pntox == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pigcd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lpigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lpigcd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wpigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wpigcd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ppigcdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3ppigcd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3nsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3nsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lnsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wnsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pnsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pnsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3vfbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3vfb == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3vfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3vfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3tvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3tvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ltvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3ltvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wtvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wtvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ptvfbsdoffGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3ptvfbsdoff == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3poxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3poxedge == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lpoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lpoxedge == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wpoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wpoxedge == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ppoxedgeGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3ppoxedge == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3dlcigGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3dlcig == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigsGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigs == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3aigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3aigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3laigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3laigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3waigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3waigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3paigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3paigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcigsdGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcigsd == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3nigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3nigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lnigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wnigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pnigbaccGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pnigbacc == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3agislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3agisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wagisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lagisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pagislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pagisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3bgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3bgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wbgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lbgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pbgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pbgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3cgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3cgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wcgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lcgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pcgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pcgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3egislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3egisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wegislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wegisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3legislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3legisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pegislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pegisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3rgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3rgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wrgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wrgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lrgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lrgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3prgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3prgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3kgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3kgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wkgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lkgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pkgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pkgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3fgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3fgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wfgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lfgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pfgislGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pfgisl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3rgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3rgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wrgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wrgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lrgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lrgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3prgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3prgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3kgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3kgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wkgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lkgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pkgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pkgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3fgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3fgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wfgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3lfgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pfgidlGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3pfgidl == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wlodGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wlod == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3ku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3ku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3kvsatGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3kvsat == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3kvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3kvth0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3tku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3tku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3llodku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3llodku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wlodku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3wlodku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3llodvthGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3llodvth == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wlodvthGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3wlodvth == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3lku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3wku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pku0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3pku0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3lkvth0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3wkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3wkvth0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3pkvth0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3pkvth0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3stk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3stk2 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lodk2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3lodk2 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3steta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3steta0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3lodeta0Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3lodeta0 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3saGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sbGiven = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa1 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa2 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa3 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa4 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa5 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa6Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa6 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa7Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa7 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa8Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa8 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa9Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa9 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sa10Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sa10 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb1 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb2 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb3 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb4 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb5 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb6Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb6 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb7Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb7 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb8Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb8 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb9Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb9 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sb10Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sb10 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw1Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw1 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw2Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw2 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw3Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw3 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw4Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw4 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw5Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw5 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw6Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw6 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw7Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw7 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw8Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw8 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw9Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw9 == -9.9999e-99),0,1))*UNITFAD).val();
  BSIM3sw10Given = ((cogenda_ternary_op<CogendaFadType>((BSIM3sw10 == -9.9999e-99),0,1))*UNITFAD).val();
  if(!BSIM3toxeGiven) {
    BSIM3toxe = ((BSIM3tox)*UNITFAD).val();
  }
  if(!BSIM3vtm0Given) {
    BSIM3vtm0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3toxrefGiven) {
    BSIM3toxref = ((30.0e-10)*UNITFAD).val();
  }
  if(!BSIM3nsdGiven) {
    BSIM3nsd = ((1.0e20)*UNITFAD).val();
  }
  if(!BSIM3lnsdGiven) {
    BSIM3lnsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wnsdGiven) {
    BSIM3wnsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pnsdGiven) {
    BSIM3pnsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3vfbsdoffGiven) {
    BSIM3vfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lvfbsdoffGiven) {
    BSIM3lvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wvfbsdoffGiven) {
    BSIM3wvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pvfbsdoffGiven) {
    BSIM3pvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3tvfbsdoffGiven) {
    BSIM3tvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3ltvfbsdoffGiven) {
    BSIM3ltvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wtvfbsdoffGiven) {
    BSIM3wtvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3ptvfbsdoffGiven) {
    BSIM3ptvfbsdoff = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3dlcigGiven) {
    BSIM3dlcig = ((BSIM3Lint)*UNITFAD).val();
  }
  if(!BSIM3agislGiven) {
    BSIM3agisl = ((BSIM3agidl)*UNITFAD).val();
  }
  if(!BSIM3wagislGiven) {
    BSIM3wagisl = ((BSIM3wagidl)*UNITFAD).val();
  }
  if(!BSIM3lagislGiven) {
    BSIM3lagisl = ((BSIM3lagidl)*UNITFAD).val();
  }
  if(!BSIM3pagislGiven) {
    BSIM3pagisl = ((BSIM3pagidl)*UNITFAD).val();
  }
  if(!BSIM3bgislGiven) {
    BSIM3bgisl = ((BSIM3bgidl)*UNITFAD).val();
  }
  if(!BSIM3wbgislGiven) {
    BSIM3wbgisl = ((BSIM3wbgidl)*UNITFAD).val();
  }
  if(!BSIM3lbgislGiven) {
    BSIM3lbgisl = ((BSIM3lbgidl)*UNITFAD).val();
  }
  if(!BSIM3pbgislGiven) {
    BSIM3pbgisl = ((BSIM3pbgidl)*UNITFAD).val();
  }
  if(!BSIM3cgislGiven) {
    BSIM3cgisl = ((BSIM3cgidl)*UNITFAD).val();
  }
  if(!BSIM3wcgislGiven) {
    BSIM3wcgisl = ((BSIM3wcgidl)*UNITFAD).val();
  }
  if(!BSIM3lcgislGiven) {
    BSIM3lcgisl = ((BSIM3lcgidl)*UNITFAD).val();
  }
  if(!BSIM3pcgislGiven) {
    BSIM3pcgisl = ((BSIM3pcgidl)*UNITFAD).val();
  }
  if(!BSIM3egislGiven) {
    BSIM3egisl = ((BSIM3egidl)*UNITFAD).val();
  }
  if(!BSIM3wegislGiven) {
    BSIM3wegisl = ((BSIM3wegidl)*UNITFAD).val();
  }
  if(!BSIM3legislGiven) {
    BSIM3legisl = ((BSIM3legidl)*UNITFAD).val();
  }
  if(!BSIM3pegislGiven) {
    BSIM3pegisl = ((BSIM3pegidl)*UNITFAD).val();
  }
  if(!BSIM3rgislGiven) {
    BSIM3rgisl = ((BSIM3rgidl)*UNITFAD).val();
  }
  if(!BSIM3wrgislGiven) {
    BSIM3wrgisl = ((BSIM3wrgidl)*UNITFAD).val();
  }
  if(!BSIM3lrgislGiven) {
    BSIM3lrgisl = ((BSIM3lrgidl)*UNITFAD).val();
  }
  if(!BSIM3prgislGiven) {
    BSIM3prgisl = ((BSIM3prgidl)*UNITFAD).val();
  }
  if(!BSIM3kgislGiven) {
    BSIM3kgisl = ((BSIM3kgidl)*UNITFAD).val();
  }
  if(!BSIM3wkgislGiven) {
    BSIM3wkgisl = ((BSIM3wkgidl)*UNITFAD).val();
  }
  if(!BSIM3lkgislGiven) {
    BSIM3lkgisl = ((BSIM3lkgidl)*UNITFAD).val();
  }
  if(!BSIM3pkgislGiven) {
    BSIM3pkgisl = ((BSIM3pkgidl)*UNITFAD).val();
  }
  if(!BSIM3fgislGiven) {
    BSIM3fgisl = ((BSIM3fgidl)*UNITFAD).val();
  }
  if(!BSIM3wfgislGiven) {
    BSIM3wfgisl = ((BSIM3wfgidl)*UNITFAD).val();
  }
  if(!BSIM3lfgislGiven) {
    BSIM3lfgisl = ((BSIM3lfgidl)*UNITFAD).val();
  }
  if(!BSIM3pfgislGiven) {
    BSIM3pfgisl = ((BSIM3pfgidl)*UNITFAD).val();
  }
  if(!BSIM3agidlGiven) {
    BSIM3agidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3bgidlGiven) {
    BSIM3bgidl = ((2.3e9)*UNITFAD).val();
  }
  if(!BSIM3cgidlGiven) {
    BSIM3cgidl = ((0.5)*UNITFAD).val();
  }
  if(!BSIM3egidlGiven) {
    BSIM3egidl = ((0.8)*UNITFAD).val();
  }
  if(!BSIM3aigcGiven) {
    BSIM3aigc = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
  }
  if(!BSIM3bigcGiven) {
    BSIM3bigc = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
  }
  if(!BSIM3cigcGiven) {
    BSIM3cigc = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.075,0.03))*UNITFAD).val();
  }
  if(BSIM3aigsdGiven) {
    {
      BSIM3aigs = ((BSIM3aigsd)*UNITFAD).val();
      BSIM3aigd = ((BSIM3aigsd)*UNITFAD).val();
    }
  }
  else {
    {
      BSIM3aigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
      if(!BSIM3aigsGiven) {
        BSIM3aigs = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
      }
      if(!BSIM3aigdGiven) {
        BSIM3aigd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.36e-2,9.80e-3))*UNITFAD).val();
      }
    }
  }
  if(BSIM3bigsdGiven) {
    {
      BSIM3bigs = ((BSIM3bigsd)*UNITFAD).val();
      BSIM3bigd = ((BSIM3bigsd)*UNITFAD).val();
    }
  }
  else {
    {
      BSIM3bigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
      if(!BSIM3bigsGiven) {
        BSIM3bigs = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
      }
      if(!BSIM3bigdGiven) {
        BSIM3bigd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),1.71e-3,7.59e-4))*UNITFAD).val();
      }
    }
  }
  if(BSIM3cigsdGiven) {
    {
      BSIM3cigs = ((BSIM3cigsd)*UNITFAD).val();
      BSIM3cigd = ((BSIM3cigsd)*UNITFAD).val();
    }
  }
  else {
    {
      BSIM3cigsd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.075,0.03))*UNITFAD).val();
      if(!BSIM3cigsGiven) {
        BSIM3cigs = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.075,0.03))*UNITFAD).val();
      }
      if(!BSIM3cigdGiven) {
        BSIM3cigd = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),0.075,0.03))*UNITFAD).val();
      }
    }
  }
  if(!BSIM3aigbaccGiven) {
    BSIM3aigbacc = ((1.36e-2)*UNITFAD).val();
  }
  if(!BSIM3bigbaccGiven) {
    BSIM3bigbacc = ((1.71e-3)*UNITFAD).val();
  }
  if(!BSIM3cigbaccGiven) {
    BSIM3cigbacc = ((0.075)*UNITFAD).val();
  }
  if(!BSIM3aigbinvGiven) {
    BSIM3aigbinv = ((1.11e-2)*UNITFAD).val();
  }
  if(!BSIM3bigbinvGiven) {
    BSIM3bigbinv = ((9.49e-4)*UNITFAD).val();
  }
  if(!BSIM3cigbinvGiven) {
    BSIM3cigbinv = ((0.006)*UNITFAD).val();
  }
  if(!BSIM3nigcGiven) {
    BSIM3nigc = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3nigbinvGiven) {
    BSIM3nigbinv = ((3.0)*UNITFAD).val();
  }
  if(!BSIM3nigbaccGiven) {
    BSIM3nigbacc = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3ntoxGiven) {
    BSIM3ntox = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3eigbinvGiven) {
    BSIM3eigbinv = ((1.1)*UNITFAD).val();
  }
  if(!BSIM3pigcdGiven) {
    BSIM3pigcd = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3poxedgeGiven) {
    BSIM3poxedge = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3lagidlGiven) {
    BSIM3lagidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lbgidlGiven) {
    BSIM3lbgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lcgidlGiven) {
    BSIM3lcgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3legidlGiven) {
    BSIM3legidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3laigcGiven) {
    BSIM3laigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lbigcGiven) {
    BSIM3lbigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lcigcGiven) {
    BSIM3lcigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3laigsdGiven) {
    BSIM3laigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lbigsdGiven) {
    BSIM3lbigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lcigsdGiven) {
    BSIM3lcigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3laigbaccGiven) {
    BSIM3laigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lbigbaccGiven) {
    BSIM3lbigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lcigbaccGiven) {
    BSIM3lcigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3laigbinvGiven) {
    BSIM3laigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lbigbinvGiven) {
    BSIM3lbigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lcigbinvGiven) {
    BSIM3lcigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lnigcGiven) {
    BSIM3lnigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lnigbinvGiven) {
    BSIM3lnigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lnigbaccGiven) {
    BSIM3lnigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lntoxGiven) {
    BSIM3lntox = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3leigbinvGiven) {
    BSIM3leigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lpigcdGiven) {
    BSIM3lpigcd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lpoxedgeGiven) {
    BSIM3lpoxedge = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wagidlGiven) {
    BSIM3wagidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wbgidlGiven) {
    BSIM3wbgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wcgidlGiven) {
    BSIM3wcgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wegidlGiven) {
    BSIM3wegidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3waigcGiven) {
    BSIM3waigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wbigcGiven) {
    BSIM3wbigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wcigcGiven) {
    BSIM3wcigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3waigsdGiven) {
    BSIM3waigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wbigsdGiven) {
    BSIM3wbigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wcigsdGiven) {
    BSIM3wcigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3waigbaccGiven) {
    BSIM3waigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wbigbaccGiven) {
    BSIM3wbigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wcigbaccGiven) {
    BSIM3wcigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3waigbinvGiven) {
    BSIM3waigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wbigbinvGiven) {
    BSIM3wbigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wcigbinvGiven) {
    BSIM3wcigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wnigcGiven) {
    BSIM3wnigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wnigbinvGiven) {
    BSIM3wnigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wnigbaccGiven) {
    BSIM3wnigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wntoxGiven) {
    BSIM3wntox = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3weigbinvGiven) {
    BSIM3weigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wpigcdGiven) {
    BSIM3wpigcd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wpoxedgeGiven) {
    BSIM3wpoxedge = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pagidlGiven) {
    BSIM3pagidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pbgidlGiven) {
    BSIM3pbgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pcgidlGiven) {
    BSIM3pcgidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pegidlGiven) {
    BSIM3pegidl = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3paigcGiven) {
    BSIM3paigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pbigcGiven) {
    BSIM3pbigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pcigcGiven) {
    BSIM3pcigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3paigsdGiven) {
    BSIM3paigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pbigsdGiven) {
    BSIM3pbigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pcigsdGiven) {
    BSIM3pcigsd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3paigbaccGiven) {
    BSIM3paigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pbigbaccGiven) {
    BSIM3pbigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pcigbaccGiven) {
    BSIM3pcigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3paigbinvGiven) {
    BSIM3paigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pbigbinvGiven) {
    BSIM3pbigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pcigbinvGiven) {
    BSIM3pcigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pnigcGiven) {
    BSIM3pnigc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pnigbinvGiven) {
    BSIM3pnigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pnigbaccGiven) {
    BSIM3pnigbacc = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pntoxGiven) {
    BSIM3pntox = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3peigbinvGiven) {
    BSIM3peigbinv = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3ppigcdGiven) {
    BSIM3ppigcd = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3ppoxedgeGiven) {
    BSIM3ppoxedge = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wlodGiven) {
    BSIM3wlod = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3ku0Given) {
    BSIM3ku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3kvsatGiven) {
    BSIM3kvsat = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3kvth0Given) {
    BSIM3kvth0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3tku0Given) {
    BSIM3tku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3llodku0Given) {
    BSIM3llodku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wlodku0Given) {
    BSIM3wlodku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3llodvthGiven) {
    BSIM3llodvth = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wlodvthGiven) {
    BSIM3wlodvth = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lku0Given) {
    BSIM3lku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wku0Given) {
    BSIM3wku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pku0Given) {
    BSIM3pku0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lkvth0Given) {
    BSIM3lkvth0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3wkvth0Given) {
    BSIM3wkvth0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3pkvth0Given) {
    BSIM3pkvth0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3stk2Given) {
    BSIM3stk2 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lodk2Given) {
    BSIM3lodk2 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3steta0Given) {
    BSIM3steta0 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3lodeta0Given) {
    BSIM3lodeta0 = ((1.0)*UNITFAD).val();
  }
  if(!BSIM3sa1Given) {
    BSIM3sa1 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa2Given) {
    BSIM3sa2 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa3Given) {
    BSIM3sa3 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa4Given) {
    BSIM3sa4 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa5Given) {
    BSIM3sa5 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa6Given) {
    BSIM3sa6 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa7Given) {
    BSIM3sa7 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa8Given) {
    BSIM3sa8 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa9Given) {
    BSIM3sa9 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sa10Given) {
    BSIM3sa10 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb1Given) {
    BSIM3sb1 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb2Given) {
    BSIM3sb2 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb3Given) {
    BSIM3sb3 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb4Given) {
    BSIM3sb4 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb5Given) {
    BSIM3sb5 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb6Given) {
    BSIM3sb6 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb7Given) {
    BSIM3sb7 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb8Given) {
    BSIM3sb8 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb9Given) {
    BSIM3sb9 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sb10Given) {
    BSIM3sb10 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw2Given) {
    BSIM3sw2 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw3Given) {
    BSIM3sw3 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw4Given) {
    BSIM3sw4 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw5Given) {
    BSIM3sw5 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw6Given) {
    BSIM3sw6 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw7Given) {
    BSIM3sw7 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw8Given) {
    BSIM3sw8 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw9Given) {
    BSIM3sw9 = ((0.0)*UNITFAD).val();
  }
  if(!BSIM3sw10Given) {
    BSIM3sw10 = ((0.0)*UNITFAD).val();
  }
  Igd = ((0.0)*UNITFAD).val();
  Igs = ((0.0)*UNITFAD).val();
  Voxacc = ((0.0)*UNITFAD).val();
  Voxdepinv = ((0.0)*UNITFAD).val();
  xmult = ((1.0)*UNITFAD).val();
  if((BSIM3bulkJctPotential < 0.1)) {
    {
      BSIM3bulkJctPotential = ((0.1)*UNITFAD).val();
      printf("\nWarning: Given pb is less than 0.1. Pb is set to 0.1.\n");
    }
  }
  if((BSIM3sidewallJctPotential < 0.1)) {
    {
      BSIM3sidewallJctPotential = ((0.1)*UNITFAD).val();
      printf("\nWarning: Given pbsw is less than 0.1. Pbsw is set to 0.1.\n");
    }
  }
  if((BSIM3GatesidewallJctPotential < 0.1)) {
    {
      BSIM3GatesidewallJctPotential = ((0.1)*UNITFAD).val();
      printf("\nWarning: Given pbswg is less than 0.1. Pbswg is set to 0.1.\n");
    }
  }
  T = ((_TEMPER_)*UNITFAD).val();
  TRatio = (((T/BSIM3tnom))*UNITFAD).val();
  BSIM3factor1 = ((cogenda_ternary_op<CogendaFadType>((((1.03594e-10/3.453133e-11)*BSIM3tox) > 0.0),sqrt(((1.03594e-10/3.453133e-11)*BSIM3tox)),0.0))*UNITFAD).val();
  Vtm0 = (((8.617087e-5*BSIM3tnom))*UNITFAD).val();
  Eg0 = (((1.16-(((7.02e-4*BSIM3tnom)*BSIM3tnom)/(BSIM3tnom+1108.0))))*UNITFAD).val();
  ni = (((((1.45e10*(BSIM3tnom/300.15))*cogenda_ternary_op<CogendaFadType>(((BSIM3tnom/300.15) > 0.0),sqrt((BSIM3tnom/300.15)),0.0))*exp((21.5565981-(Eg0/(2.0*Vtm0))))))*UNITFAD).val();
  BSIM3vtm = (((8.617087e-5*T))*UNITFAD).val();
  Eg = (((1.16-(((7.02e-4*T)*T)/(T+1108.0))))*UNITFAD).val();
  if((T != BSIM3tnom)) {
    {
      T0 = (((((Eg0/Vtm0)-(Eg/BSIM3vtm))+(BSIM3jctTempExponent*log((T/BSIM3tnom)))))*UNITFAD).val();
      T1 = ((exp((T0/BSIM3jctEmissionCoeff)))*UNITFAD).val();
      BSIM3jctTempSatCurDensity = (((BSIM3jctSatCurDensity*T1))*UNITFAD).val();
      BSIM3jctSidewallTempSatCurDensity = (((BSIM3jctSidewallSatCurDensity*T1))*UNITFAD).val();
    }
  }
  else {
    {
      BSIM3jctTempSatCurDensity = ((BSIM3jctSatCurDensity)*UNITFAD).val();
      BSIM3jctSidewallTempSatCurDensity = ((BSIM3jctSidewallSatCurDensity)*UNITFAD).val();
    }
  }
  if((BSIM3jctTempSatCurDensity < 0.0)) {
    BSIM3jctTempSatCurDensity = ((0.0)*UNITFAD).val();
  }
  if((BSIM3jctSidewallTempSatCurDensity < 0.0)) {
    BSIM3jctSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
  }
  if((this->ad == -9.9999e-99)) {
    BSIM3drainArea = ((0.0)*UNITFAD).val();
  }
  if((this->as == -9.9999e-99)) {
    BSIM3sourceArea = ((0.0)*UNITFAD).val();
  }
  if((this->pd == -9.9999e-99)) {
    BSIM3drainPerimeter = ((0.0)*UNITFAD).val();
  }
  if((this->ps == -9.9999e-99)) {
    BSIM3sourcePerimeter = ((0.0)*UNITFAD).val();
  }
  if((this->nrd == -9.9999e-99)) {
    BSIM3drainSquares = ((0.0)*UNITFAD).val();
  }
  if((this->nrs == -9.9999e-99)) {
    BSIM3sourceSquares = ((0.0)*UNITFAD).val();
  }
  arg = (((this->wmlt*xmult))*UNITFAD).val();
  Leff = (((((this->l*this->lmlt)+this->xl)-((this->ld+this->del)*2.0)))*UNITFAD).val();
  Weff = ((((this->w*this->wmlt)+(this->xw*2.0)))*UNITFAD).val();
  here_w = (((Weff-(this->wd*2.0)))*UNITFAD).val();
  if((here_w == 0.0)) {
    here_w = ((1.0e-12)*UNITFAD).val();
  }
  rseff = ((this->rs)*UNITFAD).val();
  rdeff = ((this->rd)*UNITFAD).val();
  if(((((((this->wrs != 0.0) || (this->lrs != 0.0)) || (this->prs != 0.0)) || (this->wrd != 0.0 
      	)) || (this->lrd != 0.0)) || (this->prd != 0.0))) {
    {
      larg = (((((this->lref*this->lmlt)+this->xlref)-((this->ld+this->del)*2.0)))*UNITFAD).val();
      warg = (((((this->wref*this->wmlt)+this->xwref)-(this->wd*2.0)))*UNITFAD).val();
      if((this->lref > 0.0)) {
        T0 = (((1.0/(larg*1e6)))*UNITFAD).val();
      }
      else {
        T0 = ((0.0)*UNITFAD).val();
      }
      if((this->wref > 0.0)) {
        T1 = (((1.0/(warg*1e6)))*UNITFAD).val();
      }
      else {
        T1 = ((0.0)*UNITFAD).val();
      }
      T2 = ((((1.0/(Leff*1e6))-T0))*UNITFAD).val();
      T3 = ((((1.0/(here_w*1e6))-T1))*UNITFAD).val();
      T4 = (((T2*T3))*UNITFAD).val();
      rseff = (((((this->rs+(this->wrs*T3))+(this->lrs*T2))+(this->prs*T4)))*UNITFAD).val();
      rdeff = (((((this->rd+(this->wrd*T3))+(this->lrd*T2))+(this->prd*T4)))*UNITFAD).val();
    }
  }
  switch(BSIM3acm){
  case 0:
  case 10:
    {
      BSIM3drainArea = ((((BSIM3drainArea*arg)*this->wmlt))*UNITFAD).val();
      BSIM3sourceArea = ((((BSIM3sourceArea*arg)*this->wmlt))*UNITFAD).val();
      BSIM3drainPerimeter = (((BSIM3drainPerimeter*arg))*UNITFAD).val();
      BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*arg))*UNITFAD).val();
      rdpr = (((rdeff+this->rdc))*UNITFAD).val();
      rspr = (((rseff+this->rsc))*UNITFAD).val();
    }
    break;
  case 1:
  case 11:
    {
      BSIM3drainArea = (((Weff*this->wmlt))*UNITFAD).val();
      BSIM3sourceArea = (((Weff*this->wmlt))*UNITFAD).val();
      BSIM3drainPerimeter = ((Weff)*UNITFAD).val();
      BSIM3sourcePerimeter = ((Weff)*UNITFAD).val();
      val = ((((this->ld+this->ldif)/Weff))*UNITFAD).val();
      rdpr = ((((val*rdeff)+(BSIM3drainSquares*this->rsh)))*UNITFAD).val();
      rspr = ((((val*rseff)+(BSIM3sourceSquares*this->rsh)))*UNITFAD).val();
      rdpr = (((rdpr+this->rdc))*UNITFAD).val();
      rspr = (((rspr+this->rsc))*UNITFAD).val();
    }
    break;
  case 2:
  case 12:
    {
      T0 = (((this->hdif*this->wmlt))*UNITFAD).val();
      if((this->ad == -9.9999e-99)) {
        {
          BSIM3drainArea = ((((T0+T0)*Weff))*UNITFAD).val();
          BSIM3drainArea = (((BSIM3drainArea*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3drainArea = ((((BSIM3drainArea*arg)*this->wmlt))*UNITFAD).val();
      }
      if((this->as == -9.9999e-99)) {
        {
          BSIM3sourceArea = ((((T0+T0)*Weff))*UNITFAD).val();
          BSIM3sourceArea = (((BSIM3sourceArea*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3sourceArea = ((((BSIM3sourceArea*arg)*this->wmlt))*UNITFAD).val();
      }
      if((this->pd == -9.9999e-99)) {
        {
          BSIM3drainPerimeter = ((((((T0+T0)+T0)+T0)+(Weff+Weff)))*UNITFAD).val();
          BSIM3drainPerimeter = (((BSIM3drainPerimeter*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3drainPerimeter = (((BSIM3drainPerimeter*arg))*UNITFAD).val();
      }
      if((this->ps == -9.9999e-99)) {
        {
          BSIM3sourcePerimeter = ((((((T0+T0)+T0)+T0)+(Weff+Weff)))*UNITFAD).val();
          BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*arg))*UNITFAD).val();
      }
      ecjsw = ((this->cjsw)*UNITFAD).val();
      if((this->cjgate != this->cjsw)) {
        {
          czbdsw = ((((this->cjsw*BSIM3drainPerimeter)+((this->cjgate-this->cjsw)*min(BSIM3drainPerimeter,Weff))))*UNITFAD).val();
          if(((this->cjsw == 0.0) && (this->cjgate > 0.0))) {
            ecjsw = ((this->cjgate)*UNITFAD).val();
          }
          if((ecjsw > 0.0)) {
            BSIM3drainPerimeter = (((czbdsw/ecjsw))*UNITFAD).val();
          }
          czbssw = ((((this->cjsw*BSIM3sourcePerimeter)+((this->cjgate-this->cjsw)*min(BSIM3sourcePerimeter,Weff))))*UNITFAD).val();
          if((ecjsw > 0.0)) {
            BSIM3sourcePerimeter = (((czbssw/ecjsw))*UNITFAD).val();
          }
        }
      }
      val = ((((this->ld+this->ldif)/Weff))*UNITFAD).val();
      if((this->nrd == -9.9999e-99)) {
        {
          BSIM3drainSquares = (((T0/Weff))*UNITFAD).val();
        }
      }
      rdpr = (((((val*rdeff)+(BSIM3drainSquares*this->rsh))+this->rdc))*UNITFAD).val();
      if((this->nrs == -9.9999e-99)) {
        BSIM3sourceSquares = (((T0/Weff))*UNITFAD).val();
      }
      rspr = (((((val*rseff)+(BSIM3sourceSquares*this->rsh))+this->rsc))*UNITFAD).val();
    }
    break;
  case 3:
  case 13:
    {
      T0 = (((this->hdif*this->wmlt))*UNITFAD).val();
      ecjgat = ((this->cjgate)*UNITFAD).val();
      if((this->cjgate != 0.0)) {
        ecjgat = ((((this->cjgate*Weff)*xmult))*UNITFAD).val();
      }
      if((BSIM3drainArea == 0)) {
        {
          if(((this->geo == 0) || (this->geo == 2))) {
            BSIM3drainArea = ((((T0+T0)*Weff))*UNITFAD).val();
          }
          else {
            BSIM3drainArea = (((T0*Weff))*UNITFAD).val();
          }
          BSIM3drainArea = (((BSIM3drainArea*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3drainArea = ((((BSIM3drainArea*arg)*this->wmlt))*UNITFAD).val();
      }
      if((BSIM3sourceArea == 0)) {
        {
          if(((this->geo == 0) || (this->geo == 1))) {
            BSIM3sourceArea = ((((T0+T0)*Weff))*UNITFAD).val();
          }
          else {
            BSIM3sourceArea = (((T0*Weff))*UNITFAD).val();
          }
          BSIM3sourceArea = (((BSIM3sourceArea*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3sourceArea = ((((BSIM3sourceArea*arg)*this->wmlt))*UNITFAD).val();
      }
      if((this->pd == -9.9999e-99)) {
        {
          if(((this->geo == 0) || (this->geo == 2))) {
            BSIM3drainPerimeter = ((((((T0+T0)+T0)+T0)+Weff))*UNITFAD).val();
          }
          else {
            BSIM3drainPerimeter = (((T0+T0))*UNITFAD).val();
          }
          BSIM3drainPerimeter = (((BSIM3drainPerimeter*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3drainPerimeter = (((BSIM3drainPerimeter*arg))*UNITFAD).val();
      }
      if((this->ps == -9.9999e-99)) {
        {
          if(((this->geo == 0) || (this->geo == 1))) {
            BSIM3sourcePerimeter = ((((((T0+T0)+T0)+T0)+Weff))*UNITFAD).val();
          }
          else {
            BSIM3sourcePerimeter = (((T0+T0))*UNITFAD).val();
          }
          BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*xmult))*UNITFAD).val();
        }
      }
      else {
        BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*arg))*UNITFAD).val();
      }
      val = ((((this->ld+this->ldif)/Weff))*UNITFAD).val();
      if((this->nrd == -9.9999e-99)) {
        BSIM3drainSquares = (((T0/Weff))*UNITFAD).val();
      }
      rdpr = (((((val*rdeff)+(BSIM3drainSquares*this->rsh))+this->rdc))*UNITFAD).val();
      if((this->nrs == -9.9999e-99)) {
        BSIM3sourceSquares = (((T0/Weff))*UNITFAD).val();
      }
      rspr = (((((val*rseff)+(BSIM3sourceSquares*this->rsh))+this->rsc))*UNITFAD).val();
    }
    break;
  default:
    {
      BSIM3drainArea = ((((BSIM3drainArea*arg)*this->wmlt))*UNITFAD).val();
      BSIM3sourceArea = ((((BSIM3sourceArea*arg)*this->wmlt))*UNITFAD).val();
      BSIM3drainPerimeter = (((BSIM3drainPerimeter*arg))*UNITFAD).val();
      BSIM3sourcePerimeter = (((BSIM3sourcePerimeter*arg))*UNITFAD).val();
      rdpr = (((rdeff+this->rdc))*UNITFAD).val();
      rspr = (((rseff+this->rsc))*UNITFAD).val();
    }
    break;
  }
  rdpr = ((cogenda_ternary_op<CogendaFadType>((rdpr > 1.0e-28),max(1.0e-5,rdpr),0.0))*UNITFAD).val();
  rspr = ((cogenda_ternary_op<CogendaFadType>((rspr > 1.0e-28),max(1.0e-5,rspr),0.0))*UNITFAD).val();
  if((((BSIM3stiMod != 0) && (BSIM3stiMod != 1)) && (BSIM3stiMod != 2))) {
    BSIM3stiMod = ((0)*UNITFAD).val();
  }
  delTemp = (((T-BSIM3tnom))*UNITFAD).val();
  T0 = (((BSIM3tcj*delTemp))*UNITFAD).val();
  if((T0 >= -1.0)) {
    BSIM3unitAreaTempJctCap = (((BSIM3unitAreaJctCap*(1.0+T0)))*UNITFAD).val();
  }
  else if((BSIM3unitAreaJctCap > 0.0)) {
    {
      BSIM3unitAreaTempJctCap = ((0.0)*UNITFAD).val();
      printf("Temperature effect has caused cj to be negative. \ 
                      Cj is clamped to zero.\n");
    }
  }
  T0 = (((BSIM3tcjsw*delTemp))*UNITFAD).val();
  if((T0 >= -1.0)) {
    BSIM3unitLengthSidewallTempJctCap = (((BSIM3unitLengthSidewallJctCap*(1.0+T0)))*UNITFAD).val();
  }
  else if((BSIM3unitLengthSidewallJctCap > 0.0)) {
    {
      BSIM3unitLengthSidewallTempJctCap = ((0.0)*UNITFAD).val();
      printf("Temperature effect has caused cjsw to be negative. \
                      Cjsw is clamped to zero.\n");
    }
  }
  T0 = (((BSIM3tcjswg*delTemp))*UNITFAD).val();
  if((T0 >= -1.0)) {
    BSIM3unitLengthGateSidewallTempJctCap = ((((1.0+T0)*BSIM3unitLengthGateSidewallJctCap))*UNITFAD).val();
  }
  else if((BSIM3unitLengthGateSidewallJctCap > 0.0)) {
    {
      BSIM3unitLengthGateSidewallTempJctCap = ((0.0)*UNITFAD).val();
      printf("Temperature effect has caused cjswg to be negative. \
                      Cjswg is clamped to zero.\n");
    }
  }
  BSIM3PhiB = (((BSIM3bulkJctPotential-(BSIM3tpb*delTemp)))*UNITFAD).val();
  if((BSIM3PhiB < 0.01)) {
    {
      BSIM3PhiB = ((0.01)*UNITFAD).val();
      printf("Temperature effect has caused pb to be less than 0.01. \
                      Pb is clamped to 0.01.\n");
    }
  }
  BSIM3PhiBSW = (((BSIM3sidewallJctPotential-(BSIM3tpbsw*delTemp)))*UNITFAD).val();
  if((BSIM3PhiBSW <= 0.01)) {
    {
      BSIM3PhiBSW = ((0.01)*UNITFAD).val();
      printf("Temperature effect has caused pbsw to be less than 0.01. \
                      Pbsw is clamped to 0.01.\n");
    }
  }
  BSIM3PhiBSWG = (((BSIM3GatesidewallJctPotential-(BSIM3tpbswg*delTemp)))*UNITFAD).val();
  if((BSIM3PhiBSWG <= 0.01)) {
    {
      BSIM3PhiBSWG = ((0.01)*UNITFAD).val();
      printf("Temperature effect has caused pbswg to be less than 0.01.\
                        Pbswg is clamped to 0.01.\n");
    }
  }
  Lnew = ((this->l)*UNITFAD).val();
  Wnew = ((this->w)*UNITFAD).val();
  Ldrn = (((Lnew-this->xl))*UNITFAD).val();
  Wdrn = (((Wnew-this->xw))*UNITFAD).val();
  if(!BSIM3sw1Given) {
    BSIM3sw1 = ((Wnew)*UNITFAD).val();
  }
  T0 = ((pow(Lnew,BSIM3Lln))*UNITFAD).val();
  T1 = ((pow(Wnew,BSIM3Lwn))*UNITFAD).val();
  tmp1 = (((((BSIM3Ll/T0)+(BSIM3Lw/T1))+(BSIM3Lwl/(T0*T1))))*UNITFAD).val();
  BSIM3dl = (((BSIM3Lint+tmp1))*UNITFAD).val();
  tmp2 = (((((BSIM3Llc/T0)+(BSIM3Lwc/T1))+(BSIM3Lwlc/(T0*T1))))*UNITFAD).val();
  BSIM3dlc = (((BSIM3dlc+tmp2))*UNITFAD).val();
  T2 = ((pow(Lnew,BSIM3Wln))*UNITFAD).val();
  T3 = ((pow(Wnew,BSIM3Wwn))*UNITFAD).val();
  tmp1 = (((((BSIM3Wl/T2)+(BSIM3Ww/T3))+(BSIM3Wwl/(T2*T3))))*UNITFAD).val();
  BSIM3dw = (((BSIM3Wint+tmp1))*UNITFAD).val();
  tmp2 = (((((BSIM3Wlc/T2)+(BSIM3Wwc/T3))+(BSIM3Wwlc/(T2*T3))))*UNITFAD).val();
  BSIM3dwc = (((BSIM3dwc+tmp2))*UNITFAD).val();
  BSIM3leff = (((Lnew-(2.0*BSIM3dl)))*UNITFAD).val();
  BSIM3weff = (((Wnew-(2.0*BSIM3dw)))*UNITFAD).val();
  BSIM3leffCV = (((Lnew-(2.0*BSIM3dlc)))*UNITFAD).val();
  BSIM3weffCV = (((Wnew-(2.0*BSIM3dwc)))*UNITFAD).val();
  if((BSIM3leff <= 0.0)) {
    printf("\nFatal: Effective channel length %g <= 0",((BSIM3leff)*UNITFAD).val());
  }
  if((BSIM3weff <= 0.0)) {
    printf("\nFatal: Effective channel width %g <= 0",((BSIM3weff)*UNITFAD).val());
  }
  if((BSIM3leffCV <= 0.0)) {
    printf("\nFatal: Effective channel length-CV %g <= 0",((BSIM3leffCV)*UNITFAD).val());
  }
  if((BSIM3weffCV <= 0.0)) {
    printf("\nFatal: Effective channel width-CV %g <= 0",((BSIM3weffCV)*UNITFAD).val());
  }
  if(((BSIM3acm < 3.1) || ((BSIM3calcacm > 0) && ((BSIM3acm < 12.1) && (BSIM3acm > 11.9))))) {
  }
  else {
    {
      BSIM3sourceArea = ((this->as)*UNITFAD).val();
      BSIM3drainArea = ((this->ad)*UNITFAD).val();
      BSIM3sourcePerimeter = ((this->ps)*UNITFAD).val();
      BSIM3drainPerimeter = ((this->pd)*UNITFAD).val();
    }
  }
  if((BSIM3sourceArea < 0.0)) {
    BSIM3sourceArea = ((0.0)*UNITFAD).val();
  }
  if((BSIM3drainArea < 0.0)) {
    BSIM3drainArea = ((0.0)*UNITFAD).val();
  }
  if((BSIM3sourcePerimeter < 0.0)) {
    BSIM3sourcePerimeter = ((0.0)*UNITFAD).val();
  }
  if((BSIM3drainPerimeter < 0.0)) {
    BSIM3drainPerimeter = ((0.0)*UNITFAD).val();
  }
  if((((this->binflag > 0.9) && (this->lref > 0)) && (this->wref > 0))) {
    BSIM3binflag = ((1 )*UNITFAD).val();
  }
  else {
    BSIM3binflag = ((0 )*UNITFAD).val();
  }
  if((BSIM3binflag > 0.9)) {
    {
      if((BSIM3binUnit == 1)) {
        {
          Inv_L = ((((1.0e-6/BSIM3leff)-(1.0e-6/this->lref)))*UNITFAD).val();
          Inv_W = ((((1.0e-6/BSIM3weff)-(1.0e-6/this->wref)))*UNITFAD).val();
          Inv_LW = (((Inv_L*Inv_W))*UNITFAD).val();
        }
      }
      else {
        {
          Inv_L = ((((1.0/BSIM3leff)-(1.0/this->lref)))*UNITFAD).val();
          Inv_W = ((((1.0/BSIM3weff)-(1.0/this->wref)))*UNITFAD).val();
          Inv_LW = (((Inv_L*Inv_W))*UNITFAD).val();
        }
      }
    }
  }
  else {
    {
      if((BSIM3binUnit == 1)) {
        {
          Inv_L = (((1.0e-6/BSIM3leff))*UNITFAD).val();
          Inv_W = (((1.0e-6/BSIM3weff))*UNITFAD).val();
          Inv_LW = (((1.0e-12/(BSIM3leff*BSIM3weff)))*UNITFAD).val();
        }
      }
      else {
        {
          Inv_L = (((1.0/BSIM3leff))*UNITFAD).val();
          Inv_W = (((1.0/BSIM3weff))*UNITFAD).val();
          Inv_LW = (((1.0/(BSIM3leff*BSIM3weff)))*UNITFAD).val();
        }
      }
    }
  }
  BSIM3cdsc = (((((BSIM3cdsc+(BSIM3lcdsc*Inv_L))+(BSIM3wcdsc*Inv_W))+(BSIM3pcdsc*Inv_LW)))*UNITFAD).val();
  BSIM3cdscb = (((((BSIM3cdscb+(BSIM3lcdscb*Inv_L))+(BSIM3wcdscb*Inv_W))+(BSIM3pcdscb*Inv_LW)))*UNITFAD).val();
  BSIM3cdscd = (((((BSIM3cdscd+(BSIM3lcdscd*Inv_L))+(BSIM3wcdscd*Inv_W))+(BSIM3pcdscd*Inv_LW)))*UNITFAD).val();
  BSIM3cit = (((((BSIM3cit+(BSIM3lcit*Inv_L))+(BSIM3wcit*Inv_W))+(BSIM3pcit*Inv_LW)))*UNITFAD).val();
  BSIM3nfactor = (((((BSIM3nfactor+(BSIM3lnfactor*Inv_L))+(BSIM3wnfactor*Inv_W))+(BSIM3pnfactor*Inv_LW)))*UNITFAD).val();
  BSIM3xj = (((((BSIM3xj+(BSIM3lxj*Inv_L))+(BSIM3wxj*Inv_W))+(BSIM3pxj*Inv_LW)))*UNITFAD).val();
  BSIM3vsat = (((((BSIM3vsat+(BSIM3lvsat*Inv_L))+(BSIM3wvsat*Inv_W))+(BSIM3pvsat*Inv_LW)))*UNITFAD).val();
  BSIM3at = (((((BSIM3at+(BSIM3lat*Inv_L))+(BSIM3wat*Inv_W))+(BSIM3pat*Inv_LW)))*UNITFAD).val();
  BSIM3a0 = (((((BSIM3a0+(BSIM3la0*Inv_L))+(BSIM3wa0*Inv_W))+(BSIM3pa0*Inv_LW)))*UNITFAD).val();
  BSIM3ags = (((((BSIM3ags+(BSIM3lags*Inv_L))+(BSIM3wags*Inv_W))+(BSIM3pags*Inv_LW)))*UNITFAD).val();
  BSIM3a1 = (((((BSIM3a1+(BSIM3la1*Inv_L))+(BSIM3wa1*Inv_W))+(BSIM3pa1*Inv_LW)))*UNITFAD).val();
  BSIM3a2 = (((((BSIM3a2+(BSIM3la2*Inv_L))+(BSIM3wa2*Inv_W))+(BSIM3pa2*Inv_LW)))*UNITFAD).val();
  BSIM3keta = (((((BSIM3keta+(BSIM3lketa*Inv_L))+(BSIM3wketa*Inv_W))+(BSIM3pketa*Inv_LW)))*UNITFAD).val();
  BSIM3nsub = (((((BSIM3nsub+(BSIM3lnsub*Inv_L))+(BSIM3wnsub*Inv_W))+(BSIM3pnsub*Inv_LW)))*UNITFAD).val();
  BSIM3npeak = (((((BSIM3npeak+(BSIM3lnpeak*Inv_L))+(BSIM3wnpeak*Inv_W))+(BSIM3pnpeak*Inv_LW)))*UNITFAD).val();
  BSIM3ngate = (((((BSIM3ngate+(BSIM3lngate*Inv_L))+(BSIM3wngate*Inv_W))+(BSIM3pngate*Inv_LW)))*UNITFAD).val();
  BSIM3gamma1 = (((((BSIM3gamma1+(BSIM3lgamma1*Inv_L))+(BSIM3wgamma1*Inv_W))+(BSIM3pgamma1*Inv_LW)))*UNITFAD).val();
  BSIM3gamma2 = (((((BSIM3gamma2+(BSIM3lgamma2*Inv_L))+(BSIM3wgamma2*Inv_W))+(BSIM3pgamma2*Inv_LW)))*UNITFAD).val();
  BSIM3vbx = (((((BSIM3vbx+(BSIM3lvbx*Inv_L))+(BSIM3wvbx*Inv_W))+(BSIM3pvbx*Inv_LW)))*UNITFAD).val();
  BSIM3vbm = (((((BSIM3vbm+(BSIM3lvbm*Inv_L))+(BSIM3wvbm*Inv_W))+(BSIM3pvbm*Inv_LW)))*UNITFAD).val();
  BSIM3xt = (((((BSIM3xt+(BSIM3lxt*Inv_L))+(BSIM3wxt*Inv_W))+(BSIM3pxt*Inv_LW)))*UNITFAD).val();
  BSIM3vfb = (((((BSIM3vfb+(BSIM3lvfb*Inv_L))+(BSIM3wvfb*Inv_W))+(BSIM3pvfb*Inv_LW)))*UNITFAD).val();
  BSIM3k1 = (((((BSIM3k1+(BSIM3lk1*Inv_L))+(BSIM3wk1*Inv_W))+(BSIM3pk1*Inv_LW)))*UNITFAD).val();
  BSIM3kt1 = (((((BSIM3kt1+(BSIM3lkt1*Inv_L))+(BSIM3wkt1*Inv_W))+(BSIM3pkt1*Inv_LW)))*UNITFAD).val();
  BSIM3kt1l = (((((BSIM3kt1l+(BSIM3lkt1l*Inv_L))+(BSIM3wkt1l*Inv_W))+(BSIM3pkt1l*Inv_LW)))*UNITFAD).val();
  BSIM3k2 = (((((BSIM3k2+(BSIM3lk2*Inv_L))+(BSIM3wk2*Inv_W))+(BSIM3pk2*Inv_LW)))*UNITFAD).val();
  BSIM3kt2 = (((((BSIM3kt2+(BSIM3lkt2*Inv_L))+(BSIM3wkt2*Inv_W))+(BSIM3pkt2*Inv_LW)))*UNITFAD).val();
  BSIM3k3 = (((((BSIM3k3+(BSIM3lk3*Inv_L))+(BSIM3wk3*Inv_W))+(BSIM3pk3*Inv_LW)))*UNITFAD).val();
  BSIM3k3b = (((((BSIM3k3b+(BSIM3lk3b*Inv_L))+(BSIM3wk3b*Inv_W))+(BSIM3pk3b*Inv_LW)))*UNITFAD).val();
  BSIM3w0 = (((((BSIM3w0+(BSIM3lw0*Inv_L))+(BSIM3ww0*Inv_W))+(BSIM3pw0*Inv_LW)))*UNITFAD).val();
  BSIM3nlx = (((((BSIM3nlx+(BSIM3lnlx*Inv_L))+(BSIM3wnlx*Inv_W))+(BSIM3pnlx*Inv_LW)))*UNITFAD).val();
  BSIM3dvt0 = (((((BSIM3dvt0+(BSIM3ldvt0*Inv_L))+(BSIM3wdvt0*Inv_W))+(BSIM3pdvt0*Inv_LW)))*UNITFAD).val();
  BSIM3dvt1 = (((((BSIM3dvt1+(BSIM3ldvt1*Inv_L))+(BSIM3wdvt1*Inv_W))+(BSIM3pdvt1*Inv_LW)))*UNITFAD).val();
  BSIM3dvt2 = (((((BSIM3dvt2+(BSIM3ldvt2*Inv_L))+(BSIM3wdvt2*Inv_W))+(BSIM3pdvt2*Inv_LW)))*UNITFAD).val();
  BSIM3dvt0w = (((((BSIM3dvt0w+(BSIM3ldvt0w*Inv_L))+(BSIM3wdvt0w*Inv_W))+(BSIM3pdvt0w*Inv_LW)))*UNITFAD).val();
  BSIM3dvt1w = (((((BSIM3dvt1w+(BSIM3ldvt1w*Inv_L))+(BSIM3wdvt1w*Inv_W))+(BSIM3pdvt1w*Inv_LW)))*UNITFAD).val();
  BSIM3dvt2w = (((((BSIM3dvt2w+(BSIM3ldvt2w*Inv_L))+(BSIM3wdvt2w*Inv_W))+(BSIM3pdvt2w*Inv_LW)))*UNITFAD).val();
  BSIM3drout = (((((BSIM3drout+(BSIM3ldrout*Inv_L))+(BSIM3wdrout*Inv_W))+(BSIM3pdrout*Inv_LW)))*UNITFAD).val();
  BSIM3dsub = (((((BSIM3dsub+(BSIM3ldsub*Inv_L))+(BSIM3wdsub*Inv_W))+(BSIM3pdsub*Inv_LW)))*UNITFAD).val();
  BSIM3vth0 = (((((BSIM3vth0+(BSIM3lvth0*Inv_L))+(BSIM3wvth0*Inv_W))+(BSIM3pvth0*Inv_LW)))*UNITFAD).val();
  BSIM3ua = (((((BSIM3ua+(BSIM3lua*Inv_L))+(BSIM3wua*Inv_W))+(BSIM3pua*Inv_LW)))*UNITFAD).val();
  BSIM3ua1 = (((((BSIM3ua1+(BSIM3lua1*Inv_L))+(BSIM3wua1*Inv_W))+(BSIM3pua1*Inv_LW)))*UNITFAD).val();
  BSIM3ub = (((((BSIM3ub+(BSIM3lub*Inv_L))+(BSIM3wub*Inv_W))+(BSIM3pub*Inv_LW)))*UNITFAD).val();
  BSIM3ub1 = (((((BSIM3ub1+(BSIM3lub1*Inv_L))+(BSIM3wub1*Inv_W))+(BSIM3pub1*Inv_LW)))*UNITFAD).val();
  BSIM3uc = (((((BSIM3uc+(BSIM3luc*Inv_L))+(BSIM3wuc*Inv_W))+(BSIM3puc*Inv_LW)))*UNITFAD).val();
  BSIM3uc1 = (((((BSIM3uc1+(BSIM3luc1*Inv_L))+(BSIM3wuc1*Inv_W))+(BSIM3puc1*Inv_LW)))*UNITFAD).val();
  BSIM3u0 = (((((BSIM3u0+(BSIM3lu0*Inv_L))+(BSIM3wu0*Inv_W))+(BSIM3pu0*Inv_LW)))*UNITFAD).val();
  BSIM3ute = (((((BSIM3ute+(BSIM3lute*Inv_L))+(BSIM3wute*Inv_W))+(BSIM3pute*Inv_LW)))*UNITFAD).val();
  BSIM3voff = (((((BSIM3voff+(BSIM3lvoff*Inv_L))+(BSIM3wvoff*Inv_W))+(BSIM3pvoff*Inv_LW)))*UNITFAD).val();
  BSIM3delta = (((((BSIM3delta+(BSIM3ldelta*Inv_L))+(BSIM3wdelta*Inv_W))+(BSIM3pdelta*Inv_LW)))*UNITFAD).val();
  BSIM3rdsw = (((((BSIM3rdsw+(BSIM3lrdsw*Inv_L))+(BSIM3wrdsw*Inv_W))+(BSIM3prdsw*Inv_LW)))*UNITFAD).val();
  BSIM3prwg = (((((BSIM3prwg+(BSIM3lprwg*Inv_L))+(BSIM3wprwg*Inv_W))+(BSIM3pprwg*Inv_LW)))*UNITFAD).val();
  BSIM3prwb = (((((BSIM3prwb+(BSIM3lprwb*Inv_L))+(BSIM3wprwb*Inv_W))+(BSIM3pprwb*Inv_LW)))*UNITFAD).val();
  BSIM3prt = (((((BSIM3prt+(BSIM3lprt*Inv_L))+(BSIM3wprt*Inv_W))+(BSIM3pprt*Inv_LW)))*UNITFAD).val();
  BSIM3eta0 = (((((BSIM3eta0+(BSIM3leta0*Inv_L))+(BSIM3weta0*Inv_W))+(BSIM3peta0*Inv_LW)))*UNITFAD).val();
  BSIM3etab = (((((BSIM3etab+(BSIM3letab*Inv_L))+(BSIM3wetab*Inv_W))+(BSIM3petab*Inv_LW)))*UNITFAD).val();
  BSIM3pclm = (((((BSIM3pclm+(BSIM3lpclm*Inv_L))+(BSIM3wpclm*Inv_W))+(BSIM3ppclm*Inv_LW)))*UNITFAD).val();
  BSIM3pdibl1 = (((((BSIM3pdibl1+(BSIM3lpdibl1*Inv_L))+(BSIM3wpdibl1*Inv_W))+(BSIM3ppdibl1*Inv_LW)))*UNITFAD).val();
  BSIM3pdibl2 = (((((BSIM3pdibl2+(BSIM3lpdibl2*Inv_L))+(BSIM3wpdibl2*Inv_W))+(BSIM3ppdibl2*Inv_LW)))*UNITFAD).val();
  BSIM3pdiblb = (((((BSIM3pdiblb+(BSIM3lpdiblb*Inv_L))+(BSIM3wpdiblb*Inv_W))+(BSIM3ppdiblb*Inv_LW)))*UNITFAD).val();
  BSIM3pscbe1 = (((((BSIM3pscbe1+(BSIM3lpscbe1*Inv_L))+(BSIM3wpscbe1*Inv_W))+(BSIM3ppscbe1*Inv_LW)))*UNITFAD).val();
  BSIM3pscbe2 = (((((BSIM3pscbe2+(BSIM3lpscbe2*Inv_L))+(BSIM3wpscbe2*Inv_W))+(BSIM3ppscbe2*Inv_LW)))*UNITFAD).val();
  BSIM3pvag = (((((BSIM3pvag+(BSIM3lpvag*Inv_L))+(BSIM3wpvag*Inv_W))+(BSIM3ppvag*Inv_LW)))*UNITFAD).val();
  BSIM3wr = (((((BSIM3wr+(BSIM3lwr*Inv_L))+(BSIM3wwr*Inv_W))+(BSIM3pwr*Inv_LW)))*UNITFAD).val();
  BSIM3dwg = (((((BSIM3dwg+(BSIM3ldwg*Inv_L))+(BSIM3wdwg*Inv_W))+(BSIM3pdwg*Inv_LW)))*UNITFAD).val();
  BSIM3dwb = (((((BSIM3dwb+(BSIM3ldwb*Inv_L))+(BSIM3wdwb*Inv_W))+(BSIM3pdwb*Inv_LW)))*UNITFAD).val();
  BSIM3b0 = (((((BSIM3b0+(BSIM3lb0*Inv_L))+(BSIM3wb0*Inv_W))+(BSIM3pb0*Inv_LW)))*UNITFAD).val();
  BSIM3b1 = (((((BSIM3b1+(BSIM3lb1*Inv_L))+(BSIM3wb1*Inv_W))+(BSIM3pb1*Inv_LW)))*UNITFAD).val();
  BSIM3alpha0 = (((((BSIM3alpha0+(BSIM3lalpha0*Inv_L))+(BSIM3walpha0*Inv_W))+(BSIM3palpha0*Inv_LW)))*UNITFAD).val();
  BSIM3alpha1 = (((((BSIM3alpha1+(BSIM3lalpha1*Inv_L))+(BSIM3walpha1*Inv_W))+(BSIM3palpha1*Inv_LW)))*UNITFAD).val();
  BSIM3beta0 = (((((BSIM3beta0+(BSIM3lbeta0*Inv_L))+(BSIM3wbeta0*Inv_W))+(BSIM3pbeta0*Inv_LW)))*UNITFAD).val();
  BSIM3elm = (((((BSIM3elm+(BSIM3lelm*Inv_L))+(BSIM3welm*Inv_W))+(BSIM3pelm*Inv_LW)))*UNITFAD).val();
  BSIM3cgsl = (((((BSIM3cgsl+(BSIM3lcgsl*Inv_L))+(BSIM3wcgsl*Inv_W))+(BSIM3pcgsl*Inv_LW)))*UNITFAD).val();
  BSIM3cgdl = (((((BSIM3cgdl+(BSIM3lcgdl*Inv_L))+(BSIM3wcgdl*Inv_W))+(BSIM3pcgdl*Inv_LW)))*UNITFAD).val();
  BSIM3ckappa = (((((BSIM3ckappa+(BSIM3lckappa*Inv_L))+(BSIM3wckappa*Inv_W))+(BSIM3pckappa*Inv_LW)))*UNITFAD).val();
  BSIM3cf = (((((BSIM3cf+(BSIM3lcf*Inv_L))+(BSIM3wcf*Inv_W))+(BSIM3pcf*Inv_LW)))*UNITFAD).val();
  BSIM3clc = (((((BSIM3clc+(BSIM3lclc*Inv_L))+(BSIM3wclc*Inv_W))+(BSIM3pclc*Inv_LW)))*UNITFAD).val();
  BSIM3cle = (((((BSIM3cle+(BSIM3lcle*Inv_L))+(BSIM3wcle*Inv_W))+(BSIM3pcle*Inv_LW)))*UNITFAD).val();
  BSIM3vfbcv = (((((BSIM3vfbcv+(BSIM3lvfbcv*Inv_L))+(BSIM3wvfbcv*Inv_W))+(BSIM3pvfbcv*Inv_LW)))*UNITFAD).val();
  BSIM3acde = (((((BSIM3acde+(BSIM3lacde*Inv_L))+(BSIM3wacde*Inv_W))+(BSIM3pacde*Inv_LW)))*UNITFAD).val();
  BSIM3moin = (((((BSIM3moin+(BSIM3lmoin*Inv_L))+(BSIM3wmoin*Inv_W))+(BSIM3pmoin*Inv_LW)))*UNITFAD).val();
  BSIM3noff = (((((BSIM3noff+(BSIM3lnoff*Inv_L))+(BSIM3wnoff*Inv_W))+(BSIM3pnoff*Inv_LW)))*UNITFAD).val();
  BSIM3voffcv = (((((BSIM3voffcv+(BSIM3lvoffcv*Inv_L))+(BSIM3wvoffcv*Inv_W))+(BSIM3pvoffcv*Inv_LW)))*UNITFAD).val();
  BSIM3agisl = (((((BSIM3agisl+(BSIM3lagisl*Inv_L))+(BSIM3wagisl*Inv_W))+(BSIM3pagisl*Inv_LW)))*UNITFAD).val();
  BSIM3bgisl = (((((BSIM3bgisl+(BSIM3lbgisl*Inv_L))+(BSIM3wbgisl*Inv_W))+(BSIM3pbgisl*Inv_LW)))*UNITFAD).val();
  BSIM3cgisl = (((((BSIM3cgisl+(BSIM3lcgisl*Inv_L))+(BSIM3wcgisl*Inv_W))+(BSIM3pcgisl*Inv_LW)))*UNITFAD).val();
  BSIM3egisl = (((((BSIM3egisl+(BSIM3legisl*Inv_L))+(BSIM3wegisl*Inv_W))+(BSIM3pegisl*Inv_LW)))*UNITFAD).val();
  BSIM3rgisl = (((((BSIM3rgisl+(BSIM3lrgisl*Inv_L))+(BSIM3wrgisl*Inv_W))+(BSIM3prgisl*Inv_LW)))*UNITFAD).val();
  BSIM3kgisl = (((((BSIM3kgisl+(BSIM3lkgisl*Inv_L))+(BSIM3wkgisl*Inv_W))+(BSIM3pkgisl*Inv_LW)))*UNITFAD).val();
  BSIM3fgisl = (((((BSIM3fgisl+(BSIM3lfgisl*Inv_L))+(BSIM3wfgisl*Inv_W))+(BSIM3pfgisl*Inv_LW)))*UNITFAD).val();
  BSIM3rgidl = (((((BSIM3rgidl+(BSIM3lrgidl*Inv_L))+(BSIM3wrgidl*Inv_W))+(BSIM3prgidl*Inv_LW)))*UNITFAD).val();
  BSIM3kgidl = (((((BSIM3kgidl+(BSIM3lkgidl*Inv_L))+(BSIM3wkgidl*Inv_W))+(BSIM3pkgidl*Inv_LW)))*UNITFAD).val();
  BSIM3fgidl = (((((BSIM3fgidl+(BSIM3lfgidl*Inv_L))+(BSIM3wfgidl*Inv_W))+(BSIM3pfgidl*Inv_LW)))*UNITFAD).val();
  BSIM3agidl = (((((BSIM3agidl+(BSIM3lagidl*Inv_L))+(BSIM3wagidl*Inv_W))+(BSIM3pagidl*Inv_LW)))*UNITFAD).val();
  BSIM3bgidl = (((((BSIM3bgidl+(BSIM3lbgidl*Inv_L))+(BSIM3wbgidl*Inv_W))+(BSIM3pbgidl*Inv_LW)))*UNITFAD).val();
  BSIM3cgidl = (((((BSIM3cgidl+(BSIM3lcgidl*Inv_L))+(BSIM3wcgidl*Inv_W))+(BSIM3pcgidl*Inv_LW)))*UNITFAD).val();
  BSIM3egidl = (((((BSIM3egidl+(BSIM3legidl*Inv_L))+(BSIM3wegidl*Inv_W))+(BSIM3pegidl*Inv_LW)))*UNITFAD).val();
  BSIM3nigc = (((((BSIM3nigc+(BSIM3lnigc*Inv_L))+(BSIM3wnigc*Inv_W))+(BSIM3pnigc*Inv_LW)))*UNITFAD).val();
  BSIM3aigc = (((((BSIM3aigc+(BSIM3laigc*Inv_L))+(BSIM3waigc*Inv_W))+(BSIM3paigc*Inv_LW)))*UNITFAD).val();
  BSIM3bigc = (((((BSIM3bigc+(BSIM3lbigc*Inv_L))+(BSIM3wbigc*Inv_W))+(BSIM3pbigc*Inv_LW)))*UNITFAD).val();
  BSIM3cigc = (((((BSIM3cigc+(BSIM3lcigc*Inv_L))+(BSIM3wcigc*Inv_W))+(BSIM3pcigc*Inv_LW)))*UNITFAD).val();
  BSIM3aigbacc = (((((BSIM3aigbacc+(BSIM3laigbacc*Inv_L))+(BSIM3waigbacc*Inv_W))+(BSIM3paigbacc*Inv_LW)))*UNITFAD).val();
  BSIM3bigbacc = (((((BSIM3bigbacc+(BSIM3lbigbacc*Inv_L))+(BSIM3wbigbacc*Inv_W))+(BSIM3pbigbacc*Inv_LW)))*UNITFAD).val();
  BSIM3cigbacc = (((((BSIM3cigbacc+(BSIM3lcigbacc*Inv_L))+(BSIM3wcigbacc*Inv_W))+(BSIM3pcigbacc*Inv_LW)))*UNITFAD).val();
  BSIM3aigbinv = (((((BSIM3aigbinv+(BSIM3laigbinv*Inv_L))+(BSIM3waigbinv*Inv_W))+(BSIM3paigbinv*Inv_LW)))*UNITFAD).val();
  BSIM3bigbinv = (((((BSIM3bigbinv+(BSIM3lbigbinv*Inv_L))+(BSIM3wbigbinv*Inv_W))+(BSIM3pbigbinv*Inv_LW)))*UNITFAD).val();
  BSIM3cigbinv = (((((BSIM3cigbinv+(BSIM3lcigbinv*Inv_L))+(BSIM3wcigbinv*Inv_W))+(BSIM3pcigbinv*Inv_LW)))*UNITFAD).val();
  BSIM3eigbinv = (((((BSIM3eigbinv+(BSIM3leigbinv*Inv_L))+(BSIM3weigbinv*Inv_W))+(BSIM3peigbinv*Inv_LW)))*UNITFAD).val();
  BSIM3nigbinv = (((((BSIM3nigbinv+(BSIM3lnigbinv*Inv_L))+(BSIM3wnigbinv*Inv_W))+(BSIM3pnigbinv*Inv_LW)))*UNITFAD).val();
  BSIM3ntox = (((((BSIM3ntox+(BSIM3lntox*Inv_L))+(BSIM3wntox*Inv_W))+(BSIM3pntox*Inv_LW)))*UNITFAD).val();
  BSIM3nsd = (((((BSIM3nsd+(BSIM3lnsd*Inv_L))+(BSIM3wnsd*Inv_W))+(BSIM3pnsd*Inv_LW)))*UNITFAD).val();
  BSIM3vfbsdoff = (((((BSIM3vfbsdoff+(BSIM3lvfbsdoff*Inv_L))+(BSIM3wvfbsdoff*Inv_W))+(BSIM3pvfbsdoff*Inv_LW)))*UNITFAD).val();
  BSIM3tvfbsdoff = (((((BSIM3tvfbsdoff+(BSIM3ltvfbsdoff*Inv_L))+(BSIM3wtvfbsdoff*Inv_W))+(BSIM3ptvfbsdoff*Inv_LW)))*UNITFAD).val();
  BSIM3aigs = (((((BSIM3aigs+(BSIM3laigs*Inv_L))+(BSIM3waigs*Inv_W))+(BSIM3paigs*Inv_LW)))*UNITFAD).val();
  BSIM3bigs = (((((BSIM3bigs+(BSIM3lbigs*Inv_L))+(BSIM3wbigs*Inv_W))+(BSIM3pbigs*Inv_LW)))*UNITFAD).val();
  BSIM3cigs = (((((BSIM3cigs+(BSIM3lcigs*Inv_L))+(BSIM3wcigs*Inv_W))+(BSIM3pcigs*Inv_LW)))*UNITFAD).val();
  BSIM3aigd = (((((BSIM3aigd+(BSIM3laigd*Inv_L))+(BSIM3waigd*Inv_W))+(BSIM3paigd*Inv_LW)))*UNITFAD).val();
  BSIM3bigd = (((((BSIM3bigd+(BSIM3lbigd*Inv_L))+(BSIM3wbigd*Inv_W))+(BSIM3pbigd*Inv_LW)))*UNITFAD).val();
  BSIM3cigd = (((((BSIM3cigd+(BSIM3lcigd*Inv_L))+(BSIM3wcigd*Inv_W))+(BSIM3pcigd*Inv_LW)))*UNITFAD).val();
  BSIM3aigsd = (((((BSIM3aigsd+(BSIM3laigsd*Inv_L))+(BSIM3waigsd*Inv_W))+(BSIM3paigsd*Inv_LW)))*UNITFAD).val();
  BSIM3bigsd = (((((BSIM3bigsd+(BSIM3lbigsd*Inv_L))+(BSIM3wbigsd*Inv_W))+(BSIM3pbigsd*Inv_LW)))*UNITFAD).val();
  BSIM3cigsd = (((((BSIM3cigsd+(BSIM3lcigsd*Inv_L))+(BSIM3wcigsd*Inv_W))+(BSIM3pcigsd*Inv_LW)))*UNITFAD).val();
  BSIM3nigbacc = (((((BSIM3nigbacc+(BSIM3lnigbacc*Inv_L))+(BSIM3wnigbacc*Inv_W))+(BSIM3pnigbacc*Inv_LW)))*UNITFAD).val();
  BSIM3abulkCVfactor = (((1.0+pow((BSIM3clc/BSIM3leffCV),BSIM3cle)))*UNITFAD).val();
  T0 = (((TRatio-1.0))*UNITFAD).val();
  BSIM3ua = ((((BSIM3ua*BSIM3mulua)+(BSIM3ua1*T0)))*UNITFAD).val();
  BSIM3ub = ((((BSIM3ub*BSIM3mulub)+(BSIM3ub1*T0)))*UNITFAD).val();
  BSIM3uc = (((BSIM3uc+(BSIM3uc1*T0)))*UNITFAD).val();
  BSIM3u0 = (((BSIM3u0*BSIM3mulu0))*UNITFAD).val();
  if(((BSIM3u0 > 1.0) && (this->version > 3.021))) {
    BSIM3u0 = (((BSIM3u0/1.0e4))*UNITFAD).val();
  }
  BSIM3u0temp = (((BSIM3u0*pow(TRatio,BSIM3ute)))*UNITFAD).val();
  BSIM3vsattemp = (((BSIM3vsat-(BSIM3at*T0)))*UNITFAD).val();
  BSIM3rds0 = ((((BSIM3rdsw+(BSIM3prt*T0))/pow((BSIM3weff*1E6),BSIM3wr)))*UNITFAD).val();
  if((BSIM3breakmod == 1)) {
    {
      Vtm = (((8.617087e-5*T))*UNITFAD).val();
      Vtm0 = (((8.617087e-5*BSIM3tnom))*UNITFAD).val();
      Nvtm = (((Vtm*BSIM3jctEmissionCoeff))*UNITFAD).val();
      Eg0 = (((1.16-(((7.02e-4*BSIM3tnom)*BSIM3tnom)/(BSIM3tnom+1108.0))))*UNITFAD).val();
      Eg = (((1.16-(((7.02e-4*T)*T)/(T+1108.0))))*UNITFAD).val();
      if((T != BSIM3tnom)) {
        {
          T0 = (((((Eg0/Vtm0)-(Eg/Vtm))+(BSIM3jctTempExponent*log10((T/BSIM3tnom)))))*UNITFAD).val();
          T1 = ((exp((T0/BSIM3jctEmissionCoeff)))*UNITFAD).val();
          BSIM3jctTempSatCurDensity = (((BSIM3jctSatCurDensity*T1))*UNITFAD).val();
          BSIM3jctSidewallTempSatCurDensity = (((BSIM3jctSidewallSatCurDensity*T1))*UNITFAD).val();
        }
      }
      else {
        {
          BSIM3jctTempSatCurDensity = ((BSIM3jctSatCurDensity)*UNITFAD).val();
          BSIM3jctSidewallTempSatCurDensity = ((BSIM3jctSidewallSatCurDensity)*UNITFAD).val();
        }
      }
      if((BSIM3jctTempSatCurDensity < 0.0)) {
        BSIM3jctTempSatCurDensity = ((0.0)*UNITFAD).val();
      }
      if((BSIM3jctSidewallTempSatCurDensity < 0.0)) {
        BSIM3jctSidewallTempSatCurDensity = ((0.0)*UNITFAD).val();
      }
      if(((BSIM3bvd/Nvtm) > 34.0)) {
        BSIM3XExpBVD = (((BSIM3xjbvd*1.713908431e-15))*UNITFAD).val();
      }
      else {
        {
          BSIM3XExpBVD = ((exp((-BSIM3bvd/Nvtm)))*UNITFAD).val();
          BSIM3XExpBVD = (((BSIM3XExpBVD*BSIM3xjbvd))*UNITFAD).val();
        }
      }
      if(((BSIM3drainArea <= 0.0) && (BSIM3drainPerimeter <= 0.0))) {
        BSIM3DrainSatCurrent = ((1.0e-14)*UNITFAD).val();
      }
      else {
        BSIM3DrainSatCurrent = ((((BSIM3drainArea*BSIM3jctTempSatCurDensity)+(BSIM3drainPerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
      }
      if((BSIM3DrainSatCurrent != 0.0)) {
        T1 = ((((1.0+(BSIM3ijth/BSIM3DrainSatCurrent))-BSIM3XExpBVD))*UNITFAD).val();
      }
      else {
        T1 = (((1.0-BSIM3XExpBVD))*UNITFAD).val();
      }
      if((((T1*T1)+(4.0*BSIM3XExpBVD)) >= 0.0)) {
        T2 = (((0.5*(T1+cogenda_ternary_op<CogendaFadType>((((T1*T1)+(4.0*BSIM3XExpBVD)) > 0.0),sqrt(((T1*T1)+(4.0*BSIM3XExpBVD))),0.0))))*UNITFAD).val();
      }
      else {
        T2 = (((0.5*(T1+0.0)))*UNITFAD).val();
      }
      BSIM3vjdmFwd = (((Nvtm*log10(T2)))*UNITFAD).val();
      T0 = ((exp((BSIM3vjdmFwd/Nvtm)))*UNITFAD).val();
      BSIM3IVjdmFwd = (((BSIM3DrainSatCurrent*(((T0-(BSIM3XExpBVD/T0))+BSIM3XExpBVD)-1.0)))*UNITFAD).val();
      BSIM3DslpFwd = ((((BSIM3DrainSatCurrent*(T0+(BSIM3XExpBVD/T0)))/Nvtm))*UNITFAD).val();
      if((BSIM3xjbvd != 0.0)) {
        {
          T2 = (((BSIM3ijthdrev/BSIM3DrainSatCurrent))*UNITFAD).val();
          if((T2 < 1.0)) {
            {
              T2 = ((10.0)*UNITFAD).val();
              printf("Warning; ijthdrev too small and set to 10 times IdbSat. \n");
            }
          }
          BSIM3vjdmRev = (((-BSIM3bvd-(((Vtm*BSIM3jctEmissionCoeff)*log10((T2-1.0)))/BSIM3xjbvd)))*UNITFAD).val();
        }
      }
      else {
        BSIM3vjdmRev = ((-BSIM3bvd)*UNITFAD).val();
      }
      T1 = (((BSIM3xjbvd*exp((-(BSIM3bvd+BSIM3vjdmRev)/Nvtm))))*UNITFAD).val();
      BSIM3IVjdmRev = (((BSIM3DrainSatCurrent*(1.0+T1)))*UNITFAD).val();
      BSIM3DslpRev = ((((-BSIM3DrainSatCurrent*T1)/Nvtm))*UNITFAD).val();
      if(((BSIM3bvs/Nvtm) > 34.0)) {
        BSIM3XExpBVS = (((BSIM3xjbvs*1.713908431e-15))*UNITFAD).val();
      }
      else {
        {
          BSIM3XExpBVS = ((exp((-BSIM3bvs/Nvtm)))*UNITFAD).val();
          BSIM3XExpBVS = (((BSIM3XExpBVS*BSIM3xjbvs))*UNITFAD).val();
        }
      }
      if(((BSIM3sourceArea <= 0.0) && (BSIM3sourcePerimeter <= 0.0))) {
        BSIM3SourceSatCurrent = ((1.0e-14)*UNITFAD).val();
      }
      else {
        BSIM3SourceSatCurrent = ((((BSIM3sourceArea*BSIM3jctTempSatCurDensity)+(BSIM3sourcePerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
      }
      if((BSIM3SourceSatCurrent != 0.0)) {
        T1 = ((((1.0+(BSIM3ijth/BSIM3SourceSatCurrent))-BSIM3XExpBVS))*UNITFAD).val();
      }
      else {
        T1 = (((1.0-BSIM3XExpBVS))*UNITFAD).val();
      }
      if((((T1*T1)+(4.0*BSIM3XExpBVS)) >= 0.0)) {
        T2 = (((0.5*(T1+cogenda_ternary_op<CogendaFadType>((((T1*T1)+(4.0*BSIM3XExpBVS)) > 0.0),sqrt(((T1*T1)+(4.0*BSIM3XExpBVS))),0.0))))*UNITFAD).val();
      }
      else {
        T2 = (((0.5*(T1+0.0)))*UNITFAD).val();
      }
      BSIM3vjsmFwd = (((Nvtm*log10(T2)))*UNITFAD).val();
      T0 = ((exp((BSIM3vjsmFwd/Nvtm)))*UNITFAD).val();
      BSIM3IVjsmFwd = (((BSIM3SourceSatCurrent*(((T0-(BSIM3XExpBVS/T0))+BSIM3XExpBVS)-1.0)))*UNITFAD).val();
      BSIM3SslpFwd = ((((BSIM3SourceSatCurrent*(T0+(BSIM3XExpBVS/T0)))/Nvtm))*UNITFAD).val();
      if((BSIM3xjbvs != 0.0)) {
        {
          T2 = (((BSIM3ijthsrev/BSIM3SourceSatCurrent))*UNITFAD).val();
          if((T2 < 1.0)) {
            {
              T2 = ((10.0)*UNITFAD).val();
              printf("Warning; ijthsrev too small and set to 10 times IsbSat. \n");
            }
          }
          BSIM3vjsmRev = (((-BSIM3bvs-(((Vtm*BSIM3jctEmissionCoeff)*log10((T2-1.0)))/BSIM3xjbvs)))*UNITFAD).val();
        }
      }
      else {
        BSIM3vjsmRev = ((-BSIM3bvs)*UNITFAD).val();
      }
      T1 = (((BSIM3xjbvs*exp((-(BSIM3bvs+BSIM3vjsmRev)/Nvtm))))*UNITFAD).val();
      BSIM3IVjsmRev = (((BSIM3SourceSatCurrent*(1.0+T1)))*UNITFAD).val();
      BSIM3SslpRev = ((((-BSIM3SourceSatCurrent*T1)/Nvtm))*UNITFAD).val();
    }
  }
  Fatal_Flag = ((0)*UNITFAD).val();
  if(((BSIM3capMod > 2) && (this->version < 3.11))) {
    {
      BSIM3capMod = ((2)*UNITFAD).val();
      printf("Warning: CAPMOD > 2 not allowed with Version = %f \n",this->version);
    }
  }
  if((BSIM3nlx < -BSIM3leff)) {
    {
      printf("Fatal: Nlx = %g is less than -Leff.\n",((BSIM3nlx)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3tox <= 0.0)) {
    {
      printf("Fatal: Tox = %g is not positive.\n",((BSIM3tox)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3toxm <= 0.0)) {
    {
      printf("Fatal: Toxm = %g is not positive.\n",((BSIM3toxm)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3npeak <= 0.0)) {
    {
      printf("Fatal: Nch = %g is not positive.\n",((BSIM3npeak)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3nsub <= 0.0)) {
    {
      printf("Fatal: Nsub = %g is not positive.\n",((BSIM3nsub)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3ngate < 0.0)) {
    {
      printf("Fatal: Ngate = %g Ngate is not positive.\n",((BSIM3ngate)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3ngate > 1.0e25)) {
    {
      printf("Fatal: Ngate = %g Ngate is too high\n",((BSIM3ngate)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3xj <= 0.0)) {
    {
      printf("Fatal: Xj = %g is not positive.\n",((BSIM3xj)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3dvt1 < 0.0)) {
    {
      printf("Fatal: Dvt1 = %g is negative.\n",((BSIM3dvt1)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3dvt1w < 0.0)) {
    {
      printf("Fatal: Dvt1w = %g is negative.\n",((BSIM3dvt1w)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3w0 == -BSIM3weff)) {
    {
      printf("Fatal: (W0 + Weff) = 0 causing divided-by-zero.\n");
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3dsub < 0.0)) {
    {
      printf("Fatal: Dsub = %g is negative.\n",((BSIM3dsub)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3b1 == -BSIM3weff)) {
    {
      printf("Fatal: (B1 + Weff) = 0 causing divided-by-zero.\n");
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3u0temp <= 0.0)) {
    {
      printf("Fatal: u0 at current temperature = %g is not positive.\n",((BSIM3u0temp)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3delta < 0.0)) {
    {
      printf("Fatal: Delta = %g is less than zero.\n",((BSIM3delta)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3vsattemp <= 0.0)) {
    {
      printf("Fatal: Vsat at current temperature = %g is not positive.\n",((BSIM3vsattemp)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3pclm <= 0.0)) {
    {
      printf("Fatal: Pclm = %g is not positive.\n",((BSIM3pclm)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3drout < 0.0)) {
    {
      printf("Fatal: Drout = %g is negative.\n",((BSIM3drout)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3pscbe2 <= 0.0)) {
    {
      printf("Warning: Pscbe2 = %g is not positive.\n",((BSIM3pscbe2)*UNITFAD).val());
    }
  }
  if(((BSIM3unitLengthSidewallJctCap > 0.0) || (BSIM3unitLengthGateSidewallJctCap > 0.0))) {
    {
      if((BSIM3drainPerimeter < BSIM3weff)) {
        printf("\nWarning: Pd = %g is less than Weff=%g.",((BSIM3drainPerimeter)*UNITFAD).val(),((BSIM3weff)*UNITFAD).val());
      }
      if((BSIM3sourcePerimeter < BSIM3weff)) {
        printf("\nWarning: Ps = %g is less than Weff=%g.",((BSIM3sourcePerimeter)*UNITFAD).val(),((BSIM3weff)*UNITFAD).val());
      }
    }
  }
  if((BSIM3noff < 0.1)) {
    {
      printf("Warning: Noff = %g is too small.\n",((BSIM3noff)*UNITFAD).val());
    }
  }
  if((BSIM3noff > 4.0)) {
    {
      printf("Warning: Noff = %g is too large.\n",((BSIM3noff)*UNITFAD).val());
    }
  }
  if((BSIM3voffcv < -0.5)) {
    {
      printf("Warning: Voffcv = %g is too small.\n",((BSIM3voffcv)*UNITFAD).val());
    }
  }
  if((BSIM3voffcv > 0.5)) {
    {
      printf("Warning: Voffcv = %g is too large.\n",((BSIM3voffcv)*UNITFAD).val());
    }
  }
  if((BSIM3ijth < 0.0)) {
    {
      printf("Fatal: Ijth = %g cannot be negative.\n",((BSIM3ijth)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3clc < 0.0)) {
    {
      printf("Fatal: Clc = %g is negative.\n",((BSIM3clc)*UNITFAD).val());
      Fatal_Flag = ((1)*UNITFAD).val();
    }
  }
  if((BSIM3moin < 5.0)) {
    {
      printf("Warning: Moin = %g is too small.\n",((BSIM3moin)*UNITFAD).val());
    }
  }
  if((BSIM3moin > 25.0)) {
    {
      printf("Warning: Moin = %g is too large.\n",((BSIM3moin)*UNITFAD).val());
    }
  }
  if((BSIM3capMod == 3)) {
    {
      if((this->dtoxcv < 0.0)) {
        printf("Warning: dtoxcv = %g is negative! Physically, should set positive or zero.\n",this->dtoxcv);
      }
      if(((this->tox-this->dtoxcv) <= 0.0)) {
        {
          printf("Fatal: Tox - dtoxcv = %g is not positive.\n",(this->tox-this->dtoxcv));
          printf("W = %g, L = %g\n",this->w,this->l);
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3acde < 0.4)) {
        {
          printf("Warning:  Acde = %g is too small.\n",((BSIM3acde)*UNITFAD).val());
        }
      }
      if((BSIM3acde > 1.6)) {
        {
          printf("Warning:  Acde = %g is too large.\n",((BSIM3acde)*UNITFAD).val());
        }
      }
    }
  }
  if((this->paramchk == 1)) {
    {
      if((BSIM3leff <= 5.0e-8)) {
        {
          printf("Warning: Leff = %g may be too small.\n",((BSIM3leff)*UNITFAD).val());
        }
      }
      if((BSIM3leffCV <= 5.0e-8)) {
        {
          printf("Warning: Leff for CV = %g may be too small.\n",((BSIM3leffCV)*UNITFAD).val());
        }
      }
      if((BSIM3weff <= 1.0e-7)) {
        {
          printf("Warning: Weff = %g may be too small.\n",((BSIM3weff)*UNITFAD).val());
        }
      }
      if((BSIM3weffCV <= 1.0e-7)) {
        {
          printf("Warning: Weff for CV = %g may be too small.\n",((BSIM3weffCV)*UNITFAD).val());
        }
      }
      if((BSIM3nlx < 0.0)) {
        {
          printf("Warning: Nlx = %g is negative.\n",((BSIM3nlx)*UNITFAD).val());
        }
      }
      if((BSIM3tox < 1.0e-9)) {
        {
          printf("Warning: Tox = %g is less than 10A.\n",((BSIM3tox)*UNITFAD).val());
        }
      }
      if((BSIM3npeak <= 1.0e15)) {
        {
          printf("Warning: Nch = %g may be too small.\n",((BSIM3npeak)*UNITFAD).val());
        }
      }
          else if((BSIM3npeak >= 1.0e21)) {
        {
          printf("Warning: Nch = %g may be too large.\n",((BSIM3npeak)*UNITFAD).val());
        }
      }
      if((BSIM3nsub <= 1.0e14)) {
        {
          printf("Warning: Nsub = %g may be too small.\n",((BSIM3nsub)*UNITFAD).val());
        }
      }
          else if((BSIM3nsub >= 1.0e21)) {
        {
          printf("Warning: Nsub = %g may be too large.\n",((BSIM3nsub)*UNITFAD).val());
        }
      }
      if(((BSIM3ngate > 0.0) && (BSIM3ngate <= 1.0e18))) {
        {
          printf("Warning: Ngate = %g is less than 1.E18cm^-3.\n",((BSIM3ngate)*UNITFAD).val());
        }
      }
      if((BSIM3dvt0 < 0.0)) {
        {
          printf("Warning: Dvt0 = %g is negative.\n",((BSIM3dvt0)*UNITFAD).val());
        }
      }
      if((fabs((1.0e-6/(BSIM3w0+BSIM3weff))) > 10.0)) {
        {
          printf("Warning: (W0 + Weff) may be too small.\n");
        }
      }
      if((BSIM3nfactor < 0.0)) {
        {
          printf("Warning: Nfactor = %g is negative.\n",((BSIM3nfactor)*UNITFAD).val());
        }
      }
      if((BSIM3cdsc < 0.0)) {
        {
          printf("Warning: Cdsc = %g is negative.\n",((BSIM3cdsc)*UNITFAD).val());
        }
      }
      if((BSIM3cdscd < 0.0)) {
        {
          printf("Warning: Cdscd = %g is negative.\n",((BSIM3cdscd)*UNITFAD).val());
        }
      }
      if((BSIM3eta0 < 0.0)) {
        {
          printf("Warning: Eta0 = %g is negative.\n",((BSIM3eta0)*UNITFAD).val());
        }
      }
      if((fabs((1.0e-6/(BSIM3b1+BSIM3weff))) > 10.0)) {
        {
          printf("Warning: (B1 + Weff) may be too small.\n");
        }
      }
      if((BSIM3a2 < 0.01)) {
        {
          printf("Warning: A2 = %g is too small. Set to 0.01.\n",((BSIM3a2)*UNITFAD).val());
          BSIM3a2 = ((0.01)*UNITFAD).val();
        }
      }
          else if((BSIM3a2 > 1.0)) {
        {
          printf("Warning: A2 = %g is larger than 1. A2 is set to 1 and A1 is set to 0.\n",((BSIM3a2)*UNITFAD).val());
          BSIM3a2 = ((1.0)*UNITFAD).val();
          BSIM3a1 = ((0.0)*UNITFAD).val();
        }
      }
      if((this->version < 3.3)) {
        {
          if((BSIM3rdsw < 0.0)) {
            {
              printf("Warning: Rdsw = %g is negative. Set to zero.\n",((BSIM3rdsw)*UNITFAD).val());
              BSIM3rdsw = ((0.0)*UNITFAD).val();
              BSIM3rds0 = ((0.0)*UNITFAD).val();
            }
          }
                  else if(((BSIM3rds0 > 0.0) && (BSIM3rds0 < 0.001))) {
            {
              printf("Warning: Rds at current temperature = %g  < 0.001 ohm. Set to zero.\n",((BSIM3rds0)*UNITFAD).val());
              BSIM3rds0 = ((0.0)*UNITFAD).val();
            }
          }
        }
      }
      else {
        {
          if((BSIM3rdsw < 0.0)) {
            {
              printf("Warning: Rdsw = %g is negative. Set to zero.\n",((BSIM3rdsw)*UNITFAD).val());
              BSIM3rdsw = ((0.0)*UNITFAD).val();
              BSIM3rds0 = ((0.0)*UNITFAD).val();
            }
          }
          if((BSIM3rds0 < 0.0)) {
            {
              printf("Warning: Rds at current temperature = %g is negative. Set to zero.\n",((BSIM3rds0)*UNITFAD).val());
              BSIM3rds0 = ((0.0)*UNITFAD).val();
            }
          }
        }
      }
      if((BSIM3vsattemp < 1.0e3)) {
        {
          printf("Warning: Vsat at current temperature = %g may be too small.\n",((BSIM3vsattemp)*UNITFAD).val());
        }
      }
      if((BSIM3pdibl1 < 0.0)) {
        {
          printf("Warning: Pdibl1 = %g is negative.\n",((BSIM3pdibl1)*UNITFAD).val());
        }
      }
      if((BSIM3pdibl2 < 0.0)) {
        {
          printf("Warning: Pdibl2 = %g is negative.\n",((BSIM3pdibl2)*UNITFAD).val());
        }
      }
      if((BSIM3cgdo < 0.0)) {
        {
          printf("Warning: cgdo = %g is negative. Set to zero.\n",((BSIM3cgdo)*UNITFAD).val());
          BSIM3cgdo = ((0.0)*UNITFAD).val();
        }
      }
      if((BSIM3cgso < 0.0)) {
        {
          printf("Warning: cgso = %g is negative. Set to zero.\n",((BSIM3cgso)*UNITFAD).val());
          BSIM3cgso = ((0.0)*UNITFAD).val();
        }
      }
      if((BSIM3cgbo < 0.0)) {
        {
          printf("Warning: cgbo = %g is negative. Set to zero.\n",((BSIM3cgbo)*UNITFAD).val());
          BSIM3cgbo = ((0.0)*UNITFAD).val();
        }
      }
    }
  }
  if((BSIM3stiMod >= 1)) {
    {
      if(((this->saref == -9.9999e-99) && (this->sa0 == -9.9999e-99))) {
        BSIM3sa0 = ((1.0e-6)*UNITFAD).val();
      }
          else if(((this->sa0 != -9.9999e-99) && (this->saref == -9.9999e-99))) {
        BSIM3sa0 = ((this->sa0)*UNITFAD).val();
      }
      if(((this->sbref == -9.9999e-99) && (this->sb0 == -9.9999e-99))) {
        BSIM3sb0 = ((1.0e-6)*UNITFAD).val();
      }
          else if(((this->sb0 != -9.9999e-99) && (this->sbref == -9.9999e-99))) {
        BSIM3sb0 = ((this->sb0)*UNITFAD).val();
      }
      if((BSIM3stiMod == 1)) {
        {
          if(((BSIM3sa > 0.0) && (BSIM3sb > 0.0))) {
            {
              if((BSIM3sa0 <= 0.0)) {
                {
                  printf("Fatal: SAref(SA0) = %g is not positive.\n",((BSIM3sa0)*UNITFAD).val());
                  Fatal_Flag = ((1)*UNITFAD).val();
                }
              }
              if((BSIM3sb0 <= 0.0)) {
                {
                  printf("Fatal: SBref(SB0) = %g is not positive.\n",((BSIM3sb0)*UNITFAD).val());
                  Fatal_Flag = ((1)*UNITFAD).val();
                }
              }
              if((BSIM3wlod < 0.0)) {
                {
                  {
                    printf("Warning: WLOD = %g is less than 0. Set to 0.0.\n",((BSIM3wlod)*UNITFAD).val());
                  }
                  BSIM3wlod = ((0.0)*UNITFAD).val();
                }
              }
              if((BSIM3kvsat < -1.0)) {
                {
                  {
                    printf("Warning: KVSAT = %g is too small; Reset to -1.0.\n",((BSIM3kvsat)*UNITFAD).val());
                  }
                  BSIM3kvsat = ((-1.0)*UNITFAD).val();
                }
              }
              if((BSIM3kvsat > 1.0)) {
                {
                  {
                    printf("Warning: KVSAT = %g is too big; Reset to 1.0.\n",((BSIM3kvsat)*UNITFAD).val());
                  }
                  BSIM3kvsat = ((1.0)*UNITFAD).val();
                }
              }
              if((BSIM3lodk2 <= 0.0)) {
                {
                  {
                    printf("Warning: LODK2 = %g is not positive.\n",((BSIM3lodk2)*UNITFAD).val());
                  }
                }
              }
              if((BSIM3lodeta0 <= 0.0)) {
                printf("Warning: LODETA0 = %g is not positive.\n",((BSIM3lodeta0)*UNITFAD).val());
              }
            }
          }
        }
      }
          else if((BSIM3stiMod == 2)) {
        {
          if((BSIM3sa1Given && BSIM3saGiven)) {
            {
              printf("Warning: SA = %g is overwritten by SA1 = %g.\n",((BSIM3sa)*UNITFAD).val(),((BSIM3sa1)*UNITFAD).val());
            }
          }
          if((BSIM3sb1Given && BSIM3sbGiven)) {
            {
              printf("Warning: SB = %g is overwritten by SB1 = %g.\n",((BSIM3sb)*UNITFAD).val(),((BSIM3sb1)*UNITFAD).val());
            }
          }
          if(((!BSIM3sa1Given && BSIM3sb1Given) || (BSIM3sa1Given && !BSIM3sb1Given))) {
            {
              printf("Fatal: SA1 and SB1 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa2Given && BSIM3sb2Given) || (BSIM3sa2Given && !BSIM3sb2Given))) {
            {
              printf("Fatal: SA2 and SB2 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa3Given && BSIM3sb3Given) || (BSIM3sa3Given && !BSIM3sb3Given))) {
            {
              printf("Fatal: SA3 and SB3 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa4Given && BSIM3sb4Given) || (BSIM3sa4Given && !BSIM3sb4Given))) {
            {
              printf("Fatal: SA4 and SB4 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa5Given && BSIM3sb5Given) || (BSIM3sa5Given && !BSIM3sb5Given))) {
            {
              printf("Fatal: SA5 and SB5 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa6Given && BSIM3sb6Given) || (BSIM3sa6Given && !BSIM3sb6Given))) {
            {
              printf("Fatal: SA6 and SB6 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa7Given && BSIM3sb7Given) || (BSIM3sa7Given && !BSIM3sb7Given))) {
            {
              printf("Fatal: SA7 and SB7 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa8Given && BSIM3sb8Given) || (BSIM3sa8Given && !BSIM3sb8Given))) {
            {
              printf("Fatal: SA8 and SB8 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa9Given && BSIM3sb9Given) || (BSIM3sa9Given && !BSIM3sb9Given))) {
            {
              printf("Fatal: SA9 and SB9 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((!BSIM3sa10Given && BSIM3sb10Given) || (BSIM3sa10Given && !BSIM3sb10Given))) {
            {
              printf("Fatal: SA10 and SB10 have to be given in a pair.\n");
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa0 <= 0.0)) {
            {
              printf("Fatal: SA0 = %g is not positive.\n",((BSIM3sa0)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb0 <= 0.0)) {
            {
              printf("Fatal: SB0 = %g is not positive.\n",((BSIM3sb0)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3wlod < 0.0)) {
            {
              printf("Fatal: WLOD = %g is less than 0.\n",((BSIM3wlod)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if(((BSIM3kvsat < -1.0) || (BSIM3kvsat > 1.0))) {
            {
              printf("Fatal: KVSAT = %g is out of range.\n",((BSIM3kvsat)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3llodku0Given && (BSIM3llodku0 <= 0.0))) {
            {
              printf("Warning: LLODKU0 = %g is not positive.\n",((BSIM3llodku0)*UNITFAD).val());
            }
          }
          if((BSIM3wlodku0Given && (BSIM3wlodku0 <= 0.0))) {
            {
              {
                printf("Warning: WLODKU0 = %g is not positive.\n",((BSIM3wlodku0)*UNITFAD).val());
              }
            }
          }
          if((BSIM3llodvthGiven && (BSIM3llodvth <= 0.0))) {
            {
              printf("Warning: LLODVTH = %g is not positive.\n",((BSIM3llodvth)*UNITFAD).val());
            }
          }
          if((BSIM3wlodvthGiven && (BSIM3wlodvth <= 0.0))) {
            {
              {
                printf("Warning: WLODVTH = %g is not positive.\n",((BSIM3wlodvth)*UNITFAD).val());
              }
            }
          }
          if((BSIM3lodk2 <= 0.0)) {
            {
              printf("Fatal: LODK2 = %g is not positive.\n",((BSIM3lodk2)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3lodeta0 <= 0.0)) {
            {
              printf("Fatal: LODETA0 = %g is not positive.\n",((BSIM3lodeta0)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3saGiven && (BSIM3sa <= 0))) {
            {
              printf("Warning: SA = %g is not positive.\n",((BSIM3sa)*UNITFAD).val());
            }
          }
          if((BSIM3sa1Given && (BSIM3sa1 <= 0))) {
            {
              printf("Fatal: SA1 = %g is not positive.\n",((BSIM3sa1)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa2Given && (BSIM3sa2 <= 0))) {
            {
              printf("Fatal: SA2 = %g is not positive.\n",((BSIM3sa2)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa3Given && (BSIM3sa3 <= 0))) {
            {
              printf("Fatal: SA3 = %g is not positive.\n",((BSIM3sa3)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa4Given && (BSIM3sa4 <= 0))) {
            {
              printf("Fatal: SA4 = %g is not positive.\n",((BSIM3sa4)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa5Given && (BSIM3sa5 <= 0))) {
            {
              printf("Fatal: SA5 = %g is not positive.\n",((BSIM3sa5)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa6Given && (BSIM3sa6 <= 0))) {
            {
              printf("Fatal: SA6 = %g is not positive.\n",((BSIM3sa6)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa7Given && (BSIM3sa7 <= 0))) {
            {
              printf("Fatal: SA7 = %g is not positive.\n",((BSIM3sa7)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa8Given && (BSIM3sa8 <= 0))) {
            {
              printf("Fatal: SA8 = %g is not positive.\n",((BSIM3sa8)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa9Given && (BSIM3sa9 <= 0))) {
            {
              printf("Fatal: SA9 = %g is not positive.\n",((BSIM3sa9)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sa10Given && (BSIM3sa10 <= 0))) {
            {
              printf("Fatal: SA10 = %g is not positive.\n",((BSIM3sa10)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sbGiven && (BSIM3sb <= 0))) {
            {
              {
                printf("Warning: SB = %g is not positive.\n",((BSIM3sb)*UNITFAD).val());
              }
            }
          }
          if((BSIM3sb1Given && (BSIM3sb1 <= 0))) {
            {
              printf("Fatal: SB1 = %g is not positive.\n",((BSIM3sb1)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb2Given && (BSIM3sb2 <= 0))) {
            {
              printf("Fatal: SB2 = %g is not positive.\n",((BSIM3sb2)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb3Given && (BSIM3sb3 <= 0))) {
            {
              printf("Fatal: SB3 = %g is not positive.\n",((BSIM3sb3)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb4Given && (BSIM3sb4 <= 0))) {
            {
              printf("Fatal: SB4 = %g is not positive.\n",((BSIM3sb4)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb5Given && (BSIM3sb5 <= 0))) {
            {
              printf("Fatal: SB5 = %g is not positive.\n",((BSIM3sb5)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb6Given && (BSIM3sb6 <= 0))) {
            {
              printf("Fatal: SB6 = %g is not positive.\n",((BSIM3sb6)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb7Given && (BSIM3sb7 <= 0))) {
            {
              printf("Fatal: SB7 = %g is not positive.\n",((BSIM3sb7)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb8Given && (BSIM3sb8 <= 0))) {
            {
              printf("Fatal: SB8 = %g is not positive.\n",((BSIM3sb8)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb9Given && (BSIM3sb9 <= 0))) {
            {
              printf("Fatal: SB9 = %g is not positive.\n",((BSIM3sb9)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sb10Given && (BSIM3sb10 <= 0))) {
            {
              printf("Fatal: SB10 = %g is not positive.\n",((BSIM3sb10)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw1Given && (BSIM3sw1 <= 0))) {
            {
              printf("Fatal: SW1 = %g is not positive.\n",((BSIM3sw1)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw2Given && (BSIM3sw2 <= 0))) {
            {
              printf("Fatal: SW2 = %g is not positive.\n",((BSIM3sw2)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw3Given && (BSIM3sw3 <= 0))) {
            {
              printf("Fatal: SW3 = %g is not positive.\n",((BSIM3sw3)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw4Given && (BSIM3sw4 <= 0))) {
            {
              printf("Fatal: SW4 = %g is not positive.\n",((BSIM3sw4)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw5Given && (BSIM3sw5 <= 0))) {
            {
              printf("Fatal: SW5 = %g is not positive.\n",((BSIM3sw5)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw6Given && (BSIM3sw6 <= 0))) {
            {
              printf("Fatal: SW6 = %g is not positive.\n",((BSIM3sw6)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw7Given && (BSIM3sw7 <= 0))) {
            {
              printf("Fatal: SW7 = %g is not positive.\n",((BSIM3sw7)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw8Given && (BSIM3sw8 <= 0))) {
            {
              printf("Fatal: SW8 = %g is not positive.\n",((BSIM3sw8)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw9Given && (BSIM3sw9 <= 0))) {
            {
              printf("Fatal: SW9 = %g is not positive.\n",((BSIM3sw9)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3sw10Given && (BSIM3sw10 <= 0))) {
            {
              printf("Fatal: SW10 = %g is not positive.\n",((BSIM3sw10)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          tmp = (((((((((((BSIM3sw1+BSIM3sw2)+BSIM3sw3)+BSIM3sw4)+BSIM3sw5)+BSIM3sw6)+BSIM3sw7)+BSIM3sw8)+BSIM3sw9)+BSIM3sw10))*UNITFAD).val();
          tmp1 = ((this->w)*UNITFAD).val();
          if(((fabs((tmp-tmp1))/tmp1) > 0.01)) {
            {
              printf("Warning: Sum of SW = %g is not equal to W = %g.\n",((tmp)*UNITFAD).val(),this->w);
            }
          }
        }
      }
    }
  }
  if((this->version > 3.219)) {
    {
      if(BSIM3igbMod) {
        {
          if((BSIM3nigbinv <= 0.0)) {
            {
              printf("Fatal: nigbinv = %g is non-positive.\n",((BSIM3nigbinv)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3nigbacc <= 0.0)) {
            {
              printf("Fatal: nigbacc = %g is non-positive.\n",((BSIM3nigbacc)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
        }
      }
      if(BSIM3igcMod) {
        {
          if((BSIM3nigc <= 0.0)) {
            {
              printf("Fatal: nigc = %g is non-positive.\n",((BSIM3nigc)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3poxedge <= 0.0)) {
            {
              printf("Fatal: poxedge = %g is non-positive.\n",((BSIM3poxedge)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
          if((BSIM3pigcd <= 0.0)) {
            {
              printf("Fatal: pigcd = %g is non-positive.\n",((BSIM3pigcd)*UNITFAD).val());
              Fatal_Flag = ((1)*UNITFAD).val();
            }
          }
        }
      }
      if((BSIM3toxe <= 0.0)) {
        {
          printf("Fatal: Toxe = %g(Tox=%g) is not positive.\n",((BSIM3toxe)*UNITFAD).val(),((BSIM3tox)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3toxe < 1.0e-10)) {
        {
          printf("Warning: Toxe = %g is less than 1A. Recommended Toxe >= 5A\n",((BSIM3toxe)*UNITFAD).val());
        }
      }
      if((BSIM3toxref <= 0.0)) {
        {
          printf("Fatal: Toxe = %g is not positive.\n",((BSIM3toxref)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
    }
  }
  BSIM3cgdo = ((((BSIM3cgdo+BSIM3cf)*BSIM3weffCV))*UNITFAD).val();
  BSIM3cgso = ((((BSIM3cgso+BSIM3cf)*BSIM3weffCV))*UNITFAD).val();
  BSIM3cgbo = (((BSIM3cgbo*BSIM3leffCV))*UNITFAD).val();
  if((!BSIM3npeakGiven && BSIM3gamma1Given)) {
    {
      T0 = (((BSIM3gamma1*BSIM3cox))*UNITFAD).val();
      BSIM3npeak = ((((3.021E22*T0)*T0))*UNITFAD).val();
      printf("\nWARNING: npeak not given, gamma1 given: npeak=%g",((BSIM3npeak)*UNITFAD).val());
    }
  }
  BSIM3phi = ((((2.0*Vtm0)*log((BSIM3npeak/ni))))*UNITFAD).val();
  BSIM3sqrtPhi = ((cogenda_ternary_op<CogendaFadType>((BSIM3phi > 0.0),sqrt(BSIM3phi),0.0))*UNITFAD).val();
  BSIM3phis3 = (((BSIM3sqrtPhi*BSIM3phi))*UNITFAD).val();
  BSIM3Xdep0 = (((cogenda_ternary_op<CogendaFadType>((((2.0*1.03594e-10)/((1.602176462e-19*BSIM3npeak)*1.0e6)) > 0.0),sqrt(((2.0*1.03594e-10)/((1.602176462e-19*BSIM3npeak)*1.0e6))),0.0)*BSIM3sqrtPhi))*UNITFAD).val();
  BSIM3litl = ((cogenda_ternary_op<CogendaFadType>((((3.0*BSIM3xj)*BSIM3tox) > 0.0),sqrt(((3.0*BSIM3xj)*BSIM3tox)),0.0))*UNITFAD).val();
  BSIM3vbi = (((Vtm0*log(((1.0e20*BSIM3npeak)/(ni*ni)))))*UNITFAD).val();
  BSIM3cdep0 = ((cogenda_ternary_op<CogendaFadType>(((((((1.602176462e-19*1.03594e-10)*BSIM3npeak)*1.0e6)/2.0)/BSIM3phi) > 0.0),sqrt((((((1.602176462e-19*1.03594e-10)*BSIM3npeak)*1.0e6)/2.0)/BSIM3phi)),0.0))*UNITFAD).val();
  BSIM3ldeb = (((cogenda_ternary_op<CogendaFadType>((((1.03594e-10*Vtm0)/((1.602176462e-19*BSIM3npeak)*1.0e6)) > 0.0),sqrt(((1.03594e-10*Vtm0)/((1.602176462e-19*BSIM3npeak)*1.0e6))),0.0)/3.0))*UNITFAD).val();
  BSIM3acde = (((BSIM3acde*pow((BSIM3npeak/2.0e16),-0.25)))*UNITFAD).val();
  if((BSIM3k1Given || BSIM3k2Given)) {
    {
      if(!BSIM3k1Given) {
        {
          printf("\nWarning: k1 should be specified with k2.");
          BSIM3k1 = ((0.53)*UNITFAD).val();
        }
      }
      if(!BSIM3k2Given) {
        {
          printf("\nWarning: k2 should be specified with k1.");
          BSIM3k2 = ((-0.0186)*UNITFAD).val();
        }
      }
      if(BSIM3nsubGiven) {
        printf("\nWarning: \
                           nsub is ignored because k1 or k2 is given.");
      }
      if(BSIM3xtGiven) {
        printf("\nWarning: \
                           xt is ignored because k1 or k2 is given.");
      }
      if(BSIM3vbxGiven) {
        printf("\nWarning: \
                           vbx is ignored because k1 or k2 is given.");
      }
      if(BSIM3gamma1Given) {
        printf("\nWarning: \
                           gamma1 is ignored because k1 or k2 is given.");
      }
      if(BSIM3gamma2Given) {
        printf("\nWarning: \
                           gamma2 is ignored because k1 or k2 is given.");
      }
    }
  }
  else {
    {
      if(!BSIM3vbxGiven) {
        BSIM3vbx = (((BSIM3phi-(((7.7348e-4*BSIM3npeak)*BSIM3xt)*BSIM3xt)))*UNITFAD).val();
      }
      if((BSIM3vbx > 0.0)) {
        BSIM3vbx = ((-BSIM3vbx)*UNITFAD).val();
      }
      if((BSIM3vbm > 0.0)) {
        BSIM3vbm = ((-BSIM3vbm)*UNITFAD).val();
      }
      if(!BSIM3gamma1Given) {
        BSIM3gamma1 = ((((5.753e-12*cogenda_ternary_op<CogendaFadType>((BSIM3npeak > 0.0),sqrt(BSIM3npeak),0.0))/BSIM3cox))*UNITFAD).val();
      }
      if(!BSIM3gamma2Given) {
        BSIM3gamma2 = ((((5.753e-12*cogenda_ternary_op<CogendaFadType>((BSIM3nsub > 0.0),sqrt(BSIM3nsub),0.0))/BSIM3cox))*UNITFAD).val();
      }
      T0 = (((BSIM3gamma1-BSIM3gamma2))*UNITFAD).val();
      T1 = (((cogenda_ternary_op<CogendaFadType>(((BSIM3phi-BSIM3vbx) > 0.0),sqrt((BSIM3phi-BSIM3vbx)),0.0)-BSIM3sqrtPhi))*UNITFAD).val();
      T2 = (((cogenda_ternary_op<CogendaFadType>(((BSIM3phi*(BSIM3phi-BSIM3vbm)) > 0.0),sqrt((BSIM3phi*(BSIM3phi-BSIM3vbm))),0.0)-BSIM3phi))*UNITFAD).val();
      BSIM3k2 = ((((T0*T1)/((2.0*T2)+BSIM3vbm)))*UNITFAD).val();
      BSIM3k1 = (((BSIM3gamma2-((2.0*BSIM3k2)*cogenda_ternary_op<CogendaFadType>(((BSIM3phi-BSIM3vbm) > 0.0),sqrt((BSIM3phi-BSIM3vbm)),0.0))))*UNITFAD).val();
    }
  }
  BSIM3k1 = (((BSIM3k1+this->delk1))*UNITFAD).val();
  BSIM3nfactor = (((BSIM3nfactor+this->delnfct))*UNITFAD).val();
  if((BSIM3k2 < 0.0)) {
    {
      T0 = ((((0.5*BSIM3k1)/BSIM3k2))*UNITFAD).val();
      BSIM3vbsc = (((0.9*(BSIM3phi-(T0*T0))))*UNITFAD).val();
      if((BSIM3vbsc > -3.0)) {
        BSIM3vbsc = ((-3.0)*UNITFAD).val();
      }
          else if((BSIM3vbsc < -30.0)) {
        BSIM3vbsc = ((-30.0)*UNITFAD).val();
      }
    }
  }
  else {
    BSIM3vbsc = ((-30.0)*UNITFAD).val();
  }
  if((BSIM3vbsc > BSIM3vbm)) {
    BSIM3vbsc = ((BSIM3vbm)*UNITFAD).val();
  }
  if((this->vfb == -9.9999e-99)) {
    {
      if(BSIM3vth0Given) {
        BSIM3vfb = (((((BSIM3type*BSIM3vth0)-BSIM3phi)-(BSIM3k1*BSIM3sqrtPhi)))*UNITFAD).val();
      }
      else {
        BSIM3vfb = ((-1.0)*UNITFAD).val();
      }
    }
  }
  BSIM3vfb = (((BSIM3vfb+this->delvto))*UNITFAD).val();
  if(!BSIM3vth0Given) {
    {
      BSIM3vth0 = (((BSIM3type*((BSIM3vfb+BSIM3phi)+(BSIM3k1*BSIM3sqrtPhi))))*UNITFAD).val();
      printf(" \nvth0 calculated: vth0=%g",((BSIM3vth0)*UNITFAD).val());
    }
  }
  else {
    BSIM3vth0 = (((BSIM3vth0+this->delvto))*UNITFAD).val();
  }
  BSIM3k1ox = ((((BSIM3k1*BSIM3tox)/BSIM3toxm))*UNITFAD).val();
  BSIM3k2ox = ((((BSIM3k2*BSIM3tox)/BSIM3toxm))*UNITFAD).val();
  if((this->vfbflag != 0.0)) {
    BSIM3vfbcv = ((BSIM3vfb)*UNITFAD).val();
  }
  T1 = ((cogenda_ternary_op<CogendaFadType>(((((1.03594e-10/3.453133e-11)*BSIM3tox)*BSIM3Xdep0) > 0.0),sqrt((((1.03594e-10/3.453133e-11)*BSIM3tox)*BSIM3Xdep0)),0.0))*UNITFAD).val();
  T0 = ((exp((((-0.5*BSIM3dsub)*BSIM3leff)/T1)))*UNITFAD).val();
  BSIM3theta0vb0 = (((T0+((2.0*T0)*T0)))*UNITFAD).val();
  T2 = (((((-0.5*BSIM3drout)*BSIM3leff)/T1))*UNITFAD).val();
  if((T2 > -40)) {
    T0 = ((exp(T2))*UNITFAD).val();
  }
  else {
    T0 = ((4.2483542552916e-18)*UNITFAD).val();
  }
  T2 = (((T0+((2.0*T0)*T0)))*UNITFAD).val();
  BSIM3thetaRout = ((((BSIM3pdibl1*T2)+BSIM3pdibl2))*UNITFAD).val();
  tmp = ((cogenda_ternary_op<CogendaFadType>((BSIM3Xdep0 > 0.0),sqrt(BSIM3Xdep0),0.0))*UNITFAD).val();
  tmp1 = (((BSIM3vbi-BSIM3phi))*UNITFAD).val();
  tmp2 = (((BSIM3factor1*tmp))*UNITFAD).val();
  T0 = ((((((-0.5*BSIM3dvt1w)*BSIM3weff)*BSIM3leff)/tmp2))*UNITFAD).val();
  if((T0 > -34.0)) {
    {
      if((T0 > 100)) {
        T1 = ((exp(100.0))*UNITFAD).val();
      }
      else {
        T1 = ((exp(T0))*UNITFAD).val();
      }
      T2 = (((T1*(1.0+(2.0*T1))))*UNITFAD).val();
    }
  }
  else {
    {
      T1 = ((1.713908431e-15)*UNITFAD).val();
      T2 = (((T1*(1.0+(2.0*T1))))*UNITFAD).val();
    }
  }
  T0 = (((BSIM3dvt0w*T2))*UNITFAD).val();
  T2 = (((T0*tmp1))*UNITFAD).val();
  T0 = (((((-0.5*BSIM3dvt1)*BSIM3leff)/tmp2))*UNITFAD).val();
  if((T0 > -34.0)) {
    {
      T1 = ((exp(T0))*UNITFAD).val();
      T3 = (((T1*(1.0+(2.0*T1))))*UNITFAD).val();
    }
  }
  else {
    {
      T1 = ((1.713908431e-15)*UNITFAD).val();
      T3 = (((T1*(1.0+(2.0*T1))))*UNITFAD).val();
    }
  }
  T3 = ((((BSIM3dvt0*T3)*tmp1))*UNITFAD).val();
  if((BSIM3capMod == 3)) {
    T4 = (((((BSIM3tox-this->dtoxcv)*BSIM3phi)/(BSIM3weff+BSIM3w0)))*UNITFAD).val();
  }
  else {
    T4 = ((((BSIM3tox*BSIM3phi)/(BSIM3weff+BSIM3w0)))*UNITFAD).val();
  }
  T0 = ((cogenda_ternary_op<CogendaFadType>(((1.0+(BSIM3nlx/BSIM3leff)) > 0.0),sqrt((1.0+(BSIM3nlx/BSIM3leff))),0.0))*UNITFAD).val();
  T5 = (((((BSIM3k1ox*(T0-1.0))*BSIM3sqrtPhi)+((BSIM3kt1+(BSIM3kt1l/BSIM3leff))*(TRatio-1.0))))*UNITFAD).val();
  tmp3 = (((((((BSIM3type*BSIM3vth0)-T2)-T3)+(BSIM3k3*T4))+T5))*UNITFAD).val();
  BSIM3vfbzb = ((((tmp3-BSIM3phi)-(BSIM3k1*BSIM3sqrtPhi)))*UNITFAD).val();
  if((BSIM3stiMod == 1)) {
    {
      T0 = ((pow(Lnew,BSIM3llodku0))*UNITFAD).val();
      W_tmp = (((Wnew+BSIM3wlod))*UNITFAD).val();
      T1 = ((pow(W_tmp,BSIM3wlodku0))*UNITFAD).val();
      tmp1 = (((((BSIM3lku0/T0)+(BSIM3wku0/T1))+(BSIM3pku0/(T0*T1))))*UNITFAD).val();
      T9 = (((1.0+tmp1))*UNITFAD).val();
      T0 = ((pow(Lnew,BSIM3llodvth))*UNITFAD).val();
      T1 = ((pow(W_tmp,BSIM3wlodvth))*UNITFAD).val();
      tmp1 = (((((BSIM3lkvth0/T0)+(BSIM3wkvth0/T1))+(BSIM3pkvth0/(T0*T1))))*UNITFAD).val();
      here_BSIM3kvth0 = (((1.0+tmp1))*UNITFAD).val();
      here_BSIM3kvth0 = ((cogenda_ternary_op<CogendaFadType>((((here_BSIM3kvth0*here_BSIM3kvth0)+1.0E-9) > 0.0),sqrt(((here_BSIM3kvth0*here_BSIM3kvth0)+1.0E-9)),0.0))*UNITFAD).val();
      T0 = (((TRatio-1.0))*UNITFAD).val();
      BSIM3ku0temp = ((((T9*(1.0+(BSIM3tku0*T0)))+1.0E-9))*UNITFAD).val();
      Inv_saref = (((1.0/(BSIM3sa0+(0.5*Ldrn))))*UNITFAD).val();
      Inv_sbref = (((1.0/(BSIM3sb0+(0.5*Ldrn))))*UNITFAD).val();
      BSIM3inv_od_ref = (((Inv_saref+Inv_sbref))*UNITFAD).val();
      BSIM3rho_ref = ((((BSIM3ku0/BSIM3ku0temp)*BSIM3inv_od_ref))*UNITFAD).val();
      if((this->verbose == 1)) {
        {
          printf("STI:Inv_saref,Inv_sbref,Ldrn,BSIM3inv_od_ref=",((Inv_saref)*UNITFAD).val(),((Inv_sbref)*UNITFAD).val(),((Ldrn)*UNITFAD).val(),((BSIM3inv_od_ref)*UNITFAD).val());
        }
      }
      T0 = (((BSIM3leffCV*BSIM3leffCV))*UNITFAD).val();
      BSIM3tconst = ((((BSIM3u0temp*BSIM3elm)/(((BSIM3cox*BSIM3weffCV)*BSIM3leffCV)*T0)))*UNITFAD).val();
    }
  }
  else if((BSIM3stiMod == 2)) {
    {
      T0 = ((pow(Lnew,BSIM3llodku0))*UNITFAD).val();
      W_tmp = (((Wnew+BSIM3wlod))*UNITFAD).val();
      T1 = ((pow(W_tmp,BSIM3wlodku0))*UNITFAD).val();
      tmp1 = (((((BSIM3lku0/T0)+(BSIM3wku0/T1))+(BSIM3pku0/(T0*T1))))*UNITFAD).val();
      T9 = (((1.0+tmp1))*UNITFAD).val();
      T0 = ((pow(Lnew,BSIM3llodvth))*UNITFAD).val();
      T1 = ((pow(W_tmp,BSIM3wlodvth))*UNITFAD).val();
      tmp1 = (((((BSIM3lkvth0/T0)+(BSIM3wkvth0/T1))+(BSIM3pkvth0/(T0*T1))))*UNITFAD).val();
      here_BSIM3kvth0 = (((1.0+tmp1))*UNITFAD).val();
      here_BSIM3kvth0 = ((cogenda_ternary_op<CogendaFadType>((((here_BSIM3kvth0*here_BSIM3kvth0)+1.0E-9) > 0.0),sqrt(((here_BSIM3kvth0*here_BSIM3kvth0)+1.0E-9)),0.0))*UNITFAD).val();
      T0 = (((TRatio-1.0))*UNITFAD).val();
      BSIM3ku0temp = (((T9*(1+(BSIM3tku0*T0))))*UNITFAD).val();
      BSIM3ku0temp = (((BSIM3ku0temp+1.0E-9))*UNITFAD).val();
      Inv_sa0 = (((1/(BSIM3sa0+(0.5*Ldrn))))*UNITFAD).val();
      Inv_sb0 = (((1/(BSIM3sb0+(0.5*Ldrn))))*UNITFAD).val();
      BSIM3inv_od_def = (((Inv_sa0+Inv_sb0))*UNITFAD).val();
      BSIM3rho_def = ((((BSIM3ku0/BSIM3ku0temp)*BSIM3inv_od_def))*UNITFAD).val();
      T0 = (((BSIM3leffCV*BSIM3leffCV))*UNITFAD).val();
      BSIM3tconst = ((((BSIM3u0temp*BSIM3elm)/(((BSIM3cox*BSIM3weffCV)*BSIM3leffCV)*T0)))*UNITFAD).val();
    }
  }
  Inv_sa = ((0)*UNITFAD).val();
  Inv_sb = ((0)*UNITFAD).val();
  LOD_flag = ((0)*UNITFAD).val();
  if((BSIM3stiMod == 2)) {
    {
      if(((BSIM3sa1 > 0.0) && (BSIM3sb1 > 0.0))) {
        {
          tmp = (((BSIM3sw1/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa1+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb1+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa2 > 0.0) && (BSIM3sb2 > 0.0))) {
        {
          tmp = (((BSIM3sw2/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa2+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb2+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa3 > 0.0) && (BSIM3sb3 > 0.0))) {
        {
          tmp = (((BSIM3sw3/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa3+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb3+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa4 > 0.0) && (BSIM3sb4 > 0.0))) {
        {
          tmp = (((BSIM3sw4/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa4+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb4+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa5 > 0.0) && (BSIM3sb5 > 0.0))) {
        {
          tmp = (((BSIM3sw5/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa5+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb5+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa6 > 0.0) && (BSIM3sb6 > 0.0))) {
        {
          tmp = (((BSIM3sw6/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa6+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb6+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa7 > 0.0) && (BSIM3sb7 > 0.0))) {
        {
          tmp = (((BSIM3sw7/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa7+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb7+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa8 > 0.0) && (BSIM3sb8 > 0.0))) {
        {
          tmp = (((BSIM3sw8/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa8+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb8+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa9 > 0.0) && (BSIM3sb9 > 0.0))) {
        {
          tmp = (((BSIM3sw9/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa9+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb9+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
      if(((BSIM3sa10 > 0.0) && (BSIM3sb10 > 0.0))) {
        {
          tmp = (((BSIM3sw10/this->w))*UNITFAD).val();
          T0 = (((tmp/(BSIM3sa10+(0.5*Ldrn))))*UNITFAD).val();
          T1 = (((tmp/(BSIM3sb10+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sa = (((Inv_sa+T0))*UNITFAD).val();
          Inv_sb = (((Inv_sb+T1))*UNITFAD).val();
          LOD_flag = ((1)*UNITFAD).val();
        }
      }
    }
  }
  if((LOD_flag == 1)) {
    {
      Inv_ODeff = (((Inv_sa+Inv_sb))*UNITFAD).val();
      rho = ((((BSIM3ku0/BSIM3ku0temp)*Inv_ODeff))*UNITFAD).val();
      T0 = ((((1+rho)/(1+BSIM3rho_def)))*UNITFAD).val();
      BSIM3u0temp = (((BSIM3u0temp*T0))*UNITFAD).val();
      T1 = ((((1+(BSIM3kvsat*rho))/(1+(BSIM3kvsat*BSIM3rho_def))))*UNITFAD).val();
      BSIM3vsattemp = (((BSIM3vsattemp*T1))*UNITFAD).val();
      OD_offset = (((Inv_ODeff-BSIM3inv_od_def))*UNITFAD).val();
      dvth0_lod = ((((BSIM3kvth0/here_BSIM3kvth0)*OD_offset))*UNITFAD).val();
      dk2_lod = ((((BSIM3stk2/pow(here_BSIM3kvth0,BSIM3lodk2))*OD_offset))*UNITFAD).val();
      deta0_lod = ((((BSIM3steta0/pow(here_BSIM3kvth0,BSIM3lodeta0))*OD_offset))*UNITFAD).val();
      BSIM3k2 = (((BSIM3k2+dk2_lod))*UNITFAD).val();
      if((!BSIM3k1Given && !BSIM3k2Given)) {
        {
          dk1_lod = ((((-2.0*dk2_lod)*cogenda_ternary_op<CogendaFadType>(((BSIM3phi-BSIM3vbm) > 0.0),sqrt((BSIM3phi-BSIM3vbm)),0.0)))*UNITFAD).val();
          BSIM3k1 = (((BSIM3k1+dk1_lod))*UNITFAD).val();
        }
      }
      else {
        {
          dk1_lod = ((0.0)*UNITFAD).val();
          BSIM3k1 = ((BSIM3k1)*UNITFAD).val();
        }
      }
      if((this->version > 3.021)) {
        {
          if((BSIM3k2 < 0.0)) {
            {
              T0 = ((((0.5*BSIM3k1)/BSIM3k2))*UNITFAD).val();
              BSIM3vbsc = (((0.9*(BSIM3phi-(T0*T0))))*UNITFAD).val();
              if((BSIM3vbsc > -3.0)) {
                BSIM3vbsc = ((-3.0)*UNITFAD).val();
              }
                          else if((BSIM3vbsc < -30.0)) {
                BSIM3vbsc = ((-30.0)*UNITFAD).val();
              }
            }
          }
          else {
            BSIM3vbsc = ((-30.0)*UNITFAD).val();
          }
          if((BSIM3vbsc > BSIM3vbm)) {
            BSIM3vbsc = ((BSIM3vbm)*UNITFAD).val();
          }
        }
      }
      else {
        {
          if((BSIM3k2 > 0.0)) {
            {
              T0 = ((((0.5*BSIM3k1)/BSIM3k2))*UNITFAD).val();
              BSIM3vbsc = (((0.9*(BSIM3phi-(T0*T0))))*UNITFAD).val();
              if((BSIM3vbsc > -3.0)) {
                BSIM3vbsc = ((-3.0)*UNITFAD).val();
              }
                          else if((BSIM3vbsc < -30.0)) {
                BSIM3vbsc = ((-30.0)*UNITFAD).val();
              }
            }
          }
                  else if((this->version < 3.001)) {
            BSIM3vbsc = ((-10.0)*UNITFAD).val();
          }
          else {
            BSIM3vbsc = ((-30.0)*UNITFAD).val();
          }
        }
      }
      BSIM3vth0 = (((BSIM3vth0+dvth0_lod))*UNITFAD).val();
      if(!BSIM3vth0Given) {
        BSIM3vth0 = (((BSIM3vth0+(BSIM3type*dk1_lod)))*UNITFAD).val();
      }
      if(!BSIM3vfbGiven) {
        {
          if(BSIM3vth0Given) {
            BSIM3vfb = ((((BSIM3vfb+(BSIM3type*dvth0_lod))-dk1_lod))*UNITFAD).val();
          }
        }
      }
      T3 = (((BSIM3leffCV*BSIM3leffCV))*UNITFAD).val();
      BSIM3tconst = ((((BSIM3u0temp*BSIM3elm)/(((BSIM3cox*BSIM3weffCV)*BSIM3leffCV)*T3)))*UNITFAD).val();
      BSIM3k1ox = ((((BSIM3k1*BSIM3tox)/BSIM3toxm))*UNITFAD).val();
      BSIM3k2ox = ((((BSIM3k2*BSIM3tox)/BSIM3toxm))*UNITFAD).val();
      T0 = ((cogenda_ternary_op<CogendaFadType>(((1.0+(BSIM3nlx/BSIM3leff)) > 0.0),sqrt((1.0+(BSIM3nlx/BSIM3leff))),0.0))*UNITFAD).val();
      BSIM3vfbzb = (((((BSIM3vfbzb+(((BSIM3k1ox-BSIM3k1ox)*(T0-1.0))*BSIM3sqrtPhi))+(BSIM3type*(BSIM3vth0-BSIM3vth0)))-(dk1_lod*BSIM3sqrtPhi)))*UNITFAD).val();
      BSIM3eta0 = (((BSIM3eta0+deta0_lod))*UNITFAD).val();
    }
  }
  if((BSIM3stiMod == 1)) {
    {
      if(((BSIM3sa > 0.0) && (BSIM3sb > 0.0))) {
        {
          Inv_sa = (((1.0/(BSIM3sa+(0.5*Ldrn))))*UNITFAD).val();
          Inv_sb = (((1.0/(BSIM3sb+(0.5*Ldrn))))*UNITFAD).val();
          Inv_ODeff = (((Inv_sa+Inv_sb))*UNITFAD).val();
          rho = ((((BSIM3ku0/BSIM3ku0temp)*Inv_ODeff))*UNITFAD).val();
          T0 = ((((1.0+rho)/(1.0+BSIM3rho_ref)))*UNITFAD).val();
          BSIM3u0temp = (((BSIM3u0temp*T0))*UNITFAD).val();
          T1 = ((((1.0+(BSIM3kvsat*rho))/(1.0+(BSIM3kvsat*BSIM3rho_ref))))*UNITFAD).val();
          BSIM3vsattemp = (((BSIM3vsattemp*T1))*UNITFAD).val();
          OD_offset = (((Inv_ODeff-BSIM3inv_od_ref))*UNITFAD).val();
          dvth0_lod = ((((BSIM3kvth0/here_BSIM3kvth0)*OD_offset))*UNITFAD).val();
          dk2_lod = ((((BSIM3stk2/pow(here_BSIM3kvth0,BSIM3lodk2))*OD_offset))*UNITFAD).val();
          deta0_lod = ((((BSIM3steta0/pow(here_BSIM3kvth0,BSIM3lodeta0))*OD_offset))*UNITFAD).val();
          BSIM3vth0 = (((BSIM3vth0+dvth0_lod))*UNITFAD).val();
          T3 = (((BSIM3leffCV*BSIM3leffCV))*UNITFAD).val();
          BSIM3tconst = ((((BSIM3u0temp*BSIM3elm)/(((BSIM3cox*BSIM3weffCV)*BSIM3leffCV)*T3)))*UNITFAD).val();
          if((!BSIM3vfbGiven && !BSIM3vth0Given)) {
            BSIM3vfb = ((-1.0)*UNITFAD).val();
          }
          else {
            BSIM3vfb = (((BSIM3vfb+(BSIM3type*dvth0_lod)))*UNITFAD).val();
          }
          BSIM3vfbzb = (((BSIM3vfbzb+(BSIM3type*dvth0_lod)))*UNITFAD).val();
          BSIM3k2 = (((BSIM3k2+dk2_lod))*UNITFAD).val();
          if((BSIM3k2 < 0.0)) {
            {
              T0 = ((((0.5*BSIM3k1)/BSIM3k2))*UNITFAD).val();
              BSIM3vbsc = (((0.9*(BSIM3phi-(T0*T0))))*UNITFAD).val();
              if((BSIM3vbsc > -3.0)) {
                BSIM3vbsc = ((-3.0)*UNITFAD).val();
              }
                          else if((BSIM3vbsc < -30.0)) {
                BSIM3vbsc = ((-30.0)*UNITFAD).val();
              }
            }
          }
          else {
            BSIM3vbsc = ((-30.0)*UNITFAD).val();
          }
          if((BSIM3vbsc > BSIM3vbm)) {
            BSIM3vbsc = ((BSIM3vbm)*UNITFAD).val();
          }
          BSIM3k2ox = ((((BSIM3k2*BSIM3tox)/BSIM3toxm))*UNITFAD).val();
          BSIM3eta0 = (((BSIM3eta0+deta0_lod))*UNITFAD).val();
        }
      }
    }
  }
  if(((BSIM3stiMod > 0) && (this->verbose == 1))) {
    {
      printf("STI:dvth0_lod,dk2_lod,deta0_lod=",((dvth0_lod)*UNITFAD).val(),((dk2_lod)*UNITFAD).val(),((deta0_lod)*UNITFAD).val());
      printf("STI:BSIM3u0temp,BSIM3vsattemp=",((BSIM3u0temp)*UNITFAD).val(),((BSIM3vsattemp)*UNITFAD).val());
      printf("STI:BSIM3kvth0,here_BSIM3kvth0,OD_offset=",((BSIM3kvth0)*UNITFAD).val(),((here_BSIM3kvth0)*UNITFAD).val(),((OD_offset)*UNITFAD).val());
    }
  }
  BSIM3drainResistance = ((rdpr)*UNITFAD).val();
  BSIM3sourceResistance = ((rspr)*UNITFAD).val();
  if((BSIM3drainResistance <= 0.0)) {
    BSIM3drainResistance = ((0)*UNITFAD).val();
  }
  if((BSIM3sourceResistance <= 0.0)) {
    BSIM3sourceResistance = ((0)*UNITFAD).val();
  }
  if((this->verbose == 1)) {
    {
      printf("rdpr,rspr, rdc, rsc,rsh=",((rdpr)*UNITFAD).val(),((rspr)*UNITFAD).val(),this->rdc,this->rsc,this->rsh);
      printf("nrd,nrs=",((BSIM3drainSquares)*UNITFAD).val(),((BSIM3sourceSquares)*UNITFAD).val());
    }
  }
  Nvtm = (((BSIM3vtm*BSIM3jctEmissionCoeff))*UNITFAD).val();
  if(((BSIM3sourceArea <= 0.0) && (BSIM3sourcePerimeter <= 0.0))) {
    SourceSatCurrent = ((1.0e-14)*UNITFAD).val();
  }
  else {
    SourceSatCurrent = ((((BSIM3sourceArea*BSIM3jctTempSatCurDensity)+(BSIM3sourcePerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
  }
  if(((SourceSatCurrent > 0.0) && (BSIM3ijth > 0.0))) {
    BSIM3vjsm = (((Nvtm*log(((BSIM3ijth/SourceSatCurrent)+1.0))))*UNITFAD).val();
  }
  else {
    BSIM3vjsm = ((0.0)*UNITFAD).val();
  }
  BSIM3IsEvjsm = (((SourceSatCurrent*_LIMEXP_(BSIM3vjsm/Nvtm)))*UNITFAD).val();
  if(((BSIM3drainArea <= 0.0) && (BSIM3drainPerimeter <= 0.0))) {
    DrainSatCurrent = ((1.0e-14)*UNITFAD).val();
  }
  else {
    DrainSatCurrent = ((((BSIM3drainArea*BSIM3jctTempSatCurDensity)+(BSIM3drainPerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
  }
  if(((DrainSatCurrent > 0.0) && (BSIM3ijth > 0.0))) {
    BSIM3vjdm = (((Nvtm*log(((BSIM3ijth/DrainSatCurrent)+1.0))))*UNITFAD).val();
  }
  else {
    BSIM3vjdm = ((0.0)*UNITFAD).val();
  }
  BSIM3IsEvjdm = (((DrainSatCurrent*AnalogFunctions::exp_lim<CogendaFadType>((BSIM3vjdm/Nvtm))))*UNITFAD).val();
  if((this->version >= 3.219999)) {
    {
      BSIM3igcMod = ((this->igcmod)*UNITFAD).val();
      BSIM3igbMod = ((this->igbmod)*UNITFAD).val();
      BSIM3tempMod = ((this->tempmod)*UNITFAD).val();
      if((((BSIM3igcMod != 0) && (BSIM3igcMod != 1)) && (BSIM3igcMod != 2))) {
        {
          BSIM3igcMod = ((0)*UNITFAD).val();
          printf("Warning: igcMod has been set to its default value: 0.\n");
        }
      }
      if(((BSIM3igbMod != 0) && (BSIM3igbMod != 1))) {
        {
          BSIM3igbMod = ((0)*UNITFAD).val();
          printf("Warning: igbMod has been set to its default value: 0.\n");
        }
      }
      if((BSIM3nigbinv <= 0.0)) {
        {
          printf("Fatal: nigbinv = %g is non-positive.\n",((BSIM3nigbinv)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3nigbacc <= 0.0)) {
        {
          printf("Fatal: nigbacc = %g is non-positive.\n",((BSIM3nigbacc)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3nigc <= 0.0)) {
        {
          printf("Fatal: nigc = %g is non-positive.\n",((BSIM3nigc)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3poxedge <= 0.0)) {
        {
          printf("Fatal: poxedge = %g is non-positive.\n",((BSIM3poxedge)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      if((BSIM3pigcd <= 0.0)) {
        {
          printf("Fatal: pigcd = %g is non-positive.\n",((BSIM3pigcd)*UNITFAD).val());
          Fatal_Flag = ((1)*UNITFAD).val();
        }
      }
      BSIM3vfbsdoff = (((BSIM3vfbsdoff*(1.0+(BSIM3tvfbsdoff*delTemp))))*UNITFAD).val();
      if((BSIM3ngate > 0.0)) {
        BSIM3vfbsd = (((Vtm0*log((BSIM3ngate/BSIM3nsd))))*UNITFAD).val();
      }
      else {
        BSIM3vfbsd = ((0.0)*UNITFAD).val();
      }
      BSIM3ToxRatio = ((((exp((BSIM3ntox*log((BSIM3toxref/BSIM3toxe))))/BSIM3toxe)/BSIM3toxe))*UNITFAD).val();
      BSIM3ToxRatioEdge = ((((((exp((BSIM3ntox*log((BSIM3toxref/(BSIM3toxe*BSIM3poxedge)))))/BSIM3toxe)/BSIM3toxe)/BSIM3poxedge)/BSIM3poxedge))*UNITFAD).val();
      BSIM3Aechvb = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),4.97232e-7,3.42537e-7))*UNITFAD).val();
      BSIM3Bechvb = ((cogenda_ternary_op<CogendaFadType>((BSIM3type == 1),7.45669e11,1.16645e12))*UNITFAD).val();
      BSIM3AechvbEdge = (((((BSIM3Aechvb*BSIM3weff)*BSIM3dlcig)*BSIM3ToxRatioEdge))*UNITFAD).val();
      BSIM3BechvbEdge = ((((-BSIM3Bechvb*BSIM3toxe)*BSIM3poxedge))*UNITFAD).val();
      BSIM3Aechvb = (((((BSIM3Aechvb*BSIM3weff)*BSIM3leff)*BSIM3ToxRatio))*UNITFAD).val();
      BSIM3Bechvb = (((-BSIM3Bechvb*BSIM3toxe))*UNITFAD).val();
      if((this->verbose == 1)) {
        {
          printf("BSIM3Aechvb=",((BSIM3Aechvb)*UNITFAD).val(),((BSIM3ToxRatio)*UNITFAD).val(),((BSIM3toxref)*UNITFAD).val(),((BSIM3toxe)*UNITFAD).val());
          printf("BSIM3AechvbEdge=",((BSIM3AechvbEdge)*UNITFAD).val(),((BSIM3dlcig)*UNITFAD).val(),((BSIM3Lint)*UNITFAD).val(),((BSIM3ToxRatioEdge)*UNITFAD).val());
        }
      }
    }
  }
  vds = (BSIM3type*probeVars[cogendaProbeID_V_di_si]);
  vbs = (BSIM3type*probeVars[cogendaProbeID_V_b_si]);
  vgs = (BSIM3type*probeVars[cogendaProbeID_V_g_si]);
  vbd = (vbs-vds);
  vgd = (vgs-vds);
  vgb = (vgs-vbs);
  if((vds >= 0.0)) {
    {
      BSIM3mode = ((1)*UNITFAD).val();
      Vds = vds;
      Vgs = vgs;
      Vbs = vbs;
    }
  }
  else {
    {
      BSIM3mode = ((-1)*UNITFAD).val();
      Vds = -vds;
      Vgs = vgd;
      Vbs = vbd;
    }
  }
  if((BSIM3acm < 9)) {
    {
      printf("Warn: ACM(%d)<9 may not full compatible with hspice...\n",BSIM3acm);
    }
  }
  Nvtm = (((BSIM3vtm*BSIM3jctEmissionCoeff))*UNITFAD).val();
  if(((BSIM3sourceArea <= 0.0) && (BSIM3sourcePerimeter <= 0.0))) {
    SourceSatCurrent = ((1.0e-14)*UNITFAD).val();
  }
  else {
    SourceSatCurrent = ((((BSIM3sourceArea*BSIM3jctTempSatCurDensity)+(BSIM3sourcePerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
  }
  if((SourceSatCurrent <= 0.0)) {
    {
      BSIM3cbs = (this->_ckt_gmin*vbs);
    }
  }
  else {
    {
      if((BSIM3ijth == 0.0)) {
        {
          evbs = AnalogFunctions::exp_lim<CogendaFadType>((vbs/Nvtm));
          BSIM3cbs = ((SourceSatCurrent*(evbs-1.0))+(this->_ckt_gmin*vbs));
        }
      }
      else {
        {
          if((BSIM3breakmod == 0)) {
            {
              if((vbs < BSIM3vjsm)) {
                {
                  evbs = AnalogFunctions::exp_lim<CogendaFadType>((vbs/Nvtm));
                  BSIM3cbs = ((SourceSatCurrent*(evbs-1.0))+(this->_ckt_gmin*vbs));
                }
              }
                          else if((this->version >= 3.21)) {
                {
                  T0 = (((BSIM3IsEvjsm/Nvtm))*UNITFAD).val();
                  BSIM3cbs = (((BSIM3IsEvjsm-SourceSatCurrent)+(T0*(vbs-BSIM3vjsm)))+(this->_ckt_gmin*vbs));
                }
              }
              else {
                {
                  T0 = ((((BSIM3ijth+SourceSatCurrent)/Nvtm))*UNITFAD).val();
                  BSIM3cbs = ((BSIM3ijth+(T0*(vbs-BSIM3vjsm)))+(this->_ckt_gmin*vbs));
                }
              }
            }
          }
          else {
            {
              if((vbs < BSIM3vjsmRev)) {
                {
                  evbs = (vbs/Nvtm);
                  if((evbs < -34.0)) {
                    {
                      evbs = 1.713908431e-15;
                      devbsdvbs = ((0.0)*UNITFAD).val();
                    }
                  }
                  else {
                    {
                      evbs = exp(evbs);
                      devbsdvbs = (evbs/Nvtm);
                    }
                  }
                  T1 = (evbs-1.0);
                  T2 = (BSIM3IVjsmRev+(BSIM3SslpRev*(vbs-BSIM3vjsmRev)));
                  BSIM3cbs = (T1*T2);
                }
              }
                          else if((vbs <= BSIM3vjsmFwd)) {
                {
                  T0 = (vbs/Nvtm);
                  if((T0 < -34.0)) {
                    {
                      evbs = 1.713908431e-15;
                      devbsdvbs = 0.0;
                    }
                  }
                  else {
                    {
                      evbs = exp(T0);
                      devbsdvbs = (evbs/Nvtm);
                    }
                  }
                  T1 = ((BSIM3bvs+vbs)/Nvtm);
                  if((T1 > 34.0)) {
                    {
                      T2 = 1.713908431e-15;
                      T3 = ((0.0)*UNITFAD).val();
                    }
                  }
                  else {
                    {
                      T2 = exp(-T1);
                      T3 = (-T2/Nvtm);
                    }
                  }
                  BSIM3cbs = (SourceSatCurrent*(((evbs+BSIM3XExpBVS)-1.0)-(BSIM3xjbvs*T2)));
                }
              }
              else {
                BSIM3cbs = (BSIM3IVjsmFwd+(BSIM3SslpFwd*(vbs-BSIM3vjsmFwd)));
              }
            }
          }
        }
      }
    }
  }
  if(((BSIM3drainArea <= 0.0) && (BSIM3drainPerimeter <= 0.0))) {
    DrainSatCurrent = ((1.0e-14)*UNITFAD).val();
  }
  else {
    DrainSatCurrent = ((((BSIM3drainArea*BSIM3jctTempSatCurDensity)+(BSIM3drainPerimeter*BSIM3jctSidewallTempSatCurDensity)))*UNITFAD).val();
  }
  if((DrainSatCurrent <= 0.0)) {
    {
      BSIM3cbd = (this->_ckt_gmin*vbd);
    }
  }
  else {
    {
      if((BSIM3ijth == 0.0)) {
        {
          evbd = AnalogFunctions::exp_lim<CogendaFadType>((vbd/Nvtm));
          BSIM3cbd = ((DrainSatCurrent*(evbd-1.0))+(this->_ckt_gmin*vbd));
        }
      }
      else {
        {
          if((BSIM3breakmod == 0)) {
            {
              if((vbd < BSIM3vjdm)) {
                {
                  evbd = AnalogFunctions::exp_lim<CogendaFadType>((vbd/Nvtm));
                  BSIM3cbd = ((DrainSatCurrent*(evbd-1.0))+(this->_ckt_gmin*vbd));
                }
              }
                          else if((this->version >= 3.21)) {
                {
                  T0 = (BSIM3IsEvjdm/Nvtm);
                  BSIM3cbd = (((BSIM3IsEvjdm-DrainSatCurrent)+(T0*(vbd-BSIM3vjdm)))+(this->_ckt_gmin*vbd));
                }
              }
              else {
                {
                  T0 = ((BSIM3ijth+DrainSatCurrent)/Nvtm);
                  BSIM3cbd = ((BSIM3ijth+(T0*(vbd-BSIM3vjdm)))+(this->_ckt_gmin*vbd));
                }
              }
            }
          }
          else {
            {
              if((vbd < BSIM3vjdmRev)) {
                {
                  evbd = (vbd/Nvtm);
                  if((evbd < -34.0)) {
                    {
                      evbd = 1.713908431e-15;
                      devbddvbd = ((0.0)*UNITFAD).val();
                    }
                  }
                  else {
                    {
                      evbd = exp(evbd);
                      devbddvbd = (evbd/Nvtm);
                    }
                  }
                  T1 = (evbd-1.0);
                  T2 = (BSIM3IVjdmRev+(BSIM3DslpRev*(vbd-BSIM3vjdmRev)));
                  BSIM3cbd = (T1*T2);
                }
              }
                          else if((vbd <= BSIM3vjdmFwd)) {
                {
                  T0 = (vbd/Nvtm);
                  if((T0 < -34.0)) {
                    {
                      evbd = 1.713908431e-15;
                      devbddvbd = 0.0;
                    }
                  }
                  else {
                    {
                      evbd = exp(T0);
                      devbddvbd = (evbd/Nvtm);
                    }
                  }
                  T1 = ((BSIM3bvd+vbd)/Nvtm);
                  if((T1 > 34.0)) {
                    {
                      T2 = 1.713908431e-15;
                      T3 = 0.0;
                    }
                  }
                  else {
                    {
                      T2 = exp(-T1);
                      T3 = (-T2/Nvtm);
                    }
                  }
                  BSIM3cbd = (DrainSatCurrent*(((evbd+BSIM3XExpBVD)-1.0
				)-(BSIM3xjbvd*T2)));
                }
              }
              else {
                {
                  BSIM3cbd = (BSIM3IVjdmFwd+(BSIM3DslpFwd*(vbd-BSIM3vjdmFwd)));
                }
              }
            }
          }
        }
      }
    }
  }
  T0 = ((Vbs-BSIM3vbsc)-0.001);
  T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)-(0.004*BSIM3vbsc)) > 0.0),sqrt(((T0*T0)-(0.004*BSIM3vbsc))),0.0);
  Vbseff = (BSIM3vbsc+(0.5*(T0+T1)));
  if((Vbseff < Vbs)) {
    Vbseff = Vbs;
  }
  if((Vbseff > 0.0)) {
    {
      T0 = (BSIM3phi/(BSIM3phi+Vbseff));
      Phis = (BSIM3phi*T0);
      sqrtPhis = (BSIM3phis3/(BSIM3phi+(0.5*Vbseff)));
    }
  }
  else {
    {
      Phis = (BSIM3phi-Vbseff);
      sqrtPhis = cogenda_ternary_op<CogendaFadType>((Phis > 0.0),sqrt(Phis),0.0);
    }
  }
  Xdep = ((BSIM3Xdep0*sqrtPhis)/BSIM3sqrtPhi);
  Leff = ((BSIM3leff)*UNITFAD).val();
  Vtm = ((BSIM3vtm)*UNITFAD).val();
  T3 = cogenda_ternary_op<CogendaFadType>((Xdep > 0.0),sqrt(Xdep),0.0);
  V0 = (((BSIM3vbi-BSIM3phi))*UNITFAD).val();
  T0 = (BSIM3dvt2*Vbseff);
  if((T0 >= -0.5)) {
    {
      T1 = (1.0+T0);
    }
  }
  else {
    {
      T4 = (1.0/(3.0+(8.0*T0)));
      T1 = ((1.0+(3.0*T0))*T4);
    }
  }
  lt1 = ((BSIM3factor1*T3)*T1);
  T0 = (BSIM3dvt2w*Vbseff);
  if((T0 >= -0.5)) {
    {
      T1 = (1.0+T0);
    }
  }
  else {
    {
      T4 = (1.0/(3.0+(8.0*T0)));
      T1 = ((1.0+(3.0*T0))*T4);
    }
  }
  ltw = ((BSIM3factor1*T3)*T1);
  T0 = (((-0.5*BSIM3dvt1)*Leff)/lt1);
  if((T0 > -34.0)) {
    {
      T1 = exp(T0);
      Theta0 = (T1*(1.0+(2.0*T1)));
    }
  }
  else {
    {
      T1 = 1.713908431e-15;
      Theta0 = (T1*(1.0+(2.0*T1)));
    }
  }
  BSIM3thetavth = (BSIM3dvt0*Theta0);
  Delt_vth = (BSIM3thetavth*V0);
  T0 = ((((-0.5*BSIM3dvt1w)*BSIM3weff)*Leff)/ltw);
  if((T0 > -34.0)) {
    {
      T1 = exp(T0);
      T2 = (T1*(1.0+(2.0*T1)));
    }
  }
  else {
    {
      T1 = 1.713908431e-15;
      T2 = (T1*(1.0+(2.0*T1)));
    }
  }
  T0 = (BSIM3dvt0w*T2);
  T2 = (T0*V0);
  TempRatio = ((((T/BSIM3tnom)-1.0))*UNITFAD).val();
  T0 = cogenda_ternary_op<CogendaFadType>(((1.0+(BSIM3nlx/Leff)) > 0.0),sqrt((1.0+(BSIM3nlx/Leff))),0.0);
  T1 = (((BSIM3k1ox*(T0-1.0))*BSIM3sqrtPhi)+(((BSIM3kt1+(BSIM3kt1l/Leff))+(BSIM3kt2*Vbseff))*TempRatio));
  tmp2 = ((((BSIM3tox*BSIM3phi)/(BSIM3weff+BSIM3w0)))*UNITFAD).val();
  T3 = (BSIM3eta0+(BSIM3etab*Vbseff));
  if((T3 < 1.0e-4)) {
    {
      T9 = (1.0/(3.0-(2.0e4*T3)));
      T3 = ((2.0e-4-T3)*T9);
    }
  }
  dDIBL_Sft_dVd = (T3*BSIM3theta0vb0);
  DIBL_Sft = (dDIBL_Sft_dVd*Vds);
  Vth = (((((((((BSIM3type*BSIM3vth0)-(BSIM3k1*BSIM3sqrtPhi))+(BSIM3k1ox*sqrtPhis))-(BSIM3k2ox*Vbseff))-Delt_vth)-T2)+((BSIM3k3+(BSIM3k3b*Vbseff))*tmp2))+T1)-DIBL_Sft);
  tmp2 = ((BSIM3nfactor*1.03594e-10)/Xdep);
  tmp3 = ((BSIM3cdsc+(BSIM3cdscb*Vbseff))+(BSIM3cdscd*Vds));
  tmp4 = (((tmp2+(tmp3*Theta0))+BSIM3cit)/BSIM3cox);
  if((tmp4 >= -0.5)) {
    n = (1.0+tmp4);
  }
  else {
    {
      T0 = (1.0/(3.0+(8.0*tmp4)));
      n = ((1.0+(3.0*tmp4))*T0);
    }
  }
  if((BSIM3vgslim > 0.0)) {
    {
      tmp = ((0.0001)*UNITFAD).val();
      T8 = (Vgs+tmp);
      X1 = (BSIM3vgslim+T8);
      T1 = (BSIM3vgslim-T8);
      T2 = (T1*T1);
      X2 = cogenda_ternary_op<CogendaFadType>(((T2+((4.0*tmp)*BSIM3vgslim)) > 0.0),sqrt((T2+((4.0*tmp)*BSIM3vgslim))),0.0);
      T3 = (1.0/(X1+X2));
      Vgs_x2 = (2.0*Vgs);
      vgsmax_t3 = (BSIM3vgslim*T3);
      Vgs_lim = (Vgs_x2*vgsmax_t3);
      vgslim_t3 = (Vgs_lim*T3);
      T6 = (T1/X2);
      T7 = (2.0*vgsmax_t3);
      Vgs = Vgs_lim;
    }
  }
  T0 = (BSIM3vfb+BSIM3phi);
  if((((BSIM3ngate > 1.0e18) && (BSIM3ngate < 1.0e25)) && (Vgs > T0))) {
    {
      T1 = ((((1.0e6*1.602176462e-19)*1.03594e-10)*BSIM3ngate)/(BSIM3cox*BSIM3cox));
      T4 = cogenda_ternary_op<CogendaFadType>(((1.0+((2.0*(Vgs-T0))/T1)) > 0.0),sqrt((1.0+((2.0*(Vgs-T0))/T1))),0.0);
      T2 = (T1*(T4-1.0));
      T3 = (((0.5*T2)*T2)/T1);
      T7 = ((1.12-T3)-0.05);
      T6 = cogenda_ternary_op<CogendaFadType>((((T7*T7)+0.224) > 0.0),sqrt(((T7*T7)+0.224)),0.0);
      T5 = (1.12-(0.5*(T7+T6)));
      Vgs_eff = (Vgs-T5);
    }
  }
  else {
    Vgs_eff = Vgs;
  }
  Vgst = (Vgs_eff-Vth);
  if((((BSIM3ngate > 1.0e18) && (BSIM3ngate < 1.0e25)) && (vgd > T0))) {
    {
      T1 = ((((1.0e6*1.602176462e-19)*1.03594e-10)*BSIM3ngate)/(BSIM3cox*BSIM3cox));
      T4 = cogenda_ternary_op<CogendaFadType>(((1.0+((2.0*(vgd-T0))/T1)) > 0.0),sqrt((1.0+((2.0*(vgd-T0))/T1))),0.0);
      T2 = (T1*(T4-1.0));
      T3 = (((0.5*T2)*T2)/T1);
      T7 = (1.07-T3);
      T6 = cogenda_ternary_op<CogendaFadType>((((T7*T7)+0.224) > 0.0),sqrt(((T7*T7)+0.224)),0.0);
      T5 = (1.12-(0.5*(T7+T6)));
      vgd_eff = (vgd-T5);
    }
  }
  else {
    vgd_eff = vgd;
  }
  T10 = ((2.0*n)*Vtm);
  VgstNVt = (Vgst/T10);
  ExpArg = (((2.0*BSIM3voff)-Vgst)/T10);
  if((VgstNVt > 34.0)) {
    Vgsteff = Vgst;
  }
  else if((ExpArg > 34.0)) {
    {
      T0 = ((Vgst-BSIM3voff)/(n*Vtm));
      ExpVgst = exp(T0);
      Vgsteff = (((Vtm*BSIM3cdep0)/BSIM3cox)*ExpVgst);
    }
  }
  else {
    {
      ExpVgst = exp(VgstNVt);
      T1 = (T10*log((1.0+ExpVgst)));
      dT2_dVg = ((-BSIM3cox/(Vtm*BSIM3cdep0))*exp(ExpArg));
      T2 = (1.0-(T10*dT2_dVg));
      Vgsteff = (T1/T2);
    }
  }
  T9 = (sqrtPhis-BSIM3sqrtPhi);
  Weff = (BSIM3weff-(2.0*((BSIM3dwg*Vgsteff)+(BSIM3dwb*T9))));
  if((Weff < 2.0e-8)) {
    {
      T0 = (1.0/(6.0e-8-(2.0*Weff)));
      Weff = ((2.0e-8*(4.0e-8-Weff))*T0);
    }
  }
  T0 = ((BSIM3prwg*Vgsteff)+(BSIM3prwb*T9));
  if((T0 >= -0.9)) {
    Rds = (BSIM3rds0*(1.0+T0));
  }
  else {
    {
      T1 = (1.0/(17.0+(20.0*T0)));
      Rds = ((BSIM3rds0*(0.8+T0))*T1);
    }
  }
  T1 = ((0.5*BSIM3k1ox)/sqrtPhis);
  T9 = cogenda_ternary_op<CogendaFadType>(((BSIM3xj*Xdep) > 0.0),sqrt((BSIM3xj*Xdep)),0.0);
  tmp1 = (Leff+(2.0*T9));
  T5 = (Leff/tmp1);
  tmp2 = (BSIM3a0*T5);
  tmp3 = (BSIM3weff+BSIM3b1);
  tmp4 = (BSIM3b0/tmp3);
  T2 = (tmp2+tmp4);
  T6 = (T5*T5);
  T7 = (T5*T6);
  Abulk0 = (1.0+(T1*T2));
  T8 = ((BSIM3ags*BSIM3a0)*T7);
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
    }
  }
  T2 = (BSIM3keta*Vbseff);
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
  doNoise = ((analysis_noise)*UNITFAD).val();
  if(doNoise) {
    {
      dAbulk_dVb = ((0.0)*UNITFAD).val();
      dEsatL_dVb = ((0.0)*UNITFAD).val();
      dEsatL_dVd = ((0.0)*UNITFAD).val();
      dEsatL_dVg = ((0.0)*UNITFAD).val();
      dRds_dVb = ((0.0)*UNITFAD).val();
      dRds_dVg = ((0.0)*UNITFAD).val();
      dVa_dVb = ((0.0)*UNITFAD).val();
      dVa_dVd = ((0.0)*UNITFAD).val();
      dVa_dVg = ((0.0)*UNITFAD).val();
      dVbseff_dVb = ((0.0)*UNITFAD).val();
      dVdseff_dVb = ((0.0)*UNITFAD).val();
      dVdseff_dVd = ((0.0)*UNITFAD).val();
      dVdseff_dVg = ((0.0)*UNITFAD).val();
      dVgsteff_dVb = ((0.0)*UNITFAD).val();
      dVgsteff_dVd = ((0.0)*UNITFAD).val();
      dVgsteff_dVg = ((0.0)*UNITFAD).val();
      dWeff_dVb = ((0.0)*UNITFAD).val();
      dWeff_dVg = ((0.0)*UNITFAD).val();
      dueff_dVb = ((0.0)*UNITFAD).val();
      dueff_dVd = ((0.0)*UNITFAD).val();
      dueff_dVg = ((0.0)*UNITFAD).val();
    }
  }
  if((BSIM3mobMod == 1)) {
    {
      T0 = ((Vgsteff+Vth)+Vth);
      T2 = (BSIM3ua+(BSIM3uc*Vbseff));
      T3 = (T0/BSIM3tox);
      T5 = (T3*(T2+(BSIM3ub*T3)));
    }
  }
  else if((BSIM3mobMod == 2)) {
    T5 = ((Vgsteff/BSIM3tox)*((BSIM3ua+(BSIM3uc*Vbseff))+((BSIM3ub*Vgsteff)/BSIM3tox)));
  }
  else {
    {
      T0 = ((Vgsteff+Vth)+Vth);
      T2 = (1.0+(BSIM3uc*Vbseff));
      T3 = (T0/BSIM3tox);
      T4 = (T3*(BSIM3ua+(BSIM3ub*T3)));
      T5 = (T4*T2);
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
  ueff = (BSIM3u0temp/Denomi);
  WVCox = ((Weff*BSIM3vsattemp)*BSIM3cox);
  WVCoxRds = (WVCox*Rds);
  Esat = ((2.0*BSIM3vsattemp)/ueff);
  EsatL = (Esat*Leff);
  if((BSIM3a1 == 0.0)) {
    Lambda = ((BSIM3a2)*UNITFAD).val();
  }
  else if((BSIM3a1 > 0.0)) {
    {
      T0 = (1.0-BSIM3a2);
      T1 = ((T0-(BSIM3a1*Vgsteff))-0.0001);
      T2 = cogenda_ternary_op<CogendaFadType>((((T1*T1)+(0.0004*T0)) > 0.0),sqrt(((T1*T1)+(0.0004*T0))),0.0);
      Lambda = ((BSIM3a2+T0)-(0.5*(T1+T2)));
    }
  }
  else {
    {
      T1 = ((BSIM3a2+(BSIM3a1*Vgsteff))-0.0001);
      T2 = cogenda_ternary_op<CogendaFadType>((((T1*T1)+(0.0004*BSIM3a2)) > 0.0),sqrt(((T1*T1)+(0.0004*BSIM3a2))),0.0);
      Lambda = (0.5*(T1+T2));
    }
  }
  Vgst2Vtm = (Vgsteff+(2.0*Vtm));
  if(((Rds == 0.0) && (Lambda == 1.0))) {
    {
      T0 = (1.0/((Abulk*EsatL)+Vgst2Vtm));
      T3 = (EsatL*Vgst2Vtm);
      Vdsat = (T3*T0);
    }
  }
  else {
    {
      T9 = (Abulk*WVCoxRds);
      T7 = (Vgst2Vtm*T9);
      T6 = (Vgst2Vtm*WVCoxRds);
      T0 = ((2.0*Abulk)*((T9-1.0)+(1.0/Lambda)));
      T1 = (((Vgst2Vtm*((2.0/Lambda)-1.0))+(Abulk*EsatL))+(3.0*T7));
      T2 = (Vgst2Vtm*(EsatL+(2.0*T6)));
      T3 = cogenda_ternary_op<CogendaFadType>((((T1*T1)-((2.0*T0)*T2)) > 0.0),sqrt(((T1*T1)-((2.0*T0)*T2))),0.0);
      Vdsat = ((T1-T3)/T0);
    }
  }
  if((this->version == 53)) {
    {
      T1 = ((Vdsat-Vds)-BSIM3delta);
      T2 = cogenda_ternary_op<CogendaFadType>((((T1*T1)+((4.0*BSIM3delta)*Vdsat)) > 0.0),sqrt(((T1*T1)+((4.0*BSIM3delta)*Vdsat))),0.0);
      Vdseff = (Vdsat-(0.5*(T1+T2)));
    }
  }
  else {
    {
      X1 = ((Vdsat+Vds)+BSIM3delta);
      T1 = ((Vdsat-Vds)-BSIM3delta);
      T2 = (T1*T1);
      X2 = cogenda_ternary_op<CogendaFadType>(((T2+((4.0*BSIM3delta)*Vdsat)) > 0.0),sqrt((T2+((4.0*BSIM3delta)*Vdsat))),0.0);
      T3 = (1.0/(X1+X2));
      T4 = (2.0*Vds);
      T5 = (T4*T3);
      Vdseff = (T5*Vdsat);
    }
  }
  if((Vds == 0.0)) {
    Vdseff = 0.0;
  }
  tmp4 = (1.0-(((0.5*Abulk)*Vdsat)/Vgst2Vtm));
  T9 = (WVCoxRds*Vgsteff);
  T0 = ((EsatL+Vdsat)+((2.0*T9)*tmp4));
  T9 = (WVCoxRds*Abulk);
  T1 = (((2.0/Lambda)-1.0)+T9);
  Vasat = (T0/T1);
  if((Vdseff > Vds)) {
    Vdseff = Vds;
  }
  diffVds = (Vds-Vdseff);
  if(((BSIM3pclm > 0.0) && (diffVds > 1.0e-10))) {
    {
      T0 = (1.0/((BSIM3pclm*Abulk)*BSIM3litl));
      T2 = (Vgsteff/EsatL);
      T1 = (Leff*(Abulk+T2));
      T9 = (T0*T1);
      VACLM = (T9*diffVds);
    }
  }
  else {
    VACLM = 5.834617425e14;
  }
  if((BSIM3thetaRout > 0.0)) {
    {
      T8 = (Abulk*Vdsat);
      T0 = (Vgst2Vtm*T8);
      T1 = (Vgst2Vtm+T8);
      T2 = BSIM3thetaRout;
      VADIBL = ((Vgst2Vtm-(T0/T1))/T2);
      T7 = (BSIM3pdiblb*Vbseff);
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
    }
  }
  else {
    VADIBL = 5.834617425e14;
  }
  T8 = (BSIM3pvag/EsatL);
  T9 = (T8*Vgsteff);
  if((T9 > -0.9)) {
    T0 = (1.0+T9);
  }
  else {
    {
      T1 = (1.0/(17.0+(20.0*T9)));
      T0 = ((0.8+T9)*T1);
    }
  }
  tmp3 = (VACLM+VADIBL);
  T1 = ((VACLM*VADIBL)/tmp3);
  Va = (Vasat+(T0*T1));
  if(((BSIM3pscbe2 > 0.0) && (BSIM3pscbe1 >= 0.0))) {
    {
      if((diffVds > ((BSIM3pscbe1*BSIM3litl)/34.0))) {
        {
          T0 = ((BSIM3pscbe1*BSIM3litl)/diffVds);
          VASCBE = ((Leff*exp(T0))/BSIM3pscbe2);
          T1 = ((T0*VASCBE)/diffVds);
          if(doNoise) {
            {
              dVASCBE_dVg = (T1*dVdseff_dVg);
              dVASCBE_dVd = (-T1*(1.0-dVdseff_dVd));
              dVASCBE_dVb = (T1*dVdseff_dVb);
            }
          }
        }
      }
      else {
        {
          VASCBE = ((5.834617425e14*Leff)/BSIM3pscbe2);
          if(doNoise) {
            {
              dVASCBE_dVg = 0.0;
              dVASCBE_dVd = 0.0;
              dVASCBE_dVb = 0.0;
            }
          }
        }
      }
    }
  }
  else {
    {
      VASCBE = 5.834617425e14;
      if(doNoise) {
        {
          dVASCBE_dVg = 0.0;
          dVASCBE_dVd = 0.0;
          dVASCBE_dVb = 0.0;
        }
      }
    }
  }
  Gm = ((0.0)*UNITFAD).val();
  Gds = ((0.0)*UNITFAD).val();
  Gmb = ((0.0)*UNITFAD).val();
  CoxWovL = ((BSIM3cox*Weff)/Leff);
  beta = (ueff*CoxWovL);
  T0 = (1.0-(((0.5*Abulk)*Vdseff)/Vgst2Vtm));
  fgche1 = (Vgsteff*T0);
  T9 = (Vdseff/EsatL);
  fgche2 = (1.0+T9);
  gche = ((beta*fgche1)/fgche2);
  T0 = (1.0+(gche*Rds));
  T9 = (Vdseff/T0);
  Idl = (gche*T9);
  T9 = (diffVds/Va);
  T0 = (1.0+T9);
  Idsa = (Idl*T0);
  T9 = (diffVds/VASCBE);
  T0 = (1.0+T9);
  Ids = (Idsa*T0);
  if(doNoise) {
    {
      if(((BSIM3noiMod == 1) || (BSIM3noiMod == 3))) {
        {
          dbeta_dVg = ((CoxWovL*dueff_dVg)+((beta*dWeff_dVg)/Weff));
          dbeta_dVd = (CoxWovL*dueff_dVd);
          dbeta_dVb = ((CoxWovL*dueff_dVb)+((beta*dWeff_dVb)/Weff));
          T0 = (1.0-(((0.5*Abulk)*Vdseff)/Vgst2Vtm));
          dT0_dVg = ((-0.5*(((Abulk*dVdseff_dVg)-((Abulk*Vdseff)/Vgst2Vtm))+(Vdseff*dAbulk_dVg)))/Vgst2Vtm);
          dT0_dVd = (((-0.5*Abulk)*dVdseff_dVd)/Vgst2Vtm);
          dT0_dVb = ((-0.5*((Abulk*dVdseff_dVb)+(dAbulk_dVb*Vdseff)))/Vgst2Vtm);
          dfgche1_dVg = ((Vgsteff*dT0_dVg)+T0);
          dfgche1_dVd = (Vgsteff*dT0_dVd);
          dfgche1_dVb = (Vgsteff*dT0_dVb);
          T9 = (Vdseff/EsatL);
          dfgche2_dVg = ((dVdseff_dVg-(T9*dEsatL_dVg))/EsatL);
          dfgche2_dVd = ((dVdseff_dVd-(T9*dEsatL_dVd))/EsatL);
          dfgche2_dVb = ((dVdseff_dVb-(T9*dEsatL_dVb))/EsatL);
          dgche_dVg = ((((beta*dfgche1_dVg)+(fgche1*dbeta_dVg))-(gche*dfgche2_dVg))/fgche2);
          dgche_dVd = ((((beta*dfgche1_dVd)+(fgche1*dbeta_dVd))-(gche*dfgche2_dVd))/fgche2);
          dgche_dVb = ((((beta*dfgche1_dVb)+(fgche1*dbeta_dVb))-(gche*dfgche2_dVb))/fgche2);
          T0 = (1.0+(gche*Rds));
          T9 = (Vdseff/T0);
          dIdl_dVg = ((((gche*dVdseff_dVg)+(T9*dgche_dVg))/T0)-(((Idl*gche)/T0)*dRds_dVg));
          dIdl_dVd = (((gche*dVdseff_dVd)+(T9*dgche_dVd))/T0);
          dIdl_dVb = ((((gche*dVdseff_dVb)+(T9*dgche_dVb))-((Idl*dRds_dVb)*gche))/T0);
          T9 = (diffVds/Va);
          T0 = (1.0+T9);
          dIdsa_dVg = ((T0*dIdl_dVg)-((Idl*(dVdseff_dVg+(T9*dVa_dVg)))/Va));
          dIdsa_dVd = ((T0*dIdl_dVd)+((Idl*((1.0-dVdseff_dVd)-(T9*dVa_dVd)))/Va));
          dIdsa_dVb = ((T0*dIdl_dVb)-((Idl*(dVdseff_dVb+(T9*dVa_dVb)))/Va));
          T9 = (diffVds/VASCBE);
          T0 = (1.0+T9);
          Gm = ((T0*dIdsa_dVg)-((Idsa*(dVdseff_dVg+(T9*dVASCBE_dVg)))/VASCBE));
          Gds = ((T0*dIdsa_dVd)+((Idsa*((1.0-dVdseff_dVd)-(T9*dVASCBE_dVd)))/VASCBE));
          Gmb = ((T0*dIdsa_dVb)-((Idsa*(dVdseff_dVb+(T9*dVASCBE_dVb)))/VASCBE));
          Gds = (Gds+(Gm*dVgsteff_dVd));
          Gmb = (Gmb+(Gm*dVgsteff_dVb));
          Gm = (Gm*dVgsteff_dVg);
          Gmb = (Gmb*dVbseff_dVb);
        }
      }
    }
  }
  tmp = (((BSIM3alpha0+(BSIM3alpha1*Leff)))*UNITFAD).val();
  if(((tmp <= 0.0) || (BSIM3beta0 <= 0.0))) {
    {
      Isub = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      T2 = (tmp/Leff);
      if((diffVds > (BSIM3beta0/34.0))) {
        {
          T0 = (-BSIM3beta0/diffVds);
          T1 = ((T2*diffVds)*exp(T0));
        }
      }
      else {
        {
          T3 = (T2*1.713908431e-15);
          T1 = (T3*diffVds);
        }
      }
      Isub = (T1*Idsa);
    }
  }
  if((this->gidlmod == 0)) {
    {
      T0 = (3.0*BSIM3toxe);
      T1 = (((vds-vgs_eff)-BSIM3egidl)/T0);
      if((((((BSIM3agidl <= 0.0) || (BSIM3bgidl <= 0.0)) || (T1 <= 0.0)) || (BSIM3cgidl <= 0.0)) || (vbd > 0.0))) {
        Igidl = ((0.0)*UNITFAD).val();
      }
      else {
        {
          T2 = (BSIM3bgidl/T1);
          if((T2 < 100.0)) {
            {
              Igidl = (((BSIM3agidl*BSIM3weffCV)*T1)*exp(-T2));
              T3 = ((Igidl*(1.0+T2))/T1);
            }
          }
          else {
            {
              Igidl = ((BSIM3agidl*BSIM3weffCV)*3.720075976e-44);
              Igidl = (Igidl*T1);
            }
          }
          T4 = (vbd*vbd);
          T5 = (-vbd*T4);
          T6 = (BSIM3cgidl+T5);
          T7 = (T5/T6);
          Igidl = (Igidl*T7);
        }
      }
      T1 = (((-vds-vgd_eff)-BSIM3egisl)/T0);
      if((((((BSIM3agisl <= 0.0) || (BSIM3bgisl <= 0.0)) || (T1 <= 0.0)) || (BSIM3cgisl <= 0.0)) || (vbs > 0.0))) {
        Igisl = ((0.0)*UNITFAD).val();
      }
      else {
        {
          T2 = (BSIM3bgisl/T1);
          if((T2 < 100.0)) {
            {
              Igisl = (((BSIM3agisl*BSIM3weffCV)*T1)*exp(-T2));
            }
          }
          else {
            {
              Igisl = ((BSIM3agisl*BSIM3weffCV)*3.720075976e-44);
              Igisl = (Igisl*T1);
            }
          }
          T4 = (vbs*vbs);
          T5 = (-vbs*T4);
          T6 = (BSIM3cgisl+T5);
          T7 = (T5/T6);
          Igisl = (Igisl*T7);
        }
      }
    }
  }
  else {
    {
      T1 = (((-vds-(BSIM3rgisl*vgd_eff))-BSIM3egisl)/T0);
      if(((((BSIM3agisl <= 0.0) || (BSIM3bgisl <= 0.0)) || (T1 <= 0.0)) || (BSIM3cgisl < 0.0))) {
        Igisl = 0.0;
      }
      else {
        {
          T2 = (BSIM3bgisl/T1);
          if((T2 < 100.0)) {
            Igisl = (((BSIM3weffCV*BSIM3agisl)*T1)*exp(-T2));
          }
          else {
            {
              T3 = ((BSIM3weffCV*BSIM3agisl)*3.720075976e-44);
              Igisl = (T3*T1);
            }
          }
          T4 = (vbs-BSIM3fgisl);
          if((T4 == 0)) {
            T5 = (BSIM3kgisl/T4);
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
      T1 = (((vds-(BSIM3rgidl*vgs_eff))-BSIM3egidl)/T0);
      if(((((BSIM3agidl <= 0.0) || (BSIM3bgidl <= 0.0)) || (T1 <= 0.0)) || (BSIM3cgidl < 0.0))) {
        Igidl = 0.0;
      }
      else {
        {
          T2 = (BSIM3bgidl/T1);
          if((T2 < 100.0)) {
            {
              Igidl = (((BSIM3weffCV*BSIM3agidl)*T1)*exp(-T2));
            }
          }
          else {
            {
              T3 = ((BSIM3weffCV*BSIM3agidl)*3.720075976e-44);
              Igidl = (T3*T1);
            }
          }
          T4 = (vbd-BSIM3fgidl);
          if((T4 == 0)) {
            T5 = 100.0;
          }
          else {
            T5 = (BSIM3kgidl/T4);
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
      printf("Igisl=",Igisl.val());
      printf("Igidl=",Igidl.val());
    }
  }
  if(((BSIM3igcMod != 0) || (BSIM3igbMod != 0))) {
    {
      Vfb = ((BSIM3vfbzb)*UNITFAD).val();
      V3 = (((Vfb-Vgs_eff)+Vbseff)-0.02);
      if((Vfb <= 0.0)) {
        T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)-((4.0*0.02)*Vfb)) > 0.0),sqrt(((V3*V3)-((4.0*0.02)*Vfb))),0.0);
      }
      else {
        T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)+((4.0*0.02)*Vfb)) > 0.0),sqrt(((V3*V3)+((4.0*0.02)*Vfb))),0.0);
      }
      T1 = (0.5*(1.0+(V3/T0)));
      Vfbeff = (Vfb-(0.5*(V3+T0)));
      Voxacc = (Vfb-Vfbeff);
      if((Voxacc < 0.0)) {
        Voxacc = 0.0;
      }
      T0 = (0.5*BSIM3k1ox);
      T3 = (((Vgs_eff-Vfbeff)-Vbseff)-Vgsteff);
      if((BSIM3k1ox == 0.0)) {
        Voxdepinv = ((0.0)*UNITFAD).val();
      }
          else if((T3 < 0.0)) {
        Voxdepinv = -T3;
      }
      else {
        {
          T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+T3) > 0.0),sqrt(((T0*T0)+T3)),0.0);
          Voxdepinv = (BSIM3k1ox*(T1-T0));
        }
      }
      Voxdepinv = (Voxdepinv+Vgsteff);
    }
  }
  if((BSIM3tempMod < 2)) {
    tmp = ((Vtm)*UNITFAD).val();
  }
  else {
    tmp = ((Vtm0)*UNITFAD).val();
  }
  if(BSIM3igcMod) {
    {
      T0 = (tmp*BSIM3nigc);
      if((BSIM3igcMod == 1)) {
        {
          VxNVt = ((Vgs_eff-(BSIM3type*BSIM3vth0))/T0);
          if((VxNVt > 34.0)) {
            Vaux = (Vgs_eff-(BSIM3type*BSIM3vth0));
          }
        }
      }
          else if((BSIM3igcMod == 2)) {
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
      T11 = ((BSIM3Aechvb)*UNITFAD).val();
      T3 = ((BSIM3aigc*BSIM3cigc)-BSIM3bigc);
      T4 = (BSIM3bigc*BSIM3cigc);
      T5 = (BSIM3Bechvb*((BSIM3aigc+(T3*Voxdepinv))-((T4*Voxdepinv)*Voxdepinv)));
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
      if(BSIM3pigcdGiven) {
        Pigcd = ((BSIM3pigcd)*UNITFAD).val();
      }
      else {
        {
          T11 = ((-BSIM3Bechvb)*UNITFAD).val();
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
      T0 = (vgs-(BSIM3vfbsd+BSIM3vfbsdoff));
      vgs_eff = cogenda_ternary_op<CogendaFadType>((((T0*T0)+1.0e-4) > 0.0),sqrt(((T0*T0)+1.0e-4)),0.0);
      T2 = (vgs*vgs_eff);
      T11 = ((BSIM3AechvbEdge)*UNITFAD).val();
      T12 = BSIM3BechvbEdge;
      T3 = ((BSIM3aigs*BSIM3cigs)-BSIM3bigs);
      T4 = (BSIM3bigs*BSIM3cigs);
      T5 = (T12*((BSIM3aigsd+(T3*vgs_eff))-((T4*vgs_eff)*vgs_eff)));
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
        printf("Igs=",Igs.val(),((T11)*UNITFAD).val(),T2.val(),T6.val(),((BSIM3aigsd)*UNITFAD).val(),T5.val(),T3.val(),T4.val(),T12.val(),vgs_eff.val());
      }
      T0 = (vgd-(BSIM3vfbsd+BSIM3vfbsdoff));
      vgd_eff = cogenda_ternary_op<CogendaFadType>((((T0*T0)+1.0e-4) > 0.0),sqrt(((T0*T0)+1.0e-4)),0.0);
      T2 = (vgd*vgd_eff);
      T11 = ((BSIM3AechvbEdge)*UNITFAD).val();
      T3 = ((BSIM3aigd*BSIM3cigd)-BSIM3bigd);
      T4 = (BSIM3bigd*BSIM3cigd);
      T5 = (T12*((BSIM3aigd+(T3*vgd_eff))-((T4*vgd_eff)*vgd_eff)));
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
        printf("Igd=",Igd.val(),((T11)*UNITFAD).val(),T2.val(),T6.val());
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
  if(BSIM3igbMod) {
    {
      T0 = (tmp*BSIM3nigbacc);
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
      T11 = (((((4.97232e-7*BSIM3weff)*BSIM3leff)*BSIM3ToxRatio))*UNITFAD).val();
      T12 = (-7.45669e11*BSIM3toxe);
      T3 = ((BSIM3aigbacc*BSIM3cigbacc)-BSIM3bigbacc);
      T4 = (BSIM3bigbacc*BSIM3cigbacc);
      T5 = (T12*((BSIM3aigbacc+(T3*Voxacc))-((T4*Voxacc)*Voxacc)));
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
        printf("Igbacc=",Igbacc.val());
      }
      T0 = (tmp*BSIM3nigbinv);
      T1 = (Voxdepinv-BSIM3eigbinv);
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
      T11 = (((T11*0.75610))*UNITFAD).val();
      T12 = (T12*1.31724);
      T3 = ((BSIM3aigbinv*BSIM3cigbinv)-BSIM3bigbinv);
      T4 = (BSIM3bigbinv*BSIM3cigbinv);
      T5 = (T12*((BSIM3aigbinv+(T3*Voxdepinv))-((T4*Voxdepinv)*Voxdepinv)));
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
        printf("Igbinv=",Igbinv.val());
      }
      Igb = (Igbinv+Igbacc);
    }
  }
  else {
    Igb = 0.0;
  }
  if((BSIM3xpart < 0)) {
    {
      qgate = ((0.0)*UNITFAD).val();
      qdrn = ((0.0)*UNITFAD).val();
      qbulk = ((0.0)*UNITFAD).val();
      qsrc = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      if((BSIM3capMod == 0)) {
        {
          if((Vbseff < 0.0)) {
            Vbseff = Vbs;
          }
          else {
            Vbseff = (BSIM3phi-Phis);
          }
          Vfb = ((BSIM3vfbcv)*UNITFAD).val();
          Vth = ((Vfb+BSIM3phi)+(BSIM3k1ox*sqrtPhis));
          Vgst = (Vgs_eff-Vth);
          CoxWL = ((((BSIM3cox*BSIM3weffCV)*BSIM3leffCV))*UNITFAD).val();
          Arg1 = ((Vgs_eff-Vbseff)-Vfb);
          if((Arg1 <= 0.0)) {
            {
              qgate = (CoxWL*Arg1);
              qbulk = -qgate;
              qdrn = ((0.0)*UNITFAD).val();
            }
          }
                  else if((Vgst <= 0.0)) {
            {
              T1 = (0.5*BSIM3k1ox);
              T2 = cogenda_ternary_op<CogendaFadType>((((T1*T1)+Arg1) > 0.0),sqrt(((T1*T1)+Arg1)),0.0);
              qgate = ((CoxWL*BSIM3k1ox)*(T2-T1));
              qbulk = -qgate;
              qdrn = ((0.0)*UNITFAD).val();
            }
          }
          else {
            {
              One_Third_CoxWL = (((CoxWL/3.0))*UNITFAD).val();
              Two_Third_CoxWL = (((2.0*One_Third_CoxWL))*UNITFAD).val();
              AbulkCV = (Abulk0*BSIM3abulkCVfactor);
              Vdsat = (Vgst/AbulkCV);
              if((BSIM3xpart > 0.5)) {
                {
                  if((Vdsat <= Vds)) {
                    {
                      T1 = (Vdsat/3.0);
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-T1));
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
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-(0.5*(Vds-T3))));
                      T10 = (T4*T8);
                      qdrn = (T4*T7);
                      qbulk = -((qgate+qdrn)+T10);
                    }
                  }
                }
              }
                          else if((BSIM3xpart < 0.5)) {
                {
                  if((Vds >= Vdsat)) {
                    {
                      T1 = (Vdsat/3.0);
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-T1));
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
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-(0.5*(Vds-T3))));
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
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-T1));
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
                      qgate = (CoxWL*(((Vgs_eff-Vfb)-BSIM3phi)-(0.5*(Vds-T3))));
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
            VbseffCV = (BSIM3phi-Phis);
          }
          CoxWL = ((((BSIM3cox*BSIM3weffCV)*BSIM3leffCV))*UNITFAD).val();
          local_noff = (n*BSIM3noff);
          T0 = (Vtm*local_noff);
          local_voffcv = ((BSIM3voffcv)*UNITFAD).val();
          VgstNVt = ((Vgst-local_voffcv)/T0);
          if((VgstNVt > 34.0)) {
            Vgsteff = (Vgst-local_voffcv);
          }
                  else if((VgstNVt < -34.0)) {
            Vgsteff = (T0*log((1.0+1.713908431e-15)));
          }
          else {
            {
              ExpVgst = exp(VgstNVt);
              Vgsteff = (T0*log((1.0+ExpVgst)));
            }
          }
          if((BSIM3capMod == 1)) {
            {
              Vfb = ((BSIM3vfbzb)*UNITFAD).val();
              Arg1 = (((Vgs_eff-VbseffCV)-Vfb)-Vgsteff);
              if((Arg1 <= 0.0)) {
                qgate = (CoxWL*Arg1);
              }
              else {
                {
                  T0 = (0.5*BSIM3k1ox);
                  T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+Arg1) > 0.0),sqrt(((T0*T0)+Arg1)),0.0);
                  qgate = ((CoxWL*BSIM3k1ox)*(T1-T0));
                }
              }
              qbulk = -qgate;
              One_Third_CoxWL = (((CoxWL/3.0))*UNITFAD).val();
              Two_Third_CoxWL = (((2.0*One_Third_CoxWL))*UNITFAD).val();
              AbulkCV = (Abulk0*BSIM3abulkCVfactor);
              VdsatCV = (Vgsteff/AbulkCV);
              if((VdsatCV < Vds)) {
                {
                  T0 = (Vgsteff-(VdsatCV/3.0));
                  qgate = (qgate+(CoxWL*T0));
                  T0 = (VdsatCV-Vgsteff);
                  qbulk = (qbulk+(One_Third_CoxWL*T0));
                  if((BSIM3xpart > 0.5)) {
                    T0 = -Two_Third_CoxWL;
                  }
                                  else if((BSIM3xpart < 0.5)) {
                    T0 = (-0.4*CoxWL);
                  }
                  else {
                    T0 = -One_Third_CoxWL;
                  }
                  qsrc = (T0*Vgsteff);
                }
              }
              else {
                {
                  T0 = (AbulkCV*Vds);
                  T1 = (12.0*((Vgsteff-(0.5*T0))+1.0e-20));
                  T2 = (Vds/T1);
                  T3 = (T0*T2);
                  qgate = (qgate+(CoxWL*((Vgsteff-(0.5*Vds))+T3)));
                  qbulk = (qbulk+((CoxWL*(1.0-AbulkCV))*((0.5*Vds)-T3)));
                  if((BSIM3xpart > 0.5)) {
                    {
                      T1 = (T1+T1);
                      qsrc = (-CoxWL*(((0.5*Vgsteff)+(0.25*T0))-((T0*T0)/T1)));
                    }
                  }
                                  else if((BSIM3xpart < 0.5)) {
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
                }
              }
              qdrn = -((qgate+qbulk)+qsrc);
            }
          }
                  else if((BSIM3capMod == 2)) {
            {
              Vfb = ((BSIM3vfbzb)*UNITFAD).val();
              V3 = (((Vfb-Vgs_eff)+VbseffCV)-0.02);
              if((Vfb <= 0.0)) {
                T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)-((4.0*0.02)*Vfb)) > 0.0),sqrt(((V3*V3)-((4.0*0.02)*Vfb))),0.0);
              }
              else {
                T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)+((4.0*0.02)*Vfb)) > 0.0),sqrt(((V3*V3)+((4.0*0.02)*Vfb))),0.0);
              }
              Vfbeff = (Vfb-(0.5*(V3+T0)));
              Qac0 = (CoxWL*(Vfbeff-Vfb));
              T0 = (0.5*BSIM3k1ox);
              T3 = (((Vgs_eff-Vfbeff)-VbseffCV)-Vgsteff);
              if((BSIM3k1ox == 0.0)) {
                T1 = 0.0;
              }
                          else if((T3 < 0.0)) {
                T1 = (T0+(T3/BSIM3k1ox));
              }
              else {
                T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+T3) > 0.0),sqrt(((T0*T0)+T3)),0.0);
              }
              Qsub0 = ((CoxWL*BSIM3k1ox)*(T1-T0));
              AbulkCV = (Abulk0*BSIM3abulkCVfactor);
              VdsatCV = (Vgsteff/AbulkCV);
              V4 = ((VdsatCV-Vds)-0.02);
              T0 = cogenda_ternary_op<CogendaFadType>((((V4*V4)+((4.0*0.02)*VdsatCV)) > 0.0),sqrt(((V4*V4)+((4.0*0.02)*VdsatCV))),0.0);
              VdseffCV = (VdsatCV-(0.5*(V4+T0)));
              if((Vds == 0.0)) {
                VdseffCV = 0.0;
              }
              T0 = (AbulkCV*VdseffCV);
              T1 = (12.0*((Vgsteff-(0.5*T0))+1e-20));
              T2 = (VdseffCV/T1);
              T3 = (T0*T2);
              qgate = (CoxWL*((Vgsteff-(0.5*VdseffCV))+T3));
              T7 = (1.0-AbulkCV);
              qbulk = ((CoxWL*T7)*((0.5*VdseffCV)-T3));
              if((BSIM3xpart > 0.5)) {
                {
                  T1 = (T1+T1);
                  qsrc = (-CoxWL*(((0.5*Vgsteff)+(0.25*T0))-((T0*T0)/T1)));
                }
              }
                          else if((BSIM3xpart < 0.5)) {
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
              qgate = ((qgate+Qac0)+Qsub0);
              qbulk = (qbulk-(Qac0+Qsub0));
              qdrn = -((qgate+qbulk)+qsrc);
            }
          }
                  else if((BSIM3capMod == 3)) {
            {
              V3 = (((BSIM3vfbzb-Vgs_eff)+VbseffCV)-0.02);
              if((BSIM3vfbzb <= 0.0)) {
                T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)-((4.0*0.02)*BSIM3vfbzb)) > 0.0),sqrt(((V3*V3)-((4.0*0.02)*BSIM3vfbzb))),0.0);
              }
              else {
                T0 = cogenda_ternary_op<CogendaFadType>((((V3*V3)+((4.0*0.02)*BSIM3vfbzb)) > 0.0),sqrt(((V3*V3)+((4.0*0.02)*BSIM3vfbzb))),0.0);
              }
              Vfbeff = (BSIM3vfbzb-(0.5*(V3+T0)));
              Cox = ((BSIM3cox)*UNITFAD).val();
              Tox = (((1.0e8*BSIM3tox))*UNITFAD).val();
              T0 = (((Vgs_eff-VbseffCV)-BSIM3vfbzb)/Tox);
              tmp = (T0*BSIM3acde);
              if(((-34.0 < tmp) && (tmp < 34.0))) {
                Tcen = (BSIM3ldeb*exp(tmp));
              }
                          else if((tmp <= -34.0)) {
                Tcen = (BSIM3ldeb*1.713908431e-15);
              }
              else {
                Tcen = (BSIM3ldeb*5.834617425e14);
              }
              LINK = (((1.0e-3*BSIM3tox))*UNITFAD).val();
              V3 = ((BSIM3ldeb-Tcen)-LINK);
              V4 = cogenda_ternary_op<CogendaFadType>((((V3*V3)+((4.0*LINK)*BSIM3ldeb)) > 0.0),sqrt(((V3*V3)+((4.0*LINK)*BSIM3ldeb))),0.0);
              Tcen = (BSIM3ldeb-(0.5*(V3+V4)));
              Ccen = (1.03594e-10/Tcen);
              T2 = (Cox/(Cox+Ccen));
              Coxeff = (T2*Ccen);
              CoxWLcen = ((CoxWL*Coxeff)/Cox);
              Qac0 = (CoxWLcen*(Vfbeff-BSIM3vfbzb));
              T0 = (0.5*BSIM3k1ox);
              T3 = (((Vgs_eff-Vfbeff)-VbseffCV)-Vgsteff);
              if((BSIM3k1ox == 0.0)) {
                T1 = 0.0;
              }
                          else if((T3 < 0.0)) {
                T1 = (T0+(T3/BSIM3k1ox));
              }
              else {
                T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+T3) > 0.0),sqrt(((T0*T0)+T3)),0.0);
              }
              Qsub0 = ((CoxWLcen*BSIM3k1ox)*(T1-T0));
              if((BSIM3k1ox <= 0.0)) {
                {
                  Denomi = ((0.25*BSIM3moin)*Vtm);
                  T0 = (0.5*BSIM3sqrtPhi);
                }
              }
              else {
                {
                  Denomi = (((BSIM3moin*Vtm)*BSIM3k1ox)*BSIM3k1ox);
                  T0 = (BSIM3k1ox*BSIM3sqrtPhi);
                }
              }
              T1 = ((2.0*T0)+Vgsteff);
              DeltaPhi = (Vtm*log((1.0+((T1*Vgsteff)/Denomi))));
              T0 = ((Vgsteff-DeltaPhi)-0.001);
              T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+(Vgsteff*0.004)) > 0.0),sqrt(((T0*T0)+(Vgsteff*0.004))),0.0);
              VgDP = (0.5*(T0+T1));
              T3 = (4.0*((Vth-BSIM3vfbzb)-BSIM3phi));
              Tox = (((Tox+Tox))*UNITFAD).val();
              if((T3 >= 0.0)) {
                T0 = ((Vgsteff+T3)/Tox);
              }
              else {
                T0 = ((Vgsteff+1.0e-20)/Tox);
              }
              tmp = exp((0.7*log(T0)));
              T1 = (1.0+tmp);
              Tcen = (1.9e-9/T1);
              Ccen = (1.03594e-10/Tcen);
              T0 = (Cox/(Cox+Ccen));
              Coxeff = (T0*Ccen);
              CoxWLcen = ((CoxWL*Coxeff)/Cox);
              AbulkCV = (Abulk0*BSIM3abulkCVfactor);
              VdsatCV = (VgDP/AbulkCV);
              T0 = ((VdsatCV-Vds)-0.02);
              T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+((4.0*0.02)*VdsatCV)) > 0.0),sqrt(((T0*T0)+((4.0*0.02)*VdsatCV))),0.0);
              if((T0 >= 0.0)) {
                VdseffCV = (VdsatCV-(0.5*(T0+T1)));
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
              if((BSIM3xpart > 0.5)) {
                {
                  qsrc = (-CoxWLcen*(((T1/2.0)+(T0/4.0))-(((0.5 
*T0)*T0)/T2)));
                }
              }
                          else if((BSIM3xpart < 0.5)) {
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
              qgate = (((qgate+Qac0)+Qsub0)-qbulk);
              qbulk = (qbulk-(Qac0+Qsub0));
              qdrn = -((qgate+qbulk)+qsrc);
            }
          }
        }
      }
    }
  }
  if((BSIM3acm < 9)) {
    {
      printf("Warn: ACM(%d)<9 may not full compatible with hspice...\n",BSIM3acm);
    }
  }
  czbd = (((BSIM3unitAreaTempJctCap*BSIM3drainArea))*UNITFAD).val();
  czbs = (((BSIM3unitAreaTempJctCap*BSIM3sourceArea))*UNITFAD).val();
  if((BSIM3drainPerimeter < BSIM3weff)) {
    {
      czbdswg = (((BSIM3unitLengthGateSidewallTempJctCap*BSIM3drainPerimeter))*UNITFAD).val();
      czbdsw = ((0.0)*UNITFAD).val();
    }
  }
  else {
    {
      czbdsw = (((BSIM3unitLengthSidewallTempJctCap*(BSIM3drainPerimeter-BSIM3weff)))*UNITFAD).val();
      czbdswg = (((BSIM3unitLengthGateSidewallTempJctCap*BSIM3weff))*UNITFAD).val();
    }
  }
  if((BSIM3sourcePerimeter < BSIM3weff)) {
    {
      czbssw = ((0.0)*UNITFAD).val();
      czbsswg = (((BSIM3unitLengthGateSidewallTempJctCap*BSIM3sourcePerimeter))*UNITFAD).val();
    }
  }
  else {
    {
      czbssw = (((BSIM3unitLengthSidewallTempJctCap*(BSIM3sourcePerimeter-BSIM3weff)))*UNITFAD).val();
      czbsswg = (((BSIM3unitLengthGateSidewallTempJctCap*BSIM3weff))*UNITFAD).val();
    }
  }
  MJ = ((BSIM3bulkJctBotGradingCoeff)*UNITFAD).val();
  MJSW = ((BSIM3bulkJctSideGradingCoeff)*UNITFAD).val();
  MJSWG = ((BSIM3bulkJctGateSideGradingCoeff)*UNITFAD).val();
  if((vbs < 0.0)) {
    {
      if((czbs > 0.0)) {
        {
          arg = (1.0-(vbs/BSIM3PhiB));
          if((MJ == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJ*log(arg)));
          }
          qbs = (((BSIM3PhiB*czbs)*(1.0-(arg*sarg)))/(1.0-MJ));
        }
      }
      else {
        {
          qbs = 0.0;
        }
      }
      if((czbssw > 0.0)) {
        {
          arg = (1.0-(vbs/BSIM3PhiBSW));
          if((MJSW == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJSW*log(arg)));
          }
          qbs = (qbs+(((BSIM3PhiBSW*czbssw)*(1.0-(arg*sarg)))/(1.0-MJSW)));
        }
      }
      if((czbsswg > 0.0)) {
        {
          arg = (1.0-(vbs/BSIM3PhiBSWG));
          if((MJSWG == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJSWG*log(arg)));
          }
          qbs = (qbs+(((BSIM3PhiBSWG*czbsswg)*(1.0-(arg*sarg)))/(1.0-MJSWG)));
        }
      }
    }
  }
  else {
    {
      T0 = ((czbs+czbssw)+czbsswg);
      T1 = (vbs*((((czbs*MJ)/BSIM3PhiB)+((czbssw*MJSW)/BSIM3PhiBSW))+((czbsswg*MJSWG)/BSIM3PhiBSWG)));
      qbs = (vbs*(T0+(0.5*T1)));
    }
  }
  if((vbd < 0.0)) {
    {
      if((czbd > 0.0)) {
        {
          arg = (1.0-(vbd/BSIM3PhiB));
          if((MJ == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJ*log(arg)));
          }
          qbd = (((BSIM3PhiB*czbd)*(1.0-(arg*sarg)))/(1.0-MJ));
        }
      }
      else {
        {
          qbd = 0.0;
        }
      }
      if((czbdsw > 0.0)) {
        {
          arg = (1.0-(vbd/BSIM3PhiBSW));
          if((MJSW == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJSW*log(arg)));
          }
          qbd = (qbd+(((BSIM3PhiBSW*czbdsw)*(1.0-(arg*sarg)))/(1.0-MJSW)));
        }
      }
      if((czbdswg > 0.0)) {
        {
          arg = (1.0-(vbd/BSIM3PhiBSWG));
          if((MJSWG == 0.5)) {
            sarg = (1.0/cogenda_ternary_op<CogendaFadType>((arg > 0.0),sqrt(arg),0.0));
          }
          else {
            sarg = exp((-MJSWG*log((1.0-(vbd/BSIM3PhiBSWG)))));
          }
          qbd = (qbd+(((BSIM3PhiBSWG*czbdswg)*(1.0-(arg*sarg)))/(1.0-MJSWG)));
        }
      }
    }
  }
  else {
    {
      T0 = ((czbd+czbdsw)+czbdswg);
      T1 = (vbd*((((czbd*MJ)/BSIM3PhiB)+((czbdsw*MJSW)/BSIM3PhiBSW))+((czbdswg*MJSWG)/BSIM3PhiBSWG)));
      qbd = (vbd*(T0+(0.5*T1)));
    }
  }
  if((BSIM3capMod == 0)) {
    {
      local_cgdo = ((BSIM3cgdo)*UNITFAD).val();
      qgdo = (local_cgdo*vgd);
      local_cgso = ((BSIM3cgso)*UNITFAD).val();
      qgso = (local_cgso*vgs);
    }
  }
  else if((BSIM3capMod == 1)) {
    {
      if((vgd < 0.0)) {
        {
          T1 = cogenda_ternary_op<CogendaFadType>(((1.0-((4.0*vgd)/BSIM3ckappa)) > 0.0),sqrt((1.0-((4.0*vgd)/BSIM3ckappa))),0.0);
          local_cgdo = (BSIM3cgdo+((BSIM3weffCV*BSIM3cgdl)/T1));
          qgdo = ((local_cgdo*vgd)-((((BSIM3weffCV*0.5)*BSIM3cgdl)*BSIM3ckappa)*(T1-1.0)));
        }
      }
      else {
        {
          local_cgdo = (BSIM3cgdo+(BSIM3weffCV*BSIM3cgdl));
          qgdo = (((BSIM3weffCV*BSIM3cgdl)+local_cgdo)*vgd);
        }
      }
      if((vgs < 0.0)) {
        {
          T1 = cogenda_ternary_op<CogendaFadType>(((1.0-((4.0*vgs)/BSIM3ckappa)) > 0.0),sqrt((1.0-((4.0*vgs)/BSIM3ckappa))),0.0);
          local_cgso = (BSIM3cgso+((BSIM3weffCV*BSIM3cgsl)/T1));
          qgso = ((local_cgso*vgs)-((((BSIM3weffCV*0.5)*BSIM3cgsl)*BSIM3ckappa)*(T1-1.0)));
        }
      }
      else {
        {
          local_cgso = (BSIM3cgso+(BSIM3weffCV*BSIM3cgsl));
          qgso = (((BSIM3weffCV*BSIM3cgsl)+local_cgso)*vgs);
        }
      }
    }
  }
  else {
    {
      T0 = (vgd+0.02);
      T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+(4.0*0.02)) > 0.0),sqrt(((T0*T0)+(4.0*0.02))),0.0);
      T2 = (0.5*(T0-T1));
      T3 = (BSIM3weffCV*BSIM3cgdl);
      T4 = cogenda_ternary_op<CogendaFadType>(((1.0-((4.0*T2)/BSIM3ckappa)) > 0.0),sqrt((1.0-((4.0*T2)/BSIM3ckappa))),0.0);
      local_cgdo = ((BSIM3cgdo+T3)-((T3*(1.0-(1.0/T4)))*(0.5-((0.5*T0)/T1))));
      qgdo = (((local_cgdo+T3)*vgd)-(T3*(T2+((0.5 
*BSIM3ckappa)*(T4-1.0)))));
      T0 = (vgs+0.02);
      T1 = cogenda_ternary_op<CogendaFadType>((((T0*T0)+(4.0*0.02)) > 0.0),sqrt(((T0*T0)+(4.0*0.02))),0.0);
      T2 = (0.5*(T0-T1));
      T3 = (BSIM3weffCV*BSIM3cgsl);
      T4 = cogenda_ternary_op<CogendaFadType>(((1.0-((4.0*T2)/BSIM3ckappa)) > 0.0),sqrt((1.0-((4.0*T2)/BSIM3ckappa))),0.0);
      local_cgso = ((BSIM3cgso+T3)-((T3*(1.0-(1.0/T4)))*(0.5-((0.5*T0)/T1))));
      qgso = (((local_cgso+T3)*vgs)-(T3*(T2+((0.5 
*BSIM3ckappa)*(T4-1.0)))));
    }
  }
  if((BSIM3mode > 0)) {
    {
      if((BSIM3nqsMod == 0)) {
        {
          qgd = qgdo;
          qgs = qgso;
          qgb = (BSIM3cgbo*vgb);
          qgate = (((qgate+qgd)+qgs)+qgb);
          qbulk = (qbulk-qgb);
          qdrn = (qdrn-qgd);
        }
      }
      else {
        {
          qgd = qgdo;
          qgs = qgso;
          qgb = (BSIM3cgbo*vgb);
          qgate = ((qgd+qgs)+qgb);
          qbulk = -qgb;
          qdrn = -qgd;
        }
      }
    }
  }
  else {
    {
      if((BSIM3nqsMod == 0)) {
        {
          qgd = qgdo;
          qgs = qgso;
          qgb = (BSIM3cgbo*vgb);
          qgate = (((qgate+qgd)+qgs)+qgb);
          qbulk = (qbulk-qgb);
          qsrc = (qdrn-qgs);
          qdrn = -((qgate+qbulk)+qsrc);
        }
      }
      else {
        {
          qgd = qgdo;
          qgs = qgso;
          qgb = (BSIM3cgbo*vgb);
          qgate = ((qgd+qgs)+qgb);
          qbulk = -qgb;
          qsrc = -qgs;
          qdrn = -((qgate+qbulk)+qsrc);
        }
      }
    }
  }
  if((this->verbose == 1)) {
    {
      if(((BSIM3sourceResistance > 0.0) || (BSIM3drainResistance > 0.0))) {
        printf("BSIM3sourceResistance,BSIM3drainResistance,rsh=",((BSIM3sourceResistance)*UNITFAD).val(),((BSIM3drainResistance)*UNITFAD).val(),this->rsh);
      }
    }
  }
  if((BSIM3sourceResistance > 0.0)) {
    {
//I-contrib...
      staticContributions[cogendaNodeID_s] += (probeVars[cogendaProbeID_V_s_si]/BSIM3sourceResistance);
      staticContributions[cogendaNodeID_si] -= (probeVars[cogendaProbeID_V_s_si]/BSIM3sourceResistance);
    }
  }
  else {
//V-contrib...
//ZERO_For_Collapsed_Node
  }
  if((BSIM3drainResistance > 0.0)) {
    {
//I-contrib...
      staticContributions[cogendaNodeID_d] += (probeVars[cogendaProbeID_V_d_di]/BSIM3drainResistance);
      staticContributions[cogendaNodeID_di] -= (probeVars[cogendaProbeID_V_d_di]/BSIM3drainResistance);
      if((this->verbose == 1)) {
        {
          printf("BSIM3drainResistance=",((BSIM3drainResistance)*UNITFAD).val());
        }
      }
    }
  }
  else {
//V-contrib...
//ZERO_For_Collapsed_Node
  }
  if((BSIM3mode > 0)) {
    {
      Ibs = ((0.0)*UNITFAD).val();
      Ibd = (-BSIM3type*Isub);
    }
  }
  else {
    {
      Ids = -Ids;
      Ibd = 0.0;
      Ibs = (-BSIM3type*Isub);
    }
  }
//I-contrib...
  staticContributions[cogendaNodeID_di] += (BSIM3type*Ids);
  staticContributions[cogendaNodeID_si] -= (BSIM3type*Ids);
//I-contrib...
  staticContributions[cogendaNodeID_g] += (BSIM3type*Igs);
  staticContributions[cogendaNodeID_si] -= (BSIM3type*Igs);
//I-contrib...
  staticContributions[cogendaNodeID_g] += (BSIM3type*Igd);
  staticContributions[cogendaNodeID_di] -= (BSIM3type*Igd);
//I-contrib...
  staticContributions[cogendaNodeID_di] += (BSIM3type*Igidl);
  staticContributions[cogendaNodeID_b] -= (BSIM3type*Igidl);
//I-contrib...
  staticContributions[cogendaNodeID_si] += (BSIM3type*Igisl);
  staticContributions[cogendaNodeID_b] -= (BSIM3type*Igisl);
//I-contrib...
  staticContributions[cogendaNodeID_g] += (BSIM3type*Igcs);
  staticContributions[cogendaNodeID_si] -= (BSIM3type*Igcs);
//I-contrib...
  staticContributions[cogendaNodeID_g] += (BSIM3type*Igcd);
  staticContributions[cogendaNodeID_di] -= (BSIM3type*Igcd);
//I-contrib...
  staticContributions[cogendaNodeID_g] += (BSIM3type*Igb);
  staticContributions[cogendaNodeID_b] -= (BSIM3type*Igb);
  if((this->verbose == 1)) {
    {
      printf("Ids=",Ids.val());
      printf("Igs,Igd,Igb,Igcs,Igcd=",Igs.val(),Igd.val(),Igb.val(),Igcs.val(),Igcd.val());
      printf("Isub=",Ibs.val(),Ibd.val());
      printf("Ibs,Ibd=",BSIM3cbs.val(),BSIM3cbd.val());
    }
  }
//I-contrib...
  staticContributions[cogendaNodeID_b] += ((BSIM3type*BSIM3cbs)+Ibs);
  staticContributions[cogendaNodeID_si] -= ((BSIM3type*BSIM3cbs)+Ibs);
//I-contrib...
  staticContributions[cogendaNodeID_b] += ((BSIM3type*BSIM3cbd)+Ibd);
  staticContributions[cogendaNodeID_di] -= ((BSIM3type*BSIM3cbd)+Ibd);
//Q-contrib...
  dynamicContributions[cogendaNodeID_g] += (BSIM3type*(qgate));
  dynamicContributions[cogendaNodeID_si] -= (BSIM3type*(qgate));
//Q-contrib...
  dynamicContributions[cogendaNodeID_b] += (BSIM3type*((qbs+qbulk)));
  dynamicContributions[cogendaNodeID_si] -= (BSIM3type*((qbs+qbulk)));
//Q-contrib...
  dynamicContributions[cogendaNodeID_b] += (BSIM3type*(qbd));
  dynamicContributions[cogendaNodeID_di] -= (BSIM3type*(qbd));
//Q-contrib...
  dynamicContributions[cogendaNodeID_di] += (BSIM3type*(qdrn));
  dynamicContributions[cogendaNodeID_si] -= (BSIM3type*(qdrn));
  if(doNoise) {
    {
      switch(BSIM3noiMod){
      case 1:
      case 3:
        thermalNoiseContrib = ((((8*1.3806503e-23)*T)*((Gm+Gds)+Gmb))/3);        break;
      case 2:
      case 4:
        {
          Qinv = ((((-Weff*Leff)*Cox)*Vgsteff)*(1-((Abulk/(2*(Vgsteff+(2*Vtm))))*Vdseff)));
          thermalNoiseContrib = (((((4*1.3806503e-23)*T)*ueff)*fabs(Qinv))/((Leff*Leff)+((ueff*fabs(Qinv))*Rds)));
        }
        break;
      default:
        thermalNoiseContrib = 0;        break;
      }
      switch(BSIM3noiMod){
      case 1:
      case 4:
        flickerNoiseContrib = ((BSIM3kf*pow(fabs(Ids),BSIM3af))/((Cox*Leff)*Leff));        break;
      case 2:
      case 3:
        {
          Esat = ((2.0*BSIM3vsattemp)/ueff);
          if((BSIM3em <= 0.0)) {
            DelClm = ((0.0)*UNITFAD).val();
          }
          else {
            {
              T0 = ((((Vds-Vdseff)/BSIM3litl)+BSIM3em)/Esat);
              DelClm = (BSIM3litl*log(max(T0,1.0e-38)));
            }
          }
          T1 = (((((1.602176462e-19*1.602176462e-19)*8.62e-5)*Ids)*T)*ueff);
          T2 = ((((1.0e8*Abulk)*Cox)*Leff)*Leff);
          N0 = ((Cox*Vgsteff)/1.602176462e-19);
          Vgst2Vtm = (Vgsteff+(2.0*Vtm));
          AbovVgst2Vtm = (Abulk/Vgst2Vtm);
          Nl = (((Cox*Vgsteff)*(1.0-(AbovVgst2Vtm*Vdseff)))/1.602176462e-19);
          T3 = (this->noia*log(max(((N0+2.0e14)/(Nl+2.0e14)),1.0e-38)));
          T4 = (this->noib*(N0-Nl));
          T5 = ((this->noic*0.5)*((N0*N0)-(Nl*Nl)));
          T6 = (((8.62e-5*T)*Ids)*Ids);
          T7 = (((1.0e8*Leff)*Leff)*Weff);
          T8 = ((this->noia+(this->noib*Nl))+((this->noic*Nl)*Nl));
          T9 = ((Nl+2.0e14)*(Nl+2.0e14));
          flickerNoiseContrib = (((T1/T2)*((T3+T4)+T5))+((((T6/T7)*DelClm)*T8)/T9));
        }
        break;
      default:
        flickerNoiseContrib = 0.0;        break;
      }
//I-contrib...
//ZERO_For_Small_Signal_Node;
//I-contrib...
//ZERO_For_Small_Signal_Node;
      if(((this->rsh > 0.0) && (BSIM3sourceSquares > 0.0))) {
//I-contrib...
//ZERO_For_Small_Signal_Node;
      }
      if(((this->rsh > 0.0) && (BSIM3drainSquares > 0.0))) {
//I-contrib...
//ZERO_For_Small_Signal_Node;
      }
    }
  }
}
  if (loadLeadCurrent) {
    for ( int unCtNode=0; unCtNode < 4 ; unCtNode++) {
      leadCurrentF[unCtNode] = 0.0;
      leadCurrentQ[unCtNode] = 0.0;
    }
    for ( int unCtNode=0; unCtNode < 6; unCtNode++) {
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
  (*f_b_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si);
  (*f_b_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si);
  (*f_b_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  (*f_b_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  (*f_d_Equ_d_Node_Ptr) +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  (*f_d_Equ_di_Node_Ptr) +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  (*f_di_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si);
  (*f_di_Equ_d_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  (*f_di_Equ_di_Node_Ptr) +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di)  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  (*f_di_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  (*f_di_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  (*f_g_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  (*f_g_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si);
  (*f_g_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  (*f_g_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  (*f_s_Equ_s_Node_Ptr) +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  (*f_s_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  (*f_si_Equ_b_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
  (*f_si_Equ_di_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  (*f_si_Equ_g_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
  (*f_si_Equ_s_Node_Ptr) +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
  (*f_si_Equ_si_Node_Ptr) +=  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
#else
  //use the offsets instead of pointers
  dFdx[li_b][m_b_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si);
  dFdx[li_b][m_b_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si);
  dFdx[li_b][m_b_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  dFdx[li_b][m_b_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  dFdx[li_d][m_d_Equ_d_NodeOffset] +=  +staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  dFdx[li_d][m_d_Equ_di_NodeOffset] +=  -staticContributions[cogendaNodeID_d].dx(cogendaProbeID_V_d_di);
  dFdx[li_di][m_di_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si);
  dFdx[li_di][m_di_Equ_d_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di);
  dFdx[li_di][m_di_Equ_di_NodeOffset] +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_d_di)  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  dFdx[li_di][m_di_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  dFdx[li_di][m_di_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  dFdx[li_g][m_g_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  dFdx[li_g][m_g_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si);
  dFdx[li_g][m_g_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  dFdx[li_g][m_g_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  dFdx[li_s][m_s_Equ_s_NodeOffset] +=  +staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  dFdx[li_s][m_s_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_s].dx(cogendaProbeID_V_s_si);
  dFdx[li_si][m_si_Equ_b_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
  dFdx[li_si][m_si_Equ_di_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  dFdx[li_si][m_si_Equ_g_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
  dFdx[li_si][m_si_Equ_s_NodeOffset] +=  +staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si);
  dFdx[li_si][m_si_Equ_si_NodeOffset] +=  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_s_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -staticContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
#endif
  return bsuccess;
}

  //bool Instance::loadDAEdQdx
bool Instance::loadDAEdQdx()
{
  bool bsuccess = true;
  Linear::Matrix & dQdx = *(extData.dQdxMatrixPtr);
#ifndef Xyce_NONPOINTER_MATRIX_LOAD
  (*q_b_Equ_b_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si);
  (*q_b_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si);
  (*q_b_Equ_g_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  (*q_b_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si)  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  (*q_di_Equ_b_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si);
  (*q_di_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  (*q_di_Equ_g_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  (*q_di_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  (*q_g_Equ_b_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  (*q_g_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si);
  (*q_g_Equ_g_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  (*q_g_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si)  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  (*q_si_Equ_b_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
  (*q_si_Equ_di_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  (*q_si_Equ_g_Node_Ptr) +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
  (*q_si_Equ_si_Node_Ptr) +=  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
#else
  //use the offsets instead of pointers
  dQdx[li_b][m_b_Equ_b_NodeOffset] +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si);
  dQdx[li_b][m_b_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si);
  dQdx[li_b][m_b_Equ_g_NodeOffset] +=  +dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  dQdx[li_b][m_b_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_b_si)  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_b].dx(cogendaProbeID_V_g_si);
  dQdx[li_di][m_di_Equ_b_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si);
  dQdx[li_di][m_di_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si);
  dQdx[li_di][m_di_Equ_g_NodeOffset] +=  +dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  dQdx[li_di][m_di_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_b_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_di].dx(cogendaProbeID_V_g_si);
  dQdx[li_g][m_g_Equ_b_NodeOffset] +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  dQdx[li_g][m_g_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si);
  dQdx[li_g][m_g_Equ_g_NodeOffset] +=  +dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si);
  dQdx[li_g][m_g_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_g_si)  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_g].dx(cogendaProbeID_V_b_si);
  dQdx[li_si][m_si_Equ_b_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
  dQdx[li_si][m_si_Equ_di_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si);
  dQdx[li_si][m_si_Equ_g_NodeOffset] +=  +dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si);
  dQdx[li_si][m_si_Equ_si_NodeOffset] +=  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_g_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_di_si)  -dynamicContributions[cogendaNodeID_si].dx(cogendaProbeID_V_b_si);
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
  if(!given("A0"))
    a0 = 1;
  if(!given("A1"))
    a1 = 0;
  if(!given("A2"))
    a2 = 1;
  if(!given("ACDE"))
    acde = 1;
  if(!given("ACM"))
    acm = -9999999;
  if(!given("AD"))
    ad = -9.9999e-99;
  if(!given("AF"))
    af = 1;
  if(!given("AGIDL"))
    agidl = -9.9999e-99;
  if(!given("AGISL"))
    agisl = -9.9999e-99;
  if(!given("AGS"))
    ags = 0;
  if(!given("AIGBACC"))
    aigbacc = -9.9999e-99;
  if(!given("AIGBINV"))
    aigbinv = -9.9999e-99;
  if(!given("AIGC"))
    aigc = -9.9999e-99;
  if(!given("AIGD"))
    aigd = -9.9999e-99;
  if(!given("AIGS"))
    aigs = -9.9999e-99;
  if(!given("AIGSD"))
    aigsd = -9.9999e-99;
  if(!given("ALPHA0"))
    alpha0 = 0;
  if(!given("ALPHA1"))
    alpha1 = 0;
  if(!given("AS"))
    as = -9.9999e-99;
  if(!given("AT"))
    at = 3.3e4;
  if(!given("B0"))
    b0 = 0;
  if(!given("B1"))
    b1 = 0;
  if(!given("BETA0"))
    beta0 = 30;
  if(!given("BGIDL"))
    bgidl = -9.9999e-99;
  if(!given("BGISL"))
    bgisl = -9.9999e-99;
  if(!given("BIGBACC"))
    bigbacc = -9.9999e-99;
  if(!given("BIGBINV"))
    bigbinv = -9.9999e-99;
  if(!given("BIGC"))
    bigc = -9.9999e-99;
  if(!given("BIGD"))
    bigd = -9.9999e-99;
  if(!given("BIGS"))
    bigs = -9.9999e-99;
  if(!given("BIGSD"))
    bigsd = -9.9999e-99;
  if(!given("BINFLAG"))
    binflag = 0;
  if(!given("BINUNIT"))
    binunit = 1;
  if(!given("BREAKMOD"))
    breakmod = 0;
  if(!given("BVD"))
    bvd = 10.0;
  if(!given("BVJ"))
    bvj = -9.9999e-99;
  if(!given("BVS"))
    bvs = 10.0;
  if(!given("CALCACM"))
    calcacm = -9999999;
  if(!given("CAPMOD"))
    capmod = 3;
  else if ( !(capmod >= 0 && capmod <= 3 ))
    UserError0(*this) << "Model Parameter value " << capmod << " out of range [0:3]";
  if(!given("CBD"))
    cbd = 0;
  if(!given("CBS"))
    cbs = 0;
  if(!given("CDSC"))
    cdsc = 2.4e-4;
  if(!given("CDSCB"))
    cdscb = 0;
  if(!given("CDSCD"))
    cdscd = 0;
  if(!given("CF"))
    cf = -9.9999e-99;
  if(!given("CGBO"))
    cgbo = -9.9999e-99;
  if(!given("CGDL"))
    cgdl = 0;
  if(!given("CGDO"))
    cgdo = -9.9999e-99;
  if(!given("CGIDL"))
    cgidl = -9.9999e-99;
  if(!given("CGISL"))
    cgisl = -9.9999e-99;
  if(!given("CGSL"))
    cgsl = 0;
  if(!given("CGSO"))
    cgso = -9.9999e-99;
  if(!given("CIGBACC"))
    cigbacc = -9.9999e-99;
  if(!given("CIGBINV"))
    cigbinv = -9.9999e-99;
  if(!given("CIGC"))
    cigc = -9.9999e-99;
  if(!given("CIGD"))
    cigd = -9.9999e-99;
  if(!given("CIGS"))
    cigs = -9.9999e-99;
  if(!given("CIGSD"))
    cigsd = -9.9999e-99;
  if(!given("CIT"))
    cit = 0;
  if(!given("CJ"))
    cj = 5e-4;
  if(!given("CJGATE"))
    cjgate = 5e-10;
  if(!given("CJSW"))
    cjsw = 5e-10;
  if(!given("CJSWG"))
    cjswg = 5e-10;
  if(!given("CKAPPA"))
    ckappa = 0.6;
  if(!given("CLC"))
    clc = 0.1e-6;
  if(!given("CLE"))
    cle = 0.6;
  if(!given("CTA"))
    cta = 0;
  if(!given("CTP"))
    ctp = 0;
  if(!given("DEL"))
    del = 0.0;
  if(!given("DELK1"))
    delk1 = 0.0;
  if(!given("DELNFCT"))
    delnfct = 0.0;
  if(!given("DELTA"))
    delta = 0.01;
  if(!given("DELTOX"))
    deltox = 0.0;
  if(!given("DELVTO"))
    delvto = 0.0;
  if(!given("DLC"))
    dlc = -9.9999e-99;
  if(!given("DLCIG"))
    dlcig = -9.9999e-99;
  if(!given("DROUT"))
    drout = 0.56;
  if(!given("DSUB"))
    dsub = 0.56;
  if(!given("DTOXCV"))
    dtoxcv = 0.0;
  if(!given("DVT0"))
    dvt0 = 2.2;
  if(!given("DVT0W"))
    dvt0w = 0;
  if(!given("DVT1"))
    dvt1 = 0.53;
  if(!given("DVT1W"))
    dvt1w = 5.3e6;
  if(!given("DVT2"))
    dvt2 = -0.032;
  if(!given("DVT2W"))
    dvt2w = -0.032;
  if(!given("DWB"))
    dwb = 0;
  if(!given("DWC"))
    dwc = 0;
  if(!given("DWG"))
    dwg = 0;
  if(!given("EF"))
    ef = 1;
  if(!given("EG"))
    eg = 1.12452;
  if(!given("EGIDL"))
    egidl = -9.9999e-99;
  if(!given("EGISL"))
    egisl = -9.9999e-99;
  if(!given("EIGBINV"))
    eigbinv = -9.9999e-99;
  if(!given("ELM"))
    elm = 5;
  if(!given("EM"))
    em = -9.9999e-99;
  if(!given("ETA0"))
    eta0 = 0.08;
  if(!given("ETAB"))
    etab = -0.07;
  if(!given("FC"))
    fc = 0.5;
  if(!given("FCSW"))
    fcsw = 0.5;
  if(!given("FGIDL"))
    fgidl = -9.9999e-99;
  if(!given("FGISL"))
    fgisl = -9.9999e-99;
  if(!given("FLKMOD"))
    flkmod = 0;
  if(!given("GAMMA1"))
    gamma1 = -9.9999e-99;
  if(!given("GAMMA2"))
    gamma2 = 0.0;
  if(!given("GAP1"))
    gap1 = 7.02e-4;
  if(!given("GAP2"))
    gap2 = 1108;
  if(!given("GEO"))
    geo = 0;
  if(!given("GIDLMOD"))
    gidlmod = 0;
  if(!given("HDIF"))
    hdif = 0;
  if(!given("IGBMOD"))
    igbmod = 0.0;
  if(!given("IGCMOD"))
    igcmod = 0.0;
  if(!given("IJTH"))
    ijth = -9.9999e-99;
  if(!given("IJTHDREV"))
    ijthdrev = 0.1;
  if(!given("IJTHSREV"))
    ijthsrev = 0.1;
  if(!given("IMAX"))
    imax = 1;
  if(!given("IMELT"))
    imelt = -9.9999e-99;
  if(!given("IS"))
    is = 1e-14;
  if(!given("JMAX"))
    jmax = 1e8;
  if(!given("JMELT"))
    jmelt = -9.9999e-99;
  if(!given("JS"))
    js = 1e-4;
  if(!given("JSSW"))
    jssw = 0.0;
  if(!given("JSW"))
    jsw = 0;
  if(!given("K1"))
    k1 = -9.9999e-99;
  if(!given("K2"))
    k2 = -9.9999e-99;
  if(!given("K3"))
    k3 = 80;
  if(!given("K3B"))
    k3b = 0;
  if(!given("KETA"))
    keta = -0.047;
  if(!given("KF"))
    kf = 0;
  if(!given("KGIDL"))
    kgidl = -9.9999e-99;
  if(!given("KGISL"))
    kgisl = -9.9999e-99;
  if(!given("KT1"))
    kt1 = -0.11;
  if(!given("KT1L"))
    kt1l = 0;
  if(!given("KT2"))
    kt2 = 0.022;
  if(!given("KU0"))
    ku0 = -9.9999e-99;
  if(!given("KVSAT"))
    kvsat = -9.9999e-99;
  if(!given("KVTH0"))
    kvth0 = -9.9999e-99;
  if(!given("L"))
    l = 5e-6;
  if(!given("LA0"))
    la0 = 0.0;
  if(!given("LA1"))
    la1 = 0.0;
  if(!given("LA2"))
    la2 = 0.0;
  if(!given("LACDE"))
    lacde = 0.0;
  if(!given("LAGIDL"))
    lagidl = -9.9999e-99;
  if(!given("LAGISL"))
    lagisl = -9.9999e-99;
  if(!given("LAGS"))
    lags = 0.0;
  if(!given("LAIGBACC"))
    laigbacc = -9.9999e-99;
  if(!given("LAIGBINV"))
    laigbinv = -9.9999e-99;
  if(!given("LAIGC"))
    laigc = -9.9999e-99;
  if(!given("LAIGD"))
    laigd = -9.9999e-99;
  if(!given("LAIGS"))
    laigs = -9.9999e-99;
  if(!given("LAIGSD"))
    laigsd = -9.9999e-99;
  if(!given("LALPHA0"))
    lalpha0 = 0.0;
  if(!given("LALPHA1"))
    lalpha1 = 0.0;
  if(!given("LAT"))
    lat = 0.0;
  if(!given("LB0"))
    lb0 = 0.0;
  if(!given("LB1"))
    lb1 = 0.0;
  if(!given("LBETA0"))
    lbeta0 = 0.0;
  if(!given("LBGIDL"))
    lbgidl = -9.9999e-99;
  if(!given("LBGISL"))
    lbgisl = -9.9999e-99;
  if(!given("LBIGBACC"))
    lbigbacc = -9.9999e-99;
  if(!given("LBIGBINV"))
    lbigbinv = -9.9999e-99;
  if(!given("LBIGC"))
    lbigc = -9.9999e-99;
  if(!given("LBIGD"))
    lbigd = -9.9999e-99;
  if(!given("LBIGS"))
    lbigs = -9.9999e-99;
  if(!given("LBIGSD"))
    lbigsd = -9.9999e-99;
  if(!given("LCDSC"))
    lcdsc = 0.0;
  if(!given("LCDSCB"))
    lcdscb = 0.0;
  if(!given("LCDSCD"))
    lcdscd = 0.0;
  if(!given("LCF"))
    lcf = 0.0;
  if(!given("LCGDL"))
    lcgdl = 0.0;
  if(!given("LCGIDL"))
    lcgidl = -9.9999e-99;
  if(!given("LCGISL"))
    lcgisl = -9.9999e-99;
  if(!given("LCGSL"))
    lcgsl = 0.0;
  if(!given("LCIGBACC"))
    lcigbacc = -9.9999e-99;
  if(!given("LCIGBINV"))
    lcigbinv = -9.9999e-99;
  if(!given("LCIGC"))
    lcigc = -9.9999e-99;
  if(!given("LCIGD"))
    lcigd = -9.9999e-99;
  if(!given("LCIGS"))
    lcigs = -9.9999e-99;
  if(!given("LCIGSD"))
    lcigsd = -9.9999e-99;
  if(!given("LCIT"))
    lcit = 0.0;
  if(!given("LCKAPPA"))
    lckappa = 0.0;
  if(!given("LCLC"))
    lclc = 0.0;
  if(!given("LCLE"))
    lcle = 0.0;
  if(!given("LD"))
    ld = 0.0;
  if(!given("LDELTA"))
    ldelta = 0.0;
  if(!given("LDIF"))
    ldif = 0;
  if(!given("LDROUT"))
    ldrout = 0.0;
  if(!given("LDSUB"))
    ldsub = 0.0;
  if(!given("LDVT0"))
    ldvt0 = 0.0;
  if(!given("LDVT0W"))
    ldvt0w = 0.0;
  if(!given("LDVT1"))
    ldvt1 = 0.0;
  if(!given("LDVT1W"))
    ldvt1w = 0.0;
  if(!given("LDVT2"))
    ldvt2 = 0.0;
  if(!given("LDVT2W"))
    ldvt2w = 0.0;
  if(!given("LDWB"))
    ldwb = 0.0;
  if(!given("LDWG"))
    ldwg = 0.0;
  if(!given("LEGIDL"))
    legidl = -9.9999e-99;
  if(!given("LEGISL"))
    legisl = -9.9999e-99;
  if(!given("LEIGBINV"))
    leigbinv = -9.9999e-99;
  if(!given("LELM"))
    lelm = 0.0;
  if(!given("LETA0"))
    leta0 = 0.0;
  if(!given("LETAB"))
    letab = -0.0;
  if(!given("LEVEL"))
    level = 49;
  else if ( !(level >= 49 && level <= 53 ))
    UserError0(*this) << "Model Parameter value " << level << " out of range [49:53]";
  if(!given("LFGIDL"))
    lfgidl = -9.9999e-99;
  if(!given("LFGISL"))
    lfgisl = -9.9999e-99;
  if(!given("LGAMMA1"))
    lgamma1 = 0.0;
  if(!given("LGAMMA2"))
    lgamma2 = 0.0;
  if(!given("LGCD"))
    lgcd = 0;
  if(!given("LGCS"))
    lgcs = 0;
  if(!given("LINT"))
    lint = 0;
  if(!given("LK1"))
    lk1 = 0.0;
  if(!given("LK2"))
    lk2 = 0.0;
  if(!given("LK3"))
    lk3 = 0.0;
  if(!given("LK3B"))
    lk3b = 0.0;
  if(!given("LKETA"))
    lketa = 0.0;
  if(!given("LKGIDL"))
    lkgidl = -9.9999e-99;
  if(!given("LKGISL"))
    lkgisl = -9.9999e-99;
  if(!given("LKT1"))
    lkt1 = 0.0;
  if(!given("LKT1L"))
    lkt1l = 0.0;
  if(!given("LKT2"))
    lkt2 = 0.0;
  if(!given("LKU0"))
    lku0 = -9.9999e-99;
  if(!given("LKVTH0"))
    lkvth0 = -9.9999e-99;
  if(!given("LL"))
    ll = 0;
  if(!given("LLC"))
    llc = 0;
  if(!given("LLN"))
    lln = 1;
  if(!given("LLODKU0"))
    llodku0 = -9.9999e-99;
  if(!given("LLODVTH"))
    llodvth = -9.9999e-99;
  if(!given("LMAX"))
    lmax = 1;
  if(!given("LMIN"))
    lmin = 0;
  if(!given("LMLT"))
    lmlt = 1.0;
  if(!given("LMOIN"))
    lmoin = 0.0;
  if(!given("LNCH"))
    lnch = 0.0;
  if(!given("LNFACTOR"))
    lnfactor = 0.0;
  if(!given("LNGATE"))
    lngate = 0.0;
  if(!given("LNIGBACC"))
    lnigbacc = -9.9999e-99;
  if(!given("LNIGBINV"))
    lnigbinv = -9.9999e-99;
  if(!given("LNIGC"))
    lnigc = -9.9999e-99;
  if(!given("LNLX"))
    lnlx = 0.0;
  if(!given("LNOFF"))
    lnoff = 0.0;
  if(!given("LNPEAK"))
    lnpeak = 0.0;
  if(!given("LNSD"))
    lnsd = -9.9999e-99;
  if(!given("LNSUB"))
    lnsub = 0.0;
  if(!given("LNTOX"))
    lntox = -9.9999e-99;
  if(!given("LODETA0"))
    lodeta0 = -9.9999e-99;
  if(!given("LODK2"))
    lodk2 = -9.9999e-99;
  if(!given("LPCLM"))
    lpclm = 0.0;
  if(!given("LPDIBLC1"))
    lpdiblc1 = 0.0;
  if(!given("LPDIBLC2"))
    lpdiblc2 = 0.0;
  if(!given("LPDIBLCB"))
    lpdiblcb = 0.0;
  if(!given("LPIGCD"))
    lpigcd = -9.9999e-99;
  if(!given("LPOXEDGE"))
    lpoxedge = -9.9999e-99;
  if(!given("LPRT"))
    lprt = 0.0;
  if(!given("LPRWB"))
    lprwb = 0.0;
  if(!given("LPRWG"))
    lprwg = 0.0;
  if(!given("LPSCBE1"))
    lpscbe1 = 0.0;
  if(!given("LPSCBE2"))
    lpscbe2 = 0;
  if(!given("LPVAG"))
    lpvag = 0.0;
  if(!given("LRD"))
    lrd = 0;
  if(!given("LRDSW"))
    lrdsw = 0.0;
  if(!given("LREF"))
    lref = 0.0;
  if(!given("LRGIDL"))
    lrgidl = -9.9999e-99;
  if(!given("LRGISL"))
    lrgisl = -9.9999e-99;
  if(!given("LRS"))
    lrs = 0;
  if(!given("LTVFBSDOFF"))
    ltvfbsdoff = -9.9999e-99;
  if(!given("LU0"))
    lu0 = 0.0;
  if(!given("LUA"))
    lua = 0.0;
  if(!given("LUA1"))
    lua1 = 0.0;
  if(!given("LUB"))
    lub = 0.0;
  if(!given("LUB1"))
    lub1 = 0.0;
  if(!given("LUC"))
    luc = 0.0;
  if(!given("LUC1"))
    luc1 = 0.0;
  if(!given("LUTE"))
    lute = 0.0;
  if(!given("LVBM"))
    lvbm = 0.0;
  if(!given("LVBX"))
    lvbx = 0.0;
  if(!given("LVFB"))
    lvfb = 0.0;
  if(!given("LVFBCV"))
    lvfbcv = 0.0;
  if(!given("LVFBSDOFF"))
    lvfbsdoff = -9.9999e-99;
  if(!given("LVOFF"))
    lvoff = 0.0;
  if(!given("LVOFFCV"))
    lvoffcv = 0.0;
  if(!given("LVSAT"))
    lvsat = 0.0;
  if(!given("LVTH0"))
    lvth0 = 0.0;
  if(!given("LW"))
    lw = 0;
  if(!given("LW0"))
    lw0 = 0.0;
  if(!given("LWC"))
    lwc = 0;
  if(!given("LWL"))
    lwl = 0;
  if(!given("LWLC"))
    lwlc = 0;
  if(!given("LWN"))
    lwn = 1;
  if(!given("LWR"))
    lwr = 0.0;
  if(!given("LXJ"))
    lxj = 0.0;
  if(!given("LXT"))
    lxt = 0.0;
  if(!given("METO"))
    meto = 0;
  if(!given("MINR"))
    minr = 0.1;
  if(!given("MJ"))
    mj = 0.5;
  if(!given("MJSW"))
    mjsw = 0.33;
  if(!given("MJSWG"))
    mjswg = 0.33;
  if(!given("MOBMOD"))
    mobmod = 1;
  if(!given("MOIN"))
    moin = 15;
  if(!given("MULU0"))
    mulu0 = 1.0;
  if(!given("MULUA"))
    mulua = 1.0;
  if(!given("MULUB"))
    mulub = 1.0;
  if(!given("NCH"))
    nch = -9.9999e-99;
  if(!given("NF"))
    nf = 1.0;
  if(!given("NFACTOR"))
    nfactor = 1;
  if(!given("NGATE"))
    ngate = 0.0;
  if(!given("NIGBACC"))
    nigbacc = -9.9999e-99;
  if(!given("NIGBINV"))
    nigbinv = -9.9999e-99;
  if(!given("NIGC"))
    nigc = -9.9999e-99;
  if(!given("NJ"))
    nj = 1;
  if(!given("NLX"))
    nlx = 1.74e-7;
  if(!given("NOFF"))
    noff = 1;
  if(!given("NOIA"))
    noia = 1e20;
  if(!given("NOIB"))
    noib = 5e4;
  if(!given("NOIC"))
    noic = -1.4e-12;
  if(!given("NOIMOD"))
    noimod = 1;
  if(!given("NQSMOD"))
    nqsmod = 0;
  if(!given("NRD"))
    nrd = -9.9999e-99;
  if(!given("NRS"))
    nrs = -9.9999e-99;
  if(!given("NSD"))
    nsd = -9.9999e-99;
  if(!given("NSUB"))
    nsub = -9.9999e-99;
  if(!given("NTOX"))
    ntox = -9.9999e-99;
  if(!given("PA0"))
    pa0 = 0.0;
  if(!given("PA1"))
    pa1 = 0.0;
  if(!given("PA2"))
    pa2 = 0.0;
  if(!given("PACDE"))
    pacde = 0.0;
  if(!given("PAGIDL"))
    pagidl = -9.9999e-99;
  if(!given("PAGISL"))
    pagisl = -9.9999e-99;
  if(!given("PAGS"))
    pags = 0.0;
  if(!given("PAIGBACC"))
    paigbacc = -9.9999e-99;
  if(!given("PAIGBINV"))
    paigbinv = -9.9999e-99;
  if(!given("PAIGC"))
    paigc = -9.9999e-99;
  if(!given("PAIGD"))
    paigd = -9.9999e-99;
  if(!given("PAIGS"))
    paigs = -9.9999e-99;
  if(!given("PAIGSD"))
    paigsd = -9.9999e-99;
  if(!given("PALPHA0"))
    palpha0 = 0.0;
  if(!given("PALPHA1"))
    palpha1 = 0.0;
  if(!given("PARAMCHK"))
    paramchk = 0;
  if(!given("PAT"))
    pat = 0.0;
  if(!given("PB"))
    pb = 1;
  if(!given("PB0"))
    pb0 = 0.0;
  if(!given("PB1"))
    pb1 = 0.0;
  if(!given("PBETA0"))
    pbeta0 = 0.0;
  if(!given("PBGIDL"))
    pbgidl = -9.9999e-99;
  if(!given("PBGISL"))
    pbgisl = -9.9999e-99;
  if(!given("PBIGBACC"))
    pbigbacc = -9.9999e-99;
  if(!given("PBIGBINV"))
    pbigbinv = -9.9999e-99;
  if(!given("PBIGC"))
    pbigc = -9.9999e-99;
  if(!given("PBIGD"))
    pbigd = -9.9999e-99;
  if(!given("PBIGS"))
    pbigs = -9.9999e-99;
  if(!given("PBIGSD"))
    pbigsd = -9.9999e-99;
  if(!given("PBSW"))
    pbsw = 1;
  if(!given("PBSWG"))
    pbswg = 1;
  if(!given("PCDSC"))
    pcdsc = 0.0;
  if(!given("PCDSCB"))
    pcdscb = 0.0;
  if(!given("PCDSCD"))
    pcdscd = 0.0;
  if(!given("PCF"))
    pcf = 0.0;
  if(!given("PCGDL"))
    pcgdl = 0.0;
  if(!given("PCGIDL"))
    pcgidl = -9.9999e-99;
  if(!given("PCGISL"))
    pcgisl = -9.9999e-99;
  if(!given("PCGSL"))
    pcgsl = 0.0;
  if(!given("PCIGBACC"))
    pcigbacc = -9.9999e-99;
  if(!given("PCIGBINV"))
    pcigbinv = -9.9999e-99;
  if(!given("PCIGC"))
    pcigc = -9.9999e-99;
  if(!given("PCIGD"))
    pcigd = -9.9999e-99;
  if(!given("PCIGS"))
    pcigs = -9.9999e-99;
  if(!given("PCIGSD"))
    pcigsd = -9.9999e-99;
  if(!given("PCIT"))
    pcit = 0.0;
  if(!given("PCKAPPA"))
    pckappa = 0.0;
  if(!given("PCLC"))
    pclc = 0.0;
  if(!given("PCLE"))
    pcle = 0.0;
  if(!given("PCLM"))
    pclm = 1.3;
  if(!given("PD"))
    pd = -9.9999e-99;
  if(!given("PDELTA"))
    pdelta = 0.0;
  if(!given("PDIBLC1"))
    pdiblc1 = 0.39;
  if(!given("PDIBLC2"))
    pdiblc2 = 0.0086;
  if(!given("PDIBLCB"))
    pdiblcb = 0.0;
  if(!given("PDROUT"))
    pdrout = 0.0;
  if(!given("PDSUB"))
    pdsub = 0.0;
  if(!given("PDVT0"))
    pdvt0 = 0.0;
  if(!given("PDVT0W"))
    pdvt0w = 0.0;
  if(!given("PDVT1"))
    pdvt1 = 0.0;
  if(!given("PDVT1W"))
    pdvt1w = 0.0;
  if(!given("PDVT2"))
    pdvt2 = 0.0;
  if(!given("PDVT2W"))
    pdvt2w = 0.0;
  if(!given("PDWB"))
    pdwb = 0.0;
  if(!given("PDWG"))
    pdwg = 0.0;
  if(!given("PEGIDL"))
    pegidl = -9.9999e-99;
  if(!given("PEGISL"))
    pegisl = -9.9999e-99;
  if(!given("PEIGBINV"))
    peigbinv = -9.9999e-99;
  if(!given("PELM"))
    pelm = 0.0;
  if(!given("PETA0"))
    peta0 = 0.0;
  if(!given("PETAB"))
    petab = 0.0;
  if(!given("PFGIDL"))
    pfgidl = -9.9999e-99;
  if(!given("PFGISL"))
    pfgisl = -9.9999e-99;
  if(!given("PGAMMA1"))
    pgamma1 = 0.0;
  if(!given("PGAMMA2"))
    pgamma2 = 0.0;
  if(!given("PHP"))
    php = 1;
  if(!given("PIGCD"))
    pigcd = -9.9999e-99;
  if(!given("PK1"))
    pk1 = 0.0;
  if(!given("PK2"))
    pk2 = 0.0;
  if(!given("PK3"))
    pk3 = 0.0;
  if(!given("PK3B"))
    pk3b = 0.0;
  if(!given("PKETA"))
    pketa = 0.0;
  if(!given("PKGIDL"))
    pkgidl = -9.9999e-99;
  if(!given("PKGISL"))
    pkgisl = -9.9999e-99;
  if(!given("PKT1"))
    pkt1 = 0.0;
  if(!given("PKT1L"))
    pkt1l = 0.0;
  if(!given("PKT2"))
    pkt2 = 0.0;
  if(!given("PKU0"))
    pku0 = -9.9999e-99;
  if(!given("PKVTH0"))
    pkvth0 = -9.9999e-99;
  if(!given("PMOIN"))
    pmoin = 0.0;
  if(!given("PNCH"))
    pnch = 0.0;
  if(!given("PNFACTOR"))
    pnfactor = 0.0;
  if(!given("PNGATE"))
    pngate = 0.0;
  if(!given("PNIGBACC"))
    pnigbacc = -9.9999e-99;
  if(!given("PNIGBINV"))
    pnigbinv = -9.9999e-99;
  if(!given("PNIGC"))
    pnigc = -9.9999e-99;
  if(!given("PNLX"))
    pnlx = 0.0;
  if(!given("PNOFF"))
    pnoff = 0.0;
  if(!given("PNPEAK"))
    pnpeak = 0.0;
  if(!given("PNSD"))
    pnsd = -9.9999e-99;
  if(!given("PNSUB"))
    pnsub = 0.0;
  if(!given("PNTOX"))
    pntox = -9.9999e-99;
  if(!given("POXEDGE"))
    poxedge = -9.9999e-99;
  if(!given("PPCLM"))
    ppclm = 0.0;
  if(!given("PPDIBLC1"))
    ppdiblc1 = 0.0;
  if(!given("PPDIBLC2"))
    ppdiblc2 = 0.0;
  if(!given("PPDIBLCB"))
    ppdiblcb = 0.0;
  if(!given("PPIGCD"))
    ppigcd = -9.9999e-99;
  if(!given("PPOXEDGE"))
    ppoxedge = -9.9999e-99;
  if(!given("PPRT"))
    pprt = 0.0;
  if(!given("PPRWB"))
    pprwb = 0.0;
  if(!given("PPRWG"))
    pprwg = 0.0;
  if(!given("PPSCBE1"))
    ppscbe1 = 0.0;
  if(!given("PPSCBE2"))
    ppscbe2 = 0.0;
  if(!given("PPVAG"))
    ppvag = 0.0;
  if(!given("PRD"))
    prd = 0;
  if(!given("PRDSW"))
    prdsw = 0.0;
  if(!given("PRGIDL"))
    prgidl = -9.9999e-99;
  if(!given("PRGISL"))
    prgisl = -9.9999e-99;
  if(!given("PRS"))
    prs = 0;
  if(!given("PRT"))
    prt = 0;
  if(!given("PRWB"))
    prwb = 0;
  if(!given("PRWG"))
    prwg = 0;
  if(!given("PS"))
    ps = -9.9999e-99;
  if(!given("PSCBE1"))
    pscbe1 = 4.24e8;
  if(!given("PSCBE2"))
    pscbe2 = 1e-5;
  if(!given("PTA"))
    pta = 0;
  if(!given("PTP"))
    ptp = 0;
  if(!given("PTVFBSDOFF"))
    ptvfbsdoff = -9.9999e-99;
  if(!given("PU0"))
    pu0 = 0.0;
  if(!given("PUA"))
    pua = 0.0;
  if(!given("PUA1"))
    pua1 = 0.0;
  if(!given("PUB"))
    pub = 0.0;
  if(!given("PUB1"))
    pub1 = 0.0;
  if(!given("PUC"))
    puc = 0.0;
  if(!given("PUC1"))
    puc1 = 0.0;
  if(!given("PUTE"))
    pute = 0.0;
  if(!given("PVAG"))
    pvag = 0;
  if(!given("PVBM"))
    pvbm = 0.0;
  if(!given("PVBX"))
    pvbx = 0.0;
  if(!given("PVFB"))
    pvfb = 0.0;
  if(!given("PVFBCV"))
    pvfbcv = 0.0;
  if(!given("PVFBSDOFF"))
    pvfbsdoff = -9.9999e-99;
  if(!given("PVOFF"))
    pvoff = 0.0;
  if(!given("PVOFFCV"))
    pvoffcv = 0.0;
  if(!given("PVSAT"))
    pvsat = 0.0;
  if(!given("PVTH0"))
    pvth0 = 0.0;
  if(!given("PW0"))
    pw0 = 0.0;
  if(!given("PWR"))
    pwr = 0.0;
  if(!given("PXJ"))
    pxj = 0.0;
  if(!given("PXT"))
    pxt = 0.0;
  if(!given("RD"))
    rd = 0;
  if(!given("RDC"))
    rdc = 0;
  if(!given("RDSW"))
    rdsw = 0;
  if(!given("RGIDL"))
    rgidl = -9.9999e-99;
  if(!given("RGISL"))
    rgisl = -9.9999e-99;
  if(!given("RS"))
    rs = 0;
  if(!given("RSC"))
    rsc = 0;
  if(!given("RSH"))
    rsh = 0;
  if(!given("SA"))
    sa = -9.9999e-99;
  if(!given("SA0"))
    sa0 = -9.9999e-99;
  if(!given("SA1"))
    sa1 = -9.9999e-99;
  if(!given("SA10"))
    sa10 = -9.9999e-99;
  if(!given("SA2"))
    sa2 = -9.9999e-99;
  if(!given("SA3"))
    sa3 = -9.9999e-99;
  if(!given("SA4"))
    sa4 = -9.9999e-99;
  if(!given("SA5"))
    sa5 = -9.9999e-99;
  if(!given("SA6"))
    sa6 = -9.9999e-99;
  if(!given("SA7"))
    sa7 = -9.9999e-99;
  if(!given("SA8"))
    sa8 = -9.9999e-99;
  if(!given("SA9"))
    sa9 = -9.9999e-99;
  if(!given("SAREF"))
    saref = -9.9999e-99;
  if(!given("SB"))
    sb = -9.9999e-99;
  if(!given("SB0"))
    sb0 = -9.9999e-99;
  if(!given("SB1"))
    sb1 = -9.9999e-99;
  if(!given("SB10"))
    sb10 = -9.9999e-99;
  if(!given("SB2"))
    sb2 = -9.9999e-99;
  if(!given("SB3"))
    sb3 = -9.9999e-99;
  if(!given("SB4"))
    sb4 = -9.9999e-99;
  if(!given("SB5"))
    sb5 = -9.9999e-99;
  if(!given("SB6"))
    sb6 = -9.9999e-99;
  if(!given("SB7"))
    sb7 = -9.9999e-99;
  if(!given("SB8"))
    sb8 = -9.9999e-99;
  if(!given("SB9"))
    sb9 = -9.9999e-99;
  if(!given("SBREF"))
    sbref = -9.9999e-99;
  if(!given("SC"))
    sc = -9.9999e-99;
  if(!given("SD"))
    sd = 0.0;
  if(!given("STETA0"))
    steta0 = -9.9999e-99;
  if(!given("STIMOD"))
    stimod = 0;
  else if ( !(stimod >= 0 && stimod <= 2 ))
    UserError0(*this) << "Model Parameter value " << stimod << " out of range [0:2]";
  if(!given("STK2"))
    stk2 = -9.9999e-99;
  if(!given("SW1"))
    sw1 = -9.9999e-99;
  if(!given("SW10"))
    sw10 = -9.9999e-99;
  if(!given("SW2"))
    sw2 = -9.9999e-99;
  if(!given("SW3"))
    sw3 = -9.9999e-99;
  if(!given("SW4"))
    sw4 = -9.9999e-99;
  if(!given("SW5"))
    sw5 = -9.9999e-99;
  if(!given("SW6"))
    sw6 = -9.9999e-99;
  if(!given("SW7"))
    sw7 = -9.9999e-99;
  if(!given("SW8"))
    sw8 = -9.9999e-99;
  if(!given("SW9"))
    sw9 = -9.9999e-99;
  if(!given("TCJ"))
    tcj = 0;
  if(!given("TCJSW"))
    tcjsw = 0;
  if(!given("TCJSWG"))
    tcjswg = 0;
  if(!given("TEMPMOD"))
    tempmod = 0.0;
  if(!given("TKU0"))
    tku0 = -9.9999e-99;
  if(!given("TLEV"))
    tlev = 0;
  if(!given("TLEVC"))
    tlevc = 0;
  if(!given("TNOM"))
    tnom = 25;
  else
    if(tnom < -100 || tnom>150)
    tnom -= P_CELSIUS0;
  if(!given("TOX"))
    tox = 150.0e-10;
  if(!given("TOXE"))
    toxe = -9.9999e-99;
  if(!given("TOXM"))
    toxm = 150.0e-10;
  if(!given("TOXREF"))
    toxref = -9.9999e-99;
  if(!given("TPB"))
    tpb = 0;
  if(!given("TPBSW"))
    tpbsw = 0;
  if(!given("TPBSWG"))
    tpbswg = 0;
  if(!given("TRD"))
    trd = 0;
  if(!given("TRISE"))
    trise = 0;
  if(!given("TRS"))
    trs = 0;
  if(!given("TVFBSDOFF"))
    tvfbsdoff = -9.9999e-99;
  if(!given("TYPE"))
    type = 1;
  if(!given("U0"))
    u0 = -9.9999e-99;
  if(!given("UA"))
    ua = 2.25e-9;
  if(!given("UA1"))
    ua1 = 4.31e-9;
  if(!given("UB"))
    ub = 5.87e-19;
  if(!given("UB1"))
    ub1 = -7.61e-18;
  if(!given("UC"))
    uc = -9.9999e-99;
  if(!given("UC1"))
    uc1 = -9.9999e-99;
  if(!given("UTE"))
    ute = -1.5;
  if(!given("VBM"))
    vbm = -3;
  if(!given("VBOX"))
    vbox = 1e9;
  if(!given("VBX"))
    vbx = -9.9999e-99;
  if(!given("VERBOSE"))
    verbose = -9999999;
  if(!given("VERSION"))
    version = 3.3;
  else if ( !(version >= 3.00 && version <= 3.3 ))
    UserError0(*this) << "Model Parameter value " << version << " out of range [3.00:3.3]";
  if(!given("VFB"))
    vfb = -9.9999e-99;
  if(!given("VFBCV"))
    vfbcv = -1;
  if(!given("VFBFLAG"))
    vfbflag = 0;
  if(!given("VFBSDOFF"))
    vfbsdoff = -9.9999e-99;
  if(!given("VGSLIM"))
    vgslim = 0.0;
  if(!given("VOFF"))
    voff = -0.08;
  if(!given("VOFFCV"))
    voffcv = 0;
  if(!given("VSAT"))
    vsat = 8e4;
  if(!given("VTH0"))
    vth0 = -9.9999e-99;
  if(!given("VTM0"))
    vtm0 = -9.9999e-99;
  if(!given("W"))
    w = 5e-6;
  if(!given("W0"))
    w0 = 2.5e-6;
  if(!given("WA0"))
    wa0 = 0.0;
  if(!given("WA1"))
    wa1 = 0.0;
  if(!given("WA2"))
    wa2 = 0.0;
  if(!given("WACDE"))
    wacde = 0.0;
  if(!given("WAGIDL"))
    wagidl = -9.9999e-99;
  if(!given("WAGISL"))
    wagisl = -9.9999e-99;
  if(!given("WAGS"))
    wags = 0.0;
  if(!given("WAIGBACC"))
    waigbacc = -9.9999e-99;
  if(!given("WAIGBINV"))
    waigbinv = -9.9999e-99;
  if(!given("WAIGC"))
    waigc = -9.9999e-99;
  if(!given("WAIGD"))
    waigd = -9.9999e-99;
  if(!given("WAIGS"))
    waigs = -9.9999e-99;
  if(!given("WAIGSD"))
    waigsd = -9.9999e-99;
  if(!given("WALPHA0"))
    walpha0 = 0.0;
  if(!given("WALPHA1"))
    walpha1 = 0.0;
  if(!given("WAT"))
    wat = 0.0;
  if(!given("WB0"))
    wb0 = 0.0;
  if(!given("WB1"))
    wb1 = 0.0;
  if(!given("WBETA0"))
    wbeta0 = 0.0;
  if(!given("WBGIDL"))
    wbgidl = -9.9999e-99;
  if(!given("WBGISL"))
    wbgisl = -9.9999e-99;
  if(!given("WBIGBACC"))
    wbigbacc = -9.9999e-99;
  if(!given("WBIGBINV"))
    wbigbinv = -9.9999e-99;
  if(!given("WBIGC"))
    wbigc = -9.9999e-99;
  if(!given("WBIGD"))
    wbigd = -9.9999e-99;
  if(!given("WBIGS"))
    wbigs = -9.9999e-99;
  if(!given("WBIGSD"))
    wbigsd = -9.9999e-99;
  if(!given("WCDSC"))
    wcdsc = 0.0;
  if(!given("WCDSCB"))
    wcdscb = 0.0;
  if(!given("WCDSCD"))
    wcdscd = 0.0;
  if(!given("WCF"))
    wcf = 0.0;
  if(!given("WCGDL"))
    wcgdl = 0.0;
  if(!given("WCGIDL"))
    wcgidl = -9.9999e-99;
  if(!given("WCGISL"))
    wcgisl = -9.9999e-99;
  if(!given("WCGSL"))
    wcgsl = 0.0;
  if(!given("WCIGBACC"))
    wcigbacc = -9.9999e-99;
  if(!given("WCIGBINV"))
    wcigbinv = -9.9999e-99;
  if(!given("WCIGC"))
    wcigc = -9.9999e-99;
  if(!given("WCIGD"))
    wcigd = -9.9999e-99;
  if(!given("WCIGS"))
    wcigs = -9.9999e-99;
  if(!given("WCIGSD"))
    wcigsd = -9.9999e-99;
  if(!given("WCIT"))
    wcit = 0.0;
  if(!given("WCKAPPA"))
    wckappa = 0.0;
  if(!given("WCLC"))
    wclc = 0.0;
  if(!given("WCLE"))
    wcle = 0.0;
  if(!given("WD"))
    wd = 0.0;
  if(!given("WDELTA"))
    wdelta = 0.0;
  if(!given("WDROUT"))
    wdrout = 0.0;
  if(!given("WDSUB"))
    wdsub = 0.0;
  if(!given("WDVT0"))
    wdvt0 = 0.0;
  if(!given("WDVT0W"))
    wdvt0w = 0.0;
  if(!given("WDVT1"))
    wdvt1 = 0.0;
  if(!given("WDVT1W"))
    wdvt1w = 0.0;
  if(!given("WDVT2"))
    wdvt2 = 0.0;
  if(!given("WDVT2W"))
    wdvt2w = 0.0;
  if(!given("WDWB"))
    wdwb = 0.0;
  if(!given("WDWG"))
    wdwg = 0.0;
  if(!given("WEGIDL"))
    wegidl = -9.9999e-99;
  if(!given("WEGISL"))
    wegisl = -9.9999e-99;
  if(!given("WEIGBINV"))
    weigbinv = -9.9999e-99;
  if(!given("WELM"))
    welm = 0.0;
  if(!given("WETA0"))
    weta0 = 0.0;
  if(!given("WETAB"))
    wetab = 0.0;
  if(!given("WFGIDL"))
    wfgidl = -9.9999e-99;
  if(!given("WFGISL"))
    wfgisl = -9.9999e-99;
  if(!given("WGAMMA1"))
    wgamma1 = 0.0;
  if(!given("WGAMMA2"))
    wgamma2 = 0.0;
  if(!given("WINT"))
    wint = 0;
  if(!given("WK1"))
    wk1 = 0.0;
  if(!given("WK2"))
    wk2 = 0.0;
  if(!given("WK3"))
    wk3 = 0.0;
  if(!given("WK3B"))
    wk3b = 0.0;
  if(!given("WKETA"))
    wketa = 0.0;
  if(!given("WKGIDL"))
    wkgidl = -9.9999e-99;
  if(!given("WKGISL"))
    wkgisl = -9.9999e-99;
  if(!given("WKT1"))
    wkt1 = 0.0;
  if(!given("WKT1L"))
    wkt1l = 0.0;
  if(!given("WKT2"))
    wkt2 = 0.0;
  if(!given("WKU0"))
    wku0 = -9.9999e-99;
  if(!given("WKVTH0"))
    wkvth0 = -9.9999e-99;
  if(!given("WL"))
    wl = 0;
  if(!given("WLC"))
    wlc = 0;
  if(!given("WLN"))
    wln = 1;
  if(!given("WLOD"))
    wlod = -9.9999e-99;
  if(!given("WLODKU0"))
    wlodku0 = -9.9999e-99;
  if(!given("WLODVTH"))
    wlodvth = -9.9999e-99;
  if(!given("WMAX"))
    wmax = 1;
  if(!given("WMIN"))
    wmin = 0;
  if(!given("WMLT"))
    wmlt = 1.0;
  if(!given("WMOIN"))
    wmoin = 0.0;
  if(!given("WNCH"))
    wnch = 0.0;
  if(!given("WNFACTOR"))
    wnfactor = 0.0;
  if(!given("WNGATE"))
    wngate = 0.0;
  if(!given("WNIGBACC"))
    wnigbacc = -9.9999e-99;
  if(!given("WNIGBINV"))
    wnigbinv = -9.9999e-99;
  if(!given("WNIGC"))
    wnigc = -9.9999e-99;
  if(!given("WNLX"))
    wnlx = 0.0;
  if(!given("WNOFF"))
    wnoff = 0.0;
  if(!given("WNOI"))
    wnoi = 1e-5;
  if(!given("WNPEAK"))
    wnpeak = 0.0;
  if(!given("WNSD"))
    wnsd = -9.9999e-99;
  if(!given("WNSUB"))
    wnsub = 0.0;
  if(!given("WNTOX"))
    wntox = -9.9999e-99;
  if(!given("WPCLM"))
    wpclm = 0.0;
  if(!given("WPDIBLC1"))
    wpdiblc1 = 0.0;
  if(!given("WPDIBLC2"))
    wpdiblc2 = 0.0;
  if(!given("WPDIBLCB"))
    wpdiblcb = 0.0;
  if(!given("WPIGCD"))
    wpigcd = -9.9999e-99;
  if(!given("WPOXEDGE"))
    wpoxedge = -9.9999e-99;
  if(!given("WPRT"))
    wprt = 0.0;
  if(!given("WPRWB"))
    wprwb = 0.0;
  if(!given("WPRWG"))
    wprwg = 0.0;
  if(!given("WPSCBE1"))
    wpscbe1 = 0.0;
  if(!given("WPSCBE2"))
    wpscbe2 = 0.0;
  if(!given("WPVAG"))
    wpvag = 0.0;
  if(!given("WR"))
    wr = 1;
  if(!given("WRD"))
    wrd = 0;
  if(!given("WRDSW"))
    wrdsw = 0.0;
  if(!given("WREF"))
    wref = 0.0;
  if(!given("WRGIDL"))
    wrgidl = -9.9999e-99;
  if(!given("WRGISL"))
    wrgisl = -9.9999e-99;
  if(!given("WRS"))
    wrs = 0;
  if(!given("WTVFBSDOFF"))
    wtvfbsdoff = -9.9999e-99;
  if(!given("WU0"))
    wu0 = 0.0;
  if(!given("WUA"))
    wua = 0.0;
  if(!given("WUA1"))
    wua1 = 0.0;
  if(!given("WUB"))
    wub = 0.0;
  if(!given("WUB1"))
    wub1 = 0.0;
  if(!given("WUC"))
    wuc = 0.0;
  if(!given("WUC1"))
    wuc1 = 0.0;
  if(!given("WUTE"))
    wute = 0.0;
  if(!given("WVBM"))
    wvbm = 0.0;
  if(!given("WVBX"))
    wvbx = 0.0;
  if(!given("WVFB"))
    wvfb = 0.0;
  if(!given("WVFBCV"))
    wvfbcv = 0.0;
  if(!given("WVFBSDOFF"))
    wvfbsdoff = -9.9999e-99;
  if(!given("WVOFF"))
    wvoff = 0.0;
  if(!given("WVOFFCV"))
    wvoffcv = 0.0;
  if(!given("WVSAT"))
    wvsat = 0.0;
  if(!given("WVTH0"))
    wvth0 = 0.0;
  if(!given("WW"))
    ww = 0;
  if(!given("WW0"))
    ww0 = 0.0;
  if(!given("WWC"))
    wwc = 0;
  if(!given("WWL"))
    wwl = 0;
  if(!given("WWLC"))
    wwlc = 0;
  if(!given("WWN"))
    wwn = 1;
  if(!given("WWR"))
    wwr = 0.0;
  if(!given("WXJ"))
    wxj = 0.0;
  if(!given("WXT"))
    wxt = 0.0;
  if(!given("XJ"))
    xj = 0.15e-6;
  if(!given("XJBVD"))
    xjbvd = 1.0;
  if(!given("XJBVS"))
    xjbvs = 1.0;
  if(!given("XL"))
    xl = 0;
  if(!given("XLREF"))
    xlref = 0.0;
  if(!given("XPART"))
    xpart = -9.9999e-99;
  if(!given("XT"))
    xt = -9.9999e-99;
  if(!given("XTI"))
    xti = 3;
  if(!given("XW"))
    xw = 0;
  if(!given("XWREF"))
    xwref = 0.0;
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
    a0(1),
    a1(0),
    a2(1),
    acde(1),
    acm(-9999999),
    ad(-9.9999e-99),
    af(1),
    agidl(-9.9999e-99),
    agisl(-9.9999e-99),
    ags(0),
    aigbacc(-9.9999e-99),
    aigbinv(-9.9999e-99),
    aigc(-9.9999e-99),
    aigd(-9.9999e-99),
    aigs(-9.9999e-99),
    aigsd(-9.9999e-99),
    alpha0(0),
    alpha1(0),
    as(-9.9999e-99),
    at(3.3e4),
    b0(0),
    b1(0),
    beta0(30),
    bgidl(-9.9999e-99),
    bgisl(-9.9999e-99),
    bigbacc(-9.9999e-99),
    bigbinv(-9.9999e-99),
    bigc(-9.9999e-99),
    bigd(-9.9999e-99),
    bigs(-9.9999e-99),
    bigsd(-9.9999e-99),
    binflag(0),
    binunit(1),
    breakmod(0),
    bvd(10.0),
    bvj(-9.9999e-99),
    bvs(10.0),
    calcacm(-9999999),
    capmod(3),
    cbd(0),
    cbs(0),
    cdsc(2.4e-4),
    cdscb(0),
    cdscd(0),
    cf(-9.9999e-99),
    cgbo(-9.9999e-99),
    cgdl(0),
    cgdo(-9.9999e-99),
    cgidl(-9.9999e-99),
    cgisl(-9.9999e-99),
    cgsl(0),
    cgso(-9.9999e-99),
    cigbacc(-9.9999e-99),
    cigbinv(-9.9999e-99),
    cigc(-9.9999e-99),
    cigd(-9.9999e-99),
    cigs(-9.9999e-99),
    cigsd(-9.9999e-99),
    cit(0),
    cj(5e-4),
    cjgate(5e-10),
    cjsw(5e-10),
    cjswg(5e-10),
    ckappa(0.6),
    clc(0.1e-6),
    cle(0.6),
    cta(0),
    ctp(0),
    del(0.0),
    delk1(0.0),
    delnfct(0.0),
    delta(0.01),
    deltox(0.0),
    delvto(0.0),
    dlc(-9.9999e-99),
    dlcig(-9.9999e-99),
    drout(0.56),
    dsub(0.56),
    dtoxcv(0.0),
    dvt0(2.2),
    dvt0w(0),
    dvt1(0.53),
    dvt1w(5.3e6),
    dvt2(-0.032),
    dvt2w(-0.032),
    dwb(0),
    dwc(0),
    dwg(0),
    ef(1),
    eg(1.12452),
    egidl(-9.9999e-99),
    egisl(-9.9999e-99),
    eigbinv(-9.9999e-99),
    elm(5),
    em(-9.9999e-99),
    eta0(0.08),
    etab(-0.07),
    fc(0.5),
    fcsw(0.5),
    fgidl(-9.9999e-99),
    fgisl(-9.9999e-99),
    flkmod(0),
    gamma1(-9.9999e-99),
    gamma2(0.0),
    gap1(7.02e-4),
    gap2(1108),
    geo(0),
    gidlmod(0),
    hdif(0),
    igbmod(0.0),
    igcmod(0.0),
    ijth(-9.9999e-99),
    ijthdrev(0.1),
    ijthsrev(0.1),
    imax(1),
    imelt(-9.9999e-99),
    is(1e-14),
    jmax(1e8),
    jmelt(-9.9999e-99),
    js(1e-4),
    jssw(0.0),
    jsw(0),
    k1(-9.9999e-99),
    k2(-9.9999e-99),
    k3(80),
    k3b(0),
    keta(-0.047),
    kf(0),
    kgidl(-9.9999e-99),
    kgisl(-9.9999e-99),
    kt1(-0.11),
    kt1l(0),
    kt2(0.022),
    ku0(-9.9999e-99),
    kvsat(-9.9999e-99),
    kvth0(-9.9999e-99),
    l(5e-6),
    la0(0.0),
    la1(0.0),
    la2(0.0),
    lacde(0.0),
    lagidl(-9.9999e-99),
    lagisl(-9.9999e-99),
    lags(0.0),
    laigbacc(-9.9999e-99),
    laigbinv(-9.9999e-99),
    laigc(-9.9999e-99),
    laigd(-9.9999e-99),
    laigs(-9.9999e-99),
    laigsd(-9.9999e-99),
    lalpha0(0.0),
    lalpha1(0.0),
    lat(0.0),
    lb0(0.0),
    lb1(0.0),
    lbeta0(0.0),
    lbgidl(-9.9999e-99),
    lbgisl(-9.9999e-99),
    lbigbacc(-9.9999e-99),
    lbigbinv(-9.9999e-99),
    lbigc(-9.9999e-99),
    lbigd(-9.9999e-99),
    lbigs(-9.9999e-99),
    lbigsd(-9.9999e-99),
    lcdsc(0.0),
    lcdscb(0.0),
    lcdscd(0.0),
    lcf(0.0),
    lcgdl(0.0),
    lcgidl(-9.9999e-99),
    lcgisl(-9.9999e-99),
    lcgsl(0.0),
    lcigbacc(-9.9999e-99),
    lcigbinv(-9.9999e-99),
    lcigc(-9.9999e-99),
    lcigd(-9.9999e-99),
    lcigs(-9.9999e-99),
    lcigsd(-9.9999e-99),
    lcit(0.0),
    lckappa(0.0),
    lclc(0.0),
    lcle(0.0),
    ld(0.0),
    ldelta(0.0),
    ldif(0),
    ldrout(0.0),
    ldsub(0.0),
    ldvt0(0.0),
    ldvt0w(0.0),
    ldvt1(0.0),
    ldvt1w(0.0),
    ldvt2(0.0),
    ldvt2w(0.0),
    ldwb(0.0),
    ldwg(0.0),
    legidl(-9.9999e-99),
    legisl(-9.9999e-99),
    leigbinv(-9.9999e-99),
    lelm(0.0),
    leta0(0.0),
    letab(-0.0),
    level(49),
    lfgidl(-9.9999e-99),
    lfgisl(-9.9999e-99),
    lgamma1(0.0),
    lgamma2(0.0),
    lgcd(0),
    lgcs(0),
    lint(0),
    lk1(0.0),
    lk2(0.0),
    lk3(0.0),
    lk3b(0.0),
    lketa(0.0),
    lkgidl(-9.9999e-99),
    lkgisl(-9.9999e-99),
    lkt1(0.0),
    lkt1l(0.0),
    lkt2(0.0),
    lku0(-9.9999e-99),
    lkvth0(-9.9999e-99),
    ll(0),
    llc(0),
    lln(1),
    llodku0(-9.9999e-99),
    llodvth(-9.9999e-99),
    lmax(1),
    lmin(0),
    lmlt(1.0),
    lmoin(0.0),
    lnch(0.0),
    lnfactor(0.0),
    lngate(0.0),
    lnigbacc(-9.9999e-99),
    lnigbinv(-9.9999e-99),
    lnigc(-9.9999e-99),
    lnlx(0.0),
    lnoff(0.0),
    lnpeak(0.0),
    lnsd(-9.9999e-99),
    lnsub(0.0),
    lntox(-9.9999e-99),
    lodeta0(-9.9999e-99),
    lodk2(-9.9999e-99),
    lpclm(0.0),
    lpdiblc1(0.0),
    lpdiblc2(0.0),
    lpdiblcb(0.0),
    lpigcd(-9.9999e-99),
    lpoxedge(-9.9999e-99),
    lprt(0.0),
    lprwb(0.0),
    lprwg(0.0),
    lpscbe1(0.0),
    lpscbe2(0),
    lpvag(0.0),
    lrd(0),
    lrdsw(0.0),
    lref(0.0),
    lrgidl(-9.9999e-99),
    lrgisl(-9.9999e-99),
    lrs(0),
    ltvfbsdoff(-9.9999e-99),
    lu0(0.0),
    lua(0.0),
    lua1(0.0),
    lub(0.0),
    lub1(0.0),
    luc(0.0),
    luc1(0.0),
    lute(0.0),
    lvbm(0.0),
    lvbx(0.0),
    lvfb(0.0),
    lvfbcv(0.0),
    lvfbsdoff(-9.9999e-99),
    lvoff(0.0),
    lvoffcv(0.0),
    lvsat(0.0),
    lvth0(0.0),
    lw(0),
    lw0(0.0),
    lwc(0),
    lwl(0),
    lwlc(0),
    lwn(1),
    lwr(0.0),
    lxj(0.0),
    lxt(0.0),
    meto(0),
    minr(0.1),
    mj(0.5),
    mjsw(0.33),
    mjswg(0.33),
    mobmod(1),
    moin(15),
    mulu0(1.0),
    mulua(1.0),
    mulub(1.0),
    nch(-9.9999e-99),
    nf(1.0),
    nfactor(1),
    ngate(0.0),
    nigbacc(-9.9999e-99),
    nigbinv(-9.9999e-99),
    nigc(-9.9999e-99),
    nj(1),
    nlx(1.74e-7),
    noff(1),
    noia(1e20),
    noib(5e4),
    noic(-1.4e-12),
    noimod(1),
    nqsmod(0),
    nrd(-9.9999e-99),
    nrs(-9.9999e-99),
    nsd(-9.9999e-99),
    nsub(-9.9999e-99),
    ntox(-9.9999e-99),
    pa0(0.0),
    pa1(0.0),
    pa2(0.0),
    pacde(0.0),
    pagidl(-9.9999e-99),
    pagisl(-9.9999e-99),
    pags(0.0),
    paigbacc(-9.9999e-99),
    paigbinv(-9.9999e-99),
    paigc(-9.9999e-99),
    paigd(-9.9999e-99),
    paigs(-9.9999e-99),
    paigsd(-9.9999e-99),
    palpha0(0.0),
    palpha1(0.0),
    paramchk(0),
    pat(0.0),
    pb(1),
    pb0(0.0),
    pb1(0.0),
    pbeta0(0.0),
    pbgidl(-9.9999e-99),
    pbgisl(-9.9999e-99),
    pbigbacc(-9.9999e-99),
    pbigbinv(-9.9999e-99),
    pbigc(-9.9999e-99),
    pbigd(-9.9999e-99),
    pbigs(-9.9999e-99),
    pbigsd(-9.9999e-99),
    pbsw(1),
    pbswg(1),
    pcdsc(0.0),
    pcdscb(0.0),
    pcdscd(0.0),
    pcf(0.0),
    pcgdl(0.0),
    pcgidl(-9.9999e-99),
    pcgisl(-9.9999e-99),
    pcgsl(0.0),
    pcigbacc(-9.9999e-99),
    pcigbinv(-9.9999e-99),
    pcigc(-9.9999e-99),
    pcigd(-9.9999e-99),
    pcigs(-9.9999e-99),
    pcigsd(-9.9999e-99),
    pcit(0.0),
    pckappa(0.0),
    pclc(0.0),
    pcle(0.0),
    pclm(1.3),
    pd(-9.9999e-99),
    pdelta(0.0),
    pdiblc1(0.39),
    pdiblc2(0.0086),
    pdiblcb(0.0),
    pdrout(0.0),
    pdsub(0.0),
    pdvt0(0.0),
    pdvt0w(0.0),
    pdvt1(0.0),
    pdvt1w(0.0),
    pdvt2(0.0),
    pdvt2w(0.0),
    pdwb(0.0),
    pdwg(0.0),
    pegidl(-9.9999e-99),
    pegisl(-9.9999e-99),
    peigbinv(-9.9999e-99),
    pelm(0.0),
    peta0(0.0),
    petab(0.0),
    pfgidl(-9.9999e-99),
    pfgisl(-9.9999e-99),
    pgamma1(0.0),
    pgamma2(0.0),
    php(1),
    pigcd(-9.9999e-99),
    pk1(0.0),
    pk2(0.0),
    pk3(0.0),
    pk3b(0.0),
    pketa(0.0),
    pkgidl(-9.9999e-99),
    pkgisl(-9.9999e-99),
    pkt1(0.0),
    pkt1l(0.0),
    pkt2(0.0),
    pku0(-9.9999e-99),
    pkvth0(-9.9999e-99),
    pmoin(0.0),
    pnch(0.0),
    pnfactor(0.0),
    pngate(0.0),
    pnigbacc(-9.9999e-99),
    pnigbinv(-9.9999e-99),
    pnigc(-9.9999e-99),
    pnlx(0.0),
    pnoff(0.0),
    pnpeak(0.0),
    pnsd(-9.9999e-99),
    pnsub(0.0),
    pntox(-9.9999e-99),
    poxedge(-9.9999e-99),
    ppclm(0.0),
    ppdiblc1(0.0),
    ppdiblc2(0.0),
    ppdiblcb(0.0),
    ppigcd(-9.9999e-99),
    ppoxedge(-9.9999e-99),
    pprt(0.0),
    pprwb(0.0),
    pprwg(0.0),
    ppscbe1(0.0),
    ppscbe2(0.0),
    ppvag(0.0),
    prd(0),
    prdsw(0.0),
    prgidl(-9.9999e-99),
    prgisl(-9.9999e-99),
    prs(0),
    prt(0),
    prwb(0),
    prwg(0),
    ps(-9.9999e-99),
    pscbe1(4.24e8),
    pscbe2(1e-5),
    pta(0),
    ptp(0),
    ptvfbsdoff(-9.9999e-99),
    pu0(0.0),
    pua(0.0),
    pua1(0.0),
    pub(0.0),
    pub1(0.0),
    puc(0.0),
    puc1(0.0),
    pute(0.0),
    pvag(0),
    pvbm(0.0),
    pvbx(0.0),
    pvfb(0.0),
    pvfbcv(0.0),
    pvfbsdoff(-9.9999e-99),
    pvoff(0.0),
    pvoffcv(0.0),
    pvsat(0.0),
    pvth0(0.0),
    pw0(0.0),
    pwr(0.0),
    pxj(0.0),
    pxt(0.0),
    rd(0),
    rdc(0),
    rdsw(0),
    rgidl(-9.9999e-99),
    rgisl(-9.9999e-99),
    rs(0),
    rsc(0),
    rsh(0),
    sa(-9.9999e-99),
    sa0(-9.9999e-99),
    sa1(-9.9999e-99),
    sa10(-9.9999e-99),
    sa2(-9.9999e-99),
    sa3(-9.9999e-99),
    sa4(-9.9999e-99),
    sa5(-9.9999e-99),
    sa6(-9.9999e-99),
    sa7(-9.9999e-99),
    sa8(-9.9999e-99),
    sa9(-9.9999e-99),
    saref(-9.9999e-99),
    sb(-9.9999e-99),
    sb0(-9.9999e-99),
    sb1(-9.9999e-99),
    sb10(-9.9999e-99),
    sb2(-9.9999e-99),
    sb3(-9.9999e-99),
    sb4(-9.9999e-99),
    sb5(-9.9999e-99),
    sb6(-9.9999e-99),
    sb7(-9.9999e-99),
    sb8(-9.9999e-99),
    sb9(-9.9999e-99),
    sbref(-9.9999e-99),
    sc(-9.9999e-99),
    sd(0.0),
    steta0(-9.9999e-99),
    stimod(0),
    stk2(-9.9999e-99),
    sw1(-9.9999e-99),
    sw10(-9.9999e-99),
    sw2(-9.9999e-99),
    sw3(-9.9999e-99),
    sw4(-9.9999e-99),
    sw5(-9.9999e-99),
    sw6(-9.9999e-99),
    sw7(-9.9999e-99),
    sw8(-9.9999e-99),
    sw9(-9.9999e-99),
    tcj(0),
    tcjsw(0),
    tcjswg(0),
    tempmod(0.0),
    tku0(-9.9999e-99),
    tlev(0),
    tlevc(0),
    tnom(25),
    tox(150.0e-10),
    toxe(-9.9999e-99),
    toxm(150.0e-10),
    toxref(-9.9999e-99),
    tpb(0),
    tpbsw(0),
    tpbswg(0),
    trd(0),
    trise(0),
    trs(0),
    tvfbsdoff(-9.9999e-99),
    type(1),
    u0(-9.9999e-99),
    ua(2.25e-9),
    ua1(4.31e-9),
    ub(5.87e-19),
    ub1(-7.61e-18),
    uc(-9.9999e-99),
    uc1(-9.9999e-99),
    ute(-1.5),
    vbm(-3),
    vbox(1e9),
    vbx(-9.9999e-99),
    verbose(-9999999),
    version(3.3),
    vfb(-9.9999e-99),
    vfbcv(-1),
    vfbflag(0),
    vfbsdoff(-9.9999e-99),
    vgslim(0.0),
    voff(-0.08),
    voffcv(0),
    vsat(8e4),
    vth0(-9.9999e-99),
    vtm0(-9.9999e-99),
    w(5e-6),
    w0(2.5e-6),
    wa0(0.0),
    wa1(0.0),
    wa2(0.0),
    wacde(0.0),
    wagidl(-9.9999e-99),
    wagisl(-9.9999e-99),
    wags(0.0),
    waigbacc(-9.9999e-99),
    waigbinv(-9.9999e-99),
    waigc(-9.9999e-99),
    waigd(-9.9999e-99),
    waigs(-9.9999e-99),
    waigsd(-9.9999e-99),
    walpha0(0.0),
    walpha1(0.0),
    wat(0.0),
    wb0(0.0),
    wb1(0.0),
    wbeta0(0.0),
    wbgidl(-9.9999e-99),
    wbgisl(-9.9999e-99),
    wbigbacc(-9.9999e-99),
    wbigbinv(-9.9999e-99),
    wbigc(-9.9999e-99),
    wbigd(-9.9999e-99),
    wbigs(-9.9999e-99),
    wbigsd(-9.9999e-99),
    wcdsc(0.0),
    wcdscb(0.0),
    wcdscd(0.0),
    wcf(0.0),
    wcgdl(0.0),
    wcgidl(-9.9999e-99),
    wcgisl(-9.9999e-99),
    wcgsl(0.0),
    wcigbacc(-9.9999e-99),
    wcigbinv(-9.9999e-99),
    wcigc(-9.9999e-99),
    wcigd(-9.9999e-99),
    wcigs(-9.9999e-99),
    wcigsd(-9.9999e-99),
    wcit(0.0),
    wckappa(0.0),
    wclc(0.0),
    wcle(0.0),
    wd(0.0),
    wdelta(0.0),
    wdrout(0.0),
    wdsub(0.0),
    wdvt0(0.0),
    wdvt0w(0.0),
    wdvt1(0.0),
    wdvt1w(0.0),
    wdvt2(0.0),
    wdvt2w(0.0),
    wdwb(0.0),
    wdwg(0.0),
    wegidl(-9.9999e-99),
    wegisl(-9.9999e-99),
    weigbinv(-9.9999e-99),
    welm(0.0),
    weta0(0.0),
    wetab(0.0),
    wfgidl(-9.9999e-99),
    wfgisl(-9.9999e-99),
    wgamma1(0.0),
    wgamma2(0.0),
    wint(0),
    wk1(0.0),
    wk2(0.0),
    wk3(0.0),
    wk3b(0.0),
    wketa(0.0),
    wkgidl(-9.9999e-99),
    wkgisl(-9.9999e-99),
    wkt1(0.0),
    wkt1l(0.0),
    wkt2(0.0),
    wku0(-9.9999e-99),
    wkvth0(-9.9999e-99),
    wl(0),
    wlc(0),
    wln(1),
    wlod(-9.9999e-99),
    wlodku0(-9.9999e-99),
    wlodvth(-9.9999e-99),
    wmax(1),
    wmin(0),
    wmlt(1.0),
    wmoin(0.0),
    wnch(0.0),
    wnfactor(0.0),
    wngate(0.0),
    wnigbacc(-9.9999e-99),
    wnigbinv(-9.9999e-99),
    wnigc(-9.9999e-99),
    wnlx(0.0),
    wnoff(0.0),
    wnoi(1e-5),
    wnpeak(0.0),
    wnsd(-9.9999e-99),
    wnsub(0.0),
    wntox(-9.9999e-99),
    wpclm(0.0),
    wpdiblc1(0.0),
    wpdiblc2(0.0),
    wpdiblcb(0.0),
    wpigcd(-9.9999e-99),
    wpoxedge(-9.9999e-99),
    wprt(0.0),
    wprwb(0.0),
    wprwg(0.0),
    wpscbe1(0.0),
    wpscbe2(0.0),
    wpvag(0.0),
    wr(1),
    wrd(0),
    wrdsw(0.0),
    wref(0.0),
    wrgidl(-9.9999e-99),
    wrgisl(-9.9999e-99),
    wrs(0),
    wtvfbsdoff(-9.9999e-99),
    wu0(0.0),
    wua(0.0),
    wua1(0.0),
    wub(0.0),
    wub1(0.0),
    wuc(0.0),
    wuc1(0.0),
    wute(0.0),
    wvbm(0.0),
    wvbx(0.0),
    wvfb(0.0),
    wvfbcv(0.0),
    wvfbsdoff(-9.9999e-99),
    wvoff(0.0),
    wvoffcv(0.0),
    wvsat(0.0),
    wvth0(0.0),
    ww(0),
    ww0(0.0),
    wwc(0),
    wwl(0),
    wwlc(0),
    wwn(1),
    wwr(0.0),
    wxj(0.0),
    wxt(0.0),
    xj(0.15e-6),
    xjbvd(1.0),
    xjbvs(1.0),
    xl(0),
    xlref(0.0),
    xpart(-9.9999e-99),
    xt(-9.9999e-99),
    xti(3),
    xw(0),
    xwref(0.0)
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
    .registerDevice("bsim3_va", 1)
    .registerModelType("bsim3_va", 1);
}
} // namespace COGENDAbsim3_va
} // namespace Device
} // namespace Xyce

struct Bootstrap {
 Bootstrap() {
  Xyce::Device::COGENDAbsim3_va::registerDevice(); }}; 
Bootstrap COGENDAbsim3_va_bootstrap;

