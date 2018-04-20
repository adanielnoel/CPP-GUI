//
//  Logger.h
//  graphics
//
//  Created by Alejandro Daniel Noel on 14/11/2016.
//
//

#ifndef Logger_h
#define Logger_h

#include <stdlib.h>

class Logger {
    char* filename;
    
public:
    Logger(char* _filename);
    bool restart_log ();
    bool log (const char* message, ...);
    bool log_error (const char* message, ...);
};

#endif /* Logger_h */
