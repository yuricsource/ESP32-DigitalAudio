#include "ApplicationMgr.h"

namespace Applications
{

ApplicationMgr *ApplicationMgr::_applications;

ApplicationMgr::ApplicationMgr()
{
    if (_applications == nullptr)
    {
        _applications = this;
        initialize();
    }
    else
    {
        assert(0);
    }
}

void ApplicationMgr::initialize()
{
    _menu = new MenuService();
    _ai = new AIService();
    _inputScan = new InputScanService();
    _audioAnalyzer = new AudioAnalyzer();
    _audioPlayer = new AudioPlayer();
}

} // namespace Applications
