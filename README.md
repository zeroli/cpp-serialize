## Introduction

This is a simple C++ serialization library. It supports basic C++ data type serialization and deserialization: bool, char, int32_t, int64_t, float, double, c-string, std::string

## encoding of data types
| data type | encoding |
| -- | -- |
| bool | type(1B) + data(1B) |
| char | type(1B) + data(1B) |
| int32_t | type(1B) + data(4B) |
| int64_t | type(1B) + data(8B) |
| float | type(1B) + data(4B) |
| double | type(1B) + data(8B) |
| string | type(1B) + len(4B) + data(variadic len) |

## Usage
```c++
serialize::DataStream ds;
ds << true; ds >> value;
ds << 'x'; ds >> value;
ds << 123; ds >> value;
ds << 1234LL; ds >> value;
ds << 123.f; ds >> value;
ds << 123.0; ds >> value;
ds << "hello"; ds >> value;
```
