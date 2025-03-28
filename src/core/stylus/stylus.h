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
		std::vector<std::string> read(const std::string& text);

	public:
		std::string input;
		std::vector<std::string> history;

	private:
		// Render Token Position COORD
		struct RTP_COORD // from which token to render
		{
			size_t t; // from which token
			size_t c; // from which char in the above token
		};

		console::color get_token_color(const lex::token_type& type);
		COORD get_current_cursor_pos(const COORD& orig_cursor_pos);
		RTP_COORD calc_render_token_pos_coord(const std::vector<lex::token>& tokens, const size_t& cursor_pos);
		void render_tokens(const size_t& start_pos);

	private:
		size_t h_idx; // history index
		size_t s_idx; // suggestions index
		size_t idx; // cursor pos index (in terms of length of input)
		std::string input_suggestion;
	};
}
