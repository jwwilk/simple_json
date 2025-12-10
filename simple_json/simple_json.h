// Distributed under the MIT License, see accompanying file LICENSE.txt
// Copyright John W. Wilkinson 2025
//
// Parses and formats JSON.
// Does not support real numbers. No Unicode support.

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
        using std::vector<Value>::vector; // inherit all constructors
    };

    // A JSON object is a map of string/Values pairs.
    //
    struct Object : public std::map<std::string, Value>
    {
        using std::map<std::string, Value>::map; // inherit all constructors
    };

    // parses a JSON string and return an Object or an error message
    //
    std::expected<Value, std::string> parse( const std::string& json_str );

    // formats an Object as a JSON string and writes it to the output stream
    //
    std::ostream& operator<<( std::ostream& os, const Value& value );

    // helper to get a value from a JSON object
    template <typename T>
    std::expected<T, std::string> get_value( const simple_json::Object& obj, const std::string& key )
    {
        auto it = obj.find( key );
        if ( it == obj.end() )
        {
            return std::unexpected( "field \"" + key + "\" not found" );
        }
        if ( auto ptr = std::get_if<T>( &( it->second ) ) )
        {
            return *ptr;
        }
        return std::unexpected( "field \"" + key + "\" is not the expected type" );
    }

} // namespace simple_json
