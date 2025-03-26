#include "arcpch.h"
#include "stylus.h"

#include "strings/strings.h"
#include "array/array.h"

namespace console
{
	stylus::stylus()
	{
		this->history = {};
		this->h_idx = 0;
	}

	std::vector<std::string> stylus::read()
	{
		this->idx = 0;
		this->input = "";
		this->r_input = "";
		bool moved_in_history = false;

		COORD orig_cursor_pos = console::get_cursor_pos();
		std::vector<lex::token> tokens = {};

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
				this->idx = this->input.size();
				std::cout << std::endl;
                break;
			}

            // backspace
            if (virtual_key_code == VK_BACK)
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

					this->idx -= chunk_len;
					this->input.erase(this->idx, chunk_len);

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
					std::cout << std::string(this->input.size() - this->idx + chunk_len, ' ');
					console::set_cursor_pos(current_cursor_pos);
				}
                continue;
            }

			if (virtual_key_code == VK_LEFT)
			{
				if (this->idx > 0)
				{
					this->idx--;

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
				}
				continue;
			}

			if (virtual_key_code == VK_RIGHT)
			{
				if (this->idx < this->input.size())
				{
					this->idx++;

					COORD current_cursor_pos = this->get_current_cursor_pos(orig_cursor_pos);
					console::set_cursor_pos(current_cursor_pos);
				}
				continue;
			}

			if (virtual_key_code == VK_UP && !array::is_empty(this->history))
			{
				if (this->h_idx > 0)
					this->h_idx--;

				if (this->input == this->history[this->h_idx])
					continue;

				console::set_cursor_pos(orig_cursor_pos);
				std::cout << std::string(this->input.size(), ' ');
				console::set_cursor_pos(orig_cursor_pos);

				this->r_input = "";
				this->input = this->history[this->h_idx];
				this->idx = this->input.size();
				moved_in_history = true;
			}

			if (virtual_key_code == VK_DOWN && !array::is_empty(this->history))
			{
				if (this->h_idx < this->history.size() - 1)
					this->h_idx++;

				if (this->input == this->history[this->h_idx])
					continue;

				console::set_cursor_pos(orig_cursor_pos);
				std::cout << std::string(this->input.size(), ' ');
				console::set_cursor_pos(orig_cursor_pos);

				this->r_input = "";
				this->input = this->history[this->h_idx];
				this->idx = this->input.size();
				moved_in_history = true;
			}

            // regular character input
            if (key_event.uChar.AsciiChar && isprint(key_event.uChar.AsciiChar))
			{
				this->input.insert(this->idx, 1, key_event.uChar.AsciiChar);
				this->idx++;
			}

			// render if input is changed
			if (this->input == this->r_input)
				continue;

			// tokenize input
			tokens = lex::tokenize(this->input, false);

			if (moved_in_history)
			{
				for (std::vector<lex::token>::size_type i = 0; i < tokens.size(); i++)
				{
					if (i == 0)
					{
						this->render_token(tokens[i], console::LIGHT_WHITE, false);
						continue;
					}
					this->render_token(tokens[i], this->get_token_color(tokens[i].type), false);
				}

				moved_in_history = false;
				continue;
			}

			if (tokens.size() == 1)
			{
				this->render_token(tokens.back(), console::LIGHT_WHITE);
				continue;
			}
			this->render_token(tokens.back(), this->get_token_color(tokens.back().type));
		}

		tokens = lex::tokenize(this->input);

		if (strings::is_empty(input) || tokens.empty() || tokens.size() == 0)
			return {};

		// _s means converted to strings
		std::vector<std::string> tokens_s(tokens.size());
		std::transform(tokens.begin(), tokens.end(), tokens_s.begin(), [](const lex::token& token) { return token.name; });

		if (!strings::in_array(this->input, this->history))
		{
			this->history.push_back(input);
			this->h_idx = this->history.size() - 1;
		}
		std::cout << "[" << this->input << "]\n";
		return {};
		// return tokens_s;
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

	void stylus::render_token(const lex::token& token, const console::color &fore, const bool& render_char)
	{
		std::string token_name = render_char ? std::string(1, token.name.back()) : token.name;
		console::print(token_name, fore, false, true);
		this->r_input = this->input;
	}

	COORD stylus::get_current_cursor_pos(const COORD& orig_cursor_pos)
	{
		COORD cursor_pos = console::get_cursor_pos(this->idx);
		return {
			static_cast<SHORT>(cursor_pos.X + orig_cursor_pos.X),
			static_cast<SHORT>(cursor_pos.Y + orig_cursor_pos.Y)
		};
	}
}
