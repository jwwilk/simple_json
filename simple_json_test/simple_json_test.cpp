// Distributed under the MIT License, see accompanying file LICENSE.txt
// Copyright John W. Wilkinson 2025

#include "simple_json.h"
#include <gtest/gtest.h>
#include <chrono>

using namespace simple_json;
using namespace std;

namespace
{
    void check_json( const string& json_str )
    {
        expected<Value, string> value = parse( json_str );

        if ( !value )
        {
            FAIL() << "Failed to parse JSON: " << value.error();
            return;
        }

        ostringstream os;
        os << *value;
        EXPECT_EQ( json_str, os.str() );
    }

    void check_invalid( const string& json_str, const string& expected_error )
    {
        expected<Value, string> obj = parse( json_str );
        ASSERT_FALSE( obj );
        EXPECT_EQ( obj.error(), expected_error );
    }
} // namespace

TEST( Simple_json_test, test_parsing )
{
    const Object o0{};

    EXPECT_EQ( o0.size(), 0 );

    check_json( "\"hello world\"" );

    check_json( "12345" );
    check_json( "-12345" );

    check_json( "true" );

    check_json( "false" );

    check_json( "null" );

    check_json( "{\n"
                "    \"foo\" : \"\"\n"
                "}" );

    check_json( "{\n"
                "    \"a\" : \"b\"\n"
                "}" );

    check_json( "{\n"
                "    \"aa\" : \"bb\"\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : \"bar\"\n"
                "}" );

    check_json( "{\n"
                "    \"foo_1\" : \"bar_1\",\n"
                "    \"foo_2\" : \"bar_2\"\n"
                "}" );

    check_json( "{\n"
                "    \"foo_1\" : \"bar_1\",\n"
                "    \"foo_2\" : \"bar_2\",\n"
                "    \"foo_3\" : \"bar_3\"\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : {\n"
                "        \"foo_1\" : \"bar_1\"\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : {\n"
                "        \"foo_1\" : \"bar_1\",\n"
                "        \"foo_2\" : \"bar_2\"\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : {\n"
                "        \"foo_1\" : \"bar_1\",\n"
                "        \"foo_2\" : \"bar_2\"\n"
                "    },\n"
                "    \"foo_3\" : \"bar_3\",\n"
                "    \"foo_4\" : \"bar_4\"\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : {\n"
                "        \"foo_1\" : \"bar_1\",\n"
                "        \"foo_2\" : \"bar_2\"\n"
                "    },\n"
                "    \"foo2\" : {\n"
                "        \"foo_1\" : \"bar_1\",\n"
                "        \"foo_2\" : \"bar_2\"\n"
                "    }\n"
                "}" );

    check_json( "[\n"
                "    123, \"asdf\", true, null, {\n"
                "        \"foo\" : \"bar\"\n"
                "    }\n"
                "]" );

    check_json( "[\n"
                "    123, \"asdf\", true, null, [\n"
                "        1, 2\n"
                "    ]\n"
                "]" );

    check_json( "{\n"
                "    \"\" : \"value\",\n"
                "    \"name\" : \"\"\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : {\n"
                "        \"name 3\" : \"value 3\",\n"
                "        \"name_4\" : \"value_4\"\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : {\n"
                "        \"name 3\" : \"value 3\",\n"
                "        \"name_4\" : \"value_4\",\n"
                "        \"name_5\" : {\n"
                "            \"name_6\" : \"value_6\",\n"
                "            \"name_7\" : \"value_7\"\n"
                "        }\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : {\n"
                "        \"name 3\" : \"value 3\",\n"
                "        \"name_4\" : {\n"
                "            \"name_5\" : \"value_5\",\n"
                "            \"name_6\" : \"value_6\"\n"
                "        },\n"
                "        \"name_7\" : \"value_7\"\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : {\n"
                "        \"name 3\" : \"value 3\",\n"
                "        \"name_4\" : {\n"
                "            \"name_5\" : \"value_5\",\n"
                "            \"name_6\" : \"value_6\"\n"
                "        },\n"
                "        \"name_7\" : \"value_7\"\n"
                "    },\n"
                "    \"name_8\" : \"value_8\",\n"
                "    \"name_9\" : {\n"
                "        \"name_10\" : \"value_10\"\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : {\n"
                "        \"name 2\" : {\n"
                "            \"name 3\" : {\n"
                "                \"name_4\" : {\n"
                "                    \"name_5\" : \"value\"\n"
                "                }\n"
                "            }\n"
                "        }\n"
                "    }\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : true,\n"
                "    \"name 3\" : false,\n"
                "    \"name_4\" : \"value_4\",\n"
                "    \"name_5\" : true\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : null,\n"
                "    \"name 3\" : \"value 3\",\n"
                "    \"name_4\" : null\n"
                "}" );

    check_json( "{\n"
                "    \"name 1\" : \"value 1\",\n"
                "    \"name 2\" : 123,\n"
                "    \"name 3\" : \"value 3\",\n"
                "    \"name_4\" : -567\n"
                "}" );

    check_json( "[\n"
                "    \n"
                "]" );

    check_json( "[\n"
                "    1\n"
                "]" );

    check_json( "[\n"
                "    1, \"john]\", true, false, null\n"
                "]" );

    check_json( "[\n"
                "    1, [\n"
                "        2, 3\n"
                "    ]\n"
                "]" );

    check_json( "[\n"
                "    1, [\n"
                "        2, 3\n"
                "    ], [\n"
                "        4, [\n"
                "            5, 6, 7\n"
                "        ]\n"
                "    ]\n"
                "]" );

    check_json( "[\n"
                "    {\n"
                "        \"name\" : \"value\"\n"
                "    }\n"
                "]" );

    check_json( "{\n"
                "    \"name\" : [\n"
                "        1\n"
                "    ]\n"
                "}" );

    check_json( "[\n"
                "    {\n"
                "        \"name 1\" : \"value\",\n"
                "        \"name 2\" : [\n"
                "            1, 2, 3\n"
                "        ]\n"
                "    }\n"
                "]" );

    check_json( "{\n"
                "    \"name 1\" : [\n"
                "        1, {\n"
                "            \"name 2\" : \"value 2\"\n"
                "        }\n"
                "    ]\n"
                "}" );

    check_json( "[\n"
                "    {\n"
                "        \"name 1\" : \"value 1\",\n"
                "        \"name 2\" : [\n"
                "            1, 2, {\n"
                "                \"name 3\" : \"value 3\"\n"
                "            }\n"
                "        ]\n"
                "    }\n"
                "]" );

    check_json( "{\n"
                "    \"name 1\" : [\n"
                "        1, {\n"
                "            \"name 2\" : [\n"
                "                1, 2, 3\n"
                "            ]\n"
                "        }\n"
                "    ]\n"
                "}" );

} // namespace

