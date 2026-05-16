#include "internal/QCompletingSymbolModel.hpp"

#include "internal/QCompletingSymbol.hpp"
#include <QAbstractTableModel>
#include <QObject>
#include <QSet>
#include <algorithm>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <utility>

void CompletingSymbolModel::setStaticSymbols(const SymbolsList &staticSymbols)
{
    beginResetModel();
    m_staticSymbols = staticSymbols;
    rebuildCombinedList();
    endResetModel();
}

void CompletingSymbolModel::setDynamicSymbols(const SymbolsList &dynamicSymbols)
{
    beginResetModel();
    m_dynamicSymbols = dynamicSymbols;
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

void CompletingSymbolModel::rebuildCombinedList()
{
    m_combinedSymbols.clear();

    const auto processList = [](const SymbolsList &input) {
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

    auto sortedStatic = processList(m_staticSymbols);
    auto sortedDynamic = processList(m_dynamicSymbols);

    m_combinedSymbols.reserve(sortedStatic.size() + sortedDynamic.size());
    m_combinedSymbols.append(std::move(sortedStatic));
    m_combinedSymbols.append(std::move(sortedDynamic));
}
