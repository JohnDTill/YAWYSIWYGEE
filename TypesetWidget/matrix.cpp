#include "matrix.h"

#include "algorithm.h"
#include "cursor.h"
#include "document.h"
#include "globals.h"
#include <QMenu>
#include <QPainter>

namespace Typeset{

Matrix::Matrix(const std::vector<SubPhrase*>& c, minor_integer rows, minor_integer cols)
    : NaryConstruct(c),
      rows(rows),
      cols(cols) {
    updateLayout();
}

void Matrix::updateLayout(){
    std::vector<double> W(cols,0);
    std::vector<double> U(rows,0);
    std::vector<double> D(rows,0);

    //Loop over elements to calculate W, U, and D.
    for(major_integer i = 0; i < rows; i++){
        for(major_integer j = 0; j < cols; j++){
            SubPhrase* e = children[getFlatIndex(i,j)];
            W[j] = qMax( W[j], e->w );
            U[i] = qMax( U[i], e->u );
            D[i] = qMax( D[i], e->d );
        }
    }

    //Layout elements and calculate size
    qreal x = text_hoffset + bracket_hoffset;
    qreal y = bracket_voffset;
    for(major_integer i = 0; i < rows; i++){
        x = text_hoffset + bracket_hoffset;
        for(major_integer j = 0; j < cols; j++){
            Phrase* e = children[getFlatIndex(i,j)];
            e->setPos( x+(W[j]-e->w)/2, y+U[i]-e->u );
            x += W[j]+hspace;
        }
        y += U[i]+D[i]+vspace;
    }

    w = x + text_hoffset - hspace + bracket_hoffset;
    h = y - vspace + bracket_voffset;
    u = d = h/2;
}

Text* Matrix::textUp(const SubPhrase* caller, qreal x) const{
    major_integer i = caller->child_id;
    return (i >= cols) ? Algorithm::textAtSetpoint(*children[i-cols], x) : prev;
}

Text* Matrix::textDown(const SubPhrase* caller, qreal x) const{
    major_integer i = caller->child_id + cols;
    return (i < children.size()) ? Algorithm::textAtSetpoint(*children[i], x) : next;
}

void Matrix::populateMenu(QMenu& menu, const SubPhrase* caller){
    if(caller == nullptr){
        Construct::populateMenu(menu);
        return;
    }

    active = caller->child_id;

    menu.addSeparator();
    QAction* createRowBelow = menu.addAction("Matrix: Create row below");
    QAction* createRowAbove = menu.addAction("Matrix: Create row above");
    QAction* createColRight = menu.addAction("Matrix: Create col right");
    QAction* createColLeft = menu.addAction("Matrix: Create col left");
    QAction* deleteRow = menu.addAction("Matrix: Delete row");
    QAction* deleteCol = menu.addAction("Matrix: Delete col");
    connect(createRowBelow, SIGNAL(triggered()), this, SLOT(createRowBelow()));
    connect(createRowAbove, SIGNAL(triggered()), this, SLOT(createRowAbove()));
    connect(createColRight, SIGNAL(triggered()), this, SLOT(createColRight()));
    connect(createColLeft, SIGNAL(triggered()), this, SLOT(createColLeft()));
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(deleteRow()));
    connect(deleteCol, SIGNAL(triggered()), this, SLOT(deleteCol()));
    deleteRow->setEnabled(rows > 1);
    deleteCol->setEnabled(cols > 1);
}

void Matrix::write(QTextStream& out) const{
    out << ESCAPE << QChar(8862);
    out << OPEN << rows << CLOSE << OPEN << cols << CLOSE;
    for(SubPhrase* c : children) c->write(out);
}

void Matrix::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);

    painter->drawLine(QLineF(0, 0, bracket_width, 0));
    painter->drawLine(QLineF(0, 0, 0, h));
    painter->drawLine(QLineF(0, h, bracket_width, h));

    qreal right_bracket_x = w - bracket_width;

    painter->drawLine(QLineF(right_bracket_x, 0, w, 0));
    painter->drawLine(QLineF(w, 0, w, h));
    painter->drawLine(QLineF(right_bracket_x, h, w, h));
}

major_integer Matrix::getFlatIndex(major_integer row, major_integer col) const{
    return row*cols + col;
}

major_integer Matrix::getRow(major_integer flat_index) const{
    return flat_index / cols;
}

major_integer Matrix::getCol(major_integer flat_index) const{
    return flat_index % cols;
}

void Matrix::insertRow(major_integer row, const std::vector<SubPhrase*> inserted){
    major_integer start = row*cols;

    //Shift everything beyond this row down
    major_integer old_size = static_cast<major_integer>(children.size());
    children.resize(children.size() + cols);
    for(major_integer i = old_size-1; i >= start; i--){
        children[i+cols] = children[i];
        children[i]->child_id += cols;
        if(i==0) break;
    }

    //Initialize the new row
    for(major_integer i = start; i < start+cols; i++){
        children[i] = inserted[i-start];
        children[i]->child_id = i;
        children[i]->show();
    }

    rows++;
    updateToTop();
}

