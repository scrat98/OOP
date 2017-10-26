//
//  WavException.hpp
//  Wav exceptions
//  Laba3 OOP
//

#ifndef WavException_hpp
#define WavException_hpp

#include <iostream>
#include <string>
#include <exception>

class WavReaderException : public std::exception
{
public:
    WavReaderException(const std::string& message) : message(message) {};

    const char* what() const override
    {
        return this->message.c_str();
    }

private:
    const std::string message;
};

class FileError : public WavReaderException
{
public:
    FileError(const std::string& message) : WavReaderException("File error: " + message) {};
};

class BadFormat : public WavReaderException
{
public:
    BadFormat(const std::string& message) : WavReaderException("Bad format: " + message) {};
};

class UnsupportedFormat : public WavReaderException
{
public:
    UnsupportedFormat(const std::string& message) : WavReaderException("Unsupported format : " + message) {};
};

class BadParams : public WavReaderException
{
public:
    BadParams(const std::string& message) : WavReaderException("Bad params : " + message) {};
};

class DataSizeError : public WavReaderException
{
public:
    DataSizeError(const std::string& message) : WavReaderException("Data size error : " + message) {};
};

class HeaderError : public WavReaderException
{
public:
    HeaderError(const std::string& message) : WavReaderException("Header error : " + message) {};
};

#endif /* WavException_hpp */