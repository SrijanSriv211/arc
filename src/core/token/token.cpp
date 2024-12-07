#include "arcpch.h"
#include "token.h"

#include "console\console.h"
#include "strings\strings.h"

namespace lex
{
	std::vector<token> tokenize(const std::string& line)
	{
		std::vector<token> tokens;
		std::string tok;

		for (std::string::size_type i = 0; i < line.size(); i++)
		{
			tok = line[i];

			if (tok == " ")
			{
				i++;
				while(i < line.size() && line[i] == ' ')
				{
					tok += line[i];
					i++;
				}
				i--;

				tokens.push_back({tok, lex::token_type::WHITESPACE});
			}

			else if (i < line.size() && tok == "/" && line[i+1] == '/')
			{
				i++;
				while(i < line.size())
				{
					tok += line[i];
					i++;
				}
				i--;

				tokens.push_back({tok, lex::token_type::COMMENT});
			}

			else if (tok == ";")
			{
				tokens.push_back({tok, lex::token_type::SEMICOLON});
			}

			else if (strings::any(tok, {"\"", "'", "`"}, true))
			{
				char str_literal = tok.front();

				i++;
				if (i >= line.size())
				{
                    std::string error_detail = "unexpected end of tokens after " + std::string(1, str_literal);
                    console::errors::syntax(error_detail);
					return tokens;
				}
				i--;

				// make string
				i++;
				while(i < line.size() && line[i] != str_literal)
				{
					if (line[i] == '\\')
					{
						i++;

						switch (line[i])
						{
							case '\\': tok += "\\"; break;
							case '"': tok += "\""; break;
							case '\'': tok += "'"; break;
							case 'n': tok += "\n"; break;
							case '0': tok += "\0"; break;
							case 't': tok += "\t"; break;
							case 'r': tok += "\r"; break;
							case 'b': tok += "\b"; break;
							case 'a': tok += "\a"; break;
							case 'f': tok += "\f"; break;
							default: tok += std::string(1, line[i]); break;
						}
					}

					else
						tok += line[i];

					i++;
				}
				tok += line[i];

				if (i >= line.size())
				{
					std::string error_detail = "missing terminating " + std::string(1, str_literal) + " character";
                    console::errors::syntax(error_detail);
					return tokens;
				}

				tokens.push_back({tok, lex::token_type::STRING});
			}

			else
			{
				i++;
				while(i < line.size() && !strings::any(std::string(1, line[i]), {" ", "\"", "'", ";"}, true))
				{
					if (line[i] == '/' && line[i+1] == '/')
						break;

					tok += line[i];
					i++;
				}
				i--;

				if (strings::startswith_any(tok, {"-", "/"}) && tok.size() > 1)
					tokens.push_back({tok, lex::token_type::FLAG});

				else if (strings::only(tok, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "_", ".", "-", "+", "*", "/", "(", ")"}))
					tokens.push_back({tok, lex::token_type::MATH});

				else
					tokens.push_back({tok, lex::token_type::ID});
			}
		}

		return tokens;
	}
}
