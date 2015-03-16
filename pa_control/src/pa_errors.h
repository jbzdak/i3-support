//
// Created by jb on 3/16/15.
//

#ifndef _PA_CONTROL_PA_ERRORS_H_
#define _PA_CONTROL_PA_ERRORS_H_

#include <exception>
#include <stdexcept>

namespace jb {
namespace pa {

class pa_exception : public std::runtime_error {

public:
  pa_exception(std::string const &__arg) : runtime_error(__arg) {
  }

};

class pa_no_connection : public pa_exception {


public:
  pa_no_connection(std::string const &__arg) : pa_exception(__arg){};
};

}}
#endif //_PA_CONTROL_PA_ERRORS_H_

