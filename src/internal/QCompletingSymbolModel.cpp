#include "internal/QCompletingSymbolModel.hpp"

#include "internal/QCompletingSymbol.hpp"
#include <QAbstractTableModel>
#include <QObject>
#include <QSet>
#include <algorithm>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <utility>

namespace
{
// Makes list unique case-sensitive, then sort it case-insensitive by "name" field.
SymbolsList sortUniqueKeywords(const SymbolsList &input)
{
    SymbolsList uniqueList;
    QSet<QString> seen;
    for (const auto &s : input)
    {
        if (!seen.contains(s.name))
        {
            uniqueList.append(s);
            seen.insert(s.name);
        }
    }
    std::sort(uniqueList.begin(), uniqueList.end(), [](const CompletingSymbol &a, const CompletingSymbol &b) {
        return QString::compare(a.name, b.name, Qt::CaseInsensitive) < 0;
    });

    return uniqueList;
};
} // namespace

void CompletingSymbolModel::accessStaticSymbols(const SymbolsAccessT &accessFunc)
{
    beginResetModel();
    accessList(m_staticSymbols, accessFunc);
    rebuildCombinedList();
    endResetModel();
}

void CompletingSymbolModel::accessDynamicSymbols(const SymbolsAccessT &accessFunc)
{
    beginResetModel();
    accessList(m_dynamicSymbols, accessFunc);

    // Remove anything already present in static data.
    if (!m_staticSymbols.empty())
    {
        QSet<QString> staticNames;
        for (const auto &s : std::as_const(m_staticSymbols))
        {
            staticNames.insert(s.name);
        }
        m_dynamicSymbols.erase(
            std::remove_if(m_dynamicSymbols.begin(), m_dynamicSymbols.end(),
                           [&staticNames](const CompletingSymbol &s) { return staticNames.contains(s.name); }),
            m_dynamicSymbols.end());
    }
    rebuildCombinedList();
    endResetModel();
}

int CompletingSymbolModel::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return static_cast<int>(m_combinedSymbols.size());
}

int CompletingSymbolModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    return 1;
}

QVariant CompletingSymbolModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_combinedSymbols.size())
    {
        return {};
    }

    const auto &symbol = m_combinedSymbols[index.row()];

    switch (role)
    {
    case Qt::ToolTipRole:
        return symbol.tooltip;
    case Qt::DisplayRole:
        [[fallthrough]];
    case Qt::EditRole:
        return symbol.name;
    default:
        break;
    }
    return {};
}

void CompletingSymbolModel::accessList(SymbolsList &src, const SymbolsAccessT &accessFunc)
{
    accessFunc(src);
    src = sortUniqueKeywords(src);
}

void CompletingSymbolModel::rebuildCombinedList()
{
    m_combinedSymbols.clear();
    m_combinedSymbols.reserve(m_staticSymbols.size() + m_dynamicSymbols.size());
    m_combinedSymbols.append(m_staticSymbols);
    m_combinedSymbols.append(m_dynamicSymbols);
}
