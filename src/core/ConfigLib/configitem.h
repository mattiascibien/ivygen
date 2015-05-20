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
#ifndef CONFIGITEM_H_
#define CONFIGITEM_H_

#include "configfile.h"
#include "configitem_base.h"
#include <sstream>

namespace configlib
{

/*
 * Template for defining a configuration item.
 * The tamplate extends the configitem_base object with all
 * object specific functionality.
 */	
template <typename _Tp>
class configitem : protected configitem_base
{
public:
	configitem(configfile& file,
			   const std::string& section,
			   const std::string& name,
			   const std::string& param,
			   const _Tp& def)
	: configitem_base(file, section, name, param)
	, def_value(def)
	, cur_value(def)
	{
	}
	
	virtual ~configitem() {}

	/*
	 * Parse a command line parameter normally.
	 */	
	virtual bool parse(const std::string& value)
	{
		item_parse(cur_value, value);
		return true;		
	}
	
	/*
	 * A switched value calls teh toggle routine.
	 */
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
	
	/*
	 * A configuration value is parsed.
	 */
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

	/*
	 * Get and set values
	 */	
	_Tp & get() { return cur_value; }
	const _Tp & get() const { return cur_value; }
	void set(const _Tp & value)
	{
		cur_value = value;
		set_value(item_get(cur_value));
	}
	const _Tp& def_get() const { return def_value; }
	virtual void toggle() { item_toggle(cur_value, def_get()); set(cur_value); }

	// If an error shows up on any of the following lines, then the
	// caller tried to call an operator function on an object that
	// doesn't support that operator.  Don't change the functions
	// below, just add an object specific version of the template
	// function called. 	
	_Tp & operator=(const _Tp & rhs) { set(rhs); return get(); }
	_Tp & operator++() { item_toggle(cur_value, def_get()); set(cur_value); return get(); }
	_Tp operator++(int) { _Tp temp = get(); operator++(); return temp; }
	bool operator==(const _Tp & other) const { return other == cur_value; }
	bool operator!=(const _Tp & other) const { return !(*this == other); }
	operator const _Tp() { return get(); }
	
private:
	_Tp def_value;
	_Tp cur_value;
};

/**
 * Template functions to override for special cases.
 **/

/*
 * item_parse - template function for parsing a string into the template class.
 */
template <class _Tp>
void item_parse(_Tp& item, const std::string& value)
{
	std::stringstream stream_value(value, std::stringstream::in);
	stream_value >> item;	
}

/*
 * item_parse - strings must be overridden or just the first word will be parsed
 */
void item_parse(std::string& item, const std::string& value)
{
	item = value;
}

/*
 * item_get - template function for turning the template class into a string.
 */
template <class _Tp>
std::string item_get(const _Tp& item)
{
	std::stringstream stream_value(std::stringstream::out);
	stream_value << item;
	return stream_value.str();
}

/*
 * item_toggle - template function for incrementing the template class, called
 * 				for parameterless configuration settings
 */
template <class _Tp>
void item_toggle(_Tp& item, const _Tp& /*default_value*/)
{
	++item;
}

/*
 * item_toggle - strings don't increment.
 */
void item_toggle(std::string& item, const std::string& default_value)
{
	if (default_value.length() > 0)
		item.clear();
	else
		item = "Set";
}

/*
 * item_toggle - bool variables should flip-flop
 */
void item_toggle(bool& item, const bool /*default_value*/)
{
	item = !item;
}

}

#endif /*CONFIGITEM_H_*/
