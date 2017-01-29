/*
 * inih -- simple .INI file parser
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

#ifndef LCF_INI_H
#define LCF_INI_H
#include <istream>

/**
 * Parse given INI-style file. May have [section]s, name=value pairs
 * (whitespace stripped), and comments starting with ';' (semicolon). Section
 * is "" if name=value pair parsed before any section heading.
 *
 * For each name=value pair parsed, call handler function with given user
 * pointer as well as section, name, and value (data only valid for duration
 * of handler call). Handler should return nonzero on success, zero on error.
 *
 * @returns 0 on success, line number of first error on parse error (doesn't
 *          stop on first error), or -1 on file open error.
 */
int ini_parse(std::istream & filestream,
			int (*handler)(void* user, const char* section,
							 const char* name, const char* value),
			void* user);

/**
 * Nonzero to allow multi-line value parsing, in the style of Python's
 * ConfigParser. If allowed, ini_parse() will call the handler with the same
 * name for each subsequent line parsed.
 */
#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif


#endif
