/*
 * Copyright (c) 2016 liblcf authors
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef LCF_READER_UTIL_H
#define LCF_READER_UTIL_H

#include <string>

/**
 * ReaderUtil namespace.
 */
namespace ReaderUtil {
	/**
	 * Returns the encoding name of a windows numeric codepage.
	 *
	 * @param codepage codepage to lookup.
	 * @return name used by the converter or empty string if not found.
	 */
	std::string CodepageToEncoding(int codepage);

	/**
	 * Detects the encoding based on text analysis.
	 *
	 * @param filestream stream containing the database file
	 *
	 * @return encoding or empty string if not detected.
	 */
	std::string DetectEncoding(std::istream & filestream);

	/**
	* Detects the encoding based on text analysis.
	*
	* @param filestream stream containing the database file
	*
	* @return encoding or empty string if not detected.
	*/
	std::string DetectEncoding(std::string const & data);

	/**
	 * Detects the encoding based on text analysis and returns a vector with
	 * possible candidates, highest candidate being at the beginning.
	 *
	 * @param filestream stream containing the database file
	 *
	 * @return list of encodings or empty if not detected
	 */
	std::vector<std::string> DetectEncodings(std::istream & filestream);

	/**
	* Detects the encoding based on text analysis and returns a vector with
	* possible candidates, highest candidate being at the beginning.
	*
	* @param string encoded data of a few hundred bytes
	*
	* @return list of encodings or empty if not detected
	*/
	std::vector<std::string> DetectEncodings(std::string const & data);

	/**
	 * Returns the encoding set in the ini file.
	 *
	 * @param filestream The ini file to parse.
	 *
	 * @return encoding or empty string if not found.
	 */
	std::string GetEncoding(std::istream & filestream);

	/**
	 * Returns the system encoding based on current locale settings.
	 *
	 * @return system encoding or western if no locale found.
	 */
	std::string GetLocaleEncoding();

	/**
	 * Converts a string to unicode.
	 *
	 * @param str_to_encode string to encode
	 * @param source_encoding Encoding of str_to_encode
	 *
	 * @return the recoded string.
	 */
	std::string Recode(const std::string& str_to_encode, const std::string& source_encoding);

	/**
	 * Converts a string between encodings.
	 *
	 * @param str_to_encode the string to convert.
	 * @param src_enc the source encoding.
	 * @param dst_enc the destination encoding.
	 * @return the recoded string.
	 */
	std::string Recode(const std::string& str_to_encode,
					   const std::string& src_enc,
					   const std::string& dst_enc);
}

#endif
