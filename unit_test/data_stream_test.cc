#include <gtest/gtest.h>

#include "data_stream.h"

using namespace serialize;

TEST(data_stream, test_write_types)
{
    {
        DataStream ds;
        ds << true;
        EXPECT_EQ(2, ds.buf_size());
    }
    {
        DataStream ds;
        ds << 'x';
        EXPECT_EQ(2, ds.buf_size());
    }
    {
        DataStream ds;
        ds << 123;
        EXPECT_EQ(5, ds.buf_size());
    }
    {
        DataStream ds;
        ds << (int64_t)123;
        EXPECT_EQ(9, ds.buf_size());
    }
    {
        DataStream ds;
        ds << 123.f;
        EXPECT_EQ(5, ds.buf_size());
    }
    {
        DataStream ds;
        ds << 123.0;
        EXPECT_EQ(9, ds.buf_size());
    }
    {
        DataStream ds;
        ds << "hello";
        EXPECT_EQ(1+4+5, ds.buf_size());
    }
}

TEST(data_stream, test_write_types2)
{
    {
        DataStream ds;
        ds << true;
        EXPECT_EQ(0+2, ds.buf_size());
        ds << 'x';
        EXPECT_EQ(0+2+2, ds.buf_size());
        ds << 123;
        EXPECT_EQ(0+2+2+5, ds.buf_size());
        ds << (int64_t)123;
        EXPECT_EQ(0+2+2+5+9, ds.buf_size());
        ds << 123.f;
        EXPECT_EQ(0+2+2+5+9+5, ds.buf_size());
        ds << 123.0;
        EXPECT_EQ(0+2+2+5+9+5+9, ds.buf_size());
        ds << "hello";
        EXPECT_EQ(0+2+2+5+9+5+9+(1+4+5), ds.buf_size());
    }
}

TEST(data_stream, test_read)
{
    DataStream ds;
    ds << true << 'x' << 123 << (int64_t)1234 << 123.f << 123.4 << "hello world";

    bool val1;
    char val2;
    int32_t val3;
    int64_t val4;
    float val5;
    double val6;
    std::string val7;
    ds >> val1 >> val2 >> val3 >> val4 >> val5 >> val6 >> val7;
    EXPECT_EQ(true, val1);
    EXPECT_EQ('x', val2);
    EXPECT_EQ(123, val3);
    EXPECT_EQ(1234, val4);
    EXPECT_FLOAT_EQ(123.f, val5);
    EXPECT_FLOAT_EQ(123.4, val6);
    EXPECT_EQ("hello world", val7);
}
