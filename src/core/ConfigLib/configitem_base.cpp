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
#include "configitem_base.h"
#include <iostream>
#ifdef _MSC_VER
#include <string.h>
#else
#include <strings.h>
#endif //_MSC_VER

namespace configlib
{

/*
 * Constructor - register the configuration object with teh configuration file
 */
configitem_base::configitem_base(configfile& file,
					const std::string& section,
					const std::string& name,
					const std::string& param)
: comp_section(section)
, comp_name(name)
, comp_param(param)
, comp_file(&file)
{
	file.push_back(this);
}

/*
 * Destructor - unregister the configuration object
 */
configitem_base::~configitem_base()
{
	comp_file->erase(this);
}

/*
 * parse - return false if the value cannot be stored.
 *   This is not a pure virtual function because it shouldn't
 * be required for the base class to handle this.  This is
 * only needed for configuration settings that can be put on
 * the comand line.
 */
bool configitem_base::parse(const std::string& value)
{
	return false;
}

/*
 * parse - return true only if the data applies to this object
 */
bool configitem_base::parse(const std::string& section,
						  	const std::string& name)
{
	// If the section is right and the name is right, parse the value
	return (configfile::match(comp_section.c_str(), section.c_str()) &&
	        configfile::match(comp_name.c_str(), name.c_str()));
}

/*
 * parse - return true only if the data applies to this object
 */
bool configitem_base::parse(const std::string& section,
						  	const std::string& name,
						   	const std::string& value)
{
	// If the section is right and the name is right, parse the value
	return (configfile::match(comp_section.c_str(), section.c_str()) &&
	        configfile::match(comp_name.c_str(), name.c_str()));
}

/*
 * clear_value - remove this item from the configuration file
 */
void configitem_base::clear_value()
{
	comp_file->clear(comp_section, comp_name);
}

/*
 * clear_value - remove the named item from the section in the configuration file.
 */
void configitem_base::clear_value(const std::string& name)
{
	comp_file->clear(comp_section, name);
}

/*
 * set_value - set the passed value in the configuration file
 */
void configitem_base::set_value(const std::string& value)
{
	set_value(comp_section, comp_name, value);
}

/*
 * set_value - set the passed section, name and value in the configuration file
 */
void configitem_base::set_value(const std::string& section, const std::string& value)
{
	comp_file->add(section, value);
}

/*
 * set_value - set the passed section, name and value in the configuration file
 */
void configitem_base::set_value(const std::string& section, const std::string& name, const std::string& value)
{
	comp_file->set(section, name, value);
}

/*
 * match - match pattern against data.  Centralized to ensure matching behavior can be
 * changed univerally and uniformly.
 */
bool configitem_base::match(const std::string& pattern, const std::string& data)
{
#ifdef _MSC_VER
	return _stricmp(pattern.c_str(), data.c_str()) == 0;
#else
	return strcasecmp(pattern.c_str(), data.c_str()) == 0;
#endif //_MSC_VER
}

}
