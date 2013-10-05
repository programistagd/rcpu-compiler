/* 
 * File:   Command.h
 * Author: radek
 *
 * Created on 28 wrzesień 2013, 20:12
 */

#ifndef COMMAND_H
#define	COMMAND_H

#include <string>
#include <vector>
class Command;
#include "Program.h"

class Command {
public:
    explicit Command(std::string code);
    virtual ~Command();
    
    std::vector<std::string> _code;
    unsigned short id;
    std::string label;
    std::string acronymA, acronymB;
    
    short getSize();
    unsigned short bytes[3];
    bool lateIdDecisionBasedOffArguments = false;
    bool isSpecial = false;
    
protected:
    bool parseArgument(std::string code, unsigned short& byte, bool& specialCase, Acronyms* acronyms = NULL);
};

#endif	/* COMMAND_H */

