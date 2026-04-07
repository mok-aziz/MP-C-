#ifndef RESOURCES_H
#define RESOURCES_H

class Resources {
private:
    int gold;
    int elixir;
public:
    Resources(int gold = 400, int elixir = 400);
    void addGold(int amount);
    void addElixir(int amount);
    bool spendGold(int amount);
    bool spendElixir(int amount);
    int  getGold()   const;
    int  getElixir() const;
    bool canAfford(const Resources& cost) const;
    void consume(const Resources& cost);
};
#endif