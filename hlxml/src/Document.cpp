/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hdir.h>
#include <hltypes/hexception.h>
#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hltypes/hstring.h>

#include "RapidXmlConfig.h"
#include <rapidxml.hpp>

#include "Document.h"
#include "Exception.h"
#include "Node.h"

#define RAPIDXML_DOCUMENT ((rapidxml::xml_document<char>*)this->document)

namespace hlxml
{
	hstr logTag = "hlxml";

	Document::Document(chstr filename, bool fromResource) : document(NULL), data(NULL), rootNode(NULL)
	{
		this->filename = filename;
		this->fromResource = fromResource;
	}

	Document::Document(hsbase& stream) : document(NULL), data(NULL), rootNode(NULL)
	{
		int64_t position = stream.position();
		this->_setup(stream, "stream");
		stream.seek(position, hsbase::START);
	}

	Document::~Document()
	{
		this->rootNode = NULL;
		foreach_map (void*, Node*, it, this->nodes)
		{
			delete it->second;
		}
		this->nodes.clear();
		if (this->document != NULL)
		{
			delete RAPIDXML_DOCUMENT;
		}
		if (this->data != NULL)
		{
			delete[] this->data;
		}
	}

	void Document::_setup(hsbase& stream, chstr realFilename)
	{
		int dataSize = (int)stream.size();
		this->data = new char[dataSize + 1];
		int read = stream.readRaw(this->data, dataSize);
		this->data[read] = 0;
		this->realFilename = realFilename;
	}

	void Document::_parse()
	{
		if (this->data == NULL)
		{
			if (this->fromResource)
			{
				hresource resource;
				resource.open(this->filename);
				this->_setup(resource, hrdir::normalize(this->filename));
			}
			else
			{
				hfile file;
				file.open(this->filename);
				this->_setup(file, hdir::normalize(this->filename));
			}
		}
		this->document = new rapidxml::xml_document<char>();
		try
		{
			RAPIDXML_DOCUMENT->parse<rapidxml::parse_validate_closing_tags | rapidxml::parse_no_string_terminators | rapidxml::parse_no_data_nodes>(this->data);
		}
		catch (rapidxml::parse_error& e)
		{
			hstr desc = e.what() + hstr(" [") + e.where<char>() + "]";
			delete RAPIDXML_DOCUMENT;
			this->document = NULL;
			throw XMLException(hsprintf("An error occcured parsing XML file '%s': %s", this->realFilename.cStr(), desc.cStr()), NULL);
		}
	}

	Node* Document::root(chstr name)
	{
		if (this->document == NULL)
		{
			this->_parse();
		}
		if (this->rootNode == NULL)
		{
			rapidxml::xml_node<char>* rapidXmlNode = RAPIDXML_DOCUMENT->first_node();
			if (rapidXmlNode == NULL)
			{
				throw XMLException("No root node found in XML file '" + this->filename + "'!", NULL);
			}
			this->rootNode = this->_node(rapidXmlNode);
			if (name != "" && this->rootNode->name != name)
			{
				throw XMLException("Root node type is not '" + name + "' in XML file '" + this->filename + "'!", NULL);
			}
		}
		return this->rootNode;
	}

}
