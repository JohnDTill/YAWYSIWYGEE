#ifndef TYPESETINLINECONSTRUCT_H
#define TYPESETINLINECONSTRUCT_H

#include <QGraphicsObject>
class Line;
class SubPhrase;
class TypesetScene;
class Text;
class QTextStream;

class Construct : public QGraphicsObject{
public:
    int type() const override {return UserType + 4;}
    Text* prev = nullptr;
    Text* next = nullptr;
    qreal w; //width
    qreal u; //distance from horizontal to top
    qreal d; //distance from horizontal to bottom

public:
    Construct();
    #ifdef YAWYSIWYGEE_TEST
    virtual ~Construct() override;
    static bool verify();
    static bool allFreed();
    #endif
    qreal h() const;
    virtual void deletePostorder() = 0;
    virtual void select() = 0;
    virtual void updateLayout() = 0;
    virtual void notifyPrevUpdate(){/*DO NOTHING*/}
    virtual void notifyPrevPrevUpdate(Construct*){/*DO NOTHING*/}
    void updateToTop();

    virtual SubPhrase* front() const {return nullptr;}
    virtual SubPhrase* back() const {return nullptr;}
    virtual Text* textRight(const SubPhrase* caller) const = 0;
    virtual Text* textLeft(const SubPhrase* caller) const = 0;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const = 0;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const = 0;
    Line& getLine() const;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr);
    virtual void write(QTextStream& out) const = 0;
    virtual void populateTextPointers(std::vector<Text*>& text_pointers) const = 0;
    static void setupPainter(QPainter* painter, const QStyleOptionGraphicsItem* options);

protected:
    virtual QRectF boundingRect() const override final;
    TypesetScene* typesetDocument() const;
};

void link(Text* t, Construct* c);
void link(Construct* c, Text* t);

class TerminalConstruct : public Construct{
public:
    virtual void deletePostorder() override final;
    virtual void select() override;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase*) const override final;
    virtual Text* textLeft(const SubPhrase*) const override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void populateTextPointers(std::vector<Text*>&) const override final;
};

class UnaryConstruct : public Construct{
protected:
    SubPhrase* child;

public:
    UnaryConstruct(SubPhrase* c);
    virtual void deletePostorder() override final;
    virtual void select() override;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase*) const override final;
    virtual Text* textLeft(const SubPhrase*) const override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void populateTextPointers(std::vector<Text*>& text_pointers) const override final;
};

class BinaryConstruct : public Construct{
protected:
    SubPhrase* first;
    SubPhrase* second;

public:
    BinaryConstruct(SubPhrase* f, SubPhrase* s);
    virtual void deletePostorder() override final;
    virtual void select() override;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase* caller) const override final;
    virtual Text* textLeft(const SubPhrase* caller) const override final;
    virtual void populateTextPointers(std::vector<Text*>& text_pointers) const override final;
};

class NaryConstruct : public Construct{
protected:
    std::vector<SubPhrase*> children;

public:
    NaryConstruct(const std::vector<SubPhrase*> c);
    virtual void deletePostorder() override final;
    virtual void select() override final;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase* caller) const override final;
    virtual Text* textLeft(const SubPhrase* caller) const override final;
    virtual void populateTextPointers(std::vector<Text*>& text_pointers) const override final;
};

#endif // TYPESETINLINECONSTRUCT_H
