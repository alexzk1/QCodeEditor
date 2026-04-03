#pragma once

#include <QWidget>

class QCodeEditor;
class QLineEdit;
class QSyntaxStyle;
class QPushButton;
class QLabel;

/**
 * @brief Class that provides a search widget for QCodeEditor.
 */
class QSearchWidget : public QWidget
{
    Q_OBJECT;

public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent QCodeEditor widget.
     */
    explicit QSearchWidget(QCodeEditor *parent = nullptr);

    // Disable copying
    QSearchWidget(const QSearchWidget &) = delete;
    QSearchWidget &operator=(const QSearchWidget &) = delete;

    /**
     * @brief Method for setting syntax style object.
     * @param style Pointer to syntax style.
     */
    void setSyntaxStyle(QSyntaxStyle *style);

    /**
     * @brief Shows the search widget and focuses the input field.
     */
    void showSearch();

    /**
     * @brief Hides the search widget and clears highlights.
     */
    void hideSearch();

    /**
     * @brief Returns true if search widget is currently visible.
     */
    bool isSearchVisible() const;

    /**
     * @brief Get current search text.
     */
    QString searchText() const;

signals:
    /**
     * @brief Signal emitted when search text changes.
     */
    void searchTextChanged(const QString &text);

    /**
     * @brief Signal emitted when user requests next match.
     */
    void findNext();

    /**
     * @brief Signal emitted when user requests previous match.
     */
    void findPrevious();

    /**
     * @brief Signal emitted when search is closed.
     */
    void searchClosed();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void updateStyle();
    void updateResultCount(int current, int total);

    QCodeEditor *m_codeEditor;
    QLineEdit *m_searchInput;
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    QPushButton *m_closeButton;
    QLabel *m_resultLabel;
    QSyntaxStyle *m_syntaxStyle;

    friend class QCodeEditor;
};
