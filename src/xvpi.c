#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <math.h>

#include "xvpi.h"


struct xvpi_sim_state xvpi = {
	.warning_level = vpiNotice,
	.fatal_error_level = vpiError,
	.abort_error_level = vpiInternal,
	.root_object = { .refcount = 1 },
	.message_buf = { 0 },
	.error_info = { 0 },
	.mcd_list = { { .name = "stdout" }, { 0 } },
	.vlog_info = { 0 },
	.sim = { 0 }
};

int xvpi_init (int argc, char **argv)
{
	xvpi.vlog_info.argc = argc;
	xvpi.vlog_info.argv = argv;
	xvpi.vlog_info.product = "Verilog/AMS 2.3 with changes proposed for 2.3.1 in www.verilog.org/mantis issue tracker";
	if (pthread_key_create(&xvpi.thread_local_systf_key, NULL) != 0)
		return -1;
	return 0;
}

void xvpi_shutdown (void)
{
	/* free VPI root object and all childs */
	vpi_free_object(NULL);
	pthread_key_delete(xvpi.thread_local_systf_key);
}

PLI_INT32 xvpi_error (PLI_INT32 state, PLI_INT32 level, PLI_BYTE8 *code, PLI_BYTE8 *file, PLI_INT32 line, char *message_fmt, ...)
{
	static const char *level_string [] = { "notice", "warning", "error", "system", "internal" };
	va_list ap;
	int len;

	xvpi.error_info.state = state;
	xvpi.error_info.level = level;
	xvpi.error_info.code = code;
	xvpi.error_info.file = file;
	xvpi.error_info.line = line;
	xvpi.error_info.message = xvpi.message_buf;
	xvpi.error_info.product = xvpi.vlog_info.product;

	if (level >= xvpi.warning_level) {
		if (level > vpiNotice)
			fflush(stdout);
		if (file)
			fprintf(stderr, "%s:%d: ", file, line);
		if (level >= vpiNotice && level <= vpiInternal)
			fprintf(stderr, "%s: ", level_string[level-vpiNotice]);
		va_start(ap, message_fmt);
		vfprintf(stderr, message_fmt, ap);
		va_end(ap);
		if (level > vpiNotice)
			fflush(stderr);
	}

	va_start(ap, message_fmt);
	len = vsnprintf(xvpi.message_buf, sizeof(xvpi.message_buf), message_fmt, ap);
	va_end(ap);

	if (len == sizeof(xvpi.message_buf)) {
		fprintf(stderr, "Fatal internal error: Error message buffer overflow!\n");
		fflush(stderr);
		level = vpiInternal;
	}

	/* XXX FIXME: trigger_obj (1st arg in xvpi_trigger_callbacks) shall report current scope or module */
	if (state == vpiPLI)
		xvpi_trigger_callbacks(NULL, cbPLIError);
	else
		xvpi_trigger_callbacks(NULL, cbError);

	if (level >= xvpi.abort_error_level)
		abort();

	if (level >= xvpi.fatal_error_level)
		exit(EXIT_FAILURE);

	return 1;
}

vpiHandle xvpi_application_load (const char *module_name)
{
	void *vpi_mod = dlopen(module_name, RTLD_LAZY);
	void (**startup_routines) (void);
	struct xvpi_object *app_obj;
	int i;

	if (!vpi_mod) {
		err("Error while loading VPI module '%s': %s\n", module_name, dlerror());
		return NULL;
	}

	startup_routines = (void (**) (void)) dlsym(vpi_mod, "vlog_startup_routines");

	if (startup_routines == NULL) {
		err("Error while loading VPI module '%s': %s\n", module_name, dlerror());
		dlclose(vpi_mod);
		return NULL;
	}

	if ((app_obj = xvpi_object_new(xvpiApplication, xvpiApplication, vpi_mod)) == NULL)
		return NULL;

	xvpi_object_add_child(app_obj, xvpi_object_new(vpiName, xvpiStrProp, module_name, -1));
	xvpi_object_add_child(app_obj, xvpi_object_new(xvpiApplicationPointer, xvpiPointerProp, vpi_mod, dlclose));
	xvpi_object_add_child(xvpi_object_get_child_or_create(NULL, xvpiApplication, vpiIterator), app_obj);

	for (i=0; startup_routines[i] != NULL; i++)
		startup_routines[i]();

	return (vpiHandle) app_obj;
}

void xvpi_trigger_callbacks (vpiHandle trigger_obj, PLI_INT32 reason)
{
	vpiHandle iterator = vpi_iterate(vpiCallback, trigger_obj);
	if (iterator) {
		vpiHandle callback;
		while ((callback = vpi_scan(iterator)) != NULL) {
			struct t_cb_data cb;
			memcpy(&cb, xvpi_get_pointer(xvpiCbData, callback), sizeof(cb));
			if (cb.reason == reason) {
				if (reason == cbUnresolvedSystf)
					cb.user_data = vpi_get_str(vpiName, trigger_obj);
				/* XXX FIXME: NYI: time, value and index fields in t_cb_data */
				cb.cb_rtn(&cb);
			}
		}
	}
}

