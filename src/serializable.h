#pragma once

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

}  // namespace serialize
