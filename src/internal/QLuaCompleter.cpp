// QCodeEditor
#include "internal/QLuaCompleter.hpp"
#include "internal/QLanguage.hpp"

// Qt
#include <QCompleter>
#include <QFile>
#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <qnamespace.h>
#include <qtresource.h>

QLuaCompleter::QLuaCompleter(QObject *parent) : QCompleter(parent)
{
    // Setting up LUA types
    QStringList list;

    Q_INIT_RESOURCE(qcodeeditor_resources);
    QFile fl(":/languages/lua.xml");

    if (!fl.open(QIODevice::ReadOnly))
    {
        return;
    }

    QLanguage language(&fl);

    if (!language.isLoaded())
    {
        return;
    }

    auto keys = language.keys();
    for (auto &&key : keys)
    {
        auto names = language.names(key);
        list.append(names);
    }

    setModel(new QStringListModel(list, this));
    setCompletionColumn(0);
    setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setCaseSensitivity(Qt::CaseSensitive);
    setWrapAround(true);
}