static struct t_vpi_systf_data * xvpi_lookup_systf (const char *systf_name)
{
	vpiHandle iterator = vpi_iterate(vpiUserSystf, NULL);
        if (iterator) {
                vpiHandle systf;
                while ((systf = vpi_scan(iterator)) != NULL) {
               		if (strcmp(systf_name, vpi_get_str(vpiName, systf)) == 0)
				return xvpi_get_pointer(xvpiSysTfData, systf);
		}
        }
	return NULL;
}

static struct t_vpi_systf_data * xvpi_get_systf_data_and_setup_arguments (vpiHandle systf)
{
	struct t_vpi_systf_data *systf_data = xvpi_get_pointer(xvpiSysTfData, systf);
	if (!systf_data) {
		char *systf_name = vpi_get_str(vpiName, systf);
		if (!(systf_data = xvpi_lookup_systf(systf_name))) {
			xvpi_trigger_callbacks(systf, cbUnresolvedSystf);
			if (!(systf_data = xvpi_lookup_systf(systf_name)))
				fatal("xvpi_call_systf() unable to resolve SysTf '%s'!\n", systf_name);
		}
		xvpi_object_add_child((struct xvpi_object *) systf, xvpi_object_new(xvpiSysTfData, xvpiPointerProp, systf_data, NULL));
	}
	pthread_setspecific(xvpi.thread_local_systf_key, (void *) systf);
	return systf_data;
}

PLI_INT32 xvpi_systf_compiletf (vpiHandle systf)
{
	struct t_vpi_systf_data *systf_data = xvpi_get_systf_data_and_setup_arguments(systf);
	PLI_INT32 retval = systf_data->compiletf(systf_data->user_data);
	pthread_setspecific(xvpi.thread_local_systf_key, NULL);
	return retval;
}

PLI_INT32 xvpi_systf_sizetf (vpiHandle systf)
{
	struct t_vpi_systf_data *systf_data = xvpi_get_systf_data_and_setup_arguments(systf);
	PLI_INT32 retval = systf_data->sizetf(systf_data->user_data);
	pthread_setspecific(xvpi.thread_local_systf_key, NULL);
	return retval;
}

PLI_INT32 xvpi_systf_calltf (vpiHandle systf)
{
	struct t_vpi_systf_data *systf_data = xvpi_get_systf_data_and_setup_arguments(systf);
	PLI_INT32 retval = systf_data->calltf(systf_data->user_data);
	pthread_setspecific(xvpi.thread_local_systf_key, NULL);
	return retval;
}

int xvpi_object_has_childs (struct xvpi_object *o)
{
	if (o == NULL)
		return 0;
	switch (o->type) {
	case xvpiBoolProp:
	case xvpiIntProp:
	case xvpiRealProp:
	case xvpiStrProp:
	case xvpiPointerProp:
		return 0;
	}
	return 1;
}

struct xvpi_object * xvpi_object_new (PLI_INT32 name, PLI_INT32 type, ...)
{
	struct xvpi_object *obj;
	va_list ap;
	int len;
	char *s;

	if (!(obj = calloc(1, sizeof(*obj)))) {
		fatal("failed allocating %d bytes for xvpi object!\n", sizeof(*obj));
		return NULL;
	}

	obj->type = type;
	obj->name = name;

	switch (type) {
	case xvpiBoolProp:
		va_start(ap, type);
		obj->u.boolean = va_arg(ap, PLI_INT32) ? 1 : 0;
		va_end(ap);
		break;
	case xvpiIntProp:
		va_start(ap, type);
		obj->u.integer = va_arg(ap, PLI_INT32);
		va_end(ap);
		break;
	case xvpiRealProp:
		va_start(ap, type);
		obj->u.real = va_arg(ap, double);
		va_end(ap);
		break;
	case xvpiStrProp:
		va_start(ap, type);
		s = (char *) va_arg(ap, PLI_BYTE8 *);
		len = va_arg(ap, int);
		obj->u.string = (PLI_BYTE8 *) strndup(s, len);
		va_end(ap);
		break;
	case xvpiPointerProp:
		va_start(ap, type);
		obj->u.pointer.ptr = va_arg(ap, void *);
		obj->u.pointer.free = va_arg(ap, void *);
		va_end(ap);
		break;
	default:
		;
	}

	return obj;
}

static void xvpi_object_free (struct xvpi_object *obj)
{
	if (obj->type == xvpiStrProp) {
		if (obj->u.string != NULL)
			free(obj->u.string);
	} else if (obj->type == xvpiPointerProp) {
		if (obj->u.pointer.free != NULL && obj->u.pointer.ptr != NULL)
			obj->u.pointer.free(obj->u.pointer.ptr);
	}

	if (xvpi_object_has_childs(obj)) {
		int i;
		for (i=0; i<obj->u.object.n_childs; i++)
			xvpi_object_unref(obj->u.object.childs[i]);
		obj->u.object.n_childs = 0;
		if (obj->u.object.childs) {
			free(obj->u.object.childs);
			obj->u.object.childs = NULL;
		}
	}

	if (obj != &xvpi.root_object)
		free(obj);
}

