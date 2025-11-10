/*
 * Complex RTLIL Library Usage Example
 *
 * Demonstrates a more realistic circuit with:
 * - D flip-flops (registers)
 * - Multiplexers
 * - Adders
 * - Comparators
 *
 * Equivalent Verilog:
 *
 * module counter_with_load(
 *     input wire clk,
 *     input wire rst,
 *     input wire load,
 *     input wire [7:0] load_value,
 *     output reg [7:0] count,
 *     output wire overflow
 * );
 *     wire [7:0] next_count;
 *     wire [7:0] incremented;
 *     wire [7:0] mux_out;
 *
 *     // Adder: count + 1
 *     assign incremented = count + 8'd1;
 *
 *     // Mux: select between incremented and load_value
 *     assign mux_out = load ? load_value : incremented;
 *
 *     // Reset mux: select between mux_out and 0
 *     assign next_count = rst ? 8'd0 : mux_out;
 *
 *     // Register
 *     always @(posedge clk)
 *         count <= next_count;
 *
 *     // Overflow detection: count == 255
 *     assign overflow = (count == 8'd255);
 * endmodule
 */

#include "../include/yosys.h"
#include <iostream>
#include <fstream>

using namespace Yosys;

// Helper function to write JSON output
void write_json(RTLIL::Design *design, const std::string &filename);

