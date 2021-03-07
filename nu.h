/* ================================================================
by Jonathan Nunez
 


================================================================ */
#ifndef NU_H
#define NU_H

#define MAX_QUAD_COUNT  1000
#define MAX_VERT_COUNT  MAX_QUAD_COUNT*4
#define MAX_INDEX_COUNT MAX_QUAD_COUNT*6
#define MAX_TEXTURE_COUNT 16
#define NU_BYTES_PER_PIXEL 4

/*
TODO

[] #define implementation
[] improve set/view_fps
[] improve batch renderer
[] improve font loading
[] remove comments

[] logging
[] input
[] collision
*/

#include <stdint.h>

#define GLBIND_IMPLEMENTATION
#include "include/glbind.h"

#define GB_MATH_IMPLEMENTATION
#include "include/gb_math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "include/stb_truetype.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "include/stb_sprintf.h"

#define internal        static
#define local_persist   static
#define global_variable static
#define NU_DEF          static

#define true  1
#define false 0

#if _MSC_VER
#define inline __inline
#endif

#if defined(NU_MALLOC) && defined(NU_FREE)
#elif !defined(NU_MALLOC) && !defined(NU_FREE)
#else
#error "Must define both or none of NU_MALLOC and NU_FREE."
#endif

#ifndef NU_MALLOC
#define NU_MALLOC(sz) malloc(sz)
#define NU_FREE(p)    free(p)
#endif

#if defined(__cplusplus)
#define NU_CLITERAL(type) type
#else
#define NU_CLITERAL(type) (type)
#endif

#define loop    for(;;)
#define forever for(;;)

#define NU_MIN(a, b) ((a < b) ? (a) : (b))
#define NU_MAX(a, b) ((a > b) ? (a) : (b))

#define KILOBYTES(value) ((value)*1024LL)
#define MEGABYTES(value) (KILOBYTES(value)*1024LL)
#define GIGABYTES(value) (MEGABYTES(value)*1024LL)
#define TERABYTES(value) (GIGABYTES(value)*1024LL)

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

#define foreach(item, array) \
for(int keep=1, index=0, size=sizeof(array)/sizeof((array)[0]); \
keep && index != size; \
keep = !keep, index++) \
for(item = (array)+index; keep; keep = !keep)


#if DEBUG
#define nu_assert(expression) if(!(expression)) {*(int *)0 = 0;}
#else
#define nu_assert(expression)
#endif

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef i32      b32; //bool

typedef float    f32;
typedef double   f64;

typedef gbVec2   v2;
typedef gbVec3   v3;
typedef gbVec4   v4;

typedef gbMat4   m4;

typedef union v4i
{
    struct { i32 x, y, z, w; };
    struct { i32 left, top, right, bottom; };
    i32 e[4];
    
} v4i;


internal char *
nu_read_file_SLOW(const char *file_name)
{
    char *result = 0;
    
    FILE *file = fopen(file_name, "r");
    
    if(file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char *)NU_MALLOC(file_size);
        fread(result, file_size, 1, file);
        fclose(file);
    }
    
    return result;
}

//NOTE(JN): Reads the entire file and null terminates.
internal char *
nu_read_file_null_term_SLOW(const char *file_name)
{
    char *result = 0;
    
    FILE *file = fopen(file_name, "r");
    
    if(file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char *)NU_MALLOC(file_size + 1);
        fread(result, file_size, 1, file);
        result[file_size] = 0;
        
        fclose(file);
    }
    
    return result;
}

void
concat_strings(size_t a_count,   char *a,
               size_t b_count,   char *b,
               size_t out_count, char *out)
{
    for(u32 index = 0; index < a_count; ++index) { *out++ = *a++; }
    for(u32 index = 0; index < b_count; ++index) { *out++ = *b++; }
    *out++ = 0;
}

u32
string_count(const char *string)
{
    u32 count = 0;
    while(*string++) { ++count; }
    return(count);
}


inline u32
safe_truncate_u64(u64 value)
{
    assert(value <= 0xFFFFFFFF);
    u32 result = (u32)value;
    return result;
}

