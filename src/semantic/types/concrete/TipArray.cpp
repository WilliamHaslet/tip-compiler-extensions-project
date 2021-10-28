#include "TipArray.h"
#include "TipTypeVisitor.h"

TipArray::TipArray(std::shared_ptr<TipType> arrayType)
  : TipCons(std::move(std::vector<std::shared_ptr<TipType>> {arrayType})) { }


std::ostream& TipArray::print(std::ostream &out) const {
    out << "[] ";
    for(auto &init : arguments) {
        out << *init;
    }
    return out;
}

bool TipArray::operator==(const TipType &other) const {
    /*
    auto tipArray = dynamic_cast<const TipArray *>(&other);
    if(!tipArray) {
        return false;
    }

    if(arity() != tipArray->arity()) {
        return false;
    }

    for(int i = 0; i < arity(); i++) {
        if(*(arguments.at(i)) != *(tipArray->arguments.at(i))) {
            return false;
        }
    }

    return true;
    */

    auto otherTipArray = dynamic_cast<const TipArray *>(&other);
    if(!otherTipArray) {
        return false;
    }

    return *arguments.front() == *otherTipArray->getType();
}

bool TipArray::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::shared_ptr<TipType> TipArray::getType() const{
    return arguments.front();
}


void TipArray::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto a : arguments) {
       a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}