Token parse(const std::string &code) {
    std::vector<Token> tokens = lex(code);
    // std::cout << tokens << std::endl;
    return parse(tokens);
}

#include <chrono>

using namespace std::chrono;

int main() {
    std::string code = "";
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

    microseconds duration(0);
    for (int i = 0; i < 100; i++) {
        auto start = high_resolution_clock::now();
        try {
            Token res = parse(code);
            // std::cout << "OK" << std::endl;
            // std::cout << res;
        } catch (syntax_error &e) {
            std::cout << "ERROR AT :" << e.get_error_token() << std::endl;
        }
        auto stop = high_resolution_clock::now();
        duration += duration_cast<microseconds>(stop - start);
    }
    std::cout << "Time taken by function: " << duration_cast<milliseconds>(duration).count()
              << " ms" << std::endl;

    return 0;
}