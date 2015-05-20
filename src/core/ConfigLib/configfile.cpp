/*  The following is the Simplified BSD License
 * 
 * Copyright (c) 2008, Warren Kurt vonRoeschlaub
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer 
 *       in the documentation and/or other materials provided with the 
 *       distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "configfile.h"
#include "configitem_base.h"
#include <fstream>
#include <iostream>

namespace configlib
{

/*
 * The following are used by command line processing.
 */	
#define CHAR_DASH '-'
#define CHAR_EQUALS '='

/*
 * Constructor - assigns a file name
 */
configfile::configfile(const std::string& file_name)
: fileName(file_name)
{
}

/*
 * Destructor -
 */
configfile::~configfile()
{
}

/*
 * read - reads and parses the configuration file.
 */
void configfile::read()
{
	std::string line;
    std::ifstream fileStream(fileName.c_str());
    if (fileStream)
    {
    	// Push each line into the cache
        while(getline(fileStream, line))
            data.push_back(line);
        
        fileStream.close();
    }
    
    // Now parse everything into the attatched objects.
    parse();
}

/*
 * write - Writes the updated configuration file.
 */
void configfile::write()
{
	//Open the file for writing with truncation.
    std::ofstream fileStream(fileName.c_str(), std::ofstream::trunc);
    if (fileStream) 
    {
    	// Iterate over the cache and write out all data.
        std::vector<std::string>::iterator it;
        for (it = data.begin();it != data.end();++it)
            fileStream << (*it) << std::endl;
        
        fileStream.close();
    }
}

/*
 * parse - parses the command line.  The caller is responsible for error
 * 		checking.  The return value is any erro that may have ocurred.
 *   ERROR_NONE    - No error.
 *   ERROR_UNKNOWN - An unknown parameter was passed.
 *   ERROR_MISSING - A parameter was needed but missing.
 *   ERROR_EXTRA   - Too many unswitched paremeters. 
 * Note: ERROR_EXTRA is used internally to signal the subroutines to
 * 		increment.  These cases do not result in an actual error.
 */
int configfile::parse(int argc, char *argv[])
{
	// For simplicity's sake, the configfile object tracks the name of
	// the program.
	prog_name = argv[0];
	
	// Now parse each argument.  Config items are scanned using simple
	// rules for command line parameters defined as follows:
	//   A character - The object uses that character
	//   A digit - The object uses the unswitched parameter with that index (starting with 0)
	//   A dash - the object uses the standard long-name format of two
	//			dashes followed by the configuration file name with any
	//			spaces in the name replaced with dashes.
	// One or more of the following can be appended to the string
	//   A dash - The object uses the character or the long-name
	//			convention as above.
	//   An equal sign - For a single character the next parameter will be
	//			used, for a long name anything after the = will be used.
	int err;
	int index = 1;
	int parm_index = 0;
	
	while (index < argc)
	{
		if (CHAR_DASH == argv[index][0])
		{
			// The first character is a dash, so the parameter must 
			// be a switch item.
			if (CHAR_DASH == argv[index][1])
			{
				// Long-name parameter
				err = parse_long(&(argv[index][2]));
				if (ERROR_NONE != err)
					return err;
			}
			else
			{
				// single character parameter (may be multiple)
				for (int count = 1;0 != argv[index][count];++count)
				{
					// Parse the character.  We will fail if the character is unknown.
					err = parse_char(argv[index][count], ((index + 1) < argc) ? argv[index + 1] : NULL);
					if (ERROR_UNKNOWN == err)
						return err;
					else if (ERROR_EXTRA == err)
					{
						// The next parameter was also read, so skip it.
						index++;
						break;
					}
				}
			}
		}
		else
		{
			// Cache the unescaped parameters
			cmds.push_back(argv[index]);
			
			// Parse the indexed parameter, if there is one.
			if (parm_index <= 9)
			{
				err = parse_index(parm_index, argv[index]);
				if ((ERROR_NONE != err) && (ERROR_EXTRA != err))
					return err;
			}
			++parm_index;
		}
	
		++index;	
	}
	
	// Return no error.
	return ERROR_NONE; 
}

/*
 * parse - parse the configuration file.  Parsing is done by interating over the
 * 		configuration file settings and passing them to each item until it finds
 * 		one that returns true - a signal that the item processes the configuration
 *      setting.
 */
