#include "super_runtime.h"

void *super_interface_vtable(void *obj, const void *iface) {
    (void)obj;
    (void)iface;
    return NULL;
}

void super_interface_cast(void *obj, const void *iface) {
    (void)obj;
    (void)iface;
}