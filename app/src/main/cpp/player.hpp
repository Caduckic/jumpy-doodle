//
// Created by meisj on 19/08/2024.
//

#ifndef RAYLIBANDROID_PLAYER_HPP
#define RAYLIBANDROID_PLAYER_HPP

#include <raylib.h>
#include <cmath>
#include "config.hpp"
#include "entity.hpp"

class Player : public Entity {
private:
    Color color;

    Vector2 startTouchPos {0.f,0.f};
    float directionStrength {0.f};
    float speed {2000.f};
    float gravityVel {0.f};

public:
    Player() {}
    Player(float startX, float startY, float width, float height, Color col) :
        Entity(startX, startY, width, height, startX, startY, width, PLAYER_HITBOX_HEIGHT) {
            color = col;
    }
    ~Player() = default;

    void readInput() override {
        Vector2 touchPos = GetTouchPosition(0);
        int currentGesture = GetGestureDetected();
        switch (currentGesture) {
            case GESTURE_TAP:
                startTouchPos = GetTouchPosition(0);
                break;
            case GESTURE_NONE:
                directionStrength = 0.f;
                break;
            default:
                directionStrength = (touchPos.x - startTouchPos.x) / ((float)GetScreenWidth() / 10.f);
                if (directionStrength >= 1.f) directionStrength = 1.f;
                else if (directionStrength <= -1.f) directionStrength = -1.f;
                //debugString << directionStrength;

                //__android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "%s", debugString.str().c_str());
                break;
        }
    }

    void update() override {
        Rectangle lastHitBox = hitBox;
        gravityVel = std::fmin(gravityVel + (GRAVITY_STRENGTH * GetFrameTime()), MAX_GRAVITY_STRENGTH);

        rect.y += gravityVel * GetFrameTime();
        rect.x += directionStrength * speed * GetFrameTime();
        if (rect.x < 0.f) rect.x += PLAY_SIZE_WIDTH;
        else if (rect.x > PLAY_SIZE_WIDTH) rect.x -= PLAY_SIZE_WIDTH;

        hitBox.x = rect.x;
        hitBox.y = rect.y + rect.height - PLAYER_HITBOX_HEIGHT;
        updateDynamicHitBox(lastHitBox);
    }

    void draw() const override {
        DrawRectangleRec(rect, color);
        DrawRectangle(rect.x + PLAY_SIZE_WIDTH, rect.y, rect.width, rect.height, BLUE);
        DrawRectangle(rect.x - PLAY_SIZE_WIDTH, rect.y, rect.width, rect.height, BLUE);
    }

    void setGravityVel(float vel) {
        gravityVel = vel;
    }

    float getGravityVel() const {
        return gravityVel;
    }
};


#endif //RAYLIBANDROID_PLAYER_HPP
