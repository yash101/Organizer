#ifndef ENTRY_H
#define ENTRY_H
//@"fn": "entry.h"
//@"author": "yash101 (Dev)"
//@"description": "This file is the header file for <entry.cpp>

//@"description": "Entry point for the program"
//@"arg[argc]": "Argument count -- How many arguments were issued to the program"
//@"arg[argv]": "Argument C String array"
int main(int argc, char** argv);

//@"description": "This namespace is essentially a structure for the arguments
namespace Arguments
{
  //@"description": "Argument count for the arguments issued to this program"
  extern int argc;
  //@"description": "Argument array for the arguments issued to this program"
  extern char** argv;
}
#endif
