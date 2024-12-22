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

## custom type serialization
there are 2 ways to support custom type serialization
- derive custom type from interface class `Serializable` and implement the interfaces
  ```c++
  #include "data_stream.h"
  class CustomType : public Serializable {
    virtual void serialize(DataStream& ds) const = 0;
    virtual bool deserialize(DataStream& ds) = 0;
    //...
  };
  ```
  or embed below macro into the custom class body
  ```c++
  #include "data_stream.h"
  class CustomType : public Serializable {
    SERIALIZE(data1, data2, ...);
    //...
  };
  ```
- any custom type that defines below pair of methods
  ```c++
  class CustomType {
  public:
    template <typename DS>
    void serialize(DS& ds) const {
        ds.write_type(DS::Type::CUSTOM);
        ds << data1 << data2 << ...;
    }
    template <typename DS>
    bool deserialize(DS& ds) const {
        if (!ds.read_type(DS::Type::CUSTOM)) {
            return false;
        }
        ds >> data1 >> data2 >> ...;
        return true;
    }
  };

  ```
  Refer to unit_test/* for examples
