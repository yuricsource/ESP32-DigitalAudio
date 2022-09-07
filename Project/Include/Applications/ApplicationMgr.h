#pragma once    
#include "Hardware.h"
#include "thread.hpp"
#include "MenuService.h"
#include "AIService.h"

namespace Applications
{

class ApplicationMgr
{
public:
    ApplicationMgr();

    void Initialize();

    static inline ApplicationMgr *Instance()
    {
        if (_applications == nullptr)
        {
            _applications = new ApplicationMgr();
        }
        return _applications;
    }

    MenuService& GetMenu() {return *_menu;}
    AIService& GetAI() {return *_ai;}

private:
    static ApplicationMgr *_applications;
    MenuService *_menu;
    AIService *_ai;
private:
    /// @brief	Hide Copy constructor.
    ApplicationMgr(const ApplicationMgr &) = delete;

    /// @brief	Hide Assignment operator.
    ApplicationMgr &operator=(const ApplicationMgr &) = delete;

    /// @brief	Hide Move constructor.
    ApplicationMgr(ApplicationMgr &&) = delete;

    /// @brief	Hide Move assignment operator.
    ApplicationMgr &operator=(ApplicationMgr &&) = delete;
};

} // namespace Applications
