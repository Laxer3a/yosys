/* -*- c++ -*-
 *  Stub register.h for standalone RTLIL library
 *  (Pass/Command registration is not supported in this minimal library)
 */

#ifndef REGISTER_H
#define REGISTER_H

#include "yosys_common.h"

// Dummy source_location for compatibility
struct source_location {
	int line() const { return 0; }
	int column() const { return 0; }
	const char* file_name() const { return "unknown"; }
	const char* function_name() const { return "unknown"; }
	static const source_location current(...) { return source_location(); }
};

YOSYS_NAMESPACE_BEGIN

// Stub Pass class (not functional in standalone library)
struct Pass {
	std::string pass_name, short_help;
	Pass(std::string name, std::string short_help = "") : pass_name(name), short_help(short_help) {}
	virtual ~Pass() {}
};

YOSYS_NAMESPACE_END

#endif