TEST( Simple_json_test, test_parsing_integers )
{
    check_json( "{\n"
                "    \"foo\" : 1\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : 12\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : 123\n"
                "}" );

    check_json( "{\n"
                "    \"foo_1\" : 123456789,\n"
                "    \"foo_2\" : 987654321\n"
                "}" );
}

TEST( Simple_json_test, test_parsing_bools )
{
    check_json( "{\n"
                "    \"foo\" : true\n"
                "}" );

    check_json( "{\n"
                "    \"foo\" : false\n"
                "}" );
}

TEST( Simple_json_test, test_parsing_null )
{
    check_json( "{\n"
                "    \"foo\" : null\n"
                "}" );
}

TEST( Simple_json_test, test_whitespace_ignored )
{
    const string json_str = "    {  \n"
                            "\" foo 1  \":\"  bar 1  \"    ,  "
                            "    \"foo 2\"     :     \"bar  2\",\n"
                            "\t\"foo   3\"    :    123    \n"
                            "   }  ";

    expected<Value, string> obj = parse( json_str );

    ASSERT_TRUE( obj );

    ostringstream os;
    os << *obj;

    const string expected = "{\n"
                            "    \" foo 1  \" : \"  bar 1  \",\n"
                            "    \"foo   3\" : 123,\n"
                            "    \"foo 2\" : \"bar  2\"\n"
                            "}";

    EXPECT_EQ( os.str(), expected );
}

