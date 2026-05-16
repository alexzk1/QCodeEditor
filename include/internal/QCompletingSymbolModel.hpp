#pragma once

#include "internal/QCompletingSymbol.hpp"
#include <QAbstractTableModel>
#include <QObject>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qtmetamacros.h>

/// @brief This is generic model for QCompleter which provides dynamic & static data separated.
/// Dynamic data is what user types and you gather somehow, static data is keywords predefined list.
/// Also it may provide tooltip too as part of CompletingSymbol.
/// @note Warning! Model is not sorted globaly, it keeps statics sorted + dynamics sorted, which means
/// QCompleter must NOT relay on sorted list (setModelSorting()).
class CompletingSymbolModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    explicit CompletingSymbolModel(QObject *parent) : QAbstractTableModel(parent)
    {
    }

    ~CompletingSymbolModel() override = default;
    CompletingSymbolModel(const CompletingSymbolModel &) = delete;
    CompletingSymbolModel &operator=(const CompletingSymbolModel &) = delete;
    CompletingSymbolModel(CompletingSymbolModel &&) = delete;
    CompletingSymbolModel &operator=(CompletingSymbolModel &&) = delete;

    /// @brief Set static model data.
    void setStaticSymbols(const SymbolsList &staticSymbols);
    /// @brief Set dynamic model data.
    void setDynamicSymbols(const SymbolsList &dynamicSymbols);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  private:
    void rebuildCombinedList();

    QList<CompletingSymbol> m_staticSymbols;
    QList<CompletingSymbol> m_dynamicSymbols;
    QList<CompletingSymbol> m_combinedSymbols;
};
