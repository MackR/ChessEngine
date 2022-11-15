//
//  Rect.cpp
//  Chess
//
//  Created by Mack Ragland on 11/7/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Rect.hpp"

Rectbuild::Rectbuild(int x, int y, int w, int h, const std::string &imagePath, SDL_Renderer *renderer): m_x(x), m_y(y), m_w(w), m_h(h){

    auto surface  = IMG_Load(imagePath.c_str());
    //auto imgPath = imagePath.c_str();
    //SDL_Delay(20);
    if (!surface) {
        std::cerr << "Failed to create surface" << std::endl;
    }
    m_renderer = renderer;
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    

    if (!texture) {
        std::cerr << "Failed to create texture" << std::endl;
    }
    
    SDL_FreeSurface(surface);

}

Rectbuild::Rectbuild(int x, int y, int w, int h, int r, int g, int b, int a, SDL_Renderer *renderer): m_x(x), m_y(y), m_w(w), m_h(h), m_r(r), m_g(g), m_b(b), m_a(a){
    m_renderer = renderer;
}

void Rectbuild::draw(){
    SDL_Rect rect = {m_x,m_y,m_w,m_h};
    if (texture) {
        SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
    }
    else{
        SDL_SetRenderDrawColor(m_renderer, m_r, m_g, m_b, m_a);
        SDL_RenderFillRect(m_renderer, &rect);
    }
}

Rectbuild::Rectbuild(){
    m_x = 0;
    m_y = 0;
    m_w = 0;
    m_h = 0;
    path = NULL;
    m_renderer = NULL;
    
}
void Rectbuild::setColor(int r, int g, int b, int a){
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
}