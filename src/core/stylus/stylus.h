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
		std::vector<std::string> history;

	private:
		console::color get_token_color(const lex::token_type& type);
		void render_token(const lex::token& token, const console::color &fore, const bool& render_char=true);
		COORD get_current_cursor_pos(const COORD& orig_cursor_pos);

	private:
		std::vector<std::string>::size_type h_idx; // history index
		std::string r_input; // rendered input
		size_t idx; // cursor pos index (in terms of length of input)
	};
}
