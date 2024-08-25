//
// Created by meisj on 21/08/2024.
//

#ifndef RAYLIBANDROID_PLATFORM_HPP
#define RAYLIBANDROID_PLATFORM_HPP

#include "entity.hpp"
#include "config.hpp"

class Platform : public Entity{
private:
    Color color;
public:
    Platform() {}
    Platform(float posX, float posY, float width, float height, Color col) : Entity(posX, posY, width, height), color{col} {
    }
    ~Platform() = default;

    void readInput() override {

    }
    void setColor(Color col) {
        color = col;
    }
    void update() override {

    }
    void draw() const override {
        DrawRectangleRec(rect, color);
    }
};

#endif //RAYLIBANDROID_PLATFORM_HPP