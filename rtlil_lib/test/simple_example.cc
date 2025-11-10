/*
 * Simple RTLIL Library Usage Example
 *
 * This demonstrates basic RTLIL API usage for creating a simple netlist.
 * Creates a module with input/output wires and a simple logic gate.
 */

#include "../include/yosys.h"
#include <iostream>

using namespace Yosys;

int main()
{
	std::cout << "RTLIL Standalone Library - Simple Example\n";
	std::cout << "==========================================\n\n";

	// Initialize RTLIL library
	rtlil_init();

	// Create a new design
	RTLIL::Design *design = new RTLIL::Design;
	std::cout << "Created new RTLIL::Design\n";

	// Create a module named "top"
	RTLIL::Module *module = design->addModule(ID(top));
	std::cout << "Created module: " << module->name.c_str() << "\n";

	// Add input wire 'a' (1 bit)
	RTLIL::Wire *wire_a = module->addWire(ID(a), 1);
	wire_a->port_input = true;
	wire_a->port_id = 1;
	std::cout << "Added input wire 'a' (1 bit)\n";

	// Add input wire 'b' (1 bit)
	RTLIL::Wire *wire_b = module->addWire(ID(b), 1);
	wire_b->port_input = true;
	wire_b->port_id = 2;
	std::cout << "Added input wire 'b' (1 bit)\n";

	// Add output wire 'y' (1 bit)
	RTLIL::Wire *wire_y = module->addWire(ID(y), 1);
	wire_y->port_output = true;
	wire_y->port_id = 3;
	std::cout << "Added output wire 'y' (1 bit)\n";

	// Finalize port setup
	module->fixup_ports();

	// Add an AND gate
	RTLIL::Cell *and_cell = module->addCell(ID(and_gate), ID($and));
	and_cell->setPort(ID::A, RTLIL::SigSpec(wire_a));
	and_cell->setPort(ID::B, RTLIL::SigSpec(wire_b));
	and_cell->setPort(ID::Y, RTLIL::SigSpec(wire_y));
	and_cell->setParam(ID::A_SIGNED, RTLIL::Const(0));
	and_cell->setParam(ID::B_SIGNED, RTLIL::Const(0));
	and_cell->setParam(ID::A_WIDTH, RTLIL::Const(1));
	and_cell->setParam(ID::B_WIDTH, RTLIL::Const(1));
	and_cell->setParam(ID::Y_WIDTH, RTLIL::Const(1));
	std::cout << "Added $and cell 'and_gate'\n";

	// Print design statistics
	std::cout << "\nDesign Statistics:\n";
	std::cout << "------------------\n";
	std::cout << "Modules: " << design->modules().size() << "\n";
	std::cout << "Wires in 'top': " << module->wires().size() << "\n";
	std::cout << "Cells in 'top': " << module->cells().size() << "\n";

	// Print module structure
	std::cout << "\nModule 'top' structure:\n";
	std::cout << "  Inputs:\n";
	for (auto wire : module->wires()) {
		if (wire->port_input)
			std::cout << "    " << wire->name.c_str() << " [" << wire->width << " bit(s)]\n";
	}
	std::cout << "  Outputs:\n";
	for (auto wire : module->wires()) {
		if (wire->port_output)
			std::cout << "    " << wire->name.c_str() << " [" << wire->width << " bit(s)]\n";
	}
	std::cout << "  Cells:\n";
	for (auto cell : module->cells()) {
		std::cout << "    " << cell->name.c_str() << " (type: " << cell->type.c_str() << ")\n";
	}

	// Cleanup
	delete design;
	std::cout << "\nDesign deleted successfully.\n";
	std::cout << "\nExample completed successfully!\n";

	return 0;
}
