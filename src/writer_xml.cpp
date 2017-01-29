/*
 * Copyright (c) 2016 liblcf authors
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <vector>
#include <fstream>
#include "writer_xml.h"

XmlWriter::XmlWriter(std::unique_ptr<std::ostream> filestream) :
	filename(""),
	indent(0),
	stream(std::move(filestream)),
	at_bol(true)
{
	Open();
}

XmlWriter::XmlWriter(const char* filename) :
	filename(filename),
	indent(0),
	stream(new std::ofstream(filename,std::ios::ios_base::out)),
	at_bol(true)
{
	Open();
}

XmlWriter::XmlWriter(const std::string& filename) :
	filename(filename),
	indent(0),
	stream(new std::ofstream(filename, std::ios::ios_base::out)),
	at_bol(true)
{
	Open();
}

XmlWriter::~XmlWriter() {
	Close();
}

void XmlWriter::Open() {
	(*stream) << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
}

void XmlWriter::Close() {
	stream.reset();
}

template <>
void XmlWriter::Write<bool>(const bool& val) {
	Indent();
	(*stream)<< (val ? "T" : "F");
}

template <>
void XmlWriter::Write<int>(const int& val) {
	Indent();
	(*stream) << val;
}

template <>
void XmlWriter::Write<uint8_t>(const uint8_t& val) {
	WriteInt((int) val);
}

template <>
void XmlWriter::Write<int16_t>(const int16_t& val) {
	WriteInt((int) val);
}

template <>
void XmlWriter::Write<uint32_t>(const uint32_t& val) {
	Indent();
	(*stream) << val;
}

template <>
void XmlWriter::Write<double>(const double& val) {
	Indent();
	(*stream) << val;
}

template <>
void XmlWriter::Write<std::string>(const std::string& val) {
	Indent();
	std::string::const_iterator it;
	for (it = val.begin(); it != val.end(); it++) {
		int c = (int) *it;
		switch (c) {
			case '<':
				(*stream) << "&lt;";
				break;
			case '>':
				(*stream) << "&gt;";
				break;
			case '&':
				(*stream) << "&amp;";
				break;
			case '\n':
				stream->put(c);
				at_bol = true;
				Indent();
			case '\r':
			case '\t':
				stream->put(c);
				break;
			default:
				if (c >= 0 && c < 32) {
					char temp[10];
					snprintf(temp,10, "&#x%04x;", 0xE000 + c);
					(*stream) << temp;
				}				
				else
					stream->put(c);
				break;
		}
	}
}

template <>
void XmlWriter::Write<std::vector<int> >(const std::vector<int>& val) {
	WriteVector<int>(val);
}

template <>
void XmlWriter::Write<std::vector<bool> >(const std::vector<bool>& val) {
	WriteVector<bool>(val);
}

template <>
void XmlWriter::Write<std::vector<uint8_t> >(const std::vector<uint8_t>& val) {
	WriteVector<uint8_t>(val);
}

template <>
void XmlWriter::Write<std::vector<int16_t> >(const std::vector<int16_t>& val) {
	WriteVector<int16_t>(val);
}

template <>
void XmlWriter::Write<std::vector<uint32_t> >(const std::vector<uint32_t>& val) {
	WriteVector<uint32_t>(val);
}

template <>
void XmlWriter::Write<std::vector<double> >(const std::vector<double>& val) {
	WriteVector<double>(val);
}

void XmlWriter::WriteInt(int val) {
	Write<int>(val);
}

template <class T>
void XmlWriter::WriteVector(const std::vector<T>& val) {
	Indent();
	typename std::vector<T>::const_iterator it;
	bool first = true;
	for (it = val.begin(); it != val.end(); it++) {
		if (!first)
			stream->put(' ');
		first = false;
		Write<T>(*it);
	}
}

template <class T>
void XmlWriter::WriteNode(const std::string& name, const T& val) {
	BeginElement(name);
	Write<T>(val);
	EndElement(name);
}

void XmlWriter::BeginElement(const std::string& name) {
	NewLine();
	Indent();
	(*stream) << "<" << name << ">";
	indent++;
}

void XmlWriter::BeginElement(const std::string& name, int ID) {
	NewLine();
	Indent();
	char temp[6];
	snprintf(temp, 6, "%04d", ID);
	(*stream) << "<" << name << "id=\"" << temp << "\n>";
	indent++;
}

void XmlWriter::EndElement(const std::string& name) {
	indent--;
	Indent();
	(*stream) << "</" << name << ">";
	NewLine();
}

void XmlWriter::NewLine() {
	if (at_bol)
		return;
	stream->put('\n');
	at_bol = true;
}

void XmlWriter::Indent() {
	if (!at_bol)
		return;
	for (int i = 0; i < indent; i++)
		stream->put(' ');
	at_bol = false;
}

bool XmlWriter::IsOk() const {
	return (stream && stream->good());
}

template void XmlWriter::WriteNode<int>(const std::string& name, const int& val);
template void XmlWriter::WriteNode<bool>(const std::string& name, const bool& val);
template void XmlWriter::WriteNode<uint8_t>(const std::string& name, const uint8_t& val);
template void XmlWriter::WriteNode<int16_t>(const std::string& name, const int16_t& val);
template void XmlWriter::WriteNode<uint32_t>(const std::string& name, const uint32_t& val);
template void XmlWriter::WriteNode<double>(const std::string& name, const double& val);
template void XmlWriter::WriteNode<std::string>(const std::string& name, const std::string& val);

template void XmlWriter::WriteNode<std::vector<int> >(const std::string& name, const std::vector<int>& val);
template void XmlWriter::WriteNode<std::vector<bool> >(const std::string& name, const std::vector<bool>& val);
template void XmlWriter::WriteNode<std::vector<uint8_t> >(const std::string& name, const std::vector<uint8_t>& val);
template void XmlWriter::WriteNode<std::vector<int16_t> >(const std::string& name, const std::vector<int16_t>& val);
template void XmlWriter::WriteNode<std::vector<uint32_t> >(const std::string& name, const std::vector<uint32_t>& val);
