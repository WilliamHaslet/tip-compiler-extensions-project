#include "TipArray.h"
#include "TipTypeVisitor.h"

TipArray::TipArray(std::shared_ptr<TipType> arrayType)
  : TipCons(std::move(std::vector<std::shared_ptr<TipType>> {arrayType})) { }


std::ostream& TipArray::print(std::ostream &out) const {
    out << "[";
    bool first = true;
    for(auto &init : arguments) {
        if(first) {
            out << *init;
            first = false;
            continue;
        }
        out << "," << *init;
    }
    out << "]";
    return out;
}

// This does not obey the semantics of alpha init values 
bool TipArray::operator==(const TipType &other) const {
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
}

bool TipArray::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::vector<std::shared_ptr<TipType>>& TipArray::getTypes() {
    return arguments;
}


void TipArray::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto a : arguments) {
       a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}