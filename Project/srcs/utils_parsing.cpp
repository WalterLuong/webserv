#include "../include/webserve.hpp"

size_t skip_white_space(std::string str) {
	size_t i = 0;
	while (i < str.size())
	{
		if (str[i] != '\f' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r' && str[i] != '\v' && str[i] != ' ')
			break;
		i++;
	}
	return i;
}

std::string skip_word_and_ws(std::string line, std::string word) {
	line.erase(0, word.size());
	line.erase(0, skip_white_space(line));
	return line;
}
