#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <math.h>

#include "xvpi.h"


vpiHandle vpi_register_cb (p_cb_data cb_data_p)
{
	struct xvpi_object *cb = xvpi_object_new(vpiCallback, vpiCallback);
	struct t_cb_data *cb_data = malloc(sizeof(*cb_data));
	if (cb_data == NULL) {
		fatal("vpi_register_cb(): failed to allocate memory for internal storage!\n");
		return NULL;
	}
	memcpy(cb_data, cb_data_p, sizeof(*cb_data));
	xvpi_object_add_child(cb, xvpi_object_new(vpiType, xvpiStrProp, "vpiCallback", -1));
	xvpi_object_add_child(cb, xvpi_object_new(xvpiCbData, xvpiPointerProp, cb_data, free));
	xvpi_object_add_child(xvpi_object_get_child_or_create((struct xvpi_object *) cb_data->obj, vpiCallback, vpiIterator), cb);
	return (vpiHandle) cb;
}


PLI_INT32 vpi_remove_cb (vpiHandle cb_obj)
{
	struct xvpi_object *cb_iterator = xvpi_object_get_child(NULL, vpiCallback, vpiIterator);
	if (cb_iterator != NULL) {
		warn("trying to remove unknown callback: no callbacks are registered yet!\n");
		return 0;
	}
	return xvpi_object_remove_child(cb_iterator, (struct xvpi_object *) cb_obj);
}


void vpi_get_cb_info (vpiHandle object, p_cb_data cb_data_p)
{
	void *p = xvpi_get_pointer(xvpiCbData, object);
	if (p != NULL)
		memcpy(cb_data_p, p, sizeof(*cb_data_p));
	else
		warn("vpi_get_cb_info() called with invalid object handle!\n");
}


vpiHandle vpi_register_systf (p_vpi_systf_data systf_data_p)
{
	struct xvpi_object *systf = xvpi_object_new(vpiUserSystf, vpiUserSystf);
	struct t_vpi_systf_data *systf_data = malloc(sizeof(*systf_data));
	if (systf_data == NULL) {
		fatal("vpi_register_systf(): failed to allocate memory for internal storage!\n");
		return NULL;
	}
	memcpy(systf_data, systf_data_p, sizeof(*systf_data));
	xvpi_object_add_child(systf, xvpi_object_new(vpiType, xvpiStrProp, "vpiUserSystf", -1));
	xvpi_object_add_child(systf, xvpi_object_new(vpiName, xvpiStrProp, systf_data->tfname, -1));
	xvpi_object_add_child(systf, xvpi_object_new(vpiUserDefn, xvpiIntProp, 1));
	xvpi_object_add_child(systf, xvpi_object_new(xvpiSysTfData, xvpiPointerProp, systf_data, free));
	xvpi_object_add_child(xvpi_object_get_child_or_create(NULL, vpiUserSystf, vpiIterator), systf);
	return (vpiHandle) systf;
}


void vpi_get_systf_info (vpiHandle object, p_vpi_systf_data systf_data_p)
{
	void *p = xvpi_get_pointer(xvpiSysTfData, object);
	if (p != NULL)
		memcpy(systf_data_p, p, sizeof(*systf_data_p));
	else
		warn("vpi_get_systf_info() called with invalid object handle!\n");
}


/* for obtaining handles */
vpiHandle vpi_handle_by_name (PLI_BYTE8 * name, vpiHandle scope)
{
	struct xvpi_object *obj = (scope == NULL) ? &xvpi.root_object : (struct xvpi_object *) scope;
	int len;
	int i;

	if (name == NULL) {
		warn("vpi_handle_by_name() called with NULL name!\n");
		return NULL;
	}

	if (name[0] == '\0') {
		xvpi_object_ref(obj);
		return (vpiHandle) obj;
	}

	len = 0;
	while (name[len] != '\0' && name[len] != '.')
		len++;

	for (i=0; i<obj->u.object.n_childs; i++) {
		struct xvpi_object *child = obj->u.object.childs[i];
		struct xvpi_object *p;
		if (child->type == vpiIterator) {
			vpiHandle h = vpi_handle_by_name(name, (vpiHandle) child);
			if (h != NULL)
				return h;
		} else {
			p = xvpi_object_get_child(child, vpiName, xvpiStrProp);
			if (p != NULL && strlen(p->u.string) == len) {
				if (strncmp((char *) name, p->u.string, len) == 0) {
					name += len;
					if (name[0] == '.')
						name++;
					return vpi_handle_by_name(name, (vpiHandle) child);
				}
			}
		}
	}

	return NULL;
}


