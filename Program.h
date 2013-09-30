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
#include "Command.h"

class Program {
public:
    Program(std::istream& stream);
    virtual ~Program();
    
    std::vector<unsigned short> getByteCode();
    
private:
    void readProgram(std::istream& stream);
    void scanForAcronyms();
    void resolveAcronyms();
    
    std::vector<Command> commands;
    std::map<std::string, unsigned short> labels;
    std::map<std::string, unsigned short> variables;
    //TODO functions
};

#endif	/* PROGRAM_H */

