/* 
 * File:   Program.h
 * Author: radek
 *
 * Created on 28 wrzesień 2013, 20:18
 */

#ifndef PROGRAM_H
#define	PROGRAM_H

#include <istream>
#include <vector>
#include <map>
#include <list>
class Program;
class Acronyms;
#include "Command.h"

class Acronyms{
    public:
        std::map<std::string, unsigned short> labels;
        std::map<std::string, unsigned short> variables;
        //TODO functions
};

class Program {
public:
    Program(std::istream& stream, bool highlevel = false);
    virtual ~Program();
    
    std::vector<unsigned short> getByteCode();
    
    Acronyms acronyms;
    
private:
    void readProgram(std::istream& stream);
    
    void preprocess();
    void precompile();
    
    void scanForAcronyms();
    void resolveAcronyms();
    void applyAcronyms();
    
    bool needsPreprocess;
    bool compiled = false;
    
    std::vector<unsigned short> bytecode;
    std::list<std::string> code;
    std::vector<Command> commands;
    
};

#endif	/* PROGRAM_H */