vpiHandle vpi_handle_by_index (vpiHandle object, PLI_INT32 indx)
{
	nyi(__FUNCTION__);
	return 0;
}


/* for traversing relationships */
vpiHandle vpi_handle (PLI_INT32 name, vpiHandle refHandle)
{
	struct xvpi_object *child = xvpi_object_get_child((struct xvpi_object *) refHandle, name, vpiUndefined);

	if (child) {
		/* iterator objects are not accessible using vpi_handle() */
		if (child->type == vpiIterator)
			return NULL;
		if (xvpi_object_has_childs(child)) {
			xvpi_object_ref(child);
			return (vpiHandle) child;
		}
		if (child->type == xvpiPointerProp)
			return (vpiHandle) child->u.pointer.ptr;
	}

	return NULL;
}


vpiHandle vpi_handle_multi (PLI_INT32 type, vpiHandle refHandle1, vpiHandle refHandle2, ...)
{
	nyi(__FUNCTION__);
	return 0;
}


vpiHandle vpi_iterate (PLI_INT32 name, vpiHandle refHandle)
{
	struct xvpi_object *obj = (refHandle == NULL) ? &xvpi.root_object : (struct xvpi_object *) refHandle;
	struct xvpi_object *child = xvpi_object_get_child(obj, name, vpiIterator);
	struct xvpi_object *i = NULL;
	int k;

	if (child) {
		if (child->u.object.n_childs > 0) {
			i = xvpi_object_new(0, vpiIterator);
			xvpi_object_ref(i);
			xvpi_object_ref(obj);
			xvpi_object_add_child(i, xvpi_object_new(vpiUse, xvpiPointerProp, obj, xvpi_object_unref));
			xvpi_object_add_child(i, xvpi_object_new(vpiIteratorType, xvpiIntProp, name));
			for (k=child->u.object.n_childs-1; k>=0; k--)
				xvpi_object_add_child(i, child->u.object.childs[k]);
		}
	}

	return (vpiHandle) i;
}


vpiHandle vpi_scan (vpiHandle iterator)
{
	struct xvpi_object *i = (struct xvpi_object *) iterator;
	struct xvpi_object *o;

	o = i->u.object.childs[i->u.object.n_childs-1];

	if (o->name == vpiIteratorType || o->name == vpiUse) {
		xvpi_object_unref(i);
		return NULL;
	}

	i->u.object.n_childs--;
	xvpi_object_unref(o);
	return (vpiHandle) o;
}


/* for processing properties */
PLI_INT32 vpi_get (PLI_INT32 property, vpiHandle object)
{
	struct xvpi_object *obj = (struct xvpi_object *) object;
	struct xvpi_object *p;
	PLI_INT32 val = vpiUndefined;

	if (obj) {
		if (property == vpiType) {
			val = obj->type;
		} else if (property == vpiSize && obj->type == vpiIterator) {
			val = obj->u.object.n_childs - 2;
		} else {
			if ((p = xvpi_object_get_child(obj, property, xvpiIntProp)) != NULL)
				val = p->u.integer;
			if ((p = xvpi_object_get_child(obj, property, xvpiBoolProp)) != NULL)
				val = p->u.boolean;
		}
	}

	return val;
}

static PLI_BYTE8 *vpi_get_str_buf = NULL;

static void vpi_get_str_buf_free () __attribute__((destructor));
static void vpi_get_str_buf_free () { free(vpi_get_str_buf); }

