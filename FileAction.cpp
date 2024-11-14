#include "FileAction.h"

void FileAction::segment(const std::string &filename, std::size_t byte_count) {
	std::ifstream infile(filename, std::ifstream::binary);
	if (!infile.is_open())
		throw std::runtime_error(filename + " doesn't exist");
	std::size_t file_size = file_size_bytes(infile);
	std::size_t segment_count = (file_size + byte_count - 1) / byte_count;

	std::ofstream outfile;
	std::size_t bytes_read = 0;
	std::size_t file_count = 0;

	for (char current_byte; infile.get(current_byte); ++bytes_read) {
		if (bytes_read % byte_count == 0) {
			if (outfile.is_open())
				outfile.close();
			outfile.open(filename + extension +
					add_leading_zeroes(digit_count(segment_count), file_count++), std::ofstream::binary);
		}
		outfile.put(current_byte);
	}
}

void FileAction::construct(const std::string &first_filename) {
	std::string::size_type extension_pos = first_filename.rfind(extension);
	if (extension_pos == std::string::npos)
		throw std::runtime_error("filename has invalid extension, (current: \"" + extension + "\")");
	std::size_t part_digit_count = first_filename.substr(extension_pos + extension.size()).size();
	if (part_digit_count == 0)
		throw std::runtime_error("no part number (input first part's number)");

	std::ifstream infile;
	std::string out_filename = first_filename.substr(0, extension_pos);
	std::ofstream outfile(out_filename, std::ofstream::binary);
	std::size_t file_num = 0;

	while (true) {
		std::string next_filename = next_part_filename(out_filename, part_digit_count, file_num++);
		infile.open(next_filename, std::ifstream::binary);
		if (!infile.is_open()) break;
		part_filenames.push_back(next_filename);
		outfile << infile.rdbuf();
		infile.close();
	}

	if (delete_after_construct) {
		for (const auto &s : part_filenames) {
			std::remove(s.c_str());
		}
	}
}

std::size_t FileAction::file_size_bytes(std::ifstream &fs) {
	std::ifstream::pos_type old_pos = fs.tellg();
	fs.seekg(0, std::ifstream::end);
	std::ifstream::pos_type ret = fs.tellg();
	fs.seekg(old_pos);
	return ret;
}

std::size_t FileAction::digit_count(std::size_t num) const {
	std::size_t ret = 1;
	while (num >= 10) {
		num /= 10;
		++ret;
	}
	return ret;
}

std::string FileAction::add_leading_zeroes(std::size_t count, std::size_t num) const {
	std::ostringstream ret;
	ret << std::setw(count) << std::setfill('0') << num;
	return ret.str();
}

std::string FileAction::next_part_filename(const std::string &filename, std::size_t count, std::size_t num) const {
	return filename + extension + add_leading_zeroes(count, num);
}
