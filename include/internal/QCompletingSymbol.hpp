#pragma once

#include <QList>
#include <QString>
#include <tuple>

/// @brief Symbol used with completers.
struct CompletingSymbol
{
    decltype(auto) tie() const
    {
        return std::tie(name, tooltip);
    }

    bool operator==(const CompletingSymbol &other) const
    {
        return tie() == other.tie();
    }

    /// @brief Symbol itself to complete.
    QString name;
    /// @brief Tooltip to show related to this symbol if any.
    QString tooltip;
};

using SymbolsList = QList<CompletingSymbol>;