namespace
{
    void test_escape_chars_2( const string& json_str, const string& expected_result )
    {
        const string json_obj_str( string( "{\n    \"" ) + json_str + "\" : \"" + json_str + "\"\n}" );

        std::expected<Value, std::string> value = parse( json_obj_str );

        if ( !value )
        {
            FAIL() << "Failed to parse JSON: " << value.error();
            return;
        }

        ASSERT_TRUE( std::holds_alternative<Object>( *value ) );
        const Object& obj = std::get<Object>( *value );
        ASSERT_EQ( obj.size(), 1 );
        const auto& pair = *obj.begin();
        EXPECT_EQ( pair.first, expected_result );

        ASSERT_TRUE( std::holds_alternative<string>( pair.second ) );
        EXPECT_EQ( std::get<string>( pair.second ), expected_result );

        ostringstream os;
        os << *value;
        EXPECT_EQ( os.str(), json_obj_str );
    }

    void test_escape_chars( const string& json_str, const string& expected_result )
    {
        test_escape_chars_2( json_str, expected_result );

        test_escape_chars_2( "a" + json_str, "a" + expected_result );
        test_escape_chars_2( json_str + "b", expected_result + "b" );
        test_escape_chars_2( "a" + json_str + "b", "a" + expected_result + "b" );

        test_escape_chars_2( "12" + json_str, "12" + expected_result );
        test_escape_chars_2( json_str + "12", expected_result + "12" );
        test_escape_chars_2( "12" + json_str + "34", "12" + expected_result + "34" );

        test_escape_chars_2( "123" + json_str, "123" + expected_result );
        test_escape_chars_2( json_str + "123", expected_result + "123" );
        test_escape_chars_2( "123" + json_str + "344", "123" + expected_result + "344" );
    }
} // namespace

TEST( Simple_json_test, test_escape_chars )
{
    // check_json( R"("a \"quoted\" string")" );
    test_escape_chars( "X", "X" );
    test_escape_chars( "\\\"", "\"" );
    test_escape_chars( "\\t", "\t" );
    test_escape_chars( "\\b", "\b" );
    test_escape_chars( "\\f", "\f" );
    test_escape_chars( "\\n", "\n" );
    test_escape_chars( "\\r", "\r" );
    test_escape_chars( "\\/", "/" );
    test_escape_chars( "\\\\", "\\" );
    test_escape_chars( "\\n\\r", "\n\r" );
    test_escape_chars( "\\\"\\n\\r\\t\\b\\/", "\"\n\r\t\b/" );
}

