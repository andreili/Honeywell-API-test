#include "hscserver.h"
#include "log.h"

HSCServer* HSCServer::m_inst = nullptr;

const char* server_names[15] = {
    "SCAN 3000",
    "XSM",
    "XFi",
    "Experion PKS Server",
    "ProcessLogixServer",
    "EBI Server",
    "EBI Server",
    "EBI Server",
    "Experion PKS Server",
    "Experion PKS Server",
    "Experion PKS Server",
    "Experion PKS Server",
    "Experion PKS Console Station",
    "Experion PKS Server",
    "Experion PKS Server",
};

HSCServer::HSCServer()
    : m_have_name (false)
{
    m_inst = this;
}

std::string HSCServer::get_reg_entry_silent(std::string name)
{
    Log::message(Log::LOG_VERBOSE, "CALL HSCServer::get_reg_entry_silent\n");
    std::string ret = get_name_silent();
    if (ret.size() > 0)
        return get_general_reg_entry_silent(HKEY_LOCAL_MACHINE, "Software\\Honeywell", ret, name);
    else
        return "";
}

std::string HSCServer::get_name_silent()
{
    if (m_inst->m_have_name)
        return m_inst->m_name;

    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Honeywell", 0, KEY_READ, &key))
        return "";
    DWORD type = 2;
    unsigned char data[260];
    DWORD cbData = 259;
    if (RegQueryValueExA(key, "HSCServerType", nullptr, &type, data, &cbData) == 0)
    {
        RegCloseKey(key);
        m_inst->m_have_name = true;
        m_inst->m_name = std::string(reinterpret_cast<char*>(data));
        return m_inst->m_name;
    }

    bool finded = false;
    int name_idx = 0;
    HKEY key2;
    for ( ; name_idx<15 ; ++name_idx)
    {
        if (RegOpenKeyExA(key, server_names[name_idx], 0, KEY_READ, &key2) == 0)
        {
            finded = true;
            break;
        }
    }

    if (finded)
        RegCloseKey(key2);

    RegCloseKey(key);

    if (finded)
    {
        m_inst->m_have_name = true;
        m_inst->m_name = std::string(server_names[name_idx]);
        return m_inst->m_name;
    }
    else
        return "";
}

std::string HSCServer::get_general_reg_entry_silent(HANDLE handle, char *rname, std::string subname, std::string val_name)
{
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, rname, 0, KEY_READ, &key))
        return "";

    HKEY key2;
    if (RegOpenKeyExA(key, subname.c_str(), 0, KEY_READ, &key2))
        return "";

    DWORD type = 2;
    unsigned char data[260];
    DWORD cbData = 259;
    if (RegQueryValueExA(key2, val_name.c_str(), nullptr, &type, data, &cbData))
    {
        RegCloseKey(key2);
        RegCloseKey(key);
        return "";
    }
    RegCloseKey(key2);
    RegCloseKey(key);

    return std::string(reinterpret_cast<char*>(data));
}
