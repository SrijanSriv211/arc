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
		ANGLE, 				// 5+1
		FLAG, 				// 7
		MATH, 				// 8
		DOT, 				// 9
		ID	 				// 10
	};

	struct token
	{
		std::string name;
		token_type type;
	};

	std::vector<token> tokenize(const std::string& line, const bool& throw_error=true);
}
