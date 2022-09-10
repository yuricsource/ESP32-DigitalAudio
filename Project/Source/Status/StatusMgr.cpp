#include "StatusMgr.h"

namespace Status
{

StatusMgr *StatusMgr::_statusMgr;

StatusMgr::StatusMgr()
{
    if (_statusMgr == nullptr)
    {
        _statusMgr = this;
        initialize();
    }
    else
    {
        printf("Error!\n");
    }
}

void StatusMgr::initialize()
{
    _boardStatus = new BoardStatus();
    _inputStatusList = new InputStatusList();
}

} // namespace Applications
