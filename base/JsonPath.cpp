/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/JsonObject.h>
#include <base/JsonPath.h>
#include <base/JsonValue.h>

namespace Base {

JsonPathElement JsonPathElement::any_array_element { Kind::AnyIndex };
JsonPathElement JsonPathElement::any_object_element { Kind::AnyKey };

JsonValue JsonPath::resolve(const JsonValue& top_root) const
{
    auto root = top_root;
    for (auto& element : *this) {
        switch (element.kind()) {
        case JsonPathElement::Kind::Key:
            root = JsonValue { root.as_object().get(element.key()) };
            break;
        case JsonPathElement::Kind::Index:
            root = JsonValue { root.as_array().at(element.index()) };
            break;
        default:
            VERIFY_NOT_REACHED();
        }
    }
    return root;
}

String JsonPath::to_string() const
{
    StringBuilder builder;
    builder.append("{ .");
    for (auto& el : *this) {
        builder.append(" > ");
        builder.append(el.to_string());
    }
    builder.append(" }");
    return builder.to_string();
}

}