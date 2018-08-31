#include "xvpi.h"
#include "xvpi_elaborate.h"

static int clear_toplevel_module_property (PLI_BYTE8 *module_name)
{
	struct xvpi_object *module = (struct xvpi_object *) vpi_handle_by_name(module_name, NULL);
	if (module) {
		struct xvpi_object *toplevel_property = xvpi_object_get_child_or_create(module, vpiTopModule, xvpiBoolProp);
		toplevel_property->u.boolean = 0;
		xvpi_object_unref(module);
		return 0;
	}
	warn("unable to find module '%s'!\n", (char *) module_name);
	return -1;
}

static int identify_and_tag_toplevel_modules (void)
{
	struct xvpi_object *module_iterator = xvpi_object_get_child(NULL, vpiModule, vpiIterator);
	size_t i;
	if (!module_iterator) {
		err("Hierarchy contains no modules!\n");
		return -1;
	}
	for (i=0; i<module_iterator->u.object.n_childs; i++) {
		struct xvpi_object *module = module_iterator->u.object.childs[i];
		struct xvpi_object *module_instance_iterator = xvpi_object_get_child(module, vpiModule, vpiIterator);
		if (module_instance_iterator) {
			size_t j;
			for (j=0; j<module_instance_iterator->u.object.n_childs; j++) {
				struct xvpi_object *instance = module_instance_iterator->u.object.childs[j];
				PLI_BYTE8 *defname = vpi_get_str(vpiDefName, (vpiHandle) instance);
				int err;
				if ((err = clear_toplevel_module_property(defname)) != 0)
					return err;
			}
		}
	}
	return 0;
}

#if 0
int xvpi_recurs_elabrate(D_ParseNode *pn, const char *key)
{
  struct D_ParseNode *discipline_pn = parsenode_find_by_name_f(pn, key, 1);
  struct xvpi_object *discipline = parsenode_to_xvpi_object_ref_f(discipline_pn, vpiDiscipline);
  if(discipline){
    msg("parsetree find key:'%s' with obj.\n", key);
    return 0;
  }
  else{
    warn("parsetree unable to find key:'%s'!\n", (char *) key);
    return 1;
  }

}
#endif

int xvpi_elaborate (void)
{
	int err;
        const int max_depth=1000;
        test_dump_vpi_obj(max_depth, (void *)(&(xvpi.root_object)));
	if ((err = identify_and_tag_toplevel_modules()) != 0)
		return err;
	return 0;
}

