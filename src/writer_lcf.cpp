/*
 * Copyright (c) 2016 liblcf authors
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include "writer_lcf.h"
#include <fstream>

LcfWriter::LcfWriter(std::unique_ptr<std::ostream> filestream, std::string encoding) :
	filename(""),
	encoding(encoding),
	stream(std::move(filestream))
{
}

LcfWriter::LcfWriter(const char* filename, std::string encoding) :
	filename(filename),
	encoding(encoding),
	stream(new std::ofstream(filename, std::ios::ios_base::binary | std::ios::ios_base::out))
{
}

LcfWriter::LcfWriter(const std::string& filename, std::string encoding) :
	filename(filename),
	encoding(encoding),
	stream(new std::ofstream(filename, std::ios::ios_base::binary | std::ios::ios_base::out))
{
}

LcfWriter::~LcfWriter() {
	Close();
}

void LcfWriter::Close() {
	stream.reset();
}

void LcfWriter::Write(const void *ptr, size_t size, size_t nmemb) {
#ifdef NDEBUG
	stream->write(reinterpret_cast<const char*>(ptr), size*nmemb);
#else
	assert(stream->write(reinterpret_cast<const char*>(ptr), size*nmemb).good());
#endif
}

template <>
void LcfWriter::Write<uint8_t>(uint8_t val) {
	Write(&val, 1, 1);
}

template <>
void LcfWriter::Write<int16_t>(int16_t val) {
	SwapByteOrder(val);
	Write(&val, 2, 1);
}

template <>
void LcfWriter::Write<uint32_t>(uint32_t val) {
	SwapByteOrder(val);
	Write(&val, 4, 1);
}

void LcfWriter::WriteInt(int val) {
	uint32_t value = (uint32_t) val;
	for (int i = 28; i >= 0; i -= 7)
		if (value >= (1U << i) || i == 0)
			Write<uint8_t>((uint8_t)(((value >> i) & 0x7F) | (i > 0 ? 0x80 : 0)));
}

template <>
void LcfWriter::Write<int>(int val) {
	WriteInt(val);
}

template <>
void LcfWriter::Write<bool>(bool val) {
	uint8_t x = val ? 1 : 0;
	Write(x);
}

template <>
void LcfWriter::Write<double>(double val) {
	SwapByteOrder(val);
	Write(&val, 8, 1);
}

template <>
void LcfWriter::Write<bool>(const std::vector<bool>& buffer) {
	std::vector<bool>::const_iterator it;
	for (it = buffer.begin(); it != buffer.end(); it++) {
		uint8_t val = *it ? 1 : 0;
		Write(val);
	}
}

template <>
void LcfWriter::Write<uint8_t>(const std::vector<uint8_t>& buffer) {
	Write(&buffer.front(), 1, buffer.size());
}

template <>
void LcfWriter::Write<int16_t>(const std::vector<int16_t>& buffer) {
	std::vector<int16_t>::const_iterator it;
	for (it = buffer.begin(); it != buffer.end(); it++)
		Write(*it);
}

template <>
void LcfWriter::Write<uint32_t>(const std::vector<uint32_t>& buffer) {
	std::vector<uint32_t>::const_iterator it;
	for (it = buffer.begin(); it != buffer.end(); it++)
		Write(*it);
}

void LcfWriter::Write(const std::string& _str) {
	std::string str = Decode(_str);
	if (!str.empty()) {
		Write(&*str.begin(), 1, str.size());
	}
}

bool LcfWriter::IsOk() const {
	return (stream && stream->good());
}

std::string LcfWriter::Decode(const std::string& str_to_encode) {
	return ReaderUtil::Recode(str_to_encode, "UTF-8", encoding);
}

#ifdef WORDS_BIGENDIAN
void LcfWriter::SwapByteOrder(uint16_t& us)
{
	us =	(us >> 8) |
			(us << 8);
}

void LcfWriter::SwapByteOrder(uint32_t& ui)
{
	ui =	(ui >> 24) |
			((ui<<8) & 0x00FF0000) |
			((ui>>8) & 0x0000FF00) |
			(ui << 24);
}

void LcfWriter::SwapByteOrder(double& d)
{
	uint32_t *p = reinterpret_cast<uint32_t *>(&d);
	SwapByteOrder(p[0]);
	SwapByteOrder(p[1]);
	uint32_t tmp = p[0];
	p[0] = p[1];
	p[1] = tmp;
}
#else
void LcfWriter::SwapByteOrder(uint16_t& /* us */) {}
void LcfWriter::SwapByteOrder(uint32_t& /* ui */) {}
void LcfWriter::SwapByteOrder(double& /* d */) {}
#endif

void LcfWriter::SwapByteOrder(int16_t& s)
{
	SwapByteOrder((uint16_t&) s);
}
