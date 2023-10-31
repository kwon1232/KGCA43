#pragma once
#include <iostream> 
#include <vector> 
#include <list> 
#include <map> 
#include <string> 
#include <atlconv.h> // A2W

using namespace std;

static std::wstring to_mw(const std::string& _src)
{
    USES_CONVERSION;
    return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
    USES_CONVERSION;
    return std::string(W2A(_src.c_str()));
};

using C_STR = std::basic_string<char>;  //  ��Ƽ����Ʈ(����:1����Ʈ, �ѱ�:2����Ʈ) 
using W_STR = std::basic_string<wchar_t>;// �����ڵ�(������ 2����Ʈ�� ���� ǥ��)