#define WHITE      NU_CLITERAL(color){ 255, 255, 255, 255 }
#define BLACK      NU_CLITERAL(color){ 0, 0, 0, 255 }
#define BLANK      NU_CLITERAL(color){ 0, 0, 0, 0 }
#define MAGENTA    NU_CLITERAL(color){ 255, 0, 255, 255 }
#define LIGHTGREY  NU_CLITERAL(color){ 200, 200, 200, 255 }
#define GREY       NU_CLITERAL(color){ 130, 130, 130, 255 }
#define DARKGREY   NU_CLITERAL(color){ 80, 80, 80, 255 }
#define YELLOW     NU_CLITERAL(color){ 253, 249, 0, 255 }
#define GOLD       NU_CLITERAL(color){ 255, 203, 0, 255 }
#define ORANGE     NU_CLITERAL(color){ 255, 161, 0, 255 }
#define PINK       NU_CLITERAL(color){ 255, 109, 194, 255 }
#define RED        NU_CLITERAL(color){ 230, 41, 55, 255 }
#define MAROON     NU_CLITERAL(color){ 190, 33, 55, 255 }
#define GREEN      NU_CLITERAL(color){ 0, 228, 48, 255 }
#define LIME       NU_CLITERAL(color){ 0, 158, 47, 255 }
#define DARKGREEN  NU_CLITERAL(color){ 0, 117, 44, 255 }
#define SKYBLUE    NU_CLITERAL(color){ 102, 191, 255, 255 }
#define BLUE       NU_CLITERAL(color){ 0, 121, 241, 255 }
#define DARKBLUE   NU_CLITERAL(color){ 0, 82, 172, 255 }
#define PURPLE     NU_CLITERAL(color){ 200, 122, 255, 255 }
#define VIOLET     NU_CLITERAL(color){ 135, 60, 190, 255 }
#define DARKPURPLE NU_CLITERAL(color){ 112, 31, 126, 255 }
#define BEIGE      NU_CLITERAL(color){ 211, 176, 131, 255 }
#define BROWN      NU_CLITERAL(color){ 127, 106, 79, 255 }
#define DARKBROWN  NU_CLITERAL(color){ 76, 63, 47, 255 }

#define PLATFORM_FREE_FILE(name) void name(void *memory)
typedef PLATFORM_FREE_FILE(platform_free_file);

typedef struct platform_read_file_result
{
    u32 content_size;
    void *content;
    
} platform_read_file_result;
#define PLATFORM_READ_FILE(name) platform_read_file_result name(const char *fn)
typedef PLATFORM_READ_FILE(platform_read_file);

#define PLATFORM_WRITE_FILE(name) b32 name(const char *fn, u32 memory_size, void *memory)
typedef PLATFORM_WRITE_FILE(platform_write_file);

typedef struct gl_context
{
    u32 shader;
    u32 viewport_width;
    u32 viewport_height;
    u32 vao, vbo, ebo;
    
} gl_context;

typedef struct color
{
    u8 r, g, b, a;
    
} color;

typedef struct texture
{
    u32 id;
    u32 width;
    u32 height;
    
} texture;

typedef struct character
{
    u32 x;
    u8  y;
    u8  width;
    u8  height;
    u8  c;
    
} character;

typedef struct font
{
    u32 base_height;
    u32 character_count;
    texture texture;
    character *characters;
    
} font;

typedef struct vertex
{
    v2  position;
    v4  color;
    i32 texture_id;
    v2  texture_coord;
    
} vertex;

typedef struct vertex_buffer
{
    u32 count;
    u32 size;
    void *memory;
    
} vertex_buffer;

typedef struct platform_api
{
    platform_read_file  *read_file;
    platform_write_file *write_file;
    platform_free_file  *free_file;
    
} platform_api;

typedef struct nu_camera
{
    union
    {
        v2 position;
        struct { f32 x , y; };
    };
    
    f32 zoom;
    
} nu_camera;


typedef struct nu_context
{
    gl_context gl;
    vertex_buffer vb;
    nu_camera camera;
    platform_api platform;
    
} nu_context;


//NOTE FUNCTIONS

NU_DEF void nu_init(nu_context *nu);

