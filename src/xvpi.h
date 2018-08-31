#ifndef __xvpi_h
#define __xvpi_h

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include "vpi_user.h"


/******* VPI API extensions ***********/

/* basic VPI object types that are accidentally missing in VAMS 2.3 LRM */
#define vpiAnalogBuiltinFuncCall	3001
#define vpiAnalogFuncCall		3002
#define vpiAnalogFunction		3003
#define vpiAnalogFilterFuncCall		3004
#define vpiAnalogSmallSignalFuncCall	3005
#define vpiAnalogSysTaskCall		3006
#define vpiAnalogSysFuncCall		3007
#define vpiAnalogUserSystf		3008
#define vpiAnalysisFuncCall		3009
#define vpiBranchProbeFuncCall		3010
#define vpiContrib			3011
#define vpiDiscipline			3012
#define vpiDomain			3013
#define vpiContinuous			3014
#define vpiDiscrete			3015
#define vpiNature			3016
#define vpiNode				3017
#define vpiPortProbeFuncCall		3018
#define vpiValueRange			3019

/* vpiValueRange properties */
#define vpiValueRangeType		3020

/* vpiContrib properties */
#define vpiDirect			3101

/* new vpiNetType properties */
#define vpiGround			3201
#define vpiUwire			3202
#define vpiWreal			3203

/* new values for vpiOpType */
#define vpiAboveOp			3301
#define vpiCrossOp			3302
#define vpiFinalStepOp			3303
#define vpiInitialStepOp		3304
#define vpiTimerOp			3305

/* new values for vpiConstType */
#define vpiRealtimeConst		3401
#define vpiTimeConst			3402

/* vpiType of realtime variables */
#define vpiRealtimeVar			3501

/* values for vpiValueRangeType */
#define vpiFrom				3601
#define vpiExclude			3602

/* new values for vpiUnconnDrive */
/* #define vpiHighZ			1  -- defined as vpiUnconnDrive value in 1364-2005 */
/* #define vpiPull1			2  -- defined as vpiUnconnDrive value in 1364-2005 */
/* #define vpiPull0			3  -- defined as vpiUnconnDrive value in 1364-2005 */
/* #define vpiSupply1			10  -- defined as vpiNetType value in 1364-2005 */
/* #define vpiSupply0			11  -- defined as vpiNetType value in 1364-2005 */
#define vpiStrong1			3703
#define vpiStrong0			3704
#define vpiWeak1			3705
#define vpiWeak0			3706

/******* Private VPI API extensions, internally used. Do we want to expose them?  ***********/

/* we're allocating a private enum range starting at 4000. Watch out for conflicts in vpi_user.h and friends */
#define xvpiValue			4000

/* object properties */
#define XVPI_PROPERTY(x)		((x) | 0x8000)
#define IS_XVPI_PROPERTY(x)		((x) & 0x8000)

#define xvpiBoolProp			XVPI_PROPERTY(1)
#define xvpiIntProp			XVPI_PROPERTY(2)
#define xvpiRealProp			XVPI_PROPERTY(3)
#define xvpiStrProp			XVPI_PROPERTY(4)
#define xvpiPointerProp			XVPI_PROPERTY(6)	/* Internal pointer */

#define xvpiDimension			4001		/* Dimension of object arrays before elaboration. */
#define xvpiReference			4002		/* Reference to yet unresolved object defined by name. */
#define xvpiApplication			4003
#define xvpiApplicationPointer		4004
#define xvpiCbData			4005
#define xvpiSysTfData			4006


struct xvpi_object {
	PLI_INT32 type;
	PLI_INT32 name;
	int refcount;
	union {
		struct {
			int n_childs;
			struct xvpi_object **childs;
		} object;
		struct {
			void *ptr;
			void (* free) (void *);
		} pointer;
		PLI_BYTE8 *string;
		PLI_INT32 boolean;
		PLI_INT32 integer;
		double real;
	} u;
};


