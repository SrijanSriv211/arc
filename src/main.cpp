#include "arcpch.h"
#include "arc.h"

int main(int argc, char const *argv[])
{
	arc::clear_console();
	arc::run_startlist();
	return arc::is_running();
}
