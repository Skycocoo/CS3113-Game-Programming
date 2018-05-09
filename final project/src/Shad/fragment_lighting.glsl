uniform sampler2D diffuse;
uniform float brightness;
uniform vec2 lightPositions[2];

varying vec2 varPosition;
varying vec2 texCoordVar;

float attenuate(float dist, float a, float b){
    return 1.0 / (1.0 + a*dist + b*dist*dist);
}

void main() {
    float light = 0.0;

    for(int i = 0; i < 2; i++){
        light += attenuate(distance(lightPositions[i], varPosition), 0.5, 0.8);
    }

    if(light > 1.0)
        light = 1.0;

    vec4 textureColor = texture2D(diffuse, texCoordVar);
    gl_FragColor = textureColor * light * brightness;
    gl_FragColor.a = textureColor.a;
}
