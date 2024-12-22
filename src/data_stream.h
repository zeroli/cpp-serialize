#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>

namespace serialize
{
class DataStream
{
public:
    enum Type {
        BOOL = 0,
        CHAR,
        INT32,
        INT64,
        FLOAT,
        DOUBLE,
        STRING,
        CUSTOM,
    };

    DataStream() = default;
    ~DataStream() = default;

    size_t buf_size() const {
        return d_buf.size();
    }

    void write(const char* buf, size_t len);
    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const char* value);
    void write(const std::string& value);

    template <typename T>
    DataStream& operator << (const T& value) {
        write(value);
        return *this;
    }

    bool read(bool& value);
    bool read(char& value);
    bool read(int32_t& value);
    bool read(int64_t& value);
    bool read(float& value);
    bool read(double& value);
    bool read(std::string& value);

    template <typename T>
    DataStream& operator >> (T& value) {
        read(value);
        return *this;
    }
private:
    std::vector<uint8_t> d_buf;
    size_t d_read_pos {0};
};
}  // namespace serialize
