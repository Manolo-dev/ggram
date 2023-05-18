#include "error.hpp"
#include <filesystem>
#include <fstream>
#include <functional>

/**
 * @brief Helper class to write to a file using an overloaded operator<<
 *
 */
class FileWriter {
  public:
    template<class T> FileWriter &operator<<(const T &t) {
        file << t;
        return *this;
    }
    FileWriter &operator<<(std::ostream &(*os)(std::ostream &));

  private:
    explicit FileWriter(std::ofstream &file);
    std::ofstream &file;
    friend class FileHandler;
};

/**
 * @brief This class handles the input and the two output files
 *
 */
class FileHandler {
  public:
    /**
     * @brief Write mode is used to determine which file to write to
     * 		  with the overloaded operator<< in the FileWriter class
     *
     * @see FileWriter
     */
    enum class WriteMode { CPP, HPP };

    FileHandler();
    ~FileHandler();
    /**
     * @brief Closes all files
     *
     */
    void close();

    /**
     * @brief Opens all files
     *
     * @param input_path path to the input file
     * @param cpp_out_path path to the cpp file used for output
     * @param hpp_out_path path to the hpp file used for output
     */
    void open(const std::filesystem::path &input_path, const std::filesystem::path &cpp_out_path,
              const std::filesystem::path &hpp_out_path);

    /**
     * @brief Overloaded operator<< to write to a file
     *
     * @param mode WriteMode to determine which file to write to
     * 			   i.e. CPP or HPP file
     * @return FileWriter the FileWriter object to write to the file
     * @see FileWriter
     */
    FileWriter operator<<(WriteMode mode);

    /**
     * @brief Reads a line from the input file, it acts like std::getline
     *
     * @param line the line to read into
     * @return true if a line was read
     * @return false if the end of the file was reached
     */
    bool getline(std::string &line);

    /**
     * @brief copy the content of a file to the output file
     *
     * @param input_path path to the input file
     * @param mode WriteMode to determine which file to write to
     * 			   i.e. CPP or HPP file
     */
    void copy(const std::string &input_path, WriteMode mode);

  private:
    static bool open(std::ifstream &file, const std::filesystem::path &path);
    static bool open(std::ofstream &file, const std::filesystem::path &path);

    /**
     * @brief get the file to write to based on the WriteMode
     *
     * @param mode WriteMode to determine which we want to write to
     * 			   i.e. CPP or HPP file
     * @return std::ofstream& the file to write to
     */
    std::ofstream &get_file(WriteMode mode);

    std::ifstream input_file;
    std::ofstream out_cpp;
    std::ofstream out_hpp;
};