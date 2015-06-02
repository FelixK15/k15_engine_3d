#include "K15_ResourceCompiler_Help.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	void Help::printHelp()
	{
		static const char* HELP_MESSAGE = "\n\n"
		"K15 Resource Compiler Version %s.%s\n"
		"====================================\n"
		"Usage:\n"
		"k15_resourcecompiler -i [file|dir] -o [dir] -q [high|medium|low]\n"
		"                     -f [mesh|shader|textures|all] -r\n"
		"\n"
		"Parameters:\n"
		"-i   -   Input [file|dir] either a single file or a complete directory\n"
		"-o   -   Output [dir] directory where to put compiled resources\n"
		"-q   -   Quality of the resources. **Not yet used**\n"
		"         DEFAULT: medium\n"
		"-u	  -   Replace old compiled file with new compiled file.\n"
		"-f   -   Filter types of assets that gets processed **Not yet used**\n"
		"         DEFAULT: all\n"
		"-r   -   Recursively search directory if input is a directory\n"
		"\n"
		"Description:\n"
		"The K15 Resource compiler is a command line tool, which compiles specific\n"
		"game resource files (e.g textures or meshes) into binary files which can be\n"
		"used by the 3D game engine \"K15 Engine 3D\".\n\n";

		printf(HELP_MESSAGE, VERSION_MAJOR, VERSION_MINOR);
	}
	/*********************************************************************************/
}