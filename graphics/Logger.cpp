//
//  Logger.cpp
//  Graphics
//
//  Created by Alejandro Daniel Noel on 14/11/2016.
//
//

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "Logger.h"


Logger::Logger(char* _filename){
    this->filename = _filename;
    this->restart_log();
}

bool Logger::restart_log (){
    time_t now;
    char* date;
    FILE* file = fopen (this->filename, "w");
    if (!file) {
        fprintf (
                 stderr,
                 "ERROR: could not open log file %s for writing\n",
                 this->filename
                 );
        return false;
    }
    now = time (NULL);
    date = ctime (&now);
    fprintf (file, "Local time %s", date);
    fprintf (file, "Build version: %s %s\n\n", "--", "--");
    fclose (file);
    return true;
}

bool Logger::log (const char* message, ...){
    va_list argptr;
    FILE* file = fopen (this->filename, "a");
    if (!file) {
        fprintf (
                 stderr,
                 "ERROR: could not open %s file for appending\n",
                 this->filename
                 );
        return false;
    }
    va_start (argptr, message);
    vfprintf (file, message, argptr);
    va_end (argptr);
    fclose (file);
    return true;
}

/* same as gl_log except also prints to stderr */
bool Logger::log_error (const char* message, ...){
    bool ret = this->log(message);
    va_list argptr;
    va_start (argptr, message);
    vfprintf (stderr, message, argptr);
    va_end (argptr);
    return ret;
}