PLI_BYTE8 *vpi_get_str (PLI_INT32 property, vpiHandle object)
{
	struct xvpi_object *p;

	if (vpi_get_str_buf != NULL) {
		free(vpi_get_str_buf);
		vpi_get_str_buf = NULL;
	}

	if ((p = xvpi_object_get_child((struct xvpi_object *) object, property, xvpiStrProp)) != NULL)
		vpi_get_str_buf = strdup(p->u.string);

	return vpi_get_str_buf;
}


/* delay processing */
void vpi_get_delays (vpiHandle object, p_vpi_delay delay_p)
{
	nyi(__FUNCTION__);
}


void vpi_put_delays (vpiHandle object, p_vpi_delay delay_p)
{
	nyi(__FUNCTION__);
}


/* value processing */
void vpi_get_value (vpiHandle expr, p_vpi_value value_p)
{
	nyi(__FUNCTION__);
}


vpiHandle vpi_put_value (vpiHandle object, p_vpi_value value_p, p_vpi_time time_p, PLI_INT32 flags)
{
	nyi(__FUNCTION__);
	return NULL;
}


/* time processing */
void vpi_get_time (vpiHandle object, p_vpi_time time_p)
{
	if (time_p == NULL) {
		warn("vpi_get_time() called with time_p == NULL pointer!\n");
		return;
	}

	if (time_p->type == vpiSimTime) {
		time_p->high = (xvpi.sim.time >> 32) & 0xffffffff;
		time_p->low = xvpi.sim.time & 0xffffffff;
		time_p->real = (double) xvpi.sim.time;
	} else if (time_p->type == vpiScaledRealTime) {
		PLI_INT32 time_unit = vpi_get(vpiTimeUnit, object);
		time_p->real = (double) xvpi.sim.time * pow(10.0, (double) time_unit);
	} else {
		warn("vpi_get_time() called with invalid time_p->type == %d\n", time_p->type);
	}
}


/* I/O	routines */
PLI_UINT32 vpi_mcd_open (PLI_BYTE8 * fileName)
{
	unsigned i;

	for (i=1; i<31; i++) {
		if (strcmp(xvpi.mcd_list[i].name, fileName) == 0) {
			xvpi.mcd_list[i].open_count++;
			return (1 << i);
		}
	}

	for (i=1; i<31; i++) {
		if (xvpi.mcd_list[i].file == NULL) {
			if ((xvpi.mcd_list[i].file = fopen(fileName, "w")) == NULL) {
				err("failed opening mcd file '%s' for writing: %s\n", strerror(errno));
				return 0;
			}
			xvpi.mcd_list[i].name = strdup(fileName);
			xvpi.mcd_list[i].open_count = 1;
			return (1 << i);
		}
	}
	return 0;
}


PLI_UINT32 vpi_mcd_close (PLI_UINT32 mcd)
{
	PLI_UINT32 retval = 0;
	unsigned i;

	if (mcd & (1 << 31)) {
		nyi("vpi_mcd_close() on file descriptors opened with $fopen() system call.\n");
		return 0;
	}

	for (i=1; i<31; i++) {
		if (mcd & (1 << i)) {
			if (--xvpi.mcd_list[i].open_count == 0) {
				free(xvpi.mcd_list[i].name);
				if (fclose(xvpi.mcd_list[i].file) != 0) {
					retval |= i;
					err("error while closing mcd 0x%08x: %s\n", 1 << i, strerror(errno));
				}
				xvpi.mcd_list[i].file = NULL;
				xvpi.mcd_list[i].name = NULL;
			}
		}
	}

	return retval;
}


PLI_BYTE8 *vpi_mcd_name (PLI_UINT32 cd)
{
	int i;

	if (cd & (1 << 31)) {
		nyi("vpi_mcd_close() on file descriptors opened with $fopen() system call.\n");
		return 0;
	}

	for (i=0; i<31; i++) {
		if (cd & (1 << i)) {
			if (cd & ~(1 << i))
				err("vpi_mcd_name() called with multiple channel descriptors!\n");
			return xvpi.mcd_list[i].name;
		}
	}

	return NULL;
}


