// Distributed under the MIT License, see accompanying file LICENSE.txt
// Copyright John W. Wilkinson 2025
//
// Parses and formats JSON.
// Does not support real numbers or escaping of characters. No Unicode support.

#pragma once
#include <expected>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace simple_json
{
    struct Object;
    struct Array;

    struct Null // a JSON null value.
    {
    };

    using Value = std::variant<std::string, bool, int64_t, Null, Array, Object>;

    // A JSON array is a vector of JSON values.
    //
    // (It would be nicer for Array and Object to be type aliases, e.g.
    //
    //     using Array = public std::vector<Value>;
    //
    // but you can't forward declare type aliases.)
    //
    struct Array : public std::vector<Value>
    {
    };

    // A JSON object is a map of string/Values pairs.
    //
    struct Object : public std::map<std::string, Value>
    {
    };

    // parses a JSON string and return an Object or an error message
    //
    std::expected<Value, std::string> parse( const std::string& json_str );

    // formats an Object as a JSON string and writes it to the output stream
    //
    std::ostream& operator<<( std::ostream& os, const Value& value );
} // namespace simple_json
