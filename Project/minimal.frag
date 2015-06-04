#version 420

in vec3 color;

void main(void)
{
    const float d = 0.57735026918962576;
    gl_FragColor = vec4((color+vec3(d))/vec3(2*d), 1.0);
}