void configfile::parse()
{
	// Build the iterator. 
	iterator it(data);
	while (!it.end())
	{
		// Pull out the name and value.
		std::string name = it.get_name();
		std::string value = it.get_value();
		
		// Loop over the config item list and attempt to parse with each one.
		for (std::vector<configitem_base*>::iterator vit = item_list.begin();
		     item_list.end() != vit;++vit)
		{
			// Items with a value will have that value parsed, but if there is no
			// value the parse item will do the appropriate toggle action.
			if (0 < value.length())
			{
				if ((*vit)->parse(it.get_section(), name, value))
					break;
			}
			else if ((*vit)->parse(it.get_section(), name))
				break;
		}

		++it;
	}
}

/*
 * parse - parse a single item from the configuration file.  This iterates over
 * 		the configuration file items looking for the one the passed item processes.
 * 		Sometimes the item needs to read more than one line, so do not stop
 *      processing until the end of the file is reached.
 */
void configfile::parse(configitem_base* item)
{
	// Get the iterator to loop over all cached data.
	iterator it(data);
	while (!it.end())
	{
		// Pull out the name and value
		std::string name = it.get_name();
		std::string value = it.get_value();
		
		// Process the value if there is one, otherwise perform the
		// toggle action.
		if (0 < value.length())
			item->parse(it.get_section(), name, value);
		else
			item->parse(it.get_section(), name);

		// Next item.
		++it;
	}
}

/*
 * parse_char - parse a single character command line parameter
 */
int configfile::parse_char(char item, const std::string& next)
{
	// Iterate over all config item objects.
	std::vector<configitem_base*>::iterator it;
	for (it = item_list.begin();item_list.end() != it;++it)
	{
		// If the character is in the paramter item list, use it.
		if ((*it)->get_param().find(item) != std::string::npos)
		{
			if ((*it)->get_param().find(CHAR_EQUALS) != std::string::npos)
			{
				// If the parameter list has a CHAR_EQUALS parse the next command 
				// line parameter.
				(*it)->parse(next);
				return ERROR_EXTRA;
			}
			else
			{
				// It is just switched, toggle it
				(*it)->toggle();
				return ERROR_NONE;
			}
		}
	}
	
	return ERROR_UNKNOWN;
}

/*
 * parse_long - parse a long-name command line parameter
 */
int configfile::parse_long(const std::string& name)
{
	// Long names use CHAR_EQUALS to separate the parameter.
	std::size_t eq_pos = name.find(CHAR_EQUALS);
	std::string compare = name.substr(0, eq_pos);
	
	// Convert the passed name by removing the dashes that
	// are standard in long-name parameters.
	std::size_t pos = compare.find(' ');
	while (std::string::npos != pos)
	{
		compare.replace(pos, 1, 1, CHAR_DASH);
		pos = compare.find(' ');
	}
	
	// Iterate over the stored items and find any match.
	std::vector<configitem_base*>::iterator it;
	for (it = item_list.begin();item_list.end() != it;++it)
	{
		// Only items with a dash in their parameter field can use
		// long names.
		if ((*it)->get_param().find(CHAR_DASH) != std::string::npos)
		{
			// If there is an equals in the parameter field, then the
			// value should be an assignment.
			if ((*it)->get_param().find(CHAR_EQUALS) != std::string::npos)
			{
				if (match((*it)->get_name().c_str(), compare.c_str()))
				{
					// If there is a CHAR_EQUALS in the definition there must be a
					// parameter associated with the name.
					if (std::string::npos != eq_pos)
						(*it)->parse(name.substr(eq_pos + 1));
					else
						return ERROR_MISSING;
				}
			}
			else if (match((*it)->get_name().c_str(), name.c_str()))
			{
				// It is just switched, so toggle it.
				(*it)->toggle();
				return ERROR_NONE;
			}
		}
	}
	
	return ERROR_UNKNOWN;
}

/*
 * parse_index - parse an unswitched parameter from the command line based on index
 */
int configfile::parse_index(int index, const std::string& next)
{
	std::vector<configitem_base*>::iterator it;
	for (it = item_list.begin();item_list.end() != it;++it)
	{
		// This code only allows 10 unswitched parameters
		if ((*it)->get_param().find((char)(index + '0')) != std::string::npos)
		{
			(*it)->parse(next);
			return ERROR_NONE;
		}
	}
	
	return ERROR_EXTRA;
}

/*
 * clear - comment out a parameter in the loaded config file
 */
void configfile::clear(const std::string& section, const std::string& name)
{
	iterator it(data);
	while (!it.end())
	{
		if (match(section.c_str(), it.get_section().c_str()) &&
	        match(name.c_str(), it.get_name().c_str()))
	    {
			it.insert(0, "#");
			break;
		}

		++it;
    }
}

