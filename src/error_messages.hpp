#ifndef error_hh
#define error_hh

#include <stdexcept>

class NotImplementedError : public std::logic_error {
public:
    NotImplementedError(std::string what)
        : std::logic_error(what + " not yet implemented."){};
};

class FileNotFoundError : public std::logic_error {
public:
    FileNotFoundError(std::string what)
        : std::logic_error("Failed to find input file " + what){};
};

class InputFileError : public std::logic_error {
public:
    InputFileError(std::string what)
        : std::logic_error("Failed to find " + what + " entry from the input file."){};
};

class H5FileCloseError : public std::logic_error {
public:
    H5FileCloseError(std::string what)
        : std::logic_error("Failed to close " + what + "."){};
};

class H5WriteError : public std::logic_error {
public:
    H5WriteError(std::string what, std::string to)
        : std::logic_error("Failed to write " + what + "to " + to + "."){};
};

#endif
