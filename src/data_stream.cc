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
    int8_t type = Type::BOOL;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(char value)
{
    int8_t type = Type::CHAR;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(int32_t value)
{
    int8_t type = Type::INT32;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(int64_t value)
{
    int8_t type = Type::INT64;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(float value)
{
    int8_t type = Type::FLOAT;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(double value)
{
    int8_t type = Type::DOUBLE;
    write((const char*)&type, sizeof(int8_t));
    write((const char*)&value, sizeof(value));
}

void DataStream::write(const char* value)
{
    int8_t type = Type::STRING;
    write((const char*)&type, sizeof(int8_t));
    int len = strlen(value);
    write((const char*)&len, sizeof(len));
    write((const char*)value, len);
}

void DataStream::write(const std::string& value)
{
    int8_t type = Type::STRING;
    write((const char*)&type, sizeof(int8_t));
    int len = (int)value.size();
    write((const char*)&len, sizeof(len));
    write((const char*)&value[0], len);
}

bool DataStream::read(bool& value)
{
    if (d_buf[d_read_pos] != Type::BOOL) {
        return false;
    }
    ++d_read_pos;
    value = (bool)d_buf[d_read_pos];
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(char& value)
{
    if (d_buf[d_read_pos] != Type::CHAR) {
        return false;
    }
    ++d_read_pos;
    value = (char)d_buf[d_read_pos];
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(int32_t& value)
{
    if (d_buf[d_read_pos] != Type::INT32) {
        return false;
    }
    ++d_read_pos;
    value = (int32_t)(*(int32_t*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(int64_t& value)
{
    if (d_buf[d_read_pos] != Type::INT64) {
        return false;
    }
    ++d_read_pos;
    value = (int64_t)(*(int64_t*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(float& value)
{
    if (d_buf[d_read_pos] != Type::FLOAT) {
        return false;
    }
    ++d_read_pos;
    value = (float)(*(float*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(double& value)
{
    if (d_buf[d_read_pos] != Type::DOUBLE) {
        return false;
    }
    ++d_read_pos;
    value = (double)(*(double*)(&d_buf[d_read_pos]));
    d_read_pos += sizeof(value);
    return true;
}

bool DataStream::read(std::string& value)
{
    if (d_buf[d_read_pos] != Type::STRING) {
        return false;
    }
    ++d_read_pos;
    int len = (int)(*(int*)&d_buf[d_read_pos]);
    if (len < 0) {
        return false;
    }
    d_read_pos += sizeof(len);
    value.assign((char*)&d_buf[d_read_pos], len);
    d_read_pos += len;
    return true;
}
}  // namespace serialize
