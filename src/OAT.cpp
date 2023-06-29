#include "OAT.h"
#include "Constants.h"

#include <cmath>
#include <string>
#include <vector>

using std::string;
using std::vector;
using IncludesVector = vector<string>;

namespace vars
{
	static string def{ constants::DEFAULT };
	static bool def_reference{ false };
	static bool ipow_dec{ false };
	static bool fpow_dec{ false };
	static bool dpow_dec{ false };
	static bool type_dec{ false };
}

string prefixCheck(string prefix = "")
{
	if (vars::def_reference) { return ""; }
	else { return prefix; }
}

// Variables
string OAT::dec(string var_name, string var_value) { return "auto " + var_name + "{ " + var_value + " };\n"; }
string OAT::dec(string var_name, string var_value, string type) { return type + " " + var_name + "{ " + var_value + " };\n"; }
string OAT::mov(string l_var, string r_var) { return "auto " + r_var + "{ " + l_var + " };\n"; }
string OAT::anydef(string var_name, bool reference)
{
	vars::def = var_name;
	vars::def_reference = reference;
	return string("");
}
string OAT::conv(string var_name, string type) { return prefixCheck(type + " ") + vars::def + "{ static_cast<" + type + ">(" + var_name + ") };\n"; }
string OAT::iconv(string old_var, string new_var) { return "int " + new_var + "{ std::stoi(" + old_var + ") };\n"; }
string OAT::fconv(string old_var, string new_var) { return "float " + new_var + "{ strtof(" + old_var + ") };\n"; }
string OAT::dconv(string old_var, string new_var) { return "double " + new_var + "{ atof(" + old_var + ".c_str()) };\n"; }
string OAT::type(string var_name)
{
	// Add 
	return "";
}
string OAT::round(string l_var, string r_var) { return "auto " + r_var + "{ round(" + l_var + ") };\n"; }

// Math
string OAT::math(string l_var, string r_var, string mode, bool def)
{
	if (mode == "pow") { return prefixCheck("int ") + vars::def + " = pow(" + l_var + ", " + r_var + ");\n"; }
	else if (mode == "rpow") { return l_var + " = pow(" + l_var + ", " + r_var + ");\n"; }

	if (def) { return prefixCheck("auto ") + vars::def + " {" + l_var + " " + mode + " " + r_var + "};\n"; }
	else { return l_var + " " + mode + "= " + r_var + ";\n"; }
}
string OAT::cat(string l_var, string r_var) { return l_var + " += " + "std::string(" + r_var + ");\n"; }

string OAT::mathpow(string base, string exp, string mode) { return prefixCheck("int ") + vars::def + " = " + mode + "pow(" + base + ", " + exp + ");\n"; } // mode == i, d, etc

// Input/Output
string OAT::io(string mode, string var)
{
	if (mode == "out") { return "std::cout << " + var + ";\n"; }
	else if (mode == "nlout") { return "std::cout << " + var + " << '\\n';\n"; }
	else if (mode == "in") { return "std::string " + var + "{};\n" + constants::INDENTATION + "std::cin >> " + var + ";\n"; }
	else if (mode == "input")
	{
		if (vars::def_reference)
		{
			return "std::cout << " + var + ";\n"
				"" + constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}
		else
		{
			return "std::cout << " + var + ";\n"
				"std::string " + vars::def + " {};\n"
				"" + constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}
	}
	else if (mode == "defin")
	{
		if (vars::def_reference)
		{
			return "std::cin >> " + vars::def + ";\n";
		}
		else
		{
			return "std::string " + vars::def + "{};\n" + constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}
	}
	else { return string(""); }
}

string OAT::assert(string l_var, string r_var, string mode, bool format)
{
	if (format)
	{
		return "if (" + l_var + " " + mode + " " + r_var + ")"
			" { std::cout << \"" + l_var + " " + mode + " " + r_var + " = true\\n\"; }\n"
			"else { std::cout << \"" + l_var + " " + mode + " " + r_var + " = false\\n\"; }\n";
	}
	else
	{
		return "if (" + l_var + " " + mode + " " + r_var + ") { std::cout << \"true\\n\"; }\n"
			"else { std::cout << \"false\\n\";\n";
	}
} // mode == "==", <, >, etc

string OAT::cpp(string b, string c) { return "" + b + c + '\n'; } // DANGEROUS

vector<string> getIncludes(string to_include)
{
	if (to_include == "io") { return constants::IO_INCLUDES; }
	else if (to_include == "conv") { return constants::CONV_INCLUDES; }
	else if (to_include == "math") { return constants::MATH_INCLUDES; }
	else if (to_include == "string") { return constants::STRING_INCLUDES; }
	else if (to_include == "type") { return constants::TYPE_INCLUDES; }
	else { return constants::NONE_INCLUDE; }
}

string OAT::process_c(string c)
{
	// Removes " ;" and anything past it
	string cut_c{ c.substr(0, c.find(" ;", 0)) };

	if (cut_c == "NEWLINE") { return constants::NEWLINE_CHARACTER; }
	else if (cut_c == "SPACE") { return constants::SPACE; }
	else if (cut_c == "TRUE") { return constants::TRUE; }
	else if (cut_c == "FALSE") { return constants::FALSE; }
	else if (cut_c == "NONE") { return constants::NONE; }
	else { return cut_c; };
}

