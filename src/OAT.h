#ifndef OAT_H
#define OAT_H

#include <string>
#include <vector>

using std::string;
using std::vector;
using IncludesVector = vector<string>;

class OAT
{
public:
	static string process_line(string a, string b, string c, IncludesVector *include_vector);
	static string process_c(string c);
	static string declare_function(string function_name);
private:
	// Variables
	static string dec(string var_name, string var_value); // dec foo 5 - Uses auto unless specificed
	static string dec(string var_name, string var_value, string type); // dec foo 5 - Uses auto unless specificed
	static string mov(string l_var, string r_var); // mov foo bar
	static string anydef(string var_name, bool reference); // def name - Change default placeholder
	static string conv(string var_name, string type);
	static string iconv(string old_var, string new_var);
	static string fconv(string old_var, string new_var);
	static string dconv(string old_var, string new_var);
	static string type(string var_name);

	// Math
	static string math(string l_var, string r_var, string mode, bool def); // Mode is +, -, *, /, etc
	static string cat(string l_var, string r_var); // Concatenate r_var to l_var. cat foo NEWLINE
	static string mathpow(string base, string exp, string mode);
	static string round(string l_var, string r_var);

	// Input/Output
	static string io(string mode, string var = string("DEF")); // Mode could be out or in. Var is the message displayed.

	// Conditional statements
	static string assert(string l_var, string r_var, string mode, bool format);

	// Meta
	static string cpp(string b, string c);
};

#endif // OAT_H