#include "third_party/python/Include/intrcheck.h"
#include "third_party/python/Include/pyerrors.h"
/* clang-format off */

/* Sigcheck is similar to intrcheck() but sets an exception when an
   interrupt occurs.  It can't be in the intrcheck.c file since that
   file (and the whole directory it is in) doesn't know about objects
   or exceptions.  It can't be in errors.c because it can be
   overridden (at link time) by a more powerful version implemented in
   signalmodule.c. */

#pragma weak PyErr_CheckSignals

/* ARGSUSED */
int
PyErr_CheckSignals(void)
{
	if (!PyOS_InterruptOccurred())
		return 0;
	PyErr_SetNone(PyExc_KeyboardInterrupt);
	return -1;
}