NU_DEF void nu_assign_read_file_pointer(nu_context *nu, platform_read_file *p);
NU_DEF void nu_assign_write_file_pointer(nu_context *nu, platform_write_file *p);
NU_DEF void nu_assign_free_file_pointer(nu_context *nu, platform_free_file *p);

NU_DEF void nu_load_texture(nu_context *nu, texture *texture, const char *file_name);

NU_DEF void nu_load_texture_from_memory(nu_context *nu, texture *texture,
                                        u32 width, u32 height, u8 *data);

NU_DEF void nu_load_font_from_file_SLOW(nu_context *nu, font *font, u32 font_size,
                                        u32 count, const char *file_name);

NU_DEF void nu_load_font_from_memory(nu_context *nu, font *font, u32 font_size,
                                     u32 count, u8 *font_data);

NU_DEF void nu_draw_rect(nu_context *nu, f32 x, f32 y, f32 width, f32 height, color color);

NU_DEF void nu_draw_texture(nu_context *nu, texture texture,
                            f32 x, f32 y, f32 width, f32 height, color color);

NU_DEF void nu_draw_texture_ex(nu_context *nu, texture texture,
                               u32 coord_left, u32 coord_right,
                               u32 coord_top,  u32 coord_bottom, 
                               f32 x, f32 y, f32 width, f32 height, color color);

NU_DEF void nu_draw_text(nu_context *nu, font *font, u32 space,
                         f32 x, f32 y, color color, const char *text);

NU_DEF void nu_render(nu_context *nu);

NU_DEF void nu_uninit(nu_context *nu);

NU_DEF const char *nu_text_format(const char *text, ...);

NU_DEF void nu_screen_shot(u32 width, u32 height);
//NU_DEF void nu_gif();


//NOTE IMPLEMENTATION

internal u32
nu__compile_shader(u32 type, const char *source)
{
    u32 id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);
    
    //TODO(JN): log
    i32 status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        u8 buffer[512];
        glGetShaderInfoLog(id, 512, NULL, buffer);
        printf(buffer);
        glDeleteShader(id);
        
        return 0;
    }
    
    return id;
}

internal void
nu__create_shader(nu_context *nu, const char *vertex_shader, const char *frag_shader)
{
    nu->gl.shader = glCreateProgram();
    
    u32 vs = nu__compile_shader(GL_VERTEX_SHADER, vertex_shader);
    u32 fs = nu__compile_shader(GL_FRAGMENT_SHADER, frag_shader);
    
    glAttachShader(nu->gl.shader, vs);
    glAttachShader(nu->gl.shader, fs);
    
    glLinkProgram(nu->gl.shader);
    
    //TODO(JN): log
    u32 success;
    u8 log[512];
    glGetProgramiv(nu->gl.shader, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(nu->gl.shader, 512, NULL, log);
        printf(log);
    }
    
    glValidateProgram(nu->gl.shader);
    glUseProgram(nu->gl.shader);
    
    glDeleteProgram(vs);
    glDeleteProgram(fs);
}

internal void
nu_load_shader_from_file(nu_context *nu, const char *vs_fn, const char *fs_fn)
{
    platform_read_file_result vs = nu->platform.read_file(vs_fn);
    platform_read_file_result fs = nu->platform.read_file(fs_fn);
    
    nu__create_shader(nu, vs.content, fs.content);
    
    nu->platform.free_file(&vs);
    nu->platform.free_file(&fs);
}

internal void
nu_clear(nu_context *nu, color color)
{
    nu->vb.count = 0;
    
    f32 r = (f32)color.r/255; 
    f32 g = (f32)color.g/255; 
    f32 b = (f32)color.b/255; 
    f32 a = (f32)color.a/255; 
    
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
}

inline internal void
nu__put_vertex(vertex *v, f32 x, f32 y, f32 red, f32 green, f32 blue, f32 alpha,
               u32 texture_id, f32 coord_x, f32 coord_y)
{
    v->position.x = x;
    v->position.y = y;
    
    v->color.r = red;
    v->color.g = green;
    v->color.b = blue;
    v->color.a = alpha;
    
    v->texture_id = texture_id;
    
    v->texture_coord.x = coord_x;
    v->texture_coord.y = coord_y;
}

