//
//  Exceptions.hpp
//  IniParser
//

#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <iostream>
#include <string>
#include <exception>

class IniParserException : public std::exception
{
public:
    IniParserException(const std::string& message) : message(message) {};

    const char* what() const override
    {
        return this->message.c_str();
    }

private:
    const std::string message;
};

class FileError : public IniParserException
{
public:
    FileError(const std::string& message) : IniParserException("File error: " + message) {};
};

class SyntaxError : public IniParserException
{
public:
    SyntaxError(const std::string& message) : IniParserException("Syntax error: " + message) {};
};

class BadCast : public IniParserException
{
public:
    BadCast(const std::string& message) : IniParserException("Bad cast parameter: " + message) {};
};

class ParamNotExist : public IniParserException
{
public:
    ParamNotExist(const std::string& message) : IniParserException("Param not exist: " + message) {};
};
#endif /* Exceptions_hpp */