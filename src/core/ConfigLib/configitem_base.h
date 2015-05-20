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
#ifndef CONFIGITEM_BASE_H_
#define CONFIGITEM_BASE_H_

#include "configfile.h"

namespace configlib
{

/*
 * The base configuration item, responsible for acting as a variable. All
 * configuration variables should be derived from this.
 * When a configuration item is created it registers itself with the
 * appropriate configuration file.  It is used to store all the configutaion
 * settings in the file, and sends any updates to its value back to the 
 * configuration file.  Merely creating the variable with teh appropriate
 * parameters is enough to make everything work behind the scenes.
 */
class configitem_base
{
public:
	configitem_base(configfile& file,
					const std::string& section,
					const std::string& name,
					const std::string& param);
	virtual ~configitem_base();
	
	/*
	 * Used for parsing data from the configuration file.
	 */
	virtual bool parse(const std::string& value);
	virtual bool parse(const std::string& section,
					   const std::string& name);
	virtual bool parse(const std::string& section,
			           const std::string& name,
			   		   const std::string& value);
	virtual void toggle() { };

	/*
	 * Access to the name data.
	 */
	const std::string& get_section() const { return comp_section; }
	const std::string& get_name()	 const { return comp_name; }
	const std::string& get_param()	 const { return comp_param; }
	
	/*
	 * Only to be caled when the setting in the configuration file
	 * is realy to be removed (this ensures it is commented out)
	 */
	void clear() { clear_value(); }
		
	/*
	 * If there is no data, incrementing should not do anything.
	 */	   
	configitem_base & operator++() { return *this; }
	configitem_base operator++(int) { return *this; };
	
protected:
	/*
	 * These functions change the data stored in the configuration file.
	 */ 
	void clear_value();
	void clear_value(const std::string& name);
	void set_value(const std::string& value);
	void set_value(const std::string& section, const std::string& value);
	void set_value(const std::string& section, const std::string& name, const std::string& value);
	
	/*
	 * Encapsulatethe matching algorithm.  If matching bahvior needs to be changed
	 * it will be centeralized here.
	 */
	bool static match(const std::string& pattern, const std::string& data); 
	
private:
	/*
	 * Section and name store the ID for this item identified in the
	 * configuration file.  param is the command line parsing data.
	 */
	std::string comp_section;
	std::string comp_name;
	std::string comp_param;
	
	/*
	 * The configuration item needs to know what file it is associated with
	 * in order to handle updates and unregister itself if it goes out of scope.
	 */
	configfile* comp_file;
};

}

#endif /*CONFIGITEM_BASE_H_*/
