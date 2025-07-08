// Distributed under the MIT License, see accompanying file LICENSE.txt
// Copyright John W. Wilkinson 2025

#include "simple_json.h"
#include <charconv>
#include <iostream>
#include <optional>

using namespace simple_json;
using namespace std;

namespace
{
    class Parser
    {
      public:
        Parser( const string& json_str )
            : posn_( json_str.begin() ),
              end_( json_str.end() )
        {
        }

        std::expected<Value, std::string> parse_completely()
        {
            auto result = parse_value();

            if ( result )
            {
                // Check that we have consumed the entire input string
                skip_whitespace();
                if ( posn_() != end_ )
                {
                    return std::unexpected( "unprocessed data" + where() );
                }
            }

            return result;
        }

      private:
        std::expected<Value, std::string> parse_value()
        {
            skip_whitespace();

            if ( posn_() == end_ )
            {
                return std::unexpected( "end of string reached while looking for value" + where() );
            }
            if ( *posn_() == '{' )
            {
                return parse_object();
            }
            if ( *posn_() == '[' )
            {
                return parse_array();
            }
            if ( *posn_() == '"' )
            {
                return parse_string();
            }
            if ( *posn_() == 't' )
            {
                return parse_true();
            }
            if ( *posn_() == 'f' )
            {
                return parse_false();
            }
            if ( *posn_() == 'n' )
            {
                return parse_null();
            }
            if ( isdigit( *posn_() ) || *posn_() == '-' )
            {
                return parse_integer();
            }
            return std::unexpected( string( "unexpected character '" ) + *posn_() + "'" + where() );
        }

        std::expected<Array, std::string> parse_array()
        {
            Array arr;

            posn_.incr(); // skip opening '['

            skip_whitespace();

            if ( posn_() == end_ )
            {
                return std::unexpected( "missing closing ']'" + where() );
            }

            if ( *posn_() == ']' )
            {
                posn_.incr(); // end of object, skip closing ']'
                return arr;
            }

            while ( true )
            {
                expected<Value, string> value = parse_value();

                if ( !value )
                {
                    return std::unexpected( value.error() );
                }

                arr.push_back( *value );

                skip_whitespace();

                if ( posn_() == end_ )
                {
                    return std::unexpected( "missing closing ']'" + where() );
                }

                if ( *posn_() == ']' )
                {
                    posn_.incr();
                    break; // end of array
                }

                if ( *posn_() == ',' )
                {
                    posn_.incr(); // skip ','
                }
                else
                {
                    return std::unexpected( string( "unexpected character '" ) + *posn_() + "'" + where() );
                }
            }
            return arr;
        }

        std::expected<Object, std::string> parse_object()
        {
            Object obj;

            posn_.incr(); // skip opening '{'

            while ( true )
            {
                skip_whitespace();

                if ( posn_() == end_ )
                {
                    return std::unexpected( "missing closing '}'" + where() );
                }

                if ( *posn_() == '}' )
                {
                    posn_.incr();
                    break; // end of object
                }

                if ( *posn_() == '"' )
                {
                    expected<Object::value_type, string> pair = parse_pair();
                    if ( !pair )
                    {
                        return std::unexpected( pair.error() );
                    }

                    obj.insert( *pair );
                }
                else if ( *posn_() == ',' )
                {
                    posn_.incr(); // skip ','
                }
                else
                {
                    return std::unexpected( string( "unexpected character '" ) + *posn_() + "'" + where() );
                }
            }

            return obj;
        }

        void skip( int ( *pred )( int ) )
        {
            for ( ; posn_() != end_; posn_.incr() )
            {
                if ( !pred( *posn_() ) )
                {
                    break;
                }
            }
        }

        void skip_whitespace()
        {
            skip( std::isspace );
        }

        expected<Object::value_type, string> parse_pair()
        {
            expected<string, string> name = parse_string();

            if ( !name )
            {
                return std::unexpected( name.error() );
            }

            skip_whitespace();

            if ( posn_() == end_ || *posn_() != ':' )
            {
                return std::unexpected( "missing ':'" + where() );
            }

            posn_.incr();

            skip_whitespace();

            if ( posn_() == end_ )
            {
                return std::unexpected( "end of string reached while looking for second of pair" + where() );
            }

            // Create and return a pair with the parsed name and value.
            // Note if parse_value() fails, the error will be propagated.
            return parse_value().transform( [ & ]( const auto& value ) {
                return Object::value_type{ *name, value };
            } );
        }

        expected<string, string> parse_string()
        {
            posn_.incr(); // Skip the opening '"'

            string result;

            bool prev_esc = false;

            for ( ; posn_() != end_; posn_.incr() ) // we don't want to skip whitespace here
            {
                if ( prev_esc )
                {
                    const char* alph_esc_chars = "bfnrt\"\\/";     // alphabetic escape characters
                    const char* bin_esc_chars = "\b\f\n\r\t\"\\/"; // their binary equivalents

                    const char* esc_pos = strchr( alph_esc_chars, *posn_() );
                    if ( esc_pos == nullptr )
                    {
                        return std::unexpected( string( "invalid escape character '\\" ) + *posn_() + "'" + where() );
                    }

                    result.push_back( bin_esc_chars[ esc_pos - &alph_esc_chars[ 0 ] ] );

                    prev_esc = false;
                }
                else if ( *posn_() == '"' )
                {
                    posn_.incr(); // Skip the closing '"'

                    return result;
                }
                else if ( *posn_() == '\\' )
                {
                    prev_esc = true;
                }
                else
                {
                    result.push_back( *posn_() );
                }
            }

            return std::unexpected( "missing closing '\"'" + where() );
        }