internal void
nu__draw_quad(nu_context *nu, f32 x, f32 y, f32 width, f32 height, color color,
              u32 texture_id, f32 left, f32 right, f32 top, f32 bottom)
{
    
    f32 r = (f32)color.r/255;
    f32 g = (f32)color.g/255;
    f32 b = (f32)color.b/255;
    f32 a = (f32)color.a/255;
    
    vertex *v = (vertex *)(nu->vb.memory);
    
    nu__put_vertex(&v[nu->vb.count++], x,       y,        r, g, b, a, texture_id, left,  top);
    nu__put_vertex(&v[nu->vb.count++], x,       y+height, r, g, b, a, texture_id, left,  bottom);
    nu__put_vertex(&v[nu->vb.count++], x+width, y+height, r, g, b, a, texture_id, right, bottom);
    nu__put_vertex(&v[nu->vb.count++], x+width, y,        r, g, b, a, texture_id, right, top);
}

void
nu_init(nu_context *nu)
{
    const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec4 color;\n"
        "layout (location = 2) in int  texindex;\n"
        "layout (location = 3) in vec2 texcoord;\n"
        
        "out vec4 v_color;\n"
        "out vec2 v_texcoord;\n"
        "flat out int  v_texindex;\n"
        
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        
        "void main()\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(position, 0.0, 1.0);\n"
        
        "v_color    = color;\n"
        "v_texcoord = texcoord;\n"
        "v_texindex = texindex;\n"
        "}\n";
    
    const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 f_color;\n"
        
        "in vec4 v_color;\n"
        "in vec2 v_texcoord;\n"
        "flat in int v_texindex;\n"
        
        "uniform sampler2D u_textures[16];\n"
        
        "void main()\n"
        "{\n"
        "if(v_texindex != 0)\n"
        "{\n"
        "f_color = texture(u_textures[v_texindex], v_texcoord) * v_color;\n"
        "}\n"
        "else\n"
        "{\n"
        "f_color = v_color;\n"
        "}\n"
        "}\n";
    
    nu__create_shader(nu, vertex_shader_source, fragment_shader_source);
    
    u32 indices[MAX_INDEX_COUNT];
    for(u32 offset = 0, index = 0;
        index < MAX_INDEX_COUNT;
        offset += 4, index += 6)
    {
        indices[index + 0] = 1 + offset;
        indices[index + 1] = 2 + offset;
        indices[index + 2] = 3 + offset;
        
        indices[index + 3] = 3 + offset;
        indices[index + 4] = 0 + offset;
        indices[index + 5] = 1 + offset;
    }
    
    glGenVertexArrays(1, &nu->gl.vao);
    glBindVertexArray(nu->gl.vao);
    
    glGenBuffers(1, &nu->gl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, nu->gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*MAX_VERT_COUNT, 0, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &nu->gl.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nu->gl.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    
    //TODO Add an offset of macro/func
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)8);
    
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(vertex), (void *)24);
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)28);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    u32 samples[MAX_TEXTURE_COUNT];
    for(u32 i = 0; i < MAX_TEXTURE_COUNT; i++) { samples[i] = i; }
    
    glUniform1iv(glGetUniformLocation(nu->gl.shader, "u_textures"), MAX_TEXTURE_COUNT, samples);
    
    nu->vb.size = sizeof(vertex)*MAX_VERT_COUNT;
    nu->vb.memory = NU_MALLOC(nu->vb.size);
    
    //TODO(JN): Do I want to do this or check in render?
    nu->camera.zoom = 1.0f;
}

inline void
nu_assign_read_file_pointer(nu_context *nu, platform_read_file *p)
{
    nu->platform.read_file = p;
}

inline void
nu_assign_write_file_pointer(nu_context *nu, platform_write_file *p)
{
    nu->platform.write_file = p;
}

inline void
nu_assign_free_file_pointer(nu_context *nu, platform_free_file *p)
{
    nu->platform.free_file = p;
}

void
nu_load_texture_from_memory(nu_context *nu, texture *texture,
                            u32 width, u32 height, u8 *data)
{
    texture->width  = width;
    texture->height = height;
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glFinish();
    
    glBindTextureUnit(texture->id, texture->id);
}

