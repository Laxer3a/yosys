/*
 * Initialization code for standalone RTLIL library
 */

#include "../include/yosys.h"

YOSYS_NAMESPACE_BEGIN

RTLIL::Design *rtlil_design = nullptr;

void rtlil_init()
{
	// Initialize IdString system (use public function)
	RTLIL::IdString::ensure_prepopulated();

	// Optionally create a global design
	// rtlil_design = new RTLIL::Design();
}

void rtlil_shutdown()
{
	// Cleanup global design if it was created
	if (rtlil_design) {
		delete rtlil_design;
		rtlil_design = nullptr;
	}
}

YOSYS_NAMESPACE_END
