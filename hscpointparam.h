#ifndef HSCPOINTPARAM_H
#define HSCPOINTPARAM_H

#include <hsc_include.h>
#include <string>

class HSCPoint;

class HSCPointParam
{
public:
    HSCPointParam(HSCPoint *point, int param, std::string name);

    void dump_to_file(std::string fn);
    void dump_value_to_file(std::string fn);

    bool get_data();

    bool is_historisable();

    int get_index() { return m_param; }
    int get_type() { if (get_data()) return m_data->type; else return 0; }

    std::string get_name() { return m_name; }

    HSCPoint* get_parent() { return m_point; }

    static bool is_param_acessible(int param_no);
private:
    HSCPoint    *m_point;
    int         m_param;
    PNTparam    *m_data;
    std::string m_name;
};

#endif // HSCPOINTPARAM_H
