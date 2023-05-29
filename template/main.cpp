Token parse(const std::string &code) {
    std::vector<Token> tokens = lex(code);
    // std::cout << tokens << std::endl;
    return parse(tokens);
}

#include <chrono>
#include <fstream>

using namespace std::chrono;

int main(int argc, char const **argv) {
    std::string code = "";
    if (argc == 1) {
        code = "";
        for (char letter = 'a'; letter <= 'z'; letter++) {
            code += letter;
            code += " = ";
            for (char letter2 = 'a'; letter2 < letter; letter2++) {
                code += letter2;
                code += " + ";
            }
            code += "1\n";
        }
        // std::cout << code;
    } else if (argc == 2) {
        std::ifstream t(argv[1]);
        code = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    } else {
        std::cout << "Usage: " << argv[0] << " [file]" << std::endl;
        return 1;
    }

    microseconds duration(0);
    for (int i = 0; i < 100; i++) {
        auto start = high_resolution_clock::now();
        try {
            Token res = parse(code);
            // std::cout << "OK" << std::endl;
            // std::cout << res;
        } catch (syntax_error &e) {
            std::cout << "ERROR AT :" << e.get_error_token() << std::endl;
            return 2;
        }
        auto stop = high_resolution_clock::now();
        duration += duration_cast<microseconds>(stop - start);
    }
    std::cout << "Time taken by function: " << duration_cast<milliseconds>(duration).count()
              << " ms" << std::endl;

    return 0;
}
