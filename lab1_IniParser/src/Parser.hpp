//
//  Parser.hpp
//  IniParser
//

#ifndef Parser_hpp
#define Parser_hpp

#include "Exceptions.hpp"
#include <boost/regex.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

class IniParser final {
public:
    IniParser() {};
    ~IniParser() {};

    void Parse(std::istream& content) throw(IniParserException);
    void Parse(const std::string& filePath) throw(IniParserException);

    bool IsHaveSection(const std::string& sectionName) const noexcept;
    bool IsHaveParam(const std::string& sectionName, const std::string& paramName) const noexcept;

    template<typename T>
    T getValue(const std::string& sectionName, const std::string& paramName) const throw(BadCast, ParamNotExist);

private:
    void execute(const boost::smatch& what) throw(SyntaxError);
    void addNewParam(const std::string& section, const std::string& key, const std::string& value) noexcept;
    boost::regex getRegex() const;

    void checkSection(const boost::smatch& what) noexcept;
    void checkParam(const boost::smatch& what) throw(SyntaxError);
    void checkError(const boost::smatch& what) throw(SyntaxError);

    boost::regex regex = getRegex();
    boost::smatch what;
    std::string currentLine;
    int currentlineNumber;
    std::string currentSection;

    std::map<std::string, std::map<std::string, std::string>> data;
};
#endif /* Parser_hpp */