void Matrix::removeRow(major_integer row){
    major_integer start = row*cols;

    //Hide
    for(major_integer i = start; i < start+cols; i++)
        children[i]->hide();

    //Move subsequent elements up
    for(major_integer i = start+cols; i < children.size(); i++){
        children[i-cols] = children[i];
        children[i]->child_id = i-cols;
    }

    children.resize(children.size() - cols);

    rows--;
    updateToTop();
}

void Matrix::insertCol(major_integer col, const std::vector<SubPhrase*> inserted){
    children.resize(children.size() + rows);

    //Move the old data
    for(major_integer i = rows-1; i < rows; i--){
        for(major_integer j = cols-1; j >= col; j--){
            major_integer index = getFlatIndex(i,j);
            children[index+i+1] = children[index];
            children[index]->child_id = index+i+1;
            if(j==0) break;
        }
        for(major_integer j = col-1; j < col; j--){
            major_integer index = getFlatIndex(i,j);
            children[index+i] = children[index];
            children[index]->child_id = index+i;
        }
    }

    cols++;

    //Initialize the new column
    for(major_integer i = col; i < children.size(); i += cols){
        children[i] = inserted[i/cols];
        children[i]->child_id = i;
        children[i]->show();
    }

    updateToTop();
}

void Matrix::removeCol(major_integer col){
    for(major_integer i = 0; i < rows; i++)
        children[getFlatIndex(i,col)]->hide();

    //Move subsequent elements left
    for(major_integer i = 0; i < rows; i++){
        for(major_integer j = 0; j < col; j++){
            major_integer new_index = getFlatIndex(i,j) - i;
            major_integer old_index = getFlatIndex(i,j);
            children[new_index] = children[old_index];
            children[new_index]->child_id = new_index;
        }

        for(major_integer j = col+1; j < cols; j++){
            major_integer new_index = getFlatIndex(i,j) - i - 1;
            major_integer old_index = getFlatIndex(i,j);
            children[new_index] = children[old_index];
            children[new_index]->child_id = new_index;
        }
    }

    children.resize(children.size() - rows);

    cols--;
    updateToTop();
}

void Matrix::createRowBelow(){
    typesetDocument()->undo_stack->push( new AddRow(*this, getRow(active)+1 ) );
}

void Matrix::createRowAbove(){
    typesetDocument()->undo_stack->push( new AddRow(*this, getRow(active) ) );
}

void Matrix::createColRight(){
    typesetDocument()->undo_stack->push( new AddCol(*this, getCol(active)+1 ) );
}

void Matrix::createColLeft(){
    typesetDocument()->undo_stack->push( new AddCol(*this, getCol(active) ) );
}

void Matrix::deleteRow(){
    typesetDocument()->undo_stack->push( new RemoveRow(*this, getRow(active) ) );
}

void Matrix::deleteCol(){
    typesetDocument()->undo_stack->push( new RemoveCol(*this, getCol(active) ) );
}

Matrix::AddRow::AddRow(Matrix& mat, major_integer row)
    : mat(mat),
      row(row) {
    uint8_t script_level = mat.children.front()->front->getScriptLevel();

    for(std::vector<SubPhrase*>::size_type i = 0; i < mat.cols; i++){
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        data.push_back( new SubPhrase(t) );
        data[i]->setParentConstruct(mat);
    }
}

Matrix::AddRow::~AddRow(){
    if(active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Matrix::AddRow::redo(){
    active = true;
    mat.insertRow(row, data);
}

void Matrix::AddRow::undo(){
    active = false;
    mat.removeRow(row);
}

Matrix::AddCol::AddCol(Matrix& mat, major_integer col)
    : mat(mat),
      col(col) {
    uint8_t script_level = mat.children.front()->front->getScriptLevel();

    for(std::vector<SubPhrase*>::size_type i = 0; i < mat.rows; i++){
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        data.push_back( new SubPhrase(t) );
        data[i]->setParentConstruct(mat);
    }
}

Matrix::AddCol::~AddCol(){
    if(active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Matrix::AddCol::redo(){
    active = true;
    mat.insertCol(col, data);
}

void Matrix::AddCol::undo(){
    active = false;
    mat.removeCol(col);
}

Matrix::RemoveRow::RemoveRow(Matrix& mat, major_integer row)
    : mat(mat),
      row(row) {
    major_integer start = mat.cols*row;
    for(std::vector<SubPhrase*>::size_type i = start; i < start + mat.cols; i++)
        data.push_back(mat.children[i]);
}

Matrix::RemoveRow::~RemoveRow(){
    if(!active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Matrix::RemoveRow::redo(){
    active = true;
    mat.removeRow(row);
}

void Matrix::RemoveRow::undo(){
    active = false;
    mat.insertRow(row, data);
}

Matrix::RemoveCol::RemoveCol(Matrix& mat, major_integer col)
    : mat(mat),
      col(col) {
    for(major_integer i = 0; i < mat.rows; i++)
        data.push_back(mat.children[ mat.getFlatIndex(i,col) ]);
}

Matrix::RemoveCol::~RemoveCol(){
    if(!active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Matrix::RemoveCol::redo(){
    active = true;
    mat.removeCol(col);
}

void Matrix::RemoveCol::undo(){
    active = false;
    mat.insertCol(col, data);
}

}
