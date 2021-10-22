#pragma once

#include "TipCons.h"
#include "TipType.h"
#include <string>
#include <vector>
#include <ostream>

/*!
 * \class TipArray
 *
 * \brief A proper type representing an array
 */
class TipArray: public TipCons {
public:
    TipArray(std::shared_ptr<TipType> arrayType);

    std::vector<std::shared_ptr<TipType>>& getTypes();
    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;
};