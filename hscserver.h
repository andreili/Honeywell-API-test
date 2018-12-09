#ifndef HSCSERVER_H
#define HSCSERVER_H

#include <string>
#include <windows.h>

class HSCServer
{
public:
    HSCServer();

    static std::string get_reg_entry_silent(std::string name);
private:
    static HSCServer*   m_inst;
    bool                m_have_name;
    std::string         m_name;

    static std::string get_name_silent();
    static std::string get_general_reg_entry_silent(HANDLE handle, char *rname, std::string subname, std::string val_name);
};

#endif // HSCSERVER_H
