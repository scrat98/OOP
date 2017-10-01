//
//  Parser.cpp
//  IniParser
//

#include "Parser.hpp"

boost::regex IniParser::getRegex() const
{
    return boost::regex("^(?<empty>\\s*)$|" 
                        "^\\s*(?<comment>;.*)$|" 
                        "^\\s*\\[(?<section>\\w+)\\]\\s*(;.*)?$|"
                        "^\\s*(?<key>\\w+)\\s*=\\s*(?<value>[^;\\s]*)\\s*(;.*)?$|"
                        "^(?<error>.*)$");
}

void IniParser::Parse(const std::string& filePath) throw(IniParserException)
{
    std::ifstream file(filePath);
    Parse(file);
}

void IniParser::Parse(std::istream& content) throw(IniParserException)
{
    if (!content)
        throw FileError("Content can't be opened");

    currentlineNumber = 0;
    while (std::getline(content, currentLine)) {
        currentlineNumber++;
        boost::regex_search(currentLine, what, regex);
        execute(what);
    }
}

void IniParser::execute(const boost::smatch& what) throw(SyntaxError)
{
    checkSection(what);
    checkParam(what);
    checkError(what);
}

void IniParser::checkSection(const boost::smatch& what) noexcept
{
    if (what["section"].matched) {
        currentSection = what["section"];
    }
}

void IniParser::checkParam(const boost::smatch& what) throw(SyntaxError)
{
    if (what["key"].matched && what["value"].matched) {
        if (currentSection.empty())
            throw SyntaxError("Section not found to add param");

        addNewParam(currentSection, what["key"], what["value"]);
    }
}


void IniParser::checkError(const boost::smatch& what) throw(SyntaxError)
{
    if (what["error"].matched) {
        throw SyntaxError("line " + std::to_string(currentlineNumber) + " falied");
    }
}

void IniParser::addNewParam(const std::string& section, const std::string& key, const std::string& value) noexcept
{
    this->data[section][key] = value;
}

bool IniParser::IsHaveSection(const std::string& sectionName) const noexcept
{
    return this->data.count(sectionName);
}

bool IniParser::IsHaveParam(const std::string& sectionName, const std::string& paramName) const noexcept
{
    return
        this->data.count(sectionName)
        ? data.at(sectionName).count(paramName) 
        : false;
}

template <>
std::string IniParser::getValue<std::string>(const std::string& sectionName, const std::string& paramName) const throw(BadCast, ParamNotExist)
{
    if (!IsHaveParam(sectionName, paramName))
        throw ParamNotExist("Section: " + sectionName + " " + "Param: " + paramName);

    return this->data.at(sectionName).at(paramName);
}

template<>
int IniParser::getValue<int>(const std::string& sectionName, const std::string& paramName) const throw(BadCast, ParamNotExist)
{
    if (!IsHaveParam(sectionName, paramName))
        throw ParamNotExist("Section: " + sectionName + " " + "Param: " + paramName);

    return std::stoi(this->data.at(sectionName).at(paramName));
}

template<>
float IniParser::getValue<float>(const std::string& sectionName, const std::string& paramName) const throw(BadCast, ParamNotExist)
{
    if (!IsHaveParam(sectionName, paramName))
        throw ParamNotExist("Section: " + sectionName + " " + "Param: " + paramName);

     return std::stof(this->data.at(sectionName).at(paramName));
}

template<>
double IniParser::getValue<double>(const std::string& sectionName, const std::string& paramName) const throw(BadCast, ParamNotExist)
{
    if (!IsHaveParam(sectionName, paramName))
        throw ParamNotExist("Section: " + sectionName + " " + "Param: " + paramName);

    return std::stod(this->data.at(sectionName).at(paramName));
}