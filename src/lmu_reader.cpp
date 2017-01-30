/*
 * Copyright (c) 2016 liblcf authors
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include "lmu_reader.h"
#include "lmu_chunks.h"
#include "reader_lcf.h"
#include "reader_util.h"
#include "reader_struct.h"

std::unique_ptr<RPG::Map> LMU_Reader::Load(std::istream & filestream, const std::string& encoding) {
	LcfReader reader(filestream, encoding);
	if (!reader.IsOk()) {
		LcfReader::SetError("Couldn't parse map file.\n");
		return std::unique_ptr<RPG::Map>();
	}
	std::string header;
	reader.ReadString(header, reader.ReadInt());
	if (header.length() != 10) {
		LcfReader::SetError("This is not a valid RPG2000 map.\n");
		return std::unique_ptr<RPG::Map>();
	}
	if (header != "LcfMapUnit") {
		fprintf(stderr, "Warning: This header is not LcfMapUnit and might not be a valid RPG2000 map.\n");
	}

	RPG::Map* map = new RPG::Map();
	Struct<RPG::Map>::ReadLcf(*map, reader);
	return std::unique_ptr<RPG::Map>(map);
}

bool LMU_Reader::Save(std::ostream & filestream, const RPG::Map& map, const std::string& encoding) {
	LcfWriter writer(filestream, encoding);
	if (!writer.IsOk()) {
		LcfReader::SetError("Couldn't parse map file.\n");
		return false;
	}
	const std::string header("LcfMapUnit");
	writer.WriteInt(header.size());
	writer.Write(header);

	Struct<RPG::Map>::WriteLcf(map, writer);
	return true;
}

bool LMU_Reader::SaveXml(std::ostream & filestream, const RPG::Map& map) {
	XmlWriter writer(filestream);
	if (!writer.IsOk()) {
		LcfReader::SetError("Couldn't parse map file.\n");
		return false;
	}
	writer.BeginElement("LMU");
	Struct<RPG::Map>::WriteXml(map, writer);
	writer.EndElement("LMU");
	return true;
}

std::unique_ptr<RPG::Map> LMU_Reader::LoadXml(std::istream & filestream) {
	XmlReader reader(filestream);
	if (!reader.IsOk()) {
		LcfReader::SetError("Couldn't parse map file.\n");
		return std::unique_ptr<RPG::Map>();
	}

	RPG::Map* map = new RPG::Map();
	reader.SetHandler(new RootXmlHandler<RPG::Map>(*map, "LMU"));
	reader.Parse();
	return std::unique_ptr<RPG::Map>(map);
}

