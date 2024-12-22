#include "data_stream.h"

#include <cstring>
#include <iostream>

namespace serialize {
void DataStream::write(const char* buf, size_t len)
{
    size_t pos = d_buf.size();
    d_buf.resize(pos + len);
    std::memcpy(&d_buf[pos], buf, len);
}

void DataStream::write(bool value)
{
    write_type(Type::BOOL);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(char value)
{
    write_type(Type::CHAR);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(int32_t value)
{
    write_type(Type::INT32);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(int64_t value)
{
    write_type(Type::INT64);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(float value)
{
    write_type(Type::FLOAT);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(double value)
{
    write_type(Type::DOUBLE);
    write((const char*)&value, sizeof(value));
}

void DataStream::write(const char* value)
{
    write_type(Type::STRING);
    int len = strlen(value);
    write((const char*)&len, sizeof(len));
    write((const char*)value, len);
}

void DataStream::write(const std::string& value)
{
    write_type(Type::STRING);
    int len = (int)value.size();
    write((const char*)&len, sizeof(len));
    write((const char*)&value[0], len);
}

bool DataStream::read(bool& value)
{
    if (!read_type(Type::BOOL)) {
        return false;
    }
    value = (bool)d_buf[d_read_pos];
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(char& value)
{
    if (!read_type(Type::CHAR)) {
        return false;
    }
    value = (char)d_buf[d_read_pos];
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(int32_t& value)
{
    if (!read_type(Type::INT32)) {
        return false;
    }
    value = (int32_t)(*(int32_t*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(int64_t& value)
{
    if (!read_type(Type::INT64)) {
        return false;
    }
    value = (int64_t)(*(int64_t*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(float& value)
{
    if (!read_type(Type::FLOAT)) {
        return false;
    }
    value = (float)(*(float*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(double& value)
{
    if (!read_type(Type::DOUBLE)) {
        return false;
    }
    value = (double)(*(double*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(std::string& value)
{
    if (!read_type(Type::STRING)) {
        return false;
    }
    int len = 0;
    if (!read_len(len)) {
        return false;
    }
    value.assign((char*)&d_buf[d_read_pos], len);
    d_read_pos += len;
    return true;
}
}  // namespace serialize
