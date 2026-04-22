#ifndef ELIXIRCOLLECTOR_H
#define ELIXIRCOLLECTOR_H
#include "ResourceGenerator.h"

class ElixirCollector : public ResourceGenerator {
public:
    ElixirCollector(Position p);
    virtual ~ElixirCollector() {}
    Resources Collect();
    void update(Board& board) override;
};
#endif
