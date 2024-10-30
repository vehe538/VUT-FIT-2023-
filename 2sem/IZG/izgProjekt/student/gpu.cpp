/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

void bind_framebuffer(GPUMemory&mem, BindFramebufferCommand cmd) {

  mem.activatedFramebuffer = cmd.id;

}
void bind_program(GPUMemory&mem, BindProgramCommand cmd) {

  mem.activatedProgram = cmd.id;

}
void bind_vertexarray(GPUMemory&mem, BindVertexArrayCommand cmd) {

  mem.activatedVertexArray = cmd.id;

}



void clear(GPUMemory&mem, ClearCommand cmd) {
  
  
  Framebuffer*fbo = mem.framebuffers+mem.activatedFramebuffer;

  if (cmd.clearColor) {
    if (fbo->color.data) {
      
      for (uint32_t h = 0; h < fbo->height; h++) {
        for (uint32_t w = 0; w < fbo->width; w++){

          uint8_t* pixelStart = ((uint8_t*)fbo->color.data) + h*fbo->color.pitch + w*fbo->color.bytesPerPixel;

          if(fbo->color.format == Image::FLOAT32){
            float*pixelf = (float*)pixelStart;
            pixelf[0] = cmd.color[0];
            pixelf[1] = cmd.color[1];
            pixelf[2] = cmd.color[2];
            pixelf[3] = cmd.color[3];

          }

          if(fbo->color.format == Image::UINT8) {
            uint8_t *pixelu = (uint8_t*)pixelStart;
            pixelu[0] = cmd.color[0]*255;
            pixelu[1] = cmd.color[1]*255;
            pixelu[2] = cmd.color[2]*255;
            pixelu[3] = cmd.color[3]*255;
          }
        }
      }
    }
  }

  if (cmd.clearDepth) {
    if (fbo->depth.data) {

      for (uint32_t w = 0; w < fbo->width; w++) {
        for (uint32_t h = 0; h < fbo->height; h++) {

          uint8_t* pixelStart = ((uint8_t*)fbo->depth.data) + h*fbo->depth.pitch + w*fbo->depth.bytesPerPixel;

          if (fbo->depth.format == Image::FLOAT32) {
            float *pixelf = (float*)pixelStart;
            pixelf[0] = cmd.depth;
            
          }

          if (fbo->depth.format == Image::UINT8) {
            uint8_t *pixelu = (uint8_t*)pixelStart;
            pixelu[0] = cmd.depth;
          }

        }
      }
    }
  }
}

void draw (GPUMemory&mem, DrawCommand cmd) {

  Program prg = mem.programs[mem.activatedProgram];

  for (uint32_t n = 0; n < cmd.nofVertices; ++n) {
    InVertex inVertex;
    OutVertex outVertex;
    ShaderInterface si;
    si.gl_DrawID = mem.gl_DrawID;
    prg.vertexShader(outVertex, inVertex, si); 
  }

}


//! [izg_enqueue]
void izg_enqueue(GPUMemory&mem,CommandBuffer const&cb){
  mem.gl_DrawID = 0;
  for(uint32_t i=0;i<cb.nofCommands;++i){
    CommandType type = cb.commands[i].type;
    CommandData data = cb.commands[i].data;

    if (type == CommandType::CLEAR) {
      clear(mem,data.clearCommand);
    }
    if (type == CommandType::BIND_FRAMEBUFFER) {
      bind_framebuffer(mem,data.bindFramebufferCommand);
    }
    if (type == CommandType::BIND_PROGRAM) {
      bind_program(mem,data.bindProgramCommand);
    }
    if (type == CommandType::BIND_VERTEXARRAY) {
      bind_vertexarray(mem,data.bindVertexArrayCommand);
    }
    if (type == CommandType::DRAW) {
      draw(mem,data.drawCommand);
      mem.gl_DrawID++;
    }
    if (type == CommandType::SET_DRAW_ID) {
      mem.gl_DrawID = cb.commands[i].data.setDrawIdCommand.id;
    }
    if (type == CommandType::SUB_COMMAND) {
      izg_enqueue(mem, *data.subCommand.commandBuffer);
    }
  }
}
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
//! [izg_enqueue]


/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.img.data)return glm::vec4(0.f);
  auto&img = texture.img;
  auto uv1 = glm::fract(glm::fract(uv)+1.f);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  return texelFetch(texture,pix);
}

/**
 * @brief This function reads color from texture with clamping on the borders.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_textureClamp(Texture const&texture,glm::vec2 uv){
  if(!texture.img.data)return glm::vec4(0.f);
  auto&img = texture.img;
  auto uv1 = glm::clamp(uv,0.f,1.f);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  return texelFetch(texture,pix);
}

/**
 * @brief This function fetches color from texture.
 *
 * @param texture texture
 * @param pix integer coorinates
 *
 * @return color 4 floats
 */
glm::vec4 texelFetch(Texture const&texture,glm::uvec2 pix){
  auto&img = texture.img;
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  if(pix.x>=texture.width || pix.y >=texture.height)return color;
  if(img.format == Image::UINT8){
    auto colorPtr = (uint8_t*)getPixel(img,pix.x,pix.y);
    for(uint32_t c=0;c<img.channels;++c)
      color[c] = colorPtr[img.channelTypes[c]]/255.f;
  }
  if(texture.img.format == Image::FLOAT32){
    auto colorPtr = (float*)getPixel(img,pix.x,pix.y);
    for(uint32_t c=0;c<img.channels;++c)
      color[c] = colorPtr[img.channelTypes[c]];
  }
  return color;
}