PLI_INT32 vpi_mcd_printf (PLI_UINT32 mcd, PLI_BYTE8 * format, ...)
{
	PLI_UINT32 retval;
	va_list ap;
	va_start(ap, format);
	retval = vpi_mcd_vprintf(mcd, format, ap);
	va_end(ap);
	return retval;
}


PLI_INT32 vpi_printf (PLI_BYTE8 * format, ...)
{
	PLI_INT32 retval;
	va_list ap;
	va_start(ap, format);
	retval = vpi_mcd_vprintf(VPI_MCD_STDOUT, format, ap);
	va_end(ap);
	return retval;
}


/* utility routines */
PLI_INT32 vpi_compare_objects (vpiHandle object1, vpiHandle object2)
{
	return (object1 == object2) ? 1 : 0;
}


PLI_INT32 vpi_chk_error (p_vpi_error_info error_info_p)
{
	PLI_INT32 retval = xvpi.error_info.level;
	if (error_info_p != NULL)
		memcpy(error_info_p, &xvpi.error_info, sizeof(*error_info_p));
	memset(&xvpi.error_info, 0, sizeof(xvpi.error_info));
	return retval;
}


PLI_INT32 vpi_free_object (vpiHandle object)
{
	xvpi_object_unref((struct xvpi_object *) object);
	return 1;
}


PLI_INT32 vpi_get_vlog_info (p_vpi_vlog_info vlog_info_p)
{
	if (vlog_info_p == NULL) {
		warn("vpi_get_vlog_info() called with NULL pointer arg!\n");
		return 0;
	}
	memcpy(vlog_info_p, &xvpi.vlog_info, sizeof(*vlog_info_p));
	return 1;
}


/* routines added with 1364-2001 */
PLI_INT32 vpi_get_data (PLI_INT32 id, PLI_BYTE8 * dataLoc, PLI_INT32 numOfBytes)
{
	nyi(__FUNCTION__);
	return 0;
}


PLI_INT32 vpi_put_data (PLI_INT32 id, PLI_BYTE8 * dataLoc, PLI_INT32 numOfBytes)
{
	nyi(__FUNCTION__);
	return 0;
}


void * vpi_get_userdata (vpiHandle obj)
{
	struct t_vpi_systf_data *systf_data = xvpi_get_pointer(xvpiValue, obj);
	if (systf_data == NULL) {
		warn("vpi_get_userdata() called with NULL object handle!\n");
		return NULL;
	}
	return systf_data->user_data;
}


PLI_INT32 vpi_put_userdata (vpiHandle obj, void *userdata)
{
	struct xvpi_object *tfcall_obj = (struct xvpi_object *) obj;
	struct xvpi_object *ptr_obj;
	if (tfcall_obj == NULL || tfcall_obj->type != vpiSysTfCall) {
		warn("vpi_put_userdata() called with invalid object handle!\n");
		return 0;
	}
	if ((ptr_obj = xvpi_object_get_child(tfcall_obj, xvpiValue, xvpiPointerProp)) == NULL)
		return xvpi_object_add_child(tfcall_obj, xvpi_object_new(xvpiValue, xvpiPointerProp, userdata, NULL));
	else
		ptr_obj->u.pointer.ptr = userdata;
	return 1;
}


PLI_INT32 vpi_vprintf (PLI_BYTE8 * format, va_list ap)
{
	return vpi_mcd_vprintf(VPI_MCD_STDOUT, format, ap);
}


