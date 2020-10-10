#ifndef TYPESETTEXT_H
#define TYPESETTEXT_H

#include <QGraphicsTextItem>
class QTextStream;
class Construct;
class Phrase;

class Text : public QGraphicsTextItem{
public:
    int type() const override {return UserType + 3;}
    Phrase* parent = nullptr;
    Construct* next = nullptr;
    Construct* prev = nullptr;
    qreal w; //width
    qreal u; //distance from horizontal to top- equal to half height for Text

private:
    static constexpr qreal margin = 1;
    static constexpr qreal subtract_from_height = 2.5;
    static constexpr qreal subtract_from_width = 1;

public:
    Text(uint8_t script_level = 0, QString str = "");
    #ifdef YAWYSIWYGEE_TEST
    virtual ~Text() override;
    static bool verify();
    static bool allFreed();
    #endif
    bool isEmpty() const;
    void setParentPhrase(Phrase* parent);
    static bool isDeepestScriptLevel(uint8_t script_level);
    void updateToTop();
    void notifyPrevUpdate();
    void startSignalToNext();
    void calculateSize();
    void populateMenu(QMenu& menu);
    void write(QTextStream& out) const;
    uint8_t getScriptLevel() const;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override final;
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
};

#endif // TYPESETTEXT_H
