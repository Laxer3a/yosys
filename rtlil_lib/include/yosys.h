/* -*- c++ -*-
 *  Simplified yosys.h for standalone RTLIL library
 *  Based on Yosys Open SYnthesis Suite
 *
 *  Copyright (C) 2012  Claire Xenia Wolf <claire@yosyshq.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef YOSYS_H
#define YOSYS_H

#include "yosys_common.h"

// SHA1 stub (declared before log.h which has forward declaration)
class SHA1 {
public:
	void update(const std::string &) { }
};

#include "log.h"
#include "rtlil.h"

YOSYS_NAMESPACE_BEGIN

// Minimal initialization for RTLIL library
void rtlil_init();
void rtlil_shutdown();

// Global design pointer (optional, for compatibility)
extern RTLIL::Design *rtlil_design;

// Stub functions for compatibility with log.cc
inline RTLIL::Design *yosys_get_design() { return rtlil_design; }
inline void yosys_shutdown() { rtlil_shutdown(); }

// RTLIL_BACKEND stub namespace
namespace RTLIL_BACKEND {
	inline void dump_sigspec(std::ostream &os, const RTLIL::SigSpec &sig, bool autoint = true) {
		(void)autoint;
		os << "[SigSpec]";  // Simplified output
	}
	inline void dump_module(std::ostream &os, std::string indent, RTLIL::Module *module,
	                       RTLIL::Design *design, bool only_selected) {
		(void)indent; (void)design; (void)only_selected;
		os << "module " << module->name.c_str() << "\n";
	}
	inline void dump_cell(std::ostream &os, std::string indent, RTLIL::Cell *cell) {
		(void)indent;
		os << cell->type.c_str() << " " << cell->name.c_str() << "\n";
	}
}

// Pass stub
struct Pass {
	static void call(RTLIL::Design *, std::vector<std::string>) {
		// Stub: Pass system not available in standalone lib
	}
};

YOSYS_NAMESPACE_END

#endif
