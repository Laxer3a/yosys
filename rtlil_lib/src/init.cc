/*
 * Initialization code for standalone RTLIL library
 */

#include "../include/yosys.h"
#include "../include/celltypes.h"

YOSYS_NAMESPACE_BEGIN

RTLIL::Design *rtlil_design = nullptr;
int yosys_xtrace = 0;
int autoidx = 0;
CellTypes yosys_celltypes;

// Memhasher (not used in standalone lib, but needed for linking)
bool memhasher_active = false;
void memhasher_do() { }

// Hash library needs this constant
namespace hashlib {
	uint32_t HasherDJB32::fudge = 0x1337c0d3;
}

// new_id stub
RTLIL::IdString new_id(std::string, int, std::string) {
	return RTLIL::IdString("\\$auto$" + std::to_string(autoidx++));
}

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
