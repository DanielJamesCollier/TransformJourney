#include <iostream>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

std::ostream & operator<< (std::ostream & stream, const glm::vec2& v) {
  return stream << "vec2(" << v.x << ", " << v.y << ")\n";
}

std::ostream & operator<< (std::ostream & stream, const glm::vec4& v) {
  return stream << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
}

int main() {
    float width = 1024;
    float height = 576;
    float aspectRatio = width / height;

    /* LOCAL SPACE */
    //------------------------------
    /* INFO ROUND
      - (x, y, z) should be between -1.0 and 1.0
      - (w) needs to be 1.0 or else no transform will happen when multiplying against the perspective matrix
    */

    /* BONUS ROUND
      - (z) if there is no clipping (there probably is) then z cannot be 0 or else when dividing 
            point_clipSpace.(x,y,z) by point_clipSpace.w then fun will arise.
    */

    glm::vec4 point_localSpace(-1.0f, -1.0f, -3.0f, 1);
    std::cout << "\nstart: " << point_localSpace << std::endl;
    //------------------------------
    
    // LOCAL -> CLIP SPACE
    //------------------------------
    /* INFO ROUND
     - when doing ProjectionMatrix * Vec4 the said Vec4 will be put into clip space
     - projection matrix is an odd name as all other matricies are named as to what space they project into 
       except this lil dude
    */

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), aspectRatio, 0.1f, 1000.0f);
    glm::vec4 point_clipSpace = projectionMatrix * point_localSpace;
    std::cout << "proj point: " << point_clipSpace << std::endl;
    //------------------------------

    // CLIP POLYGONS
    //------------------------------
    {
    /* INFO ROUND
        - clipping makes sure that all vertices are inside the screen before drawing
          if there was no clipping then the program would crash because it would be accessing out of bounds 
          memory
        - clipping clips against the w of a given vertex with the previous vetex. this is if only the current or
          previous vertices are outside the clip space
        - clipping is done for free on the GPU hardware, although it can be emulated on the CPU
    */

    // rudementory algo
    /* 
      
        if ((x >= -w && x <= w) &&
            (y >= -w && y <= w) &&
            (z >= -w && z <= w) 
            {
              // point does not need clipping
            }
            else
            {
              // clip against w from previous Point
            }
      
      */
    }
    //------------------------------


    // CLIP SPACE -> NORMALISED DEVICE SPACE (NDC)
    //------------------------------
    /* INFO ROUND
      - ndc is in the range of -1.0 to 1.0 in the x,y and z axis.
      - w now equals the origional z value but sign flipped
    */

    glm::vec4 point_ndcSpace(point_clipSpace.x / point_clipSpace.w,
                             point_clipSpace.y / point_clipSpace.w,
                             point_clipSpace.z / point_clipSpace.w,
                             point_clipSpace.w);

    std::cout << "ndc space: " << point_ndcSpace << std::endl; 
    //------------------------------


    // NORMALISED DEVICE SPACE -> SCREEN SPACE
    //------------------------------
    /* INFO ROUND
      - screen space converts the previouse -1 to 1 mapping into 0 to screen width/ height depending on the axis
    */

    glm::vec4 point_screenSpace((point_ndcSpace.x + 1) * (width / 2),
                                (point_ndcSpace.y + 1) * (height / 2),
                                 point_ndcSpace.z,
                                 point_ndcSpace.w);

    std::cout << "screen space: " << point_screenSpace << std::endl;                                    
    //------------------------------                                                            


    return 0;
}