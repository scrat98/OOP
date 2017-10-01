#include "Parser.hpp"

int main() {
    IniParser parser;

    try {
        parser.Parse("../test/lab01_input_correct_09.ini");

        // std::cout << parser.IsHaveParam("asfasfas", "asfsafas") << std::endl;
        std::cout << parser.IsHaveParam("SECTION2", "MaxWaitTimeMs") << std::endl;
        std::cout << parser.IsHaveSection("DEMODS") << std::endl;

        // std::cout << parser.getValue<int>("DEMODS", "DefaultDemod") << std::endl;
        std::cout << parser.getValue<double>("DEMODS", "StartDataPort") << std::endl;
    }
    catch(const std::exception& e) {
        std::cout << e.what();
    }

    system("pause");
    return 0;
}