TEST( Simple_json_test, test_parsing_invalid_strings )
{
    check_invalid( R"("foo":"bar"})", "unprocessed data at line 1 column 6" );
    check_invalid( R"({"foo:"bar"})", "missing ':' at line 1 column 8" );
    check_invalid( R"({"foo""bar"})", "missing ':' at line 1 column 7" );
    check_invalid( R"({"foo"-"bar"})", "missing ':' at line 1 column 7" );
    check_invalid( R"({"foo":bar"})", "unexpected character 'b' at line 1 column 8" );
    check_invalid( R"({"foo":"bar})", "missing closing '\"' at line 1 column 13" );
    check_invalid( R"({"foo":"bar")", "missing closing '}' at line 1 column 13" );
    check_invalid( R"({"foo":)", "end of string reached while looking for second of pair at line 1 column 8" );
    check_invalid( R"({"foo":t)", "expected \"true\" at line 1 column 8" );
    check_invalid( R"({"foo":truX)", "expected \"true\" at line 1 column 8" );
    check_invalid( R"({"foo":f)", "expected \"false\" at line 1 column 8" );
    check_invalid( R"({"foo":falsX)", "expected \"false\" at line 1 column 8" );
    check_invalid( R"({"foo":n)", "expected \"null\" at line 1 column 8" );
    check_invalid( R"({"foo":nulX)", "expected \"null\" at line 1 column 8" );
    check_invalid( R"({"foo_1":"bar_1";"foo_2":"bar_2"})", "unexpected character ';' at line 1 column 17" );
    check_invalid( R"({"foo":1111111111111111111111111111111111})",
                   R"(could not convert "1111111111111111111111111111111111" to an integer at line 1 column 42)" );
    check_invalid( "[1:2]", "unexpected character ':' at line 1 column 3" );
    check_invalid( "[1,2:3]", "unexpected character ':' at line 1 column 5" );
    check_invalid( "[1,2", "missing closing ']' at line 1 column 5" );
    check_invalid( "[", "missing closing ']' at line 1 column 2" );
    check_invalid( "  [", "missing closing ']' at line 1 column 4" );
    check_invalid( "  [  ", "missing closing ']' at line 1 column 6" );
    check_invalid( R"("foo \q bar")", "invalid escape character '\\q' at line 1 column 7" );

    check_invalid( "{\n"
                   "    \"foo_1\" : 123456789,\n"
                   "    \"foo_2\" X 987654321\n"
                   "}",
                   "missing ':' at line 3 column 13" );
}

namespace
{
    struct Student
    {
        string name;
        int age;
        std::vector<int> grades;
    };

    std::expected<Student, std::string> parse_student_obj( const std::string& json_str )
    {
        auto value = simple_json::parse( json_str );

        if ( !value )
        {
            return std::unexpected( value.error() );
        }

        Object* obj = get_if<Object>( &*value );
        if ( !obj )
        {
            return std::unexpected( "JSON root is not an object" );
        }

        const auto name = get_value<string>( *obj, "name" );
        if ( !name )
        {
            return std::unexpected( name.error() );
        }

        const auto age = get_value<int64_t>( *obj, "age" );
        if ( !age )
        {
            return std::unexpected( age.error() );
        }

        const auto grades_array = get_value<Array>( *obj, "grades" );
        if ( !grades_array )
        {
            return std::unexpected( grades_array.error() );
        }

        std::vector<int> grades;
        for ( const auto grade_value : grades_array->get() )
        {
            const int64_t* grade_ptr = get_if<int64_t>( &grade_value );
            if ( !grade_ptr )
            {
                return std::unexpected( "array \"grade\" contains a non integer value" );
            }
            grades.push_back( *grade_ptr );
        }

        return { Student{ name->get(), (int)age->get(), grades } };
    }
} // namespace

