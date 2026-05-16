// QCodeEditor
#include "internal/QLuaCompleter.hpp"
#include "internal/QCompletingSymbol.hpp"
#include "internal/QCompletingSymbolModel.hpp"
#include "internal/QLanguage.hpp"

// Qt
#include <QCompleter>
#include <QFile>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qtresource.h>
#include <utility>

namespace
{
SymbolsList loadKeywordsFromResouces()
{
    QFile fl(":/languages/lua.xml");
    if (!fl.open(QIODevice::ReadOnly))
    {
        return {};
    }

    QLanguage language(&fl);

    if (!language.isLoaded())
    {
        return {};
    }

    SymbolsList list;

    auto keys = language.keys();
    for (auto &&key : keys)
    {
        auto names = language.names(key);
        for (auto &&name : names)
        {
            static const QString kTooltipForKeyword = "Lua Keyword";
            list.append({std::move(name), kTooltipForKeyword});
        }
    }
    return list;
}
} // namespace

QLuaCompleter::QLuaCompleter(QObject *parent) : QCompleter(parent), m_model(new CompletingSymbolModel(this))
{
    Q_INIT_RESOURCE(qcodeeditor_resources);

    m_model->setStaticSymbols(loadKeywordsFromResouces());

    setModel(m_model);
    setCompletionColumn(0);
    setModelSorting(QCompleter::UnsortedModel);
    setCaseSensitivity(Qt::CaseSensitive);
    setWrapAround(true);
}

void QLuaCompleter::updateDynamicSymbols(const SymbolsList &symbols)
{
    m_model->setDynamicSymbols(symbols);
}
