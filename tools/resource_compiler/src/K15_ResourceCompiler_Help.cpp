#include "K15_ResourceCompiler_Help.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	void Help::printHelp()
	{
		static const char* HELP_MESSAGE = ""
		"K15 Resource Compiler Version %d.%d\n"
		"====================================\n"
		"Usage:\n"
		"k15_resourcecompiler -i [file|dir] -o [dir] -q [high|medium|low]\n"
		"                     -f [mesh|shader|textures|all] -r\n"
		"\n"
		"Parameters:\n"
		"-i   -   Input [file|dir] either a single file or a complete directory\n"
		"-o   -   Ouput [dir] directory where to put compiled resources\n"
		"-q   -   Quality of the resources. **Not yet used**\n"
		"         DEFAULT: medium\n"
		"-f   -   Filter types of assets that get processed **Not yet used**\n"
		"         DEFAULT: all\n"
		"-r   -   Recursively search directory if input is a directory\n"
		"\n"
		"Description:\n"
		"The K15 Resourcecompiler is a commandline which compiled specific\n"
		"resource like textures (png, tga, tiff, pds) or meshes (fbx, dae)\n"
		"into specific binary formats which can be used by the 3D game\n"
		"engine \"K15 Engine 3D\".";

		printf(HELP_MESSAGE, VERSION_MAJOR, VERSION_MINOR);
	}
	/*********************************************************************************/
}