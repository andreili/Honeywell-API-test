#ifndef HSCPOINT_H
#define HSCPOINT_H

#include "hsc_include.h"
#include <string>
#include <inttypes.h>
#include <functional>
#include <deque>
#include <map>

class HSCPointParam;

class HSCPoint
{
public:
    HSCPoint(int32_t point_number);
    HSCPoint(std::string name);

    bool get_data();

    void recursive(int level, std::function<void(int,HSCPoint*)> on_point);
    void params_proc(std::function<void(HSCPointParam*)> on_param);

    void dump_to_file(std::string fn);

    STApoint* get_data_ptr() { return m_data; }
    int32_t get_number() { return m_number; }
    uint16_t get_type() { return m_data->type; }
    uint16_t get_subtype() { return m_data->subtype; }
    GINDX get_parameters() { return m_data->parameters; }

    std::string get_name();

    int get_data_size();
private:
    int32_t             m_number;
    int32_t             m_index;
    std::string         m_name;
    STApoint            *m_data;
    std::deque<int32_t> m_childs;
    std::deque<HSCPointParam*>  m_params;

    uint2* decode_prmdef_ex(uint2 prmdef);
    int getch_ex(uint2 prmdef, char *value, short sizeofvalue);
    bool get_childrens();
    bool get_params();
};

#endif // HSCPOINT_H
