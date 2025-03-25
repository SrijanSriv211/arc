#include "arcpch.h"
#include "stylus.h"

namespace console
{
	stylus::stylus()
	{
		this->input = "";
		this->r_input = "";
		this->history = {};
	}

	std::vector<std::string> stylus::read()
	{
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
				std::cout << std::endl;
                break;
			}

            // Ctrl+Backspace - delete last word
            if (virtual_key_code == VK_BACK && (modifier_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)))
            {
				COORD cursor_pos = console::get_cursor_pos();
				if (!this->input.empty() && cursor_pos.X != orig_cursor_pos.X && cursor_pos.Y != orig_cursor_pos.Y)
				{
                    this->delete_last_word(this->input);

					// clear screen
					console::set_cursor_pos(orig_cursor_pos);
					std::cout << std::string(this->input.size(), ' ');
					console::set_cursor_pos(orig_cursor_pos);
				}
                continue;
            }

            // backspace
            if (virtual_key_code == VK_BACK)
            {
				COORD cursor_pos = console::get_cursor_pos();
                if (!this->input.empty() && cursor_pos.X != orig_cursor_pos.X && cursor_pos.Y != orig_cursor_pos.Y)
				{
                    this->input.pop_back();
					std::cout << "\b \b";
				}

                continue;
            }

            // regular character input
            if (key_event.uChar.AsciiChar && isprint(key_event.uChar.AsciiChar))
                this->input += key_event.uChar.AsciiChar;

			// render if input is changed
			if (this->input != this->r_input)
			{
				// tokenize input
				tokens = lex::tokenize(this->input, false);

				if (tokens.size() == 1)
				{
					this->render_char(tokens.back(), console::LIGHT_WHITE);
					continue;
				}
				this->render_char(tokens.back(), this->get_token_color(tokens.back().type));
			}
		}

		tokens = lex::tokenize(this->input);

		if (tokens.empty() || tokens.size() == 0)
			return {};

		// _s means converted to strings
		std::vector<std::string> tokens_s(tokens.size());
		std::transform(tokens.begin(), tokens.end(), tokens_s.begin(), [](const lex::token& token) { return token.name; });

		this->history.push_back(tokens_s);
		return tokens_s;
	}

	void stylus::delete_last_word(std::string& input)
	{
        // trim trailing spaces
        while (!input.empty() && input.back() == ' ')
            input.pop_back();

        // remove characters until space or beginning of string
        while (!input.empty() && input.back() != ' ')
            input.pop_back();
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

	void stylus::render_char(const lex::token& token, const console::color &fore)
	{
		std::string token_name = std::string(1, token.name.back());
		console::print(token_name, fore, false, true);
		this->r_input = this->input;
	}
}
