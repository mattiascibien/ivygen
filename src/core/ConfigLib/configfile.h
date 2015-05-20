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
#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <vector>

namespace configlib
{

// Parsing error codes	
#define ERROR_NONE 0
#define ERROR_EXTRA 1
#define ERROR_UNKNOWN 2
#define ERROR_MISSING 3

// Forward declaration
class configitem_base;
	
/*
 * The file handler, responsible for parsing all configitem_base derived
 * objects assigned to it.
 * For each configuration file, create a configfile object and assign
 * configitem_base derived objects to it.  Do not free the file object
 * until all members are freed.
 */
class configfile
{
	friend class configitem_base;

public:
	// Iterator for unswitched command line parameters.
	typedef std::vector<std::string>::const_iterator cmd_iterator;
	typedef std::vector<std::string>::const_reverse_iterator cmd_reverse_iterator;

public:
	configfile(const std::string& file_name);
	virtual ~configfile();
	
	/*
	 * Configuration reading and writing.
	 */
	void read();
	void write();
	int parse(int argc, char *argv[]);
	
	/*
	 * File and program name information
	 */
	const std::string& name() const { return prog_name; }
	const char* get() const { return fileName.c_str(); }

	/*
	 * Command line parameter iteration
	 */
	cmd_iterator begin() const { return cmds.begin(); }
	cmd_iterator end() const { return cmds.end(); }
	cmd_reverse_iterator rbegin() const { return cmds.rbegin(); }
	cmd_reverse_iterator rend() const { return cmds.rend(); }
	std::size_t size() const { return cmds.size(); }
		
private:
	// This is not a real iterator, just a class to handle internal
	// iteration-like operations on the data vector.  It automatically
	// skips lines that are not configuration settings and keeps track
	// of the current section.
	class iterator
	{
	public:
		iterator(std::vector<std::string>& source);
		
		const std::string& get_section() const { return section; }
		const std::string get_name() const;
		const std::string get_value() const;
		std::vector<std::string>::iterator get_next() { return ++index; }
		
		void insert(std::size_t pos, const std::string& str) { (*index).insert(pos, str); }
		void replace_value(const std::string& value);
		
		void begin() { index = data->begin(); section.erase(); if (parse()) operator++();}
		bool end() { return (data->end() == index); }
		
		iterator& operator=(const iterator& rhs);
		iterator& operator++();
		operator std::vector<std::string>::iterator() { return index; }
		
	protected:
		bool parse();
	
	private:
		std::string section;
		std::vector<std::string>* data;
		std::vector<std::string>::iterator index;
	};
		
protected:
	/*
	 * Configuration manipulation.
	 */
    void parse();
    void parse(configitem_base* item);
    int parse_char(char item, const std::string& next);
    int parse_long(const std::string& name);
    int parse_index(int index, const std::string& next);
    void clear(const std::string& section, const std::string& name);
    void set(const std::string& section, const std::string& name, const std::string& value);
    void add(const std::string& section, const std::string& line);
    
    /*
     * Config item handling.
     */
    void push_back(configitem_base* item) { item_list.push_back(item); parse(item); }
    void erase(configitem_base* item);
    
	static bool match(const char* pattern, const char* data);
	
private:
	std::string prog_name;
    std::string fileName;
    std::vector<std::string> data;
    std::vector<configitem_base*> item_list;
    std::vector<std::string> cmds;
};

}

#endif /*CONFIGFILE_H_*/
