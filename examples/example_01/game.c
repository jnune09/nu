#define NU_MALLOC(sz) VirtualAlloc(0, sz, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#define NU_FREE(p)    VirtualFree(p, 0, MEM_RELEASE);

#include "../../nu.h"

b32 global_running;

LRESULT CALLBACK
win32_main_window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;
    
    switch(message)
    {
        case WM_CLOSE:
        {
            global_running = false;
            
        } break;
        
        case WM_DESTROY:
        {
            global_running = false;
            
        } break;
        
        default:
        {
            result = DefWindowProcA(window, message, w_param, l_param);
            
        } break;
    }
    
    return result;
}

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int show_code)
{
    
    //NOTE Create window.
    {
        
        WNDCLASSA window_class = {0};
        HWND window = {0};
        
        window_class.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW; 
        window_class.lpfnWndProc   = win32_main_window_callback;
        window_class.hInstance     = instance;
        window_class.hCursor       = LoadCursor(NULL, IDC_ARROW);;
        window_class.lpszClassName = "nu_window_class";
        
        if (RegisterClassA(&window_class))
        {
            
            window = CreateWindowExA(0, window_class.lpszClassName,
                                     "nu_example",
                                     WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     512,
                                     512,
                                     0, 0, instance, 0);
            if (window)
            {
                HDC device_context = GetDC(window);
                
                nu_context nu;
                GLBapi gl;
                GLenum error = glbInit(&gl, NULL);
                
                if(error == GL_NO_ERROR)
                {
                    glbBindAPI(&gl);
                    
                    HGLRC opengl_rc = glbGetRC();
                    
                    SetPixelFormat(device_context, glbGetPixelFormat(), glbGetPFD());
                    
                    if(gl.wglMakeCurrent(device_context, opengl_rc))
                    {
                        gl.wglSwapIntervalEXT(1);
                        
                        nu_init(&nu);
                    }
                }
                
                //NOTE Initialization
                texture player_texture;
                const char* player_texture_fn = "player.png";
                nu_load_texture(&nu, &player_texture, player_texture_fn);
                
                u32 sprite_width  = 16;
                u32 sprite_height = 32;
                
                u32 count = 0;
                u32 frame = 0;
                
                
                //NOTE Loop
                global_running = true;
                while(global_running)
                {
                    MSG msg;
                    
                    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
                    {
                        if (msg.message == WM_QUIT)
                        {
                            global_running = false;
                        }
                        
                        TranslateMessage(&msg);
                        DispatchMessageA(&msg);
                    }
                    
                    RECT client_rect;
                    GetClientRect(window, &client_rect);
                    
                    nu.window_width  = client_rect.right  - client_rect.left;
                    nu.window_height = client_rect.bottom - client_rect.top;
                    
                    
                    //NOTE Update
                    if(count >= 60/4)
                    {
                        if(frame >= 3)
                        {
                            frame = 0;
                        }
                        else
                        {
                            frame++;
                        }
                        
                        count = 0;
                    }
                    else
                    {
                        count++;
                    }
                    
                    
                    //NOTE Draw
                    nu_clear(&nu, DARKBLUE);
                    
                    nu_draw_texture_ex(&nu, player_texture, 
                                       sprite_width*frame,
                                       sprite_width*(frame+1),
                                       sprite_height*0,
                                       sprite_height*1,
                                       (nu.window_width /2)-32,
                                       (nu.window_height/2)-64,
                                       64, 128, WHITE);
                    
                    nu_render(&nu);
                    
                    SwapBuffers(GetDC(window));
                }
                
                DestroyWindow(window);
                nu_uninit(&nu);
            }
        }
    }
    
    return 0;
}