void nu_load_texture(nu_context *nu, texture *texture, const char *file_name)
{
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    u32 nr;
    //stbi_set_flip_vertically_on_load(true);
    
    u8 *image = stbi_load(file_name,
                          &texture->width,
                          &texture->height,
                          &nr, STBI_rgb_alpha);
    
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //TODO(JN): log
    }
    
    stbi_image_free(image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glFinish();
    
    glBindTextureUnit(texture->id, texture->id);
}

void nu_load_font_from_file_SLOW(nu_context *nu, font *font, u32 font_size, u32 count, const char *file_name)
{
    u8* font_data = nu_read_file_SLOW(file_name);
    nu_load_font_from_memory(nu, font, font_size, count, font_data);
}


void nu_load_font_from_memory(nu_context *nu, font *font, u32 font_size, u32 count, u8 *font_data)
{
    font->base_height = font_size;
    font->character_count = count;
    
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, font_data, 0))
    {
        //TODO(JN): log
    }
    
    u32 b_w = 1024;
    u32 b_h = 1024;
    u32 l_h = font_size;
    
    /* create a bitmap for the font */
    u8* monobitmap = (u8 *)NU_MALLOC(b_w * b_h * sizeof(u8));
    u8* bitmap     = (u8 *)NU_MALLOC(b_w * b_h * sizeof(u8)*4);
    
    /* calculate font scaling */
    f32 scale = stbtt_ScaleForPixelHeight(&info, l_h);
    
    font->characters = (character *)NU_MALLOC(sizeof(character)*count);;
    
    i32 x = 0;
    i32 offset_y = 0;
    
    i32 ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent  = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    for (u32 i = 0; i < count; ++i)
    {
        
        //NOTE(JN): Character width.
        i32 ax;
        i32 lsb;
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);
        
        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        i32 c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, i, scale, scale,
                                    &c_x1, &c_y1, &c_x2, &c_y2);
        
        if(x > b_w-(u32)(roundf(ax * scale)))
        {
            offset_y += font_size;
            x = 0;
        }
        
        i32 y = ascent + c_y1 + offset_y;
        i32 byte_offset = x + roundf(lsb * scale) + (y * b_w);
        
        stbtt_MakeCodepointBitmap(&info, monobitmap + byte_offset,
                                  c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, i);
        
        font->characters[i].x = x;
        font->characters[i].y = y;
        font->characters[i].width = (u32)(roundf(ax * scale));
        font->characters[i].height = c_y2 - c_y1;
        font->characters[i].c = i;
        
        //NOTE(JN): Advance x
        x += roundf(ax * scale);
        
        //TODO(JN): Add kerning
        //int kern;
        //kern = stbtt_GetCodepointKernAdvance(&info, i, i + 1]);
        //x += roundf(kern * scale);
    }
    
    u8 *source = monobitmap;
    u8 *dest_row = (u8 *)bitmap;
    for(u32 y = 0; y < b_h; y++)
    {
        u32 *dest = (u32 *)dest_row;
        for(u32 x = 0; x < b_w; x++)
        {
            u8 alpha = *source++;
            *dest++ = ((alpha << 24)|
                       (alpha << 16)|
                       (alpha << 8)|
                       (alpha << 0));
        }
        dest_row += b_w*NU_BYTES_PER_PIXEL;
    }
    
    nu_load_texture_from_memory(nu, &font->texture, b_w, b_h, bitmap);
    
    NU_FREE(monobitmap);
    NU_FREE(bitmap);
}

void
nu_draw_rect(nu_context *nu, f32 x, f32 y, f32 width, f32 height, color color)
{
    nu__draw_quad(nu, x, y, width, height, color,
                  0, 0.0f, 1.0f, 0.0f, 1.0f);
}

void
nu_draw_texture(nu_context *nu, texture texture,
                f32 x, f32 y, f32 width, f32 height, color color)
{
    nu__draw_quad(nu, x, y, width, height, color,
                  texture.id, 0.0f, 1.0f, 0, 1.0f);
}

