#ifndef vpi_user_VAMS_2_3_h
#define vpi_user_VAMS_2_3_h

/**
 * The Verilog/AMS 2.3 LRM does not provide a normative vpi_user.h file. 
 * So we use the one from the 1364-2005 spec and only define the missing
 * bits here:
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Use to export a symbol */
#if WIN32
#ifndef PLI_DLLISPEC
#define PLI_DLLISPEC __declspec(dllimport)
#define VPI_USER_DEFINED_DLLISPEC 1
#endif
#else
#ifndef PLI_DLLISPEC
#define PLI_DLLISPEC
#endif
#endif

/* Use to import a symbol */
#if WIN32
#ifndef PLI_DLLESPEC

#define PLI_DLLESPEC __declspec(dllexport)
#define VPI_USER_DEFINED_DLLESPEC 1
#endif
#else
#ifndef PLI_DLLESPEC
#define PLI_DLLESPEC
#endif
#endif

/* Use to mark a function as external */
#ifndef PLI_EXTERN
#define PLI_EXTERN
#endif

/* Use to mark a variable as external */
#ifndef PLI_VEXTERN
#define PLI_VEXTERN extern
#endif

#ifndef PLI_PROTOTYPES
#define PLI_PROTOTYPES
#define PROTO_PARAMS(params) params
/* object is defined imported by the application */
#define XXTERN PLI_EXTERN PLI_DLLISPEC
/* object is exported by the application */
#define EETERN PLI_EXTERN PLI_DLLESPEC
#endif


enum {
	/**
	 * Does the spec provides free ranges for new enums/defines? Since we
	 * do not know, we'll occupy 1000...1031 -- let's hope this is safe.
	 */
	vpiAnalogSysFunction = 1000,
	vpiAnalogSysTask,
	vpiAnalogTime,
	vpiBranch,
	vpiChild,
	vpiDerivative,
	vpiDomain,
	vpiEndFrequency,
	vpiEndTime,
	vpiEquationTarget,
	vpiExpStrVal,
	vpiFlow,
	vpiFlowNature,
	vpiHiConn,
	vpiLoConn,
	vpiNegNode,
	vpiPosNode,
	vpiPotential,
	vpiPotentialNature,
	vpiPureTransportDelayNo,
	vpiRejectTransientStep,
	vpiScaledTime,
	vpiScaledTme,
	vpiSource,
	vpiStartFrequency,
	vpiStartTime,
	vpiSysAnalogTask,
	vpiTransientFailConverge,
	vpiTransientMaxStep,

	/******** VAMS analog-related callback reasons ********/
	acbInitialStep,		/* Upon acceptance of the first analog solution */
	acbFinalStep,		/* Upon acceptance of the last analog solution */
	acbAbsTime,		/* Upon acceptance of the analog solution for the given time
				 * (this callback shall force a solution at that time)
				 */
	acbElapsedTime,		/* Upon acceptance of the solution advanced from the current 
				 * solution by the given interval
				 * (this callback shall force a solution at that time)
				 */
	acbConvergenceTest,	/* Prior acceptance of the analog solution for the given time
				 * (this callback allows rejection of the analog solution at
				 * that time and backup to an earlier time)
				 */
	acbAcceptedPoint,	/* Upon acceptance of the solution at the given time */
};


typedef struct t_vpi_stf_partials { 
	int count; 
	int *derivative_of;	/* 0 = returned value, 1 = 1st arg, etc. */ 
	int *derivative_wrt;	/* 1 = 1st arg, 2 = 2nd arg, etc. */ 
} s_vpi_stf_partials, *p_vpi_stf_partials; 

typedef struct t_vpi_analog_systf_data {
	int type;		/* vpiSys[Task,Function] */
	int sysfunctype;	/* vpi[IntFunc,RealFunc,TimeFunc,SizedFunc] */
	char *tfname;		/* first character shall be "$" */
	int (*calltf) (void);
	int (*compiletf) (void);
	int (*sizetf) (void);	/* for vpiSizedFunc system functions only */
	p_vpi_stf_partials (*derivtf) (void); /* for partial derivatives */
	char *user_data;
} s_vpi_analog_systf_data, *p_vpi_analog_systf_data;


typedef struct t_vpi_analog_value {
	int format;	/* vpiRealVal,vpiExpStrVal,vpiDecStrVal,vpiStringVal */
	union {
		char *str;
		double real;
		char *misc;
	} real;
	union {
		char *str;
		double real;
		char *misc;
	} imaginary;
} s_vpi_analog_value, *p_vpi_analog_value;


XXTERN double vpi_get_analog_delta PROTO_PARAMS((void));

XXTERN double vpi_get_analog_freq PROTO_PARAMS((void));

XXTERN void vpi_get_analog_systf_info PROTO_PARAMS((vpiHandle obj, p_vpi_analog_systf_data systf_data_p));

XXTERN double vpi_get_analog_time PROTO_PARAMS((void));

XXTERN void vpi_get_analog_value PROTO_PARAMS((vpiHandle obj, p_vpi_analog_value value_p));

XXTERN double vpi_get_real PROTO_PARAMS((int prop, vpiHandle obj));

XXTERN vpiHandle vpi_register_analog_systf PROTO_PARAMS((p_vpi_analog_systf_data systf_analog_data_p));


#undef PLI_EXTERN
#undef PLI_VEXTERN

#ifdef VPI_USER_DEFINED_DLLISPEC
#undef VPI_USER_DEFINED_DLLISPEC
#undef PLI_DLLISPEC
#endif
#ifdef VPI_USER_DEFINED_DLLESPEC
#undef VPI_USER_DEFINED_DLLESPEC
#undef PLI_DLLESPEC
#endif

#ifdef PLI_PROTOTYPES
#undef PLI_PROTOTYPES
#undef PROTO_PARAMS
#undef XXTERN
#undef EETERN
#endif

#ifdef __cplusplus
}
#endif

#endif

