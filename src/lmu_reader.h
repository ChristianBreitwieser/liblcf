/*
 * Copyright (c) 2016 liblcf authors
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef LCF_LMU_READER_H
#define LCF_LMU_READER_H

#include <string>
#include <memory>
#include "rpg_map.h"

/**
 * LMU Reader namespace.
 */
namespace LMU_Reader {

	/**
	 * Loads map.
	 */
	std::unique_ptr<RPG::Map> Load(std::istream& filestream, const std::string& encoding);

	/**
	 * Saves map.
	 */
	bool Save(std::ostream& filestream, const RPG::Map& map, const std::string& encoding);

	/**
	 * Saves map as XML.
	 */
	bool SaveXml(std::ostream& filestream, const RPG::Map& map);

	/**
	 * Loads map as XML.
	 */
	std::unique_ptr<RPG::Map> LoadXml(std::istream& filestream);
}

#endif
