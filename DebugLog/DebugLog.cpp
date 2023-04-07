#include <iostream>

#include <vector>
#include <queue>
#include <string>

#include <cstdio>
#include <cwchar>
#include <cstdarg>

#include <Windows.h>

std::queue<std::wstring> _LoggerQueue;

void Append(const wchar_t *fmt, ...)
{
    va_list args;
    int len;

    // format the string
    va_start(args, fmt);
    len = _vscwprintf(fmt, args) + 1; // fmt len + \0 character

    std::vector<wchar_t> buffer(len); // resv the buffer

    // append the string to the _LoggerQueue
    vswprintf_s(&buffer[0], len, fmt, args);

    _LoggerQueue.push(std::wstring{ buffer.begin(), buffer.end() });

    va_end(args);
}

#define DEBUG_LOG(m, ...) Append(m, __VA_ARGS__)


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    AllocConsole();

    // output a sample log to the queue
    DEBUG_LOG(L"%s meets you at %d.", L"John", 42);

    // consume the queue
    while (!_LoggerQueue.empty())
    {
        DWORD bytesWritten;
        auto l = _LoggerQueue.front();

        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), l.c_str(), (DWORD)l.size(), &bytesWritten, 0);
        _LoggerQueue.pop();
    }
}