void xvpi_object_ref (struct xvpi_object *obj)
{
	if (obj == NULL)
		obj = &xvpi.root_object; 
	obj->refcount++;
	//vpi_printf("ref: obj %p, type %d (0x%08x), refcount %d\n", obj, obj->type, obj->type, obj->refcount);
}

void xvpi_object_unref (struct xvpi_object *obj)
{
	if (obj == NULL)
		obj = &xvpi.root_object; 
	obj->refcount--;
	//vpi_printf("unref: obj %p, type %d (0x%08x), refcount %d\n", obj, obj->type, obj->type, obj->refcount);
	if (obj->refcount <= 0)
		xvpi_object_free(obj);
}

struct xvpi_object * xvpi_object_get_child (struct xvpi_object *obj, PLI_INT32 name, PLI_INT32 type)
{
	int i;

	if (obj == NULL) {
		if (name == vpiSysTfCall)
			return (struct xvpi_object *) pthread_getspecific(xvpi.thread_local_systf_key);
		obj = &xvpi.root_object; 
	}

	if (xvpi_object_has_childs(obj)) {
		for (i=0; i<obj->u.object.n_childs; i++) {
			if (obj->u.object.childs[i]->name == name) {
				if (obj->u.object.childs[i]->type == type || type == vpiUndefined)
					return obj->u.object.childs[i];
			}
		}
	}

	return NULL;
}

static PLI_INT32 xvpi_object_insert_child (struct xvpi_object *obj, struct xvpi_object *child_obj)
{
	if (obj == NULL)
		obj = &xvpi.root_object;

	if (child_obj == NULL) {
		warn("trying to insert NULL child object!\n");
		return 0;
	} else {
		size_t list_size = (obj->u.object.n_childs + 1) * sizeof(struct xvpi_object *);
		struct xvpi_object **p = realloc(obj->u.object.childs, list_size);
		if (!p) {
			fatal("failed allocating %d bytes for xvpi object child list!\n", list_size);
			return 0;
		} else {
			xvpi_object_ref(child_obj);
			p[obj->u.object.n_childs] = child_obj;
			obj->u.object.childs = p;
			obj->u.object.n_childs++;
		}
	}

	return 1;
}

PLI_INT32 xvpi_object_remove_child (struct xvpi_object *obj, struct xvpi_object *child_obj)
{
	int i;

	if (obj == NULL)
		obj = &xvpi.root_object;

	for (i=0; i<obj->u.object.n_childs; i++) {
		if (obj->u.object.childs[i] == child_obj) {
			size_t list_size;
			if (i < obj->u.object.n_childs - 1) {
				void *dst = &obj->u.object.childs[i];
				void *src = &obj->u.object.childs[i+1];
				size_t n = obj->u.object.n_childs - i - 1;
				memmove(dst, src, n * sizeof(struct xvpi_object *));
			}
			obj->u.object.n_childs--;
			list_size = obj->u.object.n_childs * sizeof(struct xvpi_object *);
			obj->u.object.childs = realloc(obj->u.object.childs, list_size);
			xvpi_object_unref(child_obj);
			return 1;
		}
	}

	warn("trying to remove unknown child object!\n");
	return 0;
}

PLI_INT32 xvpi_object_add_child (struct xvpi_object *obj, struct xvpi_object *new_child)
{
	struct xvpi_object *child;

	if (new_child == NULL)
		return 0;

	if (obj == NULL)
		obj = &xvpi.root_object;

	child = xvpi_object_get_child(obj, new_child->name, new_child->type);

	if (obj->type != vpiIterator && child != NULL) {
		err("trying to add object property twice (obj name %d, type %d, child name %d, type %d)!\n",
			obj->name, obj->type, new_child->name, new_child->type);
		return 0;
	}

	if (!IS_XVPI_PROPERTY(new_child->type) && (child = xvpi_object_get_child(obj, new_child->name, vpiIterator)) != NULL)
		return xvpi_object_insert_child(child, new_child);

	return xvpi_object_insert_child(obj, new_child);
}

struct xvpi_object * xvpi_object_get_child_or_create (struct xvpi_object *obj, PLI_INT32 name, PLI_INT32 type)
{
	struct xvpi_object *child = xvpi_object_get_child(obj, name, type);

	if (child == NULL) {
		child = xvpi_object_new(name, type);
		xvpi_object_insert_child(obj, child);
	}

	return child;
}

void * xvpi_get_pointer (int prop, vpiHandle obj)
{
	struct xvpi_object *p = xvpi_object_get_child((struct xvpi_object *) obj, prop, xvpiPointerProp);
	void *val = NULL;
	if (p)
		val = p->u.pointer.ptr;
	return val;
}