PLI_INT32 vpi_mcd_vprintf (PLI_UINT32 mcd, PLI_BYTE8 * format, va_list ap)
{
	PLI_UINT32 retval = 0;
	unsigned i;

	xvpi.mcd_list[0].file = stdout;

	if (mcd & (1 << 31)) {
		nyi("vpi_mcd_vprintf() on file descriptors opened with $fopen() system call.\n");
		return 0;
	}

	for (i=0; i<31; i++) {
		if (mcd & (1 << i)) {
			if (vfprintf(xvpi.mcd_list[i].file, format, ap) < 0) {
				if (i > 0)
					err("vpi_mcd_vfprintf() on mcd 0x%08x failed: %s\n", strerror(errno));
				retval = (1 << i);
			}
			mcd &= ~(1 << i);
			if (mcd == 0)
				break;
		}
	}

	return retval;
}


PLI_INT32 vpi_flush (void)
{
	return vpi_mcd_flush(VPI_MCD_STDOUT);
}


PLI_INT32 vpi_mcd_flush (PLI_UINT32 mcd)
{
	PLI_UINT32 retval = 0;
	unsigned i;

	xvpi.mcd_list[0].file = stdout;

	if (mcd & (1 << 31)) {
		nyi("vpi_mcd_flush() on file descriptors opened with $fopen() system call.\n");
		return 0;
	}

	for (i=0; i<31; i++) {
		if (mcd & (1 << i)) {
			if (fflush(xvpi.mcd_list[i].file) != 0) {
				err("vpi_mcd_flush on mcd 0x%08x failed: %s\n", 1 << i, strerror(errno));
				retval |= (1 << i);
			}
			mcd &= ~(1 << i);
			if (mcd == 0)
				break;
		}
	}

	return retval;
}


PLI_INT32 vpi_control (PLI_INT32 operation, ...)
{
	nyi(__FUNCTION__);
	return 0;
}


vpiHandle vpi_handle_by_multi_index (vpiHandle obj, PLI_INT32 num_index, PLI_INT32 * index_array)
{
	nyi(__FUNCTION__);
	return 0;
}


/* routines added by Verilog/AMS */
double vpi_get_analog_delta (void)
{
	return xvpi.sim.analog_delta;
}


double vpi_get_analog_freq (void)
{
	return xvpi.sim.analog_freq;
}


void vpi_get_analog_systf_info (vpiHandle obj, p_vpi_analog_systf_data systf_data_p)
{
	void *p = xvpi_get_pointer(xvpiValue, obj);
	if (p != NULL)
		memcpy(systf_data_p, p, sizeof(*systf_data_p));
	else
		warn("vpi_get_analg_systf_info() called with NULL object handle!\n");
}


double vpi_get_analog_time (void)
{
	return xvpi.sim.analog_time;
}


void vpi_get_analog_value (vpiHandle obj, p_vpi_analog_value value_p)
{
	nyi(__FUNCTION__);
}


double vpi_get_real (int prop, vpiHandle obj)
{
	struct xvpi_object *p = xvpi_object_get_child((struct xvpi_object *) obj, prop, xvpiRealProp);
	double val = (double) vpiUndefined;
	if (p)
		val = p->u.real;
	return val;
}


vpiHandle vpi_register_analog_systf (p_vpi_analog_systf_data systf_analog_data_p)
{
	struct xvpi_object *systf = xvpi_object_new(vpiAnalogUserSystf, vpiAnalogUserSystf);
	p_vpi_analog_systf_data p = malloc(sizeof(*p));
	if (p == NULL) {
		fatal("vpi_register_analog_systf(): failed to allocate memory for internal storage!\n");
		return NULL;
	}
	memcpy(p, systf_analog_data_p, sizeof(*p));
	xvpi_object_add_child(systf, xvpi_object_new(vpiType, xvpiStrProp, "vpiAnalogUserSystf", -1));
	xvpi_object_add_child(systf, xvpi_object_new(vpiName, xvpiStrProp, p->tfname, -1));
	xvpi_object_add_child(systf, xvpi_object_new(vpiUserDefn, xvpiIntProp, 1));
	xvpi_object_add_child(systf, xvpi_object_new(xvpiValue, xvpiPointerProp, p, free));
	xvpi_object_add_child(xvpi_object_get_child_or_create(NULL, vpiAnalogUserSystf, vpiIterator), systf);
	return (vpiHandle) systf;
}


