#include "arcpch.h"
#include "stylus.h"

#include "functions/functions.h"
#include "settings/settings.h"
#include "strings/strings.h"
#include "array/array.h"
#include "math/math.h"

namespace console
{
	stylus::stylus()
	{
		this->history = {};
		this->h_idx = 0;
	}

	std::vector<std::string> stylus::read(const std::string& text)
	{
		this->input = text;
		this->idx = text.size();

		std::vector<lex::token> tokens = lex::tokenize(this->input, false);

		if (strings::is_empty(this->input) || tokens.empty() || tokens.size() == 0)
			return {};

		this->render_tokens(0);
		std::cout << std::endl;

		// to avoid error message coming before the text is rendered
		tokens = lex::tokenize(this->input);

		if (strings::is_empty(this->input) || tokens.empty() || tokens.size() == 0)
			return {};

		// _s means converted to strings
		std::vector<std::string> tokens_s(tokens.size());
		std::transform(tokens.begin(), tokens.end(), tokens_s.begin(), [](const lex::token& token) { return token.name; });

		if (!strings::in_array(this->input, this->history))
		{
			this->history.push_back(input);
			this->h_idx = this->history.size() - 1;
		}

		return tokens_s;
	}

	std::vector<std::string> stylus::read()
	{
		this->idx = 0;
		this->s_idx = 0;
		this->input = "";

		COORD orig_cursor_pos = console::get_cursor_pos();
		std::vector<lex::token> tokens = {};
		std::string input_suggestion = "";
		std::string prev_suggestion = "";

		while (true)
		{
			// get key event
			KEY_EVENT_RECORD key_event;
			if (!console::getconchar(key_event))
				continue;

			// handle key press
			DWORD modifier_state = console::get_modifier_state(key_event);
			int virtual_key_code = key_event.wVirtualKeyCode;

            // enter
            if (virtual_key_code == VK_RETURN)
			{
				// ctrl+enter to select current suggestion
				if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
				{
					if (strings::is_empty(input_suggestion))
						continue;

					this->input += input_suggestion;
					this->render_tokens(this->idx);
					this->idx = this->input.size();

					input_suggestion = "";
					prev_suggestion = "";
					this->s_idx = 0;
					continue;
				}

				this->idx = this->input.size();
				std::cout << std::endl;
                break;
			}

			else if (virtual_key_code == VK_TAB)
			{
				std::vector<std::string> all_cmds = array::flat({array::flat(settings::get_all_cmds_list()), array::flat(functions::get_all_cmds_list(true))});
				all_cmds.insert(all_cmds.end(), this->history.begin(), this->history.end());
				std::vector<std::string> suggestions_list = {};

				for (const std::string& cmd : all_cmds)
				{
					if (!cmd.starts_with(this->input))
						continue;
					suggestions_list.push_back(cmd);
				}
				suggestions_list.push_back("");

				if (suggestions_list.size() == 1)
					continue;

				this->s_idx = math::modulo(this->s_idx, suggestions_list.size());
				input_suggestion = suggestions_list[this->s_idx];

				if (input_suggestion.size() > 0)
					input_suggestion = input_suggestion.substr(this->input.size());

				COORD end_of_input = this->get_cursor_pos(orig_cursor_pos, this->input.size());
				console::set_cursor_pos(end_of_input);
				this->idx = this->input.size();

				console::print(input_suggestion, console::color::GRAY, false, true);
				std::cout << std::string(prev_suggestion.size(), ' ');

				console::set_cursor_pos(end_of_input);

				prev_suggestion = input_suggestion;
				this->s_idx++;
			}

            // backspace
            else if (virtual_key_code == VK_BACK)
            {
				COORD cursor_pos = console::get_cursor_pos();
				if (!this->input.empty() && this->idx > 0 && cursor_pos.X >= orig_cursor_pos.X && cursor_pos.Y >= orig_cursor_pos.Y)
				{
					// chunk len to delete
					size_t chunk_len = 1;

					// ctrl+backspace
					if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
					{
						size_t start_idx = this->idx;
						while (!this->input.empty() && this->input[start_idx - 1] == ' ' && start_idx > 0)
							start_idx--;

						while (!this->input.empty() && this->input[start_idx - 1] != ' ' && start_idx > 0)
							start_idx--;
						chunk_len = this->idx - start_idx;
					}

					std::string old_input = this->input;
					this->idx -= chunk_len;
					this->input.erase(this->idx, chunk_len);

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
					this->render_tokens(this->idx);
					std::cout << std::string(old_input.size() - this->input.size() + input_suggestion.size(), ' ');

					current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
					input_suggestion = "";
					prev_suggestion = "";
					this->s_idx = 0;
				}
            }

			else if (virtual_key_code == VK_LEFT)
			{
				if (this->idx > 0)
				{
					// chunk len to jump left
					size_t chunk_len = 1;

					// ctrl+left
					if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
					{
						size_t start_idx = this->idx;
						while (!this->input.empty() && this->input[start_idx - 1] == ' ' && start_idx > 0)
							start_idx--;

						while (!this->input.empty() && this->input[start_idx - 1] != ' ' && start_idx > 0)
							start_idx--;
						chunk_len = this->idx - start_idx;
					}

					this->idx -= chunk_len;

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
				}
				continue;
			}

			else if (virtual_key_code == VK_RIGHT)
			{
				if (this->idx < this->input.size())
				{
					// chunk len to jump right
					size_t chunk_len = 1;

					// ctrl+left
					if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
					{
						chunk_len = 0;
						while (!this->input.empty() && this->input[chunk_len - 1] == ' ' && chunk_len < this->input.size())
							chunk_len++;

						while (!this->input.empty() && this->input[chunk_len - 1] != ' ' && chunk_len < this->input.size())
							chunk_len++;
					}

					this->idx += chunk_len;

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
				}
				continue;
			}

			else if (virtual_key_code == VK_UP && !array::is_empty(this->history))
			{
				if (this->h_idx > 0)
					this->h_idx--;

				// ctrl+up to move to the first command in history
				if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
					this->h_idx = 0;

				if (this->input == this->history[this->h_idx])
					continue;

				const std::string old_input = this->input;
				this->input = this->history[this->h_idx];
				this->idx = this->input.size();

				console::set_cursor_pos(orig_cursor_pos);
				this->render_tokens(0);

				if (old_input.size() > this->input.size())
					std::cout << std::string(old_input.size() - this->input.size() + input_suggestion.size(), ' ');

				COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
				console::set_cursor_pos(current_cursor_pos);
				input_suggestion = "";
				prev_suggestion = "";
				this->s_idx = 0;
			}

			else if (virtual_key_code == VK_DOWN && !array::is_empty(this->history))
			{
				if (this->h_idx < this->history.size() - 1)
					this->h_idx++;

				// ctrl+down to move to the latest command in history
				if (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
					this->h_idx = this->history.size() - 1;

				if (this->input == this->history[this->h_idx])
					continue;

				const std::string old_input = this->input;
				this->input = this->history[this->h_idx];
				this->idx = this->input.size();

				console::set_cursor_pos(orig_cursor_pos);
				this->render_tokens(0);

				if (old_input.size() > this->input.size())
					std::cout << std::string(old_input.size() - this->input.size() + input_suggestion.size(), ' ');

				COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
				console::set_cursor_pos(current_cursor_pos);
				input_suggestion = "";
				prev_suggestion = "";
				this->s_idx = 0;
			}

            // regular character input
            else if (key_event.uChar.AsciiChar && isprint(key_event.uChar.AsciiChar))
			{
				if (!strings::is_empty(input_suggestion))
				{
					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					COORD end_of_input = this->get_cursor_pos(orig_cursor_pos, this->input.size());
					console::set_cursor_pos(end_of_input);
					std::cout << std::string(input_suggestion.size(), ' ');
					console::set_cursor_pos(current_cursor_pos);
					input_suggestion = "";
					prev_suggestion = "";
					this->s_idx = 0;
				}

				this->input.insert(this->idx, 1, key_event.uChar.AsciiChar);

				COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
				console::set_cursor_pos(current_cursor_pos);
				this->render_tokens(this->idx);

				this->idx++;
				current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
				console::set_cursor_pos(current_cursor_pos);
			}
		}

		tokens = lex::tokenize(this->input);

		if (strings::is_empty(this->input) || tokens.empty() || tokens.size() == 0)
			return {};

		// _s means converted to strings
		std::vector<std::string> tokens_s(tokens.size());
		std::transform(tokens.begin(), tokens.end(), tokens_s.begin(), [](const lex::token& token) { return token.name; });

		if (!strings::in_array(this->input, this->history))
		{
			this->history.push_back(input);
			this->h_idx = this->history.size() - 1;
		}
		return tokens_s;
	}

    // Color mapping for different token types
    console::color stylus::get_token_color(const lex::token_type& type)
    {
        switch (type)
        {
            case lex::token_type::STRING:     	return console::color::CYAN;
            case lex::token_type::MATH:       	return console::color::LIGHT_YELLOW;
            case lex::token_type::FLAG:       	return console::color::LIGHT_MAGENTA;
            case lex::token_type::COMMENT:    	return console::color::GRAY;
            case lex::token_type::SEMICOLON:    return console::color::GRAY;
            case lex::token_type::ANGLE:    	return console::color::LIGHT_WHITE;
            case lex::token_type::WHITESPACE: 	return console::color::WHITE;
            case lex::token_type::ID: 			return console::color::WHITE;
            case lex::token_type::DOT: 			return console::color::WHITE;
            default:                          	return console::color::WHITE;
        }
    }

	COORD stylus::get_cursor_pos(const COORD& orig_cursor_pos, const int& total_dist)
	{
		COORD cursor_pos = console::get_cursor_pos(total_dist);
		return {
			static_cast<SHORT>(cursor_pos.X + orig_cursor_pos.X),
			static_cast<SHORT>(cursor_pos.Y + orig_cursor_pos.Y)
		};
	}

	COORD stylus::get_current_cursor_pos(const COORD& orig_cursor_pos)
	{
		COORD cursor_pos = console::get_cursor_pos(this->idx);
		return {
			static_cast<SHORT>(cursor_pos.X + orig_cursor_pos.X),
			static_cast<SHORT>(cursor_pos.Y + orig_cursor_pos.Y)
		};
	}

	stylus::RTP_COORD stylus::calc_render_token_pos_coord(const std::vector<lex::token>& tokens, const size_t& cursor_pos)
	{
		if (cursor_pos == 0)
			return {0, 0};

		size_t token_idx = 0, char_idx = 0;
		for (std::vector<lex::token>::size_type i = 0; i < tokens.size(); i++)
		{
			token_idx = i;
			char_idx += tokens[i].name.size();

			if (char_idx >= cursor_pos)
			{
				char_idx -= cursor_pos;
				char_idx = tokens[i].name.size() - char_idx;
				break;
			}
		}
		return {token_idx, char_idx};
	}

	void stylus::render_tokens(const size_t& start_pos)
	{
		std::vector<lex::token> tokens = lex::tokenize(this->input, false);
		RTP_COORD coord = this->calc_render_token_pos_coord(tokens, start_pos);

		for (std::vector<lex::token>::size_type i = coord.t; i < tokens.size(); i++)
		{
			std::string token_name = tokens[i].name;
			if (i == coord.t)
				token_name = token_name.substr(coord.c);

			console::print(token_name, (i == 0) ? console::color::LIGHT_WHITE : this->get_token_color(tokens[i].type), false);
		}
	}
}
