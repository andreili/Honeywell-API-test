#include "hscpointparam.h"
#include "hscpoint.h"
#include "log.h"
#include <fstream>

std::map<int,std::string> param_type_names = {
    {0              , "BOOL"},
    {DT_CHAR        , "CHAR"},
    {DT_INT2        , "INT2"},
    {DT_INT4        , "INT4"},
    {DT_REAL        , "REAL"},
    {DT_DBLE        , "DOUBLE"},
    {DT_HIST        , "HISTORY"},
    {DT_VAR         , "VAR"},
    {DT_ENUM        , "ENUM"},
    {DT_DATE_TIME   , "DATE_TIME"},
    {DT_STATUS      , "STATUS"},
    {DT_SRCADDR     , "SRC_ADDR"},
    {DT_DSTADDR     , "DST_ADDR"},
    {DT_SERVAR      , "SERVAR"},
    {DT_POINTREF    , "POINT_REF"},
    {DT_INT8        , "INT8"},
    {DT_TIME        , "TIME"},
    {DT_DELTATIME   , "DELTA_TIME"},
    {DT_TIMEOFDAY   , "TIME_O_FDAY"},
    {DT_ALARMHANDLE , "ALARM_HANDLE"},
    {DT_POINTREF2   , "POINT_REF2"},
};

HSCPointParam::HSCPointParam(HSCPoint *point, int param, std::string name)
    : m_point (point)
    , m_param (param)
    , m_data (nullptr)
    , m_name (name)
    , m_type (0)
{
}

void HSCPointParam::dump_to_file(std::string fn)
{
    Log::message(Log::LOG_VERBOSE, "HSCPointParam::dump_to_file(%s):\n", fn.c_str());

    if (get_data())
    {
        std::ofstream file(fn, std::ios::out | std::ios::binary | std::ios::app);
        file.write(reinterpret_cast<char*>(m_data), m_data->size);
        file.close();
    }
}

void HSCPointParam::dump_value_to_file(std::string fn)
{
    Log::message(Log::LOG_VERBOSE, "HSCPointParam::dump_value_to_file(%s):\n", fn.c_str());

    if (get_data() && (m_data->gValue != 0))
    {
        char *data = shitol(POINTSID, m_data->gValue);
        if (data != nullptr)
        {
            int size = SHMSZ(POINTSID, data);

            std::ofstream file(fn, std::ios::out | std::ios::binary | std::ios::app);
            file.write(data, size);
            file.close();
        }
    }
}

bool HSCPointParam::get_data()
{
    if (m_data != nullptr)
        return true;

    Log::message(Log::LOG_VERBOSE, "HSCPointParam::get_data()\n");

    STApoint *pnt = m_point->get_data_ptr();
    m_data = reinterpret_cast<PNTparam*>(shitoll(POINTSID, m_point->get_parameters(), reinterpret_cast<int*>(&pnt)));
    Log::message(Log::LOG_VERBOSE, "\t shitoll=%p\n", static_cast<void*>(m_data));
    if (m_data == nullptr)
    {
        //Log::message(Log::LOG_ERROR, "Unable to get point parameters data (%i)!\n", m_param);
        return false;
    }

    while (m_data->size != 0)
    {
        PRMNUM num = m_data->number;
        if (num == 0)
            break;

        if (m_param == num)
        {
            shitolu(POINTSID, m_point->get_parameters());
            return true;
        }

        m_data += m_data->size;
    }

    shitolu(1, m_point->get_parameters());
    m_data = nullptr;
    return false;
}

bool HSCPointParam::is_historisable()
{
    return false;
}

std::string HSCPointParam::get_type_name()
{
    return param_type_names[m_type];
}

bool HSCPointParam::is_param_acessible(int param_no)
{
    if (param_no == 0)
        return false;

    if ((param_no >= param_FlexStationAssignment) && (param_no <= param_ProfileListPermission))
        return false;

    if ((param_no >= param_FlexStationPropagatedAssignment) && (param_no <= param_ProfileListPropagatedPermission))
        return false;

    return true;
}
