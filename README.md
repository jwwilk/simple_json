This is a simple JSON parse writter in C++. A JSON value is a std::variant, a JSON array is an std::vector and a JSON object is an std::map. It used the C++23 std::expected to return parsing error messages.

It does not support real numbers, only integers. No unicode support.
