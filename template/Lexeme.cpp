struct Lexeme {
    /**
     * @brief Lexeme struct
     * @details This struct represents a lexeme: its name and its checking regex
     */
    const char *name;
    const std::regex &regex;
};
