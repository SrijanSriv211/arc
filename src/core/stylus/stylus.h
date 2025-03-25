#pragma once

#include "console/console.h"
#include "token/token.h"

namespace console
{
	class stylus
	{
	public:
		stylus();
		std::vector<std::string> read();

	public:
		std::string input;

	private:
		void delete_last_word(std::string& input);
		console::color get_token_color(const lex::token_type& type);
		void render_char(const lex::token& token, const console::color &fore);

	private:
		std::vector<std::vector<std::string>> history;
		std::string r_input; // rendered input
	};
}
