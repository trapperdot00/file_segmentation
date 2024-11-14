#ifndef FILEACTION_H
#define FILEACTION_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <vector>

class FileAction {
public:
	FileAction(bool del = false) : delete_after_construct(del) {}

	void segment(const std::string &filename, std::size_t byte_count);
	void construct(const std::string &first_filename);
private:

	std::size_t file_size_bytes(std::ifstream &fs);
	std::size_t digit_count(std::size_t num) const;
	std::string add_leading_zeroes(std::size_t count, std::size_t num) const;
	std::string next_part_filename(const std::string &filename, std::size_t count, std::size_t num) const;

	std::string extension = ".part";
	std::vector<std::string> part_filenames;

	bool delete_after_construct;
};

#endif
