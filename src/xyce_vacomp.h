#ifndef __xyce_vacomp_h
#define __xyce_vacomp_h
#include <vpi_user.h>
#if defined(__XVPI__)
  #include "xvpi.h"
#endif

#ifdef __cplusplus
extern "C" {
void CgenHeader(vpiHandle root);
void CgenImplement(vpiHandle root);
int vpi_gen_ccode (vpiHandle obj);
}
#else
void CgenHeader( vpiHandle root);
void CgenImplement( vpiHandle root);
int vpi_gen_ccode (vpiHandle obj);
#endif

#endif