int main()
{
	std::cout << "================================================================\n";
	std::cout << "  RTLIL Standalone Library - Complex Example\n";
	std::cout << "================================================================\n\n";

	// Initialize library
	rtlil_init();

	// Create design
	RTLIL::Design *design = new RTLIL::Design;
	std::cout << "[1] Created RTLIL::Design\n";

	// Create module
	RTLIL::Module *module = design->addModule(ID(counter_with_load));
	std::cout << "[2] Created module: " << module->name.c_str() << "\n\n";

	// ====================================================================
	// Add ports
	// ====================================================================
	std::cout << "Adding ports:\n";

	// Input: clk (1 bit)
	RTLIL::Wire *clk = module->addWire(ID(clk), 1);
	clk->port_input = true;
	clk->port_id = 1;
	std::cout << "  - clk (input, 1 bit)\n";

	// Input: rst (1 bit)
	RTLIL::Wire *rst = module->addWire(ID(rst), 1);
	rst->port_input = true;
	rst->port_id = 2;
	std::cout << "  - rst (input, 1 bit)\n";

	// Input: load (1 bit)
	RTLIL::Wire *load = module->addWire(ID(load), 1);
	load->port_input = true;
	load->port_id = 3;
	std::cout << "  - load (input, 1 bit)\n";

	// Input: load_value (8 bits)
	RTLIL::Wire *load_value = module->addWire(ID(load_value), 8);
	load_value->port_input = true;
	load_value->port_id = 4;
	std::cout << "  - load_value (input, 8 bits)\n";

	// Output: count (8 bits)
	RTLIL::Wire *count = module->addWire(ID(count), 8);
	count->port_output = true;
	count->port_id = 5;
	std::cout << "  - count (output, 8 bits)\n";

	// Output: overflow (1 bit)
	RTLIL::Wire *overflow = module->addWire(ID(overflow), 1);
	overflow->port_output = true;
	overflow->port_id = 6;
	std::cout << "  - overflow (output, 1 bit)\n";

	// ====================================================================
	// Add internal wires
	// ====================================================================
	std::cout << "\nAdding internal wires:\n";

	RTLIL::Wire *incremented = module->addWire(ID(incremented), 8);
	std::cout << "  - incremented (8 bits)\n";

	RTLIL::Wire *mux_out = module->addWire(ID(mux_out), 8);
	std::cout << "  - mux_out (8 bits)\n";

	RTLIL::Wire *next_count = module->addWire(ID(next_count), 8);
	std::cout << "  - next_count (8 bits)\n";

	// Finalize ports
	module->fixup_ports();

	// ====================================================================
	// Add cells
	// ====================================================================
	std::cout << "\nAdding logic cells:\n";

	// [1] Adder: incremented = count + 1
	RTLIL::Cell *add_cell = module->addCell(ID($add$1), ID($add));
	add_cell->setPort(ID::A, RTLIL::SigSpec(count));
	add_cell->setPort(ID::B, RTLIL::Const(1, 8));  // Constant 1
	add_cell->setPort(ID::Y, RTLIL::SigSpec(incremented));
	add_cell->setParam(ID::A_SIGNED, RTLIL::Const(0));
	add_cell->setParam(ID::B_SIGNED, RTLIL::Const(0));
	add_cell->setParam(ID::A_WIDTH, RTLIL::Const(8));
	add_cell->setParam(ID::B_WIDTH, RTLIL::Const(8));
	add_cell->setParam(ID::Y_WIDTH, RTLIL::Const(8));
	std::cout << "  [1] $add (count + 1 -> incremented)\n";

	// [2] Mux: mux_out = load ? load_value : incremented
	RTLIL::Cell *mux1 = module->addCell(ID($mux$2), ID($mux));
	mux1->setPort(ID::A, RTLIL::SigSpec(incremented));
	mux1->setPort(ID::B, RTLIL::SigSpec(load_value));
	mux1->setPort(ID::S, RTLIL::SigSpec(load));
	mux1->setPort(ID::Y, RTLIL::SigSpec(mux_out));
	mux1->setParam(ID::WIDTH, RTLIL::Const(8));
	std::cout << "  [2] $mux (load ? load_value : incremented -> mux_out)\n";

	// [3] Reset Mux: next_count = rst ? 0 : mux_out
	RTLIL::Cell *mux2 = module->addCell(ID($mux$3), ID($mux));
	mux2->setPort(ID::A, RTLIL::SigSpec(mux_out));
	mux2->setPort(ID::B, RTLIL::Const(0, 8));  // Constant 0
	mux2->setPort(ID::S, RTLIL::SigSpec(rst));
	mux2->setPort(ID::Y, RTLIL::SigSpec(next_count));
	mux2->setParam(ID::WIDTH, RTLIL::Const(8));
	std::cout << "  [3] $mux (rst ? 0 : mux_out -> next_count)\n";

	// [4] D Flip-Flop: count <= next_count @ posedge clk
	RTLIL::Cell *dff = module->addCell(ID($dff$4), ID($dff));
	dff->setPort(ID::CLK, RTLIL::SigSpec(clk));
	dff->setPort(ID::D, RTLIL::SigSpec(next_count));
	dff->setPort(ID::Q, RTLIL::SigSpec(count));
	dff->setParam(ID::WIDTH, RTLIL::Const(8));
	dff->setParam(ID::CLK_POLARITY, RTLIL::Const(1));  // Positive edge
	std::cout << "  [4] $dff (D flip-flop: next_count -> count @ posedge clk)\n";

	// [5] Comparator: overflow = (count == 255)
	RTLIL::Cell *eq_cell = module->addCell(ID($eq$5), ID($eq));
	eq_cell->setPort(ID::A, RTLIL::SigSpec(count));
	eq_cell->setPort(ID::B, RTLIL::Const(255, 8));  // Constant 255
	eq_cell->setPort(ID::Y, RTLIL::SigSpec(overflow));
	eq_cell->setParam(ID::A_SIGNED, RTLIL::Const(0));
	eq_cell->setParam(ID::B_SIGNED, RTLIL::Const(0));
	eq_cell->setParam(ID::A_WIDTH, RTLIL::Const(8));
	eq_cell->setParam(ID::B_WIDTH, RTLIL::Const(8));
	eq_cell->setParam(ID::Y_WIDTH, RTLIL::Const(1));
	std::cout << "  [5] $eq (count == 255 -> overflow)\n";

	// ====================================================================
	// Print design statistics
	// ====================================================================
	std::cout << "\n================================================================\n";
	std::cout << "Design Statistics:\n";
	std::cout << "================================================================\n";
	std::cout << "Modules:         " << design->modules().size() << "\n";
	std::cout << "Wires:           " << module->wires().size() << "\n";
	std::cout << "  - Inputs:      " << 4 << " (clk, rst, load, load_value)\n";
	std::cout << "  - Outputs:     " << 2 << " (count, overflow)\n";
	std::cout << "  - Internal:    " << 3 << " (incremented, mux_out, next_count)\n";
	std::cout << "Cells:           " << module->cells().size() << "\n";
	std::cout << "  - Adders:      " << 1 << " ($add)\n";
	std::cout << "  - Muxes:       " << 2 << " ($mux)\n";
	std::cout << "  - Registers:   " << 1 << " ($dff)\n";
	std::cout << "  - Comparators: " << 1 << " ($eq)\n";

	// ====================================================================
	// Detailed module walkthrough
	// ====================================================================
	std::cout << "\n================================================================\n";
	std::cout << "Module Structure:\n";
	std::cout << "================================================================\n";

	std::cout << "\nInput Ports:\n";
	for (auto wire : module->wires()) {
		if (wire->port_input) {
			std::cout << "  " << wire->name.c_str()
			          << " [" << wire->width << " bit(s)]\n";
		}
	}

	std::cout << "\nOutput Ports:\n";
	for (auto wire : module->wires()) {
		if (wire->port_output) {
			std::cout << "  " << wire->name.c_str()
			          << " [" << wire->width << " bit(s)]\n";
		}
	}

	std::cout << "\nCells (detailed):\n";
	for (auto cell : module->cells()) {
		std::cout << "  " << cell->name.c_str()
		          << " : " << cell->type.c_str() << "\n";

		// Show connections
		for (auto &conn : cell->connections()) {
			std::cout << "    ." << conn.first.c_str() << "(";

			// Show signal
			const RTLIL::SigSpec &sig = conn.second;
			if (sig.is_wire()) {
				std::cout << sig.as_wire()->name.c_str();
			} else if (sig.is_fully_const()) {
				std::cout << sig.as_const().as_int();
			} else {
				std::cout << "[complex signal]";
			}
			std::cout << ")\n";
		}
	}

	// ====================================================================
	// Write JSON output
	// ====================================================================
	std::cout << "\n================================================================\n";
	std::cout << "Writing JSON output to 'complex_example.json'...\n";
	write_json(design, "complex_example.json");
	std::cout << "Done!\n";

	// Cleanup
	delete design;
	std::cout << "\n================================================================\n";
	std::cout << "Example completed successfully!\n";
	std::cout << "================================================================\n";

	return 0;
}