string OAT::process_line(string a, string b, string c, IncludesVector *include_vector)
{
	if (a == "dec") { return OAT::dec(b, c); }
	else if (a == "sdec") // string
	{
		// Push #include <string> into vector
		include_vector->push_back("#include <string>\n");
		return OAT::dec(b, c, "std::string");
	}
	else if (a == "idec") { return OAT::dec(b, c, "int"); } // int
	else if (a == "cdec") { return OAT::dec(b, c, "char"); } // char
	else if (a == "fdec") { return OAT::dec(b, c, "float"); } // float
	else if (a == "ddec") { return OAT::dec(b, c, "double"); } // double
	else if (a == "bdec") { return OAT::dec(b, c, "bool"); } // bool
	else if (a == "cat")
	{
		include_vector->push_back("#include <string>\n");
		return OAT::cat(b, c);
	}
	else if (a == "mov") { return OAT::mov(b, c); }
	else if (a == "def") { return OAT::anydef(b, false); }
	else if (a == "rdef") { return OAT::anydef(b, true); }
	else if (a == "conv") { return OAT::conv(b, c); }
	else if (a == "iconv")
	{
		include_vector->push_back("#include <string>\n");
		return OAT::iconv(b, c);
	}
	else if (a == "fconv")
	{
		include_vector->push_back("#include <cstdlib>\n");
		return OAT::fconv(b, c);
	}
	else if (a == "dconv")
	{
		include_vector->push_back("#include <cstdlib>\n");
		return OAT::dconv(b, c);
	}
	else if (a == "add") { return OAT::math(b, c, "+", false); }
	else if (a == "sub") { return OAT::math(b, c, "-", false); }
	else if (a == "mul") { return OAT::math(b, c, "*", false); }
	else if (a == "div") { return OAT::math(b, c, "/", false); }
	else if (a == "mod") { return OAT::math(b, c, "%", false); }
	else if (a == "pow")
	{
		include_vector->push_back("#include <cmath>\n");
		return OAT::math(b, c, "pow", false);
	}
	else if (a == "rpow")
	{
		include_vector->push_back("#include <cmath>\n");
		return OAT::math(b, c, "rpow", false);
	}
	else if (a == "defadd") { return OAT::math(b, c, "+", true); }
	else if (a == "defsub") { return OAT::math(b, c, "-", true); }
	else if (a == "defmul") { return OAT::math(b, c, "*", true); }
	else if (a == "defdiv") { return OAT::math(b, c, "/", true); }
	else if (a == "defmod") { return OAT::math(b, c, "%", true); }
	else if (a == "defpow")
	{
		include_vector->push_back("#include <cmath>\n");
		return OAT::math(b, c, "pow", true);
	}
	else if (a == "ipow") { return OAT::mathpow(b, c, "i"); }
	else if (a == "fpow") { return OAT::mathpow(b, c, "f"); }
	else if (a == "dpow") { return OAT::mathpow(b, c, "d"); }
	else if (a == "round")
	{
		include_vector->push_back("#include <cmath>\n");
		return OAT::round(b, c);
	}
	else if (a == "io") {
		include_vector->push_back("#include <iostream>\n");
		return OAT::io(b, c);
	}
	else if (a == "asserteq") { return OAT::assert(b, c, "==", false); }
	else if (a == "fasserteq") { return OAT::assert(b, c, "==", true); }
	else if (a == "assertlt") { return OAT::assert(b, c, "<", false); }
	else if (a == "fassertlt") { return OAT::assert(b, c, "<", true); }
	else if (a == "assertgt") { return OAT::assert(b, c, ">", false); }
	else if (a == "fassertgt") { return OAT::assert(b, c, ">", true); }
	else if (a == "assertgteq") { return OAT::assert(b, c, ">=", false); }
	else if (a == "fassertgteq") { return OAT::assert(b, c, ">=", true); }
	else if (a == "assertlteq") { return OAT::assert(b, c, "<=", false); }
	else if (a == "fassertlteq") { return OAT::assert(b, c, "<=", true); }
	else if (a == "cpp") { return OAT::cpp(b, c); }
	else if (a == "include")
	{
		vector<string> includes{ getIncludes(b) };
		for (string include : includes) { include_vector->push_back(include); }
		return string();
	}
	else if (a == "type" || a == "ptype" || a == "fptype")
	{
		vector<string> includes{ getIncludes("type") };
		for (string include : includes) { include_vector->push_back(include); }
		if (a == "type")
		{
			return prefixCheck("auto ") + vars::def + "{ type_name<decltype(" + b + ")>() };\n";
		}
		else if (a == "ptype")
		{
			include_vector->push_back("#include <iostream>\n");
			return "std::cout << " + string() + " type_name<decltype(" + b + ")>() << '\\n';\n";
		}
		else if (a == "fptype")
		{
			include_vector->push_back("#include <iostream>\n");
			return "std::cout << \"variable '" + b + "' has type '\" << type_name<decltype(" + b + ")>() << \"'\\n\";\n";
		}
		else { return ""; }
	}
	else { return ""; }
}

string OAT::declare_function(string function_name)
{
	if (function_name == "ipow" && !(vars::ipow_dec))
	{
		vars::ipow_dec = true;
		return "int ipow(int base, int exp) { return exp == 0 ? 1 : base * ipow(base, exp - 1); }\n";
	}
	else if (function_name == "fpow" && !(vars::fpow_dec))
	{
		vars::fpow_dec = true;
		return "float fpow(float base, float exp) { return exp == 0 ? 1 : base * fpow(base, exp - 1); }\n";
	}
	else if (function_name == "dpow" && !(vars::dpow_dec))
	{
		vars::dpow_dec = true;
		return "double dpow(double base, double exp) { return exp == 0 ? 1 : base * dpow(base, exp - 1); }\n";
	}
	else if ((function_name == "type" || function_name == "ptype" || function_name == "fptype") && !(vars::type_dec))
	{
		vars::type_dec = true;
		return constants::TYPE_NAME; // Adds the template for type in source
	}
	else { return ""; }
}