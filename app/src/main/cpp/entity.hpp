//
// Created by meisj on 20/08/2024.
//

#ifndef RAYLIBANDROID_ENTITY_HPP
#define RAYLIBANDROID_ENTITY_HPP

#include <raylib.h>

class Entity {
protected:
    Rectangle rect;
    Rectangle hitBox;
    Rectangle dynamicHitBox;
public:
    Entity() {}
    Entity(float posX, float posY, float width, float height) {
        rect = {
            posX,
            posY,
            width,
            height
        };
        hitBox = rect;
        dynamicHitBox = hitBox;
    }
    Entity(float posX, float posY, float width, float height, float hitX, float hitY, float hitWidth, float hitHeight) :
        Entity(posX, posY, width, height) {
            hitBox = {
                    hitX,
                    hitY,
                    hitWidth,
                    hitHeight
            };
            dynamicHitBox = hitBox;
    }
    ~Entity() = default;

    virtual void readInput() = 0;
    virtual void update() = 0;
    virtual void draw() const = 0;

    void updateDynamicHitBox(const Rectangle& lastHitBox) {
        float dX, dY {0.f};
        if (lastHitBox.x < hitBox.x)
            dX = lastHitBox.x;
        else
            dX = hitBox.x;
        if (lastHitBox.y < hitBox.y)
            dY = lastHitBox.y;
        else
            dY = hitBox.y;
        float dWidth = lastHitBox.width + std::fabs(hitBox.x - lastHitBox.x);
        float dHeight = lastHitBox.height + std::fabs(hitBox.y - lastHitBox.y);
        dynamicHitBox = {
                dX,
                dY,
                dWidth,
                dHeight
        };
    }

    bool checkRectCollision(const Rectangle& other) const {
        return (rect.x + rect.width > other.x && rect.x < other.x + other.width &&
            rect.y + rect.height > other.y && rect.y < other.y + other.height);
    }

    /* Why I'm doing this, constructing a dynamic hit box like this gives frame independence.
     * Say you have fps of 10 and when you update() the hit box is already pasted the platform,
     * you'd just fall straight through. Instead make a hit box out of the previous frame and
     * the current frames hit box, now any platform covered from the last frame to the current frame
     * is considered. Yay for being able to play this on a potato! Yeah this is overkill for this game lol*/
    bool checkDynamicHitBoxRectCollision(const Rectangle& other) const {
        if (dynamicHitBox.x + dynamicHitBox.width > other.x && dynamicHitBox.x < other.x + other.width &&
                dynamicHitBox.y + dynamicHitBox.height > other.y && dynamicHitBox.y < other.y + other.height)
                    return true;
        if (dynamicHitBox.x < 0.f) { // lol kinda useless as the player can't actually be a negative x position, oh well haha
            Rectangle rightOfScreenHitBox = dynamicHitBox;
            rightOfScreenHitBox.x += PLAY_SIZE_WIDTH;
            if (rightOfScreenHitBox.x + rightOfScreenHitBox.width > other.x && rightOfScreenHitBox.x < other.x + other.width &&
                    rightOfScreenHitBox.y + rightOfScreenHitBox.height > other.y && rightOfScreenHitBox.y < other.y + other.height)
                return true;
        }
        if (dynamicHitBox.x + dynamicHitBox.width > PLAY_SIZE_WIDTH) {
            Rectangle leftOfScreenHitBox = dynamicHitBox;
            leftOfScreenHitBox.x -= PLAY_SIZE_WIDTH;
            if (leftOfScreenHitBox.x + leftOfScreenHitBox.width > other.x && leftOfScreenHitBox.x < other.x + other.width &&
                    leftOfScreenHitBox.y + leftOfScreenHitBox.height > other.y && leftOfScreenHitBox.y < other.y + other.height)
                return true;
        }
        return false;
    }

    void resetHitBoxes() {
        hitBox.x = rect.x;
        hitBox.y = rect.y + rect.height - PLAYER_HITBOX_HEIGHT;
        dynamicHitBox = hitBox;
    }

    Rectangle getRect() {
        return rect;
    }

    void setPosition(const Vector2 pos) {
        rect.x = pos.x;
        rect.y = pos.y;
    }
};

#endif //RAYLIBANDROID_ENTITY_HPP
