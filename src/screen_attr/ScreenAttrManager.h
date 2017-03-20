#ifndef SCREENATTRMANAGER_H
#define SCREENATTRMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "ICommand.h"
#include "IInit.h"
#include "IMainWnd.h"
#include "ISetting.h"

class ScreenAttrManager : public QObject, public IPlugin, public IInit, public ICommand
{
    Q_OBJECT

    virtual ~ScreenAttrManager() = default;

    virtual Commands commands() override;
    virtual void init(const std::list<IPlugin*> &plugins) override;

    IMainWnd *mainWnd = nullptr;
    ISetting *setting = nullptr;
};

#endif // SCREENATTRMANAGER_H
