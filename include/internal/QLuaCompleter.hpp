#pragma once

// Qt
#include <QCompleter> // Required for inheritance
#include <QObject>
#include <qtmetamacros.h>

class CompletingSymbolModel;

/**
 * @brief Class, that describes completer with
 * Lua specific types and functions.
 */
class QLuaCompleter : public QCompleter
{
    Q_OBJECT;

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent QObject.
     */
    explicit QLuaCompleter(QObject *parent = nullptr);

    CompletingSymbolModel *symbolModel();

  private:
    CompletingSymbolModel *m_model{};
};