// Simple JSON writer for RTLIL design
void write_json(RTLIL::Design *design, const std::string &filename)
{
	std::ofstream f(filename);
	if (!f.is_open()) {
		std::cerr << "Error: Could not open " << filename << " for writing\n";
		return;
	}

	f << "{\n";
	f << "  \"creator\": \"RTLIL Standalone Library\",\n";
	f << "  \"modules\": {\n";

	bool first_mod = true;
	for (auto module : design->modules()) {
		if (!first_mod) f << ",\n";
		first_mod = false;

		f << "    \"" << module->name.c_str() << "\": {\n";

		// Ports
		f << "      \"ports\": {\n";
		bool first_port = true;
		for (auto wire : module->wires()) {
			if (wire->port_input || wire->port_output) {
				if (!first_port) f << ",\n";
				first_port = false;

				f << "        \"" << wire->name.c_str() << "\": {\n";
				f << "          \"direction\": \"";
				if (wire->port_input && wire->port_output)
					f << "inout";
				else if (wire->port_input)
					f << "input";
				else
					f << "output";
				f << "\",\n";
				f << "          \"bits\": " << wire->width << "\n";
				f << "        }";
			}
		}
		f << "\n      },\n";

		// Cells
		f << "      \"cells\": {\n";
		bool first_cell = true;
		for (auto cell : module->cells()) {
			if (!first_cell) f << ",\n";
			first_cell = false;

			f << "        \"" << cell->name.c_str() << "\": {\n";
			f << "          \"type\": \"" << cell->type.c_str() << "\",\n";
			f << "          \"connections\": {\n";

			bool first_conn = true;
			for (auto &conn : cell->connections()) {
				if (!first_conn) f << ",\n";
				first_conn = false;

				f << "            \"" << conn.first.c_str() << "\": ";

				const RTLIL::SigSpec &sig = conn.second;
				if (sig.is_wire()) {
					f << "\"" << sig.as_wire()->name.c_str() << "\"";
				} else if (sig.is_fully_const()) {
					f << sig.as_const().as_int();
				} else {
					f << "\"[signal]\"";
				}
			}
			f << "\n          }\n";
			f << "        }";
		}
		f << "\n      }\n";

		f << "    }";
	}

	f << "\n  }\n";
	f << "}\n";
	f.close();
}
