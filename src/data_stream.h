#pragma once

#include "serializable.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace serialize
{
class DataStream;

class DataStream
{
public:
    enum Type {
        /// primitive types
        BOOL = 0,
        CHAR,
        INT32,
        INT64,
        FLOAT,
        DOUBLE,
        STRING,
        /// STL containers
        VECTOR,
        LIST,
        MAP,
        SET,
        CUSTOM,
    };

    DataStream() = default;
    ~DataStream() = default;

    size_t buf_size() const {
        return d_buf.size();
    }

    /// write
    void write(const char* buf, size_t len);
    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const char* value);
    void write(const std::string& value);
    // write any serializable object
    void write(const Serializable& value) {
        value.serialize(*this);
    }
    // write any type that has serialize method defined
    template <typename T,
        std::enable_if_t<detail::has_serialize_v<T>>* = nullptr
    >
    void write(const T& value) {
        value.serialize(*this);
    }

    // write STL containers
    template <typename T>
    void write(const std::vector<T>& value) {
        write_type(Type::VECTOR);
        write_container(value);
    }
    template <typename T>
    void write(const std::list<T>& value) {
        write_type(Type::LIST);
        write_container(value);
    }
    template <typename K, typename V>
    void write(const std::map<K, V>& value) {
        write_type(Type::MAP);
        write_container(value);
    }
    template <typename T>
    void write(const std::set<T>& value) {
        write_type(Type::SET);
        write_container(value);
    }
    template <typename K, typename V>
    void write(const std::pair<K, V>& value) {
        write(value.first);
        write(value.second);
    }

    // write generic container which supports for-range based iteration
    template <typename C>
    void write_container(const C& cont);

    // write variadic arguments
    void write_args() { }
    template <typename T, typename... Args>
    void write_args(const T& value, Args&&... args) {
        write(value);
        write_args(std::forward<Args>(args)...);
    }

    template <typename T>
    DataStream& operator << (const T& value) {
        write(value);
        return *this;
    }

    /// read
    bool read(bool& value);
    bool read(char& value);
    bool read(int32_t& value);
    bool read(int64_t& value);
    bool read(float& value);
    bool read(double& value);
    bool read(std::string& value);
    // read any serializable object
    bool read(Serializable& value) {
        return value.deserialize(*this);
    }
    template <typename T,
        std::enable_if_t<detail::has_deserialize_v<T>>* = nullptr
    >
    bool read(T& value) {
        return value.deserialize(*this);
    }

    // read STL containers
    template <typename T>
    bool read(std::vector<T>& value) {
        if (!read_type(Type::VECTOR)) {
            return false;
        }
        return read_container(value);
    }
    template <typename T>
    bool read(std::list<T>& value) {
        if (!read_type(Type::LIST)) {
            return false;
        }
        return read_container(value);
    }
    template <typename K, typename V>
    bool read(std::map<K, V>& value) {
        if (!read_type(Type::MAP)) {
            return false;
        }
        return read_container(value);
    }
    template <typename T>
    bool read(std::set<T>& value) {
        if (!read_type(Type::SET)) {
            return false;
        }
        return read_container(value);
    }
    template <typename K, typename V>
    bool read(std::pair<K, V>& value) {
        return read(value.first) && read(value.second);
    }

    // read generic container
    template <typename C>
    bool read_container(C& cont);
    // specialized map container read
    template <typename K, typename V>
    bool read_container(std::map<K, V>& cont);
    // specialized set container read
    template <typename T>
    bool read_container(std::set<T>& cont);

    // read variadic arguments
    bool read_args() { return true; }
    template <typename T, typename... Args>
    bool read_args(T& value, Args&&... args) {
        return read(value) &&
            read_args(std::forward<Args>(args)...);
    }

    template <typename T>
    DataStream& operator >> (T& value) {
        read(value);
        return *this;
    }

    void write_type(int8_t type) {
        write((const char*)&type, sizeof(int8_t));
    }
    bool read_type(int8_t type) {
        if (d_buf[d_read_pos] != type) {
            return false;
        }
        d_read_pos++;
        return true;
    }
private:
    bool read_len(int32_t& len) {
        len = (int)(*(int*)&d_buf[d_read_pos]);
        if (len < 0) {
            return false;
        }
        d_read_pos += sizeof(len);
        return true;
    }
private:
    std::vector<uint8_t> d_buf;
    size_t d_read_pos {0};
};

template <typename C>
void DataStream::write_container(const C& cont)
{
    int len = (int)cont.size();
    write((const char*)&len, sizeof(len));
    for (auto&& v : cont) {
        write(v);
    }
}

template <typename C>
bool DataStream::read_container(C& cont)
{
    int len = 0;
    if (!read_len(len)) {
        return false;
    }
    using value_type = typename C::value_type;
    for (int i = 0; i < len; i++) {
        value_type value;
        read(value);
        cont.emplace(cont.end(), std::move(value));
    }
    return true;
}

template <typename K, typename V>
bool DataStream::read_container(std::map<K, V>& cont)
{
    int len = 0;
    if (!read_len(len)) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        K k;
        V v;
        if (!read(k) || !read(v)) {
            return false;
        }
        cont.emplace(std::move(k), std::move(v));
    }
    return true;
}

template <typename T>
bool DataStream::read_container(std::set<T>& cont)
{
    int len = 0;
    if (!read_len(len)) {
        return false;
    }
    using value_type = T;
    for (int i = 0; i < len; i++) {
        value_type value;
        read(value);
        cont.emplace(std::move(value));
    }
    return true;
}

}  // namespace serialize
