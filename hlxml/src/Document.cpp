/// @file
/// @version 3.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <tinyxml.h>

#include <hltypes/hdir.h>
#include <hltypes/hexception.h>
#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hltypes/hstring.h>

#include "Document.h"
#include "Exception.h"
#include "Node.h"

namespace hlxml
{
	hstr logTag = "hlxml";

	Document::Document(chstr filename, bool fromResource) : rootNode(NULL)
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

	Document::Document(hsbase& stream) : rootNode(NULL)
	{
		this->_parse(stream.read(), "stream");
	}

	Document::~Document()
	{
		this->rootNode = NULL;
		foreach_map (TiXmlNode*, Node*, it, this->nodes)
		{
			delete it->second;
		}
		this->nodes.clear();
		if (this->document != NULL)
		{
			delete this->document;
		}
	}

	void Document::_parse(chstr data, chstr realFilename)
	{
		this->document = new TiXmlDocument();
		this->document->Parse(data.cStr());
		if (this->document->Error())
		{
			hstr desc = this->document->ErrorDesc();
			int row = this->document->ErrorRow();
			int col = this->document->ErrorCol();
			if (row != 0)
			{
				desc += hsprintf(" [row %d, column %d]", row, col);
				harray<hstr> lines = data.split("\n");
				if (lines.size() >= row) // just in case!
				{
					desc += "\n----------------------------------------------------------\n";
					desc += lines[row - 1].trimmed();
					desc += "\n----------------------------------------------------------";
				}
			}
			throw XMLException(hsprintf("An error occcured parsing XML file '%s': %s", realFilename.cStr(), desc.cStr()), NULL);
		}
	}

	Node* Document::root(chstr type)
	{
		if (this->rootNode == NULL)
		{
			TiXmlNode* tinyXmlNode = this->document->FirstChildElement();
			if (tinyXmlNode == NULL)
			{
				throw XMLException("No root node found in XML file '" + this->filename + "'!", NULL);
			}
			this->rootNode = this->_node(tinyXmlNode);
			if (type != "" && this->rootNode->value != type)
			{
				throw XMLException("Root node type is not '" + type + "' in XML file '" + this->filename + "'!", NULL);
			}
		}
		return this->rootNode;
	}

	Node* Document::_node(TiXmlNode* node)
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
