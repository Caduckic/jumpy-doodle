//
// Created by meisj on 19/08/2024.
//

#ifndef RAYLIBANDROID_CONFIG_HPP
#define RAYLIBANDROID_CONFIG_HPP

const int PLAY_SIZE_WIDTH = 1000;
const int PLAYER_SIZE = 100;
const int PLAYER_HITBOX_HEIGHT = 10;

const int START_HEIGHT = 200;
const int START_GROUND = 100;

const float GRAVITY_STRENGTH = 4000.f;
const float MAX_GRAVITY_STRENGTH = 2000.f; // max is higher than strength, this is because gravity strength is a scaler not an absolute value
const float JUMP_STRENGTH = -3000.f;

const float PLATFORM_WIDTH = 100.f;
const float PLATFORM_HEIGHT = 10.f;

const float MAX_PLATFORM_DISTANCE = 700.f;
const float MIN_PLATFORM_DISTANCE = 300.f;

#endif //RAYLIBANDROID_CONFIG_HPP
