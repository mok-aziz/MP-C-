#ifndef GOLDMINE_H
#define GOLDMINE_H
#include "ResourceGenerator.h"

class GoldMine : public ResourceGenerator {
public:
    GoldMine(Position p);
    virtual ~GoldMine() {}
    Resources Collect();
    void update(Board& board) override;
};
#endif