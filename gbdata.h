#ifndef GBDATA_H
#define GBDATA_H


class GBData
{
public:
    bool load();
private:
    bool load_nt(int par);
    bool is_dbservice_running();
};

#endif // GBDATA_H
