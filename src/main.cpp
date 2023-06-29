#include "Constants.h"
#include "OAT.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using std::string;
using std::vector;
using IncludesVector = vector<string>;

void displayHelpMessage()
{
	std::cout << constants::HELP_MESSAGE;
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc <= 1 || argc > 3) { displayHelpMessage(); }

	/*
	OAT.exe [file.oa] [c/noc - compile/don't compile]
	Exe: [0]
	File: [1]
	Compile: [2]
	*/

	string file{ argv[1] };

	// Get rid of .oa extension
	size_t lastIndex = file.find_last_of(".");
	string fileName = file.substr(0, lastIndex);

	string cppFile{ fileName + ".cpp" };
	string exeFile{ fileName + ".exe" };
	string compileVar{ argv[2] };

	bool compileOption{};
	if (compileVar == "c") { compileOption = true; }
	else if (compileVar == "nc") { compileOption = false; }
	else { displayHelpMessage(); } // Exits automatically

	string sourceFile{};
	sourceFile += constants::FIRST_MAIN;

	std::ifstream infile(file);

	string line{};
	IncludesVector includes{};
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		string a{};
		string b{};
		string c{};

		iss >> a >> b;
		std::getline(iss, c); // With this, you can have spaces now. E.g. io nlout "Hello World!"

		if ((a == ";") || (a == "\n") || (a == "")) { continue; } // Comment/newline, don't do anything

		sourceFile += constants::INDENTATION;
		c = OAT::process_c(c); // I need to process C so that it cuts off anything past a comment ;

		sourceFile = OAT::declare_function(a) + sourceFile;
		sourceFile += OAT::process_line(a, b, c, &includes);
	}

	sourceFile += constants::LAST_MAIN;

	/* 
	Sort through the includes vector to get rid of duplicates,
	then append the headers to eachother in a string, then append the sourceFile to the headers
	*/

	std::sort( includes.begin(), includes.end() );
	includes.erase( unique( includes.begin(), includes.end() ), includes.end() );

	// Append headers to eachother in a string
	string includesString{};
	for (size_t i = 0; i < includes.size(); ++i)
	{
		includesString += includes[i];
	}

	string fullSource{ includesString + sourceFile };

	// Write to file
	std::ofstream out(cppFile);
	out << fullSource;
	out.close();

	if (compileOption)
	{
		string command{ "g++ " + cppFile + " -o " + exeFile };
		system(command.c_str());
		std::cout << "\"" << file << "\"" << " compiled to " << "\"" << exeFile << "\"" << '\n';
	}
	else
	{
		std::cout << "\"" << file << "\"" << " transcompiled to " << "\"" << cppFile << "\"" << '\n';
	}

	if (constants::DELETE_CPP_FILE && compileOption)
	{
		if (std::remove(cppFile.c_str()) != 0)
		{
			std::cout << "Failed to delete \"" + cppFile + "\"\n";
			exit(1);
		}
	}

	return 0;
}