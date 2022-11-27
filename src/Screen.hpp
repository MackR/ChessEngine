//
//  Screen.hpp
//  Chess
//
//  Created by Mack Ragland on 10/19/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#pragma once


namespace SDLScreen {
    
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
    
    class Screen {
    public:
        static const int SCREEN_WIDTH = 800;
        static const int SCREEN_HEIGHT = 800;
        static const int INIT_WINDOWX = 500;
        static const int INIT_WINDOWY = 500;
        

        
    private:
        
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_board_buffer;
        Uint32 *m_piece_buffer;
        
        
        
    public:
        Screen();
        bool init();
        bool processEvents();
        void update();
        void close();
        SDL_Renderer* getRenderer();
        
        
    };
    
}



