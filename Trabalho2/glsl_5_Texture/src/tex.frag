//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
uniform vec2 resolution; // Adiciona a resolução da tela
varying vec4 v_texCoord;

void main()
{
    vec2 screenCoord = gl_FragCoord.xy / resolution; // Calcula as coordenadas de tela normalizadas
    float largura = 0.1; // Largura de cada listra
    float x = screenCoord.x;

    if (mod(x, largura * 2.0) < largura) {
        gl_FragColor = texture2D(texture_0, v_texCoord.xy); // Textura 0
    } else {
        gl_FragColor = texture2D(texture_1, v_texCoord.xy); // Textura 1
    }
}

/*
ALVO

//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
varying vec4 v_texCoord;


void main()
{
    vec2 coord = v_texCoord.xy - vec2(1, 1); // Centraliza as coordenadas
    float dist = length(coord); // Calcula a distância do centro
    float raio = 0.1;

    if (mod(dist, raio * 2.0) < raio) {
        gl_FragColor = texture2D(texture_0, v_texCoord.xy); // Textura 0
    } else {
        gl_FragColor = texture2D(texture_1, v_texCoord.xy); // Textura 1
    }
}

*/

/*
LISTRAS COORDENADA DE VÉRTICE

//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
varying vec4 v_texCoord;

void main()
{
    float largura = 0.1; // Largura de cada listra
    float x = v_texCoord.x;

    if (mod(x, largura * 2.0) < largura) {
        gl_FragColor = texture2D(texture_0, v_texCoord.xy); // Textura 0
    } else {
        gl_FragColor = texture2D(texture_1, v_texCoord.xy); // Textura 1
    }
}
*/

/*
LISTRAS COORDENADA DE TELA

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
uniform vec2 resolution; // Adiciona a resolução da tela
varying vec4 v_texCoord;

void main()
{
    vec2 screenCoord = gl_FragCoord.xy / resolution; // Calcula as coordenadas de tela normalizadas
    float largura = 0.1; // Largura de cada listra
    float x = screenCoord.x;

    if (mod(x, largura * 2.0) < largura) {
        gl_FragColor = texture2D(texture_0, v_texCoord.xy); // Textura 0
    } else {
        gl_FragColor = texture2D(texture_1, v_texCoord.xy); // Textura 1
    }
}
*/

/*
MORPHING

//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
uniform float morphFactor;
varying vec4 v_texCoord;

void main()
{
   float t = mod(morphFactor, 2.0); // Calcula o fator de interpolação

   vec4 color0 = texture2D(texture_0, v_texCoord.xy);
   vec4 color1 = texture2D(texture_1, v_texCoord.xy);

   vec4 finalColor;
   if(t < 1.0) {
      finalColor = mix(color0, color1, t); // Interpola entre as texturas
   }
   else{
      finalColor = mix(color1, color0, t - 1.0); // Interpola de volta
   }

   gl_FragColor = finalColor; // Aplica o brilho
}

*/