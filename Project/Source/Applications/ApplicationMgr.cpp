#include "ApplicationMgr.h"

namespace Applications
{

ApplicationMgr *ApplicationMgr::_applications;

ApplicationMgr::ApplicationMgr()
{
    if (_applications == nullptr)
    {
        _applications = this;
    }
    else
    {
        assert(0);
    }
}

void ApplicationMgr::Initialize()
{
    _menu = new MenuService();
    _ai = new AIService();
}

} // namespace Applications
