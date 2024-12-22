#pragma once

#include <type_traits>

namespace serialize {
class DataStream;

// abstract interface of any class that wants to serialize data via class DataStream
class Serializable {
public:
    virtual void serialize(DataStream& ds) const = 0;
    virtual bool deserialize(DataStream& ds) = 0;
};

/// add this macro to any subclass with those data fields to be serialized/deserialized
#define SERIALIZE(...) \
    virtual void serialize(DataStream& ds) const override { \
        ds.write_type(DataStream::Type::CUSTOM); \
        ds.write_args(__VA_ARGS__);          \
    } \
    virtual bool deserialize(DataStream& ds) override { \
        if (!ds.read_type(DataStream::Type::CUSTOM)) { \
            return false; \
        } \
        return ds.read_args(__VA_ARGS__); \
    } \
///###

// another approach to support custom type to serialize
// as long as it has `serialize` and `deserialize` method defined
// ```c++
//  template <typename DS>
//  void serialize(DS& ds) const;
//  template <typename DS>
//  bool deserialize(DS& ds);
// ```

namespace detail {
template <typename T,
    typename Enable = decltype(std::declval<T>().serialize(std::declval<DataStream&>))>
std::true_type  has_serialize_helper(int);

template <typename...>
std::false_type has_serialize_helper(...);

/// constexpr boolean value to work with std::enable_if_t traits
/// to enable function overloading
template <typename T>
constexpr bool has_serialize_v = decltype(has_serialize_helper<T>(0))::value;

template <typename T,
    typename Enable = decltype(std::declval<T>().deserialize(std::declval<DataStream&>))>
std::true_type  has_deserialize_helper(int);

template <typename...>
std::false_type has_deserialize_helper(...);

/// constexpr boolean value to work with std::enable_if_t traits
/// to enable function overloading
template <typename T>
constexpr bool has_deserialize_v = decltype(has_deserialize_helper<T>(0))::value;
}  // namespace detail

}  // namespace serialize
