#pragma once

#include "Hardware.h"
#include "BoardStatus.h"
#include "InputStatusList.h"

namespace Status
{

class StatusMgr
{
public:
    StatusMgr();

    void Initialize();

    static inline StatusMgr *Instance()
    {
        if (_statusMgr == nullptr)
        {
            _statusMgr = new StatusMgr();
        }
        return _statusMgr;
    }

    BoardStatus& GetBoardStatus() { return *_boardStatus; }
    InputStatusList& GetInputStatusList() { return *_inputStatusList; }
    
private:
    static StatusMgr *_statusMgr;
    BoardStatus * _boardStatus = nullptr;
    InputStatusList * _inputStatusList = nullptr;
    
private:
    /// @brief	Hide Copy constructor.
    StatusMgr(const StatusMgr &) = delete;

    /// @brief	Hide Assignment operator.
    StatusMgr &operator=(const StatusMgr &) = delete;

    /// @brief	Hide Move constructor.
    StatusMgr(StatusMgr &&) = delete;

    /// @brief	Hide Move assignment operator.
    StatusMgr &operator=(StatusMgr &&) = delete;
};

} // namespace Applications