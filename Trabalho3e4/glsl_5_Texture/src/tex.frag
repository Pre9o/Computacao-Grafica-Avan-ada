uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform float angle;
uniform float mouseX;
uniform float mouseY;
uniform float quantidadeBlur;
uniform int blurAtivo;
uniform int lupaBlur;
varying vec4 v_texCoord;

void main()
{
    if(lupaBlur == 0){
        vec2 uv = v_texCoord.xy;
        vec2 center = vec2(0.5, 0.5); 
        vec2 pos = uv - center; 

        float cosA = cos(angle);
        float sinA = sin(angle);
        vec2 rotatedPos = vec2(
            pos.x * cosA - pos.y * sinA,
            pos.x * sinA + pos.y * cosA
        );

        vec4 color;
        if(rotatedPos.x > 0.0){
            color = texture2D(texture_0, uv);
        }
        else{
            color = texture2D(texture_1, uv);
        }

        gl_FragColor = color;
    }
    else{
        vec2 textura_offset = 1.0 / vec2(textureSize(texture_0, 0));
        vec4 result = texture2D(texture_0, v_texCoord.xy);

        float tamanho_janela = 81.0;
        float raio = 0.3;

        vec2 mousePos = vec2(mouseX / 600.0, mouseY / 600.0);

        float distancia = distance(v_texCoord.xy, mousePos);

        if(blurAtivo == 1 && distancia > raio){
            vec4 sum = vec4(0.0);
            for(int x = -4; x <= 4; x++){
                for(int y = -4; y <= 4; y++){
                    sum += texture2D(texture_0, v_texCoord.xy + vec2(float(x), float(y)) * textura_offset * quantidadeBlur);
                }
            }
            result = sum / tamanho_janela;
        }

        gl_FragColor = result;
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
    vec2 coord = v_texCoord.xy - vec2(1, 1);
    float dist = length(coord);
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
    float largura = 0.1;
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
uniform vec2 resolution;
varying vec4 v_texCoord;

void main()
{
    vec2 screenCoord = gl_FragCoord.xy / resolution;
    float largura = 0.1;
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
   float t = mod(morphFactor, 2.0);

   vec4 color0 = texture2D(texture_0, v_texCoord.xy);
   vec4 color1 = texture2D(texture_1, v_texCoord.xy);

   vec4 finalColor;
   if(t < 1.0) {
      finalColor = mix(color0, color1, t);
   }
   else{
      finalColor = mix(color1, color0, t - 1.0);
   }

   gl_FragColor = finalColor;
}

*/

/*
GRAYSCALE

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
varying vec4 v_texCoord;

void main()
{
   vec4 color = texture2D(texture_0, v_texCoord.xy);

   float gray = dot(color.rgb, vec3(0.3, 0.5, 0.1));


   vec4 grayColor = vec4(vec3(gray), color.a);

   gl_FragColor = grayColor;
}

*/

/*
BLUR 

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform float brilho;
uniform float mouseX;
uniform float mouseY;
uniform float quantidadeBlur;
uniform int blurAtivo;
varying vec4 v_texCoord;

void main()
{
    vec2 textura_offset = 1.0 / vec2(textureSize(texture_0, 0));
    vec4 result = texture2D(texture_0, v_texCoord.xy);

    float tamanho_janela = 81.0;
    float raio = 0.3;

    vec2 mousePos = vec2(mouseX / 600.0, mouseY / 600.0);

    float distancia = distance(v_texCoord.xy, mousePos);

    if (blurAtivo == 1 && distancia > raio) {
        vec4 sum = vec4(0.0);
        for (int x = -4; x <= 4; x++){
            for (int y = -4; y <= 4; y++) {
                sum += texture2D(texture_0, v_texCoord.xy + vec2(float(x), float(y)) * textura_offset * quantidadeBlur);
            }
        }
        result = sum / tamanho_janela;
    }

    gl_FragColor = result;
}
*/