struct xvpi_sim_state {
	struct t_vpi_vlog_info vlog_info;
	struct t_vpi_error_info error_info;
	PLI_BYTE8 message_buf [8192];
	PLI_INT32 warning_level;
	PLI_INT32 abort_error_level;
	PLI_INT32 fatal_error_level;
	struct xvpi_object root_object;
	struct {
		int open_count;
		FILE *file;
		char *name;
	} mcd_list [31];
	struct {
		uint64_t time;
		double analog_delta;
		double analog_freq;
		double analog_time;
	} sim;
	pthread_key_t thread_local_systf_key;
};

extern struct xvpi_sim_state xvpi;

extern int xvpi_init (int argc, char **argv);

extern void xvpi_shutdown (void);

extern PLI_INT32 xvpi_error (PLI_INT32 state, PLI_INT32 level, PLI_BYTE8 *code, PLI_BYTE8 *file, PLI_INT32 line, char *message_fmt, ...);

extern vpiHandle xvpi_application_load (const char *module_name);

extern void xvpi_trigger_callbacks (vpiHandle trigger_obj, PLI_INT32 reason);

extern struct xvpi_object * xvpi_object_new (PLI_INT32 name, PLI_INT32 type, ...);

extern void xvpi_object_ref (struct xvpi_object * obj);

extern void xvpi_object_unref (struct xvpi_object * obj);

extern PLI_INT32 xvpi_object_add_child (struct xvpi_object *obj, struct xvpi_object *child_obj);

extern PLI_INT32 xvpi_object_remove_child (struct xvpi_object *obj, struct xvpi_object *child_obj);

extern int xvpi_object_has_childs (struct xvpi_object *o);

extern struct xvpi_object * xvpi_object_get_child (struct xvpi_object *obj, PLI_INT32 name, PLI_INT32 type);

extern struct xvpi_object * xvpi_object_get_child_or_create (struct xvpi_object *obj, PLI_INT32 name, PLI_INT32 type);

extern void * xvpi_get_pointer (int prop, vpiHandle obj);


/******** misc helper functions and macros **********/

#ifndef XVPI_ERROR_STATE
#define XVPI_ERROR_STATE vpiPLI
#endif

#define msg(x...)	(void) xvpi_error(XVPI_ERROR_STATE, vpiNotice, NULL, NULL, 0, x)
#define warn(x...)	(void) xvpi_error(XVPI_ERROR_STATE, vpiWarning, NULL, NULL, 0, x)
#define err(x...)	(void) xvpi_error(XVPI_ERROR_STATE, vpiError, NULL, NULL, 0, x)
#define fatal(x...)	(void) xvpi_error(XVPI_ERROR_STATE, vpiInternal, NULL, NULL, 0, x)
#define nyi(x)		fatal("Not yet implemented: %s", x)


/**** fake implementations for BSD- and GNU-extensions not part of the ANSI/ISO C standard ****/
#if !defined(_GNU_SOURCE)
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>

	static inline char * __fake_strndup (const char *s, size_t n)
	{
		char *buf;
		unsigned len;
		for (len=0; len<n && s[len]!='\0'; len++)
			;
		buf = (char *) malloc(len+1);
		memcpy(buf, s, len);
		buf[len] = '\0';
		return buf;
	}

	#undef strndup
	#define strndup(s,n) __fake_strndup(s,n)
	#undef strdup
	#define strdup(s) strndup(s, -1)
	#if 0  /* not required right now */
	static inline int asprintf (char **strp, const char *fmt, ...)
	{
		size_t size = 256;
		char *p = NULL;
		while (1) {
			va_list ap;
			int n;
			if ((p = realloc (p, size)) == NULL)
				return -1;
			va_start(ap, fmt);
			n = vsnprintf(p, size, fmt, ap);
			va_end(ap);
			if (n > size) {
				size = n + 1;	/* libc reports length of string: take it. */
			} else if (n == -1) {
				size *= 2;	/* libc reports error: try twice the old size. */
			} else {
				if ((*strp = realloc(p, n)) == NULL)
					return -1;
				return n;
			}
		}
	}
	#endif
#endif

#endif

