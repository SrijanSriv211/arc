#pragma once

namespace console
{
	class stylus
	{
	public:
		stylus();
		std::vector<std::string> read();

	public:
		std::string input;
	};
}
