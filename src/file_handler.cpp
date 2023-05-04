#include "file_handler.hpp"

FileWriter::FileWriter(std::ofstream &file) : file(file) {}

FileWriter &FileWriter::operator<<(std::ostream &(*os)(std::ostream &)) {
	file << os;
	return *this;
}

FileHandler::FileHandler() = default;
FileHandler::~FileHandler() {
	close();
}

void FileHandler::close() {
	input_file.close();
	out_cpp.close();
	out_hpp.close();
}

void FileHandler::open(const std::filesystem::path &input_path,
					   const std::filesystem::path &cpp_out_path,
					   const std::filesystem::path &hpp_out_path) {
	if (!open(input_file, input_path)) {
		throw FileNotFound(input_path.string());
	}
	if (!open(out_cpp, cpp_out_path)) {
		throw FileNotFound(cpp_out_path.string());
	}
	if (!open(out_hpp, hpp_out_path)) {
		throw FileNotFound(hpp_out_path.string());
	}
}

bool FileHandler::open(std::ifstream &file,
					   const std::filesystem::path &path) const {
	file.open(path);
	return file.is_open();
}

bool FileHandler::open(std::ofstream &file,
					   const std::filesystem::path &path) const {
	file.open(path);
	return file.is_open();
}

std::ofstream &FileHandler::get_file(WriteMode mode) {
	switch (mode) {
		case WriteMode::CPP:
			return out_cpp;
		case WriteMode::HPP:
			return out_hpp;
	}
	throw std::invalid_argument("Invalid WriteMode");
}

FileWriter FileHandler::operator<<(WriteMode mode) {
	switch (mode) {
		case WriteMode::CPP:
			return FileWriter(out_cpp);
		case WriteMode::HPP:
			return FileWriter(out_hpp);
	}
	throw std::invalid_argument("Invalid WriteMode");
}

bool FileHandler::getline(std::string &line) {
	return bool(std::getline(input_file, line));
}

void FileHandler::copy(const std::string &input_path, WriteMode mode) {
	std::ifstream input(input_path);
	std::ofstream &output = get_file(mode);

	if (input.is_open()) {
		std::string line;
		while (std::getline(input, line)) {
			output << line << std::endl;
		}
		output << std::endl;
	} else {
		throw FileNotFound(input_path);
	}
}
