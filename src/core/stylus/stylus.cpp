#include "arcpch.h"
#include "stylus.h"

#include "token/token.h"

namespace console
{
	stylus::stylus()
	{
		this->input = "";
	}

	std::vector<std::string> stylus::read()
	{
		// IO error!
		if (!std::getline(std::cin, this->input))
			return {};

		else if (input.empty())
			return {};

		std::vector<lex::token> tokens = lex::tokenize(input);

		// _s means converted to strings
		std::vector<std::string> tokens_s(tokens.size());
		std::transform(tokens.begin(), tokens.end(), tokens_s.begin(), [](const lex::token& token) { return token.name; });

		return tokens_s;
	}
}
