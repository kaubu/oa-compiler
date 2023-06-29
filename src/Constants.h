#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace constants
{
	// Config
	static constexpr bool DELETE_CPP_FILE{ true }; // false for debug, true for release

	// Includes
	static vector<string> IO_INCLUDES{ "#include <iostream>\n" };
	static vector<string> STRING_INCLUDES{ "#include <string>" };
	static vector<string> CONV_INCLUDES{ "#include <string>\n", "#include <cstdlib>\n" };
	static vector<string> MATH_INCLUDES{ "#include <cmath>\n" };
	static vector<string> TYPE_INCLUDES{ "#include <type_traits>\n", "#include <typeinfo>\n", "#include <memory>\n", "#include <string>\n", "#include <cstdlib>\n", "#include <typeinfo>\n" };
	static vector<string> NONE_INCLUDE{ "" };

	// Transpiler
	static const string FIRST_MAIN{ "int main()\n{\n" };
	static const string INDENTATION{ "	" };
	static const string NEWLINE{ "\n" }; // Was char, had to change
	static const string LAST_MAIN{ INDENTATION + "return 0;\n}\n" };
	static const string DEFAULT{ "DEF" }; // Placeholder variable

	// C Replacement
	static const string SPACE{ "\" \"" }; // Was char, had to change
	static const string NEWLINE_CHARACTER{ "\"\\n\"" };
	static const string TRUE{ "true" };
	static const string FALSE{ "false" };
	static const string NONE{ "" };

	// Messages
	static const string HELP_MESSAGE{
		"Incorrect arguments.\n"
		"OAT Usage:\n"
		"[OAT Executable] [anyfile.oa] [c/nc - compile/nocompile]\n"
		"Warning: You need g++ to compile.\n"
	};

	// Functions
	static const string TYPE_NAME{
		"#ifndef _MSC_VER\n"
		"#	include <cxxabi.h>\n"
		"#endif\n"
		"\n"
		"template <class T> std::string type_name()\n"
		"{\n"
		"	typedef typename std::remove_reference<T>::type TR;\n"
		"	std::unique_ptr<char, void(*)(void *)> own\n"
		"	(\n"
		"#ifndef _MSC_VER\n"
		"		abi::__cxa_demangle(typeid(TR).name(), nullptr,\n"
		"			nullptr, nullptr),\n"
		"#else\n"
		"		nullptr,\n"
		"#endif\n"
		"		std::free\n"
		"	);\n"
		"	std::string r = own != nullptr ? own.get() : typeid(TR).name();\n"
		"	if (std::is_const<TR>::value) { r += \" const\"; }\n"
		"	if (std::is_volatile<TR>::value) { r += \" volatile\"; }\n"
		"	if (std::is_lvalue_reference<T>::value) { r += \"&\"; }\n"
		"	else if (std::is_rvalue_reference<T>::value) { r += \"&&\"; }\n"
		"	return r;\n"
		"}\n"
	};
}

#endif // CONSTANTS_H