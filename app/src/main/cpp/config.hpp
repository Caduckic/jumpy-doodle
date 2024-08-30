//
// Created by meisj on 19/08/2024.
//

#ifndef RAYLIBANDROID_CONFIG_HPP
#define RAYLIBANDROID_CONFIG_HPP

const int PLAY_SIZE_WIDTH = 2000;
const int PLAYER_SIZE = 200;
const int PLAYER_HITBOX_HEIGHT = 10;

const int START_HEIGHT = 400;
const int START_GROUND = 200;

const float GRAVITY_STRENGTH = 8000.f;
const float MAX_GRAVITY_STRENGTH = 4000.f; // max is higher than strength, this is because gravity strength is a scaler not an absolute value
const float JUMP_STRENGTH = -6000.f;
const float SUPER_JUMP_STRENGTH = -12000.f;

const float PLATFORM_WIDTH = 200.f;
const float PLATFORM_HEIGHT = 20.f;

const float MAX_PLATFORM_DISTANCE = 1400.f;
const float MIN_PLATFORM_DISTANCE = 600.f;

#endif //RAYLIBANDROID_CONFIG_HPP