TEST( Simple_json_test, test_git_hub_readme )
{
    const Student alice{ "Alice", 20, { 85, 90, 78 } };

    Object alice_obj;
    alice_obj[ "name" ] = alice.name;
    alice_obj[ "age" ] = alice.age;
    alice_obj[ "grades" ] = Array{ alice.grades.begin(), alice.grades.end() };

    ostringstream os;
    os << alice_obj;
    EXPECT_EQ( os.str(), "{\n"
                         "    \"age\" : 20,\n"
                         "    \"grades\" : [\n"
                         "        85, 90, 78\n"
                         "    ],\n"
                         "    \"name\" : \"Alice\"\n"
                         "}" );

    //

    const string bob_json = "{\n"
                            "    \"age\" : 21,\n"
                            "    \"grades\" : [\n"
                            "        55, 69, 64\n"
                            "    ],\n"
                            "    \"name\" : \"Bob\"\n"
                            "}";

    auto result = parse_student_obj( bob_json );

    ASSERT_TRUE( result ); // parsing succeeded

    const Student& bob = *result;
    EXPECT_EQ( bob.name, "Bob" );
    EXPECT_EQ( bob.age, 21 );

    ASSERT_EQ( bob.grades.size(), 3 );
    EXPECT_EQ( bob.grades[ 0 ], 55 );
    EXPECT_EQ( bob.grades[ 1 ], 69 );
    EXPECT_EQ( bob.grades[ 2 ], 64 );

    //

    result = parse_student_obj( "[1,]" );
    ASSERT_FALSE( result );
    EXPECT_EQ( result.error(), "unexpected character ']' at line 1 column 4" );

    result = parse_student_obj( "[1,2,3]" );
    ASSERT_FALSE( result );
    EXPECT_EQ( result.error(), "JSON root is not an object" );

    result = parse_student_obj( "{\n"
                               "    \"grades\" : [\n"
                               "        55, 69, 64\n"
                               "    ],\n"
                               "    \"name\" : \"Bob\"\n"
                               "}" );
    ASSERT_FALSE( result );
    EXPECT_EQ( result.error(), "field \"age\" not found" );

    result = parse_student_obj( "{\n"
                               "    \"age\" : 21,\n"
                               "    \"grades\" : [\n"
                               "        55, 69, 64\n"
                               "    ],\n"
                               "    \"name\" : 1234\n"
                               "}" );
    ASSERT_FALSE( result );
    EXPECT_EQ( result.error(), "field \"name\" is not the expected type" );

    result = parse_student_obj( "{\n"
                               "    \"age\" : 21,\n"
                               "    \"grades\" : [\n"
                               "        55, \"foo\", 64\n"
                               "    ],\n"
                               "    \"name\" : \"Bob\"\n"
                               "}" );
    ASSERT_FALSE( result );
    EXPECT_EQ( result.error(), "array \"grade\" contains a non integer value" );
}

namespace
{
    string key_name( int i )
    {
        return "test_" + std::to_string( i );
    }
} // namespace

TEST( DISABLED_Simple_json_test, test_speed )
{
    Object obj;

    for ( int i = 0; i < 4000000; ++i )
    {
        obj.emplace( key_name(i), Array{ i, 222, 33333, "abcdefg", true, false, Null{} } );
    }

    auto start = std::chrono::steady_clock::now();

    ostringstream os;
    os << obj;

    auto end = std::chrono::steady_clock::now();
    cout << "write time " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ) << endl;

    start = std::chrono::steady_clock::now();

    auto value = parse( os.str() );
    ASSERT_TRUE( value );

    end = std::chrono::steady_clock::now();
    cout << "read time " <<  std::chrono::duration_cast<std::chrono::milliseconds>( end - start ) << endl;

    auto obj_2 = get_if<Object>( &*value );

    cout << "obj size " << obj.size() << endl;
}

TEST( DISABLED_Simple_json_test, test_get_value_speed )
{
    Object toplevel_obj;

    auto [ it, inserted ] = toplevel_obj.emplace( "toplevel", Object{} );

    ASSERT_TRUE( inserted );
    Value& val = it->second;
    Object& obj = std::get<Object>( val );

    for ( int i = 0; i < 10000000; ++i )
    {
        obj.emplace( key_name(i), Array{ i, 222, 33333, "abcdefg", true, false, Null{} } );
    }

    auto start = std::chrono::steady_clock::now();

    //cout << toplevel_obj << endl;

    int ensure_not_optimised_away = 0;
    for ( int i = 0; i < obj.size(); ++i )
    {
        const auto arr = get_value<Array>( obj, key_name( i ) );
        ASSERT_TRUE( arr );
        ensure_not_optimised_away += arr->get().size();
    }

    auto end = std::chrono::steady_clock::now();

    cout << "num arrays " << ensure_not_optimised_away << endl;
    cout << "read time " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ) << endl;
}