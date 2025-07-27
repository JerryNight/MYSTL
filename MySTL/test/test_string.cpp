#include <gtest/gtest.h>
#include <iostream>
#include "../string.h"
#include "cstring"

using mystl::string;

const char* ch = "liyongtao";

TEST(StringTest, ConstructAndDestroy1)
{
    // 构造
    string s1("liyongtao");
    string s2(ch, 6);
    string s3(ch, ch + 6);
    EXPECT_EQ(0, strcmp(s3.c_str(), s2.c_str()));
}

TEST(StringTest, ConstructAndDestroy2)
{
    // 重载+
    string s(ch);
    string s4 = "hello";
    string s5 = s4 + ' ' + s;
    string s6 = "hello liyongtao";
    EXPECT_EQ(s5, s6);
}

TEST(StringTest, ConstructAndDestroy3)
{
    // 拷贝构造
    string s1(ch);
    string s2(s1);
    EXPECT_EQ(0, strcmp(s1.c_str(),s2.c_str()));
}

TEST(StringTest, ConstructAndDestroy4)
{
    // 空字符串
    string s8;
    EXPECT_EQ(0, strcmp("", s8.c_str()));
}
TEST(StringTest, ConstructAndDestroy5)
{
    // length size
    string s(ch);
    EXPECT_EQ(s.length(), strlen(ch));
    EXPECT_EQ(s.size(), strlen(ch));
}
TEST(StringTest, ConstructAndDestroy6)
{
    // push_back pop_back
    string s(ch);
    s.push_back('L');
    s.push_back('Y');
    s.push_back('T');
    EXPECT_EQ(0, strcmp(s.c_str(), "liyongtaoLYT"));
    s.pop_back();
    s.pop_back();
    s.pop_back();
    EXPECT_EQ(0, strcmp(s.c_str(), ch));
}
TEST(StringTest, ConstructAndDestroy7)
{
    const char ch1[] = "liyOngtaO";
    const char ch2[] = "liyngta";
    // substr replace remove
    string s(ch);
    string s1("tao");
    EXPECT_EQ(0, strcmp(s1.c_str(), s.substr(6).c_str()));
    //std::cout << ":before replace: " << s << std::endl;
    //std::cout << ":before replace: " << s.c_str() << std::endl;
    EXPECT_EQ(0, strcmp(s.c_str(),ch));
    s.replace('o','O');
    //std::cout << ":after replace: " << s.c_str() << std::endl;
    EXPECT_EQ(0, strcmp(s.c_str(),ch1));
    s.remove('O');
    //std::cout << ":after remove: " << s.c_str() << std::endl;
    EXPECT_EQ(0, strcmp(s.c_str(),ch2));
}
TEST(StringTest, ConstructAndDestroy8)
{
    // find  count
    string s(ch);
    EXPECT_EQ(2, s.find("yong"));
    EXPECT_EQ(2, s.count('o'));
}
