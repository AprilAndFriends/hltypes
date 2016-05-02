/// @file
/// @version 4.0
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
#include <rapidxml.hpp>

#include "Document.h"
#include "Exception.h"
#include "Node.h"

#define RAPIDXML_DOCUMENT ((rapidxml::xml_document<char>*)this->document)

namespace hlxml
{
	hstr logTag = "hlxml";

	Document::Document(chstr filename, bool fromResource) : data(NULL), rootNode(NULL)
	{
		this->filename = filename;
		hstr realFilename;
		hstr data;
		if (fromResource)
		{
			if (!hresource::exists(this->filename))
			{
				throw ResourceFileCouldNotOpenException(this->filename);
			}
			data = hresource::hread(this->filename);
			realFilename = hrdir::normalize(this->filename);
		}
		else
		{
			if (!hfile::exists(this->filename))
			{
				throw FileCouldNotOpenException(this->filename);
			}
			data = hfile::hread(this->filename);
			realFilename = hdir::normalize(this->filename);
		}
		this->_parse(data, realFilename);
	}

	Document::Document(hsbase& stream) : data(NULL), rootNode(NULL)
	{
		this->_parse(stream.read(), "stream");
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

	void Document::_parse(chstr data, chstr realFilename)
	{
		int dataSize = data.size() + 1;
		this->data = new char[dataSize]();
		this->data[dataSize - 1] = 0;
		memcpy(this->data, data.cStr(), dataSize - 1);
		this->document = new rapidxml::xml_document<char>();
		try
		{
			RAPIDXML_DOCUMENT->parse<0>(this->data);
		}
		catch (rapidxml::parse_error& e)
		{
			hstr desc = e.what() + hstr(" [") + e.where<char>() + "]";
			throw XMLException(hsprintf("An error occcured parsing XML file '%s': %s", realFilename.cStr(), desc.cStr()), NULL);
		}
	}

	Node* Document::root(chstr name)
	{
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

	Node* Document::_node(void* node)
	{
		if (node == NULL)
		{
			return NULL;
		}
		Node* newNode = this->nodes.tryGet(node, NULL);
		if (newNode == NULL)
		{
			newNode = new Node(this, node);
			this->nodes[node] = newNode;
		}
		return newNode;
	}

}
