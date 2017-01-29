/*
 * Read an INI file into easy-to-access name/value pairs.
 *
 * Go to the project home page for more info:
 * http://code.google.com/p/inih/
 *
 * inih and INIReader are released under the New BSD license:
 *
 * Copyright (c) 2009, Brush Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Brush Technology nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BRUSH TECHNOLOGY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL BRUSH TECHNOLOGY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cctype>
#include <cstdlib>
#include <fstream>
#include "ini.h"
#include "inireader.h"

using std::string;

INIReader::INIReader(std::istream & filestream)
{
	_error = ini_parse(filestream, ValueHandler, this);
}

INIReader::INIReader(string filename)
{
	std::ifstream filestream(filename, std::ios::ios_base::in);
	_error = ini_parse(filestream, ValueHandler, this);
	filestream.close();
}

int INIReader::ParseError() const
{
	return _error;
}

string INIReader::Get(string section, string name, string default_value)
{
	string key = MakeKey(section, name);
	return _values.count(key) ? _values[key] : default_value;
}

long INIReader::GetInteger(string section, string name, long default_value)
{
	string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

string INIReader::MakeKey(string section, string name)
{
	string key = section + "." + name;
	// Convert to lower case to make lookups case-insensitive
	for (unsigned int i = 0; i < key.length(); i++)
		key[i] = tolower(key[i]);
	return key;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
							const char* value)
{
	INIReader* reader = (INIReader*)user;
	reader->_values[MakeKey(section, name)] = value;
	return 1;
}
