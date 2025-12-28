This is a simple JSON parse writter in C++. A JSON value is a std\:\:variant, a JSON array is an std\:\:vector and a JSON object is an std\:\:map. It used the C++23 std::expected to return parsing error messages.

It does not support real numbers, only integers. No unicode support.

# Example usage

Using the struct Student defined below:
```cpp
    struct Student
    {
        string name;
        int age;
        std::vector<int> grades;
    };
```

Writting a Student object to JSON:

```cpp
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
```

The function below reads a student object from a JSON string:

```cpp
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
```

You would use the function like this:

```cpp
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
```

We can also test error handling:
```cpp
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
```