        expected<int64_t, string> parse_integer()
        {
            const string::const_iterator int_start = posn_();

            posn_.incr(); // Skip the first character, possibly a '-'

            skip( isdigit );

            const string::const_iterator int_end = posn_();

            int64_t value;
            auto [ ptr, ec ] = std::from_chars( &*int_start, &*int_start + ( int_end - int_start ), value );
            if ( ec == std::errc() )
            {
                return value;
            }

            return std::unexpected( "could not convert \"" + string( int_start, int_end ) + "\" to an integer" + where() );
        }

        expected<void, string> parse( const string& word )
        {
            const size_t len = word.length();
            if ( end_ - posn_() >= len && string( posn_(), posn_() + len ) == word )
            {
                posn_.incr( len ); // Skip word
                return {};
            }
            return std::unexpected( "expected \"" + word + "\"" + where() );
        }

        expected<bool, string> parse_true()
        {
            return parse( "true" ).transform( []() { return true; } ); // if parse_value() fails, the error will be propagated.
        }

        expected<bool, string> parse_false()
        {
            return parse( "false" ).transform( []() { return false; } ); // if parse_value() fails, the error will be propagated.
        }

        expected<Null, string> parse_null()
        {
            return parse( "null" ).transform( []() { return Null(); } ); // if parse_value() fails, the error will be propagated.
        }

        string where() const
        {
            return posn_.where();
        }

        // Helper class to keep track of the current position in the input string
        // and the line and column numbers.
        // This is used to provide better error messages.
        //
        class Position
        {
          public:
            Position( const string::const_iterator& start )
                : iter_( start )
            {
            }

            void incr()
            {
                if ( *iter_ == '\n' )
                {
                    ++line_;
                    column_ = 0;
                }
                else
                {
                    ++column_;
                }
                ++iter_;
            }

            void incr( size_t num_chars )
            {
                for ( int i = 0; i < num_chars; ++i )
                {
                    incr();
                }
            }

            string::const_iterator operator()() const
            {
                return iter_;
            }

            string where() const
            {
                return " at line " + to_string( line_ + 1 ) + " column " + to_string( column_ + 1 );
            }

          private:
            string::const_iterator iter_;
            int line_ = 0;
            int column_ = 0;
        };

        Position posn_;              // Current position in the input string
        string::const_iterator end_; // End of the input string
    };
} // namespace

expected<Value, string> simple_json::parse( const std::string& json_str )
{
    return Parser( json_str ).parse_completely();
}

namespace
{
    // Formatter class to format the Object as a JSON string
    //
    class Formatter
    {
      public:
        Formatter( std::ostream& os, const Value& value )
            : os_( os )
        {
            format( value, 0 );
        }

      private:
        void format( const Object::value_type& member, const int level ) const
        {
            format( member.first );

            os_ << " : ";

            format( member.second, level + 1 );
        }

        void format( const string& s ) const
        {
            const char* alph_esc_chars = "bfnrt";     // alphabetic escape characters
            const char* bin_esc_chars = "\b\f\n\r\t"; // their binary equivalents

            os_ << '"';
            
            for ( const char c : s )
            {
                if ( c == '"' || c == '\\' || c == '/' )
                {
                    os_ << '\\' << c; // escape quotes and backslashes
                }
                else if ( const char* pos = strchr( bin_esc_chars, c ) ) // non-printable characters
                {
                    os_ << '\\' << alph_esc_chars[ pos - &bin_esc_chars[ 0 ] ];
                }
                else
                {
                    os_ << c; // printable character
                }
            }
                
            os_ << '"';
        }

        void format( const Object& obj, int level ) const
        {
            os_ << "{\n";

            int first = true;
            for ( const auto& member : obj )
            {
                if ( first )
                {
                    first = false;
                }
                else
                {
                    os_ << ",\n";
                }

                indent( level + 1 );

                format( member, level );
            }

            os_ << '\n';
            indent( level );
            os_ << '}';
        }

        void format( const Array& arr, int level ) const
        {
            os_ << "[\n";

            indent( level + 1 );

            int first = true;
            for ( const auto& value : arr )
            {
                if ( first )
                {
                    first = false;
                }
                else
                {
                    os_ << ", ";
                }

                format( value, level + 1 );
            }

            os_ << '\n';
            indent( level );
            os_ << ']';
        }

        void format( const Value& value, const int level ) const
        {
            struct Visitor
            {
                const Formatter* formatter;
                int level;
                void operator()( const string& s ) const
                {
                    formatter->format( s );
                }
                void operator()( const Object& obj ) const
                {
                    formatter->format( obj, level );
                }
                void operator()( const Array& arr ) const
                {
                    formatter->format( arr, level );
                }
                void operator()( int64_t i ) const
                {
                    formatter->os_ << i;
                }
                void operator()( bool b ) const
                {
                    formatter->os_ << ( b ? "true" : "false" );
                }
                void operator()( const Null& ) const
                {
                    formatter->os_ << "null";
                }
            };

            std::visit( Visitor{ this, level }, value );
        }

        void indent( int level ) const
        {
            for ( int i = 0; i < level; ++i )
            {
                os_ << "    ";
            }
        }
        std::ostream& os_;
    };
} // namespace

std::ostream&
simple_json::operator<<( std::ostream& os, const simple_json::Value& value )
{
    Formatter( os, value );
    return os;
}
