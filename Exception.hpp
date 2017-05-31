#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>

class Exception: public std::exception{
public:
    Exception() throw(){}
    virtual ~Exception() throw(){}

    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const throw(){
        return "Undefined Exception";
    }
};

class IncorrectDataException:public Exception{
public:
    const char* what() const throw(){
        return "Incorrect Data Format";
    }
};

class IncorrectSessionException:public Exception{
public:
    const char* what() const throw(){
        return "Incorrect Session";
    }
};

class HandlingFailureException: public Exception{
public:
    const char* what() const throw(){
        return "Handling Failure";
    }
};

#endif // !_EXCEPTION_h