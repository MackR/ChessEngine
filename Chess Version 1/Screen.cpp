//
//  Screen.cpp
//  Chess
//
//  Created by Mack Ragland on 10/19/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Screen.hpp"

namespace SDLScreen {
    
    Screen::Screen():
    m_window(NULL),m_renderer(NULL),m_texture(NULL),m_board_buffer(NULL)
    {
    }
    
    bool Screen::init(){
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }
        m_window = SDL_CreateWindow("Chess Board", INIT_WINDOWX, INIT_WINDOWY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (m_window == NULL) {
            SDL_Quit();
            return false;
        }
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (m_renderer == NULL) {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
       m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (m_texture == NULL) {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        };
        
        m_board_buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
        m_piece_buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
        
        int flags= IMG_INIT_PNG;
        int initted=IMG_Init(flags);
        if((initted&flags) != flags) {
            printf("IMG_Init: Failed to init required jpg and png support!\n");
            printf("IMG_Init: %s\n", IMG_GetError());
            // handle error
            return false;
        }
        
        
        return true;
        
    }
    
    void Screen::update(){
        //SDL_UpdateTexture(m_texture, NULL, m_board_buffer, SCREEN_WIDTH*sizeof(Uint32));
        //SDL_UpdateTexture(m_texture, NULL, m_piece_buffer, SCREEN_WIDTH*sizeof(Uint32));
        //SDL_RenderClear(m_renderer);
        SDL_RenderPresent(m_renderer);
    }
    
    SDL_Renderer* Screen::getRenderer(){
        return m_renderer;
    }

    
    bool Screen::processEvents(){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }
        return true;
    }
    
    void Screen::close(){
        delete [] m_piece_buffer;
        delete [] m_board_buffer;
        IMG_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
    

}
