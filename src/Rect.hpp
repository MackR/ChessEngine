//
//  Rect.hpp
//  Chess
//
//  Created by Mack Ragland on 11/7/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include "Screen.hpp"

class Rectbuild {
    int m_x, m_y, m_w, m_h;
    int m_r,m_g,m_b,m_a;
    SDL_Window *m_window;
    const std::string *path;
    SDL_Texture *texture = nullptr;
    SDL_Renderer *m_renderer;

public:
    Rectbuild();
    Rectbuild(int x, int y, int w, int h, const std::string &imagePath, SDL_Renderer *renderer);
    Rectbuild(int x, int y, int w, int h, int r, int g, int b, int a, SDL_Renderer *renderer);
    void setColor(int r, int g, int b, int a);
    void setPiecePosition(int rank, int file);
    void draw();
};