/*
 * set - set the value of an item in the loaded configuration file
 */
void configfile::set(const std::string& section, const std::string& name, const std::string& value)
{
	iterator it(data);
	while (!it.end())
	{
		if (match(section.c_str(), it.get_section().c_str()) &&
	        match(name.c_str(), it.get_name().c_str()))
	    {
	    	it.replace_value(value);
			return;
		}

		++it;
    }

	// If we get here the line does not exist (or is commented out).  Find the start of
	// the section, and insert the name/value pair.
	std::string item = name;
	item.append("=");
	item.append(value);
	add(section, item);
}

/*
 * add - add a line to a section in the config file
 */
void configfile::add(const std::string& section, const std::string& line)
{
	iterator it(data);
	while (!it.end())
	{
		if (match(section.c_str(), it.get_section().c_str()))
	    {
	    	// Find start of next section and/or end of file
	    	iterator last = it;
	    	while (!it.end() && match(section.c_str(), it.get_section().c_str()))
	    	{
	    		last = it;
	    		++it;
	    	}
	    	
	    	// At the end of the file, append, otherwise insert immediately after
	    	// the line currently pointed at.
	    	if (it.end())
	    		data.push_back(line);
	    	else
	    		data.insert(last.get_next(), line);
			return;
		}

		++it;
    }
    
    // If we get here, even the section does not exist.
    std::string item = "[";
    item.append(section);
    item.append("]");
    data.push_back(item);
    data.push_back(line);
}

/*
 * erase - unlink a configuration item from the configuration file
 */
void configfile::erase(configitem_base* item)
{
	for (std::vector<configitem_base*>::iterator it = item_list.begin();
		item_list.end() != it;++it)
	{
		if ((*it) == item)
		{
			item_list.erase(it);
			break;
		}
	}
}

/*
 * match - return true if the two strings match
 */
bool configfile::match(const char* pattern, const char* data)
{
	// Compare strings for exact match
	// TODO: Do we need to double check this for buffer overflow?  Who's
	// calling this?
	if (0 == strcasecmp(pattern, data))
		return true;
	
	// TODO: add regex matching.
	// For now, just match wildcards
	if ('*' == pattern[0])
		return true;
	
	return false;
}

/*
 * Constructor - create the fake internal iterator for the configuration
 */
configfile::iterator::iterator(std::vector<std::string>& source)
{
	data = &source;
	index = data->begin();
	while(parse())
		++index;
}

/*
 * get_name - return the name of the current setting in the config file
 */ 
const std::string configfile::iterator::get_name() const
{
	std::size_t pos = (*index).find('=');
	if (std::string::npos != pos)
		return (*index).substr(0, pos);
	else
		return (*index);
}

/*
 * get_value - return the value of the current setting in the config file
 */
const std::string configfile::iterator::get_value() const
{
	std::size_t pos = (*index).find('=');
	if (std::string::npos != pos)
		return (*index).substr(pos + 1);
	else
		return "";
}

/*
 * replace_value - replace the value in the line the iterator currently points 
 * 			to with a new value.
 */
void configfile::iterator::replace_value(const std::string& value)
{
	std::size_t pos = index->find('=');
	*index = index->substr(0, pos + 1);
	index->append(value);
}

/*
 * operator= - duplicate the fake iterator
 */
configfile::iterator& configfile::iterator::operator=(const configfile::iterator& rhs)
{
	section = rhs.section;
	data = rhs.data;
	index = rhs.index;
	return *this;
}

/*
 * operator++ - increment the iterator to the next config file setting.
 */
configfile::iterator& configfile::iterator::operator++()
{
	if (data->end() != index)
	{
		do
		{
			++index;
		} while (parse());
	}
	
	return *this;
}

/*
 * parse - advance the line pointer, return true if it needs to be parsed again
 * 			to point at a valid configuration line
 */
bool configfile::iterator::parse()
{
	if (data->end() == index)
		return false; // Reached the end.
	
	std::size_t start = (*index).find_first_not_of(" \t");
	if (std::string::npos == start)
		return true; // Blank line;
	else if ('#' == (*index).at(start))
		return true;  // Comments are not parsed
	else if ('[' == (*index).at(start))
	{
		// Section header
		std::size_t end = (*index).find(']');
		section = (*index).substr(start + 1, end - 1);
		return true; // Section headers are not data.
	}
	else
		return false; // Found a statement
}

}
