
// Input

//Output
layout (location = 0) out vec2 textureCoordsOut;
layout (location = 1) out int renderOptions;
layout (location = 2) out int materialIdx;

// Buffers
layout (std430, binding = 0) buffer TransformSBO {
    Transform transforms[];
};

uniform vec2 screenSize;
uniform mat4 orthoProjection;


void main()
{
    Transform transform = transforms[gl_InstanceID];

    vec2 vertices[6] =
    {
        transform.pos,
        vec2(transform.pos + vec2(0.0, transform.size.y)),
        vec2(transform.pos + vec2(transform.size.x, 0.0)),
        vec2(transform.pos + vec2(transform.size.x, 0.0)),
        vec2(transform.pos + vec2(0.0, transform.size.y)),
        transform.pos + transform.size
    }; 

    int left = transform.atlasOffset.x;
    int top  = transform.atlasOffset.y;
    int right = transform.atlasOffset.x + transform.spriteSize.x;
    int bottom = transform.atlasOffset.y + transform.spriteSize.y;

    if(bool(transform.renderOptions & RENDERING_OPTION_FLIP_X)) 
    {
        int tmp = left;
        left = right;
        right = tmp;
    } 

    if(bool(transform.renderOptions & RENDERING_OPTION_FLIP_Y)) 
    {
        int tmp = top;
        top = bottom;
        bottom = tmp;
    } 
    
    vec2 textureCoords[6] =
    {
        vec2(left, top),
        vec2(left, bottom),
        vec2(right, top),
        vec2(right, top),
        vec2(left, bottom),
        vec2(right, bottom),
    };
    // Normalize Position
     {
       vec2 vertexPos = vertices[gl_VertexID];
    //    vertexPos.y = -vertexPos.y + screenSize.y;
    //    vertexPos = 2.0 * (vertexPos / screenSize) - 1.0;
       gl_Position =  orthoProjection * vec4(vertexPos, 0.0, 1.0);
     }
    textureCoordsOut = textureCoords[gl_VertexID];
    renderOptions = transform.renderOptions;
    materialIdx = transform.materialIdx;
}