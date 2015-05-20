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
#ifndef CONFIGENUM_H_
#define CONFIGENUM_H_

#include "configfile.h"
#include "configitem_base.h"

namespace configlib
{

/*
 * Structure for holding the information of one enumerator item.
 */
typedef struct
{
	int value;
	char* names[99]; // C99 requires a default static size.
} enum_data;

/*
 * Macros for creating the enum array.  The final result must be of the
 * form
 * static enum_data booleans[] = {{0,  {"item1a", "item1b", NULL }},
 *	            		  		  {1, { "item2a", "item2b", "item3b", NULL }},
 *	            				  {configenum::invalid_index, { "", NULL }}};
 * 
 * To use these macros, declare an ENUM_LIST passing the name of the enum and
 * as many ENUM_ITEMs as necessary as parameters.  An ENUM_ITEM is passed an
 * integer value and as many string constants as needed.
 * A list of common errors and what causes them:
 *   expected primary token before , symbol: At least one ENUM_ITEM must be defined and
 *     must have at least one string parameter.
 *   invalid conversion from const char* to int: The first parameter of a ENUM_ITEM must
 *     be an integer constant.
 *   expected unqualified id befoe [ token: The first parameter of ENUM_LIST must be the
 *     name of the enumeration.
 */
#define ENUM_LIST(x,...) static enum_data x[] = { __VA_ARGS__, {configenum::invalid_index, { NULL }}}; 
#define ENUM_ITEM(x,...) {x, { __VA_ARGS__, NULL }}

/*
 * Class for defining an enumeraion within a configuration file.
 * Create using the configuration structure.
 */
class configenum : public configitem_base
{
public:
	static const int invalid_index = -1;
	
public:
	configenum(configfile& file,
			   const std::string& section,
			   const std::string& name,
			   const std::string& param,
			   enum_data* enumerator,
			   const std::string& def)
	: configitem_base(file, section, name, param)
	, def_value(def)
	, cur_value(def)
	, cur_index(invalid_index)
	, enumeration(enumerator)
	{
	}
	
	virtual ~configenum() {}
	
	virtual bool parse(const std::string& value)
	{
		item_parse(cur_value, value);
		return true;		
	}
	
	virtual bool parse(const std::string& section,
					   const std::string& name)
	{
		if (configitem_base::parse(section, name))
		{
			item_toggle(cur_value, def_value);
			return true;
		}
		else
			return false;
	}
	
	virtual bool parse(const std::string& section,
					   const std::string& name,
			   		   const std::string& value)
	{
		if (configitem_base::parse(section, name, value))
		{
			item_parse(cur_value, value);
			return true;
		}
		else
			return false;
	}
	
	const std::string& get() const { return cur_value; }
	int get_index()
	{
		if (invalid_index == cur_index)
			cur_index = find_index(cur_value);
		return cur_index;
	}
	void set(const std::string& value)
	{
		cur_index = invalid_index;
		cur_value = value;
		set_value(cur_value);
	}
	void set(int index)
	{
		cur_index = index;
		cur_value = find_value(index);
		set_value(cur_value);
	}
	const std::string& def_get() const { return def_value; }

	// If an error shows up on any of the following lines, then the
	// caller tried to call an operator function on an object that
	// doesn't support that operator.  Don't change the functions
	// below, just add an object specific version of the template
	// function called. 	
	const std::string& operator=(const std::string& rhs) { set(rhs); return get(); }
	int operator=(int rhs) { set(rhs); return get_index(); }
	int operator++() {	int index = get_index(); set(index + 1); return index; }
	int operator++(int) { return operator++() + 1; }
	int operator--() { int index = get_index(); set(index - 1); return index; }
	int operator--(int) { return operator--() - 1; }
	bool operator==(const std::string& other) const { return configitem_base::match(cur_value, other); }
	bool operator==(int index) const { return find_index(cur_value) == index; }
	bool operator!=(const std::string& other) const { return !(*this == other); }
	bool operator!=(int index) const { return !(*this == index); }
	operator const std::string() const { return get(); }
	operator const int() { return get_index(); }
	
private:
	int find_index(const std::string& value) const
	{
		for (int item = 0;enumeration[item].value != invalid_index;++item)
			for (int choice = 0;enumeration[item].names[choice] != NULL;++choice)
				if (configitem_base::match(enumeration[item].names[choice], value))
					return enumeration[item].value;
		
		return invalid_index;
	}
	
	const char* find_value(int index) const
	{
		for (int item = 0;enumeration[item].value != invalid_index;++item)
			if (enumeration[item].value == index)
				return enumeration[item].names[0];
		
		return NULL;
	}
	
private:
	std::string def_value;
	std::string cur_value;
	int cur_index;
	enum_data* enumeration;
};

}

#endif /*CONFIGENUM_H_*/
