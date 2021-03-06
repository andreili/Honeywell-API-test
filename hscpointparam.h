#ifndef HSCPOINTPARAM_H
#define HSCPOINTPARAM_H

#include <hsc_include.h>
#include <string>

class HSCPoint;

enum class EPrmType
{
    UNK = 0,
    CHAR = 1,
    INT2 = 2,
    INT4 = 3,
    REAL = 4,
    DBLE = 5,
    HIST = 6,
    VAR = 7,
    ENUM = 8,
    DATE_TYIME = 9,
    STATUS = 10,
    SRCADDR = 11,
    DSTADDR = 12,
    SERVAR = 13,
    POINTREF = 14,
    INT8 = 15,
    TIME = 16,
    DELTATIME = 17,
    TIMEOFDAY = 18,
    ALARMAHANDLE = 19,
    POINTREF2 = 20,
    NONE = 65535
};

class HSCPointParam
{
public:
    HSCPointParam(HSCPoint *point, int param, std::string name);

    void dump_to_file(std::string fn);
    void dump_value_to_file(std::string fn);

    bool get_data();

    bool is_historisable();

    int get_index() { return m_param; }

    EPrmType get_type() { return static_cast<EPrmType>(m_type); }
    void set_type(uint16_t type) { m_type = type; }
    std::string get_type_name();

    std::string get_name() { return m_name; }

    HSCPoint* get_parent() { return m_point; }

    void set_val_pnt(PARvalue *val) { m_value = val; }
    PARvalue* get_val_pnt() { return m_value; }

    static bool is_param_acessible(int param_no);
private:
    HSCPoint    *m_point;
    int         m_param;
    PNTparam    *m_data;
    std::string m_name;
    uint16_t    m_type;
    PARvalue    *m_value;
};

#endif // HSCPOINTPARAM_H
