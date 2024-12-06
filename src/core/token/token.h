#pragma once


namespace lex
{
	enum token_type
	{
		EOL = 0, 			// END OF LINE
		UNKNOWN, 			// 1
		COMMENT, 			// 2
		WHITESPACE, 		// 3
		SEMICOLON, 			// 4
		STRING, 			// 5
		FLAG, 				// 5+1
		MATH, 				// 7
		ID, 				// 8
	};

	struct token
	{
		std::string name;
		token_type type;
	};

	std::vector<token> tokenize(const std::string& line);
}