void
nu_draw_texture_ex(nu_context *nu, texture texture, 
                   u32 coord_left, u32 coord_right, 
                   u32 coord_top,  u32 coord_bottom,  
                   f32 x, f32 y, f32 width, f32 height, color color)
{
    f32 left   = (f32)coord_left   /texture.width;
    f32 right  = (f32)coord_right  /texture.width;
    f32 top    = (f32)coord_top    /texture.height;
    f32 bottom = (f32)coord_bottom /texture.height;
    
    nu__draw_quad(nu, x, y, width, height, color,
                  texture.id, left, right, top, bottom);
}

void
nu_draw_text(nu_context *nu, font *font, u32 space, f32 x, f32 y, color color, const char *text)
{
    character c;
    f32 current_x = x;
    
    u32 text_count = string_count(text);
    for(u32 i = 0; i < text_count; i++)
    {
        for(u32 j = 0; j < font->character_count; j++)
        {
            if(font->characters[j].c == text[i])
            {
                c = font->characters[j];
            }
        }
        
        if(c.c)
        {
            nu_draw_texture_ex(nu, font->texture,
                               (c.x), (c.x+c.width), (c.y), (c.y+c.height),
                               current_x, y+(c.y%font->base_height), c.width, c.height, color);
            
            current_x += c.width+space;
        }
    }
    
}

void
nu_render(nu_context *nu)
{
    glViewport(0, 0, nu->gl.viewport_width, nu->gl.viewport_height);
    
    glBindBuffer(GL_ARRAY_BUFFER, nu->gl.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nu->vb.size, (vertex *)nu->vb.memory);
    
    glUseProgram(nu->gl.shader);
    
    //NOTE(JN): Model
    m4 model = {0};
    
    gb_mat4_identity(&model);
    glUniformMatrix4fv(glGetUniformLocation(nu->gl.shader, "model"),
                       1, GL_FALSE, model.e);
    
    //NOTE(JN): View
    m4 view = {0};
    
    m4 position = {0};
    v3 translation = {nu->camera.x, nu->camera.y, 0.0f};
    gb_mat4_translate(&position, translation);
    
    m4 zoom = {0};
    v3 scale = {nu->camera.zoom, nu->camera.zoom, nu->camera.zoom};
    gb_mat4_scale(&zoom, scale);
    
    gb_mat4_mul(&view, &position, &zoom);
    
    glUniformMatrix4fv(glGetUniformLocation(nu->gl.shader, "view"),
                       1, GL_FALSE, view.e);
    
    //NOTE(JN): Projection
    m4 projection = {0};
    
    gb_mat4_ortho2d(&projection, 0,
                    (f32)nu->gl.viewport_width,
                    (f32)nu->gl.viewport_height, 0);
    
    glUniformMatrix4fv(glGetUniformLocation(nu->gl.shader, "projection"),
                       1, GL_FALSE, projection.e);
    
    glDrawElements(GL_TRIANGLES, (nu->vb.count*3/2), GL_UNSIGNED_INT, 0);
}

void
nu_uninit(nu_context *nu)
{
    glDeleteVertexArrays(1, &nu->gl.vao);
    glDeleteBuffers(1, &nu->gl.vbo);
    glDeleteBuffers(1, &nu->gl.ebo);
    glDeleteProgram(nu->gl.shader);
    glbUninit();
    NU_FREE(nu->vb.memory);
}

#define MAX_TEXT_BUFFER_LENGTH 1024
#define MAX_TEXTFORMAT_BUFFERS 4
const char *
nu_text_format(const char *text, ...)
{
    local_persist char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = { 0 };
    local_persist u32  index = 0;
    
    char *current_buffer = buffers[index];
    memset(current_buffer, 0, MAX_TEXT_BUFFER_LENGTH);
    
    va_list args;
    va_start(args, text);
    vsprintf(current_buffer, text, args);
    va_end(args);
    
    index += 1;
    if (index >= MAX_TEXTFORMAT_BUFFERS)
    {
        index = 0;
    }
    
    return current_buffer;
}

void nu_screen_shot(u32 width, u32 height)
{
    void *data = NU_MALLOC(width*height*NU_BYTES_PER_PIXEL);
    
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void *)data);
    
    stbi_flip_vertically_on_write(true);
    
    stbi_write_png("screenshot.png", width, height, STBI_rgb_alpha,
                   (void *)data, width*NU_BYTES_PER_PIXEL);
    
    NU_FREE(data);
};

#endif //NU_H
