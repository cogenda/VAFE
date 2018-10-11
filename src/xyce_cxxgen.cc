/*
 * Verilog-A compiling to C++ codes in Xyce format
 * whose input is from xyce_vacomp elaborator
 */
#include "xyce_cxxgen.h"


void CgenIncludeFiles()
{
std::cout << "//-------------------------------------------------------------------------" <<std::endl;
std::cout << "#include <Xyce_config.h>" <<std::endl;
std::cout << std::endl;
std::cout <<"// ---------- Standard Includes ----------" <<std::endl;
std::cout << std::endl;
std::cout <<"// ----------   Xyce Includes   ----------" <<std::endl;
std::cout <<"#include <N_DEV_Const.h>" <<std::endl;
std::cout <<"#include <N_DEV_DeviceOptions.h>" <<std::endl;
std::cout <<"#include <N_DEV_ExternData.h>" <<std::endl;
std::cout <<"#include <N_DEV_MOSFET1.h>" <<std::endl;
std::cout <<"#include <N_DEV_MOSFET_B4.h>" <<std::endl;
std::cout <<"#include <N_DEV_MatrixLoadData.h>" <<std::endl;
std::cout <<"#include <N_DEV_Message.h>" <<std::endl;
std::cout <<"#include <N_DEV_SolverState.h>" <<std::endl;
std::cout <<"#include <N_ERH_ErrorMgr.h>" <<std::endl;
std::cout <<"#include <N_LAS_Matrix.h>" <<std::endl;
std::cout <<"#include <N_LAS_Vector.h>" <<std::endl;
std::cout <<"#include <N_UTL_FeatureTest.h>" <<std::endl;
std::cout <<"#include <N_UTL_MachDepParams.h>" <<std::endl;
std::cout <<"#include <N_UTL_Math.h>" <<std::endl;
std::cout <<"// ---------- Macros Definitions ----------" <<std::endl;
std::cout <<"#define KOVERQ        8.61734e-05" <<std::endl;
std::cout <<"#define ELEM          1.0e+20" <<std::endl;
std::cout <<"#define _VT_ ((ckt->temperature) * KOVERQ)" <<std::endl;
std::cout <<"#define _TEMPER_ (ckt->temperature)" <<std::endl;
std::cout <<"#define _LIMEXP_(x) ((x)<log(ELIM)? exp(x) : (ELIM*(x) + ELIM - ELIM*log(ELIM)))" <<std::endl;

}

void
CgenHeader (vaElement& vaModuleEntries)
{
  std::cout << "This is a C++ header file !" << std::endl;
  CgenIncludeFiles();
}


void
CgenImplement (vaElement& vaModuleEntries)
{
  std::cout << "This is a C++ Main file of Implementation!" << std::endl;
}
