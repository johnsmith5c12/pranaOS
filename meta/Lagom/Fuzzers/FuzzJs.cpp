/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <libjs/Interpreter.h>
#include <libjs/Lexer.h>
#include <libjs/Parser.h>
#include <libjs/Runtime/GlobalObject.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto js = StringView(static_cast<const unsigned char*>(data), size);
    auto lexer = JS::Lexer(js);
    auto parser = JS::Parser(lexer);
    auto program = parser.parse_program();
    if (!parser.has_errors()) {
        auto vm = JS::VM::create();
        auto interpreter = JS::Interpreter::create<JS::GlobalObject>(*vm);
        interpreter->run(interpreter->global_object(), *program);
    }
    return 0;
}