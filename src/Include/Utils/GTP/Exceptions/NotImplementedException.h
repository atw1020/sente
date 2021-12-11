//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_NOTIMPLEMENTEDEXCEPTION_H
#define SENTE_NOTIMPLEMENTEDEXCEPTION_H

#include <exception>


class NotImplementedException : public std::exception {

public:

    NotImplementedException(std::string error);

};


#endif //SENTE_NOTIMPLEMENTEDEXCEPTION_H
