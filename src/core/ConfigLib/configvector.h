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
#ifndef CONFIGVECTOR_H_
#define CONFIGVECTOR_H_

#include "configfile.h"
#include "configitem_base.h"
#include "configitem.h"
#include <sstream>
#include <vector>

namespace configlib
{

/*
 * Template for defining a section of a configuration file as a vector.
 * All configuration items in the section must have the same type.
 */
template <typename _Tp>
class configvector : protected configitem_base
{
public:
	typedef typename std::vector<_Tp>::iterator iterator;
	typedef typename std::vector<_Tp>::reverse_iterator reverse_iterator;
	typedef typename std::vector<_Tp>::const_iterator const_iterator;
	typedef typename std::vector<_Tp>::const_reverse_iterator const_reverse_iterator;
	
public:
	configvector(configfile& file,
			     const std::string& section)
	: configitem_base(file, section, "*", "")
	{
	}
	
	virtual ~configvector() {}
	
	virtual bool parse(const std::string& section,
					   const std::string& name)
	{
		if (configitem_base::parse(section, name))
		{
			// Add to vector
			_Tp curr_value;
			item_parse(curr_value, name);
			data.push_back(curr_value);
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
			// Add to vector
			std::string line = name;
			if (!value.empty())
			{
				line.append("=");
				line.append(value);
			}
			_Tp curr_value;
			item_parse(curr_value, line);
			data.push_back(curr_value);
			return true;
		}
		else
			return false;
	}
	
	_Tp & get(std::size_t index) { return data[index]; }
	const _Tp & get(std::size_t index) const { return data[index]; }
	void push_back(const _Tp & value)
	{
		data.push_back(value);
		set_value(get_section(), item_get(value));
	}
	void pop_back()
	{
		iterator it = data.end() - 1;
		clear_value(item_get(*it));
		data.pop_back();
	}

	/*
	 * The iterator for this object is actaually the iterator for the
	 * underlying vector object.
	 */	
	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }
	const_iterator begin() const { return data.begin(); }
	const_iterator end() const { return data.end(); }
	reverse_iterator rbegin() { return data.rbegin(); }
	reverse_iterator rend() { return data.rend(); }
	const_reverse_iterator rbegin() const { return data.rbegin(); }
	const_reverse_iterator rend() const { return data.rend(); }
	std::size_t size() const { return data.size(); }

	/*
	 * The iterator based functions can be passed on to the vector,
	 * but additional updates need to be sent.
	 */
	iterator insert(iterator position, const _Tp& value)
	{
		set_value(get_section(), item_get(value));
		return data.insert(position, value);
	}
	iterator erase(iterator position)
	{
		clear_value(item_get(*position));
		return data.erase(position);
	}

	/*
	 * If an error shows up on any of the following lines, then the
	 * caller tried to call an operator function on an object that
	 * doesn't support that operator.  Don't change the functions
	 * below, just add an object specific version of the template
	 * function called.
	 */ 	
	_Tp & operator=(const configvector<_Tp> & rhs) { data = rhs.data; return *this; }
	_Tp & operator++() { /* this is not meaningful */  return begin()->second; }
	_Tp operator++(int) { return operator++(); }
	_Tp & operator[](std::size_t index) { return get(index); }
	bool operator==(const configvector<_Tp> & other) const { return other.data == data; }
	bool operator!=(const _Tp & other) const { return !(*this == other); }
	
private:
	std::vector<_Tp> data;
};

}

#endif /*CONFIGVECTOR_H_*/
