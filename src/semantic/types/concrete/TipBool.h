#pragma once

#include "TipType.h"
#include "TipCons.h"
#include <string>

/*!
 * \class TipBool
 *
<<<<<<< HEAD
 * \brief A proper type representing an bool
=======
 * \brief A proper type representing a bool
>>>>>>> f9e11898a1f4d22cfa5b6b768c25d875a70820e3
 */
class TipBool: public TipCons {
public:
    TipBool();

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;
<<<<<<< HEAD
};

=======
};
>>>>>>> f9e11898a1f4d22cfa5b6b768c25d875a70820e3
