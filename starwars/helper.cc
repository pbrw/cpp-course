#include "helper.h"

bool operator<(Property &a, Property &b) {
    return a.unit < b.unit;
}

bool operator==(Property &&a, Property &&b) {
    return a.unit == b.unit;
}

bool operator>(Property &a, Property &b) {
    return a.unit > b.unit;
}

bool operator<=(Property &a, Property &b) {
    return a.unit <= b.unit;
}

bool operator>=(Property &a, Property &&b) {
    return a.unit >= b.unit;
}

Property::Type operator%(Property &a, Property &&b) {
    return a.unit % b.unit;
}

Property::Type operator%(Property &a, Property &b) {
    return a.unit % b.unit;
}

std::ostream &operator<<(std::ostream &os, const Property &op) {
    return os << op.unit;
}
