#include "StatusMgr.h"

namespace Status
{

StatusMgr *StatusMgr::_statusMgr;

StatusMgr::StatusMgr()
{
    if (_statusMgr == nullptr)
    {
        _statusMgr = this;
    }
    else
    {
        printf("Error!\n");
    }
}

void StatusMgr::Initialize()
{
    _boardStatus = new BoardStatus();
    _inputStatusList = new InputStatusList();
}

} // namespace Applications
